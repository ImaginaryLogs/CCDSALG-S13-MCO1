/**
 * Unit testing for stack and its components.
 */
#include "../src/stack.h"
#include "errno.h"
#include "test-utils.h"

#ifndef _T_STACK_
#define _T_STACK_

#define stackCharacters 5

int main(){
    signal(SIGSEGV, detectSegfault);
    Stack *stackTestA;
    Stack *TestStackB;

    String31 popReceived = "";
    String31 strTestStrings[stackCharacters] = {""};
    String31 tempString = "";
    
    struct testStatistics ts = createTestStatistics();
    int i = 0;

    for (i = 0; i < stackCharacters; i++){
        strTestStrings[i][0] = 'a' + i;
        sprintf(tempString, "%d", i);
        strcat(strTestStrings[i], tempString);
    }

    for (i = 0; i < stackCharacters; i++){
        printf("%01d) %s\n", i, strTestStrings[i]);
    }
    

    stackTestA = createStack();
    
    printf("Created stackTestA and TestStack B.\n");

    testCase(&ts, assertCaseIntger("Is Stack Empty?", isStackEmpty(stackTestA), 1, true));

    i = 0;
    printf("\nPush %s on the test Stack.\n", strTestStrings[i]); 
    push(stackTestA, strTestStrings[i]);

    testCase(&ts, assertCaseString("Is the top the first testString?", stackTop(stackTestA), strTestStrings[i], true));
    testCase(&ts, assertCaseIntger("Is Stack Not Empty?", isStackEmpty(stackTestA), 1, false));

    printf("\nPop %s from the test Stack.\n", strTestStrings[i]);    
    pop(stackTestA, popReceived);

    testCase(&ts, assertCaseString("Is the top the first testString?", stackTop(stackTestA), strTestStrings[i], false));
    testCase(&ts, assertCaseString("Does popping remove the firstTestString?", popReceived, strTestStrings[i], true));
    testCase(&ts, assertCaseIntger("Is Stack Empty?", isStackEmpty(stackTestA), 1, true));

    i = 0;
    for(i = 0; i < stackCharacters; i++){
        printf("\nPush %s on the test Stack.\n", strTestStrings[i]); 
        push(stackTestA, strTestStrings[i]);
    }
    --i;

    testCase(&ts, assertCaseIntger("Is Stack Not Empty?", isStackEmpty(stackTestA), 1, false));
    testCase(&ts, assertCaseString("Is the top the first testString?", stackTop(stackTestA), strTestStrings[0], false));
    testCase(&ts, assertCaseString("Is the top the last testString?", stackTop(stackTestA), strTestStrings[i], true));
    
    printTestStatistics(&ts);
    close(STDERR_FILENO);
    return 0;
}

#endif