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

#ifndef CHPL_UAST_YIELD_H
#define CHPL_UAST_YIELD_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"

namespace chpl {
namespace uast {


/**
  This class represents a yield statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      iter foo(): int {
        for i in range 0..15 do yield i;
      }

  \endrst

 */
class Yield final : public AstNode {
 private:
  Yield(AstList children)
    : AstNode(asttags::Yield, std::move(children)) {
    CHPL_ASSERT(children_.size() == 1);
  }

  Yield(Deserializer& des)
    : AstNode(asttags::Yield, des) {
  }

  bool contentsMatchInner(const AstNode* other) const override {
    // The 'valueChildNum_' is const and does not need to be compared.
    return true;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  // Value always exists, and its position is always the same.
  static const int8_t valueChildNum_ = 0;

 public:
  ~Yield() override = default;

  /**
    Create and return a yield statement. The value formal cannot be nullptr.
  */
  static owned<Yield> build(Builder* builder, Location loc,
                            owned<AstNode> value);

  /**
    Returns the value of this yield statement.
  */
  const AstNode* value() const {
    auto ret = child(valueChildNum_);
    CHPL_ASSERT(ret);
    return ret;
  }

  void serialize(Serializer& ser) const override {
    AstNode::serialize(ser);
  }

  DECLARE_STATIC_DESERIALIZE(Yield);

};


} // end namespace uast
} // end namespace chpl

#endif
