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

#include "chpl/uast/IndexableLoop.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {

bool IndexableLoop::
indexableLoopContentsMatchInner(const IndexableLoop* other) const {
  const IndexableLoop* lhs = this;
  const IndexableLoop* rhs = other;

  if (lhs->indexVariableChildNum_ != rhs->indexVariableChildNum_)
    return false;

  if (lhs->iterandChildNum_ != rhs->iterandChildNum_)
    return false;

  if (lhs->withClauseChildNum_ != rhs->withClauseChildNum_)
    return false;

  if (!lhs->loopContentsMatchInner(other))
    return false;

  return true;
}

IndexableLoop::~IndexableLoop(){
}


} // namespace uast
} // namespace chpl
