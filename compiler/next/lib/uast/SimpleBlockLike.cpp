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

#include "chpl/uast/SimpleBlockLike.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


bool SimpleBlockLike::
simpleBlockLikeContentsMatchInner(const ASTNode* other) const {
  const SimpleBlockLike* lhs = this;
  const SimpleBlockLike* rhs = other->toSimpleBlockLike();

  if (lhs->blockStyle_ != rhs->blockStyle_)
    return false;

  if (lhs->bodyChildNum_ != rhs->bodyChildNum_)
    return false;

  if (lhs->numBodyStmts_ != rhs->numBodyStmts_)
    return false;

  if (!lhs->expressionContentsMatchInner(rhs))
    return false;

  return true;
}

SimpleBlockLike::~SimpleBlockLike() {
}

} // namespace uast
} // namespace chpl
