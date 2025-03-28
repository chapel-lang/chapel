/*
 * Copyright 2025-2025 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_ARRAY_ROW_H
#define CHPL_UAST_ARRAY_ROW_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"
#include <iterator>

namespace chpl {
namespace uast {


/**
  This class represents a dimension of a multi-dimensional array. For example,
  this Chapel code is an array with two array rows:

  \rst
  .. code-block:: chapel

      // Example 1:
      var a = [ 1, 2, 3 ; 4, 5, 6 ];

  \endrst


  An array row expression will never contain comments.
 */
class ArrayRow final : public AstNode {
 friend class AstNode;

 private:

  ArrayRow(AstList children)
    : AstNode(asttags::ArrayRow, std::move(children)) {
  }

  void serializeInner(Serializer& ser) const override {
  }

  explicit ArrayRow(Deserializer& des)
    : AstNode(asttags::ArrayRow, des) {
  }

  bool contentsMatchInner(const AstNode* other) const override {
    return true;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

 public:
  ~ArrayRow() override = default;

  /**
   Create and return an ArrayRow expression.
   */
  static owned<ArrayRow> build(Builder* builder, Location loc, AstList exprs);

  /**
    Return a way to iterate over the expressions of this array row.
  */
  AstListIteratorPair<AstNode> exprs() const {
    return AstListIteratorPair<AstNode>(children_.begin(), children_.end());
  }

  /**
    Return the number of expressions in this array row.
  */
  int numExprs() const {
    return this->numChildren();
  }

  /**
    Return the i'th expression in this array row.
  */
  const AstNode* expr(int i) const {
    const AstNode* ast = this->child(i);
    return ast;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
