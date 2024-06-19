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

enum processHierarchy {
    MAIN, 
    PREPARE,
    BUILD,
    EXECUTE
};

enum typeHeader {
    OK,
    DONE,
    WAITING,
    FAILED,
    NOTE
};


void printRelationshipState(int processHierarchy, int typeHeader, char * formattedString,...){
    switch(processHierarchy) {
        default:
        case MAIN:
            printf("[Parent]");
            break;
        case PREPARE:
            printf("[Prepar]");
            break;
        case BUILD:
            printf("[Buildr]");
            break;
        case EXECUTE:
            printf("[Excutr]");
            break;
    }

    switch (typeHeader) {
        case OK:
            printf("%s[  OK  ]%s:", F_GREEN, F_NORMAL);
            break;
        case DONE:
            printf("%s[ DONE ]%s:", F_GREEN, F_NORMAL);
            break;
        case WAITING:
            printf("%s[WAITIN]%s", F_YELLOW, F_NORMAL);
            break;
        case FAILED:
            printf("%s[FAILED]%s:", F_RED, F_NORMAL);
            break;
        case NOTE:
            printf("%s[ NOTE ]%s:", F_YELLOW, F_NORMAL);
            break;
    }
    va_list arguments;
    va_start(arguments, formattedString);
    vprintf(formattedString, arguments);
    va_end(arguments);
    
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

    int fileOut = open(outputFileName, O_CREAT | O_TRUNC | O_RDWR, 07777);

    if (fileOut == -1) 
       returnErrorTrace();

    int newFileOut = dup2(fileOut, STDOUT_FILENO);
    close(fileOut);
}

/**
 * Rebuilds the test code and executes it.
 * @param  testType: Type of test
 * @param  *nameFile: Name of testing file
 * @param  *inputFileName: Name of input file
 * @retval None
 */
void prepareTestCase(int testType, char *nameFile, char *inputFileName, int communicatingPipe[]){
    String127 buildcodeFileName = "";
    String127 executingFileName = "";
    int childExitStatus = 0;

    sprintf(buildcodeFileName, "%s.c", nameFile);
    sprintf(executingFileName, "./%s", nameFile);

    printf("[ChildA]%s[EXECUT]%s %s.c\n", F_YELLOW, F_NORMAL, nameFile);
    printf("  Test-Type ID: %d\n", testType);
    printf("  %sChild's  PID%s: %d\n", F_YELLOW, F_NORMAL, getpid());
    printf("  %sParent's PID%s: %d\n", F_YELLOW, F_NORMAL, getppid());

    close(communicatingPipe[0]);
    dup2(communicatingPipe[1], STDERR_FILENO);
    close(communicatingPipe[1]);


    int prepareID = fork();

    if (prepareID == 0){
        printf("[ChildB][REBUILD]: Deleting and Compliling .c code...\n");
        remove(nameFile);
        sleep(1);
        execl("/usr/bin/gcc", "gcc", buildcodeFileName, "-o", nameFile, (char *) NULL);
        returnErrorTrace();
    } else {
        printf("[ChildC][WAITIN]: Waiting for ChildB to finish...\n");
        wait(&childExitStatus);
        
        if (WEXITSTATUS(childExitStatus) == 0) {
            printf("[ChildC]\n");
            printf("Test Type: %d\n", testType);

            if ((testType & IN ) > 0){
                printf("Test's Input: %s\n", inputFileName);
                useStreamInputFile(inputFileName);
            }    

            if ((testType & OUT) > 0){
                printf("Test's Output: %s\n", nameFile);
                createStreamOutputFile(nameFile);
            }
            sleep(1);
            execl(executingFileName, nameFile, (char *) NULL);
            returnErrorTrace();
        }
        exit(-2);
    }
}

/**
 * A struct to store parent-child pid
 * @param initialprocessID : stored process id of a parent or a child
 * @param testStatus : the returned exit result of the child.
 */
struct testID {
    int initialprocessID;
    int testStatus;
};

/**
 * Stores the statistics of executing tests.
 * @param test_processes Stores a test's parent-child interaction
 * @param nTestOrder Current test to execute
 * @param nTestErrors Current number of test errors
 * @param nTestOk Current number of successful tests
 */
struct statsExecuteTest {
    struct testID test_processes[TEST_CASES];
    int failedTests[TEST_CASES];
    int nTestOrder;
    int nTestErrors;
    int nTestOk;
};

/**
 * @brief  
 * @note   
 * @retval 
 */
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

