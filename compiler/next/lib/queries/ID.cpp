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

#include "chpl/queries/ID.h"

#include <cstring>

namespace chpl {


// Returns 'true' if this symbol contains another AST node.
bool ID::contains(const ID& other) const {
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
    // Is thisPath a prefix of otherPath, when considering
    // the characters / and . to be the same?
    const char* thisPtr = thisPath.c_str();
    const char* othrPtr = otherPath.c_str();

    // ignore leading . or /
    while (*thisPtr == '.' || *thisPtr == '/') thisPtr++;
    while (*othrPtr == '.' || *othrPtr == '/') othrPtr++;

    while (*thisPtr != '\0') {
      if ((*thisPtr == *othrPtr) ||
          ((*thisPtr == '.' || *thisPtr == '/') &&
           (*othrPtr == '.' || *othrPtr == '/'))) {
        // ok, it matches
      } else {
        return false;
      }

      thisPtr++;
      othrPtr++;
    }

    return true;
  }
}

int ID::compare(const ID& other) const {
  // first, compare with the path portion
  UniqueString lhsPath = this->symbolPath();
  UniqueString rhsPath = other.symbolPath();
  int pathCmp = lhsPath.compare(rhsPath);
  if (pathCmp != 0)
    return pathCmp;

  // if that wasn't different, compare the id
  return this->postOrderId() - other.postOrderId();
}

std::string ID::toString() const {
  int slashPos = -1;
  const char* path = this->symbolPath().c_str();
  for (int i = 0; path[i]; i++) {
    if (path[i] == '/') slashPos = i;
  }

  std::string noslash;
  if (slashPos < 0) {
    noslash = std::string(path);
  } else {
    noslash = std::string(path, slashPos);
  }

  std::string ret = noslash;
  if (!noslash.empty()) {
    ret += "@";
    ret += std::to_string(this->postOrderId());
  }

  if (slashPos >= 0) {
    // append the / part
    ret += std::string(&path[slashPos]);
  }

  return ret;
}

} // end namespace chpl
