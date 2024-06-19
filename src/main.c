#include <stdio.h>
#include <math.h>
#include "../tests/test-utils.h"
#include "evaluate-postfix.h"
#include "infix-to-postfix.h"

int main (){
    signal(SIGSEGV, detectSegfault);
    String63 strInput  = "";
    String7 emptyString = "";
    String63 strOutput = "";
    int erStateInfixToPstfx = SUCCESSFUL_EXIT;
    int erStateEvaluateInfx = SUCCESSFUL_EXIT;

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
        erStateInfixToPstfx = infixToPostfix(strInput, postfixQueue);
        printAnswerState(erStateInfixToPstfx, emptyString, false);
        // ### Section: EVALUATE POSTFIX ###
        if (erStateInfixToPstfx == SUCCESSFUL_EXIT) {
            queuePrint(postfixQueue);
            erStateEvaluateInfx = evaluatePostfix(postfixQueue, strOutput);
            printAnswerState(erStateEvaluateInfx, strOutput, true);
        }
        
    }

    return 0;
}