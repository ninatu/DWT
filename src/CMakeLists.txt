cmake_minimum_required(VERSION 3.5)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_executable(src main.cpp fast_dtw.cpp utils.cpp common.cpp)