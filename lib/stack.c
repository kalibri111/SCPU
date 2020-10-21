//
// Created by kalibri on 15.09.2020.
//

#include <signal.h>
#include "stack.h"


struct stack* new_stack() {
    struct stack* new_stack_ = calloc(1, sizeof(struct stack));
    new_stack_->size = 0;
    new_stack_->top = NULL;
    return new_stack_;
}

void delete_stack(struct stack* stack) {
    int size = stack->size;
    for (int i = 0; i < size; ++i) {
        pop(stack);
    }
}

int pop(struct stack* stack) {
    if (stack->size > 0) {
        --stack->size;
        int to_return = stack->top->value;
        stack->top = stack->top->prev_node_;
        return to_return;
    } else {
        abort();
    }
}

void push(struct stack* stack, int value) {
    struct Node* new_node = calloc(1, sizeof(struct Node));
    new_node->value = value;
    new_node->prev_node_ = stack->top;
    stack->top = new_node;
    ++stack->size;
}
