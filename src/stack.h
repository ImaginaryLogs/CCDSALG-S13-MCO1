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
typedef struct NodeTag {
    char element[32];
    struct NodeTag* prevNode;
    struct NodeTag* nextNode;
} Node;


/**
 * A Stack is a first in, first out data structure.
 */
typedef struct StackTag {
    struct NodeTag* top;
} Stack;


/**
 * Creates an empty stack.
 * @param S a pointer containing the stack.
 */
void createStack(Stack* S) {
    S = (Stack*) malloc(sizeof(Stack));
    S->top = NULL;
};


/**
 * Inserts an element into a stack.
 * @param S stack pointer
 * @
 */
void push(Stack* S, char *element) {
    Node* newNode = (Node*) malloc(sizeof(Node));
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
char *pop(Stack* S) {
    char* value = S->top->element;
    S->top = S->top->prevNode;
    S->top->nextNode = NULL;
    return value;
}


/**
 * Returns the top element of the stack.
 */
char *top(Stack* S) {
    return S->top->element;
}


/**
 * Checks if a stack is empty.
 */
bool stackEmpty(Stack* S) {
    return S->top == NULL;
}


/**
 * Checks if a stack is full.
 */
bool stackFull(Stack* S) {

}

#endif