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

#ifndef CHPL_UAST_PRIMCALL_H
#define CHPL_UAST_PRIMCALL_H

#include "chpl/queries/Location.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/uast/Call.h"
#include "chpl/uast/PrimOp.h"

namespace chpl {
namespace uast {


/**
  This class represents a call to a primitive
  (which only appears in low-level code).

  \rst
  .. code-block:: chapel

      __primitive("=", x, y)

  \endrst


 */
class PrimCall final : public Call {
 private:
  // which primitive
  PrimitiveTag prim_;

  PrimCall(ASTList children, PrimitiveTag prim)
    : Call(asttags::PrimCall, std::move(children),
           /* hasCalledExpression */ false),
      prim_(prim) {
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const PrimCall* lhs = this;
    const PrimCall* rhs = (const PrimCall*) other;

    if (lhs->prim_ != rhs->prim_)
      return false;

    if (!lhs->callContentsMatchInner(rhs))
      return false;

    return true;
  }
  void markUniqueStringsInner(Context* context) const override {
    callMarkUniqueStringsInner(context);
  }

 public:
  ~PrimCall() override = default;
  static owned<PrimCall> build(Builder* builder,
                               Location loc,
                               PrimitiveTag prim,
                               ASTList actuals);

  /** Returns the enum value of the primitive called */
  PrimitiveTag prim() const { return prim_; }
};


} // end namespace uast
} // end namespace chpl

#endif
