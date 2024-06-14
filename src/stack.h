/**
 * Stores stack information.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

#ifndef _STACK_H_
#define _STACK_H_

/**
 * NodeTag is double-linked list.
 */
typedef struct SNodeTag {
    char element[32];
    SNode* prevNode;
    SNode* nextNode;
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
    strcpy(newNode->element, element);
    newNode->prevNode = S->top;
    newNode->nextNode = NULL;

    if (S->top != NULL)
        S->top->nextNode = newNode;
    S->top = newNode;
    free(newNode);
}


/**
 * Removes the top element of a stack.
 */
char* pop(Stack* S) {
    char* value = S->top->element;
    S->top = S->top->prevNode;
    S->top->nextNode = NULL;
    return value;
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