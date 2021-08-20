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

# cmake has some support for finding flex/bison,
# but it wasn't clear to me how to get that working with
# our model of checking in the generated parser.
#FIND_PACKAGE(BISON)
#FIND_PACKAGE(FLEX)
#IF(BISON_FOUND AND FLEX_FOUND)
#  BISON_TARGET(MyParser chpl.ypp ${CMAKE_CURRENT_SOURCE_DIR}/bison-chpl-lib.cpp
#               DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/bison-chpl-lib.h)
#  FLEX_TARGET(MyScanner chapel.lex ${CMAKE_CURRENT_SOURCE_DIR}/flex-chpl-lib.cpp)
#  ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)
#ENDIF(BISON_FOUND AND FLEX_FOUND)

target_sources(libchplcomp-obj
               PRIVATE

               # these are generated
               bison-chpl-lib.cpp
               bison-chpl-lib.h
               flex-chpl-lib.cpp
               flex-chpl-lib.h

               # these are included from the generated files
               ParserContext.h
               ParserContextImpl.h
               lexer-help.h
               parser-dependencies.h
               parser-help.h

               # these are implementing the public interface
               Parser.cpp
               parsing-queries.cpp
              )

add_custom_target(parser
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  COMMAND bison chpl.ypp
                  COMMAND flex chpl.lex
                  COMMAND rm -f bison-chpl-lib.output
                  COMMENT "generating flex/bison parser")
