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

add_subdirectory(BSort)
add_subdirectory(Common_libs/Errors)
add_subdirectory(Common_libs/Random)
add_subdirectory(Common_libs/Time)

add_executable(main main.cpp)
add_executable(tests tests.cpp)
add_executable(full_tests TESTS/test_checking.cpp)

target_link_libraries (
    main    
    
    BSort 

    Errors 
    Time

    ${OpenCL_LIBRARY}
)

target_link_libraries (
    tests    
    
    BSort 

    Errors 
    Random
    Time

    ${OpenCL_LIBRARY}
    GTest::GTest 
    GTest::Main
)

target_link_libraries (
    full_tests
    
    BSort 

    Errors
    Random 
    Time

    ${OpenCL_LIBRARY}
)