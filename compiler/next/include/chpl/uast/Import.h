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

#ifndef CHPL_UAST_IMPORT_H
#define CHPL_UAST_IMPORT_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/VisibilityClause.h"

namespace chpl {
namespace uast {


/**
  This class represents an import statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      import Foo, Bar as A;

  \endrst

  This creates an import statement that has two visibility clauses, 'Foo'
  and 'Bar as A'.
*/
class Import final : public Expression {
 private:
  Import(ASTList children, Decl::Visibility visibility)
    : Expression(asttags::Import, std::move(children)),
      visibility_(visibility) {
    assert(numChildren() >= 1);

    for (auto vc : visibilityClauses()) {
      bool acceptable = vc->limitationKind() == VisibilityClause::NONE ||
                        vc->limitationKind() == VisibilityClause::BRACES;
      assert(acceptable);
    }
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const Import* rhs = other->toImport();
    return this->visibility_ == rhs->visibility_ &&
      this->expressionContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

  Decl::Visibility visibility_;

 public:

  /**
    Create and return an import statement.
  */
  static owned<Import> build(Builder* builder, Location loc,
                             Decl::Visibility visibility,
                             ASTList visibilityClauses);

  /**
    Return the visibility of this import statement.
  */
  Decl::Visibility visibility() const {
    return visibility_;
  }

  /**
    Return a way to iterate over the visibility clauses.
  */
  ASTListIteratorPair<VisibilityClause> visibilityClauses() const {
    return ASTListIteratorPair<VisibilityClause>(children_.begin(),
                                                 children_.end());
  }

  /**
    Return the number of visibility clauses in this import statement.
  */
  int numVisibilityClauses() const {
    return this->numChildren();
  }

  /**
    Return the i'th visibility clause in this import statement.
  */
  const VisibilityClause* visibilityClause(int i) const {
    auto ret = this->child(i);
    assert(ret->isVisibilityClause());
    return (const VisibilityClause*)ret;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
