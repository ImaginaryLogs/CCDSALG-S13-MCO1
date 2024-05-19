#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NUM_OPERATIONS (15)

enum operationErrorCodes {
  Er_SUCCESS,
  Er_MISSING_OPERANDS,
  Er_DIVIDE_BY_ZERO
};

/**
 * @retval 0 if number
 * @retval 1 if operation
 * @retval 2 if error
 */
int parseInput(char *Input, int *nthPostfixChar, int *OutputNumber, char *OutputOperation) {
  String255 token = "";
  int i;
  // Grab the token
  while (Input[*nthPostfixChar] != ' ' && Input[*nthPostfixChar] != '\0') {
    strncat(token, Input + *(nthPostfixChar), 1);
    (*nthPostfixChar)++;
    printf("Processing: %s\n",token);
  }
  printf("Token Parsed: %s\n", token);
  // if there's more tokens, increase count
  if (Input[*nthPostfixChar] == ' ')
    (*nthPostfixChar)++;

  *OutputNumber = atoi(token);
  if (*OutputNumber > 0)
    return 0 ;
  // if not a number by atoi, check if its char '0';
  else if (strlen(token) == 1 && token[0] == '0') {
    *OutputNumber = 0;
    return 0;
  }
  // else, token is operation
  strcpy(OutputOperation, token);
  return 1;
}

int opAddition(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return Er_MISSING_OPERANDS;

  printf("%d %d %d\n", queueOperands[*nthToken - 1], queueOperands[*nthToken - 2], queueOperands[*nthToken - 3]);
  
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] + queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;
  
  printf("Answer %d\n", queueOperands[*nthToken - 2]);
  printf("%d %d %d\n", queueOperands[*nthToken - 1], queueOperands[*nthToken - 2], queueOperands[*nthToken - 3]);
  (*nthToken) -= 1;

  return Er_SUCCESS;
}

int opSubtraction(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return Er_MISSING_OPERANDS;
    
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] - queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;

  (*nthToken) -= 1;
  

  return Er_SUCCESS;
}

int opMultiply(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return Er_MISSING_OPERANDS;
    
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] * queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;
  (*nthToken) -= 1;
  

  return Er_SUCCESS;
}

int opDivide(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return Er_MISSING_OPERANDS;

  if (queueOperands[*nthToken - 2] == 0)
    return Er_DIVIDE_BY_ZERO;
    
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] / queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;
  (*nthToken) -= 1;
  
  
  return Er_SUCCESS;
}


/**
 * Evaluates the operation in the stringOperation given the tokens in the stack.
 * @retval 0 Success
 * @retval 1 Missing operands
 * @retval 2 Division Error
 */
int evaluateOperation(int (*operationFunctions[])(int [], int *), String7 operationSignifiers[], int *queueOperands, int *nthToken, char *stringOperation) {
  int isFinding = true;
  int ithOperation = 0;
  int nError  = 0;
  int nResult = 0;
  

  // Iterate through each operation
  while (ithOperation < NUM_OPERATIONS && isFinding == true){
    // Check if it matches
    if (strcmp(stringOperation, operationSignifiers[ithOperation]) == 0){
      printf("Operation: %s with %d in stack (%d %d) \n", stringOperation, *nthToken, queueOperands[*nthToken - 1], queueOperands[*nthToken - 2]);
      isFinding = false;
      // Stores if the operation is valid.
      nError = operationFunctions[ithOperation](queueOperands, nthToken);
    }
    ithOperation++;
  }

  return nError;
}

/**
 * Evaluates postfix notation into an output, should detect errors when it
 * arises. Interacts with the queue storing postfix notation.
 *
 * @retval 0 success
 * @retval 1 operands are not enough
 * @retval 2 division error
 *
 */
int evaluatePostfix(char *queuePostfixInput, char *stringAnswer) {
  String7 operationSignifiers[NUM_OPERATIONS] = {"+",  "-",  "*", "/"}; // ,  "%",  "^", ">",  "<", ">=", "<=", "!=", "==", "!", "&&", "||"
  int (*operationFunctions[NUM_OPERATIONS])(int [], int *) = {
    &opAddition, &opSubtraction, &opMultiply, &opDivide
  };

  String7 stringOperation;
  int queueOperands[16] = {};
  int nthToken = 0;
  int nthPostfixChar = 0;
  int parseState = 0;
  int errorOperand = Er_SUCCESS;

  while (nthPostfixChar < strlen(queuePostfixInput) && errorOperand == Er_SUCCESS) {
    parseState = parseInput(queuePostfixInput, &nthPostfixChar,
                            queueOperands + nthToken, stringOperation);
    printf("Stack: %d %d %d\n", queueOperands[0], queueOperands[1], queueOperands[2]);
    switch (parseState) {
    case 0:
      printf("Number Token: %d\n", queueOperands[nthToken]);
      nthToken++;
      break;
    case 1:
      printf("Opernd Token: %s\n", stringOperation);
      errorOperand = evaluateOperation(operationFunctions, operationSignifiers, queueOperands, &nthToken, stringOperation);
      break;
    }
  }
  printf("Answer: %d %d %d\n", queueOperands[0], queueOperands[1], queueOperands[2]);
  return errorOperand;
}
