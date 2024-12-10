cmake_minimum_required(VERSION 3.21.1)

find_package(chpl REQUIRED)

project(hello LANGUAGES CHPL)
message(STATUS "Using chpl: ${CMAKE_CHPL_COMPILER}")

add_executable(hello)
target_sources(hello PRIVATE hello.chpl)
target_link_options(hello PRIVATE --ccflags -O1)

install(TARGETS hello DESTINATION ".")
