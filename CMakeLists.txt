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

cmake_minimum_required(VERSION 3.13.4)

# TODO: Handle BUILD_VERSION for devops
# set(CHPL_BUILD_VERSION 0 CACHE ) - gets into generated.h along with version info,

# TODO: Write this to generated configured_prefix file
# CMAKE_INSTALL_PREFIX

# TODO: Work with config.h for version info
project(Chapel VERSION 1.29.0)

# This built-in module allows us to print variables easily, using
# cmake_print_variables(MY_VARIABLE)
include(CMakePrintHelpers)

# Turns on verbose output from Makefile builds. Having it ON displays each
# command line as it is launched.
# https://cmake.org/cmake/help/latest/variable/CMAKE_VERBOSE_MAKEFILE.html
# set(CMAKE_VERBOSE_MAKEFILE 1)


# Options that correspond to printchplenv --compiler
# Could be set from environment or command-line
# TODO: Update these "Description" values to meaningful values
set(CHPL_HOST_PLATFORM CACHE STRING "Description")
set(CHPL_HOST_COMPILER CACHE STRING "Description")
set(CHPL_HOST_CC CACHE STRING "Description")
set(CHPL_HOST_CXX CACHE STRING "Description")
set(CHPL_HOST_ARCH CACHE STRING "Description")
set(CHPL_HOST_MEM CACHE STRING "Description")
set(CHPL_LLVM CACHE STRING "Description")
set(CHPL_LLVM_SUPPORT CACHE STRING "Description")
set(CHPL_LLVM_CONFIG CACHE STRING "Description")
set(CHPL_SANITIZE CACHE STRING "Description")

# Some variables that aren't in printchplenv output, but are/might be useful
set(CHPL_HOME CACHE STRING "Override home directory for Chapel")
set(CHPL_DEVELOPER CACHE BOOL "Description")
set(CHPL_LLVM_GCC_PREFIX CACHE STRING "Description")

# Internal CHPL variables
set(CHPL_HOST_BUNDLED_COMPILE_ARGS CACHE INTERNAL "Description")
set(CHPL_HOST_CPU CACHE INTERNAL "Description")
set(CHPL_HOST_SYSTEM_COMPILE_ARGS CACHE INTERNAL "Description")
set(CHPL_HOST_BUNDLED_LINK_ARGS CACHE INTERNAL "Description")
set(CHPL_HOST_SYSTEM_LINK_ARGS CACHE INTERNAL "Description")
set(CHPL_HOST_JEMALLOC CACHE INTERNAL "Description")
set(CHPL_TARGET_COMPILER_PRGENV CACHE INTERNAL "Description")
set(CHPL_TARGET_BUNDLED_COMPILE_ARGS CACHE INTERNAL "Description")
set(CHPL_TARGET_SYSTEM_COMPILE_ARGS CACHE INTERNAL "Description")
set(CHPL_TARGET_BUNDLED_LINK_ARGS CACHE INTERNAL "Description")
set(CHPL_TARGET_SYSTEM_LINK_ARGS CACHE INTERNAL "Description")
set(CHPL_TARGET_MEM CACHE INTERNAL "Description")
set(CHPL_TARGET_JEMALLOC CACHE INTERNAL "Description")
set(CHPL_TARGET_CPU_FLAG CACHE INTERNAL "Description")
set(CHPL_TARGET_BACKEND_CPU CACHE INTERNAL "Description")
set(CHPL_MAKE CACHE INTERNAL "Description")
set(CHPL_GMP CACHE INTERNAL "Description")
set(CHPL_RUNTIME_CPU CACHE INTERNAL "Description")
set(CHPL_GMP_IS_OVERRIDDEN CACHE INTERNAL "Description")
set(CHPL_RE2_IS_OVERRIDDEN CACHE INTERNAL "Description")
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

# Set ENV versions of CHPL_ vars that are not in printchplenv output, in the
# case when they are set from the cmdline, otherwise get them from the environment
if (NOT DEFINED ${CHPL_DEVELOPER})
  message(NOTICE "CHPL_DEVELOPER not given, using $ENV{CHPL_DEVELOPER}")
  set(CHPL_DEVELOPER $ENV{CHPL_DEVELOPER} CACHE BOOL "Override CHPL_DEVELOPER flag" FORCE)
else()
  set(ENV{CHPL_DEVELOPER} ${CHPL_DEVELOPER})
  message(STATUS "CHPL_DEVELOPER updated to ${CHPL_DEVELOPER}")
endif()

# If we didn't get a value from the cmdline and the environment has a value, use it
if ((NOT DEFINED ${CHPL_LLVM_GCC_PREFIX}) AND (NOT $ENV{CHPL_LLVM_GCC_PREFIX} STREQUAL ""))
  message(NOTICE "CHPL_LLVM_GCC_PREFIX not set, using $ENV{CHPL_LLVM_GCC_PREFIX}")
  set(CHPL_LLVM_GCC_PREFIX $ENV{CHPL_LLVM_GCC_PREFIX} CACHE STRING "Override CHPL_LLVM_GCC_PREFIX flag" FORCE)
else()
  set(ENV{CHPL_LLVM_GCC_PREFIX} ${CHPL_LLVM_GCC_PREFIX})
  message(STATUS "CHPL_LLVM_GCC_PREFIX updated to ${CHPL_LLVM_GCC_PREFIX}")
endif()


# If any -DCHPL_ variables came to us from the command line, update them in the
# environment so that they can be used by printchplenv
if (DEFINED ${CHPL_HOME})
  set(ENV{CHPL_HOME} ${CHPL_HOME})
  message(NOTICE "CHPL_HOME updated to ${CHPL_HOME}")
