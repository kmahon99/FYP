cmake_minimum_required(VERSION 3.6)
project(Parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

set(SOURCE_FILES main.cpp parser.cpp parser.h)

add_executable(Parser ${SOURCE_FILES})