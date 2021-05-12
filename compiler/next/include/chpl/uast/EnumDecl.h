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

#ifndef CHPL_UAST_ENUMDECL_H
#define CHPL_UAST_ENUMDECL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Enum.h"

namespace chpl {
namespace uast {


/**
  This class represents a declaration of an enum.
 */
class EnumDecl final : public Decl {
 private:
  EnumDecl(owned<Enum> enumSym)
    : Decl(asttags::EnumDecl, std::move(enumSym)) {
  }
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~EnumDecl() override = default;
  static owned<EnumDecl> build(Builder* builder, Location loc,
                               UniqueString name, Sym::Visibility vis,
                               ASTList stmts);

  /**
   Returns the Enum declared by this EnumDecl
   */
  const Enum* enumSym() const {
    const Sym* sym = this->sym();
    assert(sym->isEnum());
    return (const Enum*)sym;
  }

  ASTListIteratorPair<Expression> stmts() const {
    return enumSym()->stmts();
  }
  int numStmts() const {
    return enumSym()->numStmts();
  }
  const Expression* stmt(int i) const {
    return enumSym()->stmt(i);
  }

};


} // end namespace uast
} // end namespace chpl

#endif
