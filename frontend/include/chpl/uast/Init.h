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

#ifndef CHPL_UAST_INITTHIS_H
#define CHPL_UAST_INITTHIS_H

#include "chpl/framework/Location.h"
#include "chpl/uast/Identifier.h"

namespace chpl {
namespace uast {

/**
  This class represents an `init this` statement. For example:

  \rst
  .. code-block:: chapel

    proc init() {
      init this;

      // can call methods at this point.
    }
  \endrst
*/
class Init : public AstNode {
 friend class AstNode;

 private:
  int8_t targetChildNum_;

  Init(AstList children, int8_t targetChildNum)
    : AstNode(asttags::Init, std::move(children)),
      targetChildNum_(targetChildNum) {
    CHPL_ASSERT(numChildren() == 1);
  }

  void serializeInner(Serializer& ser) const override {
    ser.write(targetChildNum_);
  }

  explicit Init(Deserializer& des)
   : AstNode(asttags::Init, des) {
    targetChildNum_ = des.read<int8_t>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Init* lhs = this;
    const Init* rhs = other->toInit();

    if (lhs->targetChildNum_ != rhs->targetChildNum_)
      return false;

    return true;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  std::string dumpChildLabelInner(int i) const override;

 public:

  /**
    Create and return an 'init' statement.
  */
  static owned<Init> build(Builder* builder, Location loc,
                            owned<Identifier> target);

  /**
    Returns the target of this 'init' expression. This should always
    be the identifier 'this'.
  */
  const Identifier* target() const {
    CHPL_ASSERT(targetChildNum_ == 0);
    auto ret = child(targetChildNum_);
    CHPL_ASSERT(ret->isIdentifier());
    return (const Identifier*)ret;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
