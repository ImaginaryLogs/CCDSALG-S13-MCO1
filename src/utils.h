/**
 * Stores any utility functions that we may need.
 * References:
 *  [1]: fgets() -
 * https://www.ibm.com/docs/en/i/7.4?topic=functions-fgets-read-string
 *  [2]: variadic functions - 
 * https://en.cppreference.com/w/cpp/utility/variadic
 *  [3]: include guards - 
 * https://www.geeksforgeeks.org/include-guards-in-c/
 *  [4]: vprintf()
 * https://cplusplus.com/reference/cstdio/vprintf/
 * 
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h> // for pow()
#include "config.h"


// Include Guards to help prevent double inclusion collisions when the compiler's preprocessor links the files.
// Reference: [3]
#ifndef _UTILS_H_
#define _UTILS_H_


// Region to denote types defined at utils.h
// UTIL_DEFINED_TYPES

typedef char String639[640];
typedef char String511[512];
typedef char String255[256];
typedef char String127[128];
typedef char String63[64];
typedef char String31[32];
typedef char String7[8];
typedef char String2[3];

/**
 * Error codes for any error prone operation
 */
enum ErrorCodes {
  SUCCESSFUL_EXIT,
  ER_NO_STRING_TO_PARSE,
  ER_UNDEFINED_OPERATION,
  ER_MISSING_OPERANDS,
  ER_MISSING_OPERATOR,
  ER_DIVIDE_BY_ZERO,
  ER_UNDEFINED,
  ER_INDETERMINATE
};

enum ParserState {
  TOKEN_NUMBER,
  TOKEN_OPERATION,
  END_OF_STRING
};



#if DEBUG 
  /**
  * A developer's version of printf that is togglable - useful for debugging. 
  * @retval None
  */
  #define LOG(X, ...) do{typeof (X) _X = (X); if (_X) fprintf(stdout, __VA_ARGS__);} while(0)
#else
  #define LOG(X, ...)
#endif

#if ENABLE_CLOCK
  #define CLOCKINITIAL() double startingTime, endingTime, timeElapsed;
  #define CLOCKSTART() do{startingTime = clock(); fprintf(stdout, "Start: %fs\n", startingTime);}while(0)
  #define CLOCKEND() do{ endingTime = clock(); timeElapsed = (double)(endingTime - startingTime) / CLOCKS_PER_SEC; fprintf(stdout, "End  : %fs\nTime : %fs\n", endingTime, timeElapsed);}while(0)
#else
  #define CLOCKINITIAL() 
  #define CLOCKSTART()
  #define CLOCKEND() 
#endif


void parseRestOfNumbers(char *token, char *strInput, int *nthInputChar,int *isCurrCharNull, int *isCurrCharNumber){
  char currChar;
  while (!(*isCurrCharNull) && *isCurrCharNumber) {
      strncat(token, strInput + *(nthInputChar), 1); // safer to use strncat(), and its useful to select certain characters.
      (*nthInputChar)++;

      currChar = strInput[*nthInputChar];
      *isCurrCharNull = currChar == '\0';
      *isCurrCharNumber = currChar >= '0' && currChar <= '9';

      LOG(LPOST, "%s, ", token);
    }
    LOG(LPOST, "[%s]\n", token);
}

