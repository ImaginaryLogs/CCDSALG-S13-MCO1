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
  int parseState      = 0;
  int errorOperand    = SUCCESSFUL_EXIT;
  int fLog = ENABLE_LOG_EVALUATE_POSTFIX;

  logPrintf(fLog, "\n#=====Processing=====#\n");

  while (nthPostfixChar < strlen(queuePostfixInput) && errorOperand == SUCCESSFUL_EXIT) {
    parseState = parseStringInput(queuePostfixInput, &nthPostfixChar,
                            queueOperands + nthToken, stringOperation);
    
    logPrintf(fLog, "\tStack: %d %d %d\n", queueOperands[0], queueOperands[1], queueOperands[2]);
    
    switch (parseState) {
      case 0:
        logPrintf(fLog, "\tNumber Token: %d\n", queueOperands[nthToken]);
        nthToken++;
        break;
      case 1:
        logPrintf(fLog, "\tOpernd Token: %s\n", stringOperation);
        errorOperand = evaluateOperation(OperationTable, queueOperands, &nthToken, stringOperation);
        break;
    }
  }
  
  printf("Answer: %d %d %d\n", queueOperands[0], queueOperands[1], queueOperands[2]);

  logPrintf(fLog, "\n#=====================#");
  logPrintf(fLog, " Question: %s\n Answer: %d\n", queuePostfixInput, queueOperands[0]);
  sprintf(stringAnswer, "%d", queueOperands[0]); // itoa() is not supported in Linux.
  return errorOperand;
}

#endif