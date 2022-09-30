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

# TODO: Handle BUILD_VERSION for devops
# set(CHPL_BUILD_VERSION 0 CACHE ) - gets into generated.h along with version info,

# TODO: Write this to generated configured_prefix file
# CMAKE_INSTALL_PREFIX

project(Chapel VERSION 1.29.0)

# This built-in module allows us to print variables easily, using
# cmake_print_variables(MY_VARIABLE)
include(CMakePrintHelpers)

# option(CHPL_LLVM_COMP_ARGS "Override compile args for LLVM")
# option(CHPL_LLVM_LINK_ARGS "Override linker args for LLVM")
set(CHPL_HOME CACHE STRING "Override home directory for Chapel")

if (NOT DEFINED ${CHPL_HOME})
  message(STATUS "CHPL_HOME not set, using $ENV{CHPL_HOME}")
  set(CHPL_HOME $ENV{CHPL_HOME} CACHE STRING "Override home directory for Chapel" FORCE)
endif()

# Options that correspond to printchplenv --internal --compiler
# Could be set from environment or command-line
# Print the output of PRINTCHPLENV to user
set(CHPL_HOST_BUNDLED_COMPILE_ARGS CACHE STRING "Description")
set(CHPL_HOST_SYSTEM_COMPILE_ARGS CACHE STRING "Description")
set(CHPL_HOST_BUNDLED_LINK_ARGS CACHE STRING "Description")
set(CHPL_HOST_SYSTEM_LINK_ARGS CACHE STRING "Description")
set(CHPL_TARGET_COMPILER_PRGENV CACHE STRING "Description")
set(CHPL_TARGET_BUNDLED_COMPILE_ARGS CACHE STRING "Description")
set(CHPL_TARGET_SYSTEM_COMPILE_ARGS CACHE STRING "Description")
set(CHPL_TARGET_BUNDLED_LINK_ARGS CACHE STRING "Description")
set(CHPL_TARGET_SYSTEM_LINK_ARGS CACHE STRING "Description")
set(CHPL_HOST_MEM CACHE STRING "Description")
set(CHPL_HOST_JEMALLOC CACHE STRING "Description")
set(CHPL_TARGET_MEM CACHE STRING "Description")
set(CHPL_TARGET_JEMALLOC CACHE STRING "Description")
set(CHPL_MAKE CACHE STRING "Description")
set(CHPL_GMP CACHE STRING "Description")
set(CHPL_LLVM CACHE STRING "Description")
set(CHPL_SANITIZE CACHE STRING "Description")
# Internal CHPL variables
set(CHPL_HOST_PLATFORM CACHE INTERNAL "Description")
set(CHPL_HOST_COMPILER CACHE INTERNAL "Description")
set(CHPL_HOST_CC CACHE INTERNAL "Description")
set(CHPL_HOST_CXX CACHE INTERNAL "Description")
set(CHPL_HOST_ARCH CACHE INTERNAL "Description")
set(CHPL_HOST_CPU CACHE INTERNAL "Description")
set(CHPL_RUNTIME_CPU CACHE INTERNAL "Description")
set(CHPL_TARGET_CPU_FLAG CACHE INTERNAL "Description")
set(CHPL_TARGET_BACKEND_CPU CACHE INTERNAL "Description")
set(CHPL_GMP_IS_OVERRIDDEN CACHE INTERNAL "Description")
set(CHPL_RE2_IS_OVERRIDDEN CACHE INTERNAL "Description")
set(CHPL_LLVM_SUPPORT CACHE INTERNAL "Description")
set(CHPL_LLVM_CONFIG CACHE INTERNAL "Description")
set(CHPL_LLVM_VERSION CACHE INTERNAL "Description")
set(CHPL_LLVM_CLANG_C CACHE INTERNAL "Description")
set(CHPL_LLVM_CLANG_CXX CACHE INTERNAL "Description")
set(CHPL_RUNTIME_SUBDIR CACHE INTERNAL "Description")
set(CHPL_LAUNCHER_SUBDIR CACHE INTERNAL "Description")
set(CHPL_COMPILER_SUBDIR CACHE INTERNAL "Description")
set(CHPL_HOST_BIN_SUBDIR CACHE INTERNAL "Description")
set(CHPL_TARGET_BIN_SUBDIR CACHE INTERNAL "Description")
set(CHPL_SYS_MODULES_SUBDIR CACHE INTERNAL "Description")
set(CHPL_LLVM_UNIQ_CFG_PATH CACHE INTERNAL "Description")
set(CHPL_GASNET_UNIQ_CFG_PATH CACHE INTERNAL "Description")
set(CHPL_GMP_UNIQ_CFG_PATH CACHE INTERNAL "Description")
set(CHPL_HWLOC_UNIQ_CFG_PATH CACHE INTERNAL "Description")
set(CHPL_HOST_JEMALLOC_UNIQ_CFG_PATH CACHE INTERNAL "Description")
set(CHPL_TARGET_JEMALLOC_UNIQ_CFG_PATH CACHE INTERNAL "Description")
set(CHPL_LIBFABRIC_UNIQ_CFG_PATH CACHE INTERNAL "Description")
set(CHPL_LIBUNWIND_UNIQ_CFG_PATH CACHE INTERNAL "Description")
set(CHPL_QTHREAD_UNIQ_CFG_PATH CACHE INTERNAL "Description")
set(CHPL_RE2_UNIQ_CFG_PATH CACHE INTERNAL "Description")
set(CHPL_PE_CHPL_PKGCONFIG_LIBS CACHE INTERNAL "Description")
# TODO: SOME VARIABLES THAT AREN'T IN THE PRINTCHPLENV OUTPUT, but are useful
# set(ENV{CHPL_HOME}) - interacts with printchplenv # need to pass -D cmake values to env
# CHPL_DEVELOPER - possibly not interacts with printchplenv
# CHPL_LLVM_GCC_PREFIX - interacts with printchplenv
# TODO: SEE IF CMAKE COVERS THESE VARIABLES
# ASSERTS - want to toggle independent of debug build
# WARNINGS
# OPTIMIZE
# DEBUG
# PROFILE

