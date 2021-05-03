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

#ifndef CHPL_UAST_VARIABLE_H
#define CHPL_UAST_VARIABLE_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Sym.h"

namespace chpl {
namespace uast {


class Builder;

/**
  This class represents a variable. For example:

  \rst
  .. code-block:: chapel

      var a = 1;
      ref b = a;
      const c = 2;
      const ref d = c;
      param e = "hi";
  \endrst

  each of these is a VariableDecl that refers to a Variable Sym.
 */
class Variable final : public Sym {
 friend class Builder;
 friend class VariableDecl;

 public:
  enum Tag {
    VAR,
    CONST,
    CONST_REF,
    REF,
    PARAM
  };

 private:
  Tag tag_;
  // TODO: Can we use an instance of a class that means "nothing",
  // or maybe nullptr in the children?
  int8_t typeExpChildNum;
  int8_t initExpChildNum;

  Variable(ASTList children,
           UniqueString name, Sym::Visibility vis, Tag tag,
           int8_t typeExpChildNum, int8_t initExpChildNum);
  bool contentsMatchInner(const ASTBase* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~Variable() override = default;
  const Tag tag() const { return this->tag_; }
  /**
    Returns the type expression used in the variable's declaration, or nullptr
    if there wasn't one.
    */
  const Exp* typeExp() const {
    if (typeExpChildNum >= 0) {
      const ASTBase* ast = this->child(typeExpChildNum);
      assert(ast->isExp());
      return (const Exp*)ast;
    } else {
      return nullptr;
    }
  }
  /**
    Returns the init expression used in the variable's declaration, or nullptr
    if there wasn't one.
    */
  const Exp* initExp() const {
    if (initExpChildNum >= 0) {
      const ASTBase* ast = this->child(initExpChildNum);
      assert(ast->isExp());
      return (const Exp*)ast;
    } else {
      return nullptr;
    }
  }
};


} // end namespace uast
} // end namespace chpl

#endif
