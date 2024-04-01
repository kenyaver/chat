cmake_minimum_required(VERSION 2.8.11)
project (onlineList)

set(SOURCE_LIB onlineList.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_LIB})