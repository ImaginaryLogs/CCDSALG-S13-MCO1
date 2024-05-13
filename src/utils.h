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

void 
repeatGetString(char *pInput, int maxCharLength){
    bool isIncorrectInput   = true;
    bool hasPossibleOverflow = false;
    bool hasExcessWhitespace = false;
    bool hasOWPenulti    = false;
    bool hasOWUltimus          = false;

    do {
        // Replace the last non-null char in string with something temporarily
        pInput[maxCharLength] = 'A';

        /** 
         * Note that fgets() is regarded to be more safer scanf()
         *  - On one hand, any excess in fgets() is left in stdin, and leaves '\n' in n - 1 space.
         *  - On the other hand, scanf() tends to overwrite beyond given space.
         * Reference: [1]
         */ 
        fgets(pInput, maxCharLength + 1, stdin); // size of array = maxCharLength + 1, allows exact 255-char-len input

        // Overwrite flags:
        hasOWPenulti = pInput[maxCharLength - 1] != '\n'; // Penultimus = second to last
        hasOWUltimus = pInput[maxCharLength] == '\0';     // Ultimus    = last
        hasPossibleOverflow = hasOWPenulti && hasOWUltimus;

        if (hasPossibleOverflow)
            hasExcessWhitespace = (getchar() == '\n'); // If input matches maxCharLength, then it should have '\n' only.

        if (hasPossibleOverflow && !hasExcessWhitespace){
            while(getchar() != '\n');
            printf("Error, input exceeds %d-character limit.\n", maxCharLength);
        } else 
            isIncorrectInput = false;
        
        if (!hasExcessWhitespace) {
            printf("er");
            pInput[maxCharLength] = '\0'; // erases 'A' normally and '\n' if exact 255 = 255 limit.
            pInput[strlen(pInput) - 1] = '\0'; // erase the '\n' that fgets() appends normally
        }

    } while(isIncorrectInput);
}