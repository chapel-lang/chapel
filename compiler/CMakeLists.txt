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



# request C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# request C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Turns on verbose output from Makefile builds. Having it ON displays each
# command line as it is launched.
# https://cmake.org/cmake/help/latest/variable/CMAKE_VERBOSE_MAKEFILE.html
set(CMAKE_VERBOSE_MAKEFILE 1)

set(SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR})

string(CONCAT CHPL_LLVM_COMP_ARGS_JOIN ${CHPL_CMAKE_HOST_BUNDLED_COMPILE_ARGS} " " ${CHPL_CMAKE_HOST_SYSTEM_COMPILE_ARGS})
string(CONCAT CHPL_LLVM_LINK_ARGS_JOIN ${CHPL_CMAKE_HOST_BUNDLED_LINK_ARGS} " " ${CHPL_CMAKE_HOST_SYSTEM_LINK_ARGS})

set(CHPL_LLVM_COMP_ARGS ${CHPL_LLVM_COMP_ARGS_JOIN})
set(CHPL_LLVM_LINK_ARGS ${CHPL_LLVM_LINK_ARGS_JOIN})

# message(DEBUG "CHPL_LLVM_COMP_ARGS: ${CHPL_LLVM_COMP_ARGS}")
# message(DEBUG "CHPL_LLVM_LINK_ARGS: ${CHPL_LLVM_LINK_ARGS}")

# remove whitespace or you'll get complaints about an error
# according to policy CMP0004
if (CHPL_LLVM_COMP_ARGS)
  string(STRIP ${CHPL_LLVM_COMP_ARGS} CHPL_LLVM_COMP_ARGS)
endif()
# string(STRIP ${CHPL_LLVM_COMP_ARGS} CHPL_LLVM_COMP_ARGS)
# if (NOT STREQUAL ${CHPL_LLVM_LINK_ARGS} "")
  string(STRIP ${CHPL_LLVM_LINK_ARGS} CHPL_LLVM_LINK_ARGS)
# endif()
# string(STRIP ${CHPL_LLVM_LINK_ARGS} CHPL_LLVM_LINK_ARGS)


# need many of the outputs from printchplenv for the following values
include_directories(
        ${SRC_DIR}/include/${CHPL_CMAKE_HOST_PLATFORM}
        ${SRC_DIR}/include
        ${SRC_DIR}/../build/compiler/${CHPL_CMAKE_COMPILER_SUBDIR}
        ${SRC_DIR}/../runtime/include/encoding
        ${SRC_DIR}/../third-party/utf8-decoder
        ${SRC_DIR}/dyno/include
        )

add_executable(chpl)
add_subdirectory(AST)
add_subdirectory(adt)
add_subdirectory(backend)
add_subdirectory(codegen)
add_subdirectory(dyno)
add_subdirectory(include)
add_subdirectory(llvm)
add_subdirectory(main)
add_subdirectory(optimizations)
add_subdirectory(parser)
add_subdirectory(passes)
add_subdirectory(resolution)
add_subdirectory(util)
add_dependencies(chpl libdyno)
target_compile_options(chpl PUBLIC SHELL:$<$<COMPILE_LANGUAGE:CXX>:${CHPL_LLVM_COMP_ARGS}>)
target_link_libraries(chpl PUBLIC ${CHPL_LLVM_LINK_ARGS} libdyno)
