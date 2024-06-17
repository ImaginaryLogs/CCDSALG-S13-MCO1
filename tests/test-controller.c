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

#define TEST_CASES 3

enum TestType{
    NONE,
    IN,
    OUT
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
    int fileOut = open(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, 644);

    if (fileOut == -1) 
       returnErrorTrace();

    int newFileOut = dup2(fileOut, STDOUT_FILENO);
    close(fileOut);
}

void prepareTestCase(int testType, char *nameFile, char *inputFileName){
    String127 buildcodeFileName = "";
    String127 distcodeFileName = "";
    String127 executingFileName = "";
    String127 strAddress = "";
    int childExitStatus = 0;

    sprintf(buildcodeFileName, "%s.c", nameFile);
    sprintf(distcodeFileName, "%s", nameFile);
    sprintf(executingFileName, "./%s", nameFile);


    int prepareID = fork();

    if (prepareID == 0){
        printf("\nBuilding ...\n");
        chmod(executingFileName, 0x0777);
        getcwd(strAddress, 127);
        remove(distcodeFileName);
        sleep(1);
        execl("/usr/bin/gcc", "gcc", buildcodeFileName, "-o", distcodeFileName, (char *) NULL);
        returnErrorTrace();
    } else {
        wait(&childExitStatus);
        
        if (WEXITSTATUS(childExitStatus) == 0) {
            printf("Building complete!\n");
            printf("Test Type: %d\n", testType & OUT > 0);
            if ((testType & IN )> 0){
                printf("Test's Input: %s\n", inputFileName);
                useStreamInputFile(inputFileName);
            }    

            if ((testType & OUT )> 0){
                printf("Test's Output: %s\n", nameFile);
                createStreamOutputFile(nameFile);
            }
            chmod(executingFileName, 0x777);
            sleep(1);
            execl(executingFileName, nameFile, (char *) NULL);
            returnErrorTrace();
        }
        exit(-2);
    }
    exit(0);
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
    return stats;
}


void handleTestCase(struct statsExecuteTest *stats, int TestType, char *fileName, char *inputFileName){
    int *thisTestNumber = &stats->nTestOrder;
    struct testID *thisTestDetails = stats->test_processes + *thisTestNumber;
    char statisticsString[1024] = "";
    #ifdef __linux__
        int communicatingPipe[2];
        pipe(communicatingPipe);

        thisTestDetails->initialprocessID = fork();

        // Check if proccess was forked/split into parent and child process successfully
        if (thisTestDetails->initialprocessID == -1){
            returnErrorTrace();
        }

        // Make child procrss do the performance
        if (thisTestDetails->initialprocessID == 0){
            printf("[Child ]%s[EXECUT]%s %s.c\n", F_YELLOW, F_NORMAL, fileName);
            printf("  Test-Type ID: %d\n", TestType);
            printf("  %sChild's  PID%s: %d\n", F_YELLOW, F_NORMAL, getpid());
            printf("  %sParent's PID%s: %d\n", F_YELLOW, F_NORMAL, getppid());
            close(communicatingPipe[0]);
            dup2(communicatingPipe[1], STDERR_FILENO);
            close(communicatingPipe[1]);
            prepareTestCase(TestType, fileName, inputFileName);
        }
        close(communicatingPipe[1]);
        int nReturn = 0;
        // Parent should wait for child test to finish
        do {
            nReturn = read(communicatingPipe[0], statisticsString, 1);
            printf("%s", statisticsString);
            strcpy(statisticsString, "");
            
        } while (nReturn > 0);
        close(communicatingPipe[0]);
        int resultingProcessID = wait(&thisTestDetails->testStatus);

        // Checking of results
        int hasChildExited = WIFEXITED(thisTestDetails->testStatus);
        int ChildExitStatus = WEXITSTATUS(thisTestDetails->testStatus);
        int hasChildSuccessfulDoneTest = ChildExitStatus == 0;

        if (hasChildExited) {
            if (hasChildSuccessfulDoneTest) {
                printf("\n[Parent]%s[  OK  ]:%s Child Process Done, PID: %d\n", F_GREEN, F_NORMAL, resultingProcessID);
                (stats->nTestOk)++;
            } else {
                printf("[Parent]%s[FAILED]%s: PID: %d -> Status code: %d\n", F_RED, F_NORMAL, resultingProcessID, ChildExitStatus);
                switch(ChildExitStatus){
                    case 255:
                        printf("[Parent]%s[ NOTE ]%s: Result shows SEGFAULT! Check code.\n", F_RED, F_NORMAL);
                        break;
                    case 254:
                        printf("[Parent]%s[ NOTE ]%s: Result shows FAILED GCC! Check filenames\n", F_RED, F_NORMAL);
                        break;
                }
                stats->failedTests[stats->nTestErrors] = *thisTestNumber;
                (stats->nTestErrors)++;
            }
        }

    #elif _WIN32
       
    #endif
    (*thisTestNumber)++;
}

struct executionDetails {
    String63 strFilename;
    String63 strInputname;
    int testType;
};

int main(){
    signal(SIGSEGV, detectSegfault);

    struct testID test_processes[TEST_CASES];
    struct statsExecuteTest stats = initializeExecutionStats();
    String63 fileNames[TEST_CASES] = {"t-evaluate-postfix", "t-queue", "t-stack"};
    
    String63 configNames[TEST_CASES][3] = {
        {"3", "t-evaluate-postfix", "infix.txt"},
        {"2", "t-stack", ""},
        {"2", "t-queue", ""}
    };

    int currentTestID = 0;
    // int testOK = 0;
    // int testERROR = 0;
    int parentID;
    #if __linux__
        parentID = getpid();
    #else
        parentID = 0;
    #endif
    printf("[Parent]%s[STARTS]%s: test-controller.c, PID: %d\n", F_GREEN, F_NORMAL, parentID);


    int i = 0;
    for(i = 0; i < TEST_CASES; i++){
        printf("[Parent][ TEST ]: Starting Test %d: %s.c\n", stats.nTestOrder, configNames[i][1]);
        handleTestCase(&stats, atoi(configNames[i][0]), configNames[i][1], configNames[i][2]);
    }
        

    printf("[Parent]%s[ DONE ]%s: All Tests are Complete!\n", F_GREEN, F_NORMAL);
    printf("ALL: %d, DONE: %d, FAILED: %d\n", TEST_CASES, stats.nTestOk, stats.nTestErrors);
    printf("[ Note ]:\n  This file automatically compiles tests within the same folder.\n  May need to change code to separate it per folder.\n");
    return 0;    
}