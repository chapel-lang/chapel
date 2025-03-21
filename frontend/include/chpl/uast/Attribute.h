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

#ifndef CHPL_UAST_ATTRIBUTE_H
#define CHPL_UAST_ATTRIBUTE_H


#include "chpl/uast/AstNode.h"
#include "chpl/uast/StringLiteral.h"

namespace chpl {
namespace uast {

class Attribute final: public AstNode {
 friend class AstNode;

 private:
  // the attribute name - deprecated or unstable or chpldoc.nodoc, for example
  UniqueString name_;
  int numActuals_; // number of child actuals
  std::vector<UniqueString> actualNames_;

  Attribute(UniqueString name, int numActuals, AstList actuals,
            std::vector<UniqueString> actualNames)
    : AstNode(asttags::Attribute, std::move(actuals)),
      name_(name),
      numActuals_(numActuals),
      actualNames_(std::move(actualNames)) {
  }

  void serializeInner(Serializer& ser) const override {
    ser.write(name_);
    ser.writeVInt(numActuals_);
    ser.write(actualNames_);
  }

  explicit Attribute(Deserializer& des) : AstNode(asttags::Attribute, des) {
    name_ = des.read<UniqueString>();
    numActuals_ = des.readVInt();
    actualNames_ = des.read<std::vector<UniqueString>>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Attribute* lhs = this;
    const Attribute* rhs = (const Attribute*) other;
    if (lhs->actualNames_.size() != rhs->actualNames_.size() ||
        lhs->name_ != rhs->name_ || lhs->numActuals_ != rhs->numActuals_) {
        return false;
    }
    int nActualNames = (int) lhs->actualNames_.size();
    for (int i = 0; i < nActualNames; i++) {
      if (lhs->actualNames_[i] != rhs->actualNames_[i])
        return false;
    }
    return true;
  }

  void markUniqueStringsInner(Context* context) const override {
    name_.mark(context);
    for (const auto& str : actualNames_) {
      str.mark(context);
    }
  }

  void dumpFieldsInner(const DumpSettings& s) const override;

public:
  ~Attribute() override = default;

  static owned<Attribute> build(Builder* builder, Location loc,
                                UniqueString name,
                                AstList actuals,
                                std::vector<UniqueString> actualNames);


  /**
    returns the name of the attribute without the toolspace
  */
  UniqueString name() const {
    return name_;
  }

  /**
   Returns an iterable expression over the actuals of an attribute.
   */
  AstListIteratorPair<AstNode> actuals() const {
    return AstListIteratorPair<AstNode>(children_.begin(), children_.end());
  }

  /*
    Returns the number of actuals of an attribute.
  */
  int numActuals() const {
    return numActuals_;
  }

  /*
    Returns the i'th actual of an attribute.
  */
  const AstNode* actual(int i) const {
    const AstNode* ast = this->child(i);
    return ast;
  }

  // returns a UniqueString or nullptr if no actual.
  // if more than 1 actual or actual not a string, raise error
  UniqueString getOnlyStringActualOrError(Context* ctx) const {
    UniqueString dummy;
    if (numActuals() == 0) {
      return dummy;
    } else if (numActuals() > 1) {
      ctx->error(this, "Attribute %s takes only one argument", name_.c_str());
    } else {
      const StringLiteral* str = actual(0)->toStringLiteral();
      if (str == nullptr) {
        ctx->error(this, "Attribute %s takes only a string argument", name_.c_str());
      } else {
        return str->value();
      }
    }
    return dummy;
  }

  /** Returns whether actual i is named as with 'f(a=3)'
      where the actual is 3 and the name is 'a'. */
  bool isNamedActual(int i) const {
    if (i < 0 || i >= (int) actualNames_.size())
      return false;

    return !actualNames_[i].isEmpty();
  }

  /** Returns the name of the actual, if used; otherwise the empty string */
  UniqueString actualName(int i) const {
    if (actualNames_.size() == 0)
      return UniqueString();

    return actualNames_[i];
  }


  /*
    Returns the full name of an attribute including its toolspace, if any.
    ex: "deprecated" or "chpldoc.nodoc", etc.
  */
  const std::string fullyQualifiedAttributeName() const {
    return name_.str();
  }
}; // end Attribute


} // end namespace uast


} // end namespace chpl

#endif //CHPL_UAST_ATTRIBUTE_H
