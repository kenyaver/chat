cmake_minimum_required(VERSION 2.8.11)
project (function)

set(SOURCE_LIB function.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_LIB})