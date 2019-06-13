cmake_minimum_required(VERSION 3.14)
project(DES)

set(CMAKE_CXX_STANDARD 14)

add_executable(DES main.cpp KeyGenerator.cpp KeyGenerator.h overall.h overall.cpp DES.h DES.cpp IOAdapter.cpp IOAdapter.h DESTest.h DESTest.cpp)