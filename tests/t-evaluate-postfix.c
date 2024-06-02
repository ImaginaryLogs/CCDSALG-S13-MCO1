#include "../src/evaluate-postfix.h"


int main(){
    String255 stringTestPostfixInput = "";
    String255 stringOutput = "";
    String63 choice = "";

    do{
        repeatGetString(stringTestPostfixInput, 255);
        int State = evaluatePostfix(stringTestPostfixInput, stringOutput);
        printf("State: %d, Answer: %s\n", State, stringOutput);
        printf("Type \"quit\" to exit, else it will evaluate.\n");
        repeatGetString(choice, 63);
    } while (strcmp(choice, "quit") != 0);
    return 0;
}