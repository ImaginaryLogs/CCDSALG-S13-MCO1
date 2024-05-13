#include <stdio.h>
#include "utils.h"


int main (){
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

    char input[8];

    while(1){
        repeatGetString(input, 7);
        printf("[%02d]: %s\n", strlen(input), input);
    }

    return 0;
}