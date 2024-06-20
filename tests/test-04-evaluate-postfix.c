#include "../src/evaluate-postfix.h"
#include "test-utils.h"
#ifdef __linux__
    #include<unistd.h>
#endif

#ifndef __t_evaluate_h__
#define __t_evaluate_h__

#define LTPOST true
#define TEST_EVALUATIONS MAX_INPUTLINES


int main(){
    signal(SIGSEGV, detectCrash);
    signal(SIGABRT, detectCrash);
    struct testStatistics ts = createTestStatistics();
    String63 strFilenameInputActual = "03-postfix-input.txt";
    String63 strFilenameInputExpect = "04-evaluation-expect.txt";
    String63 strInput       = "";
    String63 strCompared    = "";
    String63 strOutput      = "";
    FILE *fActual, *fExpect;
    int errEvalState = 0;
    int i = 0;
    

    OUT(LTPOST, "[EVAPST][START ] Testing Evaluate Postfix\n");
    queue *postfixQueue = createQueue();
    struct Operation OperationTable[MAX_NUM_OPERATIONS];
    initOperatorTable(&OperationTable);
    fActual = fileValidOpen(strFilenameInputActual);
    fExpect = fileValidOpen(strFilenameInputExpect);

    for (i = 0; i < TEST_EVALUATIONS && !feof(fActual) && !feof(fExpect); i++){
        textline255Reader(fActual, strInput);
        textline255Reader(fExpect, strCompared);
        OUT(LTPOST, "[EVAPST][START ]: %s\n", strInput);
        stringToQueue(strInput, postfixQueue);
        //queuePrint(postfixQueue);
        if (i < 20 || i > 25)
            errEvalState = evaluatePostfix(postfixQueue, strOutput, OperationTable, SUCCESSFUL_EXIT);
        outputErrorCodes(errEvalState, strOutput);
        testCase(&ts, assertCaseString("Is the Postfix Output the Same as Expected Output?", strOutput, strCompared, true));
        while (!queueEmpty(postfixQueue)){
            dequeue(postfixQueue, strInput);
        }
    }
    printTestStatistics(&ts);
    printCommunicatingPipeTestStatistics(&ts);

    OUT(LTPOST, "\n[EVAPST][ENDING]: Testing Evaluate Postfix\n");
    queueDelete(postfixQueue);
    fclose(fActual);
    fclose(fExpect);
    

    return 0;
}

#endif