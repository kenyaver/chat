cmake_minimum_required(VERSION 2.8.11)
project (session)

set(SOURCE_LIB session.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_LIB})