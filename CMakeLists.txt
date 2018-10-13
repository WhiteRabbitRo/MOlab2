cmake_minimum_required(VERSION 3.12)
project(MOlab2)

set(CMAKE_CXX_STANDARD 14)

add_executable(MOlab2 main.cpp TaskClass.hpp TaskClass.cpp Simplex.hpp Simplex.cpp)