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

#ifndef CHPL_UAST_LITERAL_H
#define CHPL_UAST_LITERAL_H

#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This is an abstract base class for literals.
  Literals are fixed values in the source code, like 1, 30.24, and "x".
 */
class Literal : public Expression {
 protected:
  Literal(ASTTag tag)
    : Expression(tag) {
  }

  bool literalContentsMatchInner(const Literal* other) const {
    return expressionContentsMatchInner(other);
  }
  void literalMarkUniqueStringsInner(Context* context) const {
    expressionMarkUniqueStringsInner(context);
  }

 public:
  virtual ~Literal() = 0; // this is an abstract base class
};


} // end namespace uast
} // end namespace chpl

#endif
