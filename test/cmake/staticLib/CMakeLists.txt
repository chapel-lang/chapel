cmake_minimum_required(VERSION 3.21.1)

find_package(chpl REQUIRED)

project(staticLib LANGUAGES CHPL C)
message(STATUS "Using chpl: ${CMAKE_CHPL_COMPILER}")


add_library(adder STATIC)
target_sources(adder PRIVATE src/adder.c)
target_include_directories(adder PUBLIC include)
install(TARGETS adder DESTINATION "lib")


add_executable(main)
target_sources(main PRIVATE main.chpl)
target_link_libraries(main adder)
target_link_options(main PRIVATE -I${CMAKE_SOURCE_DIR}/include)
install(TARGETS main DESTINATION "bin")
