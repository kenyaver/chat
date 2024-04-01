cmake_minimum_required(VERSION 2.8.11)
project (offline)

set(SOURCE_LIB offline.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_LIB})