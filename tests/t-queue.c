#include "../src/queue.h"
#include "errno.h"
#include "test-utils.h"

#define queueCharacters 5

int main(){
    signal(SIGSEGV, detectSegfault);
    int i = 0;
    queue *testQueueA = createQueue();
    queue *testQueueB = createQueue();
    char charInput[queueCharacters] = {};

    // Create Test inputs
    for (i = 0; i < queueCharacters; i++){
        charInput[i] = 'A' + i; 
    }

    for (i = 0; i < 5; i++){
        enqueue(testQueueA, charInput[i]);
    }

    assertCaseChar("[A]: Get Queue Head", queueHead(testQueueA), charInput[0], true);
    assertCaseChar("[A]: Get Queue Tail", queueTail(testQueueA), charInput[queueCharacters - 1], true);

    for (i = 0; i < 5; i++){
        dequeue(testQueueA);
    }

    assertCaseChar("[A]: Get Queue Head", queueHead(testQueueA), charInput[0], false);
    assertCaseChar("[A]: Get Queue Tail", queueTail(testQueueA), charInput[queueCharacters - 1], false);
    exit(1);
    return 0;
}