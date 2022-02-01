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

#ifndef CHPL_UAST_FUNCTION_H
#define CHPL_UAST_FUNCTION_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/IntentList.h"
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
  };

  enum ReturnIntent {
    // Use IntentList here for consistent enum values.
    DEFAULT_RETURN_INTENT   = (int) IntentList::DEFAULT_INTENT,
    CONST                   = (int) IntentList::CONST_VAR,
    CONST_REF               = (int) IntentList::CONST_REF,
    REF                     = (int) IntentList::REF,
    PARAM                   = (int) IntentList::PARAM,
    TYPE                    = (int) IntentList::TYPE
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

  Function(ASTList children,
           int attributesChildNum,
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
                attributesChildNum,
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

    assert(-1 <= formalsChildNum_ &&
                 formalsChildNum_ < (ssize_t)children_.size());
    assert(-1 <= thisFormalChildNum_ &&
                 thisFormalChildNum_ < (ssize_t)children_.size());
    assert(0 <= numFormals_ &&
                numFormals_ <= (ssize_t)children_.size());
    assert(-1 <= returnTypeChildNum_ &&
                 returnTypeChildNum_ < (ssize_t)children_.size());
    assert(-1 <= whereChildNum_ &&
                 whereChildNum_ < (ssize_t)children_.size());
    assert(-1 <= lifetimeChildNum_ &&
                 lifetimeChildNum_ < (ssize_t)children_.size());
    assert(0 <= numLifetimeParts_ &&
                numLifetimeParts_ <= (ssize_t)children_.size());

    if (bodyChildNum_ >= 0) {
      assert(bodyChildNum_ < (ssize_t)children_.size());
      assert(children_[bodyChildNum_]->isBlock());
    } else {
      assert(bodyChildNum_ == -1);
    }
    assert(isExpressionASTList(children_));

    for (auto decl : formals()) {
      bool isAcceptableDecl = decl->isFormal() || decl->isVarArgFormal() ||
                              decl->isTupleDecl();
      assert(isAcceptableDecl);
    }
  }

  bool contentsMatchInner(const ASTNode* other) const override {
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

 public:
  ~Function() override = default;

  static owned<Function> build(Builder* builder, Location loc,
                               owned<Attributes> attributes,
                               Decl::Visibility vis,
                               Decl::Linkage linkage,
                               owned<Expression> linkageName,
                               UniqueString name,
                               bool inline_,
                               bool override_,
                               Function::Kind kind,
                               owned<Formal> receiver,
                               Function::ReturnIntent returnIntent,
                               bool throws,
                               bool primaryMethod,
                               bool parenless,
                               ASTList formals,
                               owned<Expression> returnType,
                               owned<Expression> where,
                               ASTList lifetime,
                               owned<Block> body);

  Kind kind() const { return this->kind_; }
  ReturnIntent returnIntent() const { return this->returnIntent_; }
  bool isInline() const { return this->inline_; }
  bool isOverride() const { return this->override_; }
  bool throws() const { return this->throws_; }
  bool isPrimaryMethod() const { return primaryMethod_; }
  bool isParenless() const { return parenless_; }

  /**
   Return a way to iterate over the formals, including the method
   receiver, if present, as the first formal. This iterator may yield
   nodes of type Formal, TupleDecl, or VarArgFormal.
   */
  ASTListIteratorPair<Decl> formals() const {
    if (numFormals() == 0) {
      return ASTListIteratorPair<Decl>(children_.end(), children_.end());
    } else {
      auto start = children_.begin() + formalsChildNum_;
      return ASTListIteratorPair<Decl>(start, start + numFormals_);
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
    assert(numFormals_ > 0 && formalsChildNum_ >= 0);
    assert(0 <= i && i < numFormals_);
    auto ret = this->child(formalsChildNum_ + i);
    assert(ret->isFormal() || ret->isVarArgFormal() || ret->isTupleDecl());
    return (const Decl*)ret;
  }

  /**
   Returns the Formal for the 'this' formal argument,
   or 'nullptr' if there is none.
   */
  const Formal* thisFormal() const {
    if (thisFormalChildNum_ >= 0) {
      const ASTNode* ast = this->child(thisFormalChildNum_);
      assert(ast->isFormal());
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
  const Expression* returnType() const {
    if (returnTypeChildNum_ >= 0) {
      const ASTNode* ast = this->child(returnTypeChildNum_);
      assert(ast->isExpression());
      return (Expression*) ast;
    } else {
      return nullptr;
    }
  }

  /**
   Returns the expression for the where clause or nullptr if there was none.
   */
  const Expression* whereClause() const {
    if (whereChildNum_ >= 0) {
      const ASTNode* ast = this->child(whereChildNum_);
      assert(ast->isExpression());
      return (const Expression*) ast;
    } else {
      return nullptr;
    }
  }

  /**
   Return a way to iterate over the lifetime clauses.
   */
  ASTListIteratorPair<Expression> lifetimeClauses() const {
    if (numLifetimeClauses() == 0) {
      return ASTListIteratorPair<Expression>(children_.end(), children_.end());
    } else {
      auto start = children_.begin() + lifetimeChildNum_;
      return ASTListIteratorPair<Expression>(start, start + numLifetimeParts_);
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
  const Expression* lifetimeClause(int i) const {
    assert(numLifetimeClauses() > 0 && lifetimeChildNum_ >= 0);
    assert(0 <= i && i < numLifetimeClauses());
    const ASTNode* ast = this->child(lifetimeChildNum_ + i);
    assert(ast->isExpression());
    return (const Expression*) ast;
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
  ASTListIteratorPair<Expression> stmts() const {
    const Block* b = body();
    if (b == nullptr) {
      return ASTListIteratorPair<Expression>(children_.end(), children_.end());
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
  const Expression* stmt(int i) const {
    const Block* b = body();
    assert(b != nullptr);
    return b->stmt(i);
  }
};


} // end namespace uast
/// \cond DO_NOT_DOCUMENT
template<> struct stringify<chpl::uast::Function::Kind> {
  void operator()(std::ostream& streamOut,
                  chpl::StringifyKind stringKind,
                  const chpl::uast::Function::Kind& stringMe) const {
    streamOut << "uast:Function::Kind is not stringified";
  }
};
/// \endcond DO_NOT_DOCUMENT


} // end namespace chpl

namespace std {
  template<> struct hash<chpl::uast::Function::Kind> {
    inline size_t operator()(const chpl::uast::Function::Kind& key) const {
      return (size_t) key;
    }
  };
} // end namespace std

#endif
