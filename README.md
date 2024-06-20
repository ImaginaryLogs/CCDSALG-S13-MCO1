# Table of Contents
- [Table of Contents](#table-of-contents)
- [1 Introduction](#1-introduction)
  - [1.1 Background](#11-background)
  - [1.2 Authors](#12-authors)

# 1 Introduction

>[!question]
> "What is the \<Infix-to-Postfix-Calculator\> (MC01)?"

This is a simple calculator that parses in a string with 255 length and outputs it to the console.

## 1.1 Background
This project is created by Group 15 for their MC01 Project in their CCDSALG Class S13. This is under De La Salle University - Manila.

## 1.2 Authors
These are university students of De La Salle University - Manila, Philippines. They are:
- Bunyi, Christian Joseph C. @cjbnyi
- Campo, Roan Cedric V. @ImaginaryLogs
- Chan, Enzo Rafael S. @nomu-chan


# 2 Code Documentation and Description

## Stacks and Queues
The stacks are implemented using doubly-linked lists, whereas the queues are implemented using the singly-linked lists.

For the stacks, they are declared by setting one doubly-linked list node pointer, being top, which pertains the top of the stack.

For the queues, they are declared by setting two singly-linked list node pointers, being the pHead and pTail, which pertains to the head of the queue and tail of the queue, respectively. In our implementation, the tail is defined as the last element inserted rather than the memory allocation where the next element will be inserted. 

## Algorithm Implementation
For our algorithms, we followed the following operator precedence:
- Paretheses ( ), left to right
- Not Operator !, right to left
- Exponent ^, right to left
- Multiplication, Division and Modulo * / %, left to right
- Addition and Subtraction + -, left to right
- Comparison Operators > >= < <=, left to right
- Comparison Operators == !=, left to right
- And Operator &&, left to right
- Or Operator ||, left to right

### 2.1 Infix to postfix
Our first major algorithm is the infix to postfix algorithm, which takes in the ff. inputs:
- The infix string to be converted to postfix.
- A pointer to a queue which will store the postfix tokens
- An operation table containing necessary information about each operator

High-level overview
- Create a stack to keep track of operators.
- Iterate through each token of the infix string.
  - Check if the current token is an operand or an operator.
    - If it's a number/operand, enqueue it to the provided queue.
    - If it's an operator, check which operator it is.
      - If it's an open parenthesis, push it to the operator stack.
      - Else if it's a close parenthesis, pop operations from the operator stack and enqueue them to the postfix queue until an open parenthesis is encountered.
      - Else, pop operations from the operator stack while the operator at the top of stack has a higher precedence than the current token.
- Finally, pop operations from the operator stack and enqueue them to the postfix queue while there are any remaining operators in the operator stack.

By the end of the infix to postfix algorithm, the provided pointer to queue must already point to a queue that contains the tokens of the converted postfix expression in order. This queue will eventually be passed in to the evaluate postfix function, which is essentially an implementation of the Shunting yard algorithm.

### 2.2 Shunting yard algorithm
Our second major algorithm is the shunting yard algorithm, which takes in the ff. inputs:
- A pointer to a queue of postfix tokens
- A pointer to a string holding the evaluated answer
- An operation table containing necessary information about each operator

High-level overview
- Create a stack to maintain operands.
- Iterate through each token in the queue of postfix tokens.
  - Check if the current token is an operand or an operator.
    - If the current token is an operand, push it to the stack maintaining operands.
    - Else if the current token is operator, pop the two operands

By the end of the shunting yard algorithm, the queue of postfix tokens will have been have evaluated as a postfix expression. The corresponding answer must then be stored in the string.

Do note that the two provided algorithm overviews are very high-level. In the program implementation, there are many things happening under the hood, such as handling potential errors, utilizing helper functions, etc. We only expounded on the major algorithms that fulfill the main goal of the program, but they are in fact composed of many more functions with their own algorithms.

## Code Limitations and Errors
We have addressed some of the errors that can be made when inputting a string of operators and operands. In fact, we have also addressed any other incorrect string input that can be placed by the user. 

For the errors when inputting a string of operators and operands, we have addressed the following errors:
- Mismatched Parentheses, which occurs when there are either incorrectly placed parentheses or if there is an unequal number of right and left parentheses.
- Malformed expressions, cause by incorrectly placed operators.
- Division by 0.
- 0 ^ 0 expression.

As for the general case of an incorrect string input, our code also detects incorrect characters, empty inputs, unrecognizable operators, and character overflow (if the input has more than 255 characters).

Lastly, though this is not a limitation of our code itself, but a limitation of C datatypes, the code cannot handle exceedingly large integer/operand inputs.
