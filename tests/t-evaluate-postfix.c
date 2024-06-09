#include "../src/evaluate-postfix.h"
#include "unistd.h"
#include "test-utils.h"

int main(){
    String255 stringTestPostfixInput = "";
    String255 stringOutput = "";
    String63 choice = "";
    int errorState;
    fprintf(stdout, "[EVAPST][START ] Testing Evaluate Postfix\n");
    do{
        repeatGetString(stringTestPostfixInput, 255);
        printf("[EVAPST][INPUT ]: %s\n", stringTestPostfixInput);

        errorState = evaluatePostfix(stringTestPostfixInput, stringOutput);

        fprintf(stdout, "[EVAPST][OUTPUT]: %s\n", stringOutput);
        printErrorCodes(errorState);

        fprintf(stdout, "[EVAPST][PROMPT]:Type \"quit\" to exit, else it will evaluate.\n\t> ");
        repeatGetString(choice, 63);
        
        sleep(UNIT_TEST_REPEAT_DELAY);
    } while (strcmp(choice, "quit") != 0);
    fprintf(stdout, "\n[EVAPST][ENDING]Testing Evaluate Postfix\n");
    return 0;
}