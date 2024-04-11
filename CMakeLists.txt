cmake_minimum_required(VERSION 3.1)
project (chat)

add_definitions(${COMPILER_OPTS})

set(CHAT_SRCS src/server.cpp)
set(CHAT_SRCC src/client.cpp)
set(SERVER server)
set(CLIENT client)

# set(${HEADER} aheader.h check.h command.h function.h offline.h onlineList.h protocol.h session.h timer.h user.h)
set(${LIBS} check.cpp command.cpp function.cpp offline.cpp onlineList.cpp protocol.cpp session.cpp timer.cpp user.cpp)
set(${LIBC} check.cpp function.cpp command.cpp)

add_executable(${SERVER} ${CHAT_SRCS} ${LIBS})
add_executable(${CLIENT} ${CHAT_SRCC} ${LIBC})