void interpretExecutionResult(struct statsExecuteTest *stats, int resultingProcessID, int *thisTestNumber, int *thisTestChildStatus){
    int hasChildExited = WIFEXITED(*thisTestChildStatus);
    int ChildExitStatus = WEXITSTATUS(*thisTestChildStatus);
    int hasChildSuccessfulDoneTest = ChildExitStatus == 0;

    if (!hasChildExited) {
        return;
    }

    if (hasChildSuccessfulDoneTest) {
        printRelationshipState(MAIN, DONE, "Child Process Done, PID: %d\n", resultingProcessID);
        (stats->nTestOk)++;
        return;
    } 

    printRelationshipState(MAIN, DONE, "PID: %d -> Status code: %d\n", resultingProcessID, ChildExitStatus);
    switch(ChildExitStatus){
        case 255:
            printRelationshipState(MAIN, NOTE, "Result shows SEGFAULT! Check code.\n");
            break;
        case 254:
            printRelationshipState(MAIN, NOTE, "Result shows FAILED GCC! Check filenames\n");
            break;
    }
    stats->failedTests[stats->nTestErrors] = *thisTestNumber;
    (stats->nTestErrors)++;
}

int isThisAChildProcess(int childID){
    return childID == 0;
}

int hasFailedToCreateChild(int childID){
    return childID == -1;
}

void readingIndividualTestStatistics(int communicatingPipe[] ){
    char statisticsString[1024] = "";
    int nReturn = 0;
    do {
        nReturn = read(communicatingPipe[0], statisticsString, 1);
        printf("%s", statisticsString);
        strcpy(statisticsString, "");
    } while (nReturn > 0);
}

/**
 * Prepares an environment for testing 
 * @param  *stats: 
 * @param  TestType: 
 * @param  *fileName: 
 * @param  *inputFileName: 
 * @retval None
 */
void handleTestCase(struct statsExecuteTest *stats, int TestType, char *fileName, char *inputFileName){
    int *thisTestNumber = &stats->nTestOrder;
    // Parent-Child Details
    struct testID *thisTestDetails  = stats->test_processes + *thisTestNumber;
    int *thisTestPID                = &thisTestDetails->initialprocessID;
    int *thisTestChildStatus        = &thisTestDetails->testStatus;


    // Communication for future statistics.
    int communicatingPipe[2];
    pipe(communicatingPipe);

    *thisTestPID = fork();

    // Check if proccess was forked/split into parent and child process successfully.
    if (hasFailedToCreateChild(*thisTestNumber)){
        returnErrorTrace();
    }

    // Make child process do the performance
    if (isThisAChildProcess(*thisTestPID)){
        prepareTestCase(TestType, fileName, inputFileName, communicatingPipe);
    }
    close(communicatingPipe[1]);

    // Parent should read the test statistics.
    readingIndividualTestStatistics(communicatingPipe);
    close(communicatingPipe[0]);

    // Checking of results
    int resultingProcessID = wait(thisTestChildStatus);
    interpretExecutionResult(stats, resultingProcessID, thisTestNumber, thisTestChildStatus);

    (*thisTestNumber)++;
}

/**
 * @brief Executes tests automatically given their test type, name of the file, and any input files.
 * @note Only works with linux-based systems.
 * @param  configNames[][3]: String separated config. First is test type (3 - with input file and output, 2 - with output only, 1 with input only, 0 - none), name of `.c` file, and the name of the input
 * @retval None
 */
void test_controller(String63 configNames[][3]){
    struct testID test_processes[TEST_CASES];
    struct statsExecuteTest stats = initializeExecutionStats();

    printf("[Parent]%s[STARTS]%s: test-controller.c, PID: %d\n", F_GREEN, F_NORMAL, getpid());

    int i = 0;
    for(i = 0; i < TEST_CASES; i++){
        printf("[Parent][ TEST ]: Starting Test %d: %s.c\n", stats.nTestOrder, configNames[i][1]);
        handleTestCase(&stats, atoi(configNames[i][0]), configNames[i][1], configNames[i][2]);
    }
        
    printf("[Parent]%s[ DONE ]%s: All Tests are Complete!\n", F_GREEN, F_NORMAL);
    printf("ALL: %d, DONE: %d, FAILED: %d\n", TEST_CASES, stats.nTestOk, stats.nTestErrors);
    printf("[ Note ]:\n  This file automatically compiles tests within the same folder.\n  May need to change code to separate it per folder.\n"); 
}

int main(){
    signal(SIGSEGV, detectSegfault);
    
    String63 configNames[TEST_CASES][3] = {
        {"2", "t-evaluate-postfix"  , "t-evaluate-postfix-INPUT.txt"},
        {"2", "t-stack"             , ""},
        {"2", "t-queue"             , ""}
    };

    test_controller(configNames);

    return 0;
}