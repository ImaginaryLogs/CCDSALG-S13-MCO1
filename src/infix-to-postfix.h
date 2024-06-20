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

void parseRestOfNumbers(char *token, char *strInput, int *nthInputChar,int *isCurrCharNull, int *isCurrCharNumber){
  char currChar;
  while (!(*isCurrCharNull) && *isCurrCharNumber) {
      strncat(token, strInput + *(nthInputChar), 1); // safer to use strncat(), and its useful to select certain characters.
      (*nthInputChar)++;

      currChar = strInput[*nthInputChar];
      *isCurrCharNull = currChar == '\0';
      *isCurrCharNumber = currChar >= '0' && currChar <= '9';

      LOG(LPOST, "%s, ", token);
    }
    LOG(LPOST, "[%s]\n", token);
}

void parseRestOfOperation(char *token, char *strInput, int *nthInputChar, char currentChar ){
  if (currentChar == '>' || currentChar == '<' || currentChar == '!') {
    int nIsEquals = 1; // next char is '='
    int nnIsEquals = 1; // next next char is '='
    int nnnIsEquals = 1; // next next next char is '=' 

    if (strInput[*nthInputChar + 1] != '=') {
      nIsEquals = 0;
    }

    if (strInput[*nthInputChar + 2] != '=') {
      nnIsEquals = 0; 
    }

    if (strInput[*nthInputChar + 3] != '=') {
      nnnIsEquals = 0;
    }

    // Let ? be < or >
    if (!nIsEquals) { // ?
      strncat(token, strInput + *(nthInputChar), 1);
      (*nthInputChar)++;
    } else if (!nnIsEquals) { // ?=
      strncat(token, strInput + *(nthInputChar), 2);
      (*nthInputChar) += 2;
    } else if (!nnnIsEquals) { // ? followed by ==
      strncat(token, strInput + *(nthInputChar), 1);
      (*nthInputChar)++;
    } else { // ?= followed by ==
      strncat(token, strInput + *(nthInputChar), 2);
      (*nthInputChar) += 2;
    }
  } else if (currentChar == '=' || currentChar == '&' || currentChar == '|') {
    strncat(token, strInput + *(nthInputChar), 2);
    (*nthInputChar) += 2;
  } else { // 1-char operators
    strncat(token, strInput + *(nthInputChar), 1);
    (*nthInputChar)++;
  }
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

  LOG(LPOST, "\n[INPO] Token Recognition:\n", token);
  // End of string has been reached
  if (isCurrCharNull)
    return 2;

  // Parses the token
  if (isCurrCharNumber) { // parsing a operand
    parseRestOfNumbers(token, Input, nthInputChar, &isCurrCharNull, &isCurrCharNumber);
  } else { // parsing an operator
    parseRestOfOperation(token, Input, nthInputChar, currChar);
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
 * Pop operations that are within open parenthesis, e.g. ( ... ).
 * @param  *errorOperand: int inspecting if there are any errors
 * @param  *OperatorStack: Operator Stack
 * @param  *PostfixQueue: Postfix Queue to enqueue
 * @param  OperationTable[]: Operation Table to refer to
 * @retval None
 */
void popOperationsGroup(int *errorOperand, Stack *OperatorStack, queue *PostfixQueue, struct Operation OperationTable[]){
  String7 topOperationString;

  LOG(LPOST, "PARENTHESIS POPPING:\n", topOperationString);
  stackPrint(OperatorStack);
  stackTop(OperatorStack, topOperationString);
  while (strcmp(topOperationString, "(") != 0 && !isStackEmpty(OperatorStack)) {
    pop(OperatorStack, topOperationString);
    enqueue(PostfixQueue, topOperationString);
    stackTop(OperatorStack, topOperationString);
    stackPrint(OperatorStack);
  }

  if (isStackEmpty(OperatorStack))
    *errorOperand = ER_MISSING_OPERATOR;

  LOG(LPOST, "After Popping:\n", topOperationString);
  stackPrint(OperatorStack);
  pop(OperatorStack, topOperationString); // disregard the open parenthesis
}

/**
 * Pop operations of a stack until  no operations left or the incoming operation string's precedence is greater than what's on the stack
 * @param  *currOperationString: incoming operation
 * @param  *OperatorStack: Operator Stack
 * @param  *PostfixQueue: Postfix Queue to enqueue
 * @param  OperationTable[]: Operation Table to refer to
 * @retval None
 */
void popOperationsNormal(char *currOperationString, struct Operation currOperation, Stack *OperatorStack, queue *PostfixQueue, struct Operation OperationTable[]){
  int isFinishedPopping = 0;
  int topOperationIndex;
  int isLeftToRightAssocitivity, isIncomingOpLowerPrecendence; 
  struct Operation topOperation;
  String7 topOperationString;
  stackTop(OperatorStack, topOperationString);

  while (!isStackEmpty(OperatorStack) && !isFinishedPopping) {
    stackPrint(OperatorStack);
    stackTop(OperatorStack, topOperationString);
    
    topOperationIndex = searchOperatorTable(OperationTable, topOperationString);
    topOperation = OperationTable[topOperationIndex];

    if (strcmp(topOperationString, "(") == 0)
      topOperation.nPrecedence = -1; // This is the bottom cap, so that any operations after '(' has higher priority

    LOG(LPOST, "PRECEDENCE: %s [%d] vs %s [%d]\n", topOperation.stringIdentifier, topOperation.nPrecedence, currOperation.stringIdentifier, currOperation.nPrecedence);
    
    isLeftToRightAssocitivity = strcmp(topOperationString, "^") != 0;
    isIncomingOpLowerPrecendence = topOperation.nPrecedence >= currOperation.nPrecedence;

    if (isLeftToRightAssocitivity && isIncomingOpLowerPrecendence) {
      enqueue(PostfixQueue, pop(OperatorStack, topOperationString));
    } else if (!isLeftToRightAssocitivity && !isIncomingOpLowerPrecendence) {
      enqueue(PostfixQueue, pop(OperatorStack, topOperationString));
    } else
      isFinishedPopping = 1;
  }

  push(OperatorStack, currOperationString);
}

/**
 * Translates infix notation into postfix notation, and stores it in a queue.
 * If error arrises, returns one; else, zero.
 * 
 * @retval 0 SUCCESSFUL_EXIT
 * @retval 1 ER_SYNTAX
 */
int infixToPostfix(char *infixString, queue* PostfixQueue, struct Operation OperationTable[]){
  Stack* OperatorStack = createStack();
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
  struct Operation currOperation;

  while (isConvertingInfix) {
    stackTopInspect(OperatorStack);
    nextParseState = parseStringInput(infixString, &nthInfixChar, &currNumber, currOperationString);

    switch(nextParseState) {
      case TOKEN_NUMBER:
        LOG(LPOST, "[INPO] TYPE: Number [%d]\n", currNumber);
        sprintf(currNumberString, "%d", currNumber);
        enqueue(PostfixQueue, currNumberString);
        break;
      case TOKEN_OPERATION:
        LOG(LPOST, "[INPO] TYPE: Operator [%s]\n", currOperationString);
        
        currOperationIndex = searchOperatorTable(OperationTable, currOperationString);
        currOperation = OperationTable[currOperationIndex];

        if (strcmp(currOperationString, "(") == 0) { // just enqueue the open parenthesis
          push(OperatorStack, currOperationString);
        } else if (strcmp(currOperationString, ")") == 0) { // pop until first open parenthesis
          popOperationsGroup(&errorOperand, OperatorStack, PostfixQueue, OperationTable);
        } else { 
          popOperationsNormal(currOperationString, currOperation, OperatorStack, PostfixQueue, OperationTable);
        }
        break;
    }

    // Check the conditions
    hasNoErrors = errorOperand == SUCCESSFUL_EXIT;
    hasCharsToProcess = nthInfixChar < (int) strlen(infixString);
    isConvertingInfix = hasNoErrors && hasCharsToProcess;
  }

  // pop all current operators in stack and push them to queue
  while (!isStackEmpty(OperatorStack)) {
    enqueue(PostfixQueue, pop(OperatorStack, currOperationString));
  }

  return errorOperand;
}

#endif