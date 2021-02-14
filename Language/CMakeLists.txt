cmake_minimum_required(VERSION 3.12)
project(Main VERSION 1.0.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(Node)
add_subdirectory(Grammar)
add_subdirectory(Common_libs)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

flex_target(lexer Lexer/scanner.l Lexer/lex.yy.cc)
bison_target(parser Grammar/parser.y Grammar/parser.tab.cc)

add_flex_bison_dependency(lexer parser)

message("Building main!")
add_executable(main ../main.cpp ../Lexer/lex.yy.cc ../Grammar/parser.tab.cc)
target_link_libraries(main Node Symtab parser errors)