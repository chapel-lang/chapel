/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_FUNCTION_H
#define CHPL_UAST_FUNCTION_H

#include "chpl/framework/Location.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/Qualifier.h"
#include "chpl/uast/NamedDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents a function. For example:

  \rst
  .. code-block:: chapel

      proc f(arg) { }

      proc g(x: int = 32) where something() { }

      iter myiter() { }

      operator =(ref lhs, rhs) { }
  \endrst

  each of these is a Function.
 */
class Function final : public NamedDecl {
 public:
  enum Kind {
    PROC,
    ITER,
    OPERATOR,
    LAMBDA
  };

  enum ReturnIntent {
    // Use Qualifier here for consistent enum values.
    DEFAULT_RETURN_INTENT   = (int) Qualifier::DEFAULT_INTENT,
    CONST                   = (int) Qualifier::CONST_VAR,
    CONST_REF               = (int) Qualifier::CONST_REF,
    REF                     = (int) Qualifier::REF,
    PARAM                   = (int) Qualifier::PARAM,
    TYPE                    = (int) Qualifier::TYPE
  };

 private:
  bool inline_;
  bool override_;
  Kind kind_;
  ReturnIntent returnIntent_;
  bool throws_;
  bool primaryMethod_;
  bool parenless_;

  // children store
  //   formals (starting with 'this' formal for methods)
  //   return type
  //   where
  //   lifetime
  //   body
  int formalsChildNum_;
  int thisFormalChildNum_;
  int numFormals_; // includes 'this' formal for methods
  int returnTypeChildNum_;
  int whereChildNum_;
  int lifetimeChildNum_;
  int numLifetimeParts_;
  int bodyChildNum_;

  Function(AstList children,
           int attributeGroupChildNum,
           Decl::Visibility vis,
           Decl::Linkage linkage,
           UniqueString name,
           bool inline_,
           bool override_,
           Kind kind,
           ReturnIntent returnIntent,
           bool throws,
           bool primaryMethod,
           bool parenless,
           int linkageNameChildNum,
           int formalsChildNum,
           int thisFormalChildNum,
           int numFormals,
           int returnTypeChildNum,
           int whereChildNum,
           int lifetimeChildNum,
           int numLifetimeParts,
           int bodyChildNum)
    : NamedDecl(asttags::Function, std::move(children),
                attributeGroupChildNum,
                vis,
                linkage,
                linkageNameChildNum,
                name),
      inline_(inline_),
      override_(override_),
      kind_(kind),
      returnIntent_(returnIntent),
      throws_(throws),
      primaryMethod_(primaryMethod),
      parenless_(parenless),
      formalsChildNum_(formalsChildNum),
      thisFormalChildNum_(thisFormalChildNum),
      numFormals_(numFormals),
      returnTypeChildNum_(returnTypeChildNum),
      whereChildNum_(whereChildNum),
      lifetimeChildNum_(lifetimeChildNum),
      numLifetimeParts_(numLifetimeParts),
      bodyChildNum_(bodyChildNum) {

    CHPL_ASSERT(NO_CHILD <= formalsChildNum_ &&
                 formalsChildNum_ < (ssize_t)children_.size());
    CHPL_ASSERT(NO_CHILD <= thisFormalChildNum_ &&
                 thisFormalChildNum_ < (ssize_t)children_.size());
    CHPL_ASSERT(0 <= numFormals_ &&
                numFormals_ <= (ssize_t)children_.size());
    CHPL_ASSERT(NO_CHILD <= returnTypeChildNum_ &&
                 returnTypeChildNum_ < (ssize_t)children_.size());
    CHPL_ASSERT(NO_CHILD <= whereChildNum_ &&
                 whereChildNum_ < (ssize_t)children_.size());
    CHPL_ASSERT(NO_CHILD <= lifetimeChildNum_ &&
                 lifetimeChildNum_ < (ssize_t)children_.size());
    CHPL_ASSERT(0 <= numLifetimeParts_ &&
                numLifetimeParts_ <= (ssize_t)children_.size());

    if (bodyChildNum_ >= 0) {
      CHPL_ASSERT(bodyChildNum_ < (ssize_t)children_.size());
      CHPL_ASSERT(children_[bodyChildNum_]->isBlock());
    } else {
      CHPL_ASSERT(bodyChildNum_ == NO_CHILD);
    }

    #ifndef NDEBUG
      for (auto decl : formals()) {
        bool isAcceptableDecl = decl->isFormal() || decl->isVarArgFormal() ||
                                decl->isTupleDecl();
        std::ignore = isAcceptableDecl;
        CHPL_ASSERT(isAcceptableDecl);
      }
    #endif
  }

