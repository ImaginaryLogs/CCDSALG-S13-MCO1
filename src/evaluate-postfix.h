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
  String31 elem;
  int leftOperand, rightOperand;
  String31 buffer;
  int result;

  int errorOperand = SUCCESSFUL_EXIT;

  while (!queueEmpty(queuePostfix)) {
    strcpy(elem, dequeue(queuePostfix));
    
    if (elem[0] >= '0' && elem[0] <= '9') { // elem is an operand
      LOG(LPOST, "(eval post) TYPE: Number (%s)\n\n", elem);
      push(stackOperands, elem);
    }
    else { // elem is an operation
      LOG(LPOST, "(eval post) TYPE: Operation (%s)\n\n", elem);
      if (strcmp(elem, "!") == 0) { // logical NOT -- only unary operation
        rightOperand = atoi(pop(stackOperands, buffer));
        result = !rightOperand;
      }
      else { // binary operation
        rightOperand = atoi(pop(stackOperands, buffer));
        leftOperand = atoi(pop(stackOperands, buffer));
        errorOperand = evaluateBinaryOperations(OperationTable, elem, &result, &leftOperand, &rightOperand);

        LOG(LPOST, "\nLO = %d | RO = %d | result = %d\n", leftOperand, rightOperand, result);
        if (errorOperand != SUCCESSFUL_EXIT)
          return errorOperand;        
      }

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