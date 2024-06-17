#include "../src/evaluate-postfix.h"
#include "test-utils.h"
#ifdef __linux__
    #include<unistd.h>
#endif

#ifndef __t_evaluate_h__
#define __t_evaluate_h__

#define LTPOST true

int main(){
    String255 stringTestPostfixInput = "";
    String255 stringOutput = "";
    String63 choice = "";
    int errorState;
    OUT(LTPOST, "[EVAPST][START ] Testing Evaluate Postfix\n");
    
    do{
        repeatGetString(stringTestPostfixInput, 255);
        OUT(LTPOST, "[EVAPST][INPUT ]: %s\n", stringTestPostfixInput);
        fflush(stdout);

        errorState = evaluatePostfix(stringTestPostfixInput, stringOutput);

        OUT(LTPOST, "[EVAPST][OUTPUT]: %s\n", stringOutput);
        fflush(stdout);
        printErrorCodes(errorState);

        OUT(LTPOST, "[EVAPST][PROMPT]:Type \"quit\" to exit, else it will evaluate.\n\t> ");
        fflush(stdout);

        repeatGetString(choice, 63);
        printf("Received: %d\n", choice);
    } while (strcmp(choice, "quit") != 0);
    OUT(LTPOST, "\n[EVAPST][ENDING]Testing Evaluate Postfix\n");
    fflush(stdout);
    close(STDERR_FILENO);
    return 0;
}

#endif