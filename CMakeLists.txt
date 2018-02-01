cmake_minimum_required(VERSION 3.9)
project(Socket)

set(CMAKE_CXX_STANDARD 17)

add_library(Socket SHARED src/socket.cpp src/socket.hpp src/address.cpp src/address.hpp)
if(WIN32)
    target_link_libraries(Socket wsock32 ws2_32)
endif(WIN32)