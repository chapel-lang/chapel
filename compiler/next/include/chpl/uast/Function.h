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
    DEFAULT,
    EXTERN,
    EXPORT
  };
  enum Kind {
    PROC,
    ITER,
    OPERATOR,
  };
  enum RetIntent {
    DEFAULT, // aka value
    CONST,
    CONST_REF,
    REF,
    PARAM,
    TYPE
  };

 private:
  Linkage retTag_;
  Kind kind_;
  RetIntent retTag_;
  bool inline_;
  bool override_;
  bool throws_;

  // children store
  //   formals (FormalDecls) (starting with 'this' formal for methods)
  //   where
  //   lifetime
  //   body
  int formalsChildNum;
  int thisFormalChildNum;
  int whereChildNum;
  int lifetimeChildNum;
  int bodyChildNum;

  Function(ASTList children,
           UniqueString name, Sym::Visibility vis,
           Function::Kind kind,
           Function::RetTag retTag,
           bool throws,
           int formalsChildNum,
           int thisFormalChildNum,
           int whereChildNum,
           int lifetimeChildNum,
           int bodyChildNum)
    : Sym(asttags::Variable, std::move(children), name, vis),
      kind_(kind),
      retTag_(retTag),
      throws_(throws),
      formalsChildNum(formalsChildNum),
      thisFormalChildNum(thisFormalChildNum),
      whereChildNum(whereChildNum),
      lifetimeChildNum(lifetimeChildNum),
      bodyChildNum(bodyChildNum) {

    assert(-1 <= formalsChildNum && formalsChildNum < _children.size());
    assert(-1 <= thisFormalChildNum && thisFormalChildNum < _children.size());
    assert(-1 <= whereChildNum && whereChildNum < _children.size());
    assert(-1 <= lifetimeChildNum && lifetimeChildNum < _children.size());
    assert(-1 <= bodyChildNum && bodyChildNum < _children.size());
    assert(isExpressionASTList(children_));
  }
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~Function() override = default;

  Kind kind() const { return this->kind_; }
  RetTag retTag() const { return this->retTag_; }
  bool throws() const { return this->throws_; }

  // todo accessors for various elements
};


} // end namespace uast
} // end namespace chpl

#endif
