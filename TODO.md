# 2 TODO:
The following are needed to be done:
 - [ ] Complete [stack.c](#stackc)
 - [ ] Complete [queue.c](#queuec)
 - [ ] Complete [evaluate-postfix.c](#)
 - [ ] Fill in [infix.txt](#infixtxt) test
 - [ ] Document in GROUPNAME.PDF


# 3 Brief documentation of each file:
Here is stored the brief summary of each file.

## 3.1 main.c
The file contains only int main() that runs the calculator program. It should depend all other files.

It should scan for user input and output post-fix notation and result in colored text.

## 3.2 stack.c
A stack is a collection of information or objects that has a "last-in, first out" (LIFO) model. 

This file contains a stack struct and its related functions.

## 3.3 queue.c
A queue is a collection of information organized by a "first-in, first-out" (FIFO) model. 

This file contains a queue struct and its related functions.

## 3.4 evaluate-postfix.h
Evaluates postfix notation into an output, should detect errors when it arrises.

Interacts with the queue storing postfix notation.

## 3.5 infix-to-postfix.h
Translates infix notation into postfix notation, and stores it in a queue.

If error arrises, returns one; else, zero.

## 3.6 infix.txt
It contains lines of test cases to be thrown to the program in testing using IO redirection. The result should be stored in result.txt

## 3.7 GROUPNAME.pdf
All necessary documentation is here.

# 4 Diagram of the process:
![diagram](https://github.com/ImaginaryLogs/CCDSALG-S13-MCO1/blob/main/diagram.png)