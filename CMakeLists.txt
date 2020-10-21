cmake_minimum_required(VERSION 3.17)
project(soft_proc C)

set(CMAKE_C_STANDARD 11)

add_executable(
        soft_proc
        main.c
        compiler/translator.h

        lib/filestuff.h
        lib/filestuff.c
        lib/stack.h
        lib/stack.c

        soft_processor/proc.h
        soft_processor/commands.h
)

set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")