#include "../src/evaluate-postfix.h"
#include "test-utils.h"
#ifdef __linux__
    #include<unistd.h>
#endif

#ifndef __t_main_c__
#define __t_main_c__

#define LTPOST true
#define TEST_EVALUATIONS 2


int main(){
    CLOCKINITIAL();
    signal(SIGSEGV, detectCrash);
    struct testStatistics ts = createTestStatistics();
    FILE *fActual, *fExpect;
    String63 strFilenameInputActual = "01-infix-input.txt";
    String63 strFilenameInputExpect = "04-evaluation-expect.txt";
    String255 strInput       = "";
    String255 strCompared    = "";
    String255 strOutput      = "";
    String7 emptyString = "";
    int erStateInfixToPstfx = SUCCESSFUL_EXIT;
    int erStateEvaluateInfx = SUCCESSFUL_EXIT;
    struct Operation OperationTable[MAX_NUM_OPERATIONS];
    initOperatorTable(OperationTable);
    queue* postfixQueue = createQueue();

    /** 
     * ### Detects which OS you are using ### 
     * @ImaginaryLogs uses Ubuntu sometimes to code. 
     * It is assume to be on a Windows Operating System.
     **/ 
    printf("\n[!] Notifications:\n");
    #ifdef _WIN32  
        printf("\tUsing Windows Operating System\n");
        system("echo \tANSI Enabled");
    #elif __linux__    
        printf("\tUsing Linux-based System\n");
    #endif

    fActual = fileValidOpen(strFilenameInputActual);
    fExpect = fileValidOpen(strFilenameInputExpect);

    while (ts.currentTestNumber < MAX_INPUTLINES){
        // ### Scanning Inputs ###
        textline255Reader(fActual, strInput);
        textline255Reader(fExpect, strCompared);

        CLOCKSTART();    
        // ### Section: INFIX -> POSTFIX ###
        erStateInfixToPstfx = infixToPostfix(strInput, postfixQueue, OperationTable);
        //printAnswerState(erStateInfixToPstfx, emptyString, false);
        queuePrint(postfixQueue);

        // ### Section: EVALUATE POSTFIX ###
        erStateEvaluateInfx = evaluatePostfix(postfixQueue, strOutput, OperationTable, erStateInfixToPstfx);
        outputErrorCodes(erStateEvaluateInfx, strOutput);
        CLOCKEND();

        testCase(&ts, assertCaseString("Is the Infix Output the Same as Expected Output?", strOutput, strCompared, true));
    }
    printTestStatistics(&ts);
    printCommunicatingPipeTestStatistics(&ts);

    return 0;
}
#endif 