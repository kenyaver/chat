cmake_minimum_required(VERSION 3.1)
project (chat)

add_definitions(${COMPILER_OPTS})

set(CHAT_SRCS src/server.cpp)
set(CHAT_SRCC src/client.cpp)
set(SERVER server)
set(CLIENT client)

# set(${HEADER} aheader.h check.h command.h function.h offline.h onlineList.h protocol.h session.h timer.h user.h)
set(${LIBS} check/check.cpp command/command.cpp function/function.cpp offline/offline.cpp onliineList/onlineList.cpp protocol/protocol.cpp session/session.cpp 
            timer/timer.cpp user/user.cpp)
set(${LIBC} check/check.cpp function/function.cpp command/command.cpp)

add_executable(${SERVER} ${CHAT_SRCS} ${LIBS})
add_executable(${CLIENT} ${CHAT_SRCC} ${LIBC})