/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/Location.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/Qualifier.h"
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
 friend class AstNode;

 public:
  enum Intent {
    // Use Qualifier here for consistent enum values.
    VAR           = (int) Qualifier::VAR,
    CONST         = (int) Qualifier::CONST_VAR,
    CONST_REF     = (int) Qualifier::CONST_REF,
    REF           = (int) Qualifier::REF,
    IN            = (int) Qualifier::IN,
    CONST_IN      = (int) Qualifier::CONST_IN
  };

 private:
  TaskVar(AstList children, int attributeGroupChildNum, UniqueString name,
          TaskVar::Intent intent,
          int8_t typeExpressionChildNum,
          int8_t initExpressionChildNum)
      : VarLikeDecl(asttags::TaskVar, std::move(children),
                    attributeGroupChildNum,
                    Decl::DEFAULT_VISIBILITY,
                    Decl::DEFAULT_LINKAGE,
                    /*linkageNameChildNum*/ NO_CHILD,
                    name,
                    (Qualifier)((int)intent),
                    typeExpressionChildNum,
                    initExpressionChildNum) {
  }

  void serializeInner(Serializer& ser) const override {
    varLikeDeclSerializeInner(ser);
  }

  explicit TaskVar(Deserializer& des) : VarLikeDecl(asttags::TaskVar, des) { }

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
                              owned<AttributeGroup> attributeGroup,
                              UniqueString name,
                              TaskVar::Intent intent,
                              owned<AstNode> typeExpression,
                              owned<AstNode> initExpression);

  /**
    Returns the intent of this task variable.
  */
  Intent intent() const { return (Intent)((int)storageKind()); }
};

// DECLARE_SERDE_ENUM(uast::TaskVar::Intent, uint8_t);


} // end namespace uast



} // end namespace chpl

#endif
