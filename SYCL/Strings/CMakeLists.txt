cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-fsycl")

project(Strings)

find_package(Logging CONFIG REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

add_subdirectory(NativeAlg)
add_subdirectory(RabKar)
add_subdirectory(MySycl)

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
    NativeAlg RabKar MySycl 
    Logging
)

target_link_libraries  (
    ${TEST_EXEC} PRIVATE 
    NativeAlg RabKar MySycl 
    Logging 
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY}
)