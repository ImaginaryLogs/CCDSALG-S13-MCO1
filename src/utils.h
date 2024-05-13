/**
 * Stores any utility functions that we may need.
 * References:
 *  [1]: fgets() -  https://www.ibm.com/docs/en/i/7.4?topic=functions-fgets-read-string
*/
#include<stdbool.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef char String255[256];
typedef char String64[63];

/**
 * @brief Prompts the user for a string input. If it exceeds the given size, it reprompts.
 * @param *pInput: Points to the string you want to place input.
 * @param maxCharLength: Max length the string can handle.
 * @retval None
 */
void repeatGetString(char *pInput, int maxCharLength){
    // Boolean conditions for better understanding
    bool isIncorrectInput       = true;
    bool hasPossibleOverflow    = false;
    bool hasExcessWhitespace    = false;
    bool hasOverwrittenSecondLast           = false;
    bool hasOverwrittenLastChar           = false;

    while(isIncorrectInput) {
        // Replace the last non-null char in string with something temporarily
        pInput[maxCharLength] = 'A';

        /** 
         * Note that fgets() is regarded to be more safer scanf()
         *  - On one hand, any excess in fgets() is left in stdin, and leaves '\n' in n - 1 space.
         *  - On the other hand, scanf() tends to overwrite beyond given space.
         * Reference: [1]
         */ 
        fgets(pInput, maxCharLength + 1, stdin); // size of array = maxCharLength + 1, allows exact 255-character-length input

        // ### Overwrite flags for checking ###
        hasOverwrittenSecondLast = pInput[maxCharLength - 1] != '\n'; 
        hasOverwrittenLastChar = pInput[maxCharLength] == '\0';     
        hasPossibleOverflow = hasOverwrittenSecondLast && hasOverwrittenLastChar;
        hasExcessWhitespace = (hasPossibleOverflow) ? (getchar() == '\n') : false; // If input string length matches maxCharLength, then it should have '\n' only as excess.

        // ### Error handling ###
        if (hasPossibleOverflow && !hasExcessWhitespace){
            while(getchar() != '\n');
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