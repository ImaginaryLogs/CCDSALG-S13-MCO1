#include "../src/utils.h"

#include <signal.h>
#include <setjmp.h>
#ifdef __linux__
    #include <unistd.h>
#endif

#ifndef _TEST_UTILS_H_
#define _TEST_UTILS_H_

// How much delay there must be for each repeat in a unit test.
#define UNIT_TEST_REPEAT_DELAY (1)
#define LOG_ENABLE_TEST_VERBOSE (0)
#define LTEST LOG_ENABLE_TEST_VERBOSE

/**
 * Reference: https://www.youtube.com/watch?v=IZiUT-ipnj0&list=WL
 * [1] Uses errno from <errno.h> to trace errors in code. Libraries uses errno to store error information. 
 * [2] __FILE__ is a preprocessor macro for the name of the file
 * [3] __LINE__ is also a macro, but the for the line number at which the macro is executed.
 */
#define returnErrorTrace(...) ({\
    fprintf(stdout, "%s[ERROR ]%s %s (LINE: ~%d): %s\n", F_RED, F_NORMAL, __FILE__, __LINE__, strerror(errno));\
    exit(-1);\
})

void detectSegfault(int signal){
    if (signal == SIGSEGV){
        printf("%s[ERROR ]%s Segfault happened :(. Check Testcases.\n", F_RED, F_NORMAL);
        exit(-1);
    }
}

#define MAX_FAILED_TESTS 5

struct testStatistics {
    short currentTestNumber;
    short currentFailureTop;
    short successfulTests;
    short failedTests;
    short failedTestNumber[MAX_FAILED_TESTS];
};


void printTestHeader(int isActualExpected){
    if (isActualExpected == true){
        printf("\n%s#=----= [Example] =----=#%s\n", B_GREEN, F_NORMAL);
    } else {
        printf("\n%s#=----= [Counter] =----=#%s\n", B_YELLOW, F_NORMAL);
    }
}

int assertCaseString(char *Description, char *actualValue, char *expectedValue, int isActualExpected){
    String7 strTruth = "TRUE";
    String7 strFalse = "FALSE";
    int nResult = 0;
    printTestHeader(isActualExpected);
    printf("| \n| %s\n", Description);
    LOG(LTEST, "| \n|  Actual: %s\n", actualValue);
    LOG(LTEST, "|  Expect: %s\n| \n", expectedValue);
    if (actualValue != NULL && expectedValue != NULL){
        nResult = strcmp(actualValue, expectedValue) == 0 ;
    } else {
        nResult = actualValue == expectedValue == isActualExpected ;
    }
    printf("| Result: %s\n| \n", nResult ? strTruth : strFalse);
    return nResult;
}

int assertCaseIntger(char *Description, int actualValue, int expectedValue, int isActualExpected){
    String7 strTruth = "TRUE";
    String7 strFalse = "FALSE";

    printTestHeader(isActualExpected);
    printf("| \n| %s\n", Description);
    LOG(LTEST, "| \n| Actual: %d", actualValue);
    LOG(LTEST, " Expect: %d\n| \n", expectedValue);
    printf("| Result: %s\n| \n", actualValue == expectedValue == isActualExpected ? strTruth : strFalse);
    return actualValue == expectedValue == isActualExpected;
}

int assertCaseChar(char *Description, char actualValue, char expectedValue, int isActualExpected){
    String7 strTruth = "TRUE";
    String7 strFalse = "FALSE";

    printTestHeader(isActualExpected);

    printf("| \n| %s\n", Description);
    LOG(LTEST, "| \n| Actual: %c", actualValue);
    LOG(LTEST, " Expect: %c\n| \n", expectedValue);
    printf("| Result: %s\n| \n", actualValue == expectedValue == isActualExpected ? strTruth : strFalse);
}

/**
 * Initializes statistics for Tests.   
 * @param  *testStats: 
 * @retval None
 */
struct testStatistics createTestStatistics(){
    struct testStatistics testStats;
    testStats.currentFailureTop = 0;
    testStats.currentTestNumber = 1;
    testStats.successfulTests = 0;
    testStats.failedTests = 0;
    int i = 0;
    for(i = 0; i < MAX_FAILED_TESTS; i++){
        testStats.failedTestNumber[i] = 0;
    }
    return testStats;
}

/**
 * Update tests statics.
 * @param  *testStats: 
 * @param  assertReturn: 
 * @retval None
 */
void testCase(struct testStatistics *testStats, int assertReturn){
    if (assertReturn == 0){
        if (testStats->currentFailureTop <= MAX_FAILED_TESTS) {
            testStats->failedTestNumber[testStats->currentFailureTop] = testStats->currentTestNumber;
        } else {
            printf("%s[CRITICAL][FAILED]%s TOO MANY TESTS FAILED, check changes NOW!\n", F_RED, F_NORMAL);
        }
        ++(testStats->failedTests);
        ++(testStats->currentFailureTop);
    } else
        ++(testStats->successfulTests);
    
    printf("#=---= Test No: %02d =---=#\n\n", testStats->currentTestNumber);

    ++(testStats->currentTestNumber);
}

void printTestStatistics(struct testStatistics *testStats){
    int i = 0;
    printf("#=---=[ TESTING STATISTICS ]=---=#\n|\n");
    printf("|  No. of Tests Units: %d\n", testStats->currentTestNumber - 1);
    printf("|  %sSuccess%s  : %d\n", F_GREEN, F_NORMAL, testStats->successfulTests);
    printf("|  %sFailed%s   : %d\n", F_RED, F_NORMAL, testStats->failedTests);
    printf("|  %sCases Failed%s : [ ", F_RED, F_NORMAL);
    for (i = 0; i < MAX_FAILED_TESTS; i++)
       printf("%d ", testStats->failedTestNumber[i]);
    printf("]\n|\n");
    printf("#=------------------------------=#\n");
}



#endif 