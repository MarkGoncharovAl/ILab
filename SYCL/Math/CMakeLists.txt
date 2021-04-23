cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fsycl")

project(Math)
add_subdirectory(geometry)
add_subdirectory(Libs/Logging)
find_package(Logging CONFIG REQUIRED)

set(BIN main)

message("Building main")
add_executable(${BIN} main.cpp)

target_compile_features(${BIN} PRIVATE cxx_std_20)
target_compile_options (${BIN} PRIVATE -O3)
target_link_libraries  (${BIN} PRIVATE geometry Logging_lib)