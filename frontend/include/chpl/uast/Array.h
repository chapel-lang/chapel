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

#ifndef CHPL_UAST_ARRAY_H
#define CHPL_UAST_ARRAY_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"

namespace chpl {
namespace uast {


/**
  This class represents an array expression. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      var a = [ 1, 2, 3 ];

  \endrst

  An array expression will never contain comments.
 */
class Array final : public AstNode {
 friend class AstNode;

 private:
  bool trailingComma_,
       associative_;

  Array(AstList children, bool trailingComma, bool associative)
    : AstNode(asttags::Array, std::move(children)),
      trailingComma_(trailingComma),
      associative_(associative) {
  }

  void serializeInner(Serializer& ser) const override {
    ser.write(trailingComma_);
    ser.write(associative_);
  }

  explicit Array(Deserializer& des)
    : AstNode(asttags::Array, des) {
    trailingComma_ = des.read<bool>();
    associative_ = des.read<bool>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Array* rhs = other->toArray();
    return this->trailingComma_ == rhs->trailingComma_ &&
           this->associative_ == rhs->associative_;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  void dumpInner(const DumpSettings& s) const;

 public:
  ~Array() override = default;

  /**
   Create and return an Array expression.
   */
  static owned<Array> build(Builder* builder, Location loc,
                            AstList exprs, bool trailingComma=false,
                            bool associative=false);

  bool hasTrailingComma() const { return this->trailingComma_; }
  bool isAssociative() const { return this->associative_; }

  /**
    Return a way to iterate over the expressions of this array.
  */
  AstListIteratorPair<AstNode> exprs() const {
    return AstListIteratorPair<AstNode>(children_.begin(),
                                        children_.end());
  }

  /**
    Return the number of expressions in this array.
  */
  int numExprs() const {
    return this->numChildren();
  }

  /**
    Return the i'th expression in this array.
  */
  const AstNode* expr(int i) const {
    const AstNode* ast = this->child(i);
    return ast;
  }

  /**
    Return whether this is a multi-dimensional array.
  */
  bool isMultiDim() const {
    return this->numExprs() > 0 && this->expr(0)->isArrayRow();
  }
};


} // end namespace uast
} // end namespace chpl

#endif
