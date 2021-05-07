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

#ifndef CHPL_UAST_FUNCTION_H
#define CHPL_UAST_FUNCTION_H

#include "chpl/queries/Location.h"
#include "chpl/uast/FormalDecl.h"
#include "chpl/uast/Sym.h"

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

  each of these is a FunctionDecl that refers to a Function Sym.
 */
class Function final : public Sym {
 friend class FunctionDecl;

 public:
  enum Linkage {
    DEFAULT_LINKAGE,
    EXTERN,
    EXPORT
  };
  enum Kind {
    PROC,
    ITER,
    OPERATOR,
  };
  enum ReturnIntent {
    DEFAULT_RETURN_INTENT, // aka value
    CONST,
    CONST_REF,
    REF,
    PARAM,
    TYPE
  };

 private:
  Linkage linkage_;
  Kind kind_;
  ReturnIntent returnIntent_;
  bool inline_;
  bool override_;
  bool throws_;

  // children store
  //   linkageNameExpr
  //   formals (FormalDecls) (starting with 'this' formal for methods)
  //   return type
  //   where
  //   lifetime
  //   body
  int linkageNameExprChildNum;
  int formalsChildNum;
  int thisFormalChildNum;
  int numFormals_; // includes 'this' formal for methods
  int returnTypeChildNum;
  int whereChildNum;
  int lifetimeChildNum;
  int numLifetimeParts_;
  int bodyChildNum;
  int numBodyStmts;

  Function(ASTList children,
           UniqueString name, Sym::Visibility vis,
           Linkage linkage,
           Kind kind,
           ReturnIntent returnIntent,
           bool throws,
           int linkageNameExprChildNum,
           int formalsChildNum,
           int thisFormalChildNum,
           int numFormals,
           int returnTypeChildNum,
           int whereChildNum,
           int lifetimeChildNum,
           int numLifetimeParts,
           int bodyChildNum,
           int numBodyStmts)
    : Sym(asttags::Variable, std::move(children), name, vis),
      linkage_(linkage),
      kind_(kind),
      returnIntent_(returnIntent),
      throws_(throws),
      linkageNameExprChildNum(linkageNameExprChildNum),
      formalsChildNum(formalsChildNum),
      thisFormalChildNum(thisFormalChildNum),
      numFormals_(numFormals),
      returnTypeChildNum(returnTypeChildNum),
      whereChildNum(whereChildNum),
      lifetimeChildNum(lifetimeChildNum),
      numLifetimeParts_(numLifetimeParts),
      bodyChildNum(bodyChildNum),
      numBodyStmts(numBodyStmts) {

    assert(-1 <= linkageNameExprChildNum &&
                 linkageNameExprChildNum < children_.size());
    assert(-1 <= formalsChildNum && formalsChildNum < children_.size());
    assert(-1 <= thisFormalChildNum && thisFormalChildNum < children_.size());
    assert(0 <= numFormals_ && numFormals_ < children_.size());
    assert(-1 <= returnTypeChildNum && returnTypeChildNum < children_.size());
    assert(-1 <= whereChildNum && whereChildNum < children_.size());
    assert(-1 <= lifetimeChildNum && lifetimeChildNum < children_.size());
    assert(0 <= numLifetimeParts && numLifetimeParts < children_.size());
    assert(-1 <= bodyChildNum && bodyChildNum < children_.size());
    assert(0 <= numBodyStmts && numBodyStmts < children_.size());
    assert(isExpressionASTList(children_));
  }
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~Function() override = default;

  Linkage linkage() const { return this->linkage_; }
  Kind kind() const { return this->kind_; }
  ReturnIntent returnIntent() const { return this->returnIntent_; }
  bool isInline() const { return this->inline_; }
  bool isOverride() const { return this->override_; }
  bool throws() const { return this->throws_; }

  /**
   Return the linkage name expression, e.g. "f_c_name"
   in the below, or nullptr if there is none.
 
   \rst
    .. code-block:: chapel

        extern "f_c_name" proc f(arg) { }
    \endrst
   */
  const Expression* linkageNameExpression() const {
    if (linkageNameExprChildNum >= 0) {
      const ASTNode* ast = children_[linkageNameExprChildNum].get();
      assert(ast->isExpression());
      return (const Expression*) ast;
    } else {
      return nullptr;
    }
  }

  /**
   Return a way to iterate over the formals, including the method
   receiver, if present, as the first formal.
   */
  ASTListIteratorPair<Expression> formals() const {
    if (numFormals_ == 0) {
      return ASTListIteratorPair<Expression>(children_.end(), children_.end());
    } else {
      auto start = children_.begin()+formalsChildNum;
      return ASTListIteratorPair<Expression>(start, start+numFormals_);
    }
  }

  /**
   Return the number of formals
   */
  int numFormals() const {
    return numFormals_; 
  }

  /**
   Return the i'th formal
   */
  const Formal* formal(int i) const {
    assert(numFormals_ > 0 && formalsChildNum >= 0);
    assert(0 <= i && i < numFormals_);
    const ASTNode* ast = this->child(formalsChildNum + i);
    assert(ast->isFormalDecl());
    const FormalDecl* d = (FormalDecl*) ast;
    return d->formal();
  }

  /**
   Returns the expression for the return type or nullptr if there was none.
   */
  const Expression* returnType() {
    if (returnTypeChildNum >= 0) {
      const ASTNode* ast = this->child(returnTypeChildNum);
      assert(ast->isExpression());
      return (Expression*) ast; 
    } else {
      return nullptr;
    }
  }

  /**
   Returns the expression for the where clause or nullptr if there was none.
   */
  const Expression* whereClause() {
    if (whereChildNum >= 0) {
      const ASTNode* ast = this->child(whereChildNum);
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
    if (numLifetimeParts_ == 0) {
      return ASTListIteratorPair<Expression>(children_.end(), children_.end());
    } else {
      auto start = children_.begin()+lifetimeChildNum;
      return ASTListIteratorPair<Expression>(start, start+numLifetimeParts_);
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
    assert(numLifetimeParts_ > 0 && lifetimeChildNum >= 0);
    assert(0 <= i && i < numLifetimeParts_);
    const ASTNode* ast = this->child(lifetimeChildNum + i);
    assert(ast->isExpression());
    return (const Expression*) ast;
  }

  /**
    Return a way to iterate over the statements in the function body.
   */
  ASTListIteratorPair<Expression> stmts() const {
    if (numBodyStmts == 0) {
      return ASTListIteratorPair<Expression>(children_.end(), children_.end());
    } else {
      auto start = children_.begin()+bodyChildNum;
      return ASTListIteratorPair<Expression>(start, start+numBodyStmts);
    }
  }

  /**
   Return the number of statements in the function body.
   */
  int numStmts() const {
    return numBodyStmts;
  }

  /**
   Return the i'th statement in the function body.
   */
  const Expression* stmt(int i) const {
    assert(numBodyStmts > 0 && bodyChildNum >= 0);
    assert(0 <= i && i < numBodyStmts);
    const ASTNode* ast = this->child(bodyChildNum + i);
    assert(ast->isExpression());
    return (const Expression*) ast;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
