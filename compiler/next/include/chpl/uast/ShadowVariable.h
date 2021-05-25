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

#ifndef CHPL_UAST_SHADOWVARIABLE_H
#define CHPL_UAST_SHADOWVARIABLE_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/IntentList.h"
#include "chpl/uast/VarLikeDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents a shadow variable. Shadow variables are declared
  in with clauses. For example:

  \rst
  .. code-block:: chapel

      var a: atomic int;
      forall x in 1..15 with (ref a) do
        a.fetchAdd(x);
  \endrst

  Creates a shadow variable 'a' which refers to the outer variable 'a' by
  ref intent.
 */
class ShadowVariable final : public VarLikeDecl {
 public:
  enum Intent {
    // Use IntentList here for consistent enum values.
    VAR           = (int) IntentList::VAR,
    CONST         = (int) IntentList::CONST,
    CONST_REF     = (int) IntentList::CONST_REF,
    REF           = (int) IntentList::REF,
    IN            = (int) IntentList::IN,
    CONST_IN      = (int) IntentList::CONST_IN,
  };

 private:
  Intent intent_;

  ShadowVariable(ASTList children, UniqueString name,
                 ShadowVariable::Intent intent,
                 int8_t typeExpressionChildNum,
                 int8_t initExpressionChildNum)
      : VarLikeDecl(asttags::ShadowVariable, std::move(children),
                    Decl::DEFAULT_VISIBILITY,
                    name,
                    typeExpressionChildNum,
                    initExpressionChildNum),
        intent_(intent) {}

  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~ShadowVariable() override = default;

  static owned<ShadowVariable> build(Builder* builder, Location loc,
                                     UniqueString name,
                                     ShadowVariable::Intent intent,
                                     owned<Expression> typeExpression,
                                     owned<Expression> initExpression);

  /**
    Returns the intent of this shadow variable.
  */
  const Intent intent() const { return this->intent_; }

};


} // end namespace uast
} // end namespace chpl

#endif
