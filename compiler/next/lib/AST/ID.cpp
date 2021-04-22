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

#include "chpl/AST/ID.h"

#include <cstring>

namespace chpl {
namespace ast {

ID::ID()
  : symbolPath_(), postOrderId_(-1), numChildIds_(0) {
}

ID::ID(UniqueString symbolPath, int postOrderId, int numChildIds)
  : symbolPath_(symbolPath),
    postOrderId_(postOrderId),
    numChildIds_(numChildIds) {
}

// Returns 'true' if this symbol contains another AST node.
bool ID::contains(const ID other) const {
  UniqueString thisPath = this->symbolPath();
  UniqueString otherPath = other.symbolPath();

  if (thisPath == otherPath) {
    // the nodes have the same parent symbol, so consider the AST ids
    int thisId  = this->postOrderId();
    int thisNContained = this->numContainedChildren();
    int otherId = other.postOrderId();
    int thisFirstContained = thisId - thisNContained;

    return thisFirstContained <= otherId && otherId <= thisId;
  } else {
    // No need to consider the IDs in the event that thisPath
    // is a prefix of otherPath. In that event, they are different
    // symbols, but one is nested inside the other.
    return otherPath.startsWith(thisPath);
  }
}

int ID::compare(const ID other) const {
  // first, compare with the path portion
  UniqueString lhsPath = this->symbolPath();
  UniqueString rhsPath = other.symbolPath();
  int pathCmp = lhsPath.compare(rhsPath);
  if (pathCmp != 0)
    return pathCmp;

  // if that wasn't different, compare the id
  return this->postOrderId() - other.postOrderId();
}

} // end namespace ast
} // end namespace chpl
