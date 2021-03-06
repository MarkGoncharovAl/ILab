cmake_minimum_required(VERSION 3.10)
project(BitonicSort)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3") 

find_package(OpenCL REQUIRED)
find_package(GTest REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

option(MAIN "adding main" ON)
option(TEST "adding google tests" OFF)
option(E2E "adding full_tests" OFF)

add_subdirectory(BSort)
add_subdirectory(Common_libs/Random)
add_subdirectory(Common_libs/Time)

if (MAIN)
    message("Building main")
    add_executable(main main.cpp)
    target_link_libraries (
        main    
    
        BSort 
 
        Time

        ${OpenCL_LIBRARY}
        Boost::program_options
    )
endif(MAIN)

if (TEST)
    message("Building tests")
    add_executable(tests tests.cpp)
    target_link_libraries (
        tests    
    
        BSort 

        Random
        Time    

        ${OpenCL_LIBRARY}
        GTest::GTest 
        GTest::Main
        Boost::program_options
    )
endif(TEST)

if (E2E)
    message("Building E2E")
    add_executable(e2e TESTS/test_checking.cpp)
    target_link_libraries (
        e2e
    
        BSort 

        Random 
        Time

        ${OpenCL_LIBRARY}
        Boost::program_options
    )
endif(E2E)