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

# Support for generating API documentation from C++ headers

# Require Doxygen version 1.8.17 or newer
#  * observed problems with 1.8.13 and 1.8.14
#  * 1.8.17 is used in Ubuntu 20.04 LTS and seems to work
find_package(Doxygen 1.8.17)

if(DOXYGEN_FOUND)
  # doxygen config-file settings can be set here by prefixing with DOXYGEN_
  set(DOXYGEN_GENERATE_XML YES)
  set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doxygen")
  set(DOXYGEN_INCLUDE_PATH ${CHPL_MAIN_INCLUDE_DIR})

  # these aliases allow \rst and \endrst in docs comments
  set(DOXYGEN_ALIASES
     "rst=\\verbatim embed:rst"
     "endrst=\\endverbatim")
  set(DOXYGEN_EXTRACT_PRIVATE NO)
  set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)
  set(DOXYGEN_QUIET YES)

  # Enable the below line to request inherited members appear
  # as if they are not inherited.
  # set(DOXYGEN_INLINE_INHERITED_MEMB YES)

  # when Doxygen runs, define DOXYGEN in the preprocessor
  set(DOXYGEN_PREDEFINED "DOXYGEN")

  file(GLOB_RECURSE headers "${CHPL_MAIN_INCLUDE_DIR}/*.h")
  if (CMAKE_VERSION VERSION_LESS 3.16)
    # don't use USE_STAMP_FILE since it wasn't added until 3.16
    doxygen_add_docs(api-docs ${headers})
  else()
    # use USE_STAMP_FILE to prevent rebuild if headers didn't change
    doxygen_add_docs(api-docs ${headers} USE_STAMP_FILE)
  endif()

else(DOXYGEN_FOUND)
  add_custom_target(api-docs
                    COMMAND echo "doxygen missing" && exit 1
                    COMMENT "could not find doxygen -- is it installed?")
endif(DOXYGEN_FOUND)
