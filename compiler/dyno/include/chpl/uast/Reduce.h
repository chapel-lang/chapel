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

namespace chpl {
namespace uast {


/**
  This class represents a reduction.
*/
class Reduce final : public Call {
 private:
  Reduce(ASTList children, UniqueString op)
    : Call(asttags::Reduce, std::move(children),
           /*hasCalledExpression*/ false),
      op_(op) {
    assert(numChildren() == 1);
    assert(!op_.isEmpty());
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Reduce* rhs = other->toReduce();
    return this->op_ == rhs->op_ &&
      this->callContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    callMarkUniqueStringsInner(context);
    op_.mark(context);
  }

  UniqueString op_;

 public:
  ~Reduce() override = default;

  /**
    Create and return a reduction.
  */
  static owned<Reduce> build(Builder* builder,
                             Location loc,
                             UniqueString op,
                             owned<Expression> expr);

  /**
    Returns the reduction operator. It may be either a regular operator
    (e.g. '+', '-') or the name of a class.
  */
  UniqueString op() const {
    return op_;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
