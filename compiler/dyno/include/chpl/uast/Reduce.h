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

#ifndef CHPL_UAST_REDUCE_H
#define CHPL_UAST_REDUCE_H

#include "chpl/queries/Location.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/uast/Call.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/FnCall.h"

namespace chpl {
namespace uast {


/**
  This class represents a reduction.

  \rst
  .. code-block:: chapel

      // Example 1:
      var eltAvg = (+ reduce A) / size**2;

  \endrst

  Where + is the op

  Also supported are reduce intents

  \rst
  .. code-block:: chapel

      // Example 2:
      forall elm in A with (PlusReduceOp(int) reduce sum) {
        sum reduce= elm;   // bools are implicitly coerced to 'int' input type
        writeln(sum);      // accumulation state: int
      }

  \endrst

  Where PlusReduceOp(int) is a FnCall with PlusReduceOp as the op and int is
  the input type

}

*/
class Reduce final : public Call {
 private:
  static const int opChildNum_ = 0;
  static const int iterandExprChildNum_ = 1;

  Reduce(AstList children)
      : Call(asttags::Reduce, std::move(children),
             /*hasCalledExpression*/ false) {
    assert(numChildren() == 2);
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Reduce* rhs = other->toReduce();
    return this->callContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    callMarkUniqueStringsInner(context);
  }


 public:
  ~Reduce() override = default;

  /**
    Create and return a reduction.
  */
  static owned<Reduce> build(Builder* builder,
                             Location loc,
                             owned<AstNode> op,
                             owned<AstNode> iterand);

  /**
    Returns the reduce op expression, e.g. `minmax(int)` in the expression
    `minmax(int) reduce sum`.
  */
  const AstNode* op() const {
    return this->child(opChildNum_);
  }

  /**
    Returns the iterand of the reduction, e,g., `sum` in the expression
    `minmax(int) reduce sum`.
  */
  const AstNode* iterand() const {
    return this->child(iterandExprChildNum_);
  }

};


} // end namespace uast
} // end namespace chpl

#endif