  Function(Deserializer& des)
    : NamedDecl(asttags::Function, des) {
    inline_        = des.read<bool>();
    override_      = des.read<bool>();
    kind_          = des.read<Kind>();
    returnIntent_  = des.read<ReturnIntent>();
    throws_        = des.read<bool>();
    primaryMethod_ = des.read<bool>();
    parenless_     = des.read<bool>();

    formalsChildNum_    = (int)des.read<int32_t>();
    thisFormalChildNum_ = (int)des.read<int32_t>();
    numFormals_         = (int)des.read<int32_t>();
    returnTypeChildNum_ = (int)des.read<int32_t>();
    whereChildNum_      = (int)des.read<int32_t>();
    lifetimeChildNum_   = (int)des.read<int32_t>();
    numLifetimeParts_   = (int)des.read<int32_t>();
    bodyChildNum_       = (int)des.read<int32_t>();
  }


  bool contentsMatchInner(const AstNode* other) const override {
    const Function* lhs = this;
    const Function* rhs = (const Function*) other;
    return lhs->namedDeclContentsMatchInner(rhs) &&
           lhs->kind_ == rhs->kind_ &&
           lhs->returnIntent_ == rhs->returnIntent_ &&
           lhs->inline_ == rhs->inline_ &&
           lhs->override_ == rhs->override_ &&
           lhs->throws_ == rhs->throws_ &&
           lhs->primaryMethod_ == rhs->primaryMethod_ &&
           lhs->parenless_ == rhs->parenless_ &&
           lhs->formalsChildNum_ == rhs->formalsChildNum_ &&
           lhs->thisFormalChildNum_ == rhs->thisFormalChildNum_ &&
           lhs->numFormals_ == rhs->numFormals_ &&
           lhs->returnTypeChildNum_ == rhs->returnTypeChildNum_ &&
           lhs->whereChildNum_ == rhs->whereChildNum_ &&
           lhs->lifetimeChildNum_ == rhs->lifetimeChildNum_ &&
           lhs->numLifetimeParts_ == rhs->numLifetimeParts_ &&
           lhs->bodyChildNum_ == rhs->bodyChildNum_;
  }

  void markUniqueStringsInner(Context* context) const override {
    namedDeclMarkUniqueStringsInner(context);
  }

  void dumpFieldsInner(const DumpSettings& s) const override;
  std::string dumpChildLabelInner(int i) const override;

 public:
  ~Function() override = default;

  static owned<Function> build(Builder* builder, Location loc,
                               owned<AttributeGroup> attributeGroup,
                               Decl::Visibility vis,
                               Decl::Linkage linkage,
                               owned<AstNode> linkageName,
                               UniqueString name,
                               bool inline_,
                               bool override_,
                               Function::Kind kind,
                               owned<Formal> receiver,
                               Function::ReturnIntent returnIntent,
                               bool throws,
                               bool primaryMethod,
                               bool parenless,
                               AstList formals,
                               owned<AstNode> returnType,
                               owned<AstNode> where,
                               AstList lifetime,
                               owned<Block> body);

  Kind kind() const { return this->kind_; }
  ReturnIntent returnIntent() const { return this->returnIntent_; }
  bool isInline() const { return this->inline_; }
  bool isOverride() const { return this->override_; }
  bool throws() const { return this->throws_; }
  bool isPrimaryMethod() const { return primaryMethod_; }
  bool isParenless() const { return parenless_; }

  bool isAnonymous() const {
    auto ret = (kind() == LAMBDA || name() == "proc");
    return ret;
  }

  /**
   Return a way to iterate over the formals, including the method
   receiver, if present, as the first formal. This iterator may yield
   nodes of type Formal, TupleDecl, or VarArgFormal.
   */
  AstListIteratorPair<Decl> formals() const {
    if (numFormals() == 0) {
      return AstListIteratorPair<Decl>(children_.end(), children_.end());
    } else {
      auto start = children_.begin() + formalsChildNum_;
      return AstListIteratorPair<Decl>(start, start + numFormals_);
    }
  }

  /**
   Return the number of Formals
   */
  int numFormals() const {
    return numFormals_;
  }

  /**
   Return the i'th formal
   */
  const Decl* formal(int i) const {
    CHPL_ASSERT(numFormals_ > 0 && formalsChildNum_ >= 0);
    CHPL_ASSERT(0 <= i && i < numFormals_);
    auto ret = this->child(formalsChildNum_ + i);
    CHPL_ASSERT(ret->isFormal() || ret->isVarArgFormal() || ret->isTupleDecl());
    return (const Decl*)ret;
  }

  /**
   Returns the Formal for the 'this' formal argument,
   or 'nullptr' if there is none.
   */
  const Formal* thisFormal() const {
    if (thisFormalChildNum_ >= 0) {
      const AstNode* ast = this->child(thisFormalChildNum_);
      CHPL_ASSERT(ast->isFormal());
      const Formal* f = (const Formal*) ast;
      return f;
    } else {
      return nullptr;
    }
  }

  /**
   Returns 'true' if this Function represents a method.
   */
  bool isMethod() const {
    return thisFormal() != nullptr || isPrimaryMethod();
  }

  /**
   Returns the expression for the return type or nullptr if there was none.
   */
  const AstNode* returnType() const {
    if (returnTypeChildNum_ >= 0) {
      const AstNode* ast = this->child(returnTypeChildNum_);
      return ast;
    } else {
      return nullptr;
    }
  }

