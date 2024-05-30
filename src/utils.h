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
#include <stdarg.h>
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
typedef char String7[8];

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

/**
 * A developer's version of printf that is togglable - useful for debugging. 
 * @note has configurable MACROS like ENABLE_CLOCK
 * @retval None
 */
void logPrintf(bool isEnabled, char *formattedString, ...){
  // Guard clause
  if (!isEnabled)
    return;
  
  bool isClockEnabled = ENABLE_CLOCK;
  char clockString[64] = "";
  char outputString[255] = "";
  va_list ptrArgList; // Gets the pointer to arguments of alist
  int nArgIndex;

  if (isClockEnabled){
    sprintf(clockString, "[%ld] | ", clock());
    strcat(outputString, clockString);
  }
  
  va_start(ptrArgList, formattedString);

  // vprintf does printf the same way but uses the args provided in the argument.
  // Reference: [4]
  vsprintf(outputString, formattedString, ptrArgList);
  fprintf(stderr, "%s\n", outputString);
  va_end(ptrArgList);

}

#endif 