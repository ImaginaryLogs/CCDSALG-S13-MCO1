#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"

#ifndef _QUEUE_H_
#define _QUEUE_H_

#define LQUE ENABLE_LOG_QUEUE

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

void enqueue(queue* q, char* inputString) {
	qNode* newQNode = createNode(inputString);
	if (q->pTail == NULL) {
		q->pHead = q->pTail = newQNode;
	}
	q->pTail->pNext = newQNode;
	q->pTail = newQNode;
}


char* dequeue(queue* q, char* outputString) {
	if (q != NULL && q->pHead != NULL && q->pTail != NULL){
		qNode* removeNode = q->pHead;
		strcpy(outputString, q->pHead->data);
		q->pHead = q->pHead->pNext;

		if (q->pHead == NULL)
			q->pTail = NULL;

		free(removeNode);
	}
	
	return outputString;
}


char* queueHead(queue* q) {
	return q->pHead->data;
}

char* queueTail(queue *q) {

	return q->pTail->data;
}

bool queueEmpty(queue *q) {
	return q != NULL && (q->pHead == NULL && q->pTail == NULL);
}

void queueDelete(queue *q){
	qNode *previous;
	while (q->pHead != NULL && q->pHead->pNext != NULL) {
		previous = q->pHead;
        q->pHead = q->pHead->pNext;
        free(previous);
        previous = NULL;
        
    }
    free(q->pHead);
    q->pHead = NULL;
}

void queuePrint(queue *q){
	qNode *current;
	LOG(LQUE, "QUEUE: \n");
	if (q != NULL && q->pHead != NULL){
		current = q->pHead;
		LOG(LQUE, "\'%s\'", current->data);
		while (current->pNext != NULL && q->pHead != current->pNext){
			current = current->pNext;
			LOG(LQUE, "%s -> %s\'%s\'", F_RED, F_NORMAL, current->data);
		}
		LOG(LQUE, "\n");
	}
}

void queueToString(queue *q, char *queueExpect) {
	qNode *current;
	if (q != NULL && q->pHead != NULL){
		current = q->pHead;
		LOG(LQUE, "\'%s\'", current->data);
		while (current->pNext != NULL && q->pHead != current->pNext){
			current = current->pNext;
			LOG(LQUE, "%s -> %s\'%s\'", F_RED, F_NORMAL, current->data);
		}
		LOG(LQUE, "\n");
	}
}

#endif