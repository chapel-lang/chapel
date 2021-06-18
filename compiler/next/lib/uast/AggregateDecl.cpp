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

#include "chpl/uast/AggregateDecl.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


bool AggregateDecl::validAggregateChildren(ASTListIteratorPair<Expression> it) {
  for (auto elt: it) {
    if (elt->isComment() || elt->isErroneousExpression()) {
      // OK
    } else if (elt->isDecl()) {
      if (elt->isVariable() || elt->isFunction() ||
          elt->isTupleDecl() || elt->isMultiDecl()) {
        // OK
      } else {
        return false;
      }
    } else {
      return false;
    }
  }
  return true;
}

AggregateDecl::~AggregateDecl() {
}


} // namespace uast
} // namespace chpl
