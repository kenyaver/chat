cmake_minimum_required(VERSION 3.1)
project (check)

set(SOURCE_LIB check.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_LIB})
