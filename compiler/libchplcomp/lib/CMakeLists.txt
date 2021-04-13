
add_library(libchplcomp-obj OBJECT)

# turn on position-independent code so we can use the same
# libchplcomp-obj for both a static and dynamic library
#set_property(TARGET libchplcomp-obj PROPERTY POSITION_INDEPENDENT_CODE 1)

# Include the public library .h files as well as any generated .h files
target_include_directories(libchplcomp-obj PUBLIC
                           ${CHPL_MAIN_INCLUDE_DIR}
                           ${CHPL_INCLUDE_DIR})


# Library code can also use headers from the lib/ directory
# but these are not to be public
target_include_directories(libchplcomp-obj PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(AST)
add_subdirectory(Frontend)
add_subdirectory(Util)

add_library(libchplcomp $<TARGET_OBJECTS:libchplcomp-obj>)
target_include_directories(libchplcomp PUBLIC
                           ${CHPL_MAIN_INCLUDE_DIR}
                           ${CHPL_INCLUDE_DIR})