void parseRestOfOperation(char *token, char *strInput, int *nthInputChar, char currentChar ){
  if (currentChar == '>' || currentChar == '<' || currentChar == '!') {
    int nIsEquals = 1; // next char is '='
    int nnIsEquals = 1; // next next char is '='
    int nnnIsEquals = 1; // next next next char is '=' 

    if (strInput[*nthInputChar + 1] != '=') {
      nIsEquals = 0;
    }

    if (strInput[*nthInputChar + 2] != '=') {
      nnIsEquals = 0; 
    }

    if (strInput[*nthInputChar + 3] != '=') {
      nnnIsEquals = 0;
    }

    // Let ? be < or >
    if (!nIsEquals) { // ?
      strncat(token, strInput + *(nthInputChar), 1);
      (*nthInputChar)++;
    } else if (!nnIsEquals) { // ?=
      strncat(token, strInput + *(nthInputChar), 2);
      (*nthInputChar) += 2;
    } else if (!nnnIsEquals) { // ? followed by ==
      strncat(token, strInput + *(nthInputChar), 1);
      (*nthInputChar)++;
    } else { // ?= followed by ==
      strncat(token, strInput + *(nthInputChar), 2);
      (*nthInputChar) += 2;
    }
  } else if (currentChar == '=' || currentChar == '&' || currentChar == '|') {
    strncat(token, strInput + *(nthInputChar), 2);
    (*nthInputChar) += 2;
  } else { // 1-char operators
    strncat(token, strInput + *(nthInputChar), 1);
    (*nthInputChar)++;
  }
}


/**
 * Parses string input to a queue.
 * @note stack array temporarily 
 * @param Input String Input
 * @param nthInputChar The character position being read
 * @param Output
 * @retval 0 if returns a number
 * @retval 1 if returns a string
 * @retval 2 if end of string
 */
int parseStringInput(char *Input, int *nthInputChar, int *nOutputNumber, char *nOutputOperation) {
  String63 token = "";
  char currChar = Input[*nthInputChar];
  int isCurrCharNull = currChar == '\0';
  int isCurrCharNumber = currChar >= '0' && currChar <= '9';

  LOG(LPOST, "\n[INPO] Token Recognition: %s\n", token);
  // End of string has been reached
  if (isCurrCharNull){
    return END_OF_STRING;
  }
  // Parses the token
  if (isCurrCharNumber) { // parsing a operand
    parseRestOfNumbers(token, Input, nthInputChar, &isCurrCharNull, &isCurrCharNumber);
  } else { // parsing an operator
    parseRestOfOperation(token, Input, nthInputChar, currChar);
  }

  *nOutputNumber = atoi(token);
  if (*nOutputNumber > 0)
    return TOKEN_NUMBER;
  
  // if not a number by atoi, check if it's char '0';
  else if (strlen(token) == 1 && token[0] == '0') {
    *nOutputNumber = 0;
    return TOKEN_NUMBER;
  }
  // else, token is operation
  strcpy(nOutputOperation, token);
  return TOKEN_OPERATION;
}

void printAnswerState(int errorCode, char *strAnswer, int isFinalAnswer){
  switch(errorCode){
    case SUCCESSFUL_EXIT:
      LOG(DEBUG, "%sSUCCESS%s\n", F_GREEN, F_NORMAL);
      if (isFinalAnswer)
        printf("%s\n", strAnswer);
      break;
    case ER_NO_STRING_TO_PARSE:
      LOG(DEBUG, "%sNO STRING TO PARSE ERROR!%s\n", F_RED, F_NORMAL);
      break;
    case ER_UNDEFINED_OPERATION:
      printf("%sUNDEFINED OPERATION ERROR!%s\n", F_RED, F_NORMAL);
      break;
    case ER_MISSING_OPERANDS:
      printf("%sMISSING OPERANDS ERROR!%s\n", F_RED, F_NORMAL);
      break;
    case ER_MISSING_OPERATOR:
      printf("%sMISSING OPERATOR ERROR!%s\n", F_RED, F_NORMAL);
      break;
    case ER_DIVIDE_BY_ZERO:
      printf("%sDIVISION BY ZERO ERROR!%s\n", F_RED, F_NORMAL);
      break;
    case ER_UNDEFINED:
      printf("%sUNDEFINED ERROR!%s\n", F_RED, F_NORMAL);
      break;
    case ER_INDETERMINATE:
      printf("%sINDETERMINATE ERROR!%s\n", F_RED, F_NORMAL);
      break;
    default:
      printf("%sUNKNOWN ERROR!%s\n", F_RED, F_NORMAL);
  }
}

