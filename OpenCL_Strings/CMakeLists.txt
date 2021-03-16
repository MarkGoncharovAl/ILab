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

option(MAIN "adding main" ON)
option(GENERATE "generating tests" OFF)
option(COMPARE "comparing every tests" OFF)
option(COMPARE_PART "comparing every tests" OFF)
#option(TEST "adding google tests" OFF)
#option(E2E "adding full_tests" OFF)

#add_subdirectory(BSort)
add_subdirectory(MC_OpenCL)
add_subdirectory(NativeAlg)
add_subdirectory(NativeAlg_GPU)
add_subdirectory(RabKar)
#add_subdirectory(Common_libs/Random)
#add_subdirectory(Common_libs/Time)

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
        OpenSSL::Crypto
        Boost::program_options
    )
endif(MAIN)

if (GENERATE)
    message("Building generating_tests")
    add_executable(generating generating_tests.cpp)
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
        OpenSSL::Crypto
        Boost::program_options
    )
endif(COMPARE)
if (COMPARE_PART)
    message("Building comparing")
    add_executable(compare_part comparing_part.cpp)
    target_link_libraries (
        compare_part    
    
        MC_OpenCL
        NativeAlg
        NativeAlg_GPU
        RabKar

        ${OpenCL_LIBRARY}
        OpenSSL::Crypto
        Boost::program_options
    )
endif(COMPARE_PART)