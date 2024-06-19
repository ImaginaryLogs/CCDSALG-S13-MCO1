#include "utils.h"
#include "operations.h"
#include "infix-to-postfix.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
/**
 / 
*/

// Include Guards to help prevent double inclusion collisions when the compiler's preprocessor links the files.
// References: utils.h [3]
#ifndef _EVALUATE_POSTFIX_H_
#define _EVALUATE_POSTFIX_H_

#define LPOST ENABLE_LOG_EVALUATE_POSTFIX 

/**
 * Checks if a string token is a number. 
 * @param  token[]: Corresponding Token
 * @retval 1 if a number
 * @retval 0 if not a number
 */
int isTokenAnOperand(char *token){
  return token[0] >= '0' && token[0] <= '9';
}

int consumeOperator(struct Operation OperationTable[], Stack *stackOperands, char *token, int *errorOperand){
  int leftOperand, rightOperand, result;
  String31 buffer;
  if (strcmp(token, "!") == 0) { // logical NOT -- only unary operation
    rightOperand = atoi(pop(stackOperands, buffer));
    result = !rightOperand;
  } else { // binary operation
    rightOperand = atoi(pop(stackOperands, buffer));
    leftOperand  = atoi(pop(stackOperands, buffer));
    *errorOperand = evaluateBinaryOperations(OperationTable, token, &result, &leftOperand, &rightOperand);

    LOG(LPOST, "\nOperation Result: %d %s %d = %d\n", leftOperand, token, rightOperand, result);      
  }

  return result;
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
int evaluatePostfix(queue* queuePostfix, char *stringAnswer) {
  struct Operation OperationTable[MAX_NUM_OPERATIONS];
  initOperatorTable(OperationTable);

  Stack* stackOperands = createStack();
  String31 token;
  int leftOperand, rightOperand;
  String31 buffer;
  int result;

  int errorOperand = SUCCESSFUL_EXIT;

  while (!queueEmpty(queuePostfix) && errorOperand == SUCCESSFUL_EXIT) {
    dequeue(queuePostfix, token);
    
    if (isTokenAnOperand(token)) { 
      LOG(LPOST, "[EVAL POST] TYPE: Number \"%s\"\n", token);
      push(stackOperands, token);
    } else { 
      LOG(LPOST, "[EVAL POST] TYPE: Operation \"%s\"\n", token);
      result = consumeOperator(OperationTable, stackOperands, token, &errorOperand);
      sprintf(buffer, "%d", result);
      push(stackOperands, buffer);
    }  
  }

  strcpy(stringAnswer, pop(stackOperands, buffer));

  if (!isStackEmpty(stackOperands))
    return ER_MISSING_OPERATOR;

  return errorOperand;
}

#endif