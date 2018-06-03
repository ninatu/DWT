cmake_minimum_required(VERSION 3.5)
project(dtw_computation)

set(CMAKE_CXX_STANDARD 14)

add_executable(dtw_computation main.cpp sparce_dtw.cpp fast_dtw.cpp utils.cpp common.cpp)
