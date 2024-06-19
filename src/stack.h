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

    // LOG(LSTAK, "(STAK) OLD: \"%s\" <- NEW: \"%s\"\n", newNode->element, element);
    strcpy(newNode->element, element);
    // LOG(LSTAK, "       OLD: \"%s\"\n", newNode->element);

    // LOG(LSTAK, "3) %p = ", newNode->prevNode);
    newNode->prevNode = S->top;
    // LOG(LSTAK, "%p\n", S->top);


    newNode->nextNode = NULL;

    if (S->top != NULL)
        S->top->nextNode = newNode;

    S->top = newNode;
}


/**
 * Checks if a stack is empty.
 */
bool isStackEmpty(Stack* S) {
    return S->top == NULL;
}


/**
 * Removes the top element of a stack.
 */
char* pop(Stack* S, char *receivingString) {
    if (isStackEmpty(S))
        strcpy(receivingString, "");
    else if (S->top->prevNode != NULL) {
        strcpy(receivingString, S->top->element);
        S->top = S->top->prevNode;
        free(S->top->nextNode);
        S->top->nextNode = NULL;
    }
    else {
        strcpy(receivingString, S->top->element);
        free(S->top);
        S->top = NULL;
    }
    strcpy(receivingString, S->top->element); // strncat is safer

    /*
    if (S->top->prevNode != NULL) {
        LOG(LSTAK, "STAK A\n");
        S->top = S->top->prevNode;
        free(S->top->nextNode);
        S->top->nextNode = NULL;
    } else {
        LOG(LSTAK, "STAK B\n");
        free(S->top); 
        S->top = NULL;
    }
    */

    return receivingString;
}


/**
 * Returns the top element of the stack.
 */
char* stackTop(Stack* S, char* inputPointer) {
    if (isStackEmpty(S))
        strcpy(inputPointer, "");
    else
        strcpy(inputPointer, S->top->element);
    return inputPointer;
}


/**
 * Frees an entire stack from the heap.
 */
void stackDelete(Stack *S) {
    while (S->top->prevNode != NULL) {
        S->top = S->top->prevNode;
        free(S->top->nextNode);
        S->top->nextNode = NULL;
    }
    free(S->top);
    S->top = NULL;
}


#endif