
add_library(libchplcomp SHARED)

# Include the public library .h files as well as any generated .h files
target_include_directories(libchplcomp PUBLIC
                           ${CHPL_MAIN_INCLUDE_DIR}
                           ${CHPL_INCLUDE_DIR})

# Library code can also use headers from the lib/ directory
# but these are not to be public
target_include_directories(libchplcomp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(AST)
add_subdirectory(Frontend)
add_subdirectory(Util)
