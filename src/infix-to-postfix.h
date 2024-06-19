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
    while (!isCurrCharNull && isCurrCharNumber) {
      strncat(token, Input + *(nthInputChar), 1); // safer to use strncat(), and its useful to select certain characters.
      (*nthInputChar)++;

      currChar = Input[*nthInputChar];
      isCurrCharNull = currChar != '\0';
      isCurrCharNumber = currChar >= '0' && currChar <= '9';

      LOG(LPOST, "\t%s\n", token);
    }
    LOG(LPOST, "TOKEN := %s\n", token);
  }
  else { // parsing an operator
    if (currChar == '>' || currChar == '<' || currChar == '!') {
      int nIsEquals = 0; // next char is '='
      int nnIsEquals = 0; // next next char is '='
      int nnnIsEquals = 0; // next next next char is '='

      if (Input[*nthInputChar + 1] == '=') {
        nIsEquals = 1;
        if (Input[*nthInputChar + 2] == '=') {
          nnIsEquals = 1;
          if (Input[*nthInputChar + 3] == '=') {
            nnnIsEquals = 1;
          }
        }
      }

      // Let ? be < or >
      if (!nIsEquals) { // ?
        strncat(token, Input + *(nthInputChar), 1);
        (*nthInputChar)++;
      }
      else if (!nnIsEquals) { // ?=
        strncat(token, Input + *(nthInputChar), 2);
        (*nthInputChar) += 2;
      }
      else if (!nnnIsEquals) { // ? followed by ==
        strncat(token, Input + *(nthInputChar), 1);
        (*nthInputChar)++;
      }
      else { // ?= followed by ==
        strncat(token, Input + *(nthInputChar), 2);
        (*nthInputChar) += 2;
      }
    }
    else if (currChar == '=' || currChar == '&' || currChar == '|') {
      strncat(token, Input + *(nthInputChar), 2);
      (*nthInputChar) += 2;
    }
    else { // 1-char operators
      strncat(token, Input + *(nthInputChar), 1);
      (*nthInputChar)++;
    }
  }

  *nOutputNumber = atoi(token);
  if (*nOutputNumber > 0)
    return 0;
  
  // if not a number by atoi, check if it's char '0';
  else if (strlen(token) == 1 && token[0] == '0') {
    *nOutputNumber = 0;
    return 0;
  }
  // else, token is operation
  strcpy(nOutputOperation, token);
  return 1;
}

/**
 * Translates infix notation into postfix notation, and stores it in a queue.
 * If error arrises, returns one; else, zero.
 * 
 * @retval 0 SUCCESSFUL_EXIT
 * @retval 1 ER_SYNTAX
 */
int infixToPostfix(String255 infixString, queue* PostfixQueue) {
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

      nextParseState = parseStringInput(infixString, &nthInfixChar, &currNumber, currOperationString);

      switch(nextParseState) {
        case 0:
          LOG(LPOST, "(infix postfix) TYPE: Number (%d)\n\n", currNumber);

          sprintf(currNumberString, "%d", currNumber);
          enqueue(PostfixQueue, currNumberString);
          break;
        case 1:
          LOG(LPOST, "(infix postfix) TYPE: Operator (%s)\n\n", currOperationString);
          
          currOperationIndex = searchOperatorTable(OperationTable, currOperationString);
          currOperation = OperationTable[currOperationIndex];

          if (strcmp(currOperationString, "(") == 0) { // just enqueue the open parenthesis
            enqueue(PostfixQueue, currOperationString);
          }
          else if (strcmp(currOperationString, ")") == 0) { // pop until first open parenthesis
            while (strcmp(stackTop(OperatorStack), "(") != 0) {
              pop(OperatorStack, topOperationString);
              enqueue(PostfixQueue, topOperationString);
            }
            pop(OperatorStack, topOperationString); // disregard the open parenthesis
          }
          else { // pop until emptied stack or first operator with lower precedence
            int isFinishedPopping = 0;

            while (!isStackEmpty(OperatorStack) && !isFinishedPopping) {
              strcpy(topOperationString, stackTop(OperatorStack));
              topOperationIndex = searchOperatorTable(OperationTable, topOperationString);
              topOperation = OperationTable[topOperationIndex];

              if (topOperation.nPrecedence >= currOperation.nPrecedence)
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

    return 0;
}

#endif