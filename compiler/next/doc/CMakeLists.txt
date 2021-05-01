# Copyright 2021 Hewlett Packard Enterprise Development LP
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

# Support for documentation of AST header
find_package(Doxygen)

if(DOXYGEN_FOUND)
  # doxygen config-file settings can be set here by prefixing with DOXYGEN_
  set(DOXYGEN_GENERATE_XML YES)
  set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doxygen")
  # the following exclude avoids errors in the sphinx build
  set(DOXYGEN_EXCLUDE "${CHPL_MAIN_INCLUDE_DIR}/chpl/AST/ASTClassesList.h")
  # these aliases allow \rst and \endrst in docs comments
  set(DOXYGEN_ALIASES
     "rst=\\verbatim embed:rst"
     "endrst=\\endverbatim")
  set(DOXYGEN_EXTRACT_PRIVATE NO)
  # when Doxygen runs, define DOXYGEN in the preprocessor
  set(DOXYGEN_PREDEFINED "DOXYGEN")
  # exclude combine functions that the docs system has trouble with
  set(DOXYGEN_EXCLUDE_SYMBOLS "chpl::combine")
  doxygen_add_docs(api-docs ${CHPL_MAIN_INCLUDE_DIR})

else(DOXYGEN_FOUND)
  add_custom_target(api-docs
                    COMMAND echo "not running doxygen because it is missing"
                    COMMENT "using dummy api-docs because doxygen is missing")
endif(DOXYGEN_FOUND)
