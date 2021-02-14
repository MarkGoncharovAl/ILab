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
  message("Compiling test!")
  add_executable(test Test/test.cpp)
  target_link_libraries(test base_data random errors time GTest::GTest GTest::Main)
else()
  message("Compiling main!")
  add_executable(main main.cpp)
  target_link_libraries(main base_data random errors time)
endif(TEST)