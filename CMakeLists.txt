cmake_minimum_required(VERSION 4.2)
project(cpp_socket_networking_programing_basics)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
include_directories(echo_server)

add_executable(cpp_socket_networking_programing_basics
        echo_server/client.cpp
        echo_server/server.cpp)
add_executable(server
        echo_server/server.cpp)


target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 -static-libgcc -static-libstdc++)