set(PRINTCHPLENV_COMMAND printchplenv --compiler --internal --cmake)
# message(STATUS "running ${PRINTCHPLENV_COMMAND} from ${CMAKE_CURRENT_SOURCE_DIR}/util")

execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/util/${PRINTCHPLENV_COMMAND}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/util
                TIMEOUT 30
                RESULT_VARIABLE CHPLENV_RETURNCODE
                OUTPUT_VARIABLE CHPLENV_OUTPUT
                OUTPUT_STRIP_TRAILING_WHITESPACE
                )
message(NOTICE "Updating build configuration with results from printchplenv:\n${CHPLENV_OUTPUT}")
# string(REGEX REPLACE "\n$" "; " CHPLENV_OUTPUT "${CHPLENV_OUTPUT}")
# This is a workaround for CMake not having any in-built way to
# split a string into a list.  It's not perfect, but it works for this purpose
STRING(REGEX REPLACE ";" "\\\\;" CHPLENV_OUTPUT "${CHPLENV_OUTPUT}")
STRING(REGEX REPLACE "\n" ";" CHPLENV_OUTPUT "${CHPLENV_OUTPUT}")
# message(DEBUG "printchplenv returned ${CHPLENV_RETURNCODE}")
# set all the chplenv values from printchplenv
foreach (CHPLENV_LINE IN LISTS CHPLENV_OUTPUT)
  # Split the line into the parts we want, variable name and value
  string(FIND "${CHPLENV_LINE}" " " SPLIT_POS)
  if (SPLIT_POS LESS 1)
    message(WARNING "${CHPLENV_LINE} did not contain a value - unsetting")
    unset(${CHPLENV_LINE} CACHE)
    continue()
  endif()
  string(SUBSTRING "${CHPLENV_LINE}" 0 ${SPLIT_POS} CHPL_ENV_NAME)
  math(EXPR SPLIT_POS "${SPLIT_POS} + 1")
  string(SUBSTRING "${CHPLENV_LINE}" ${SPLIT_POS} -1 CHPL_ENV_VALUE)

  # message(DEBUG "${CHPL_ENV_NAME} ${CHPL_ENV_VALUE}")
  # message(DEBUG "Setting ${CHPLENV_LINE} as ${CHPL_ENV_NAME} ${CHPL_ENV_VALUE}")
  set(${CHPL_ENV_NAME} ${CHPL_ENV_VALUE} CACHE STRING "overwritten description" FORCE)
endforeach()
# message(DEBUG "${CHPL_HOST_BUNDLED_LINK_ARGS} ${CHPL_HOST_SYSTEM_LINK_ARGS}")
add_subdirectory(compiler)


