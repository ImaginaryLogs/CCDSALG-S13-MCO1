#include "../src/evaluate-postfix.h"
#include "test-utils.h"
#ifdef __linux__
    #include<unistd.h>
#endif

#ifndef __t_evaluate_h__
#define __t_evaluate_h__

int main(){
    String255 stringTestPostfixInput = "";
    String255 stringOutput = "";
    String63 choice = "";
    int errorState;
    OUT("[EVAPST][START ] Testing Evaluate Postfix\n");
    do{
        sleep(UNIT_TEST_REPEAT_DELAY);
        repeatGetString(stringTestPostfixInput, 255);
        OUT("[EVAPST][INPUT ]: %s\n", stringTestPostfixInput);
        fflush(stdout);

        errorState = evaluatePostfix(stringTestPostfixInput, stringOutput);

        OUT("[EVAPST][OUTPUT]: %s\n", stringOutput);
        fflush(stdout);
        printErrorCodes(errorState);

        OUT("[EVAPST][PROMPT]:Type \"quit\" to exit, else it will evaluate.\n\t> ");
        fflush(stdout);

        repeatGetString(choice, 63);
        
    } while (strcmp(choice, "quit") != 0);
    OUT("\n[EVAPST][ENDING]Testing Evaluate Postfix\n");
    fflush(stdout);

    return 0;
}

#endif