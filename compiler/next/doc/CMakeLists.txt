# Support for documentation of AST header
find_package(Doxygen)

if(DOXYGEN_FOUND)
  # doxygen config-file settings can be set here by prefixing with DOXYGEN_
  set(DOXYGEN_GENERATE_XML YES)
  set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doxygen")
  # the following exclude avoids errors in the sphinx build
  set(DOXYGEN_EXCLUDE "${CHPL_MAIN_INCLUDE_DIR}/chpl/AST/ASTClassesList.h")
  # these aliases allow \rst and \endrst in docs comments
  set(DOXYGEN_ALIASES
     "rst=\\verbatim embed:rst"
     "endrst=\\endverbatim")
  set(DOXYGEN_EXTRACT_PRIVATE NO)
  # when Doxygen runs, define DOXYGEN in the preprocessor
  set(DOXYGEN_PREDEFINED "DOXYGEN")
  # exclude combine functions that the docs system has trouble with
  set(DOXYGEN_EXCLUDE_SYMBOLS "chpl::combine")
  doxygen_add_docs(api-docs ${CHPL_MAIN_INCLUDE_DIR})

else(DOXYGEN_FOUND)
  add_custom_target(api-docs
                    COMMAND echo
                    COMMENT "using dummy api-docs because doxygen is missing")
  add_custom_command(TARGET api-docs POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_directory
                         ${CMAKE_CURRENT_SOURCE_DIR}/dummy-doxygen-output/xml
                         ${CMAKE_CURRENT_BINARY_DIR}/doxygen/xml)
endif(DOXYGEN_FOUND)
