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


# Write the COPYRIGHT file, or update the existing file if the
# the $CHPL_HOME/COPYRIGHT file has been modified
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/COPYRIGHT
  COMMAND ${CHPL_CMAKE_PYTHON}
          ${CMAKE_CURRENT_SOURCE_DIR}/../../util/config/generate-copyright
          ${CMAKE_CURRENT_SOURCE_DIR}/../../
          ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../../COPYRIGHT
  COMMENT "writing COPYRIGHT file updates..."
  VERBATIM)
add_executable(chpldoc chpldoc.cpp arg.cpp arg-helpers.cpp COPYRIGHT)
set_property(TARGET chpldoc PROPERTY CXX_STANDARD 14)
target_link_libraries(chpldoc ChplFrontend)
target_include_directories(chpldoc PRIVATE
                           ${CHPL_MAIN_INCLUDE_DIR}
                           ${CHPL_INCLUDE_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR})
