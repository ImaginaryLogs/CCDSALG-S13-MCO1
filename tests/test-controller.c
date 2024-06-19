#include <errno.h>

#include <limits.h>
#include <fcntl.h>
#include "test-utils.h"
#include <signal.h>
#include <stdarg.h>

#ifdef __linux__
    #include <sys/wait.h>
    #include <unistd.h>
#elif _WIN32
#endif

#define TEST_CASES 2

enum TestType{
    NONE = 0,
    IN,
    OUT = 2
};

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

    //int newFileOut = dup2(fileOut, STDOUT_FILENO);
    close(fileOut);

    int err = execl("./t-queue", "t-queue", (char *) NULL); 
    returnErrorTrace();
}

void useStreamInputFile(char *nameFile){
    int fileIn = open(nameFile, O_RDONLY, 0777);

    if (fileIn == -1) 
        returnErrorTrace();

    int newFileIn  = dup2(fileIn, STDIN_FILENO); 
    close(fileIn);
}

void createStreamOutputFile(char *nameFile){
    String127 outputFileName = "";
    sprintf(outputFileName, "%s-RESULT.txt", nameFile);
    int fileOut = open(outputFileName, O_WRONLY | O_CREAT, 0777);

    if (fileOut == -1) 
       returnErrorTrace();

    int newFileOut = dup2(fileOut, STDOUT_FILENO);
    close(fileOut);
}

void prepareTestCase(int testType, char *nameFile, char *inputFileName){
    String127 buildcodeFileName = "";
    String127 distcodeFileName = "";
    String127 executingFileName = "";
    int childExitStatus = 0;

    sprintf(buildcodeFileName, "%s.c", nameFile);
    sprintf(distcodeFileName, "%s.exe", nameFile);
    sprintf(executingFileName, "./%s", nameFile);

    int prepareID = fork();

    if (prepareID == 0){
        execl("/usr/bin/gcc", "gcc", buildcodeFileName, "-Wall" "-o", distcodeFileName, (char *) NULL);
        returnErrorTrace();
    } else {
        wait(&childExitStatus);

        if (WEXITSTATUS(childExitStatus) && WIFEXITED(childExitStatus)) {
            if (testType & IN > 0)
            useStreamInputFile(inputFileName);

            if (testType & OUT > 0)
                createStreamOutputFile(nameFile);


            execl(executingFileName, nameFile, (char *) NULL);
            returnErrorTrace();
        }
        exit(-2);
    }
}

struct testID {
    int initialprocessID;
    int testStatus;
};

struct statsExecuteTest {
    struct testID test_processes[TEST_CASES];
    int failedTests[TEST_CASES];
    int nTestOrder;
    int nTestErrors;
    int nTestOk;
};

struct statsExecuteTest initializeExecutionStats(){
    struct statsExecuteTest stats;
    int i = 0;

    for (i = 0; i < TEST_CASES; i++){
        stats.test_processes[i].initialprocessID = 0;
        stats.test_processes[i].testStatus = 0;
    }

    for (i = 0; i < TEST_CASES; i++){
        stats.failedTests[i] = 0;
    }
    stats.nTestOk = 0;
    stats.nTestOrder = 0;
    stats.nTestErrors = 0;
}

void handleTestCase(void (*performTestCase)(), struct testID test_processes[], int* testNumber, int *testOK, int *testERROR){
    int thisTestNumber = *testNumber;
    #ifdef __linux__
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
                switch(ChildExitStatus){
                    case 255:
                        printf("[Parent]%s[ NOTE ]%s: Result shows SEGFAULT! Check code.\n", F_RED, F_NORMAL);
                        break;
                    case 244:
                        printf("[Parent]%s[ NOTE ]%s: Result shows FAILED GCC! Check filenames", F_RED, F_NORMAL);
                        break;
                }
                (*testERROR)++;
            }
        }
    #elif _WIN32
        
       
    #endif
    (*testNumber)++;
}
// Everything needed to test the a process

void hanTestCase(struct statsExecuteTest *stats, char *fileName){

}

int main(){
    struct testID test_processes[TEST_CASES];
    struct statsExecuteTest stats = initializeExecutionStats();
    String63 fileNames[TEST_CASES] = {"t-evaluate-postfix", "t-queue", "t-stack"};
    int currentTestID = 0;
    int testOK = 0;
    int testERROR = 0;
    int parentID;
    #if __linux__
        parentID = getpid();
    #else
        parentID = 0;
    #endif
    printf("[Parent]%s[STARTS]%s: test-controller.c, PID: %d\n", F_GREEN, F_NORMAL, parentID);

    //printf("[Parent][ TEST ]: Starting Test 1: t-evaluate-postfix.c\n");
    //handleTestCase(&performTestCase1, test_processes, &currentTestID, &testOK, &testERROR);

    printf("[Parent][ TEST ]: Starting Test 1: t-evaluate-postfix.c\n");
    handTestCase(stats, fileNames[0]);

    // printf("[Parent][ TEST ]: Starting Test 2: t-queue.c\n");
    // handleTestCase(&performTestCase2, test_processes, &currentTestID, &testOK, &testERROR);

    printf("[Parent]%s[ DONE ]%s: All Tests are Complete!\n", F_GREEN, F_NORMAL);
    printf("ALL: %d, DONE: %d, FAILED: %d\n", TEST_CASES, testOK, testERROR);
    printf("[ Note ]:\n\tTo check changes, be sure to compile the test case!\n\tIt does not check for the .c files but the .exe files.\n");
    return 0;    
}