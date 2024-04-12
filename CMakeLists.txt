cmake_minimum_required(VERSION 3.27)
project(library_management_system C)

set(CMAKE_C_STANDARD 11)

add_executable(library_management_system main.c
        loginandcommands.h
        loginandcommands.c
        person.h
        person.c
        library.h
        library.c
        menu.h
        menu.c
        path.h
        path.c)
