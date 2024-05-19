#include "../src/evaluate-postfix.h"


int main(){
    String255 stringTestPostfixInput;
    repeatGetString(stringTestPostfixInput, 255);
    String255 stringOutput = "";
    int State = evaluatePostfix(stringTestPostfixInput, stringOutput);
    printf("State: %d, Answer: %s\n", State, stringOutput);
    return 0;
}