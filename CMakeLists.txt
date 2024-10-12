cmake_minimum_required(VERSION 3.29)
project(Extended_Shapes_Board)

set(CMAKE_CXX_STANDARD 20)

add_executable(Extended_Shapes_Board main.cpp
        UIKernel.cpp
        BlackBoard.cpp
        Shape.cpp
        FileHandler.cpp)
