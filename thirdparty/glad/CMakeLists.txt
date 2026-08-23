cmake_minimum_required(VERSION 3.10)
project(Glad)

add_library(Glad
    "src/gl.c"
)

target_include_directories(Glad PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
