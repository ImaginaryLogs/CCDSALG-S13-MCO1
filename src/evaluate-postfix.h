#include "utils.h"
#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


// Include Guards to help prevent double inclusion collisions when the compiler's preprocessor links the files.
// References: utils.h [3]
#ifndef _EVALUATE_POSTFIX_H_
#define _EVALUATE_POSTFIX_H_
/**
 * Temporary code to emulate stack behavior by a limited int array.
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

  logPrintf(ENABLE_LOG_EVALUATE_POSTFIX, "\n#=====Processing=====#\n");
  String7 stringOperation;
  int queueOperands[16] = {};
  int nthToken        = 0;
  int nthPostfixChar  = 0;
  int parseState      = 0;
  int errorOperand    = SUCCESSFUL_EXIT;

  while (nthPostfixChar < strlen(queuePostfixInput) && errorOperand == SUCCESSFUL_EXIT) {
    parseState = parseInput(queuePostfixInput, &nthPostfixChar,
                            queueOperands + nthToken, stringOperation);
    
    logPrintf(ENABLE_LOG_EVALUATE_POSTFIX, "\tStack: %d %d %d\n", queueOperands[0], queueOperands[1], queueOperands[2]);
    
    switch (parseState) {
      case 0:
        logPrintf(ENABLE_LOG_EVALUATE_POSTFIX, "\tNumber Token: %d\n", queueOperands[nthToken]);
        nthToken++;
        break;
      case 1:
        logPrintf(ENABLE_LOG_EVALUATE_POSTFIX, "\tOpernd Token: %s\n", stringOperation);
        errorOperand = evaluateOperation1(OperationTable, queueOperands, &nthToken, stringOperation);
        break;
    }
  }
  
  printf("Answer: %d %d %d\n", queueOperands[0], queueOperands[1], queueOperands[2]);

  logPrintf(ENABLE_LOG_EVALUATE_POSTFIX, "\n#=====================#");
  logPrintf(ENABLE_LOG_EVALUATE_POSTFIX, " Question: %s\n Answer: %d\n", queuePostfixInput, queueOperands[0]);
  sprintf(stringAnswer, "%d", queueOperands[0]); // itoa() is not supported in Linux.
  return errorOperand;
}

#endif