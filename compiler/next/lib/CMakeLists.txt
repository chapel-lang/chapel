# Copyright 2021 Hewlett Packard Enterprise Development LP
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


add_library(libchplcomp-obj OBJECT)

# turn on position-independent code so we can use the same
# libchplcomp-obj for both a static and dynamic library
#set_property(TARGET libchplcomp-obj PROPERTY POSITION_INDEPENDENT_CODE 1)

# Include the public library .h files as well as any generated .h files
target_include_directories(libchplcomp-obj PUBLIC
                           ${CHPL_MAIN_INCLUDE_DIR}
                           ${CHPL_INCLUDE_DIR})


# Library code can also use headers from the lib/ directory
# but these are not to be public
target_include_directories(libchplcomp-obj PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(parsing)
add_subdirectory(queries)
add_subdirectory(resolution)
add_subdirectory(types)
add_subdirectory(uast)
add_subdirectory(util)

add_library(libchplcomp $<TARGET_OBJECTS:libchplcomp-obj>)
target_include_directories(libchplcomp PUBLIC
                           ${CHPL_MAIN_INCLUDE_DIR}
                           ${CHPL_INCLUDE_DIR})
