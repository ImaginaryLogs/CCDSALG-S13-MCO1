#include "../src/evaluate-postfix.h"
#include "unistd.h"


int main(){
    String255 stringTestPostfixInput = "";
    String255 stringOutput = "";
    String63 choice = "";
    int errorState;

    do{
        repeatGetString(stringTestPostfixInput, 255);
        printf("[INPUT ]: %s\n", stringTestPostfixInput);
        errorState = evaluatePostfix(stringTestPostfixInput, stringOutput);

        fprintf(stdout, "[d ]: %s\n", stringOutput);

        printErrorCodes(errorState);
        fprintf(stderr, "Type \"quit\" to exit, else it will evaluate.\n\t> ");
        repeatGetString(choice, 63);
        sleep(1);
    } while (strcmp(choice, "quit") != 0);
    return 0;
}