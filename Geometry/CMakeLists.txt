cmake_minimum_required(VERSION 3.10)
project(Main VERSION 1.0.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

option(MAIN "Added main" OFF)
option(TEST "Added GTests" ON)
option(E2E "Added full tests" ON)

find_package(GTest REQUIRED)
  
if (MAIN)
  message("Building main!")
  add_executable(main main.cpp)
  set_property(TARGET main PROPERTY CXX_STANDARD 20)
endif(MAIN)
    
if (TEST)
  message("Building tests!")
  add_executable(test tests.cpp)
  set_property(TARGET test PROPERTY CXX_STANDARD 20)
  target_link_libraries(
    test 
    
    GTest::GTest 
    GTest::Main
  )
endif(TEST)