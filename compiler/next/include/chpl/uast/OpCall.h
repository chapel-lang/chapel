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

#ifndef CHPL_UAST_OPCALL_H
#define CHPL_UAST_OPCALL_H

#include "chpl/queries/Location.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/uast/Call.h"

namespace chpl {
namespace uast {


/**
  This class represents a call to an operator.

  For example `a + b` and `x = y` are calls to operators (where `+` and `=` are
  the operators called).

 */
class OpCall final : public Call {
 private:
  // which operator
  UniqueString op_;

  OpCall(ASTList children, UniqueString op)
    : Call(asttags::OpCall, std::move(children),
           /* hasCalledExpression */ false),
      op_(op) {
  }
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~OpCall() override = default;
  static owned<OpCall> build(Builder* builder,
                             Location loc,
                             UniqueString op,
                             owned<Expression> lhs,
                             owned<Expression> rhs);
  static owned<OpCall> build(Builder* builder,
                             Location loc,
                             UniqueString op,
                             owned<Expression> expr);

  /** Returns the name of the operator called */
  UniqueString op() const { return op_; }
  /** Returns true if this is a binary operator */
  bool isBinaryOp() const { return children_.size() == 2; }
  /** Returns true if this is a unary operator */
  bool isUnaryOp() const { return children_.size() == 1; }
};


} // end namespace uast
} // end namespace chpl

#endif
