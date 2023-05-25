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

#ifndef CHPL_UAST_WHILE_H
#define CHPL_UAST_WHILE_H

#include "chpl/framework/Location.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/Loop.h"

namespace chpl {
namespace uast {


/**
  This class represents a while loop. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      var i = 0;
      while i < 5 {
        writeln(i);
        i += 1;
      }

  \endrst

 */
class While final : public Loop {
 private:
  While(AstList children, int8_t conditionChildNum,
        BlockStyle blockStyle,
        int loopBodyChildNum)
    : Loop(asttags::While, std::move(children), blockStyle,
           loopBodyChildNum, NO_CHILD /*attributeGroup*/),
      conditionChildNum_(conditionChildNum) {
    CHPL_ASSERT(condition());
  }

  While(Deserializer& des)
    : Loop(asttags::While, des) {
    conditionChildNum_ = des.read<int8_t>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const While* lhs = this;
    const While* rhs = (const While*) other;

    if (lhs->conditionChildNum_ != rhs->conditionChildNum_)
      return false;

    if (!lhs->loopContentsMatchInner(rhs))
      return false;

    return true;
  }

  void markUniqueStringsInner(Context* context) const override {
    loopMarkUniqueStringsInner(context);
  }

  std::string dumpChildLabelInner(int i) const override;

  int8_t conditionChildNum_;

 public:
  ~While() override = default;

  /**
    Create and return a while loop.
  */
  static owned<While> build(Builder* builder, Location loc,
                            owned<AstNode> condition,
                            BlockStyle blockStyle,
                            owned<Block> stmts);


  /**
    Return the condition of this while loop.
  */
  const AstNode* condition() const {
    auto ret = child(conditionChildNum_);
    return ret;
  }

  void serialize(Serializer& ser) const override {
    Loop::serialize(ser);
    ser.write(conditionChildNum_);
  }

  DECLARE_STATIC_DESERIALIZE(While);

};


} // end namespace uast
} // end namespace chpl

#endif
