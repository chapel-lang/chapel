find_program(
  CMAKE_CHPL_COMPILER
  NAMES "chpl"
  DOC "chpl compiler")

set(CMAKE_CHPL_COMPILER_ENV_VAR "")

configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeChplCompiler.cmake.in
               ${CMAKE_PLATFORM_INFO_DIR}/CMakeChplCompiler.cmake)
