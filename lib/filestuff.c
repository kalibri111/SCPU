//
// Created by kalibri on 09.09.2020.
//
#include "filestuff.h"
#include <sys/stat.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>


int getFileSize(FILE* file) {
    struct stat buff = {};
    fstat(fileno(file), &buff);
    return buff.st_size;
}

int evaluateBuffer(char* buffer, size_t size, char orig, char new) {
    int count = 0;
    for (int i = 0; i < size; ++i) {
        if (buffer[i] == orig) {
            ++count;
            buffer[i] = new;
        }
    }
    return count;
}

void destroyBuffer(char* buffer) {
    free(buffer);
}

char* newBufFromFile(FILE* file, size_t buf_size) {
    char* buffer = (char*)calloc(buf_size, sizeof(char));
    fread(buffer, sizeof(char), buf_size, file);
    buffer[buf_size - 1] = '\n';
    return buffer;
}

void indexToFile(const char* file_name, strview_t* index, size_t index_size) {
    FILE* file = fopen(file_name, "a+");
    for (int i = 0; i < index_size; ++i) {
        fputs(index[i].str, file);
        fputc('\n', file);
    }
    fputs("----------------------------------------------\n", file);
    fclose(file);
}

void destroyIndex(strview_t* index) {
    free(index);
}

enum term_type termType(const char* file_buffer, size_t buffer_size) {
    enum term_type term_t;

    int zero_counter = 0;
    for (int i = 0; i < buffer_size; ++i) {
        if (file_buffer[i] == '\0') {
            ++zero_counter;
        } else {
            zero_counter = 0;
        }
    }

    if (zero_counter == 0 || zero_counter > 2) {
        return INVALID;
    }

    if (buffer_size >= 2) {
        if (file_buffer[buffer_size - 1] == '\0' && file_buffer[buffer_size - 2] == '\0') {
            term_t = DOUBLE_ZERO;
        } else if (file_buffer[buffer_size - 1] == '\0') {
            term_t = SINGLE_ZERO;
        } else {
            term_t = INVALID;
        }
    } else {
        if (file_buffer[buffer_size - 1] == '\0') {
            term_t = SINGLE_ZERO;
        } else {
            term_t = INVALID;
        }
    }
    return term_t;
}

strview_t* newIndex(char* file_buffer, size_t index_size, size_t buffer_size) {
    strview_t* index_buffer = malloc(index_size * sizeof(strview_t));

    enum term_type term_t = termType(file_buffer, buffer_size);

    index_buffer[0].str = file_buffer;

    size_t j = 1;

    if (term_t == DOUBLE_ZERO) {
        for (int i = 0; i < buffer_size - 1; ++i) {
            if (file_buffer[i] == '\0' && file_buffer[i + 1] != '\0') {
                index_buffer[j].str = &(file_buffer[i]) + 1;
                index_buffer[j - 1].strlen = index_buffer[j].str - index_buffer[j - 1].str - 1;
                ++j;
            } else if (file_buffer[i] == '\0' && file_buffer[i + 1] == '\0') {
                index_buffer[j - 1].strlen = &file_buffer[i] - index_buffer[j - 1].str;
            }
        }
    } else if (term_t == SINGLE_ZERO) {
        for (int i = 0; i < buffer_size; ++i) {
            if (file_buffer[i] == '\0' && i != buffer_size - 1) {
                index_buffer[j].str = &(file_buffer[i]) + 1;
                index_buffer[j - 1].strlen = index_buffer[j].str - index_buffer[j - 1].str - 1;
                ++j;
            } else if (file_buffer[i] == '\0' && i == buffer_size - 1) {
                index_buffer[j - 1].strlen = &file_buffer[i] - index_buffer[j - 1].str;
            }
        }
    } else if (term_t == INVALID) {
        return NULL;
    }

    return index_buffer;
}

void buffToFile(const char* file_name, char* buffer, size_t buffer_size) {
    FILE* file = fopen(file_name, "a+");
    evaluateBuffer(buffer, buffer_size, '\0', '\n');
    fputs(buffer, file);
    fputs("----------------------------------------------\n", file);
    fclose(file);
}

file_stat* construct_file_str(FILE* input_file) {
    file_stat* new_file_str   = (file_stat*)calloc(1, sizeof(file_stat));

    new_file_str->file_size  = getFileSize(input_file) + 1;  // будем выделять на один символ про запас для \n в конце
    new_file_str->buffer     = newBufFromFile(input_file, new_file_str->file_size);
    new_file_str->rows_count = evaluateBuffer(new_file_str->buffer, new_file_str->file_size, '\n', '\0');
    new_file_str->index      = newIndex(new_file_str->buffer, new_file_str->rows_count, new_file_str->file_size);

    return new_file_str;
}

void destruct_file_str(file_stat* file_str) {
    destroyIndex(file_str->index);
    destroyBuffer(file_str->buffer);
}
