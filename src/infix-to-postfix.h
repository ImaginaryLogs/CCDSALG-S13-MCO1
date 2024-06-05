#include "utils.h"

#ifndef _INFIX_POSTFIX_H_
#define _INFIX_POSTFIX_H_



/**
 * Temporary code to emulate stack behavior by a limited int array.
 * @retval 0 if number
 * @retval 1 if operation
 * @retval 2 if error
 */
int parseStringInput(char *Input, int *nthPostfixChar, int *OutputNumber, char *OutputOperation) {
  String255 token = "";
  int i;
  // Identifies Token
  while (Input[*nthPostfixChar] != ' ' && Input[*nthPostfixChar] != '\0') {
    strncat(token, Input + *(nthPostfixChar), 1);
    (*nthPostfixChar)++;
    printf("Processing: %s\n",token);
  }
  printf("Token Parsed: %s\n", token);

  // if there's more tokens, increase count
  while (Input[*nthPostfixChar] == ' ')
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
 * Translates infix notation into postfix notation, and stores it in a queue.
 * If error arrises, returns one; else, zero.
 * 
 * @retval 0 SUCCESSFUL_EXIT
 * @retval 1 ER_SYNTAX
 */
int infixToPostfix(String255 inputString){
    int i;

    for (i = 0; i < strlen(inputString); i++){

    }

    return 0;
}

#endif