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

#ifndef CHPL_UAST_USE_H
#define CHPL_UAST_USE_H

#include "chpl/framework/Location.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/VisibilityClause.h"

namespace chpl {
namespace uast {


/**
  This class represents a use statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      use Foo, Bar as A;

  \endrst

  This creates a use statement that has two visibility clauses, 'Foo' and
  'Bar as A'.
*/
class Use final : public AstNode {
 private:
  Use(AstList children, Decl::Visibility visibility)
    : AstNode(asttags::Use, std::move(children)),
      visibility_(visibility) {
    CHPL_ASSERT(numChildren() >= 1);

    #ifndef NDEBUG
      if (numVisibilityClauses() == 1) {
        auto vc = visibilityClause(0);
        bool acceptable = vc->limitationKind() == VisibilityClause::NONE ||
                          vc->limitationKind() == VisibilityClause::EXCEPT ||
                          vc->limitationKind() == VisibilityClause::ONLY;
        CHPL_ASSERT(acceptable);
      } else {
        for (auto vc : visibilityClauses()) {
          CHPL_ASSERT(vc->limitationKind() == VisibilityClause::NONE);
        }
      }
    #endif
  }

  Use(Deserializer& des) : AstNode(asttags::Use, des) {
    visibility_ = des.read<Decl::Visibility>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Use* rhs = other->toUse();
    return this->visibility_ == rhs->visibility_;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  void dumpFieldsInner(const DumpSettings& s) const override;

  Decl::Visibility visibility_;

 public:

  /**
    Create and return a use statement.
  */
  static owned<Use> build(Builder* builder, Location loc,
                          Decl::Visibility visibility,
                          AstList visibilityClauses);

  /**
    Return the visibility of this use statement.
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
    Return the number of visibility clauses in this use statement.
  */
  int numVisibilityClauses() const {
    return this->numChildren();
  }

  /**
    Return the i'th visibility clause in this use statement.
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

  DECLARE_STATIC_DESERIALIZE(Use);

};


} // end namespace uast
} // end namespace chpl

#endif
