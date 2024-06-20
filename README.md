# Table of Contents
- [Table of Contents](#table-of-contents)
- [1 Introduction](#1-introduction)
  - [1.1 Background](#11-background)
  - [1.2 Authors](#12-authors)

# 1 Introduction

>[!question]
> "What is the \<TITLE\> (MC01)?"

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
For our algorithm, we first followed the following operator precedence:
- Paretheses ( ), left to right
- Not Operator !, right to left
- Exponent ^, right to left
- Multiplication, Division and Modulo * / %, left to right
- Addition and Subtraction + -, left to right
- Comparison Operators > >= < <=, left to right
- Comparison Operators == !=, left to right
- And Operator &&, left to right
- Or Operator ||, left to right

then... insert how code actu works???

## Code Limitations and Errors
We have addressed some of the errors that can be made when inputting a string of operators and operands. In fact, we have also addressed any other incorrect string input that can be placed by the user. 

For the errors when inputting a string of operators and operands, we have addressed the following errors:
- Mismatched Parentheses, which occurs when there are either incorrectly placed parentheses or if there is an unequal number of right and left parentheses.
- Malformed expressions, cause by incorrectly placed operators.
- Division by 0.
- 0 ^ 0 expression.

As for the general case of an incorrect string input, our code also detects incorrect characters and character overflow (if the input has more than 255 characters).

Lastly, though this is not a limitation of our code itself, but a limitation of C datatypes, the code cannot handle exceedingly large integer/operand inputs.
