cmake_minimum_required(VERSION 2.8.11)
project (command)

set(SOURCE_LIB command.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_LIB})