endif()
if (DEFINED ${CHPL_HOST_PLATFORM})
  set(ENV{CHPL_HOST_PLATFORM} ${CHPL_HOST_PLATFORM})
  message(STATUS "CHPL_HOST_PLATFORM updated to ${CHPL_HOST_PLATFORM}")
endif()
if (DEFINED ${CHPL_HOST_COMPILER})
  set(ENV{CHPL_HOST_COMPILER} ${CHPL_HOST_COMPILER})
  message(STATUS "CHPL_HOST_COMPILER updated to ${CHPL_HOST_COMPILER}")
endif()
if (DEFINED ${CHPL_HOST_CC})
  set(ENV{CHPL_HOST_CC} ${CHPL_HOST_CC})
  message(STATUS "CHPL_HOST_CC updated to ${CHPL_HOST_CC}")
endif()
if (DEFINED ${CHPL_HOST_CXX})
  set(ENV{CHPL_HOST_CXX} ${CHPL_HOST_CXX})
  message(STATUS "CHPL_HOST_CXX updated to ${CHPL_HOST_CXX}")
endif()
if (DEFINED ${CHPL_HOST_ARCH})
  set(ENV{CHPL_HOST_ARCH} ${CHPL_HOST_ARCH})
  message(STATUS "CHPL_HOST_ARCH updated to ${CHPL_HOST_ARCH}")
endif()
if (DEFINED ${CHPL_HOST_MEM})
  set(ENV{CHPL_HOST_MEM} ${CHPL_HOST_MEM})
  message(STATUS "CHPL_HOST_MEM updated to ${CHPL_HOST_MEM}")
endif()
if (DEFINED ${CHPL_LLVM})
  set(ENV{CHPL_LLVM} ${CHPL_LLVM})
  message(STATUS "CHPL_LLVM updated to ${CHPL_LLVM}")
endif()
if (DEFINED ${CHPL_LLVM_SUPPORT})
  set(ENV{CHPL_LLVM_SUPPORT} ${CHPL_LLVM_SUPPORT})
  message(STATUS "CHPL_LLVM_SUPPORT updated to ${CHPL_LLVM_SUPPORT}")
endif()
if (DEFINED ${CHPL_LLVM_CONFIG})
  set(ENV{CHPL_LLVM_CONFIG} ${CHPL_LLVM_CONFIG})
  message(STATUS "CHPL_LLVM_CONFIG updated to ${CHPL_LLVM_CONFIG}")
endif()
if (DEFINED ${CHPL_SANITIZE})
  set(ENV{CHPL_SANITIZE} ${CHPL_SANITIZE})
  message(STATUS "CHPL_SANITIZE updated to ${CHPL_SANITIZE}")
endif()

# Makefile arguments we still want to support
# TODO: SEE IF CMAKE COVERS THESE VARIABLES
# TODO: Wire these up to do something
set(ASSERTS OFF CACHE BOOL "ASSERTS flag from make")
set(WARNINGS OFF CACHE BOOL "WARNINGS flag from make")
set(OPTIMIZE OFF CACHE BOOL "OPTIMIZE flag from make")
set(DEBUG OFF CACHE BOOL "DEBUG flag from make")
set(PROFILE OFF CACHE BOOL "PROFILE flag from make")

# TODO: Update the C/CXX flags with the proper values for each of the above options
if (${DEBUG})
  set(CMAKE_BUILD_TYPE CACHE BOOL Debug "Debug build" FORCE)
endif()

if (${WARNINGS})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

if (${OPTIMIZE})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

set(PRINTCHPLENV_COMMAND printchplenv --compiler --internal --cmake)
# message(STATUS "running ${PRINTCHPLENV_COMMAND} from ${CMAKE_CURRENT_SOURCE_DIR}/util")

# Pass the CMake detected C/CXX compilers to printchplenv
# TODO: we should support the standard cmake way of overriding these and have
# setting them from the command-line be an error, e.g., `cmake -DCHPL_HOST_CC=bla`
execute_process(COMMAND ${CMAKE_COMMAND} -E env
                  CHPL_HOST_CC=${CMAKE_C_COMPILER} CHPL_HOST_CXX=${CMAKE_CXX_COMPILER} ${CMAKE_CURRENT_SOURCE_DIR}/util/${PRINTCHPLENV_COMMAND}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/util
                TIMEOUT 30
                RESULT_VARIABLE CHPLENV_RETURNCODE
                OUTPUT_VARIABLE CHPLENV_OUTPUT
                OUTPUT_STRIP_TRAILING_WHITESPACE
                )
message(NOTICE "Updating build configuration with results from printchplenv:\n${CHPLENV_OUTPUT}")

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
    message(NOTICE "${CHPLENV_LINE} did not contain a value - unsetting")
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

if ((CHPL_HOME STREQUAL CMAKE_BINARY_DIR) OR
    (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR))
  # running cmake in the source directory would overwrite Makefiles
  # even failing with this error creates a CMakeCache.txt that
  # messes things up, so ask the user to remove that if this happens.
  message(FATAL_ERROR "It won't work to run cmake from the Chapel source directory. Please run 'rm CMakeCache.txt' and then cd to a different directory before running cmake.")
endif()

# message(DEBUG "${CHPL_HOST_BUNDLED_LINK_ARGS} ${CHPL_HOST_SYSTEM_LINK_ARGS}")
add_subdirectory(compiler)
add_subdirectory(frontend)
