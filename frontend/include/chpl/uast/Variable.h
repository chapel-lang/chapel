/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/Location.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/Qualifier.h"
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
 friend class AstNode;
 friend class Builder;

 public:
  enum Kind {
    // Use Qualifier here for consistent enum values.
    VAR         = (int) Qualifier::VAR,
    CONST       = (int) Qualifier::CONST_VAR,
    CONST_REF   = (int) Qualifier::CONST_REF,
    REF         = (int) Qualifier::REF,
    PARAM       = (int) Qualifier::PARAM,
    TYPE        = (int) Qualifier::TYPE,
    INDEX       = (int) Qualifier::INDEX
  };

 private:
  bool isConfig_;
  bool isField_;

  Variable(AstList children, int attributeGroupChildNum, Decl::Visibility vis,
           Decl::Linkage linkage,
           int linkageNameChildNum,
           UniqueString name,
           Variable::Kind kind,
           bool isConfig,
           bool isField,
           int8_t typeExpressionChildNum,
           int8_t initExpressionChildNum)
      : VarLikeDecl(asttags::Variable, std::move(children),
                    attributeGroupChildNum,
                    vis,
                    linkage,
                    linkageNameChildNum,
                    name,
                    (Qualifier)((int)kind),
                    typeExpressionChildNum,
                    initExpressionChildNum),
        isConfig_(isConfig),
        isField_(isField) {
  }

  void serializeInner(Serializer& ser) const override {
    varLikeDeclSerializeInner(ser);
    ser.write(isConfig_);
    ser.write(isField_);
  }

  explicit Variable(Deserializer& des)
    : VarLikeDecl(asttags::Variable, des) {
    isConfig_ = des.read<bool>();
    isField_ = des.read<bool>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Variable* lhs = this;
    const Variable* rhs = (const Variable*) other;
    return lhs->isConfig_ == rhs->isConfig_ &&
           lhs->isField_ == rhs->isField_ &&
           lhs->varLikeDeclContentsMatchInner(rhs);
  }
  void markUniqueStringsInner(Context* context) const override {
    varLikeDeclMarkUniqueStringsInner(context);
  }

  void dumpFieldsInner(const DumpSettings& s) const override;

  /**
   * Allows for setting a new initExpr when this Variable is a config
   * Can only be used while the uAST is mutable in the Builder
   */
  void setInitExprForConfig(owned<AstNode> ie);


 public:
  ~Variable() override = default;

  static owned<Variable> build(Builder* builder, Location loc,
                               owned<AttributeGroup> attributeGroup,
                               Decl::Visibility vis,
                               Decl::Linkage linkage,
                               owned<AstNode> linkageName,
                               UniqueString name,
                               Variable::Kind kind,
                               bool isConfig,
                               bool isField,
                               owned<AstNode> typeExpression,
                               owned<AstNode> initExpression);

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

DECLARE_SERDE_ENUM(uast::Variable::Kind, uint8_t);

} // end namespace chpl

#endif
