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

cmake_minimum_required(VERSION 3.13.4)

project(Chapel VERSION 1.29.0)

# option(CHPL_LLVM_COMP_ARGS "Override compile args for LLVM")
# option(CHPL_LLVM_LINK_ARGS "Override linker args for LLVM")
set(CHPL_CMAKE_HOME CACHE STRING "Override home directory for Chapel")

if (NOT DEFINED ${CHPL_CMAKE_HOME})
  message("CHPL_CMAKE_HOME not set, using $ENV{CHPL_HOME}")
  set(CHPL_CMAKE_HOME $ENV{CHPL_HOME} CACHE STRING "Override home directory for Chapel" FORCE)
endif()

# Options that correspond to printchplenv --internal --compiler
set(CHPL_CMAKE_HOST_PLATFORM CACHE STRING "Description")
set(CHPL_CMAKE_HOST_COMPILER CACHE STRING "Description")
set(CHPL_CMAKE_HOST_CC CACHE STRING "Description")
set(CHPL_CMAKE_HOST_CXX CACHE STRING "Description")
set(CHPL_CMAKE_HOST_BUNDLED_COMPILE_ARGS CACHE STRING "Description")
set(CHPL_CMAKE_HOST_SYSTEM_COMPILE_ARGS CACHE STRING "Description")
set(CHPL_CMAKE_HOST_BUNDLED_LINK_ARGS CACHE STRING "Description")
set(CHPL_CMAKE_HOST_SYSTEM_LINK_ARGS CACHE STRING "Description")
set(CHPL_CMAKE_HOST_ARCH CACHE STRING "Description")
set(CHPL_CMAKE_HOST_CPU CACHE STRING "Description")
set(CHPL_CMAKE_TARGET_COMPILER_PRGENV CACHE STRING "Description")
set(CHPL_CMAKE_TARGET_BUNDLED_COMPILE_ARGS CACHE STRING "Description")
set(CHPL_CMAKE_TARGET_SYSTEM_COMPILE_ARGS CACHE STRING "Description")
set(CHPL_CMAKE_TARGET_BUNDLED_LINK_ARGS CACHE STRING "Description")
set(CHPL_CMAKE_TARGET_SYSTEM_LINK_ARGS CACHE STRING "Description")
set(CHPL_CMAKE_RUNTIME_CPU CACHE STRING "Description")
set(CHPL_CMAKE_TARGET_CPU_FLAG CACHE STRING "Description")
set(CHPL_CMAKE_TARGET_BACKEND_CPU CACHE STRING "Description")
set(CHPL_CMAKE_HOST_MEM CACHE STRING "Description")
set(CHPL_CMAKE_HOST_JEMALLOC CACHE STRING "Description")
set(CHPL_CMAKE_TARGET_MEM CACHE STRING "Description")
set(CHPL_CMAKE_TARGET_JEMALLOC CACHE STRING "Description")
set(CHPL_CMAKE_MAKE CACHE STRING "Description")
set(CHPL_CMAKE_GMP CACHE STRING "Description")
set(CHPL_CMAKE_GMP_IS_OVERRIDDEN CACHE STRING "Description")
set(CHPL_CMAKE_RE2_IS_OVERRIDDEN CACHE STRING "Description")
set(CHPL_CMAKE_LLVM CACHE STRING "Description")
set(CHPL_CMAKE_LLVM_SUPPORT CACHE STRING "Description")
set(CHPL_CMAKE_LLVM_CONFIG CACHE STRING "Description")
set(CHPL_CMAKE_LLVM_VERSION CACHE STRING "Description")
set(CHPL_CMAKE_LLVM_CLANG_C CACHE STRING "Description")
set(CHPL_CMAKE_LLVM_CLANG_CXX CACHE STRING "Description")
set(CHPL_CMAKE_SANITIZE CACHE STRING "Description")
set(CHPL_CMAKE_RUNTIME_SUBDIR CACHE STRING "Description")
set(CHPL_CMAKE_LAUNCHER_SUBDIR CACHE STRING "Description")
set(CHPL_CMAKE_COMPILER_SUBDIR CACHE STRING "Description")
set(CHPL_CMAKE_HOST_BIN_SUBDIR CACHE STRING "Description")
set(CHPL_CMAKE_TARGET_BIN_SUBDIR CACHE STRING "Description")
set(CHPL_CMAKE_SYS_MODULES_SUBDIR CACHE STRING "Description")
set(CHPL_CMAKE_LLVM_UNIQ_CFG_PATH CACHE STRING "Description")
set(CHPL_CMAKE_GASNET_UNIQ_CFG_PATH CACHE STRING "Description")
set(CHPL_CMAKE_GMP_UNIQ_CFG_PATH CACHE STRING "Description")
set(CHPL_CMAKE_HWLOC_UNIQ_CFG_PATH CACHE STRING "Description")
set(CHPL_CMAKE_HOST_JEMALLOC_UNIQ_CFG_PATH CACHE STRING "Description")
set(CHPL_CMAKE_TARGET_JEMALLOC_UNIQ_CFG_PATH CACHE STRING "Description")
set(CHPL_CMAKE_LIBFABRIC_UNIQ_CFG_PATH CACHE STRING "Description")
set(CHPL_CMAKE_LIBUNWIND_UNIQ_CFG_PATH CACHE STRING "Description")
set(CHPL_CMAKE_QTHREAD_UNIQ_CFG_PATH CACHE STRING "Description")
set(CHPL_CMAKE_RE2_UNIQ_CFG_PATH CACHE STRING "Description")
set(CHPL_CMAKE_PE_CHPL_PKGCONFIG_LIBS CACHE STRING "Description")

