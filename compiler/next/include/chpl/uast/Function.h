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
#include "chpl/uast/Formal.h"
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
    PARAM = Formal::PARAM,
    TYPE = Formal::TYPE
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
  int linkageNameExprChildNum_;
  int formalsChildNum_;
  int thisFormalChildNum_;
  int numFormals_; // includes 'this' formal for methods
  int returnTypeChildNum_;
  int whereChildNum_;
  int lifetimeChildNum_;
  int numLifetimeParts_;
  int bodyChildNum_;
  int numBodyStmts_;

  Function(ASTList children,
           UniqueString name, Decl::Visibility vis,
           Linkage linkage,
           bool inline_,
           bool override_,
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
    : NamedDecl(asttags::Function, std::move(children), vis, name),
      linkage_(linkage),
      inline_(inline_),
      override_(override_),
      kind_(kind),
      returnIntent_(returnIntent),
      throws_(throws),
      linkageNameExprChildNum_(linkageNameExprChildNum),
      formalsChildNum_(formalsChildNum),
      thisFormalChildNum_(thisFormalChildNum),
      numFormals_(numFormals),
      returnTypeChildNum_(returnTypeChildNum),
      whereChildNum_(whereChildNum),
      lifetimeChildNum_(lifetimeChildNum),
      numLifetimeParts_(numLifetimeParts),
      bodyChildNum_(bodyChildNum),
      numBodyStmts_(numBodyStmts) {

    assert(-1 <= linkageNameExprChildNum_ &&
                 linkageNameExprChildNum_ < (ssize_t)children_.size());
    assert(-1 <= linkageNameExprChildNum_ &&
                 linkageNameExprChildNum_ < (ssize_t)children_.size());
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
    assert(-1 <= bodyChildNum_ &&
                 bodyChildNum_ < (ssize_t)children_.size());
    assert(0 <= numBodyStmts_ &&
                numBodyStmts_ <= (ssize_t)children_.size());
    assert(isExpressionASTList(children_));
  }
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~Function() override = default;

  static owned<Function> build(Builder* builder, Location loc,
                               UniqueString name, Decl::Visibility vis,
                               Function::Linkage linkage,
                               owned<Expression> linkageNameExpr,
                               bool inline_,
                               bool override_,
                               Function::Kind kind,
                               owned<Formal> receiver,
                               Function::ReturnIntent returnIntent,
                               bool throws,
                               ASTList formals,
                               owned<Expression> returnType,
                               owned<Expression> where,
                               ASTList lifetime,
                               ASTList body);

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
    if (linkageNameExprChildNum_ >= 0) {
      const ASTNode* ast = child(linkageNameExprChildNum_);
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
  ASTListIteratorPair<Formal> formals() const {
    if (numFormals() == 0) {
      return ASTListIteratorPair<Formal>(children_.end(), children_.end());
    } else {
      auto start = children_.begin() + formalsChildNum_;
      return ASTListIteratorPair<Formal>(start, start + numFormals_);
    }
  }

  /**
   Return the number of formals
   (which is the number of formal Decls and the number of formal Syms
    since each Decl contains one Sym)
   */
  int numFormals() const {
    return numFormals_; 
  }

  /**
   Return the i'th formal
   */
  const Formal* formal(int i) const {
    assert(numFormals_ > 0 && formalsChildNum_ >= 0);
    assert(0 <= i && i < numFormals_);
    const ASTNode* ast = this->child(formalsChildNum_ + i);
    assert(ast->isFormal());
    const Formal* f = (const Formal*) ast;
    return f;
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
    Return a way to iterate over the statements in the function body.
   */
  ASTListIteratorPair<Expression> stmts() const {
    if (numBodyStmts_ == 0) {
      return ASTListIteratorPair<Expression>(children_.end(), children_.end());
    } else {
      auto start = children_.begin() + bodyChildNum_;
      return ASTListIteratorPair<Expression>(start, start + numBodyStmts_);
    }
  }

  /**
   Return the number of statements in the function body.
   */
  int numStmts() const {
    return numBodyStmts_;
  }

  /**
   Return the i'th statement in the function body.
   */
  const Expression* stmt(int i) const {
    assert(numBodyStmts_ > 0 && bodyChildNum_ >= 0);
    assert(0 <= i && i < numBodyStmts_);
    const ASTNode* ast = this->child(bodyChildNum_ + i);
    assert(ast->isExpression());
    return (const Expression*) ast;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
