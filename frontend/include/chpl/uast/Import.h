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

#ifndef CHPL_UAST_IMPORT_H
#define CHPL_UAST_IMPORT_H

#include "chpl/framework/Location.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/AstNode.h"
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
class Import final : public AstNode {
 private:
  Import(AstList children, Decl::Visibility visibility)
    : AstNode(asttags::Import, std::move(children)),
      visibility_(visibility) {
    CHPL_ASSERT(numChildren() >= 1);

    #ifndef NDEBUG
      for (auto vc : visibilityClauses()) {
        bool acceptable = vc->limitationKind() == VisibilityClause::NONE ||
                          vc->limitationKind() == VisibilityClause::BRACES;
        CHPL_ASSERT(acceptable);
      }
    #endif
  }

  Import(Deserializer& des)
    : AstNode(asttags::Import, des)
      {visibility_=des.read<Decl::Visibility>();}

  bool contentsMatchInner(const AstNode* other) const override {
    const Import* rhs = other->toImport();
    return this->visibility_ == rhs->visibility_;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  Decl::Visibility visibility_;

 public:

  /**
    Create and return an import statement.
  */
  static owned<Import> build(Builder* builder, Location loc,
                             Decl::Visibility visibility,
                             AstList visibilityClauses);

  /**
    Return the visibility of this import statement.
  */
  Decl::Visibility visibility() const {
    return visibility_;
  }

  /**
    Return a way to iterate over the visibility clauses.
  */
  AstListIteratorPair<VisibilityClause> visibilityClauses() const {
    return AstListIteratorPair<VisibilityClause>(children_.begin(),
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
    CHPL_ASSERT(ret->isVisibilityClause());
    return (const VisibilityClause*)ret;
  }

  void serialize(Serializer& ser) const override {
    AstNode::serialize(ser);
    ser.write(visibility_);
  }

  DECLARE_STATIC_DESERIALIZE(Import);

};


} // end namespace uast
} // end namespace chpl

#endif
