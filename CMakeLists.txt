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

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.16)
  # request new behavior for INSTALL_RPATH quoting
  cmake_policy(SET CMP0095 NEW)
endif()
if (CMAKE_VERSION VERSION_LESS 3.20)
  # warn that cmake 3.20 will be required in a future release
  message(WARNING "cmake 3.20 or newer will be required in a future \
                  release. You have ${CMAKE_VERSION}")
endif()

# generate the compile_commands.json compilation database
# tools like the fieldsUsed linter require this
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

### BEGIN config.h value setting ###
# Handle all (most?) the versioning information in one place
# These values will be written to <build_dir>/frontend/include/chpl/config/config.h

set(CHPL_MAJOR_VERSION 1)
set(CHPL_MINOR_VERSION 31)
set(CHPL_PATCH_VERSION 0)
set(CHPL_BUILD_VERSION 0)

# Flip this to 'true' when we're ready to roll out a release; then back
# after branching
set(CHPL_OFFICIAL_RELEASE false)

### END config.h version value setting - configured_prefix set below ###

set(CHPL_VERSION ${CHPL_MAJOR_VERSION}.${CHPL_MINOR_VERSION}.${CHPL_PATCH_VERSION})
if (${CHPL_OFFICIAL_RELEASE} AND CHPL_BUILD_VERSION GREATER 0)
  set(CHPL_VERSION ${CHPL_MAJOR_VERSION}.${CHPL_MINOR_VERSION}.${CHPL_PATCH_VERSION}.${CHPL_BUILD_VERSION})
endif()

project(Chapel VERSION ${CHPL_VERSION})

message(STATUS "Configuring Chapel version: ${CMAKE_PROJECT_VERSION}")

# This built-in module allows us to print variables easily, using
# cmake_print_variables(MY_VARIABLE)
include(CMakePrintHelpers)


### BEGIN handling installation mode from ./configure - prefix or home are allowed ###
if (NOT DEFINED INSTALLATION_MODE)
    # it can be set to empty string if no installation mode is selected yet
    set(INSTALLATION_MODE "" CACHE STRING "prefix or home installation mode")
endif()


# Handle Installation Prefix by writing CONFIGURED_PREFIX if it's not found,
# or checking the value against the incoming CMAKE_INSTALL_PREFIX and writing
# a new one if they don't match.
set(CONFIGURED_PREFIX_CONTENTS " ")
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/CONFIGURED_PREFIX)
  message(VERBOSE "CONFIGURED_PREFIX not found: Writing CONFIGURED_PREFIX file")
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/CONFIGURED_PREFIX" "\"${CMAKE_INSTALL_PREFIX}\"")
else()
  message(VERBOSE "Reading CONFIGURED_PREFIX")
  file(STRINGS "${CMAKE_BINARY_DIR}/CONFIGURED_PREFIX" CONFIGURED_PREFIX_CONTENTS)
  message(VERBOSE "previously configured to install to ${CONFIGURED_PREFIX_CONTENTS}")
endif()

# TODO: eventually remove the quotes from the file(WRITE) call below and we can be free of them!
# TODO: Do we still need the CONFIGURED_PREFIX file for anything if we are writing it to config.h?
string(COMPARE NOTEQUAL "\"${CMAKE_INSTALL_PREFIX}\"" ${CONFIGURED_PREFIX_CONTENTS} DO_UPDATE_PREFIX)
if(${DO_UPDATE_PREFIX})
    message(VERBOSE "CMAKE_INSTALL_PREFIX updated to ${CMAKE_INSTALL_PREFIX}: Writing CONFIGURED_PREFIX file")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/CONFIGURED_PREFIX" "\"${CMAKE_INSTALL_PREFIX}\"")
else()
  message(STATUS "CMAKE_INSTALL_PREFIX is ${CMAKE_INSTALL_PREFIX}")
endif()
message(VERBOSE "Installation Mode: ${INSTALLATION_MODE}")

### END handling installation mode - CMAKE_INSTALL_PREFIX will be written to config.h ###


