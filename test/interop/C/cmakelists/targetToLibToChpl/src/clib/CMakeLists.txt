add_library(cmakeLib cmakeLib.c)

include(../../lib/FooLibrary.cmake)
target_include_directories(cmakeLib PUBLIC ${FooLibrary_INCLUDE_DIRS})
target_link_libraries(cmakeLib PUBLIC ${FooLibrary_LINK_LIBS})
