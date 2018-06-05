cmake_minimum_required(VERSION 3.9)
project(CrossSocket)

set(CMAKE_CXX_STANDARD 17)

add_library(CrossSocket SHARED
        src/socket.cpp
        src/socket.hpp
        src/address.cpp
        src/address.hpp)
if(WIN32)
    target_link_libraries(CrossSocket wsock32 ws2_32)
endif(WIN32)