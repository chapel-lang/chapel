# Copyright 2021-2023 Hewlett Packard Enterprise Development LP
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

comp_unit_test(testRecursionFails)
set_tests_properties(testRecursionFails PROPERTIES WILL_FAIL TRUE)

comp_unit_test(testDependencies)
comp_unit_test(testErrorTracking)
comp_unit_test(testIds)
comp_unit_test(testUniqueString)

# Copy the moby.txt to the binary dir for use by testUniqueString
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../../test/types/string/psahabu/perf/moby.txt
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
