#include "../src/queue.h"
#include "test-utils.h"

#define queueCharacters 5

int main(){
    signal(SIGSEGV, detectSegfault);
    int i = 0;
    struct testStatistics ts = createTestStatistics();
    queue *testQueueA = createQueue();
    String127 strTestInput[queueCharacters] = {};
    String127 strTestHolder[queueCharacters] = {};
    String7 tempString = "";
    // Create Test inputs
    for (i = 0; i < queueCharacters; i++){
        strTestInput[i][0] = 'A' + i;
        sprintf(tempString, "%d", i);
        strcat(strTestInput[i], tempString);
    }

    for (i = 0; i < 5; i++){
        enqueue(testQueueA, strTestInput[i]);
    }

    testCase(&ts, assertCaseString("Get Queue Head, is it same as testInput?", queueHead(testQueueA), strTestInput[0], true));
    testCase(&ts, assertCaseString("Get Queue Tail, is it same as testInput?", queueTail(testQueueA), strTestInput[queueCharacters - 1], true));
    testCase(&ts, assertCaseInteger("Is the queue NOT empty?", queueEmpty(testQueueA), true, false));

    for (i = 0; i < 5; i++){
        
        dequeue(testQueueA, strTestHolder[i]);
        printf("Popped %d entry of %s from queue.\n", i, strTestHolder[i]);
    }

    testCase(&ts, assertCaseString("Get Queue Head, is it empty?", queueHead(testQueueA), strTestInput[0], false));
    testCase(&ts, assertCaseString("Get Queue Tail, is it empty?", queueTail(testQueueA), strTestInput[queueCharacters - 1], false));
    testCase(&ts, assertCaseString("Is first pop in holder same as test input?", strTestHolder[0], strTestInput[0], true));
    testCase(&ts, assertCaseString("Is last pop in holder same as test input?", strTestHolder[queueCharacters - 1], strTestInput[queueCharacters - 1], true));
    testCase(&ts, assertCaseInteger("Is the queue empty?", queueEmpty(testQueueA), true, true));

    queueDelete(testQueueA);
    testCase(&ts, assertCaseInteger("Is the queue deleted?", queueEmpty(testQueueA), true, true));
    testCase(&ts, assertCaseInteger("Is the queue head empty?", testQueueA->pHead == 0, true, true));
    testCase(&ts, assertCaseInteger("Is the queue tail empty?", testQueueA->pTail == 0, true, true));
    printTestStatistics(&ts);

    close(STDERR_FILENO);
    return 0;
}