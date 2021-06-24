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
    DEFAULT_INTENT    = (int) IntentList::DEFAULT,
    CONST             = (int) IntentList::CONST,
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
  Intent intent_;

  Formal(ASTList children, UniqueString name, Formal::Intent intent,
         int8_t typeExpressionChildNum,
         int8_t initExpressionChildNum)
    : VarLikeDecl(asttags::Formal, std::move(children),
                  Decl::DEFAULT_VISIBILITY,
                  name,
                  typeExpressionChildNum,
                  initExpressionChildNum),
      intent_(intent) {}

  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~Formal() override = default;

  static owned<Formal> build(Builder* builder, Location loc,
                             UniqueString name,
                             Formal::Intent intent,
                             owned<Expression> typeExpression,
                             owned<Expression> initExpression);

  /**
   Returns the intent of the formal, e.g. in `proc f(const ref y: int)`,
   the formal `y` has intent `const ref`.
   */
  Intent intent() const { return this->intent_; }

};


} // end namespace uast
} // end namespace chpl

#endif
