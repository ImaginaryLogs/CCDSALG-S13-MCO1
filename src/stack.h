/**
 * Stores stack information.
<<<<<<< Updated upstream
*/
struct stack{
    
=======
 */

#include <stdlib.h>
#include <stdbool.h>


/**
 * 
 */
typedef struct NodeTag {
    char element;
    Node* prevNode;
    Node* nextNode;
} Node;


/**
 * 
 */
typedef struct StackTag {
    Node* top;
} Stack;


/**
 * Creates an empty stack.
 */
void createStack(Stack* S) {
    S = (Stack*) malloc(sizeof(Stack));
    S->top = NULL;
>>>>>>> Stashed changes
};


/**
 * Inserts an element into a stack.
 */
void push(Stack* S, char element) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->element = element;
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
char pop(Stack* S) {
    int value = S->top->element;
    S->top = S->top->prevNode;
    S->top->nextNode = NULL;
    return value;
}


/**
 * Returns the top element of the stack.
 */
char top(Stack* S) {
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