//
// Created by kalibri on 17.10.2020.
//
#ifndef SOFT_PROC_TRANSLATOR_H
#define SOFT_PROC_TRANSLATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../lib/filestuff.h"
#include "../soft_processor/commands.h"

int get_command_num(char* command) {
    if (!strcmp(command, "push")) {
        return PUSH;
    } else if (!strcmp(command, "pop")) {
        return POP;
    } else if (!strcmp(command, "add")) {
        return ADD;
    } else if (!strcmp(command, "mul")) {
        return MUL;
    } else if (!strcmp(command, "div")) {
        return DIV;
    } else if (!strcmp(command, "rax")) {
        return RAX;
    } else if (!strcmp(command, "rcx")) {
        return RCX;
    } else if (!strcmp(command, "rdx")) {
        return RDX;
    } else if (!strcmp(command, "rbx")) {
        return RBX;
    } else if (!strcmp(command, "prax")) {
        return POPRAX;
    } else if (!strcmp(command, "prcx")) {
        return POPRCX;
    } else if (!strcmp(command, "prdx")) {
        return POPRDX;
    } else if (!strcmp(command, "prbx")) {
        return POPRBX;
    } else if (!strcmp(command, "out")) {
        return OUT;
    } else if (!strcmp(command, "halt")) {
        return HALT;
    }
    else {
        return -1;
    }
}

char* translate_no_param(strview_t* from) {
    char* to_return = (char*)calloc(20, sizeof(char));
    sprintf(to_return, "%d\n", get_command_num(from->str));
    return to_return;
}

char* translate_one_param(strview_t* from) {
    int   param     = 0;
    char* command   = (char*)calloc(20, sizeof(char));
    char* to_return = (char*)calloc(20, sizeof(char));

    sscanf(from->str, "%s", command);

    for (int i = 0; i < from->strlen; ++i) {
        if (isdigit(from->str[i])) {
            sscanf(&(from->str[i]), "%d", &param);
            break;
        }
    }

    int command_no = get_command_num(command);

    sprintf(to_return, "%d %d\n", command_no, param);

    free(command);

    return to_return;
}

char* translate_two_param(strview_t* from) {
    int   l_param   = 0;
    int   r_param   = 0;
    char* command   = (char*)calloc(20, sizeof(char));
    char* to_return = (char*)calloc(20, sizeof(char));

    sscanf(from->str, "%s", command);

    int pos_first = 0, f_param_len = 0;

    for (int i = 0; i < from->strlen; ++i) {
        if (isdigit(from->str[i])) {
            f_param_len = sscanf(from->str, "%d", &l_param);
            pos_first   = i;
        }
    }

    sscanf(from->str + (pos_first + f_param_len), "%d", &r_param);

    int command_no = get_command_num(command);

    sprintf(to_return, "%d %d %d\n", command_no, l_param, r_param);

    free(command);

    return to_return;
}

void translate(char* file_path) {
    FILE* file = fopen(file_path, "r");

    file_stat* stat = construct_file_str(file);

    char* command       = (char*)calloc(10, sizeof(*command));
    char* output_buffer = (char*)calloc(20 * stat->rows_count, sizeof(char));

    strview_t* cur_row = NULL;


    for (int i = 0; i < stat->rows_count; ++i) {
        sscanf(stat->index[i].str, "%s", command);
        cur_row = &(stat->index[i]);
#define CHECK_COMMAND(str) !strcmp(command, str)
        if (CHECK_COMMAND("push")) {

            strcat(output_buffer, translate_one_param(cur_row));

        } else if (CHECK_COMMAND("pop")) {

            strcat(output_buffer, translate_one_param(cur_row));

        } else if (CHECK_COMMAND("add")) {

            strcat(output_buffer, translate_no_param(cur_row));

        } else if (CHECK_COMMAND("mul")) {

            strcat(output_buffer, translate_no_param(cur_row));

        } else if (CHECK_COMMAND("div")) {

            strcat(output_buffer, translate_no_param(cur_row));

        } else if (CHECK_COMMAND("rax")) {

            strcat(output_buffer, translate_one_param(cur_row));

        } else if (CHECK_COMMAND("rcx")) {

            strcat(output_buffer, translate_one_param(cur_row));

        } else if (CHECK_COMMAND("rdx")) {

            strcat(output_buffer, translate_one_param(cur_row));

        } else if (CHECK_COMMAND("rbx")) {

            strcat(output_buffer, translate_one_param(cur_row));

        } else if (CHECK_COMMAND("prax")) {

            strcat(output_buffer, translate_no_param(cur_row));

        } else if (CHECK_COMMAND("prcx")) {

            strcat(output_buffer, translate_no_param(cur_row));

        } else if (CHECK_COMMAND("prdx")) {

            strcat(output_buffer, translate_no_param(cur_row));

        } else if (CHECK_COMMAND("prbx")) {

            strcat(output_buffer, translate_no_param(cur_row));

        } else if (CHECK_COMMAND("out")) {

            strcat(output_buffer, translate_no_param(cur_row));

        } else if (CHECK_COMMAND("halt")) {

            strcat(output_buffer, translate_no_param(cur_row));

        }
        else {
            fprintf(stderr, "unknown command %s in line %d file %s", command, i, file_path);
        }
#undef CHECK_COMMAND
    }

    FILE* bitecode = fopen("../bitecode.txt", "w");
    fputs(output_buffer, bitecode);
    fclose(bitecode);
    fclose(file);

}

#endif //SOFT_PROC_TRANSLATOR_H
