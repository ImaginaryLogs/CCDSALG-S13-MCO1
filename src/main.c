#include <stdio.h>
#include "evaluate-postfix.h"
#include "infix-to-postfix.h"

int main (){
    String255 inputString = "";
    int errorStateInfix = 0;
    int errorStatePostfix = 0;
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

    while (strcmp(inputString, "quit") != 0){
        // ### Scanning Inputs ###
        repeatGetString(inputString, 255);

        // ### Section: INFIX -> POSTFIX ###
        errorStateInfix = infixToPostfix(inputString);
        
        if (errorStateInfix)
            printf("Syntax Error\n");
        
        // ### Section: EVALUATE POSTFIX ###
        errorStatePostfix = evaluatePostfix();

        switch (errorStatePostfix){
            case 1:
                printf("Division Error\n");
                break;
            case 0:
            default:
                printf("Answer:\n");
        }
    }
        

    return 0;
}