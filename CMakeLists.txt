cmake_minimum_required(VERSION 2.8.11)
project (chat)

add_definitions(${COMPILER_OPTS})

set(CHAT_SRCS src/server.cpp)
# set(${HEADER} aheader.h check.h command.h function.h offline.h onlineList.h protocol.h session.h timer.h user.h)
# set(${SOURCE} check.cpp command.cpp function.cpp offline.cpp onlineList.cpp protocol.cpp session.cpp timer.cpp user.cpp)

include_directories(libs)

add_executable(${PROJECT_NAME} ${CHAT_SRCS})

add_subdirectory(libs)

target_link_libraries(${PROJECT_NAME} libs)