cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-fsycl")

project(BitSort)
set(MAIN main)
set(TEST tests)

add_subdirectory(BSort)
add_subdirectory(Common_libs/Time)
add_subdirectory(MySycl)

message("Building main")
add_executable(${MAIN} main.cpp)

message("Building tests")
add_executable(${TEST} TESTS/test_checking.cpp)

set (TARGETS
    ${MAIN}
    ${TEST}
)

foreach (TNAME ${TARGETS})    
    target_compile_features(${TNAME} PRIVATE cxx_std_20)
    target_compile_options(${TNAME} PRIVATE -O3)
    target_link_libraries(${TNAME} BSort Time MySycl)
endforeach()