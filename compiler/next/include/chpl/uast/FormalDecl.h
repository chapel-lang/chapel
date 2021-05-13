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

#ifndef CHPL_UAST_FORMALDECL_H
#define CHPL_UAST_FORMALDECL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/SymDecl.h"
#include "chpl/uast/Formal.h"

#include <cassert>

namespace chpl {
namespace uast {


/**
  This class represents a formal declaration..
  
  For example, the Function `f` below has
  a FormalDecl for `x` than refers to a Formal sym.

  \rst
  .. code-block:: chapel

      proc f( x ) { }
  \endrst

 */
class FormalDecl final : public SymDecl {
 private:
  FormalDecl(owned<Formal> formal)
    : SymDecl(asttags::FormalDecl, std::move(formal)) {
  }
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~FormalDecl() override = default;
  static owned<FormalDecl> build(Builder* builder, Location loc,
                                 UniqueString name,
                                 Formal::Intent intent,
                                 owned<Expression> typeExpression,
                                 owned<Expression> initExpression);

  const Formal* formal() const {
    const Sym* sym = this->sym();
    assert(sym->isFormal());
    return (Formal*)sym;
  }
  const Formal::Intent intent() const {
    return this->formal()->intent();
  }
  const Expression* typeExpression() const {
    return this->formal()->typeExpression();
  }
  const Expression* initExpression() const {
    return this->formal()->initExpression();
  }
};


} // end namespace uast
} // end namespace chpl

#endif
