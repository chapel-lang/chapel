comp_unit_test(testUniqueString)
comp_unit_test(testBuildIDs)

# Add a link to the string test file.
# This link is created in the build directory at cmake "configure" time.
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/moby.txt
                 ${CMAKE_CURRENT_BINARY_DIR}/moby.txt)
