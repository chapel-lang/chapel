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

#ifndef CHPL_UAST_NUMERICLITERAL_H
#define CHPL_UAST_NUMERICLITERAL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Literal.h"

namespace chpl {
namespace uast {


/**
  This is an abstract parent class for int/real/imag numeric literals.
 */
template <typename ValueT>
class NumericLiteral : public Literal {
 protected:
  ValueT value_;
  int base_;

  NumericLiteral(ASTTag tag, ValueT value, int base)
    : Literal(tag),
      value_(value),
      base_(base)
  { }

  bool contentsMatchInner(const ASTNode* other) const override {
    const NumericLiteral<ValueT>* lhs = this;
    const NumericLiteral<ValueT>* rhs = (const NumericLiteral<ValueT>*) other;
    return lhs->literalContentsMatchInner(rhs) &&
           lhs->value_ == rhs->value_ &&
           lhs->base_ == rhs->base_;
  }
  void markUniqueStringsInner(Context* context) const override {
    literalMarkUniqueStringsInner(context);
  }

 public:
  virtual ~NumericLiteral() = 0; // this is an abstract base class

  /**
   Returns the value of this NumericLiteral.
   */
  ValueT value() const { return value_; }
 
  /**
   Returns the base of the number when it was parsed.
   */
  int base() const { return base_; }
};

template <typename ValueT>
NumericLiteral<ValueT>::~NumericLiteral() {
}

} // end namespace uast
} // end namespace chpl

#endif
