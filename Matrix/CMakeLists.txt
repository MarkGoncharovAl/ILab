cmake_minimum_required(VERSION 3.12)
project(Main VERSION 1.0.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

option(TEST "Add executable only for tests" OFF)

add_subdirectory(Common_libs)
find_package(GTest REQUIRED)
add_subdirectory(Libraries) 

if (TEST)
  message("Building tests!")
  add_executable(test tests.cpp)
  link_libraries(test iterators)
  target_link_libraries(test Matrix random errors GTest::GTest GTest::Main)
else()
  message("Building main!")
  add_executable(main main.cpp)
  link_libraries(main iterators)
  target_link_libraries(main Matrix random errors)
endif(TEST)