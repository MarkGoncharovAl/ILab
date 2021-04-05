cmake_minimum_required(VERSION 2.8)

set(NAME_LIB Logging)
set(FILE_HPP Logging.hpp)
set(FILE_HPP_EXTRA Logging_DEFINES.hpp)
set(FILE_CPP Logging.cpp)
set(FILE_CONFIG LoggingConfig.cmake)
set(FOLDER_LIB Logging)

project(${NAME_LIB})

add_library(${NAME_LIB} ${FILE_CPP} ${FILE_HPP} ${FILE_HPP_EXTRA})
install(FILES ${FILE_HPP} DESTINATION include)
install(FILES ${FILE_HPP_EXTRA} DESTINATION include)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${FILE_CONFIG} DESTINATION lib/cmake/${FOLDER_LIB})