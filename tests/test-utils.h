#include "../src/utils.h"
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

#ifndef _TEST_UTILS_H_
#define _TEST_UTILS_H_

// How much delay there must be for each repeat in a unit test.
#define UNIT_TEST_REPEAT_DELAY (1)

/**
 * Reference: https://www.youtube.com/watch?v=IZiUT-ipnj0&list=WL
 * [1] Uses errno from <errno.h> to trace errors in code. Libraries uses errno to store error information. 
 * [2] __FILE__ is a preprocessor macro for the name of the file
 * [3] __LINE__ is also a macro, but the for the line number at which the macro is executed.
 */
#define returnErrorTrace(...) ({\
    fprintf(stdout, "%s[ERROR ]%s %s (LINE: ~%d): %s\n", F_RED, F_NORMAL, __FILE__, __LINE__, strerror(errno));\
    exit(1);\
})

void detectSegfault(int signal){
    if (signal == SIGSEGV){
        printf("Segfault happened :(. Check Testcases.\n");
        returnErrorTrace();
    }
}

int assertCaseString(char *Description, char *actualValue, char *expectedValue, int isActualExpected){
    if (isActualExpected == true){
        printf("[CASE - Example]: %s\n", Description);
        printf("\tActual: %s\n", actualValue);
        printf("\tExpect: %s\n", expectedValue);
        printf("\tResult: %s\n", strcmp(actualValue, expectedValue) == 0 ? "TRUE" : "FALSE");
        return strcmp(actualValue, expectedValue) == 0;
    } else {
        printf("[CASE - Counter]: %s\n", Description);
        printf("\tActual: %s\n", actualValue);
        printf("\tExpect: %s\n", expectedValue);
        printf("\tResult: %s\n", strcmp(actualValue, expectedValue) != 0 ? "TRUE" : "FALSE");
    }
}

int assertCaseIntger(char *Description, int actualValue, int expectedValue, int isActualExpected){
    if (isActualExpected == true){
        printf("[CASE - Example]: %s\n", Description);
        printf("\tActual: %d\n", actualValue);
        printf("\tExpect: %d\n", expectedValue);
        printf("\tResult: %d\n", actualValue == expectedValue);
    } else {
        printf("[CASE - Counter]: %s\n", Description);
        printf("\tActual: %d\n", actualValue);
        printf("\tExpect: %d\n", expectedValue);
        printf("\tResult: %d\n", actualValue == expectedValue);
    }
}

int assertCaseChar(char *Description, char actualValue, char expectedValue, int isActualExpected){
    if (isActualExpected == true){
        printf("[CASE - Example]: %s\n", Description);
        printf("\tActual: %c\n", actualValue);
        printf("\tExpect: %c\n", expectedValue);
        printf("\tResult: %s\n", actualValue == expectedValue ? "\033[0;32mTRUE\033[0;0m" : "\033[0;31mFALSE\033[0;0m");
    } else {
        printf("[CASE - Counter]: %s\n", Description);
        printf("\tActual: %c\n", actualValue);
        printf("\tExpect: %c\n", expectedValue);
        printf("\tResult: %s\n", actualValue != expectedValue ? "\033[0;32mTRUE\033[0;0m" : "\033[0;31mFALSE\033[0;0m");
    }
}

/**
 * Generics in C https://www.geeksforgeeks.org/_generic-keyword-c/
 * 
 */
#define assertCase(Description, actualValue, expectedValue, isActualExpected) _Generic((actualValue), \
    char *: assertCaseString(Description, actualValue, expectedValue, isActualExpected),\
    int : assertCaseIntger(Description, actualValue, expectedValue, isActualExpected),\
    char : assertCaseChar(Description, actualValue, expectedValue, isActualExpected),\
    default : assertCaseIntger(Description, actualValue, expectedValue, isActualExpected)\
)
#define TRY do{ jmp_buf ex_buf__; if( !setjmp(ex_buf__) ){
#define CATCH } else {
#define CTRY do{ jmp_buf ex_buf__; switch( setjmp(ex_buf__) ){ case 0: while(1){
#define CCATCH(x) break; case x:
#define FINALLY break; } default:
#define ETRY } }while(0)
#define THROW(x) longjmp(ex_buf__, x)



#endif 