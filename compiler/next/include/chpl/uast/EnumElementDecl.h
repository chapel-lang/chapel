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

#ifndef CHPL_UAST_ENUMELEMENTDECL_H
#define CHPL_UAST_ENUMELEMENTDECL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/SymDecl.h"
#include "chpl/uast/EnumElement.h"

namespace chpl {
namespace uast {


/**
  This class represents a declaration of an enum element.
 */
class EnumElementDecl final : public SymDecl {
 private:
  EnumElementDecl(owned<EnumElement> enumElement)
    : SymDecl(asttags::EnumElementDecl, std::move(enumElement)) {
  }
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~EnumElementDecl() override = default;
  static owned<EnumElementDecl> build(Builder* builder, Location loc,
                                      UniqueString name,
                                      owned<Expression> initExpression);
  static owned<EnumElementDecl> build(Builder* builder, Location loc,
                                      UniqueString name);

  const EnumElement* enumElement() const {
    const Sym* sym = this->sym();
    assert(sym->isEnumElement());
    return (const EnumElement*)sym;
  }
  const Expression* initExpression() const {
    return this->enumElement()->initExpression();
  }
};


} // end namespace uast
} // end namespace chpl

#endif
