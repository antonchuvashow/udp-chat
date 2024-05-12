cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 17)

project(chat)
add_executable(chat main.cpp
        utils.cpp
        Client.cpp
        Server.cpp)