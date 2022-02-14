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

add_executable(fieldsUsed "fieldsUsed.cpp")

execute_process(COMMAND llvm-config --cxxflags OUTPUT_VARIABLE llvm_cxx_flags OUTPUT_STRIP_TRAILING_WHITESPACE)
# target_compile_options(fieldsUsed PRIVATE ${llvm_cxx_flags})

execute_process(COMMAND llvm-config-11 --libs support OUTPUT_VARIABLE llvm_libs OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND llvm-config-11 --libdir OUTPUT_VARIABLE llvm_libdir OUTPUT_STRIP_TRAILING_WHITESPACE)

target_link_directories(fieldsUsed PRIVATE ${llvm_libdir})
target_link_libraries(fieldsUsed clang-cpp ${llvm_libs})
