# Copyright 2023-2025 Hewlett Packard Enterprise Development LP
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


# if there is an env CHPL_HOME, use it to find printchplenv
if (DEFINED ENV{CHPL_HOME})
  set(CHPL_HOME $ENV{CHPL_HOME})

  execute_process(
    COMMAND ${CHPL_HOME}/util/printchplenv --value --only CHPL_HOST_BIN_SUBDIR
    OUTPUT_VARIABLE CHPL_HOST_BIN_SUBDIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  set(CHPL_HOME_BIN_PATH ${CHPL_HOME}/bin/${CHPL_HOST_BIN_SUBDIR})
  # TODO: what if its a prefix install?
else()
  set(CHPL_HOME "")
  set(CHPL_HOME_BIN_PATH "")
endif()

find_program(
  CMAKE_CHPL_COMPILER
  NAMES "chpl"
  HINTS ${CHPL_HOME} ${CHPL_HOME_BIN_PATH}
  DOC "chpl compiler"
)
# TODO: how to find chpl with just CHPL_HOME?

set(CMAKE_CHPL_COMPILER_ENV_VAR "")

configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeCHPLCompiler.cmake.in
               ${CMAKE_PLATFORM_INFO_DIR}/CMakeCHPLCompiler.cmake)
