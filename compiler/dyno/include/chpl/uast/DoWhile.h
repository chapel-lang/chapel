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

#ifndef CHPL_UAST_DOWHILE_H
#define CHPL_UAST_DOWHILE_H

#include "chpl/queries/Location.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/Loop.h"

namespace chpl {
namespace uast {


/**
  This class represents a do-while loop. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      var i = 0;
      do {
        writeln(i);
        i += 1;
      } while i < 5;

  \endrst

 */
class DoWhile final : public Loop {
 private:
  DoWhile(AstList children, BlockStyle blockStyle,
          int loopBodyChildNum,
          int conditionChildNum)
    : Loop(asttags::DoWhile, std::move(children),
           blockStyle,
           loopBodyChildNum),
      conditionChildNum_(conditionChildNum) {
    assert(condition());
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const DoWhile* lhs = this;
    const DoWhile* rhs = (const DoWhile*) other;

    if (lhs->conditionChildNum_ != rhs->conditionChildNum_)
      return false;

    if (!lhs->loopContentsMatchInner(rhs))
      return false;

    return true;
  }

  void markUniqueStringsInner(Context* context) const override {
    loopMarkUniqueStringsInner(context);
  }

  int conditionChildNum_;

 public:
  ~DoWhile() override = default;

  /**
    Create and return a do-while loop.
  */
  static owned<DoWhile> build(Builder* builder, Location loc,
                              BlockStyle blockStyle,
                              owned<Block> body,
                              owned<AstNode> condition);


  /**
    Return the condition of this do-while loop.
  */
  const AstNode* condition() const {
    auto ret = child(conditionChildNum_);
    return ret;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
