#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define F_RED "\033[0;31m"
#define F_NORMAL "\033[0m"

void detectCrash(int receivingSignal) {
    if (receivingSignal == SIGSEGV){
        printf("%s[ERROR ]%s Segfault happened :(. Check Testcases.\n", F_RED, F_NORMAL);
        exit(-1);
    }
}
int main() {
    signal(SIGSEGV, detectCrash);

    int* p = NULL;
    *p = 5;
    printf("%d\n", *p);
    return 0;
}