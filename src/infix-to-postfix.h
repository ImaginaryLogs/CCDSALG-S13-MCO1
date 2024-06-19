/**
 * References:
 * [1] strncat(): https://www.geeksforgeeks.org/strncat-function-in-c-cpp/ 
 */
#include "utils.h"
#include "stack.h"
#include "queue.h"

#ifndef _INFIX_POSTFIX_H_
#define _INFIX_POSTFIX_H_

#define LPOST ENABLE_LOG_EVALUATE_POSTFIX

void parseRestOfTokens(char *token, char *strInput, int *nthInputChar,int *isCurrCharNull, int *isCurrCharNumber){
  char currChar;
  while (!(*isCurrCharNull) && *isCurrCharNumber) {
      strncat(token, strInput + *(nthInputChar), 1); // safer to use strncat(), and its useful to select certain characters.
      (*nthInputChar)++;

      currChar = strInput[*nthInputChar];
      *isCurrCharNull = currChar == '\0';
      *isCurrCharNumber = currChar >= '0' && currChar <= '9';

      LOG(LPOST, "\t%s\n", token);
    }
    LOG(LPOST, "TOKEN := %s\n", token);
}

/**
 * Parses string input to a queue.
 * @note stack array temporarily 
 * @param Input String Input
 * @param nthInputChar The character position being read
 * @param Output
 * @retval 0 if returns a number
 * @retval 1 if returns a string
 * @retval 2 if end of string
 */
int parseStringInput(char *Input, int *nthInputChar, int *nOutputNumber, char *nOutputOperation) {
  String63 token = "";
  char currChar = Input[*nthInputChar];
  int isCurrCharNull = currChar == '\0';
  int isCurrCharNumber = currChar >= '0' && currChar <= '9';

  LOG(LPOST, "Processing TOKEN:\n");
  // End of string has been reached
  if (isCurrCharNull)
    return 2;

  // Parses the token
  if (isCurrCharNumber) { // parsing a operand
    sleep(1);
    parseRestOfTokens(token, Input, nthInputChar, &isCurrCharNull, &isCurrCharNumber);
  } else { // parsing an operator
    if (currChar == '>' || currChar == '<' || currChar == '!') {
      int nIsEquals = 1; // next char is '='
      int nnIsEquals = 1; // next next char is '='
      int nnnIsEquals = 1; // next next next char is '=' 

      if (Input[*nthInputChar + 1] != '=') {
        nIsEquals = 0;
      }

      if (Input[*nthInputChar + 2] != '=') {
        nnIsEquals = 0; 
      }

      if (Input[*nthInputChar + 3] != '=') {
        nnnIsEquals = 0;
      }

      // Let ? be < or >
      if (!nIsEquals) { // ?
        strncat(token, Input + *(nthInputChar), 1);
        (*nthInputChar)++;
      } else if (!nnIsEquals) { // ?=
        strncat(token, Input + *(nthInputChar), 2);
        (*nthInputChar) += 2;
      } else if (!nnnIsEquals) { // ? followed by ==
        strncat(token, Input + *(nthInputChar), 1);
        (*nthInputChar)++;
      } else { // ?= followed by ==
        strncat(token, Input + *(nthInputChar), 2);
        (*nthInputChar) += 2;
      }
    } else if (currChar == '=' || currChar == '&' || currChar == '|') {
      strncat(token, Input + *(nthInputChar), 2);
      (*nthInputChar) += 2;
    } else { // 1-char operators
      strncat(token, Input + *(nthInputChar), 1);
      (*nthInputChar)++;
    }
  }

  *nOutputNumber = atoi(token);
  if (*nOutputNumber > 0)
    return TOKEN_NUMBER;
  
  // if not a number by atoi, check if it's char '0';
  else if (strlen(token) == 1 && token[0] == '0') {
    *nOutputNumber = 0;
    return TOKEN_NUMBER;
  }
  // else, token is operation
  strcpy(nOutputOperation, token);
  return TOKEN_OPERATION;
}

/**
 * Translates infix notation into postfix notation, and stores it in a queue.
 * If error arrises, returns one; else, zero.
 * 
 * @retval 0 SUCCESSFUL_EXIT
 * @retval 1 ER_SYNTAX
 */
