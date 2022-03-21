/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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
#include "chpl/uast/Comment.h"
#include "chpl/uast/TypeDecl.h"

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
class Enum final : public TypeDecl {
 private:
  Enum(AstList children, int attributesChildNum, Decl::Visibility vis,
       UniqueString name)
    : TypeDecl(asttags::Enum, std::move(children), attributesChildNum,
               vis,
               Decl::DEFAULT_LINKAGE,
               /*linkageNameChildNum*/ -1,
               name) {

    #ifndef NDEBUG
      for (auto ast : declOrComments()) {
        assert(ast->isEnumElement() || ast->isComment());
      }

      if (attributes()) {
        assert(declOrCommentChildNum() > 0);
      }
    #endif
  }

  int declOrCommentChildNum() const {
    return attributes() ? 1 : 0;
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Enum* lhs = this;
    const Enum* rhs = (const Enum*) other;
    return lhs->typeDeclContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    typeDeclMarkUniqueStringsInner(context);
  }

 public:
  ~Enum() override = default;

  static owned<Enum> build(Builder* builder, Location loc,
                           owned<Attributes> attributes,
                           Decl::Visibility vis,
                           UniqueString name,
                           AstList stmts);

  /**
    Return a way to iterate over the EnumElements and Comments.
   */
  AstListIteratorPair<Expression> declOrComments() const {
    auto begin = declOrCommentChildNum() >= 0
          ? children_.begin() + declOrCommentChildNum()
          : children_.end();
    auto end = begin + numDeclOrComments();
    return AstListIteratorPair<Expression>(begin, end);
  }

  /**
   Return the number of EnumElements and Comments contained in this Enum.
   */
  int numDeclOrComments() const {
    return attributes() ? numChildren() - 1 : numChildren();
  }
  /**
   Return the i'th EnumElement or Comment in this Enum.
   */
  const Expression* declOrComment(int i) const {
    assert(0 <= i && i < numDeclOrComments());
    const AstNode* ast = this->child(declOrCommentChildNum() + i);
    assert(ast->isDecl() || ast->isComment());
    return (const Expression*)ast;
  }

  /**
   Return a way to iterate over the EnumElements (ignoring Comments)
   */
  AstListNoCommentsIteratorPair<EnumElement> enumElements() const {
    auto begin = declOrCommentChildNum() >= 0
          ? children_.begin() + declOrCommentChildNum()
          : children_.end();
    auto end = begin + numDeclOrComments();
    return AstListNoCommentsIteratorPair<EnumElement>(begin, end);
  }
};


} // end namespace uast
} // end namespace chpl

#endif
