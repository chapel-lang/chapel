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

#ifndef CHPL_UAST_DOMAIN_H
#define CHPL_UAST_DOMAIN_H

#include "chpl/queries/Location.h"
#include "chpl/uast/AstNode.h"

namespace chpl {
namespace uast {


/**
  This class represents a domain expression. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      var d = { 'foo', 'bar', 'baz' };

  \endrst

  A domain expression will never contain comments.
 */
class Domain final : public AstNode {
 private:
  bool usedCurlyBraces_;

  // TODO: Record if initializer list has trailing comma?
  Domain(AstList children, bool usedCurlyBraces)
    : AstNode(asttags::Domain, std::move(children)),
      usedCurlyBraces_(usedCurlyBraces) {
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Domain* rhs = (const Domain*) other;
    return this->usedCurlyBraces_ == rhs->usedCurlyBraces_;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

 public:
  ~Domain() override = default;

  /**
   Create and return a Domain expression.
   */
  static owned<Domain> build(Builder* builder, Location loc,
                             bool usedCurlyBraces,
                             AstList exprs);

  /**
    Return a way to iterate over the expressions of this domain.
  */
  AstListIteratorPair<AstNode> exprs() const {
    return AstListIteratorPair<AstNode>(children_.begin(),
                                           children_.end());
  }

  /**
    Return the number of expressions in this domain.
  */
  int numExprs() const {
    return this->numChildren();
  }

  /**
    Return the i'th expression in this domain.
  */
  const AstNode* expr(int i) const {
    const AstNode* ast = this->child(i);
    return ast;
  }

  /**
    Return 'true' if this domain was constructed with curly braces.
  */
  bool usedCurlyBraces() const {
    return usedCurlyBraces_;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
