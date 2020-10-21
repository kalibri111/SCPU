//
// Created by kalibri on 17.10.2020.
//
#ifndef SOFT_PROC_PROC_H
#define SOFT_PROC_PROC_H

#include "../lib/stack.h"
#include "../lib/filestuff.h"
#include <stdarg.h>
#include "commands.h"

typedef struct SoftProc {
    struct stack* stack;
    int rxx[4];  // <- rax, rbx, rcx, rdx
} sproc_t;

sproc_t* new_sproc() {
    sproc_t* new_ = (sproc_t*)calloc(1, sizeof(sproc_t));
    new_->stack = new_stack();
    return new_;
}

void destroy_sproc(sproc_t* sproc) {
    delete_stack(sproc->stack);
    sproc->stack = NULL;
    free(sproc);
    sproc = NULL;
}

int is_no_param_command(int command) {
    return command == POPRAX || command == POPRBX || command == POPRCX || command == POPRDX ||
           command == OUT || command == HALT || command == ADD || command == MUL || command == DIV;
}

int is_one_param_command(int command) {
    return command == PUSH || command == POP ||
           command == RAX || command == RBX || command == RCX || command == RDX;
}

int is_two_param_command(int command) {
//    return command == ADD || command == MUL || command == DIV;
    return 0;
}

void execute(sproc_t* sproc, int command, int params_c, ...) {
    switch (params_c) {
        case 0: {
            if (command == POPRAX) {

                push(sproc->stack, sproc->rxx[0]);

            } else if (command == POPRBX) {

                push(sproc->stack, sproc->rxx[1]);

            } else if (command == POPRCX) {

                push(sproc->stack, sproc->rxx[2]);

            } else if (command == POPRDX) {

                push(sproc->stack, sproc->rxx[3]);

            } else if (command == OUT) {

                printf("%d\n", sproc->stack->top->value);

            } else if (command == HALT) {

                exit(EXIT_SUCCESS);

            } else if (command == ADD) {

                int rhs = pop(sproc->stack);
                int lhs = pop(sproc->stack);

                push(sproc->stack, lhs + rhs);

            } else if (command == MUL) {

                int rhs = pop(sproc->stack);
                int lhs = pop(sproc->stack);

                push(sproc->stack, lhs * rhs);

            } else if (command == DIV) {

                int rhs = pop(sproc->stack);
                int lhs = pop(sproc->stack);

                push(sproc->stack, lhs / rhs);

            }
            break;
        }
        case 1: {
            va_list args;
            va_start(args, params_c);

            if (command == PUSH) {

                push(sproc->stack, va_arg(args, int));

            } else if (command == POP) {

                sproc->rxx[va_arg(args, int)] = pop(sproc->stack);

            } else if (command == RAX) {

                sproc->rxx[0] = va_arg(args, int);

            } else if (command == RBX) {

                sproc->rxx[1] = va_arg(args, int);

            } else if (command == RCX) {

                sproc->rxx[2] = va_arg(args, int);

            } else if (command == RDX) {

                sproc->rxx[3] = va_arg(args, int);

            }

            break;
        }

        default: {
            fprintf(
                    stderr,
                    "invalid params count in file: %s\n"
                    "function: %s\n"
                    "line: %d\n",
                    __FILE__,
                    __FUNCTION__ ,
                    __LINE__
                    );
            exit(EXIT_FAILURE);
        }
    }
}


void __run__(char* bite_code_file) {
    sproc_t* processor = new_sproc();

    FILE* bcode_file = fopen(bite_code_file, "r");

    file_stat* bcode_stat = construct_file_str(bcode_file);

    int command = 0, l_param = 0, r_param = 0;
    for (int i = 0; i < bcode_stat->rows_count; ++i) {

        sscanf(bcode_stat->index[i].str, "%d", &command);

        if (is_no_param_command(command)) {

            sscanf(bcode_stat->index[i].str, "%d", &command);

            execute(processor, command, 0);

        } else if (is_one_param_command(command)) {

            sscanf(bcode_stat->index[i].str, "%d %d", &command, &l_param);

            execute(processor, command, 1, l_param);

        } else if (is_two_param_command(command)) {

            sscanf(bcode_stat->index[i].str, "%d %d %d", &command, &l_param, &r_param);

            execute(processor, command, 2, l_param, r_param);

        } else {

            fprintf(stderr, "processor occurs unknown construction\n");

        }

    }

    destruct_file_str(bcode_stat);
    destroy_sproc(processor);
}

#endif //SOFT_PROC_PROC_H
