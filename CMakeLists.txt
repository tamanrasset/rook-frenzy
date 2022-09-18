cmake_minimum_required(VERSION 3.0.0)
project(Test VERSION 0.1.0)

set(SOURCES Rook.cpp Board.cpp IoUtils.cpp main.cpp)

set(CMAKE_CXX_STANDARD 17)

# add_definitions(-DENABLE_LOGGING)

add_executable(RookFrenzy  ${SOURCES})