char *outputErrorCodes(int errorCode, char *strOutput){
  String63 strResult = "";
  int hasChanged = true;

  switch(errorCode){
    case SUCCESSFUL_EXIT:
      hasChanged = false;
      break;
    case ER_NO_STRING_TO_PARSE:
      //sprintf(strResult, "%sSYNTAX ERROR!%s", F_RED, F_NORMAL);
      break;
    case ER_UNDEFINED_OPERATION:
      sprintf(strResult, "%sUNDEFINED OPERATION ERROR!%s", F_RED, F_NORMAL);
      break;
    case ER_MISSING_OPERANDS:
      sprintf(strResult, "%sMISSING OPERANDS ERROR!%s", F_RED, F_NORMAL);
      break;
    case ER_MISSING_OPERATOR:
      sprintf(strResult, "%sMISSING OPERATOR ERROR!%s", F_RED, F_NORMAL);
      break;
    case ER_DIVIDE_BY_ZERO:
      sprintf(strResult, "%sDIVISION BY ZERO ERROR!%s", F_RED, F_NORMAL);
      break;
    case ER_UNDEFINED:
      sprintf(strResult, "%sUNDEFINED ERROR!%s", F_RED, F_NORMAL);
      break;
    case ER_INDETERMINATE:
      sprintf(strResult, "%sINDETERMINATE ERROR!%s", F_RED, F_NORMAL);
      break;
    default:
      sprintf(strResult, "%sUNKNOWN ERROR!%s", F_RED, F_NORMAL);
  }

  if (hasChanged){
    strcpy(strOutput, strResult);
  }

  return strOutput;
}


/**
 * Clears stdinput.  
 * @retval None
 */
void clearStdInput(){
  while (getchar() != '\n');
}



/**
 * @brief Prompts the user for a string input. If it exceeds the given size, it
 * reprompts.
 * @param *pInput: Points to the string you want to place input.
 * @param maxCharLength: Max length the string can handle.
 * @retval None
 */
void repeatGetString(char *pInput, int maxCharLength) {
  // Boolean conditions for better understanding
  bool isIncorrectInput         = true;
  bool hasPossibleOverflow      = false;
  bool hasExcessWhitespace      = false;
  bool hasOverwrittenSecondLast = false;
  bool hasOverwrittenLastChar   = false;

  while (isIncorrectInput) {
    // Replace the last non-null char in string with something temporarily
    pInput[maxCharLength] = 'A';

    /**
     * Note that fgets() is regarded to be more safer scanf()
     *  - On one hand, any excess in fgets() is left in stdin, and leaves '\n'
     * in n - 1 space.
     *  - On the other hand, scanf() tends to overwrite beyond given space (based on experience).
     * Reference: [1]
     */
    fgets(pInput, maxCharLength + 1, stdin); 
    // size of array = maxCharLength + 1, allows exact
    // 255-character-length input

    // ### Overwrite flags for checking ###
    hasOverwrittenSecondLast  = pInput[maxCharLength - 1] != '\n';
    hasOverwrittenLastChar    = pInput[maxCharLength] == '\0';
    hasPossibleOverflow = hasOverwrittenSecondLast && hasOverwrittenLastChar;
    hasExcessWhitespace = (hasPossibleOverflow) ? (getchar() == '\n') : false; 
    // If input string length matches maxCharLength, then it
    // should have '\n' only as excess.

    // ### Error handling ###
    if (hasPossibleOverflow && !hasExcessWhitespace) {
      // Erases input
      
      printf("Error, input exceeds %d-character limit.\n", maxCharLength);
    } else
      isIncorrectInput = false;

    // ### Clean-up of '\n' ###
    if (!hasExcessWhitespace) {
      pInput[maxCharLength] = '\0'; // erases 'A' normally and '\n' if exact 255 = 255 limit.
      pInput[strlen(pInput) - 1] = '\0'; // erase the '\n' that fgets() appends normally
    }
  }
}

#endif 