#include "utils.h"
#include "stack.h"
#include <math.h>


#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

#pragma region OPERATIONS_DEFINED_TYPES

#define MAX_NUM_OPERATIONS (17)
#define LOPER ENABLE_LOG_OPERATIONS

/**
 * Struct that bundles essential identification for operations.
 */
struct Operation {
  String7 stringIdentifier;
  int nPrecedence;
  int nNumOperandsRequired;
  int nOrientation;
  int nOperandId;
};

#pragma endregion

/**
 * Initializes the operation table
 * NOTE: Order of Precedence for operations:
 * 0 : ()
 * 1 : !
 * 2 : ^
 * 3 : * / %
 * 4 : + -
 * 5 : > >= < <=
 * 6 : == !=
 * 7 : &&
 * 8 : ||
 * 
 * @param operationTable[]: struct containing an empty operation Table
 */
void initOperatorTable(struct Operation operationTable[]){
    /** 
     
     */
    LOG(LOPER, "Operation table initialized.\n");

    // Operation Identification, Precedence, Required Number of Operands
    String2 operationList[MAX_NUM_OPERATIONS][3] = {
      {"(" , "8", "0"},
      {")" , "8", "0"},
      {"!" , "7", "1"},
      {"^" , "6", "2"},
      {"*" , "5", "2"},
      {"/" , "5", "2"},
      {"%%", "5", "2"},
      {"+" , "4", "2"},
      {"-" , "4", "2"},
      {">" , "3", "2"},
      {"<" , "3", "2"},
      {">=", "3", "2"},
      {"<=", "3", "2"},
      {"!=", "2", "2"},
      {"==", "2", "2"},
      {"&&", "1", "2"},
      {"||", "0", "2"},
    };
    // iterate each list
    int i;
    for (i = 0; i < MAX_NUM_OPERATIONS; i++) {
      operationTable[i].nNumOperandsRequired = atoi(operationList[i][2]);
      operationTable[i].nPrecedence = atoi(operationList[i][1]);
      strcpy(operationTable[i].stringIdentifier, operationList[i][0]);
      operationTable[i].nOperandId = i;
    }

}

/**
 * Searches the Operator Table based on the String
 * @param  *stringOperation: 
 * @retval n  : id number of the operand in question. Note that n >= 0.
 * @retval -1 : no operands found
 */
int searchOperatorTable(struct Operation operationTable[], char *stringOperation){
  int searchResult = -1;
  int i;
  
  for (i = 0; i < MAX_NUM_OPERATIONS; i++) {
    //LOG(LOPER, "\t[%02d]: %s vs %s ?\n", i, operationTable[i].stringIdentifier, stringOperation);
    if (strcmp(operationTable[i].stringIdentifier, stringOperation) == 0) {
      searchResult = operationTable[i].nOperandId;
      //LOG(LOPER, "SEARCH: \"%s\" -> RESULT: %d\n", stringOperation, searchResult);
      return searchResult;
    }
  }
  LOG(LOPER, "SEARCH: \"%s\" -> RESULT: %d\n", stringOperation, searchResult);
  return searchResult;
}

int power(int a, int b){
  int result = 1;
  while (b > 0){
    result *= a;
    --b;
  }
  return result;
}

/**
 * Evaluates binary operations based on Operation ID.
 * @param  operationTable[]: List of Operations
 * @param  *stringOperation: String of an Operands to process
 * @param  *oprResult: Where the result will be stored
 * @param  *oprLeft: Left operand
 * @param  *oprRight: Right operand
 * @return State of evaluating the operation
 * @retval SUCCESSFUL_EXIT Operation was found
 * @retval ER_UNDEFINED_OPERATION No operation found
 */
