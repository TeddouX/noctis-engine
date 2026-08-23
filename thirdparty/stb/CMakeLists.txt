cmake_minimum_required(VERSION 3.10)
project(stb)

add_library(stb STATIC 
    "include/stb/stb_image.h"
    "src/impl.c"
)

target_include_directories(stb 
    PUBLIC 
        "include"
    PRIVATE
        "include/stb"
)