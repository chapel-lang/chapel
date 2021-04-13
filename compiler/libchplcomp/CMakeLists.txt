cmake_minimum_required(VERSION 3.13.4)

project(libchplcomp VERSION 0.1)

set(CHPL_MAIN_SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR})
set(CHPL_MAIN_INCLUDE_DIR ${CHPL_MAIN_SRC_DIR}/include)
set(CHPL_INCLUDE_DIR      ${CMAKE_CURRENT_BINARY_DIR}/include)

# save the version
configure_file(${CHPL_MAIN_INCLUDE_DIR}/chpl/Config/config.h.cmake
               ${CHPL_INCLUDE_DIR}/chpl/Config/config.h)

# see https://llvm.org/docs/CMake.html
# for instructions on extending this to include LLVM dependencies

# request C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set include directories for -I
#include_directories(include)

# lib/CMakeLists.txt defines the target libchplcomp
add_subdirectory(lib)
# also make sure headers are included in the target
add_subdirectory(include/chpl)
# Support for documentation of AST header
add_subdirectory(doc)

# Support for C++ compiler unit tests
# Needs to happen in this file for ctest to work in this dir
enable_testing()

add_custom_target(tests)

# define a function to simplify adding tests
function(comp_unit_test target)
  add_executable(${target} "${target}.cpp")
  set_property(TARGET ${target} PROPERTY EXCLUDE_FROM_ALL)
  target_link_libraries(${target} $<TARGET_OBJECTS:libchplcomp-obj>)
  target_include_directories(${target} PUBLIC
                             ${CHPL_MAIN_INCLUDE_DIR}
                             ${CHPL_INCLUDE_DIR})
  add_dependencies(${target} libchplcomp-obj)
  add_test(NAME ${target} COMMAND ${target})
  add_dependencies(tests ${target})
endfunction(comp_unit_test)

add_subdirectory(test)
