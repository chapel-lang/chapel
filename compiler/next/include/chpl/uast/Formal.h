/*
 * Copyright 2022 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_FORMAL_H
#define CHPL_UAST_FORMAL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/IntentList.h"
#include "chpl/uast/VarLikeDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents a formal. For example, `x` is a formal
  in the below:

  \rst
  .. code-block:: chapel

      proc f( x ) { }
  \endrst

  The Formals are stored inside of a Function.
 */
class Formal final : public VarLikeDecl {
 public:
  enum Intent {
    // Use IntentList here for consistent enum values.
    DEFAULT_INTENT    = (int) IntentList::DEFAULT_INTENT,
    CONST             = (int) IntentList::CONST_INTENT,
    CONST_REF         = (int) IntentList::CONST_REF,
    REF               = (int) IntentList::REF,
    IN                = (int) IntentList::IN,
    CONST_IN          = (int) IntentList::CONST_IN,
    OUT               = (int) IntentList::OUT,
    INOUT             = (int) IntentList::INOUT,
    PARAM             = (int) IntentList::PARAM,
    TYPE              = (int) IntentList::TYPE
  };

 private:
  Formal(ASTList children, int attributesChildNum, UniqueString name,
         Formal::Intent intent,
         int8_t typeExpressionChildNum,
         int8_t initExpressionChildNum)
    : VarLikeDecl(asttags::Formal, std::move(children),
                  attributesChildNum,
                  Decl::DEFAULT_VISIBILITY,
                  Decl::DEFAULT_LINKAGE,
                  /*linkageNameChildNum*/ -1,
                  name,
                  (IntentList)((int)intent),
                  typeExpressionChildNum,
                  initExpressionChildNum) {
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const Formal* lhs = this;
    const Formal* rhs = (const Formal*) other;
    return lhs->varLikeDeclContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    varLikeDeclMarkUniqueStringsInner(context);
  }

 public:
  ~Formal() override = default;

  static owned<Formal> build(Builder* builder, Location loc,
                             owned<Attributes> attributes,
                             UniqueString name,
                             Formal::Intent intent,
                             owned<Expression> typeExpression,
                             owned<Expression> initExpression);

  /**
   Returns the intent of the formal, e.g. in `proc f(const ref y: int)`,
   the formal `y` has intent `const ref`.
   */
  Intent intent() const { return (Intent)((int)storageKind()); }

};


} // end namespace uast
} // end namespace chpl

#endif
