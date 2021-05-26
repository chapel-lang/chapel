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

#include "chpl/uast/Loop.h"

namespace chpl {
namespace uast {

bool Loop::loopContentsMatchInner(const Loop* other) const {
  const Loop* lhs = this;
  const Loop* rhs = other;

  if (lhs->loopBodyChildNum_ != rhs->loopBodyChildNum_)
    return false;

  if (lhs->numLoopBodyStmts_ != rhs->numLoopBodyStmts_)
    return false;

  if (lhs->usesImplicitBlock_ != rhs->usesImplicitBlock_)
    return false;

  return true;
}

Loop::~Loop() {
}


} // namespace uast
} // namespace chpl
