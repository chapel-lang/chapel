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

#ifndef CHPL_UAST_FORMAL_H
#define CHPL_UAST_FORMAL_H

#include "chpl/framework/global-strings.h"
#include "chpl/framework/Location.h"
#include "chpl/uast/Qualifier.h"
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
 friend class AstNode;

 public:
  enum Intent {
    // Use Qualifier here for consistent enum values.
    DEFAULT_INTENT    = (int) Qualifier::DEFAULT_INTENT,
    CONST             = (int) Qualifier::CONST_INTENT,
    CONST_REF         = (int) Qualifier::CONST_REF,
    REF               = (int) Qualifier::REF,
    IN                = (int) Qualifier::IN,
    CONST_IN          = (int) Qualifier::CONST_IN,
    OUT               = (int) Qualifier::OUT,
    INOUT             = (int) Qualifier::INOUT,
    PARAM             = (int) Qualifier::PARAM,
    TYPE              = (int) Qualifier::TYPE
  };

 private:
  Formal(AstList children, int attributeGroupChildNum, UniqueString name,
         Formal::Intent intent,
         int8_t typeExpressionChildNum,
         int8_t initExpressionChildNum)
    : VarLikeDecl(asttags::Formal, std::move(children),
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

  explicit Formal(Deserializer& des)
    : VarLikeDecl(asttags::Formal, des) {
  }

  bool contentsMatchInner(const AstNode* other) const override {
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
                             owned<AttributeGroup> attributeGroup,
                             UniqueString name,
                             Intent intent,
                             owned<AstNode> typeExpression,
                             owned<AstNode> initExpression);

  /**
   Returns the intent of the formal, e.g. in `proc f(const ref y: int)`,
   the formal `y` has intent `const ref`.
   */
  Intent intent() const { return (Intent)((int)storageKind()); }

  static const char* intentToString(Intent intent);

  /**
    If `true`, then this formal's name is '_', as in `proc(_: int)`. This is
    different from a formal that is anonymous in a type context, e.g.,
    `type T = proc(int)`, where the formal _might_ be anonymous if it is
    taken to represent a type.
  */
  inline bool isExplicitlyAnonymous() const {
    return name() == USTR("_");
  }
};


} // end namespace uast
} // end namespace chpl

#endif
