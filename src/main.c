#include <stdio.h>

int main (){
    /** 
     * ### Detects which OS you are using ### 
     * @ImaginaryLogs uses Ubuntu sometimes to code. 
     * It is assume to be on a Windows Operating System.
     **/ 
    printf("\n[!] Notifications:\n");
    #ifdef _WIN32  
        printf("\tUsing Windows Operating System\n");
        system("echo any text");
    #elif __linux__    
        printf("\tUsing Linux-based System\n");
    #endif

    return 0;
}