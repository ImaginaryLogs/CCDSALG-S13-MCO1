#include "../src/queue.h"
#include "test-utils.h"

#define queueCharacters 5

int main(){
    signal(SIGSEGV, detectSegfault);
    int i = 0;
    struct testStatistics ts = createTestStatistics();
    queue *testQueueA = createQueue();
    queue *testQueueB = createQueue();
    String127 strTestInput[queueCharacters] = {};
    String7 tempString = "";
    // Create Test inputs
    for (i = 0; i < queueCharacters; i++){
        strTestInput[i][0] = 'A' + i;
        sprintf(tempString, "%d", i);
        strcat(*strTestInput, tempString);
    }

    for (i = 0; i < 5; i++){
        enqueue(testQueueA, strTestInput[i]);
    }

    testCase(&ts, assertCaseString("Get Queue Head", queueHead(testQueueA), strTestInput[0], true));
    testCase(&ts, assertCaseString("Get Queue Tail", queueTail(testQueueA), strTestInput[queueCharacters - 1], true));

    for (i = 0; i < 5; i++){
        dequeue(testQueueA);
    }

    testCase(&ts, assertCaseString("Get Queue Head", queueHead(testQueueA), strTestInput[0], false));
    testCase(&ts, assertCaseString("Get Queue Tail", queueTail(testQueueA), strTestInput[queueCharacters - 1], false));
    printTestStatistics(&ts);

    close(STDERR_FILENO);
    return 0;
}