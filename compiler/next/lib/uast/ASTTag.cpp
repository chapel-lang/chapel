/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "chpl/uast/ASTTag.h"

namespace chpl {
namespace uast {
namespace asttags {


static const char* tagToStringTable[NUM_AST_TAGS] = {
// define tag to string conversion
#define NAMESTR(NAME) \
  #NAME,
#define AST_NODE(NAME) NAMESTR(NAME)
#define AST_LEAF(NAME) NAMESTR(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) NAMESTR(START_##NAME)
#define AST_END_SUBCLASSES(NAME) NAMESTR(END_##NAME)
// Apply the above macros to ASTClassesList.h
#include "chpl/uast/ASTClassesList.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef NAMESTR
};

const char* tagToString(ASTTag tag) {
  if (0 <= tag && tag < NUM_AST_TAGS)
    return tagToStringTable[tag];
  else
    return "<unknown-tag>";
}


} // end namespace asttags
} // end namespace uast
} // end namespace chpl
