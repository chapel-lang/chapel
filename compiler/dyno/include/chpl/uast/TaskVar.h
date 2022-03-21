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

#ifndef CHPL_UAST_TASKVAR_H
#define CHPL_UAST_TASKVAR_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/IntentList.h"
#include "chpl/uast/VarLikeDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents a task variable. Task variables are declared
  in with clauses. For example:

  \rst
  .. code-block:: chapel

      var a: atomic int;
      forall x in 1..15 with (ref a) do
        a.fetchAdd(x);
  \endrst

  Creates a task variable 'a' which refers to the outer variable 'a' by
  ref intent.
 */
class TaskVar final : public VarLikeDecl {
 public:
  enum Intent {
    // Use IntentList here for consistent enum values.
    VAR           = (int) IntentList::VAR,
    CONST         = (int) IntentList::CONST_VAR,
    CONST_REF     = (int) IntentList::CONST_REF,
    REF           = (int) IntentList::REF,
    IN            = (int) IntentList::IN,
    CONST_IN      = (int) IntentList::CONST_IN
  };

 private:
  TaskVar(ASTList children, int attributesChildNum, UniqueString name,
          TaskVar::Intent intent,
          int8_t typeExpressionChildNum,
          int8_t initExpressionChildNum)
      : VarLikeDecl(asttags::TaskVar, std::move(children),
                    attributesChildNum,
                    Decl::DEFAULT_VISIBILITY,
                    Decl::DEFAULT_LINKAGE,
                    /*linkageNameChildNum*/ -1,
                    name,
                    (IntentList)((int)intent),
                    typeExpressionChildNum,
                    initExpressionChildNum) {
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const TaskVar* lhs = this;
    const TaskVar* rhs = (const TaskVar*) other;

    return lhs->varLikeDeclContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    varLikeDeclMarkUniqueStringsInner(context);
  }

 public:
  ~TaskVar() override = default;

  static owned<TaskVar> build(Builder* builder, Location loc,
                              owned<Attributes> attributes,
                              UniqueString name,
                              TaskVar::Intent intent,
                              owned<Expression> typeExpression,
                              owned<Expression> initExpression);

  /**
    Returns the intent of this task variable.
  */
  Intent intent() const { return (Intent)((int)storageKind()); }

};


} // end namespace uast
} // end namespace chpl

#endif
