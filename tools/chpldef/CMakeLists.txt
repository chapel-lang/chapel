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

# the lanuguage features used in the server require at least c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Put all the binary source files into an object blob.
add_library(chpldef-objects
    command-line-flags.cpp
    compiler-gadgets.cpp
    compute-goto-declaration.cpp
    compute-lifecycle.cpp
    compute-synchronization.cpp
    events.cpp
    Format.cpp
    Logger.cpp
    Message.cpp
    misc.cpp
    protocol-types.cpp
    Server.cpp
    Transport.cpp)

# Link the Chapel frontend to the object blob.
target_link_libraries(chpldef-objects ChplFrontend)
target_include_directories(chpldef-objects PRIVATE
                           ${CHPL_MAIN_INCLUDE_DIR}
                           ${CHPL_INCLUDE_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR})

# Create the 'chpldef' executable.
add_executable(chpldef chpldef.cpp)
target_link_libraries(chpldef chpldef-objects)
set(CHPLDEF_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(chpldef PRIVATE
                           ${CHPL_MAIN_INCLUDE_DIR}
                           ${CHPL_INCLUDE_DIR}
                           ${CHPLDEF_INCLUDE_DIR})

# Enable testing if the test folder exists.
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")
  enable_testing()
  add_subdirectory(test)
endif()
