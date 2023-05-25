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

#ifndef CHPL_UAST_RETURN_H
#define CHPL_UAST_RETURN_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"

namespace chpl {
namespace uast {


/**
  This class represents a return statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      proc foo(): int {
        return 0;
      }

  \endrst

 */
class Return final : public AstNode {
 private:
  Return(AstList children,  int8_t valueChildNum)
    : AstNode(asttags::Return, std::move(children)),
      valueChildNum_(valueChildNum) {
    CHPL_ASSERT(valueChildNum_ <= 0);
  }
  Return(Deserializer& des)
    : AstNode(asttags::Return, des) {
    valueChildNum_ = des.read<int8_t>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Return* lhs = this;
    const Return* rhs = (const Return*) other;

    if (lhs->valueChildNum_ != rhs->valueChildNum_)
      return false;

    return true;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  int8_t valueChildNum_;

 public:
  ~Return() override = default;

  /**
    Create and return a return statement. If value is nullptr, then there
    is no return value.
  */
  static owned<Return> build(Builder* builder, Location loc,
                             owned<AstNode> value);

  /**
    Returns the value of this return statement, or nullptr if there
    is none.
  */
  const AstNode* value() const {
    if (valueChildNum_ < 0) return nullptr;
    auto ret = child(valueChildNum_);
    return ret;
  }

  void serialize(Serializer& ser) const override {
    AstNode::serialize(ser);
    ser.write(valueChildNum_);
  }

  DECLARE_STATIC_DESERIALIZE(Return);

};


} // end namespace uast
} // end namespace chpl

#endif
