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

void stackTopInspect(Stack *S) {
    if (S != NULL && S->top != NULL){
        if(S->top->prevNode != NULL && S->top->prevNode->prevNode != NULL){
            LOG(LSTAK, "PREV: \"%s\" <-", S->top->prevNode->prevNode->element);
        }
        if(S->top->prevNode != NULL){
            LOG(LSTAK, "PREV: \"%s\" <-", S->top->prevNode->element);
        }
        LOG(LSTAK, " TOP: \"%s\" ",  S->top->element);
        if (S->top->nextNode != NULL){
            LOG(LSTAK, "-> NEXT: %s\n", S->top->nextNode->element);
        }
    }
}


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
    stackTopInspect(S);
    
}


void stackPrint(Stack *S){
    SNode *E = S->top;
    LOG(LSTAK, "Stack: ");
    while (E->prevNode != NULL){
        LOG(LSTAK, "%s -> ", E->element);
        E = E->prevNode;
    }
    if (E != NULL){
        LOG(LSTAK, "Last: %s\n", E->element);
    } else {
        LOG(LSTAK, "NULL!\n");
    }
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
    if (isStackEmpty(S)) {
        strcpy(receivingString, "");
    } else if (S->top->prevNode != NULL) {
        strcpy(receivingString, S->top->element); // strncat is safer
        S->top = S->top->prevNode;
        free(S->top->nextNode);
        S->top->nextNode = NULL;
    } else {
        strcpy(receivingString, S->top->element); // strncat is safer
        free(S->top);
        S->top = NULL;
    }
    
    return receivingString;
}


/**
 * Returns the top element of the stack.
 */
char* stackTop(Stack* S, char* inputPointer) {
    if (!isStackEmpty(S)){
        strcpy(inputPointer, S->top->element);
    } else {
        strcpy(inputPointer, "");
    }
    return inputPointer;
}





/**
 * Frees an entire stack from the heap.
 */
void stackDelete(Stack *S) {
    while (S->top != NULL && S->top->prevNode != NULL) {
        S->top = S->top->prevNode;
        free(S->top->nextNode);
        S->top->nextNode = NULL;
        //printf("%s<- %s ->%s\n", S->top->prevNode->element, S->top->element, S->top->nextNode->element);
    }
    if (S->top != NULL)
        free(S->top);
}


#endif