# Copyright 2026 Hewlett Packard Enterprise Development LP
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

add_executable(astIdLess EXCLUDE_FROM_ALL "astIdLess.cpp")
set_target_properties(astIdLess PROPERTIES RUNTIME_OUTPUT_DIRECTORY . )
target_link_libraries(astIdLess clang-cpp ${CHPL_LLVM_LINK_ARGS})
target_compile_options(astIdLess PRIVATE SHELL:$<$<COMPILE_LANGUAGE:CXX>:${CHPL_LLVM_COMP_ARGS}>)

target_compile_definitions(astIdLess PRIVATE
  CHPL_COMPILER_SOURCE_DIR="${SRC_DIR}"
  CHPL_LINTERS_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
if(EXISTS "${CHPL_LLVM_CLANG_CXX}")
  execute_process(COMMAND ${CHPL_LLVM_CLANG_CXX} -print-resource-dir
                  OUTPUT_VARIABLE CHPL_CLANG_RESOURCE_DIR
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  target_compile_definitions(astIdLess PRIVATE
    CHPL_CLANG_RESOURCE_DIR="${CHPL_CLANG_RESOURCE_DIR}")
endif()
if(APPLE)
  execute_process(COMMAND xcrun --show-sdk-path
                  OUTPUT_VARIABLE CHPL_SYSROOT
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  target_compile_definitions(astIdLess PRIVATE CHPL_SYSROOT="${CHPL_SYSROOT}")
endif()
