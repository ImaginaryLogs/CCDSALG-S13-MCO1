#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

#include <sys/wait.h>
#include "test-utils.h"

int main(){

    int pid = fork();

    if (pid == -1){
        returnErrorTrace();
    }

    if (pid == 0){
        printf("{Child } %s[START ]%s t-evaluate-postfix.h\n", F_YELLOW, F_NORMAL);
        int fileOut = open("t-eval-post-r.txt", O_WRONLY | O_CREAT, 0777);
        int fileIn = open("infix.txt", O_WRONLY, 0777);

        if (fileOut == -1) {
            returnErrorTrace();
        }

        if (fileIn == -1) {
            returnErrorTrace();
        }

        int newFileOut = dup2(fileOut, STDOUT_FILENO);
        int newFileIn = dup2(fileIn, STDIN_FILENO); 
        close(fileOut);
        close(fileIn);

        int err = execl("./t-evaluate-postfix", "t-evaluate-postfix", (char *) NULL); // "t-evaluate-postfix.exe", "t-evaluate-postfix.exe", NULL
        returnErrorTrace();
    } else {
        printf("{Parent} %s[START ]%s test-controller.c\n", F_YELLOW, F_NORMAL);
        int testStatus;
        wait(&testStatus);
        if (WIFEXITED(testStatus)){
            if (WEXITSTATUS(testStatus) == 0){
                printf("Done. \n");
            } else {
                printf("Failure. Status code: %d\n", WEXITSTATUS(testStatus));
            }
        }
        printf("{Parent} %s[DONE  ]%s test-controller.c\n", F_GREEN, F_NORMAL);
    }

    return 0;    
}