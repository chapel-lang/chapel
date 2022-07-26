add_library(cmakeLib cmakeLib.c)

include(../../lib/CMakeLists.FooLibrary)
target_include_directories(cmakeLib PUBLIC ${CHPL_INCLUDE_DIRS})
target_link_libraries(cmakeLib PUBLIC ${CHPL_LINK_LIBS})
