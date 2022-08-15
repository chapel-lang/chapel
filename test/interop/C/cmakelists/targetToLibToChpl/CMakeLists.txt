project(cmakeTest)
cmake_minimum_required(VERSION 3.18)

include(lib/FooLibrary.cmake)
list(GET CHPL_COMPILER 0 CMAKE_C_COMPILER)
add_subdirectory(src/clib)
add_subdirectory(src/exec)

