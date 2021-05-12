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

#ifndef CHPL_UAST_ENUM_H
#define CHPL_UAST_ENUM_H

#include "chpl/queries/Location.h"
#include "chpl/uast/TypeSym.h"

namespace chpl {
namespace uast {


/**
  This class represents an enum declaration. For example:

  \rst
  .. code-block:: chapel

      enum myEnum { a, b = 2, c }

  \endrst

  The enum itself (myEnum) is represented by an Enum AST node. The Enum
  AST node contains EnumElementDecls which contain the EnumElements
  (for a, b, c in the example).
 */
class Enum final : public TypeSym {
 friend class EnumDecl;

 private:
  Enum(ASTList children, UniqueString name, Visibility vis)
    : TypeSym(asttags::Enum, std::move(children), name, vis) {

    assert(isEnumElementDeclAndCommentList(children_));
  }
  static bool isEnumElementDeclAndCommentList(const ASTList& list);
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~Enum() override = default;

  /**
    Return a way to iterate over the EnumElements and Comments.
   */
  ASTListIteratorPair<Expression> stmts() const {
    return ASTListIteratorPair<Expression>(children_.begin(), children_.end());
  }

  /**
   Return the number of EnumElements and Comments contained in this Enum.
   */
  int numStmts() const {
    return this->numChildren();
  }
  /**
   Return the i'th EnumElement or Comment in this Enum.
   */
  const Expression* stmt(int i) const {
    const ASTNode* ast = this->child(i);
    assert(ast->isExpression());
    return (const Expression*)ast;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
