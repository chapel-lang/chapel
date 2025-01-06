# Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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


# request C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# request C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR})

# TODO: See if we can use these directly, I am not sure we can because
# the CMake way to concat strings requires an output variable
string(CONCAT CHPL_LLVM_COMP_ARGS ${CHPL_HOST_BUNDLED_COMPILE_ARGS} " " ${CHPL_HOST_SYSTEM_COMPILE_ARGS})
string(CONCAT CHPL_LLVM_LINK_ARGS ${CHPL_HOST_BUNDLED_LINK_ARGS} " " ${CHPL_HOST_SYSTEM_LINK_ARGS})

# message(DEBUG "CHPL_LLVM_COMP_ARGS: ${CHPL_LLVM_COMP_ARGS}")
# message(DEBUG "CHPL_LLVM_LINK_ARGS: ${CHPL_LLVM_LINK_ARGS}")

# remove whitespace or you'll get complaints about an error
# according to policy CMP0004
if (CHPL_LLVM_COMP_ARGS)
  string(STRIP ${CHPL_LLVM_COMP_ARGS} CHPL_LLVM_COMP_ARGS)
endif()
string(STRIP ${CHPL_LLVM_LINK_ARGS} CHPL_LLVM_LINK_ARGS)
set(CHPL_CMAKE_PYTHON $ENV{CHPL_MAKE_PYTHON})

# Write the compiler/main/LICENSE file, or update the existing file if the
# the $CHPL_HOME/LICENSE file has been modified
add_custom_command(
  OUTPUT ${SRC_DIR}/main/LICENSE
  COMMAND ${CHPL_CMAKE_PYTHON}
          ${SRC_DIR}/../util/config/generate-license
          ${SRC_DIR}/../
          ${SRC_DIR}/main
  DEPENDS ${SRC_DIR}/../LICENSE
  COMMENT "writing LICENSE file updates..."
  VERBATIM)

# Write the compiler/main/COPYRIGHT file, or update the existing file if the
# the $CHPL_HOME/COPYRIGHT file has been modified
add_custom_command(
  OUTPUT ${SRC_DIR}/main/COPYRIGHT
  COMMAND ${CHPL_CMAKE_PYTHON}
          ${SRC_DIR}/../util/config/generate-copyright
          ${SRC_DIR}/../
          ${SRC_DIR}/main
  DEPENDS ${SRC_DIR}/../COPYRIGHT
  COMMENT "writing COPYRIGHT file updates..."
  VERBATIM)

# Write the reservedSymbolNames.h file, or update the existing file if the
# contents of ${SRC_DIR}/codegen/reservedSymbolNames have changed
add_custom_command(
  OUTPUT ${SRC_DIR}/codegen/reservedSymbolNames.h
  COMMAND ${CHPL_CMAKE_PYTHON}
          ${SRC_DIR}/../util/config/generate-reservedSymbolNames
          ${SRC_DIR}/codegen
  DEPENDS ${SRC_DIR}/codegen/reservedSymbolNames
  COMMENT "writing reservedSymbolNames.h file updates..."
  VERBATIM)

add_executable(chpl)
# need many of the outputs from printchplenv for the following values
# note that headers for dyno are included by target_link_libraries(chpl PRIVATE ChplFrontend)
# because the ChplFrontend target_include_directories command in the frontend/lib
# CMakeLists.txt file specifies its includes as PUBLIC
target_include_directories(chpl PRIVATE
        ${SRC_DIR}/include/${CHPL_HOST_PLATFORM} # for free-bsd support (work-around)
        ${SRC_DIR}/include # headers for chpl, needed all the time
        ${SRC_DIR}/../build/compiler/${CHPL_COMPILER_SUBDIR} # for enabling CONFIGURED_PREFIX in configured_prefix.h
        ${SRC_DIR}/../runtime/include/encoding # support for sharing unicode support between compiler and runtime
        ${SRC_DIR}/../third-party/utf8-decoder # support for sharing unicode support between compiler and runtime
        )

# all the compiler subdirectories with sources do these same things
# the set_property command is here to define a macro that gives the
# compiler the name of the source directory so it can be used in error reports
# if the compiler segfaults
function(add_compiler_sources srcs directory_name)
  target_sources(chpl
                 PRIVATE
                 ${srcs})
  get_filename_component(DIR_NAME ${directory_name} NAME)
  set_property(SOURCE ${srcs}
               TARGET_DIRECTORY chpl
               PROPERTY COMPILE_DEFINITIONS COMPILER_SUBDIR=${DIR_NAME}
               APPEND)
endfunction()


add_subdirectory(AST)
add_subdirectory(adt)
add_subdirectory(backend)
add_subdirectory(codegen)
add_subdirectory(llvm)
add_subdirectory(main)
add_subdirectory(optimizations)
add_subdirectory(passes)
add_subdirectory(resolution)
add_subdirectory(util)
target_compile_options(chpl PRIVATE SHELL:$<$<COMPILE_LANGUAGE:CXX>:${CHPL_LLVM_COMP_ARGS}>)
target_link_libraries(chpl PRIVATE ChplFrontend ${CHPL_LLVM_LINK_ARGS})
