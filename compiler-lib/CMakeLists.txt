cmake_minimum_required(VERSION 3.13.4)

project(chplCompilerLib VERSION 0.1)

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

add_subdirectory(lib)

enable_testing()

# define a function to simplify adding tests
function(comp_unit_test target)
  add_executable(${target} "${target}.cpp")
  target_link_libraries(${target} chplCompilerLib)
  add_test(NAME ${target} COMMAND ${target})
endfunction(comp_unit_test)

add_subdirectory(test)
