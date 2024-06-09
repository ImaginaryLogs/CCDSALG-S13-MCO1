/**
 * Unit testing for stack and its components.
 */
#include "../src/stack.h"
#include "errno.h"
#include "test-utils.h"

int main(){
    Stack *TestStackA;
    Stack *TestStackB;
    String31 testStrings[5] = {
        "Alpha", "Beta", "Charlie", "Delta", "Epsilon"  
    };


    createStack(TestStackA);
    createStack(TestStackB);
    
    printf("Created TestStackA and TestStack B.\n");
    printf("Is Stack Empty? %d\n", stackEmpty(TestStackA));
    
    push(TestStackA, testStrings[0]);
    //returnErrorTrace();
    char *E = top(TestStackA);
    
    printf("[A]: %s\n", E);

    
    


    return 0;
}