cmake_minimum_required(VERSION 2.8.11)
project (check)

set(SOURCE_LIB check.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_LIB})