int infixToPostfix(char *infixString, queue* PostfixQueue) {

    Stack* OperatorStack = createStack();

    struct Operation OperationTable[MAX_NUM_OPERATIONS];
    initOperatorTable(OperationTable);

    String7 stringOperation;
    int nthInfixChar    = 0;
    int nextParseState  = 0;
    int errorOperand    = SUCCESSFUL_EXIT;

    int isConvertingInfix = true;
    int hasNoErrors       = true;
    int hasCharsToProcess = true;

    int currNumber;
    String7 currNumberString;

    String7 currOperationString, topOperationString;
    int currOperationIndex, topOperationIndex;
    struct Operation currOperation, topOperation;

    while (isConvertingInfix) {
      stackTopInspect(OperatorStack);

      nextParseState = parseStringInput(infixString, &nthInfixChar, &currNumber, currOperationString);

      switch(nextParseState) {
        case TOKEN_NUMBER:
          LOG(LPOST, "(infix postfix) TYPE: Number [%d]\n\n", currNumber);

          sprintf(currNumberString, "%d", currNumber);
          enqueue(PostfixQueue, currNumberString);
          break;
        case TOKEN_OPERATION:
          LOG(LPOST, "(infix postfix) TYPE: Operator [%s]\n\n", currOperationString);
          
          currOperationIndex = searchOperatorTable(OperationTable, currOperationString);
          currOperation = OperationTable[currOperationIndex];

          if (strcmp(currOperationString, "(") == 0) { // just enqueue the open parenthesis
            push(OperatorStack, currOperationString);
            //enqueue(PostfixQueue, currOperationString);
          } else if (strcmp(currOperationString, ")") == 0) { // pop until first open parenthesis
            stackPrint(OperatorStack);
            stackTop(OperatorStack, topOperationString);
            LOG(LPOST, "Before Loop: %s\n", topOperationString);
            while (strcmp(topOperationString, "(") != 0 && !isStackEmpty(OperatorStack)) {
              sleep(1);
              pop(OperatorStack, topOperationString);
              LOG(LPOST, "Pop & Queue: %s\n", topOperationString);
              enqueue(PostfixQueue, topOperationString);
              stackTop(OperatorStack, topOperationString);
              LOG(LPOST, "Next: %s\n", topOperationString);
            }

            if (isStackEmpty(OperatorStack))
              errorOperand = ER_MISSING_OPERATOR;

            LOG(LPOST, "After Loop: %s\n", topOperationString);
            pop(OperatorStack, topOperationString); // disregard the open parenthesis
          }
          else { 
            int isFinishedPopping = 0;
            stackTop(OperatorStack, topOperationString);

            while (!isStackEmpty(OperatorStack) && !isFinishedPopping) {
              stackPrint(OperatorStack);
              stackTop(OperatorStack, topOperationString);
              
              topOperationIndex = searchOperatorTable(OperationTable, topOperationString);
              topOperation = OperationTable[topOperationIndex];
              if (strcmp(topOperationString, "(") == 0)
                topOperation.nPrecedence = 15; // This is the cap, so, any operations after '(' has higher priority
              LOG(LPOST, "PRECEDENCE: %s [%d] vs %s [%d]\n", topOperation.stringIdentifier, topOperation.nPrecedence, currOperation.stringIdentifier, currOperation.nPrecedence);
              if (topOperation.nPrecedence <= currOperation.nPrecedence)
                enqueue(PostfixQueue, pop(OperatorStack, topOperationString));
              else
                isFinishedPopping = 1;
            }
            

            push(OperatorStack, currOperationString);
          }
          
          break;
      }

      hasNoErrors = errorOperand == SUCCESSFUL_EXIT;
      hasCharsToProcess = nthInfixChar < strlen(infixString);
      isConvertingInfix = hasNoErrors && hasCharsToProcess;
    }

    // pop all current operators in stack and push them to queue
    while (!isStackEmpty(OperatorStack)) {
      enqueue(PostfixQueue, pop(OperatorStack, currOperationString));
    }

    return errorOperand;
}

#endif