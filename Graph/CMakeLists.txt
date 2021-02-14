cmake_minimum_required(VERSION 3.10)
project(Main VERSION 1.0.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

option(MAIN "Added main" ON)
option(TEST "Added GTests" OFF)
option(E2E "Added full tests" OFF)

find_package(GTest REQUIRED)

add_subdirectory(KGraph) 
add_subdirectory(Common_libs/Random)

if (E2E)
  message("Building e2e_tests!")
  add_executable(e2e_tests TEST_MAIN/check_tests.cpp)
  target_link_libraries(
    e2e_tests
  
    KGraph 
  )
endif(E2E)
  
if (MAIN)
  message("Building main!")
  add_executable(main main.cpp)
  target_link_libraries(
    main
    
    KGraph 
  )
endif(MAIN)
    
if (TEST)
  message("Building tests!")
  add_executable(test Tests.cpp)
  target_link_libraries(
    test 
    
    KGraph 
    Random
    
    GTest::GTest 
    GTest::Main
  )
endif(TEST)