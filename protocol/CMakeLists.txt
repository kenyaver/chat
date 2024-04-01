cmake_minimum_required(VERSION 2.8.11)
project (protocol)

set(SOURCE_LIB protocol.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_LIB})