/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/Location.h"
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
  Enum(AstList children, int attributeGroupChildNum, Decl::Visibility vis,
       UniqueString name)
    : TypeDecl(asttags::Enum, std::move(children), attributeGroupChildNum,
               vis,
               Decl::DEFAULT_LINKAGE,
               /*linkageNameChildNum*/ NO_CHILD,
               name) {

    #ifndef NDEBUG
      for (auto ast : declOrComments()) {
        CHPL_ASSERT(ast->isEnumElement() || ast->isComment());
      }

      if (attributeGroup()) {
        CHPL_ASSERT(declOrCommentChildNum() > 0);
      }
    #endif
  }

  Enum(Deserializer& des)
    : TypeDecl(asttags::Enum, des) {}

  int declOrCommentChildNum() const {
    return attributeGroup() ? 1 : 0;
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
                           owned<AttributeGroup> attributeGroup,
                           Decl::Visibility vis,
                           UniqueString name,
                           AstList stmts);

  /**
    Return a way to iterate over the EnumElements and Comments.
   */
  AstListIteratorPair<AstNode> declOrComments() const {
    auto begin = declOrCommentChildNum() >= 0
          ? children_.begin() + declOrCommentChildNum()
          : children_.end();
    auto end = begin + numDeclOrComments();
    return AstListIteratorPair<AstNode>(begin, end);
  }

  /**
   Return the number of EnumElements and Comments contained in this Enum.
   */
  int numDeclOrComments() const {
    return attributeGroup() ? numChildren() - 1 : numChildren();
  }
  /**
   Return the i'th EnumElement or Comment in this Enum.
   */
  const AstNode* declOrComment(int i) const {
    CHPL_ASSERT(0 <= i && i < numDeclOrComments());
    const AstNode* ast = this->child(declOrCommentChildNum() + i);
    CHPL_ASSERT(ast->isDecl() || ast->isComment());
    return ast;
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

  void serialize(Serializer& ser) const override {
    TypeDecl::serialize(ser);
  }

  DECLARE_STATIC_DESERIALIZE(Enum);

};


} // end namespace uast
} // end namespace chpl

#endif
