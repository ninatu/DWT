cmake_minimum_required(VERSION 3.10)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(src main.cpp fast_dtw.cpp utils.cpp common.cpp)