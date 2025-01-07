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

cmake_minimum_required(VERSION 3.20.0)
project(llvm-pgas)

# relies on an external env variable, CHPL_LLVM_CONFIG, which can be set by:
# export `printchplenv --all --simple | grep CHPL_LLVM_CONFIG`
execute_process(COMMAND $ENV{CHPL_LLVM_CONFIG} --cmakedir OUTPUT_VARIABLE LLVM_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND $ENV{CHPL_LLVM_CONFIG} --bindir OUTPUT_VARIABLE LLVM_BIN OUTPUT_STRIP_TRAILING_WHITESPACE)

# uses LLVM_DIR from chplenv
find_package(LLVM 16 REQUIRED CONFIG)

if(LLVM_VERSION VERSION_LESS 16)
  message(FATAL_ERROR "Did not find LLVM 16 -- found ${LLVM_VERSION}")
endif()

set(LLVM_LIT "$ENV{CHPL_HOME}/third-party/llvm/llvm-src/utils/lit/lit.py")

set(CMAKE_CXX_STANDARD 17)

# find Python
find_package(PythonInterp)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

include(LLVM-Config)
include(HandleLLVMOptions)
include(AddLLVM)

include_directories(${LLVM_INCLUDE_DIRS})

# no interesting code unless we define this
add_definitions(-DHAVE_LLVM)

add_llvm_pass_plugin(llvm-pgas MODULE
    llvmAggregateGlobalOps.cpp
    llvmGlobalToWide.cpp
    llvmPgasPlugin.cpp
    llvmUtil.cpp
)

# set various configuration settings in the test suite
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/test/lit.cfg.in" "${CMAKE_CURRENT_BINARY_DIR}/test/lit.cfg")

file( GLOB_RECURSE test_files RELATIVE
      "${CMAKE_CURRENT_SOURCE_DIR}/" "test/*.ll" )
foreach( test_file ${test_files} )
  # test_file is e.g. test/a.ll
  # copy the test to the build directory
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${test_file}"
    "${CMAKE_CURRENT_BINARY_DIR}/${test_file}" COPYONLY)
endforeach( test_file )

#if(NOT EXISTS ${LLVM_ROOT}/bin/FileCheck)
#    message(FATAL_ERROR "need FileCheck installed to run tests; configure LLVM with -DLLVM_INSTALL_UTILS")
#endif()


# support make check with the LLVM tester lit in the tests directory
add_custom_target(check
    COMMAND ${PYTHON_EXECUTABLE} ${LLVM_LIT}
            "${CMAKE_CURRENT_BINARY_DIR}/test/" -v --path ${LLVM_BIN}
            DEPENDS llvm-pgas )

