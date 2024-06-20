#include <stdio.h>
#include <math.h>
#include "../tests/test-utils.h"
#include "evaluate-postfix.h"
#include "infix-to-postfix.h"
#include "config.h"

int main (){
    signal(SIGSEGV, detectSegfault);
    String255 strInput  = "";
    String7 emptyString = "";
    String255 strOutput = "";
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

    while (strcmp(strInput, "quit") != 0){
        // ### Scanning Inputs ###
        repeatGetString(strInput, 255);


        // ### Section: INFIX -> POSTFIX ###
        erStateInfixToPstfx = infixToPostfix(strInput, postfixQueue, OperationTable);
        printAnswerState(erStateInfixToPstfx, emptyString, false);

        // ### Section: EVALUATE POSTFIX ###
        erStateEvaluateInfx = evaluatePostfix(postfixQueue, strOutput, OperationTable, erStateInfixToPstfx);
        printAnswerState(erStateEvaluateInfx, strOutput, true);
    }

    return 0;
}