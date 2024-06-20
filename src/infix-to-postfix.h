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
 * Pop operations that are within open parenthesis, e.g. ( ... ).
 * @param  *errorOperand: int inspecting if there are any errors
 * @param  *OperatorStack: Operator Stack
 * @param  *PostfixQueue: Postfix Queue to enqueue
 * @param  OperationTable[]: Operation Table to refer to
 * @retval None
 */
void popOperationsGroup(int *errorOperand, Stack *OperatorStack, queue *PostfixQueue, struct Operation OperationTable[]){
  String7 topOperationString;

  LOG(LPOST, "PARENTHESIS POPPING: %s\n", topOperationString);
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

  LOG(LPOST, "After Popping: %s\n", topOperationString);
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
  int isLeftToRightAssocitivity, isIncomingOpLowerPrecendence, isIncomingOpSameExponent; 
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
    isIncomingOpSameExponent = strcmp(topOperation.stringIdentifier, "^") == 0 && strcmp(currOperation.stringIdentifier, "^") == 0;
    
    if (isLeftToRightAssocitivity && isIncomingOpLowerPrecendence) {
      enqueue(PostfixQueue, pop(OperatorStack, topOperationString));
    } else if (!isLeftToRightAssocitivity && isIncomingOpLowerPrecendence && !isIncomingOpSameExponent) {
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
  
  int hasAtLeastOneOperation    = false;
  int hasPassedOneInstance      = false;
  int hasNoPreviousGreaterError = false;
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
        hasAtLeastOneOperation = true;

        currOperationIndex = searchOperatorTable(OperationTable, currOperationString);
        
        if (currOperationIndex == -1) {
            errorOperand = ER_UNDEFINED_OPERATION;
        } else {
          currOperation = OperationTable[currOperationIndex];
        
          if (strcmp(currOperationString, "(") == 0) { // just enqueue the open parenthesis
            push(OperatorStack, currOperationString);
          } else if (strcmp(currOperationString, ")") == 0) { // pop until first open parenthesis
            popOperationsGroup(&errorOperand, OperatorStack, PostfixQueue, OperationTable);
          } else { 
            popOperationsNormal(currOperationString, currOperation, OperatorStack, PostfixQueue, OperationTable);
          }
        }
        break;
      case END_OF_STRING:
        if (hasPassedOneInstance == false) {
          errorOperand = ER_NO_STRING_TO_PARSE;
        }
    }

    // Check the conditions
    hasCharsToProcess = nthInfixChar < (int) strlen(infixString);
    hasNoPreviousGreaterError = errorOperand != ER_NO_STRING_TO_PARSE && errorOperand != ER_UNDEFINED_OPERATION;

    if (!hasCharsToProcess && !hasAtLeastOneOperation && hasNoPreviousGreaterError)
      errorOperand = ER_MISSING_OPERATOR;

    hasNoErrors = errorOperand == SUCCESSFUL_EXIT;
    isConvertingInfix = hasNoErrors && hasCharsToProcess;
    hasPassedOneInstance = hasPassedOneInstance || true;
  }

  // pop all current operators in stack and push them to queue
  while (errorOperand == SUCCESSFUL_EXIT && !isStackEmpty(OperatorStack)) {
    enqueue(PostfixQueue, pop(OperatorStack, currOperationString));
  }

  // Just to be safe, dequeue the operator
  if (errorOperand == ER_MISSING_OPERATOR)
    dequeue(PostfixQueue, currNumberString);

  stackDelete(OperatorStack);

  return errorOperand;
}

#endif