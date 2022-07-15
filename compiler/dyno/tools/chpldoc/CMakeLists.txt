# Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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
# set(CMAKE_VERBOSE_MAKEFILE ON)
add_executable(chpldoc "chpldoc.cpp")
# TODO: this is temporary so I could use std::filesystem
set_property(TARGET chpldoc PROPERTY CXX_STANDARD 17)
# TODO: Using LLVM here links against the shared library.
# Linking against LLVMSupport (which grabs libLLVMSupport.a) was not working
message("LLVM_LIBS = ${LLVM_LIBS}")

target_link_libraries(chpldoc ${LLVM_LIBS} $<TARGET_OBJECTS:libdyno-obj>)
target_compile_definitions(chpldoc PUBLIC ${LLVM_DEFINITIONS})
target_include_directories(chpldoc PUBLIC
                           ${CHPL_MAIN_INCLUDE_DIR}
                           ${CHPL_INCLUDE_DIR}
                           ${LLVM_INCLUDE_DIRS})
add_dependencies(chpldoc libdyno-obj)
