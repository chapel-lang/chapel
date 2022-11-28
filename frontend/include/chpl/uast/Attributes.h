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

#ifndef CHPL_UAST_ATTRIBUTES_H
#define CHPL_UAST_ATTRIBUTES_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Pragma.h"
#include "chpl/util/iteration.h"
#include <set>

namespace chpl {
namespace uast {

/**
  This class represents a collection of attributes which are associated
  with a particular declaration.

  Compiler pragmas and deprecation messages (both intended for internal
  use only) are both examples of attributes.
*/
class Attributes final : public AstNode {
 private:
  std::set<PragmaTag> pragmas_;
  bool isDeprecated_;
  bool isUnstable_; //new
  UniqueString deprecationMessage_;
  UniqueString unstableMessage_; //new

  // TODO: Do we want to preserve the location of string literals used in
  // pragmas and deprecation messages?
  Attributes(std::set<PragmaTag> pragmas,
             bool isDeprecated,
             bool isUnstable,
             UniqueString deprecationMessage,
             UniqueString unstableMessage)
    : AstNode(asttags::Attributes),
      pragmas_(std::move(pragmas)),
      isDeprecated_(isDeprecated),
      isUnstable_(isUnstable),
      deprecationMessage_(deprecationMessage),
      unstableMessage_(unstableMessage) {
    if (!deprecationMessage_.isEmpty()) {
      assert(isDeprecated_);
    }
    if (!unstableMessage_.isEmpty()) {
      assert(isUnstable_);
    }

    // This might already be a compile-time invariant? Not sure...
    assert(pragmas_.size() <= NUM_KNOWN_PRAGMAS);
  }

  Attributes(Deserializer& des)
    : AstNode(asttags::Attributes, des) {
      pragmas_ = des.read<std::set<PragmaTag>>();
      isDeprecated_ = des.read<bool>();
      isUnstable_ = des.read<bool>();
      deprecationMessage_ = des.read<UniqueString>();
      unstableMessage_ = des.read<UniqueString>();
    }


  bool contentsMatchInner(const AstNode* other) const override {
    const Attributes* rhs = (const Attributes*)other;
    return this->pragmas_ == rhs->pragmas_ &&
      this->isDeprecated_ == rhs->isDeprecated_ &&
      this->isUnstable_ == rhs->isUnstable_ &&
      this->deprecationMessage_ == rhs->deprecationMessage_ &&
      this->unstableMessage_ == rhs->unstableMessage_;
  }

  void markUniqueStringsInner(Context* context) const override {
    deprecationMessage_.mark(context);
    unstableMessage_.mark(context);
  }

 public:
  ~Attributes() override = default;

  static owned<Attributes> build(Builder* builder, Location loc,
                                 std::set<PragmaTag> pragmas,
                                 bool isDeprecated,
                                 bool isUnstable,
                                 UniqueString deprecationMessage,
                                 UniqueString unstableMessage);

  /**
    Returns true if the given pragma is set for this attributes.
  */
  bool hasPragma(PragmaTag tag) const {
    assert(tag >= 0 && tag < NUM_KNOWN_PRAGMAS);
    return pragmas_.find(tag) != pragmas_.end();
  }

  // An iterable over the pragmas of this.
  using PragmaIterable = Iterable<std::set<PragmaTag>>;

  /**
    Iterate over the pragmas stored in this attributes.
  */
  PragmaIterable pragmas() const {
    return PragmaIterable(pragmas_);
  }

  /**
    Returns true if the declaration associated with this attributes is
    deprecated.
  */
  bool isDeprecated() const {
    return isDeprecated_;
  }

  /**
    Returns true if the declaration associated with this attribute is
    unstable.
  */
  bool isUnstable() const {
    return isUnstable_;
  }

  /**
    Returns a deprecation message, or the empty string if it is not set.
  */
  UniqueString deprecationMessage() const {
    return deprecationMessage_;
  }

  /**
    Returns an unstable message, or the empty string if it is not set.
  */
  UniqueString unstableMessage() const {
    return unstableMessage_;
  }

  void serialize(Serializer& ser) const override {
    AstNode::serializePart(ser);

    ser(pragmas_);
    ser(isDeprecated_);
    ser(isUnstable_);
    ser(deprecationMessage_);
    ser(unstableMessage_);
  }

  DECLARE_STATIC_DES(Attributes);

};


} // end namespace uast
} // end namespace chpl

#endif
