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

#include "chpl/uast/Visitor.h"

namespace chpl {
namespace uast {


Visitor::~Visitor() {
}

void Visitor::visit(const ASTNode* ast) {

  switch (ast->tag()) {

#define CASE_LEAF(NAME) \
    case asttags::NAME: { \
      const NAME* casted = (const NAME*) ast; \
      this->visit##NAME(casted); \
      break; \
    }

#define CASE_NODE(NAME) \
    case asttags::NAME: { \
      const NAME* casted = (const NAME*) ast; \
      bool goInToIt = this->enter##NAME(casted); \
      if (goInToIt) { \
        int nChildren = ast->numChildren(); \
        for (int i = 0; i < nChildren; i++) { \
          this->visit(ast->child(i)); \
        } \
        this->exit##NAME(casted); \
      } \
      break; \
    }

#define CASE_OTHER(NAME) \
    case asttags::NAME: \
      assert(false && "this code should never be run"); \
      break;

#define AST_NODE(NAME) CASE_NODE(NAME)
#define AST_LEAF(NAME) CASE_LEAF(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) CASE_OTHER(START_##NAME)
#define AST_END_SUBCLASSES(NAME) CASE_OTHER(END_##NAME)

    // Apply the above macros to ASTClassesList.h
    // to fill in the cases
#include "chpl/uast/ASTClassesList.h"
  
    // and also for NUM_AST_TAGS
    CASE_OTHER(NUM_AST_TAGS)

    // clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef CASE_LEAF
#undef CASE_NODE
#undef CASE_OTHER
  }
}


} // namespace uast
} // namespace chpl
