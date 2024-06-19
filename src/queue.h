#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#ifndef _QUEUE_H_
#define _QUEUE_H_
/**
 * Stores queue information.
*/
typedef struct QNodeTag {
	char data[32];
	struct QNodeTag* pNext;
} qNode;

/*
	NOTE: In this code, "Tail" is defined as the last element inserted rather than
	the malloc where the next element will be inserted.
*/
typedef struct QueueTag {
	qNode* pHead;
	qNode* pTail;
} queue;

qNode* createNode(char* element) {
	qNode* newNode = (qNode*) malloc(sizeof(qNode));
	strcpy(newNode->data, element);
	newNode->pNext = NULL;
	return newNode;
}

queue* createQueue() {
	queue* q = (queue*) malloc (sizeof(queue));
	q->pHead = NULL;
	q->pTail = NULL;
	return q;
}

void enqueue(queue* q, char* inputChar) {
	qNode* newQNode = createNode(inputChar);
	if (q->pTail == NULL) {
		q->pHead = q->pTail = newQNode;
	}
	q->pTail->pNext = newQNode;
	q->pTail = newQNode;
}

char *dequeue(queue* q) {
	if (q->pHead == NULL)
		return '\0';

	qNode* removeNode = q->pHead;
	char *nodeData = q->pHead->data;
	q->pHead = q->pHead->pNext;

	if (q->pHead == NULL)
		q->pTail = NULL;

	free(removeNode);

	return nodeData;
}

char *queueHead(queue* q) {
	return q->pHead->data;
}

char *queueTail(queue *q) {
	return q->pTail->data;
}

bool queueEmpty(queue *q) {
	if (q->pHead == NULL && q->pTail == NULL)
		return true;
	else
		return false;
}



#endif