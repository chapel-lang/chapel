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

#ifndef CHPL_UAST_THROW_H
#define CHPL_UAST_THROW_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"

namespace chpl {
namespace uast {


/**
  This class represents a throw statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      proc mayThrow() throws {
        throw new Error();
      }

  \endrst
*/
class Throw final : public AstNode {
 private:
  Throw(AstList children)
      : AstNode(asttags::Throw, std::move(children)) {
    CHPL_ASSERT(numChildren() == 1);
  }

  Throw(Deserializer& des)
    : AstNode(asttags::Throw, des) { }

  bool contentsMatchInner(const AstNode* other) const override {
    return true;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  // Always exists, position of the first and only element.
  static const int8_t errorExprChildNum_ = 0;

 public:

  /**
    Create and return a throw statement.
  */
  static owned<Throw> build(Builder* builder, Location loc,
                            owned<AstNode> expr);

  /**
    Return the error expression of this throw statement.
  */
  const AstNode* errorExpression() const {
    const AstNode* ast = this->child(errorExprChildNum_);
    return ast;
  }

  void serialize(Serializer& ser) const override {
    AstNode::serialize(ser);
  }

  DECLARE_STATIC_DESERIALIZE(Throw);

};


} // end namespace uast
} // end namespace chpl

#endif
