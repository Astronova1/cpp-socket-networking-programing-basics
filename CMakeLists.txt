cmake_minimum_required(VERSION 4.2)
project(cpp_socket_networking_programing_basics)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
include_directories(echo_server)

# 1. Define the Client executable
add_executable(client echo_server/client.cpp)
# Link libraries to client
target_link_libraries(client PRIVATE ws2_32 -static-libgcc -static-libstdc++)

# 2. Define the Server executable
add_executable(server echo_server/server.cpp)
# Link libraries to server (Crucial for networking to work!)
target_link_libraries(server PRIVATE ws2_32 -static-libgcc -static-libstdc++)