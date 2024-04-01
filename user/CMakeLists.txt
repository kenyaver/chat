cmake_minimum_required(VERSION 2.8.11)
project (user)

set(SOURCE_LIB user.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_LIB})