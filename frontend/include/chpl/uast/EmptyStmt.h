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

#ifndef CHPL_UAST_EMPTYSTMT_H
#define CHPL_UAST_EMPTYSTMT_H

#include "chpl/framework/Location.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/TypeDecl.h"

namespace chpl {
namespace uast {

/**
  This class represents an empty statement For example:

  \rst
  .. code-block:: chapel

      ;

  \endrst

   The single semicolon, `;`, represents an empty statement

 */

class EmptyStmt final : public AstNode {
private:
  EmptyStmt()
    : AstNode(asttags::EmptyStmt) {
    CHPL_ASSERT(numChildren() == 0);
  }
  EmptyStmt(Deserializer& des)
    : AstNode(asttags::EmptyStmt, des) { }

    bool contentsMatchInner(const AstNode* other) const override {
    const EmptyStmt* rhs = other->toEmptyStmt();
    return rhs != nullptr;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

public:
  ~EmptyStmt() override = default;

  /**
   Create an EmptyStmt at this location.
  */
  static owned<EmptyStmt> build(Builder* builder, Location loc);

  void serialize(Serializer& ser) const override {
    AstNode::serialize(ser);
  }

  DECLARE_STATIC_DESERIALIZE(EmptyStmt);

}; // end EmptyStmt


} // end uast
} // end chpl

#endif
