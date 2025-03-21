/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_LET_H
#define CHPL_UAST_LET_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Decl.h"

namespace chpl {
namespace uast {


/**
  This class represents a let statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      let x = 0 in writeln(x);

  \endrst

 */
class Let final : public AstNode {
 friend class AstNode;

 private:
  int numDecls_;

  Let(AstList children, int numDecls)
    : AstNode(asttags::Let, std::move(children)),
      numDecls_(numDecls) {
    CHPL_ASSERT(numChildren() >= 2);
    CHPL_ASSERT(1 <= numDecls && (numDecls == numChildren() - 1));
  }

  void serializeInner(Serializer& ser) const override {
    ser.writeVInt(numDecls_);
  }

  explicit Let(Deserializer& des)
    : AstNode(asttags::Let, des) {
    numDecls_ = des.readVInt();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Let* lhs = this;
    const Let* rhs = other->toLet();
    return lhs->numDecls_ == rhs->numDecls_;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  std::string dumpChildLabelInner(int i) const override;

 public:
  ~Let() override = default;

  /**
   Create a Let containing the passed declarations and expression.
  */
  static owned<Let> build(Builder* builder, Location loc,
                          AstList decls,
                          owned<AstNode> expression);

  /**
    Iterate over the declarations in this let statement.
  */
  AstListIteratorPair<Decl> decls() const {
    auto begin = children_.begin();
    auto end = begin + numDecls_;
    return AstListIteratorPair<Decl>(begin, end);
  }

  /**
    Return the number of declarations in this let statement.
  */
  int numDecls() const {
    return numDecls_;
  }

  /**
    Return the i'th declaration in this let statement.
  */
  const Decl* decl(int i) const {
    CHPL_ASSERT(i >= 0 && i < numDecls_);
    auto ret = child(i);
    CHPL_ASSERT(ret && ret->isDecl());
    return (const Decl*) ret;
  }

  /**
    Return the expression of this let statement.
  */
  const AstNode* expression() const {
    auto ret = child(numDecls_);
    CHPL_ASSERT(ret);
    return ret;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
