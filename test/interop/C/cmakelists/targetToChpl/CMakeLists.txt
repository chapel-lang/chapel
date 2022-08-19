project(cmakeTest)
cmake_minimum_required(VERSION 3.18)

add_executable(cmakeTest cmakeTest.c)

include(lib/FooLibrary.cmake)
target_include_directories(cmakeTest PUBLIC ${FooLibrary_INCLUDE_DIRS})
target_link_libraries(cmakeTest PUBLIC ${FooLibrary_LINK_LIBS})

list(GET CHPL_COMPILER 0 CMAKE_C_COMPILER)
