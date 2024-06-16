/**
 * Stores stack information.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

#ifndef _STACK_H_
#define _STACK_H_

#define LSTAK ENABLE_LOG_STACK

/**
 * NodeTag is double-linked list.
 */
typedef struct SNodeTag {
    char element[32];
    struct SNodeTag* prevNode;
    struct SNodeTag* nextNode;
} SNode;


/**
 * A Stack is a first in, first out data structure.
 */
typedef struct StackTag {
    SNode* top;
} Stack;


/**
 * Creates an empty stack.
 */
Stack* createStack() {
    Stack* newStack = (Stack*) malloc(sizeof(Stack));
    newStack->top = NULL;
    return newStack;
};


/**
 * Inserts an element into a stack.
 * @param S stack pointer
 * @
 */
void push(Stack* S, char* element) {
    SNode* newNode = (SNode*) malloc(sizeof(SNode));

    LOG(LSTAK, "(STAK) OLD: \"%s\" <- NEW: \"%s\"\n", newNode->element, element);
    strcpy(newNode->element, element);
    LOG(LSTAK, "       OLD: \"%s\"\n", newNode->element);

    LOG(LSTAK, "3) %p = ", newNode->prevNode);
    newNode->prevNode = S->top;
    LOG(LSTAK, "%p\n", S->top);

    newNode->nextNode = NULL;

    if (S->top != NULL)
        S->top->nextNode = newNode;

    S->top = newNode;
}

/**
 * Removes the top element of a stack.
 */
char* pop(Stack* S, char *receivingString) {
    strcat(receivingString, S->top->element); // strncat is safer

    if (S->top->prevNode != NULL){
        S->top = S->top->prevNode;
        free(S->top->nextNode);
        S->top->nextNode = NULL;
    } else {
        free(S->top); 
        S->top = NULL;
    }
    

    return receivingString;
}


/**
 * Returns the top element of the stack.
 */
char* stackTop(Stack* S) {
    return S->top->element;
}


/**
 * Checks if a stack is empty.
 */
bool isStackEmpty(Stack* S) {
    return S->top == NULL;
}



#endif