  /**
   Returns the expression for the where clause or nullptr if there was none.
   */
  const AstNode* whereClause() const {
    if (whereChildNum_ >= 0) {
      const AstNode* ast = this->child(whereChildNum_);
      return ast;
    } else {
      return nullptr;
    }
  }

  /**
   Return a way to iterate over the lifetime clauses.
   */
  AstListIteratorPair<AstNode> lifetimeClauses() const {
    if (numLifetimeClauses() == 0) {
      return AstListIteratorPair<AstNode>(children_.end(), children_.end());
    } else {
      auto start = children_.begin() + lifetimeChildNum_;
      return AstListIteratorPair<AstNode>(start, start + numLifetimeParts_);
    }
  }

  /**
   Return the number of lifetime clauses
   */
  int numLifetimeClauses() const {
    return numLifetimeParts_;
  }

  /**
   Return the i'th lifetime clause
   */
  const AstNode* lifetimeClause(int i) const {
    CHPL_ASSERT(numLifetimeClauses() > 0 && lifetimeChildNum_ >= 0);
    CHPL_ASSERT(0 <= i && i < numLifetimeClauses());
    const AstNode* ast = this->child(lifetimeChildNum_ + i);
    return ast;
  }

  /**
    Return the function's body, or nullptr if there is none.
   */
  const Block* body() const {
    if (bodyChildNum_ < 0)
      return nullptr;

    auto ret = this->child(bodyChildNum_);
    return (const Block*) ret;
  }

  /**
    Return a way to iterate over the statements in the function body.
   */
  AstListIteratorPair<AstNode> stmts() const {
    const Block* b = body();
    if (b == nullptr) {
      return AstListIteratorPair<AstNode>(children_.end(), children_.end());
    }

    return b->stmts();
  }

  /**
    Return the number of statements in the function body or 0 if there
    is no function body.
   */
  int numStmts() const {
    const Block* b = body();
    if (b == nullptr) {
      return 0;
    }

    return b->numStmts();
  }

  /**
    Return the i'th statement in the function body.
    It is an error to call this function if there isn't one.
   */
  const AstNode* stmt(int i) const {
    const Block* b = body();
    CHPL_ASSERT(b != nullptr);
    return b->stmt(i);
  }

  static const char* returnIntentToString(ReturnIntent intent);

  static const char* kindToString(Kind kind);

  void serialize(Serializer& ser) const override {
    NamedDecl::serialize(ser);
    ser.write(inline_);
    ser.write(override_);
    ser.write(kind_);
    ser.write(returnIntent_);
    ser.write(throws_);
    ser.write(primaryMethod_);
    ser.write(parenless_);

    ser.write<int32_t>(formalsChildNum_);
    ser.write<int32_t>(thisFormalChildNum_);
    ser.write<int32_t>(numFormals_);
    ser.write<int32_t>(returnTypeChildNum_);
    ser.write<int32_t>(whereChildNum_);
    ser.write<int32_t>(lifetimeChildNum_);
    ser.write<int32_t>(numLifetimeParts_);
    ser.write<int32_t>(bodyChildNum_);
  }

  DECLARE_STATIC_DESERIALIZE(Function);
};

} // end namespace uast

DECLARE_SERDE_ENUM(uast::Function::Kind, uint8_t);
DECLARE_SERDE_ENUM(uast::Function::ReturnIntent, uint8_t);


/// \cond DO_NOT_DOCUMENT
template<> struct update<uast::Function::ReturnIntent> {
  bool operator()(uast::Function::ReturnIntent& keep,
                  uast::Function::ReturnIntent& addin) const {
    return defaultUpdateBasic(keep, addin);
  }
};

template<> struct mark<uast::Function::ReturnIntent> {
  void operator()(Context* context,
                  const uast::Function::ReturnIntent& keep) const {
    // nothing to do for enum
  }
};

template<> struct stringify<uast::Function::ReturnIntent> {
  void operator()(std::ostream& streamOut,
                  StringifyKind stringKind,
                  const uast::Function::ReturnIntent& stringMe) const {
    streamOut << uast::Function::returnIntentToString(stringMe);
  }
};

template<> struct stringify<uast::Function::Kind> {
  void operator()(std::ostream& streamOut,
                  StringifyKind stringKind,
                  const uast::Function::Kind& stringMe) const {
    streamOut << uast::Function::kindToString(stringMe);
  }
};
/// \endcond DO_NOT_DOCUMENT


} // end namespace chpl

namespace std {
  template<> struct hash<chpl::uast::Function::ReturnIntent> {
    inline size_t operator()(const chpl::uast::Function::ReturnIntent& k) const{
      return (size_t) k;
    }
  };

  template<> struct hash<chpl::uast::Function::Kind> {
    inline size_t operator()(const chpl::uast::Function::Kind& key) const {
      return (size_t) key;
    }
  };
} // end namespace std

#endif
