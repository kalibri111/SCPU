//
// Created by kalibri on 15.09.2020.
//

#ifndef STACK_STACK_H
#define STACK_STACK_H

#include <stdlib.h>

struct Node {
    int value;
    struct Node* prev_node_;
};

struct stack {
    size_t size;
    struct Node* top;
};

struct stack* new_stack();

void delete_stack(struct stack* stack);

int pop(struct stack* stack);

void push(struct stack* stack, int value);

#endif //STACK_STACK_H