set(PRINTCHPLENV_COMMAND printchplenv --compiler --internal --cmake)
# message("running ${PRINTCHPLENV_COMMAND} from ${CMAKE_CURRENT_SOURCE_DIR}/util")

find_package(Python)

execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/util/${PRINTCHPLENV_COMMAND}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/util
                TIMEOUT 5
                RESULT_VARIABLE CHPLENV_RETURNCODE
                OUTPUT_VARIABLE CHPLENV_OUTPUT
                )

# string(REGEX REPLACE "\n$" "; " CHPLENV_OUTPUT "${CHPLENV_OUTPUT}")
STRING(REGEX REPLACE ";" "\\\\;" CHPLENV_OUTPUT "${CHPLENV_OUTPUT}")
STRING(REGEX REPLACE "\n" ";" CHPLENV_OUTPUT "${CHPLENV_OUTPUT}")
# message("printchplenv returned ${CHPLENV_RETURNCODE}")
# message("got the following from printchplenv: ${CHPLENV_OUTPUT}")
# set all the chplenv values from printchplenv
foreach (CHPLENV_LINE IN LISTS CHPLENV_OUTPUT)
  string(FIND "${CHPLENV_LINE}" " " SPLIT_POS)
  if (SPLIT_POS LESS 1)
    message("printchplenv output line did not contain a space: ${CHPLENV_LINE}")
  endif()
  string(SUBSTRING "${CHPLENV_LINE}" 0 ${SPLIT_POS} CHPL_ENV_NAME)
  math(EXPR SPLIT_POS "${SPLIT_POS} + 1")
  string(SUBSTRING "${CHPLENV_LINE}" ${SPLIT_POS} -1 CHPL_ENV_VALUE)
  if ("${CHPL_ENV_VALUE}" STREQUAL "")
    continue()
  endif()
  # message("${CHPL_ENV_NAME} ${CHPL_ENV_VALUE}")
  # message("Setting ${CHPLENV_LINE} as ${CHPL_ENV_NAME} ${CHPL_ENV_VALUE}")
  set(${CHPL_ENV_NAME} ${CHPL_ENV_VALUE} CACHE STRING "overwritten description" FORCE)
endforeach()
# message("${CHPL_CMAKE_HOST_BUNDLED_LINK_ARGS} ${CHPL_CMAKE_HOST_SYSTEM_LINK_ARGS}")
add_subdirectory(compiler)


