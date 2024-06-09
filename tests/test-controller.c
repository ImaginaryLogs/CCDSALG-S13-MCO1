#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include "test-utils.h"

#define TEST_CASES 2



void performTestCase1(){

    int fileOut = open("t-eval-post-r.txt", O_WRONLY | O_CREAT, 0777);
    int fileIn = open("infix.txt", O_RDONLY, 0777);
    
    if (fileOut == -1) 
        returnErrorTrace();
    if (fileIn == -1) 
        returnErrorTrace();
    
    int newFileOut = dup2(fileOut, STDOUT_FILENO);
    int newFileIn  = dup2(fileIn, STDIN_FILENO); 
    close(fileOut);
    close(fileIn);

    int err = execl("./t-evaluate-postfix", "t-evaluate-postfix", (char *) NULL); // "t-evaluate-postfix.exe", "t-evaluate-postfix.exe", NULL
    returnErrorTrace();
}

void performTestCase2(){


    
    int fileOut = open("t-queue-r.txt", O_WRONLY | O_CREAT, 0777);

    if (fileOut == -1) 
       returnErrorTrace();

    int newFileOut = dup2(fileOut, STDOUT_FILENO);
    close(fileOut);

    int err = execl("./t-queue", "t-queue", (char *) NULL); 
    returnErrorTrace();
}

struct testID {
    int initialprocessID;
    int testStatus;
};

void prepareTestCase(void ( *performTestCase)(), struct testID test_processes[], int* testNumber, int *testOK, int *testERROR){
    int thisTestNumber = *testNumber;

    (test_processes + thisTestNumber)->initialprocessID = fork();

    // Check if proccess was forked/split into parent and child process successfully
    if ((test_processes + thisTestNumber)->initialprocessID == -1){
        returnErrorTrace();
    }

    // Make child procrss do the performance
    if ((test_processes + thisTestNumber)->initialprocessID == 0){
        printf("[Child ]%s[EXECUT]%s t-stack.h\n", F_YELLOW, F_NORMAL);
        printf("\tProcess ID: %d, \n\tParent Process ID: %d\n", getpid(), getppid());
        performTestCase();
    }
    
    // Parent should wait for child test to finish
    int resultingProcessID = wait((test_processes + thisTestNumber)->testStatus);

    // Checking of result
    int hasChildExited = WIFEXITED((test_processes + thisTestNumber)->testStatus);
    int ChildExitStatus = WEXITSTATUS((test_processes + thisTestNumber)->testStatus);
    int hasChildSuccessfulDoneTest = ChildExitStatus == 0;

    if (hasChildExited) {
        if (hasChildSuccessfulDoneTest) {
            printf("\n[Parent]%s[  OK  ]:%s Child Process Done, PID: %d\n", F_GREEN, F_NORMAL, resultingProcessID);
            (*testOK)++;
        } else {
            printf("[Parent]%s[FAILED]. Status code: %d\n", F_RED, ChildExitStatus);
            (*testERROR)++;
        }
    } 
}
// Everything needed to test the a process


int main(){
    struct testID test_processes[TEST_CASES];
    int currentTestID = 0;
    int processIDs[TEST_CASES] = {0};
    int testStatus[TEST_CASES] = {0};
    int resultingProcessID[TEST_CASES] = {0};
    int testOK = 0;
    int testERROR = 0;

    printf("[Parent]%s[START ]%s test-controller.c, PID: %d\n", F_GREEN, F_NORMAL, getpid());


    prepareTestCase(&performTestCase1, test_processes, &currentTestID, &testOK, &testERROR);
    // processIDs[0] = fork();
    // if (processIDs[0] == -1){
    //     returnErrorTrace();
    // }

    // if (processIDs[0] == 0){
    //     performTestCase1();
    //     return 0;
    // }
    

    // resultingProcessID[0] = wait(&testStatus[0]);
    // if (WIFEXITED(testStatus[0]) && WEXITSTATUS(testStatus[0]) == 0){
    //     printf("\n[Parent]%s[  OK  ]:%s Child Process Done, PID: %d\n", F_GREEN, F_NORMAL, resultingProcessID[0]);
    //     testOK++;
    // } else if (WIFEXITED(testStatus[0])) {
    //     printf("[Parent]%s[FAILED]. Status code: %d\n", F_RED, WEXITSTATUS(testStatus[0]));
    //     testERROR++;
    // }

    printf("[Parent][TEST  ]: Starting Test 2: t-queue.c\n");
    
    
    processIDs[1] = fork();
    if (processIDs[1] == -1){
        returnErrorTrace();
    }

    if (processIDs[1] == 0){
        performTestCase2();
        return 0;
    }

    
    resultingProcessID[1] = wait(testStatus + 1);

    if (WIFEXITED(testStatus[1]) ){
        if (WEXITSTATUS(testStatus[1]) == 0) {
            printf("[Child ]%s[DONE  ]%s Child Process Done, PID: %d\n", F_GREEN, F_NORMAL, resultingProcessID[1]);
            testOK++;
        } else {
            printf("[Child ]%s[FAILED]%s PID: %d, Status code: %d\n", F_RED, F_NORMAL, resultingProcessID[1], WEXITSTATUS(testStatus[1]));
            testERROR++;
        }
    }


    printf("[Parent]%s[DONE  ]%s All Tests are Complete!\n", F_GREEN, F_NORMAL);
    printf("ALL: %d, DONE: %d, FAILED: %d\n", TEST_CASES, testOK, testERROR);
    return 0;    
}