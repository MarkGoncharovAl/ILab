cmake_minimum_required(VERSION 3.10)
project(Strings)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3") 

find_package(OpenCL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(GTest REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

option(MAIN "adding main" ON)
option(GENERATE "generating tests" OFF)
option(COMPARE "comparing every tests" OFF)

add_subdirectory(MC_OpenCL)
add_subdirectory(NativeAlg)
add_subdirectory(NativeAlg_GPU)
add_subdirectory(RabKar)
add_subdirectory(Common_libs/Errors)

if (MAIN)
    message("Building main")
    add_executable(main main.cpp)
    target_link_libraries (
        main    
    
        MC_OpenCL
        NativeAlg
        NativeAlg_GPU
        RabKar

        ${OpenCL_LIBRARY}
        Boost::program_options
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        Threads::Threads
        Errors
    )
endif(MAIN)

if (GENERATE)
    message("Building generating_tests")
    add_executable(generating generating_tests.cpp)
    target_link_libraries(
        generating
        Errors
    )
endif(GENERATE)

if (COMPARE)
    message("Building comparing")
    add_executable(compare comparing.cpp)
    target_link_libraries (
        compare    
    
        MC_OpenCL
        NativeAlg
        NativeAlg_GPU
        RabKar

        ${OpenCL_LIBRARY}
        Boost::program_options
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        Threads::Threads
        Errors
    )
endif(COMPARE)