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

#ifndef CHPL_UAST_VARIABLE_H
#define CHPL_UAST_VARIABLE_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/IntentList.h"
#include "chpl/uast/VarLikeDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents a variable. For example:

  \rst
  .. code-block:: chapel

      var a = 1;
      ref b = a;
      const c = 2;
      const ref d = c;
      param e = "hi";

      class C {
        var f: int;
      }
  \endrst

  each of a-f are Variables.
 */
class Variable final : public VarLikeDecl {
 public:
  enum Kind {
    // Use IntentList here for consistent enum values.
    VAR         = (int) IntentList::VAR,
    CONST       = (int) IntentList::CONST_VAR,
    CONST_REF   = (int) IntentList::CONST_REF,
    REF         = (int) IntentList::REF,
    PARAM       = (int) IntentList::PARAM,
    TYPE        = (int) IntentList::TYPE,
    INDEX       = (int) IntentList::INDEX
  };

 private:
  Variable(ASTList children, int attributesChildNum, Decl::Visibility vis,
           Decl::Linkage linkage,
           int linkageNameChildNum,
           UniqueString name,
           Variable::Kind kind,
           bool isConfig,
           bool isField,
           int8_t typeExpressionChildNum,
           int8_t initExpressionChildNum)
      : VarLikeDecl(asttags::Variable, std::move(children),
                    attributesChildNum,
                    vis,
                    linkage,
                    linkageNameChildNum,
                    name,
                    (IntentList)((int)kind),
                    typeExpressionChildNum,
                    initExpressionChildNum),
        isConfig_(isConfig),
        isField_(isField) {
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const Variable* lhs = this;
    const Variable* rhs = (const Variable*) other;
    return lhs->isConfig_ == rhs->isConfig_ &&
           lhs->isField_ == rhs->isField_ &&
           lhs->varLikeDeclContentsMatchInner(rhs);
  }
  void markUniqueStringsInner(Context* context) const override {
    varLikeDeclMarkUniqueStringsInner(context);
  }

  bool isConfig_;
  bool isField_;

 public:
  ~Variable() override = default;

  static owned<Variable> build(Builder* builder, Location loc,
                               owned<Attributes> attributes,
                               Decl::Visibility vis,
                               Decl::Linkage linkage,
                               owned<Expression> linkageName,
                               UniqueString name,
                               Variable::Kind kind,
                               bool isConfig,
                               bool isField,
                               owned<Expression> typeExpression,
                               owned<Expression> initExpression);

  /**
    Returns the kind of the variable (`var` / `const` / `param` etc).
   */
  Kind kind() const { return (Kind)((int) storageKind()); }

  /**
    Returns true if this variable is a config variable.
  */
  bool isConfig() const { return this->isConfig_; }

  /**
    Returns true if this Variable represents a field.
  */
  bool isField() const { return this->isField_; }

};


} // end namespace uast
} // end namespace chpl

#endif
