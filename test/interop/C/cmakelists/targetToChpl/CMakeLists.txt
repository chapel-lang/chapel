project(cmakeTest)
cmake_minimum_required(VERSION 3.23)

add_executable(cmakeTest cmakeTest.c)

include(lib/CMakeLists.FooLibrary)
target_include_directories(cmakeTest PUBLIC ${CHPL_INCLUDE_DIRS})
target_link_libraries(cmakeTest PUBLIC ${CHPL_LINK_LIBS})

