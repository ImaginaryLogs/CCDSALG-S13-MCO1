/**
 * Stores queue information.
*/
typedef struct QNodeTag {
	char data;
	struct QNodeTag* pNext;
} qNode;

/*
	NOTE: In this code, "Tail" is defined as the last element inserted rather than
	the malloc where the next element will be inserted.
*/
typedef struct QueueTag {
	qNode* pHead, pTail;
} queue;

qNode* createNode(char element) {
	qNode* newNode = (qNode*) malloc (sizeof(qNode));
	newNode->key = element;
	newNode->pNext = NULL;
}

queue* createQueue() {
	queue* q = (queue*) malloc (sizeof(queue));
	q->start = q->end = NULL;
	return q;
}

void enqueue(queue* q, char inputChar) {
	qNode* newQNode = createNode(inputChar);
	if (q->pTail == NULL) {
		q->pHead = q->pTail = newQNode;
	}
	q->pTail->pNext = newQNode;
	q->pTail = newQNode;
}

char dequeue(queue* q) {
	if (q->front == NULL)
		return "\0";

	qNode* removeNode = q->pHead;
	char nodeData = q->pHead->data;
	q->pHead = g->pHead->pNext;

	if (q->pHead == NULL)
		q->pTail = NULL;

	free(removeNode);

	return nodeData;
}

char queueHead(queue* q) {
	return q->pHead->data;
}

char queueTail(queue *q) {
	return q->pTail->data;
}

bool queueEmpty(queue *q) {
	if (q->pHead == NULL && q->pTail == NULL)
		return true;
	else
		return false;
}
