cmake_minimum_required(VERSION 3.16.2)

project(stb)

add_library(${PROJECT_NAME}
    "stb_image.h"
    "stb_image.c"
)
