/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/queries/update-functions.h"

#include <cstring>

namespace chpl {


ID ID::parentSymbolId(Context* context) const {
  if (postOrderId_ >= 0) {
    // Create an ID with postorder id -1 instead
    return ID(symbolPath_, -1, 0);
  }

  // remove the last '.' component from the ID
  const char* path = symbolPath_.c_str();
  ssize_t lastDot = -1;
  for (ssize_t i = 0; path[i]; i++) {
    if (path[i] == '.') lastDot = i;
  }

  if (lastDot == -1) {
    // no path component to remove, so return an empty ID
    return ID();
  }

  // Otherwise, construct an ID for the parent symbol
  return ID(UniqueString::get(context, path, lastDot), -1, 0);
}

bool ID::contains(const ID& other) const {
  UniqueString thisPath = this->symbolPath();
  UniqueString otherPath = other.symbolPath();

  if (thisPath == otherPath) {
    // the nodes have the same parent symbol, so consider the AST ids
    int thisId  = this->postOrderId();
    int thisNContained = this->numContainedChildren();
    int otherId = other.postOrderId();
    int thisFirstContained = thisId - thisNContained;

    return thisId == -1 ||
           (thisFirstContained <= otherId && otherId <= thisId);
  } else {
    // No need to consider the IDs in the event that thisPath
    // is a prefix of otherPath. In that event, they are different
    // symbols, but one is nested inside the other.
    return otherPath.startsWith(thisPath);
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

std::vector<std::pair<UniqueString,int>>
ID::expandSymbolPath(Context* context, UniqueString symbolPath) {
  std::vector<std::pair<UniqueString,int>> ret;

  const char* s = symbolPath.c_str();
  while (s && s[0] != '\0') {
    const char* nextPeriod = strchr(s, '.');

    // compute location of the null byte or just after the next '.'
    const char* next = nullptr;
    if (nextPeriod == nullptr) {
      // location of the null byte
      next = s + strlen(s);
    } else {
      // just after the '.'
      next = nextPeriod + 1;
    }

    // if there is a '#' in s+1..next, find it
    // start at s+1 to handle e.g. an ID for a # (range count) operator
    const char* nextPound = nullptr;
    for (const char* p = s+1; p < next; p++) {
      if (*p == '#') {
        nextPound = p;
      }
    }

    // compute the repeat count
    int repeat = 0;
    if (nextPound != nullptr) {
      // convert the numeric repeat value to an integer
      // note that atoi should stop at the first non-digit
      repeat = atoi(nextPound + 1);
    }

    // compute location of the '#', '.', or the null byte,
    // whichever comes first
    const char* partEnd = nullptr;
    if (nextPound != nullptr) {
      partEnd = nextPound;
    } else if (nextPeriod != nullptr) {
      partEnd = nextPeriod;
    } else {
      partEnd = next; // next is location of null byte in this case
    }

    // compute the UniqueString containing just the symbol part
    auto part = UniqueString::get(context, s, partEnd-s);

    ret.emplace_back(part, repeat);

    s = next;
  }

  return ret;
}

bool ID::update(chpl::ID& keep, chpl::ID& addin) {
  return defaultUpdate(keep, addin);
}

void ID::stringify(std::ostream& ss,
                   chpl::StringifyKind stringKind) const {
  ss << this->symbolPath().c_str();

  if (!(symbolPath().isEmpty()) && this->postOrderId() >= 0) {
    ss << "@";
    ss << std::to_string(this->postOrderId());
  }
}

IMPLEMENT_DUMP(ID);

std::string ID::str() const {
  std::ostringstream ss;
  stringify(ss, chpl::StringifyKind::DEBUG_SUMMARY);
  return ss.str();
}

} // end namespace chpl
