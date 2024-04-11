cmake_minimum_required(VERSION 3.1)
project (protocol)

set(SOURCE_LIB protocol.cpp)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} STATIC ${SOURCE_LIB})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIR})
