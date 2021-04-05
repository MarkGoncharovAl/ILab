set(NAME_LIB Foo)

#FOO_LIBRARY_PATH must be writted by USER who wants to use it
#You will see this at target_link_libraries

add_library(${NAME_LIB} STATIC IMPORTED)
find_library(FOO_LIBRARY_PATH ${NAME_LIB} HINTS "${CMAKE_CURRENT_LIST_DIR}/../../")
set_target_properties(${NAME_LIB} PROPERTIES IMPORTED_LOCATION "${FOO_LIBRARY_PATH}")