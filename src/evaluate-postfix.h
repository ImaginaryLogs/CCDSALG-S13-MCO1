#include "utils.h"
#include "operations.h"
#include "infix-to-postfix.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
/**
 / strncat() 
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
int evaluatePostfix(char *queuePostfixInput, char *stringAnswer) {
  struct Operation OperationTable[MAX_NUM_OPERATIONS];
  String7 stringOperation;
  int queueOperands[16] = {};
  int nthToken        = 0;
  int nthPostfixChar  = 0;
  int nextParseState  = 0;
  int errorOperand    = SUCCESSFUL_EXIT;
  
  int isEvaluatingQueue = true;
  int hasNoErrors       = true;
  int hasCharsToProcess = true;

  initOperatorTable(OperationTable);
  LOG(LPOST, "\n### [EVALUATE POSTFIX] {\n");

  while (isEvaluatingQueue) {
    nextParseState = parseStringInput(queuePostfixInput, &nthPostfixChar, queueOperands + nthToken, stringOperation);
    
    switch (nextParseState) {
      case 0:
        LOG(LPOST, "TYPE: Number (%d)\n\n", queueOperands[nthToken]);
        nthToken++;
        break;
      case 1:
        LOG(LPOST, "TYPE: Operator (%s)\n\n", stringOperation);
        errorOperand = performOperation(OperationTable, queueOperands, &nthToken, stringOperation);
        break;
    }

    // Checking, broke down code for readability
    hasNoErrors = errorOperand == SUCCESSFUL_EXIT;
    hasCharsToProcess = nthPostfixChar < strlen(queuePostfixInput);
    isEvaluatingQueue = hasCharsToProcess && hasNoErrors;
  }
  
  LOG(LPOST, "\n### [END EVALUATION] }\n");
  LOG(LPOST, "\tQuestion: %s\n\tAnswer: %d\n", queuePostfixInput, queueOperands[0]);

  sprintf(stringAnswer, "%d", queueOperands[0]); // itoa() is not supported in Linux.
  return errorOperand;
}

#endif