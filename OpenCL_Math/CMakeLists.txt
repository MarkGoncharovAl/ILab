cmake_minimum_required(VERSION 3.10)
project(Math)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(OpenCL REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(GTest REQUIRED)

add_subdirectory(MC_OpenCL)
add_subdirectory(Libs/Errors)

message("Building main")
set (MAIN_EXEC main)
add_executable(${MAIN_EXEC} main.cpp geometry/geometry.cpp)

message("Building tests")
set (TEST_EXEC test)
add_executable(${TEST_EXEC} test.cpp geometry/geometry.cpp)

set (TARGETS
    ${MAIN_EXEC}
    ${TEST_EXEC}
)

foreach (TNAME ${TARGETS})
    target_compile_features(${TNAME} PRIVATE cxx_std_20)
endforeach()

message("Targeting main")
target_link_libraries (
    ${MAIN_EXEC}
    
    MC_OpenCL
    Errors

    ${OpenCL_LIBRARY}
    Boost::program_options
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
)

message("Targeting tests")
target_link_libraries(
    ${TEST_EXEC}

    MC_OpenCL
    Errors

    ${OpenCL_LIBRARY}
    Boost::program_options
    GTest::GTest 
    GTest::Main
)