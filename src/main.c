#include <stdio.h>
#include "evaluate-postfix.h"
#include "infix-to-postfix.h"

int main (){
    String255 strInput  = "";
    String255 strOutput = "";
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
        printErrorCodes(erStateInfixToPstfx);
        
        // ### Section: EVALUATE POSTFIX ###
        erStateEvaluateInfx = evaluatePostfix(postfixQueue, strOutput);
        printErrorCodes(erStateEvaluateInfx);
    }

    return 0;
}