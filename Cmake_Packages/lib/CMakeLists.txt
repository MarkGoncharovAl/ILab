cmake_minimum_required(VERSION 2.8)

set(NAME_LIB Foo)
set(FILE_HPP Foo.hpp)
set(FILE_CPP Foo.cpp)
set(FILE_CONFIG FooConfig.cmake)
set(FOLDER_LIB Foo)

project(${NAME_LIB})

add_library(${NAME_LIB} ${FILE_HPP} ${FILE_CPP})
install(FILES ${FILE_HPP} DESTINATION include)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${FILE_CONFIG} DESTINATION lib/cmake/${FOLDER_LIB})