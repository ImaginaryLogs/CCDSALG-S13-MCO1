#include <stdio.h>
#include <math.h>
#include "../tests/test-utils.h"
#include "evaluate-postfix.h"
#include "infix-to-postfix.h"
#include "config.h"

int main (){
    CLOCKINITIAL();
    signal(SIGSEGV, detectCrash);
    String255 strInput  = "";
    String639 strQueueInfo = "";
    String7 emptyString = "";
    String255 strOutput = "";
    int erStateInfixToPstfx = ER_MISSING_OPERANDS;
    int erStateEvaluateInfx = SUCCESSFUL_EXIT;
    struct Operation OperationTable[MAX_NUM_OPERATIONS];
    initOperatorTable(OperationTable);
    queue* postfixQueue = createQueue();

    #if DEBUG
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
    #endif

    do {
        // ### Scanning Inputs ###
        repeatGetString(strInput, 255);

        CLOCKSTART();

        // ### Section: INFIX -> POSTFIX ###
        erStateInfixToPstfx = infixToPostfix(strInput, postfixQueue, OperationTable);
        printAnswerState(erStateInfixToPstfx, emptyString, false);
        
        if (erStateInfixToPstfx == SUCCESSFUL_EXIT)