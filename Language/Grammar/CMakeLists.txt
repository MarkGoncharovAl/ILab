cmake_minimum_required(VERSION 3.12)
project(Grammar VERSION 1.0.0)


add_library( 
    parser STATIC
    parser.hpp
    parser.cpp
)