# Handle options that correspond to printchplenv --compiler
# Could be set from environment or command-line
# See https://chapel-lang.org/docs/usingchapel/chplenv.html# for details on each
# TODO: Update these "Description" values to meaningful values, maybe the link to the docs?
set(CHPL_HOST_PLATFORM CACHE STRING "Override chplenv value")
set(CHPL_HOST_COMPILER CACHE STRING "Override chplenv value")
set(CHPL_HOST_ARCH CACHE STRING "Override chplenv value")
set(CHPL_HOST_MEM CACHE STRING "Override chplenv value")
set(CHPL_LLVM CACHE STRING "Override chplenv value")
set(CHPL_LLVM_SUPPORT CACHE STRING "Override chplenv value")
set(CHPL_LLVM_CONFIG CACHE STRING "Override chplenv value")
set(CHPL_SANITIZE CACHE STRING "Override chplenv value")

# Some variables that aren't in printchplenv output, but are/might be useful
set(CHPL_HOME CACHE STRING "Override home directory for Chapel")
set(CHPL_DEVELOPER CACHE BOOL "Override chplenv value")
set(CHPL_LLVM_GCC_PREFIX CACHE STRING "Override chplenv value")
set(CHPL_CXX_FLAGS CACHE STRING "Compiler flags from our Makefile")
set(CHPL_LD_FLAGS CACHE STRING "Linker flags from our Makefile")
# Internal CHPL variables
set(CHPL_HOST_CC CACHE INTERNAL "Description")
set(CHPL_HOST_CXX CACHE INTERNAL "Description")
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
set(CHPL_LLVM_STATIC_DYNAMIC CACHE INTERNAL "Description")
set(CHPL_LLVM_TARGET_CPU CACHE INTERNAL "Description")
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
  if(DEFINED ENV{CHPL_DEVELOPER})
    set(CHPL_DEVELOPER $ENV{CHPL_DEVELOPER} CACHE BOOL "Override CHPL_DEVELOPER flag" FORCE)
    message(NOTICE "CHPL_DEVELOPER found in ENV, using $ENV{CHPL_DEVELOPER}")
  else()
    set(CHPL_DEVELOPER OFF CACHE BOOL "Override CHPL_DEVELOPER flag" FORCE)
    message(NOTICE "CHPL_DEVELOPER is not set, using ${CHPL_DEVELOPER}")
  endif()
else()
  # set(ENV{CHPL_DEVELOPER} ${CHPL_DEVELOPER})
  message(STATUS "CHPL_DEVELOPER environment var updated to ${CHPL_DEVELOPER}")
endif()

# If we didn't get a value from the cmdline and the environment has a value, use it
if ((NOT DEFINED ${CHPL_LLVM_GCC_PREFIX}) AND (NOT $ENV{CHPL_LLVM_GCC_PREFIX} STREQUAL ""))
  message(NOTICE "CHPL_LLVM_GCC_PREFIX not set, using $ENV{CHPL_LLVM_GCC_PREFIX}")
  set(CHPL_LLVM_GCC_PREFIX $ENV{CHPL_LLVM_GCC_PREFIX} CACHE STRING "Override CHPL_LLVM_GCC_PREFIX flag" FORCE)
elseif(DEFINED ${CHPL_LLVM_GCC_PREFIX})
  # set(ENV{CHPL_LLVM_GCC_PREFIX} ${CHPL_LLVM_GCC_PREFIX})
  message(STATUS "CHPL_LLVM_GCC_PREFIX updated to ${CHPL_LLVM_GCC_PREFIX}")
else()
  message(STATUS "No CHPL_LLVM_GCC_PREFIX env var or value given from command line.")
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
# TODO: Wire these up to do something based on compiler type and version
# as in Makefile.clang, Makefile.gnu
# set(ASSERTS OFF CACHE BOOL "ASSERTS flag from make")
# set(WARNINGS OFF CACHE BOOL "WARNINGS flag from make")
# set(OPTIMIZE OFF CACHE BOOL "OPTIMIZE flag from make")
# set(DEBUG OFF CACHE BOOL "DEBUG flag from make") # CMake Debug?
# set(PROFILE OFF CACHE BOOL "PROFILE flag from make")

# TODO: replace this with above, get proper values depending on compiler/version
#  see make/compiler/Makefile.gnu, make/compiler/Makefile.clang
add_compile_options(SHELL:$<$<COMPILE_LANGUAGE:CXX>:${CHPL_CXX_FLAGS}>)
add_link_options(SHELL:$<$<COMPILE_LANGUAGE:CXX>:${CHPL_LD_FLAGS}>)

# TODO: Update the C/CXX flags with the proper values for DEBUG
# if (${DEBUG})
#   set(CMAKE_BUILD_TYPE CACHE BOOL Debug "Debug build" FORCE)
# endif()

# TODO: Update the C/CXX flags with the proper values for WARNINGS
# if (${WARNINGS})
#   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
# endif()

