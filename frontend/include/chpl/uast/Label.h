/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_LABEL_H
#define CHPL_UAST_LABEL_H

#include "chpl/framework/Location.h"
#include "chpl/uast/Loop.h"
#include "chpl/framework/UniqueString.h"


namespace chpl {
namespace uast {


/**
  This class represents a label. For example:

  \rst
  .. code-block:: chapel

    label outer for i in 0..15 do
      label inner while true do
        if i == 12 then break outer; else break inner;

  \endrst

*/
class Label final : public AstNode {
 friend class AstNode;

 private:
  Label(AstList children, UniqueString name)
    : AstNode(asttags::Label, std::move(children)),
      name_(name) {
    CHPL_ASSERT(numChildren() == 1);
  }

  void serializeInner(Serializer& ser) const override {
    ser.write(name_);
  }

  explicit Label(Deserializer& des)
    : AstNode(asttags::Label, des) {
    name_ = des.read<UniqueString>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Label* lhs = this;
    const Label* rhs = other->toLabel();

    CHPL_ASSERT(lhs->loopChildNum_ == rhs->loopChildNum_);

    if (lhs->name_ != rhs->name_)
      return false;

    return true;
  }
  void markUniqueStringsInner(Context* context) const override {
    name_.mark(context);
  }
  void dumpFieldsInner(const DumpSettings& s) const override;

  // This always exists and its position can never change.
  static const int8_t loopChildNum_ = 0;

  UniqueString name_;

 public:
  ~Label() override = default;

  /**
    Create and return a label statement.
  */
  static owned<Label> build(Builder* builder, Location loc,
                            UniqueString name,
                            owned<Loop> loop);

  /**
    Return the loop of this label statement.
  */
  const Loop* loop() const {
    auto ret = child(loopChildNum_);
    CHPL_ASSERT(ret->isLoop());
    return (const Loop*)ret;
  }

  /**
    Return the name of this label statement.
  */
  UniqueString name() const {
    return name_;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
