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
#include "config.h"


// Include Guards to help prevent double inclusion collisions when the compiler's preprocessor links the files.
// Reference: [3]
#ifndef _UTILS_H_
#define _UTILS_H_


// Region to denote types defined at utils.h
#pragma region UTIL_DEFINED_TYPES



typedef char String255[256];
typedef char String63[64];
typedef char String31[32];
typedef char String7[8];
typedef char String2[3];

/**
 * Error codes for any error prone operation
 */
enum ErrorCodes {
  SUCCESSFUL_EXIT,
  ER_SYNTAX_ERROR,
  ER_UNDEFINED_OPERATION,
  ER_MISSING_OPERANDS,
  ER_MISSING_OPERATOR,
  ER_DIVIDE_BY_ZERO
};

#pragma endregion


#if DEBUG 
  /**
  * A developer's version of printf that is togglable - useful for debugging. 
  * @retval None
  */
  #define LOG(X, ...) ({if (X) fprintf(stderr, __VA_ARGS__);})
#else
  #define LOG(X, ...)
#endif

void printErrorCodes(int errorCode){
  LOG(DEBUG, "[ESTATE]: ");
  switch(errorCode){
    case SUCCESSFUL_EXIT:
      LOG(DEBUG, "%sSUCCESS%s\n", F_GREEN, F_NORMAL);
      break;
    case ER_SYNTAX_ERROR:
      printf("%sSYNTAX ERROR!%s\n", F_RED, F_NORMAL);
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
    default:
      printf("%sUNKNOWN ERROR!%s\n", F_RED, F_NORMAL);
  }
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
    hasExcessWhitespace =
        (hasPossibleOverflow)
            ? (getchar() == '\n')
            : false; // If input string length matches maxCharLength, then it
                     // should have '\n' only as excess.

    // ### Error handling ###
    if (hasPossibleOverflow && !hasExcessWhitespace) {
      // Erases input
      
      printf("Error, input exceeds %d-character limit.\n", maxCharLength);
    } else
      isIncorrectInput = false;

    // ### Clean-up of '\n' ###
    if (!hasExcessWhitespace) {
      pInput[maxCharLength] =
          '\0'; // erases 'A' normally and '\n' if exact 255 = 255 limit.
      pInput[strlen(pInput) - 1] =
          '\0'; // erase the '\n' that fgets() appends normally
    }
  }
}

#endif 