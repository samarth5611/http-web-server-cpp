cmake_minimum_required(VERSION 3.10)

project(http_server)
add_executable(http_server server.cpp httpServer.cpp)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)