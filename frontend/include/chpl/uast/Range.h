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

#ifndef CHPL_UAST_RANGE_H
#define CHPL_UAST_RANGE_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"

namespace chpl {
namespace uast {


/**
  This class represents a range expression. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      var r = 0..15;

  \endrst

 */
class Range final : public AstNode {
 public:
  enum OpKind {
    DEFAULT,
    OPEN_HIGH
  };

 private:
  Range(AstList children, OpKind opKind,
        int8_t lowerBoundChildNum,
        int8_t upperBoundChildNum)
    : AstNode(asttags::Range, std::move(children)),
      opKind_(opKind),
      lowerBoundChildNum_(lowerBoundChildNum),
      upperBoundChildNum_(upperBoundChildNum) {
    if (opKind_ == OPEN_HIGH) {
      CHPL_ASSERT(upperBoundChildNum_ >= 0);
    }
  }

  Range(Deserializer& des)
    : AstNode(asttags::Range, des) {
    opKind_ = des.read<OpKind>();
    lowerBoundChildNum_ = des.read<int8_t>();
    upperBoundChildNum_ = des.read<int8_t>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Range* rhs = other->toRange();
    return this->opKind_ == rhs->opKind_ &&
      this->lowerBoundChildNum_ == rhs->lowerBoundChildNum_ &&
      this->upperBoundChildNum_ == rhs->upperBoundChildNum_;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  void dumpFieldsInner(const DumpSettings& s) const override;
  std::string dumpChildLabelInner(int i) const override;

  OpKind opKind_;
  int8_t lowerBoundChildNum_;
  int8_t upperBoundChildNum_;

 public:
  ~Range() override = default;

  /**
    Create and return a range expression.
  */
  static owned<Range> build(Builder* builder, Location loc,
                            OpKind opKind,
                            owned<AstNode> lowerBound,
                            owned<AstNode> upperBound);

  /**
    Returns the operator kind used to construct this range.
  */
  OpKind opKind() const {
    return this->opKind_;
  }

  /**
    Returns the lower bound of this range, or nullptr if there is none.
  */
  const AstNode* lowerBound() const {
    if (lowerBoundChildNum_ < 0) return nullptr;
    auto ret = child(lowerBoundChildNum_);
    return ret;
  }

  /**
    Returns the upper bound of this range, or nullptr if there is none.
  */
  const AstNode* upperBound() const {
    if (upperBoundChildNum_ < 0) return nullptr;
    auto ret = child(upperBoundChildNum_);
    return ret;
  }

  /**
    Returns a string describing the passed OpKind
    */
  static const char* opKindToString(OpKind kind);

  void serialize(Serializer& ser) const override {
    AstNode::serialize(ser);
    ser.write(opKind_);
    ser.write(lowerBoundChildNum_);
    ser.write(upperBoundChildNum_);
  }

  DECLARE_STATIC_DESERIALIZE(Range);

};


} // end namespace uast

DECLARE_SERDE_ENUM(uast::Range::OpKind, uint8_t);

} // end namespace chpl

#endif
