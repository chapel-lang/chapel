# Support for documentation of AST header
find_package(Doxygen)

if(DOXYGEN_FOUND)
  set(DOXYGEN_GENERATE_XML YES)
  set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doxygen")
  #file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY})
  doxygen_add_docs(api-docs ${CHPL_MAIN_INCLUDE_DIR})
  #set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
  #set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
  #set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)
  #add_custom_command(
  #  OUTPUT ${SPHINX_INDEX_FILE}
  #  COMMAND 
  #    python3 ../../third-party/chpl-venv/install/chpldeps
  #    sphinx-build -b html
  #    # Tell Breathe where to find the Doxygen output
  #    -Dbreathe_projects.libchplcomp=${DOXYGEN_OUTPUT_DIRECTORY}/xml
  #  ${SPHINX_SOURCE} ${SPHINX_BUILD}
  #  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  #  DEPENDS
  #  # Other docs files you want to track should go here (or in some variable)
  #  ${CMAKE_CURRENT_SOURCE_DIR}/index.rst
  #  ${DOXYGEN_INDEX_FILE}
  #  MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
  #  COMMENT "Generating documentation with Sphinx") 
endif(DOXYGEN_FOUND)
