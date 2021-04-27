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

#ifndef CHPL_AST_VISITOR_H
#define CHPL_AST_VISITOR_H

#include "chpl/AST/ASTBase.h"

namespace chpl {
namespace uast {


// Generally an AST visitor has one or two routine per type of AST node.
// For nodes that can contain other nodes, it has 2 routines:
//   bool enterSomething(Something* node)
//   void exitSomething(Something* node)
// And for nodes that do not contain other nodes, it has
//   void visitSomething(Something* node)
class Visitor {
 public:
  Visitor() = default;
  virtual ~Visitor() = 0; // this is an abstract base class

// define visitLeaf and enterNode/exitNode
/// \cond DO_NOT_DOCUMENT
#define VISIT_LEAF(NAME) \
  virtual void visit##NAME(const NAME* node) { }
#define VISIT_NODE(NAME) \
  virtual bool enter##NAME(const NAME* node) { return true; } \
  virtual void exit##NAME(const NAME* node)  { }

#define AST_NODE(NAME) VISIT_NODE(NAME)
#define AST_LEAF(NAME) VISIT_LEAF(NAME)
#define AST_BEGIN_SUBCLASSES(NAME)
#define AST_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to ASTClassesList.h
#include "chpl/AST/ASTClassesList.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef VISIT_NODE
#undef VISIT_LEAF

  // entry point into the visiting
  void visit(const ASTBase* ast);
};


} // end namespace uast
} // end namespace chpl

#endif
