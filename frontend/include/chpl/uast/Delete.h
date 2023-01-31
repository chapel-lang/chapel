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

#ifndef CHPL_UAST_DELETE_H
#define CHPL_UAST_DELETE_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"

namespace chpl {
namespace uast {


/**
  This class represents a delete statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      var c = new unmanaged C();
      delete c;

  \endrst
*/
class Delete final : public AstNode {
 private:
  Delete(AstList children)
    : AstNode(asttags::Delete, std::move(children)) {
  }

  Delete(Deserializer& des)
    : AstNode(asttags::Delete, des) {}

  bool contentsMatchInner(const AstNode* other) const override {
    return true;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

 public:

  /**
    Create and return a delete statement.
  */
  static owned<Delete> build(Builder* builder, Location loc,
                             AstList exprs);

  /**
    Return a way to iterate over the expressions of this delete statement.
  */
  AstListIteratorPair<AstNode> exprs() const {
    return AstListIteratorPair<AstNode>(children_.begin(),
                                           children_.end());
  }

  /**
    Return the number of expressions in this delete statement.
  */
  int numExprs() const {
    return this->numChildren();
  }

  /**
    Return the i'th expression in this delete statement.
  */
  const AstNode* expr(int i) const {
    const AstNode* ast = this->child(i);
    return ast;
  }

  void serialize(Serializer& ser) const override {
    AstNode::serialize(ser);
  }

  DECLARE_STATIC_DESERIALIZE(Delete);


};


} // end namespace uast
} // end namespace chpl

#endif
