/**
 * References:
 * [1] strncat(): https://www.geeksforgeeks.org/strncat-function-in-c-cpp/ 
 */
#include "utils.h"

#ifndef _INFIX_POSTFIX_H_
#define _INFIX_POSTFIX_H_

#define LPOST ENABLE_LOG_EVALUATE_POSTFIX

/**
 * Parses string input to a queue.
 * @note stack array temporarily 
 * @param Input String Input
 * @param nthInputChar The character position being read
 * @param Output
 * @retval 0 if returns a number
 * @retval 1 if returns a string
 */
int parseStringInput(char *Input, int *nthInputChar, int *nOutputNumber, char *nOutputOperation) {
  String63 token = "";
  int isNotWhiteSpace = Input[*nthInputChar] != ' ' && Input[*nthInputChar] != '\0';
  int i;

  LOG(LPOST, "Processing TOKEN:\n");
  // Parses the token
  while (isNotWhiteSpace) {
    strncat(token, Input + *(nthInputChar), 1); // safer to use strncat(), and its useful to select certain characters.
    (*nthInputChar)++;
    isNotWhiteSpace = Input[*nthInputChar] != ' ' && Input[*nthInputChar] != '\0';
    LOG(LPOST, "\t%s\n",token);
  }
  LOG(LPOST, "TOKEN := %s\n", token);

  // Ignore the white space
  while (Input[*nthInputChar] == ' ')
    (*nthInputChar)++;

  *nOutputNumber = atoi(token);
  if (*nOutputNumber > 0)
    return 0 ;
  
  // if not a number by atoi, check if its char '0';
  else if (strlen(token) == 1 && token[0] == '0') {
    *nOutputNumber = 0;
    return 0;
  }
  // else, token is operation
  strcpy(nOutputOperation, token);
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