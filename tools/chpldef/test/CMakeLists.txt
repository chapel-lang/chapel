# Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

add_library(chpldef-test-utility OBJECT EXCLUDE_FROM_ALL
            TestClient.cpp)
target_link_libraries(chpldef-test-utility chpldef-objects)
set_property(TARGET chpldef-test-utility PROPERTY CXX_STANDARD 17)
target_include_directories(chpldef-test-utility PRIVATE
                           ${CHPLDEF_INCLUDE_DIR})

add_custom_target(chpldef-tests)

function(chpldef_compile_test target)
  add_executable(${target} EXCLUDE_FROM_ALL ${target}.cpp)
  set_target_properties(${target} PROPERTIES LINK_DEPENDS_NO_SHARED true
                        RUNTIME_OUTPUT_DIRECTORY . )
  target_link_libraries(${target} chpldef-objects chpldef-test-utility)
  target_link_libraries(${target} chpldef-objects)
  target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                             ${CHPLDEF_INCLUDE_DIR})
  add_test(NAME ${target} COMMAND ${target})
  set_tests_properties(${target} PROPERTIES ENVIRONMENT
                       "CHPL_HOME=${CHPL_HOME}")
  add_dependencies(chpldef-tests ${target})
  set_property(TARGET ${target} PROPERTY CXX_STANDARD 17)
endfunction(chpldef_compile_test)

chpldef_compile_test(test-lifecycle)
chpldef_compile_test(test-declaration)
