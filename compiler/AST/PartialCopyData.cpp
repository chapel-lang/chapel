/*
 * Copyright 2004-2020 Cray Inc.
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

#include "PartialCopyData.h"

#include "symbol.h"

#include <map>

static std::map<int, PartialCopyData> sFnMap;

PartialCopyData::PartialCopyData() {
  partialCopySource = NULL;
  varargOldFormal   = NULL;
}

PartialCopyData::~PartialCopyData() {
  partialCopyMap.clear();
  varargNewFormals.clear();
}

// Return true if there is an entry for 'fn' in the partialCopyFnMap
bool hasPartialCopyData(const FnSymbol* fn) {
  std::map<int, PartialCopyData>::iterator it     = sFnMap.find(fn->id);
  bool                                     retval = false;

  if (it != sFnMap.end()) {
    retval = true;
  }

  return retval;
}

// Return the entry for 'fn' in partialCopyFnMap or NULL if it does not exist.
PartialCopyData* getPartialCopyData(FnSymbol* fn) {
  std::map<int, PartialCopyData>::iterator it     = sFnMap.find(fn->id);
  PartialCopyData*                         retval = NULL;

  if (it != sFnMap.end()) {
    retval = &(it->second);
  }

  return retval;
}

// Add 'fn' to partialCopyFnMap; remove the corresponding entry.
PartialCopyData& addPartialCopyData(FnSymbol* fn) {
  INT_ASSERT(sFnMap.count(fn->id) == 0);

  return sFnMap[fn->id];
}

// Remove 'fn' from partialCopyFnMap.
void clearPartialCopyData(FnSymbol* fn) {
  size_t cnt = sFnMap.erase(fn->id);

  INT_ASSERT(cnt == 1); // Convention: clear only what was added before.
}

void clearPartialCopyDataFnMap() {
  sFnMap.clear();
}

// Since FnSymbols can get removed at pass boundaries, leaving them
// in here may result in useless entries.
// As of this writing, PartialCopyData is used only within resolution.
void checkEmptyPartialCopyDataFnMap() {
  if (sFnMap.size()) {
    INT_FATAL("partialCopyFnMap is not empty");
  }
}
