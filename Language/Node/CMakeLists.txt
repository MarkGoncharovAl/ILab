cmake_minimum_required(VERSION 3.12)
project(Node VERSION 1.0.0)

add_library(
    Node STATIC
    Node.hpp
    Node.cpp
)

add_library(
    Symtab STATIC
    Symtab.hpp
    Symtab.cpp
)