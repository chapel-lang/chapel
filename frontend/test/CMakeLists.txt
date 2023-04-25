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

# define target for the common code for all dyno tests
add_library(tests-common OBJECT EXCLUDE_FROM_ALL
            test-common.cpp test-parsing.cpp test-resolution.cpp)
target_link_libraries(tests-common ChplFrontend)

add_custom_target(tests)
set(CHPL_TEST_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR})

# define a function to simplify adding tests
function(comp_unit_test target)
  add_executable(${target} EXCLUDE_FROM_ALL "${target}.cpp")
  # don't rebuild if shared library changes, and store in the build directory
  set_target_properties(${target}  PROPERTIES
                        LINK_DEPENDS_NO_SHARED true
                        RUNTIME_OUTPUT_DIRECTORY . )
  target_link_libraries(${target} ChplFrontend tests-common)
  target_include_directories(${target} PRIVATE ${CHPL_TEST_INCLUDE_DIR})
  add_test(NAME ${target} COMMAND ${target})
  set_tests_properties(${target} PROPERTIES
                       ENVIRONMENT "CHPL_HOME=${CHPL_HOME}")
  add_dependencies(tests ${target})
endfunction(comp_unit_test)

add_subdirectory(clang)
add_subdirectory(framework)
add_subdirectory(parsing)
add_subdirectory(resolution)
add_subdirectory(uast)
add_subdirectory(util)
