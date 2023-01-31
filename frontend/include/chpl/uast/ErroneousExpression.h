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

#ifndef CHPL_UAST_ERRONEOUSEXPRESSION_H
#define CHPL_UAST_ERRONEOUSEXPRESSION_H

#include "chpl/uast/AstNode.h"
#include "chpl/framework/Location.h"

namespace chpl {
namespace uast {


/**
  This class represents some missing AST due to an error.
 */
class ErroneousExpression final : public AstNode {
 private:
  ErroneousExpression()
    : AstNode(asttags::ErroneousExpression) {
  }

  ErroneousExpression(Deserializer& des)
    : AstNode(asttags::ErroneousExpression, des) {}


  bool contentsMatchInner(const AstNode* other) const override {
    return true;
  }
  void markUniqueStringsInner(Context* context) const override {
  }

 public:
  ~ErroneousExpression() = default;
  static owned<ErroneousExpression> build(Builder* builder, Location loc);

  void serialize(Serializer& ser) const override {
    AstNode::serialize(ser);
  }

  DECLARE_STATIC_DESERIALIZE(ErroneousExpression);

};


} // end namespace uast
} // end namespace chpl

#endif
