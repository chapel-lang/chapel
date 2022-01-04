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

#ifndef CHPL_UAST_BOOLLITERAL_H
#define CHPL_UAST_BOOLLITERAL_H

#include "chpl/queries/Location.h"
#include "chpl/types/Param.h"
#include "chpl/uast/Literal.h"

namespace chpl {
namespace uast {


/**
  This class represents a boolean literal.
 */
class BoolLiteral final : public Literal {
 private:
  explicit BoolLiteral(const types::BoolParam* value)
    : Literal(asttags::BoolLiteral, value) {
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const BoolLiteral* rhs = (const BoolLiteral*) other;
    return literalContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    literalMarkUniqueStringsInner(context);
  }

 public:
  ~BoolLiteral() override = default;

  /**
    Create and return a BoolLiteral.
  */
  static owned<BoolLiteral> build(Builder* builder, Location loc,
                                  bool value);

  /**
    Returns the value of this bool literal.
  */
  bool value() const {
    auto p = (const types::BoolParam*) value_;
    return p->value() != 0;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