# TODO: Update the C/CXX flags with the proper values for OPTIMIZE
# if (${OPTIMIZE})
#   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
# endif()

# TODO: Update the C/CXX flags with the proper values for PROFILE
# if (${PROFILE})
#   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# endif()

set(PRINTCHPLENV_COMMAND printchplenv --compiler --internal --cmake)
# message(STATUS "running ${PRINTCHPLENV_COMMAND} from ${CMAKE_CURRENT_SOURCE_DIR}/util")

# Pass the CMake detected C/CXX compilers to printchplenv
# TODO: we should support the standard cmake way of overriding these and have
# setting them from the command-line be an error, e.g., `cmake -DCHPL_HOST_CC=bla`
execute_process(COMMAND ${CMAKE_COMMAND} -E env
                CHPL_LLVM_GCC_PREFIX=${CHPL_LLVM_GCC_PREFIX}
                CHPL_DEVELOPER=${CHPL_DEVELOPER}
                CHPL_HOST_CC=${CMAKE_C_COMPILER}
                CHPL_HOST_CXX=${CMAKE_CXX_COMPILER}
                ${CMAKE_CURRENT_SOURCE_DIR}/util/${PRINTCHPLENV_COMMAND}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/util
                TIMEOUT 30
                RESULT_VARIABLE CHPLENV_RETURNCODE
                OUTPUT_VARIABLE CHPLENV_OUTPUT
                OUTPUT_STRIP_TRAILING_WHITESPACE
                )

message(VERBOSE "Updating build configuration with results from printchplenv:\n${CHPLENV_OUTPUT}")

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
    message(VERBOSE "${CHPLENV_LINE} did not contain a value - unsetting")
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

# TODO: This is a hack to get past the safe directory failure from git
# when running in github CI. It looks like this same command is executed
# as part of the github workflow, but I'm not sure why it doesn't seem
# to be taking effect when cmake calls the python script to generate
# the git-version file.
if (DEFINED ENV{GITHUB_ACTIONS})
  if($ENV{GITHUB_ACTIONS} STREQUAL "true")
    message(STATUS "Running in GitHub Actions - set safe.directory")
    execute_process(COMMAND git config --global --add safe.directory ${CHPL_HOME})
  endif()
endif()
set(CHPL_CMAKE_PYTHON $ENV{CHPL_MAKE_PYTHON})
message(STATUS "Using Python: ${CHPL_CMAKE_PYTHON}")
# message(DEBUG "${CHPL_HOST_BUNDLED_LINK_ARGS} ${CHPL_HOST_SYSTEM_LINK_ARGS}")
add_subdirectory(compiler)
add_subdirectory(frontend)
add_subdirectory(tools/chpldoc)

# Adjust the install rpath for chpl and chpldoc
if (INSTALLATION_MODE STREQUAL "prefix")
  # let the build and install rpaths be absolute (which is the default)
  set_target_properties(chpl chpldoc
                        PROPERTIES
                        INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/chapel/${CHPL_MAJOR_VERSION}.${CHPL_MINOR_VERSION}/compiler")
  install(TARGETS chpl chpldoc
          RUNTIME DESTINATION bin)
else()
  # use relative rpaths for the build and do not change them on install
  if (CMAKE_VERSION VERSION_LESS 3.16)
    # do nothing to adjust the rpath because we don't know how to
    # write $ORIGIN quoted correctly
    if (INSTALLATION_MODE STREQUAL "home")
      # Error out if a 'home' installation was requested
      message(FATAL_ERROR "--chpl-home installation requires cmake 3.16 or newer")
    endif()
  else()
    set(CHPL_INSTALL_RPATH "$ORIGIN/../../lib/compiler/${CHPL_HOST_BIN_SUBDIR}")
    # handle differently on MacOS by writing @executable_path instead of $ORIGIN
    if(APPLE)
      message(STATUS "Updating RPATH for MACOS")
      set(CHPL_INSTALL_RPATH "@executable_path/../../lib/compiler/${CHPL_HOST_BIN_SUBDIR}")
    endif()

    set_target_properties(chpl chpldoc
                          PROPERTIES
                          BUILD_WITH_INSTALL_RPATH TRUE
                          INSTALL_RPATH ${CHPL_INSTALL_RPATH})
  endif()

  install(TARGETS chpl chpldoc
          RUNTIME DESTINATION "bin/${CHPL_HOST_BIN_SUBDIR}")
endif()
