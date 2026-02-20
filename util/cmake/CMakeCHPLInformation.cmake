# Copyright 2023-2026 Hewlett Packard Enterprise Development LP
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


# TODO: this is a hack, how do we tell cmake that we can't compile to object files?
# TODO: how do we respect <DEFINES> and <INCLUDES>, CMake will only fill that in for CMAKE_*_COMPILER_OBJECT
set(CMAKE_CHPL_COMPILE_OBJECT "test -f <OBJECT> || ln -s <SOURCE> <OBJECT>")

# Support for CMAKE_EXPORT_CHPL_COMMANDS (similar to CMAKE_EXPORT_COMPILE_COMMANDS)
set(CHPL_COMPILER_FLAGS "-o <TARGET> <OBJECTS> <FLAGS> <LINK_FLAGS> <LINK_LIBRARIES>")
set(CMAKE_CHPL_LINK_EXECUTABLE "<CMAKE_CHPL_COMPILER> ${CHPL_COMPILER_FLAGS}")
if(CMAKE_EXPORT_CHPL_COMMANDS)
  # Get the directory containing the chpl compiler to find chpl-shim
  get_filename_component(CHPL_BIN_DIR "${CMAKE_CHPL_COMPILER}" DIRECTORY)
  set(CHPL_SHIM_SCRIPT "${CHPL_BIN_DIR}/chpl-shim")

  message(STATUS "CMAKE_EXPORT_CHPL_COMMANDS is ON, checking for chpl-shim at ${CHPL_SHIM_SCRIPT}")

  if(EXISTS "${CHPL_SHIM_SCRIPT}")
    # Use chpl-shim as a wrapper to generate .cls-commands.json
    # Note: chpl-shim expects to be called as 'chpl-shim chpl <args>', not with full path
    message(STATUS "Found chpl-shim. Configuring build command to use chpl-shim to generate .cls-commands.json")
    set(CMAKE_CHPL_LINK_EXECUTABLE "<CMAKE_COMMAND> -E env CHPL_SHIM_REAL_COMPILER_PATH=${CMAKE_CHPL_COMPILER} ${CHPL_SHIM_SCRIPT} chpl ${CHPL_COMPILER_FLAGS}")
  else()
    message(WARNING "CMAKE_EXPORT_CHPL_COMMANDS is set but chpl-shim not found at ${CHPL_SHIM_SCRIPT}")
  endif()
endif()

# TODO: support --library directly to build Chapel shared libs for interoperability
# CMAKE_*_CREATE_SHARED_LIBRARY

set(CMAKE_CHPL_SOURCE_FILE_EXTENSIONS chpl)

set(CMAKE_CHPL_OUTPUT_EXTENSION ".chpl")


set(CMAKE_CHPL_FLAGS_RELEASE "--fast")
set(CMAKE_CHPL_FLAGS_DEBUG "-g")
