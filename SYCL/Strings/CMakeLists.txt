cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fsycl")

project(Strings)

add_subdirectory(Common_libs/Logging)
add_subdirectory(NativeAlg)
add_subdirectory(RabKar)

set(MAIN_EXEC main)
set(TEST_EXEC tests)

message("Building main")
add_executable(${MAIN_EXEC} main.cpp)
message("Building test")
add_executable(${TEST_EXEC} comparing.cpp)

set(TARGETS 
    ${MAIN_EXEC}
    ${TEST_EXEC}
)
foreach(TNAME ${TARGETS})
    target_compile_features(${TNAME} PRIVATE cxx_std_20)
    target_compile_options (${TNAME} PRIVATE -O3)
endforeach()
    
target_link_libraries  (
    ${MAIN_EXEC} PRIVATE 
    NativeAlg RabKar
    Logging_lib
)

target_link_libraries  (
    ${TEST_EXEC} PRIVATE 
    NativeAlg RabKar
    Logging_lib
)