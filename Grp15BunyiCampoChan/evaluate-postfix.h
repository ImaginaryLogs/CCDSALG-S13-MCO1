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
  return (int) strlen(token) > 0 && token[0] >= '0' && token[0] <= '9';
}

/**
 * Consume Operations
 * @param  OperationTable[]: 
 * @param  *stackOperands: 
 * @param  *token: 
 * @param  *errorOperand: 
 * @retval 
 */
int consumeOperator(struct Operation OperationTable[], Stack *stackOperands, char *token, int *errorOperand){
  String31 buffer;
  int leftOperand, rightOperand, result;
  LOG(LPOST, "Check");
  // ### Check if stack is not empty for operator use
  if (isStackEmpty(stackOperands)){
    *errorOperand = ER_MISSING_OPERANDS;
  } else if (strcmp(token, "!") == 0) { 
  // ### Unary Operation(s) -- logical NOT only 
    rightOperand = atoi(pop(stackOperands, buffer));
    result = !rightOperand;
  } else { 
  // ### Binary Operation(s)
    rightOperand = atoi(pop(stackOperands, buffer));
    //stackPrint(stackOperands);

    // ### Check if another can be used
    if(isStackEmpty(stackOperands)){
      *errorOperand = ER_MISSING_OPERANDS;
    } else
       leftOperand  = atoi(pop(stackOperands, buffer));

    if (*errorOperand == SUCCESSFUL_EXIT)
        *errorOperand = evaluateBinaryOperations(OperationTable, token, &result, &leftOperand, &rightOperand);
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
 */
int evaluatePostfix(queue* queuePostfix, char *stringAnswer, struct Operation OperationTable[], int erStateInfixToPstfx) {
  Stack* stackOperands = createStack();
  String31 token = "", buffer = "";
  int result, hasOperations = 1;
  
  // The state of the previous stage affects this stage. So, if previous is succesful, then is this.
  int errorOperand = erStateInfixToPstfx;
  LOG(LPOST, "[EVAL] Start\n");
  queuePrint(queuePostfix);
  while (errorOperand == SUCCESSFUL_EXIT && hasOperations) {
    dequeue(queuePostfix, token);
    
    if (isTokenAnOperand(token)) { 
      LOG(LPOST, "[EVAL] TYPE: Number \"%s\"\n", token);

      push(stackOperands, token);
      LOG(LPOST, "Check 2\n");
    } else { 
      LOG(LPOST, "[EVAL] TYPE: Operation \"%s\"\n", token);
      stackPrint(stackOperands);
      LOG(LPOST, "Check 2\n");
      result = consumeOperator(OperationTable, stackOperands, token, &errorOperand);
      sprintf(buffer, "%d", result);
      push(stackOperands, buffer);
    }
    LOG(LPOST, "Check 3\n");
    hasOperations = !queueEmpty(queuePostfix);
    LOG(LPOST, "Check 3.5\n");
  }

  strcpy(stringAnswer, pop(stackOperands, buffer));

  if (!isStackEmpty(stackOperands)){
    errorOperand = ER_MISSING_OPERATOR;
  }
  
  stackDelete(stackOperands);
  return errorOperand;
}

#endif