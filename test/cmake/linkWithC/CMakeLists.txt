cmake_minimum_required(VERSION 3.21.1)

find_package(chpl REQUIRED)

project(linkWithC LANGUAGES CHPL C)
message(STATUS "Using chpl: ${CMAKE_CHPL_COMPILER}")


add_executable(main)
target_sources(main PRIVATE main.chpl src/adder.c)
set_target_properties(main PROPERTIES LINKER_LANGUAGE CHPL)

# used by C compiler
target_include_directories(main PRIVATE include)
# used by Chapel compiler
target_link_options(main PRIVATE -I${CMAKE_SOURCE_DIR}/include)

install(TARGETS main DESTINATION ".")
