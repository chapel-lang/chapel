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

#ifndef CHPL_UAST_TUPLEDECL_H
#define CHPL_UAST_TUPLEDECL_H

#include "chpl/uast/Decl.h"
#include "chpl/uast/Qualifier.h"
#include "chpl/uast/Variable.h"
#include "chpl/framework/Location.h"

namespace chpl {
namespace uast {


/**
  This class represents a tuple variable declaration

  E.g.

  \rst
  .. code-block:: chapel

      var (a, b) = (1,2);
      var (c, _) = (3.0,"hi");
      var (d, e) = returnTuple();

  \endrst

  Each of the lines above is represented by a TupleDecl containing a
  list of Variables.  Note that the initial value and/or type is inferred
  from later declarations.

  Since the Tuple does not itself have a name, it is not
  a NamedDecl. Rather, it can contain NamedDecls.

 */
class TupleDecl final : public Decl {
 friend class AstNode;

 public:
  enum IntentOrKind {
    DEFAULT_INTENT = (int) Qualifier::DEFAULT_INTENT,
    CONST_INTENT   = (int) Qualifier::CONST_INTENT,
    VAR            = (int) Qualifier::VAR,
    CONST_VAR      = (int) Qualifier::CONST_VAR,
    CONST_REF      = (int) Qualifier::CONST_REF,
    REF            = (int) Qualifier::REF,
    IN             = (int) Qualifier::IN,
    CONST_IN       = (int) Qualifier::CONST_IN,
    OUT            = (int) Qualifier::OUT,
    INOUT          = (int) Qualifier::INOUT,
    INDEX          = (int) Qualifier::INDEX,
    PARAM          = (int) Qualifier::PARAM,
    TYPE           = (int) Qualifier::TYPE
  };

 private:
  TupleDecl::IntentOrKind intentOrKind_;
  int numElements_;
  int typeExpressionChildNum_;
  int initExpressionChildNum_;
  bool isTupleDeclFormal_;

  TupleDecl(AstList children, int attributeGroupChildNum, Decl::Visibility vis,
            Decl::Linkage linkage,
            IntentOrKind intentOrKind,
            int numElements,
            int typeExpressionChildNum,
            int initExpressionChildNum)
    : Decl(asttags::TupleDecl, std::move(children), attributeGroupChildNum,
           vis,
           linkage,
           /*linkageNameChildNum*/ NO_CHILD),
      intentOrKind_(intentOrKind),
      numElements_(numElements),
      typeExpressionChildNum_(typeExpressionChildNum),
      initExpressionChildNum_(initExpressionChildNum) {

    CHPL_ASSERT(assertAcceptableTupleDecl());

    isTupleDeclFormal_ = false;
    for (auto decl : decls()) {
      if (decl->isFormal()) {
        isTupleDeclFormal_ = true;
        break;
      }
    }
  }

  void serializeInner(Serializer& ser) const override {
    declSerializeInner(ser);
    ser.write(intentOrKind_);
    ser.writeVInt(numElements_);
    ser.writeVInt(typeExpressionChildNum_);
    ser.writeVInt(initExpressionChildNum_);
    ser.write(isTupleDeclFormal_);
  }

  explicit TupleDecl(Deserializer& des)
    : Decl(asttags::TupleDecl, des) {
    intentOrKind_ = des.read<IntentOrKind>();
    numElements_ = des.readVInt();
    typeExpressionChildNum_ = des.readVInt();
    initExpressionChildNum_ = des.readVInt();
    isTupleDeclFormal_ = des.read<bool>();
  }

  bool assertAcceptableTupleDecl();

  bool contentsMatchInner(const AstNode* other) const override {
    const TupleDecl* lhs = this;
    const TupleDecl* rhs = (const TupleDecl*) other;
    return lhs->declContentsMatchInner(rhs) &&
           lhs->intentOrKind_ == rhs->intentOrKind_ &&
           lhs->numElements_ == rhs->numElements_ &&
           lhs->typeExpressionChildNum_ == rhs->typeExpressionChildNum_ &&
           lhs->initExpressionChildNum_ == rhs->initExpressionChildNum_ &&
           lhs->isTupleDeclFormal_ == rhs->isTupleDeclFormal_;
  }

  void markUniqueStringsInner(Context* context) const override {
    declMarkUniqueStringsInner(context);
  }

  void dumpFieldsInner(const DumpSettings& s) const override;
  std::string dumpChildLabelInner(int i) const override;

  int declChildNum() const {
    return attributeGroup() ? 1 : 0;
  }

 public:
  ~TupleDecl() override = default;

  static owned<TupleDecl> build(Builder* builder, Location loc,
                                owned<AttributeGroup> attributeGroup,
                                Decl::Visibility vis,
                                Decl::Linkage linkage,
                                IntentOrKind intentOrKind,
                                AstList elements,
                                owned<AstNode> typeExpression,
                                owned<AstNode> initExpression);

  /**
    Returns the intent or kind of the tuple (`var` / `in` / `param` etc).
   */
  IntentOrKind intentOrKind() const { return this->intentOrKind_; }

  /**
    Return a way to iterate over the contained Decls
    (which are each Variables or TupleDecls).
   */
  AstListIteratorPair<Decl> decls() const {
    auto begin = numDecls()
        ? children_.begin()
        : children_.end() - declChildNum();
    auto end = begin + numDecls();
    return AstListIteratorPair<Decl>(begin, end);
  }

  /**
   Return the number of Decls contained within this TupleDecl.
   */
  int numDecls() const {
    return numElements_;
  }
  /**
   Return the i'th contained Decl.
   */
  const Decl* decl(int i) const {
    CHPL_ASSERT(i >= 0 && i < numDecls());
    const AstNode* ast = this->child(i);
    CHPL_ASSERT(ast->isVariable() || ast->isTupleDecl());
    CHPL_ASSERT(ast->isDecl());
    return (const Decl*)ast;
  }

  /**
    Returns the type expression used in this TupleDecl's declaration, or
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
    Returns the init expression used in this TupleDecl's declaration, or
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

  /**
    Returns a string describing the passed intentOrKind.
   */
  static const char* intentOrKindToString(IntentOrKind kind);

  /**
    Returns 'true' if this TupleDecl is a formal in a procedure.
   */
  const bool isTupleDeclFormal() const {
    return isTupleDeclFormal_;
  }
};


} // end namespace uast


DECLARE_SERDE_ENUM(uast::TupleDecl::IntentOrKind, uint8_t);


} // end namespace chpl

#endif
