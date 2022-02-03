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

#ifndef CHPL_UAST_NUMERICLITERAL_H
#define CHPL_UAST_NUMERICLITERAL_H

#include "chpl/queries/Location.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/uast/Literal.h"

namespace chpl {
namespace uast {


/**
  This is an abstract parent class for int/real/imag numeric literals.
 */
template <typename ValueT, typename ParamT>
class NumericLiteral : public Literal {
 protected:
  UniqueString text_;

  NumericLiteral(ASTTag tag, const ParamT* value, UniqueString text)
    : Literal(tag, value),
      text_(text)
  { }

  bool contentsMatchInner(const ASTNode* other) const override {
    auto lhs = this;
    auto* rhs = (const NumericLiteral<ValueT, ParamT>*) other;
    return lhs->literalContentsMatchInner(rhs) &&
           lhs->text_ == rhs->text_;
  }
  void markUniqueStringsInner(Context* context) const override {
    literalMarkUniqueStringsInner(context);
    text_.mark(context);
  }

 public:
  virtual ~NumericLiteral() = 0; // this is an abstract base class

  /**
   Returns the value of this NumericLiteral.
   */
  ValueT value() const {
    const ParamT* p = (const ParamT*) value_;
    return p->value();
  }

  /**
   Returns the number as it was written in the source code (as a string)
   */
  UniqueString text() const { return text_; }
};

template <typename ValueT, typename ParamT>
NumericLiteral<ValueT, ParamT>::~NumericLiteral() {
}

} // end namespace uast
} // end namespace chpl

#endif
