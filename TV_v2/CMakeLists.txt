cmake_minimum_required(VERSION 3.17)
project(TV_v0_1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(TV_v0_1
        components.h
        hviewer.h
        main.c
        menu.h
        readtext.h
        scroll.c
        scroll.h
        textProcessing.c
        textProcessing.h
        textViewer.c
        textViewer.h
        tviewer.h)
