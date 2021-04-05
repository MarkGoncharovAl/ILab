cmake_minimum_required(VERSION 3.10)
project(Using_Foo)

find_package(Foo CONFIG REQUIRED)

set(MAIN_EXEC main)
add_executable(${MAIN_EXEC} main.cpp)

target_link_libraries(${MAIN_EXEC} PRIVATE ${FOO_LIBRARY_PATH})