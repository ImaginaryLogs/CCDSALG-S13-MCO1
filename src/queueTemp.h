#include "utils.h"

#ifndef _QUEUE_TEMP_H_
#define _QUEUE_TEMP_H_

struct queueNode {
    String7 data;
    struct queueNode *next;
};

struct queueTemp{
    struct queueNode *head;
    struct queueNode *last;
};

#endif
