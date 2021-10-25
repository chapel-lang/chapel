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

#ifndef CHPL_UAST_VARARGFORMAL_H
#define CHPL_UAST_VARARGFORMAL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/IntentList.h"
#include "chpl/uast/VarLikeDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents a varargs formal.

  \rst
  .. code-block:: chapel
      proc f(x: int ...?k) { writeln(x); }
  \endrst

  Here `x` is a formal that may take a variable number of actual arguments,
  the number of which is denoted by the TypeQuery `?k`.
*/
class VarArgFormal final : public VarLikeDecl {
 private:
  int countChildNum_;

  VarArgFormal(ASTList children, UniqueString name, Formal::Intent intent,
               int8_t typeExpressionChildNum,
               int8_t countChildNum)
    : VarLikeDecl(asttags::VarArgFormal, std::move(children),
                  Decl::DEFAULT_VISIBILITY,
                  Decl::DEFAULT_LINKAGE,
                  /*linkageNameChildNum*/ -1,
                  name,
                  (IntentList)((int)intent),
                  typeExpressionChildNum,
                  /*initExpressionChildNum*/ -1),
      countChildNum_(countChildNum) {
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const VarArgFormal* lhs = this;
    const VarArgFormal* rhs = (const VarArgFormal*) other;
    return lhs->varLikeDeclContentsMatchInner(rhs) &&
      lhs->countChildNum_ == rhs->countChildNum_;
  }

  void markUniqueStringsInner(Context* context) const override {
    varLikeDeclMarkUniqueStringsInner(context);
  }

 public:
  ~VarArgFormal() override = default;

  static owned<VarArgFormal> build(Builder* builder, Location loc,
                                   UniqueString name,
                                   Formal::Intent intent,
                                   owned<Expression> typeExpression,
                                   owned<Expression> count);

  /**
    Returns the intent of the varargs formal, e.g. in
    `proc f(ref x: int ...?k)`, the formal `x` has intent `ref`.
  */
  Formal::Intent intent() const {
    return (Formal::Intent)((int)storageKind());
  }

  /**
    Returns the count expression of the varargs formal, e.g. in
    `proc f(ref x: int ...?k)`, the count expression is the TypeQuery
    `?k`.

    If the count expression does not exist then nullptr is returned.
  */
  const Expression* count() const {
    if (countChildNum_ < 0) return nullptr;
    auto ret = child(countChildNum_);
    assert(ret->isExpression());
    return (const Expression*)ret;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
