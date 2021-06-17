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

#ifndef CHPL_UAST_FIELD_H
#define CHPL_UAST_FIELD_H

#include "chpl/queries/Location.h"
#include "chpl/uast/IntentList.h"
#include "chpl/uast/VarLikeDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents a field. For example, `x` is a formal
  in the below:

  \rst
  .. code-block:: chapel

      class C {
        var x: int;
      }
  \endrst

  The Fields are stored inside of Class, Record, and Union.
 */
class Field final : public VarLikeDecl {
 public:
  enum Kind {
    // Use IntentList here for consistent enum values.
    VAR         = (int) IntentList::VAR,
    CONST       = (int) IntentList::CONST,
    CONST_REF   = (int) IntentList::CONST_REF,
    REF         = (int) IntentList::REF,
    PARAM       = (int) IntentList::PARAM,
    TYPE        = (int) IntentList::TYPE,
  };

 private:
  Kind kind_;

  Field(ASTList children, Decl::Visibility vis, UniqueString name,
        Field::Kind kind,
        int8_t typeExpressionChildNum,
        int8_t initExpressionChildNum)
    : VarLikeDecl(asttags::Field, std::move(children), vis, name,
                  typeExpressionChildNum,
                  initExpressionChildNum),
      kind_(kind) {}

  bool contentsMatchInner(const ASTNode* other) const override
  {
    const Field* lhs = this;
    const Field* rhs = (const Field*) other;
    return lhs->varLikeDeclContentsMatchInner(rhs) &&
           lhs->kind_ == rhs->kind_;
  }
  void markUniqueStringsInner(Context* context) const override {
    varLikeDeclMarkUniqueStringsInner(context);
  }

 public:
  ~Field() override = default;

  static owned<Field> build(Builder* builder, Location loc,
                            UniqueString name, Decl::Visibility vis,
                            Field::Kind kind,
                            owned<Expression> typeExpression,
                            owned<Expression> initExpression);

  /**
   Returns the kind of the field, e.g. in `class C { const x; }`
   the field `x` has kind `const`.
   */
  Field::Kind kind() const { return this->kind_; }
};


} // end namespace uast
} // end namespace chpl

#endif
