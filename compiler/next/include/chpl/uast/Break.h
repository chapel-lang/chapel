/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_BREAK_H
#define CHPL_UAST_BREAK_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Identifier.h"

namespace chpl {
namespace uast {

/**
  This class represents a break statement. For example:

  \rst
  .. code-block:: chapel

    var i = 0;
    while true {
      if i >= 16 then break;
      writeln(i);
      i += 1;
    }
  \endrst
*/
class Break : public Expression {
 private:
  Break(ASTList children, int8_t targetChildNum)
    : Expression(asttags::Break, std::move(children)),
      targetChildNum_(targetChildNum) {
    assert(numChildren() <= 1);
  }

  bool contentsMatchInner(const ASTNode* other) const override;

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

  int8_t targetChildNum_; 

 public:

  /**
    Create and return a break statement.
  */
  static owned<Break> build(Builder* builder, Location loc,
                            owned<Identifier> target);

  /**
    Returns the target of this break statement, or nullptr if there
    is none.
  */
  const Identifier* target() const {
    if (targetChildNum_ < 0) return nullptr;
    auto ret = child(targetChildNum_);
    assert(ret->isIdentifier());
    return (const Identifier*)ret;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
