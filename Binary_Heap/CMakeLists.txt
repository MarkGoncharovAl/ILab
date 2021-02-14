cmake_minimum_required(VERSION 3.12)
project(Main VERSION 1.0.0)

add_subdirectory(Tests) 
add_subdirectory(/usr/src/googletest gtest)
add_subdirectory(Libraries) 

add_executable(exec.o main.cpp)
link_libraries(exec.o Tree)
target_link_libraries(exec.o tests gtest)