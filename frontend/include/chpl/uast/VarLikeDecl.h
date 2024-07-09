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

#ifndef CHPL_UAST_VARLIKEDECL_H
#define CHPL_UAST_VARLIKEDECL_H

#include "chpl/framework/Location.h"
#include "chpl/uast/NamedDecl.h"
#include "chpl/uast/Qualifier.h"

namespace chpl {
namespace uast {


/**
  This abstract class represents any sort of variable-like declaration.
  This includes things like fields, formals, or variables.
 */
class VarLikeDecl : public NamedDecl {
 friend class AstNode;

 protected:
  Qualifier storageKind_;
  int8_t typeExpressionChildNum_;
  int8_t initExpressionChildNum_;

  VarLikeDecl(AstTag tag, AstList children, int attributeGroupChildNum,
              Decl::Visibility vis,
              Decl::Linkage linkage,
              int linkageNameChildNum,
              UniqueString name,
              Qualifier storageKind,
              int8_t typeExpressionChildNum,
              int8_t initExpressionChildNum)
    : NamedDecl(tag, std::move(children), attributeGroupChildNum, vis,
                linkage,
                linkageNameChildNum,
                name),
      storageKind_(storageKind),
      typeExpressionChildNum_(typeExpressionChildNum),
      initExpressionChildNum_(initExpressionChildNum) {

    // Linkage name can be a child num.
    CHPL_ASSERT(numChildren() <= 3);
    if (typeExpressionChildNum >= 0) {
      CHPL_ASSERT(typeExpressionChildNum <= 3);
    }
    if (initExpressionChildNum >= 0) {
      CHPL_ASSERT(initExpressionChildNum <= 3);
    }
  }

  void varLikeDeclSerializeInner(Serializer& ser) const {
    namedDeclSerializeInner(ser);
    ser.write(storageKind_);
    ser.write(typeExpressionChildNum_);
    ser.write(initExpressionChildNum_);
  }

  VarLikeDecl(AstTag tag, Deserializer& des)
    : NamedDecl(tag, des) {
    storageKind_ = des.read<Qualifier>();
    typeExpressionChildNum_ = des.read<int8_t>();
    initExpressionChildNum_ = des.read<int8_t>();
  }

  bool varLikeDeclContentsMatchInner(const AstNode* other) const {
    const VarLikeDecl* lhs = this;
    const VarLikeDecl* rhs = (const VarLikeDecl*) other;
    return lhs->namedDeclContentsMatchInner(rhs) &&
           lhs->storageKind_ == rhs->storageKind_ &&
           lhs->typeExpressionChildNum_ == rhs->typeExpressionChildNum_ &&
           lhs->initExpressionChildNum_ == rhs->initExpressionChildNum_;
  }

  void varLikeDeclMarkUniqueStringsInner(Context* context) const {
    namedDeclMarkUniqueStringsInner(context);
  }

  virtual void dumpFieldsInner(const DumpSettings& s) const override;
  virtual std::string dumpChildLabelInner(int i) const override;

 public:
  ~VarLikeDecl() = 0; // this is an abstract base class

  /**
    Returns the storage kind (e.g. type, param, in, etc).
    Subclasses have more specific functions that return the
    subset appropriate for that subclass.
   */
  Qualifier storageKind() const {
    return storageKind_;
  }

  /**
    Returns the type expression used in this VarLikeDecl's declaration, or
    nullptr if there wasn't one.
  */
  const AstNode* typeExpression() const {
    if (typeExpressionChildNum_ >= 0) {
      const AstNode* ast = this->child(typeExpressionChildNum_);
      return ast;
    } else {
      return nullptr;
    }
  }

  /**
    Returns the init expression used in this VarLikeDecl's declaration, or
    nullptr if there wasn't one.
  */
  const AstNode* initExpression() const {
    if (initExpressionChildNum_ >= 0) {
      const AstNode* ast = this->child(initExpressionChildNum_);
      return ast;
    } else {
      return nullptr;
    }
  }
};


} // end namespace uast

} // end namespace chpl

#endif
