/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_ATTRIBUTE_GROUP_H
#define CHPL_UAST_ATTRIBUTE_GROUP_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Pragma.h"
#include "chpl/util/iteration.h"
#include "chpl/uast/Attribute.h"
#include <set>

namespace chpl {
namespace uast {

/**
  This class represents a collection of attributes which are associated
  with a particular declaration.

  Compiler pragmas and deprecation messages (both intended for internal
  use only) are both examples of attributes.
*/
class AttributeGroup final : public AstNode {
 friend class AstNode;

 private:
  std::set<PragmaTag> pragmas_;
  bool isDeprecated_;
  bool isUnstable_;
  bool isParenfulDeprecated_;
  UniqueString deprecationMessage_;
  UniqueString unstableMessage_;
  UniqueString parenfulDeprecationMessage_;

  // TODO: Do we want to preserve the location of string literals used in
  // pragmas and deprecation messages?
  AttributeGroup(std::set<PragmaTag> pragmas,
                 bool isDeprecated,
                 bool isUnstable,
                 bool isParenfulDeprecated,
                 UniqueString deprecationMessage,
                 UniqueString unstableMessage,
                 UniqueString parenfulDeprecationMessage)
    : AstNode(asttags::AttributeGroup),
      pragmas_(std::move(pragmas)),
      isDeprecated_(isDeprecated),
      isUnstable_(isUnstable),
      isParenfulDeprecated_(isParenfulDeprecated),
      deprecationMessage_(deprecationMessage),
      unstableMessage_(unstableMessage),
      parenfulDeprecationMessage_(parenfulDeprecationMessage) {
    if (!deprecationMessage_.isEmpty()) {
      CHPL_ASSERT(isDeprecated_);
    }
    if (!unstableMessage_.isEmpty()) {
      CHPL_ASSERT(isUnstable_);
    }
    if (!parenfulDeprecationMessage_.isEmpty()) {
      CHPL_ASSERT(isParenfulDeprecated_);
    }

    // This might already be a compile-time invariant? Not sure...
    CHPL_ASSERT(pragmas_.size() <= NUM_KNOWN_PRAGMAS);
  }

  AttributeGroup(std::set<PragmaTag> pragmas,
                 bool isDeprecated,
                 bool isUnstable,
                 bool isParenfulDeprecated,
                 UniqueString deprecationMessage,
                 UniqueString unstableMessage,
                 UniqueString parenfulDeprecationMessage,
                 AstList attributes)
    : AstNode(asttags::AttributeGroup, std::move(attributes)),
      pragmas_(std::move(pragmas)),
      isDeprecated_(isDeprecated),
      isUnstable_(isUnstable),
      isParenfulDeprecated_(isParenfulDeprecated),
      deprecationMessage_(deprecationMessage),
      unstableMessage_(unstableMessage),
      parenfulDeprecationMessage_(parenfulDeprecationMessage) {
    if (!deprecationMessage_.isEmpty()) {
      CHPL_ASSERT(isDeprecated_);
    }
    if (!unstableMessage_.isEmpty()) {
      CHPL_ASSERT(isUnstable_);
    }
    if (!parenfulDeprecationMessage_.isEmpty()) {
      CHPL_ASSERT(isParenfulDeprecated_);
    }

    // This might already be a compile-time invariant? Not sure...
    CHPL_ASSERT(pragmas_.size() <= NUM_KNOWN_PRAGMAS);
  }

  void serializeInner(Serializer& ser) const override {
    ser.write(pragmas_);
    ser.write(isDeprecated_);
    ser.write(isUnstable_);
    ser.write(isParenfulDeprecated_);
    ser.write(deprecationMessage_);
    ser.write(unstableMessage_);
    ser.write(parenfulDeprecationMessage_);
  }

  explicit AttributeGroup(Deserializer& des)
    : AstNode(asttags::AttributeGroup, des) {
    pragmas_ = des.read<std::set<PragmaTag>>();
    isDeprecated_ = des.read<bool>();
    isUnstable_ = des.read<bool>();
    isParenfulDeprecated_ = des.read<bool>();
    deprecationMessage_ = des.read<UniqueString>();
    unstableMessage_ = des.read<UniqueString>();
    parenfulDeprecationMessage_ = des.read<UniqueString>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const AttributeGroup* rhs = (const AttributeGroup*)other;
    return this->pragmas_ == rhs->pragmas_ &&
           this->isDeprecated_ == rhs->isDeprecated_ &&
           this->isUnstable_ == rhs->isUnstable_ &&
           this->isParenfulDeprecated_ == rhs->isParenfulDeprecated_ &&
           this->deprecationMessage_ == rhs->deprecationMessage_ &&
           this->unstableMessage_ == rhs->unstableMessage_ &&
           this->parenfulDeprecationMessage_ == rhs->parenfulDeprecationMessage_;
  }

  void markUniqueStringsInner(Context* context) const override {
    deprecationMessage_.mark(context);
    unstableMessage_.mark(context);
    parenfulDeprecationMessage_.mark(context);
  }

  void dumpInner(const DumpSettings& s) const;

 public:
  ~AttributeGroup() override = default;

  static owned<AttributeGroup> build(Builder* builder, Location loc,
                                     std::set<PragmaTag> pragmas,
                                     bool isDeprecated,
                                     bool isUnstable,
                                     bool isParenfulDeprecated,
                                     UniqueString deprecationMessage,
                                     UniqueString unstableMessage,
                                     UniqueString parenfulDeprecationMessage);

  static owned<AttributeGroup> build(Builder* builder, Location loc,
                                     std::set<PragmaTag> pragmas,
                                     bool isDeprecated,
                                     bool isUnstable,
                                     bool isParenfulDeprecated,
                                     UniqueString deprecationMessage,
                                     UniqueString unstableMessage,
                                     UniqueString parenfulDeprecationMessage,
                                     AstList attributes);

  /**
    Returns true if the given pragma is set for this attributeGroup.
  */
  bool hasPragma(PragmaTag tag) const {
    CHPL_ASSERT(tag >= 0 && tag < NUM_KNOWN_PRAGMAS);
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

  /*
    returns the attribute with the given name, or nullptr if it is not found.
  */
  // call as getAttributeNamed(USTR("deprecated"))
  // or getAttributeNamed(UniqueString::get(<context>, "attrib.name")) etc.
  const Attribute* getAttributeNamed(UniqueString attributeName) const {
    const Attribute* ret = nullptr;
    for (auto child : children()) {
      if (auto attr = child->toAttribute()) {
        if (attr->name() == attributeName) {
          ret = attr;
          break;
        }
      }
    }
    return ret;
  }

  /**
    Returns true if the declaration associated with this attributeGroup is
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
    Returns true if the declaration associated with this attributeGroup
    has a parenful form that's deprecated in favor of a parenless form.
  */
  bool isParenfulDeprecated() const {
    return isParenfulDeprecated_;
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

  /**
    Returns a deprecation message, or the empty string if it is not set.
  */
  UniqueString parenfulDeprecationMessage() const {
    return parenfulDeprecationMessage_;
  }

  /**
    Returns the number of attributes in this group.
  */
  int numAttributes() const {
    return numChildren();
  }

};


} // end namespace uast
} // end namespace chpl

#endif
