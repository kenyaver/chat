cmake_minimum_required(VERSION 3.1)
project (command)

set(SOURCE_LIB command.cpp)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} STATIC ${SOURCE_LIB})
