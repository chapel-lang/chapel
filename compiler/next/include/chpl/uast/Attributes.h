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

#ifndef CHPL_UAST_ATTRIBUTES_H
#define CHPL_UAST_ATTRIBUTES_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Pragma.h"
#include <bitset>

namespace chpl {
namespace uast {

/**
  This class represents a collection of attributes which are associated
  with a particular declaration.

  Compiler pragmas and deprecation messages (both intended for internal
  use only) are both examples of attributes.
*/
class Attributes final : public Expression {
 private:
  std::set<PragmaTag> pragmas_;
  bool isDeprecated_;
  UniqueString deprecationMessage_;

  // TODO: Do we want to preserve the location of string literals used in
  // pragmas and deprecation messages?
  Attributes(std::set<PragmaTag> pragmas, bool isDeprecated,
             UniqueString deprecationMessage)
    : Expression(asttags::Attributes),
      pragmas_(std::move(pragmas)),
      isDeprecated_(isDeprecated),
      deprecationMessage_(deprecationMessage) {
    if (!deprecationMessage_.isEmpty()) {
      assert(isDeprecated_);
    }

    // This might already be a compile-time invariant? Not sure...
    assert(pragmas_.size() <= NUM_KNOWN_PRAGMAS);
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const Attributes* rhs = (const Attributes*)other;
    return this->pragmas_ == rhs->pragmas_ &&
      this->isDeprecated_ == rhs->isDeprecated_ &&
      this->deprecationMessage_ == rhs->deprecationMessage_ &&
      expressionContentsMatchInner(other->toExpression());
  }

  void markUniqueStringsInner(Context* context) const override {
    deprecationMessage_.mark(context);
    expressionMarkUniqueStringsInner(context);
  }

 public:
  ~Attributes() override = default;

  static owned<Attributes> build(Builder* builder, Location loc,
                                 std::set<PragmaTag> pragmas,
                                 bool isDeprecated,
                                 UniqueString deprecationMessage);

  /**
    Returns true if the given pragma is set for this attributes.
  */
  bool hasPragma(PragmaTag tag) const {
    assert(tag >= 0 && tag < NUM_KNOWN_PRAGMAS);
    return pragmas_.find(tag) != pragmas_.end();
  }

  /// \cond DO_NOT_DOCUMENT
  class PragmaIter {
    using PragmaGroup = std::set<PragmaTag>;
    PragmaGroup::const_iterator begin_;
    PragmaGroup::const_iterator end_;

  public:
    ~PragmaIter() = default;
    PragmaIter(PragmaGroup::const_iterator begin,
               PragmaGroup::const_iterator end)
      : begin_(std::move(begin)),
        end_(std::move(end)) {
    }

    PragmaGroup::const_iterator begin() const { return begin_; }
    PragmaGroup::const_iterator end() const { return end_; }
  };
  /// \endcond

  /**
    Iterate over the pragmas stored in this attributes.
  */
  PragmaIter pragmas() const {
    return PragmaIter(pragmas_.begin(), pragmas_.end());
  }

  /**
    Returns true if the declaration associated with this attributes is
    deprecated.
  */
  bool isDeprecated() const {
    return isDeprecated_;
  }

  /**
    Returns a deprecation message, or the empty string if it is not set.
  */
  UniqueString deprecationMessage() const {
    return deprecationMessage_;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