int evaluateBinaryOperations(struct Operation operationTable[], char *stringOperation, int *oprResult, int *oprLeft, int *oprRight){
  int isLeftToRight = true;
  switch (searchOperatorTable(operationTable, stringOperation)){
    case 3:
      *oprResult = power(*oprRight, *oprLeft);
      isLeftToRight = false;
      break;

    case 4:
      *oprResult = *oprLeft * *oprRight;
      break;

    case 5:
      if (*oprRight == 0)
        return ER_DIVIDE_BY_ZERO;
      *oprResult = *oprLeft / *oprRight;
      break;

    case 6:
      if (*oprRight == 0)
        return ER_DIVIDE_BY_ZERO;
      *oprResult = *oprLeft % *oprRight;
      break;

    case 7:
      *oprResult = *oprLeft + *oprRight;
      break;

    case 8:
      *oprResult = *oprLeft - *oprRight;
      break;

    case 9:
      *oprResult = *oprLeft > *oprRight;
      break;

    case 10:
      *oprResult = *oprLeft < *oprRight;
      break;

    case 11:
      *oprResult = *oprLeft >= *oprRight;
      break;

    case 12:
      *oprResult = *oprLeft <= *oprRight;
      break;

    case 13:
      *oprResult = *oprLeft != *oprRight;
      break;

    case 14:
      *oprResult = *oprLeft == *oprRight;
      break;

    case 15:
      *oprResult = *oprLeft && *oprRight;
      break;

    case 16:
      *oprResult = *oprLeft || *oprRight;
      break;
      
    default:
    case -1:
      return ER_UNDEFINED_OPERATION;
  }

  LOG(LOPER, "RESULT: %d %s %d = ", isLeftToRight ? *oprLeft : *oprRight, stringOperation, isLeftToRight ? *oprRight : *oprLeft);
  LOG(LOPER, "%d\n", *oprResult);
  return SUCCESSFUL_EXIT;
}

/**
 * Checks if it has an operand to process.
 * @param *queueSize: size of the stack
 * @return does stack have the one operand
 */
int hasLessThatOneOperand(Stack* S){
  return isStackEmpty(S);
}

/**
 * Checks if it has two operands to process.
 * @param *queueSize: size of the queue
 * @return does queue have the two operand
 */
int hasLessThanTwoOperands(Stack* S){
  return S->top == NULL || S->top->prevNode == NULL;
}

/**
 * Evaluates the operation in the stringOperation given the tokens in the stack.
 * @note Version 2.0 - uses switch statements
 * @return ErrorCodes based on what happened
 */
int performOperation(struct Operation operationTable[], Stack* stackOperands, char *stringOperation) {
  String7 operandString;
  int operandRight = atoi(pop(stackOperands, operandString));
  int operandLeft;
  int resultTwoOperands; 
  int nErrorCode = ER_MISSING_OPERATOR;
  String7 buffer;

  LOG(LOPER, "### [PERFORM OPERATIONS] {\n");

  // ### Unary Operation (R = # LEFT)
  if (hasLessThatOneOperand(stackOperands))
    return ER_MISSING_OPERANDS;

  if (strcmp(stringOperation, "!") == 0) {
    LOG(LOPER, "!%d", operandRight);
    operandRight = !operandRight;
    sprintf(buffer, "%d", operandRight);
    push(stackOperands, buffer);

    LOG(LOPER, "%d [SUCCESS]\n", operandRight);
    LOG(LOPER, "### [END OPERATIONS] }\n");

    return SUCCESSFUL_EXIT;
  }
  
  // ### Binary Operations (R = LEFT # RIGHT)
  String31 Temp = "";
  LOG(LOPER, "(eval post) STAK for Left Operand: %s\n", stackTop(stackOperands, Temp));
  operandLeft = atoi(pop(stackOperands, operandString));

  if (hasLessThanTwoOperands(stackOperands))
    return ER_MISSING_OPERANDS;

  nErrorCode = evaluateBinaryOperations(operationTable, stringOperation, &resultTwoOperands, &operandLeft, &operandRight);

  if (nErrorCode != SUCCESSFUL_EXIT)
    return nErrorCode;
  
  sprintf(buffer, "%d", resultTwoOperands);
  push(stackOperands, buffer);
  LOG(LOPER, "%d [SUCCESS]\n", resultTwoOperands);

  // Maintenance, Remove used Operands
  nErrorCode = SUCCESSFUL_EXIT;

  LOG(LOPER, "### [END OPERATIONS] }\n");

  return nErrorCode;
}


#endif