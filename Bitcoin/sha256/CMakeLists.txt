cmake_minimum_required(VERSION 3.10)
project(sha256 VERSION 1.0.0)

add_library(
    ${PROJECT_NAME}
    ${PROJECT_NAME}.hpp
    MySycl.hpp
    ${PROJECT_NAME}.cpp
)