cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fsycl -DTRACE")

project(Bitcoin)

find_package(GTest REQUIRED)
add_subdirectory(Libs/Logging)
add_subdirectory(sha256)

set(MAIN_EXEC main)
set(TEST_EXEC gtests)

message("Building main")
add_executable(${MAIN_EXEC} main.cpp)
add_executable(${TEST_EXEC} gtests.cpp)

set (
    TARGETS
    ${MAIN_EXEC}
    ${TEST_EXEC}
)

foreach(TNAME ${TARGETS})
    target_compile_features(${TNAME} PRIVATE cxx_std_20)
    target_compile_options (${TNAME} PRIVATE -O3)
endforeach()
    
target_link_libraries (${MAIN_EXEC} PRIVATE Logging_lib sha256)
target_link_libraries (${TEST_EXEC} PRIVATE Logging_lib sha256 GTest::GTest GTest::Main)