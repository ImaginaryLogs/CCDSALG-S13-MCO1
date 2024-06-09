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

void prepareTestCase(void (*performTestCase)(), struct testID test_processes[], int* testNumber, int *testOK, int *testERROR){
    int thisTestNumber = *testNumber;

    (test_processes + thisTestNumber)->initialprocessID = fork();

    // Check if proccess was forked/split into parent and child process successfully
    if ((test_processes + thisTestNumber)->initialprocessID == -1){
        returnErrorTrace();
    }

    // Make child procrss do the performance
    if ((test_processes + thisTestNumber)->initialprocessID == 0){
        printf("[Child ]%s[EXECUT]%s t-stack.h\n", F_YELLOW, F_NORMAL);
        printf("\t%sChild  PID%s: %d\n", F_YELLOW, F_NORMAL, getpid());
        printf("\t%sParent PID%s: %d\n", F_YELLOW, F_NORMAL, getppid());
        performTestCase();
    }
    
    // Parent should wait for child test to finish
    int resultingProcessID = wait(&(test_processes + thisTestNumber)->testStatus);

    // Checking of results
    int hasChildExited = WIFEXITED((test_processes + thisTestNumber)->testStatus);
    int ChildExitStatus = WEXITSTATUS((test_processes + thisTestNumber)->testStatus);
    int hasChildSuccessfulDoneTest = ChildExitStatus == 0;

    if (hasChildExited) {
        if (hasChildSuccessfulDoneTest) {
            printf("\n[Parent]%s[  OK  ]:%s Child Process Done, PID: %d\n", F_GREEN, F_NORMAL, resultingProcessID);
            (*testOK)++;
        } else {
            printf("[Parent]%s[FAILED]%s: PID: %d -> Status code: %d\n", F_RED, F_NORMAL, resultingProcessID, ChildExitStatus);
            if (ChildExitStatus == 255){
                printf("[Parent]%s[ NOTE ]%s: Result shows SEGFAULT! Check code.\n", F_RED, F_NORMAL);
            }
            (*testERROR)++;
        }
    }

    (*testNumber)++;
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

    printf("[Parent][ TEST ]: Starting Test 1: t-evaluate-postfix.c\n");
    prepareTestCase(&performTestCase1, test_processes, &currentTestID, &testOK, &testERROR);

    printf("[Parent][ TEST ]: Starting Test 2: t-queue.c\n");
    prepareTestCase(&performTestCase2, test_processes, &currentTestID, &testOK, &testERROR);

    printf("[Parent]%s[ DONE ]%s All Tests are Complete!\n", F_GREEN, F_NORMAL);
    printf("ALL: %d, DONE: %d, FAILED: %d\n", TEST_CASES, testOK, testERROR);
    printf("[ Note ]\tTo check changes, be sure to compile the test case!\n\tIt does not check for the .c files but the .exe files.\n");
    return 0;    
}