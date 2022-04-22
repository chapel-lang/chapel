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


  Reduce(AstList children)
      : Call(asttags::Reduce, std::move(children),
      /*hasCalledExpression*/ false) {
    assert(numChildren() == 2);
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Reduce* rhs = other->toReduce();
    return
        this->opExpr() == rhs->opExpr() &&
        this->callContentsMatchInner(rhs);
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
                             owned<AstNode> lhs,
                             owned<AstNode> expr);

  /**
    Returns the reduction operator. It may be either a regular operator
    (e.g. '+', '-') or the name of a class.
  */
  UniqueString op() const {
    if (this->child(1)->isIdentifier()) {
      return this->child(1)->toIdentifier()->name();
    } else {
      return this->child(1)->toFnCall()->calledExpression()->toIdentifier()->name();
    }
  }

  /**
   Returns the op expression, which may be an Identifier or a FnCall
   A FnCall here should only have one actual, which is an Identifier specifying
   the input type. However, some tests include multiple actuals here so we allow
   it here as well.
   An opExpr specified in `(minmax(int) reduce sum)` is `minmax(int)`
*/
  const AstNode* opExpr() const {
    return this->child(1);
  }

};


} // end namespace uast
} // end namespace chpl

#endif
