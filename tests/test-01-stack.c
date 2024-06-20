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
    signal(SIGSEGV, detectCrash);
    Stack *stackTestA;
    Stack *TestStackB;

    String31 popReceived = "";
    String31 strTestStrings[stackCharacters] = {""};
    String31 strOrigStrings[stackCharacters] = {""};
    String31 tempString = "";
    
    struct testStatistics ts = createTestStatistics();
    int i = 0;

    for (i = 0; i < stackCharacters; i++){
        strTestStrings[i][0] = 'a' + i;
        strOrigStrings[i][0] = 'a' + i;
        sprintf(tempString, "%d", i);
        strcat(strTestStrings[i], tempString);
        strcat(strOrigStrings[i], tempString);
    }

    for (i = 0; i < stackCharacters; i++){
        printf("%01d) %s\n", i, strTestStrings[i]);
    }
    

    stackTestA = createStack();
    
    printf("Created stackTestA and TestStack B.\n");

    testCase(&ts, assertCaseInteger("Is Stack Empty?", isStackEmpty(stackTestA), 1, true));

    i = 0;
    printf("\nPush %s on the test Stack.\n", strTestStrings[i]); 
    push(stackTestA, strTestStrings[i]);

    testCase(&ts, assertCaseString("Is the top the first testString?", stackTop(stackTestA, tempString), strTestStrings[i], true));
    testCase(&ts, assertCaseInteger("Is Stack Not Empty?", isStackEmpty(stackTestA), 1, false));

    printf("\nPop %s from the test Stack.\n", strTestStrings[i]);    
    pop(stackTestA, popReceived);
    
    testCase(&ts, assertCaseString("Is the top the first testString?", stackTop(stackTestA, tempString), strTestStrings[i], false));
    testCase(&ts, assertCaseString("Does popping remove the firstTestString?", popReceived, strTestStrings[i], true));
    testCase(&ts, assertCaseInteger("Is Stack Empty?", isStackEmpty(stackTestA), 1, true));

    printf("Push the tesst input on to the stack.\n");

    i = 0;
    for(i = 0; i < stackCharacters; i++){
        printf("\nPush %s on the test Stack.\n", strTestStrings[i]); 
        push(stackTestA, strTestStrings[i]);
    }
    --i;

    testCase(&ts, assertCaseInteger("Is Stack Not Empty?", isStackEmpty(stackTestA), 1, false));
    testCase(&ts, assertCaseString("Is the top the first testString?", stackTop(stackTestA, tempString), strTestStrings[0], false));
    testCase(&ts, assertCaseString("Is the top the last testString?", stackTop(stackTestA, tempString), strTestStrings[i], true));
    

    i = 0;
    for(i = 0; i < stackCharacters; i++){
        printf("Pop %s on the test Stack.\n", strTestStrings[i]); 
        pop(stackTestA, strTestStrings[i]);
    }
    --i;

    testCase(&ts, assertCaseInteger("Is Stack Empty?", isStackEmpty(stackTestA), 1, true));
    testCase(&ts, assertCaseString("Is the top the first testString?", stackTop(stackTestA, tempString), strTestStrings[i], false));
    testCase(&ts, assertCaseString("Is the top the last testString?", stackTop(stackTestA, tempString), strTestStrings[0], false));

    testCase(&ts, assertCaseString("Is the top the first testString the last?", strOrigStrings[0], strTestStrings[i], true));
    testCase(&ts, assertCaseString("Is the top the last testString the first?", strOrigStrings[i], strTestStrings[0], true));
    //wait(10);
    //stackDelete(stackTestA);
    testCase(&ts, assertCaseInteger("Is the stack deleted?", isStackEmpty(stackTestA), true, true));
    testCase(&ts, assertCaseInteger("Is the stack pointer empty?", stackTestA->top == 0, true, true));

    printTestStatistics(&ts);
    printCommunicatingPipeTestStatistics(&ts);

    return 0;
}

#endif