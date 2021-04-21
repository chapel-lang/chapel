comp_unit_test(testUniqueString)
comp_unit_test(testBuildIDs)

# Copy the moby.txt to the binary dir for use by the test
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../../../test/types/string/psahabu/perf/moby.txt
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
