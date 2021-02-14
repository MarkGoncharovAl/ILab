cmake_minimum_required(VERSION 3.13.4)
project(Lib VERSION 1.0.0)

set(iterators iterators.hpp)

add_library(
    errors
    errors.hpp
    errors.cpp
)

add_library(
    random
    random.hpp
    random.cpp
)

add_library(
    time
    time.hpp
    time.cpp
)
