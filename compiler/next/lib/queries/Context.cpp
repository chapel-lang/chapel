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

#include "chpl/queries/Context.h"

#include "chpl/queries/query-impl.h"

#include <cstdlib>
#include <cassert>

namespace chpl {

using namespace chpl::querydetail;

Context::Context()
  : uniqueStringsTable(), queryDB(), queryDeps(),
    currentRevisionNumber(1), lastPrepareToGCRevisionNumber(0), gcCounter(1) {
}

owned<Context> Context::build() {
  return toOwned(new Context());
}

Context::~Context() {
  // free all of the unique'd strings
  for (auto& item: this->uniqueStringsTable) {
    const char* s = item.second;
    free((void*)s);
  }
}

#define ALIGN_DN(i, size)  ((i) & ~((size) - 1))
#define ALIGN_UP(i, size)  ALIGN_DN((i) + (size) - 1, size)

static char* allocateEvenAligned(size_t amt) {
  char* buf = (char*) malloc(amt);
  // Normally, malloc returns something that is aligned to 16 bytes,
  // but it's technically possible that a platform library
  // could not do so. So, here we check.
  // We only need even alignment here.
  if ((((uintptr_t)buf) & 1) != 0) {
    free(buf);
    // try again with an aligned allocation
    size_t alignment = sizeof(void *);
    size_t align_up_len = ALIGN_UP(amt, sizeof(void*));
    buf = (char*) aligned_alloc(alignment, align_up_len);
  }
  assert(buf);
  assert((((uintptr_t)buf) & 1) == 0);
  return buf;
}

const char* Context::getOrCreateUniqueString(const char* str) {
  auto search = this->uniqueStringsTable.find(str);
  if (search != this->uniqueStringsTable.end()) {
    char* buf = search->second;
    const char* key = buf+2; // pass the 2 bytes of metadata
    // update the GC mark
    this->markUniqueCString(key);
    return key;
  }
  char gcMark = this->gcCounter & 0xff;
  size_t strLen = strlen(str);
  size_t allocLen = strLen+3; // 2 bytes of metadata, str data, '\0'
  char* buf = allocateEvenAligned(allocLen);
  // set the GC mark
  buf[0] = gcMark;
  // set the unused metadata (need to still have even alignment)
  buf[1] = 0x02;
  // copy the string data, including the null terminator
  memcpy(buf+2, str, strLen+1);
  const char* key = buf+2; // pass the 2 bytes of metadata
  // Add it to the table
  this->uniqueStringsTable.insert(search, {key, buf});
  return key;
}

const char* Context::uniqueCString(const char* s) {
  if (s == nullptr) s = "";
  return this->getOrCreateUniqueString(s);
}

void Context::markUniqueCString(const char* s) {
  // Performance: Would it be better to do this store unconditionally?
  if (this->currentRevisionNumber == this->lastPrepareToGCRevisionNumber &&
      s != nullptr) {
    char gcMark = this->gcCounter & 0xff;
    char* buf = (char*) s;
    buf -= 2; // the string is preceeded by gcMark and 0x02
    assert(buf[1] == 0x02);
    buf[0] = gcMark;
  }
}


UniqueString Context::moduleNameForID(ID id) {
  // If the symbol path is empty, this ID doesn't have a module.
  if (id.symbolPath().isEmpty()) {
    UniqueString empty;
    return empty;
  }

  // Otherwise, the module name is everything up to the first '.'
  size_t len = 0;
  const char* s = id.symbolPath().c_str();
  while (true) {
    if (s[len] == '\0' || s[len] == '.') break;
    len++;
  }

  return UniqueString::build(this, s, len);
}

UniqueString Context::filePathForID(ID id) {
  UniqueString modName = this->moduleNameForID(id);
  return this->filePathForModuleName(modName);
}

static
const UniqueString& filePathForModuleNameQuery(Context* context,
                                               UniqueString modName) {
  QUERY_BEGIN(filePathForModuleNameQuery, context, modName);

  assert(false && "This query should always use a saved result");
  auto result = UniqueString::build(context, "<unknown file path>");

  return QUERY_END(result);
}

UniqueString Context::filePathForModuleName(UniqueString modName) {
  return filePathForModuleNameQuery(this, modName);
}

void Context::advanceToNextRevision(bool prepareToGC) {
  this->currentRevisionNumber++;
  if (prepareToGC) {
    this->lastPrepareToGCRevisionNumber = this->currentRevisionNumber;
    gcCounter++;
  }
  printf("CURRENT REVISION NUMBER IS NOW %i %s\n",
         (int) this->currentRevisionNumber,
         prepareToGC?"PREPARING GC":"");
}

void Context::collectGarbage() {
  // if there are no parent queries, collect some garbage
  if (queryDeps.size() == 0) {
    printf("COLLECTING GARBAGE\n");
    // clear out the saved old results
    // warning: this loop proceeds in a nondeterministic order
    for (auto& dbEntry: queryDB) {
      QueryMapBase* queryMapBase = dbEntry.second.get();
      queryMapBase->clearOldResults(this->currentRevisionNumber);
    }

    if (this->lastPrepareToGCRevisionNumber == this->currentRevisionNumber) {
      // remove UniqueStrings that have not been marked

      size_t nUniqueStringsBefore = uniqueStringsTable.size();

      // Performance: Would it be better to modify the table in-place
      // rather than creating a new table as is done here?
      char gcMark = this->gcCounter & 0xff;
      UniqueStringsTableType newTable;
      std::vector<char*> toFree;
      // warning: this loop proceeds in a nondeterministic order
      for (auto& e: uniqueStringsTable) {
        const char* key = e.first;
        char* buf = e.second;
        if (buf[0] == gcMark) {
          printf("COPYING OVER UNIQUESTRING %s\n", key);
          newTable.insert(std::make_pair(key, buf));
        } else {
          printf("WILL FREE UNIQUESTRING %s\n", key);
          toFree.push_back(buf);
        }
      }
      for (char* buf: toFree) {
        free(buf);
      }
      uniqueStringsTable.swap(newTable);

      size_t nUniqueStringsAfter = uniqueStringsTable.size();
      printf("COLLECTED %i UniqueStrings\n",
             (int)(nUniqueStringsBefore-nUniqueStringsAfter));
    }
  }
}

void Context::setFilePathForModuleName(UniqueString modName, UniqueString path) {
  auto tupleOfArgs = std::make_tuple(modName);
  auto queryMapResult = updateResultForQuery(filePathForModuleNameQuery,
                                             tupleOfArgs, path,
                                             "filePathForModuleNameQuery",
                                             false);
  printf("SETTING FILE PATH FOR MODULE %s -> %s\n",
         modName.c_str(), path.c_str());
}

bool Context::checkAndRecomputeDependencies(const QueryMapResultBase* resultEntry) {

  if (this->currentRevisionNumber == resultEntry->lastChecked) {
    // No need to check the dependencies again.
    // We already know that we can reuse the result.
    return true;
  }

  printf("CHECKING DEPENDENCIES FOR %s\n",
         resultEntry->parentQueryMap->queryName);

  // Otherwise, check the dependencies. Have any of them
  // changed since the last revision in which we computed this?
  bool dependencyMet = true;
  for (const QueryMapResultBase* dependency : resultEntry->dependencies) {
    if (this->currentRevisionNumber == dependency->lastChecked) {
      // No need to check the dependency
    } else if (dependency->parentQueryMap->isInputQuery) {
      // For an input query, always try to compute it again,
      // ignoring the dependencies.
      // (e.g. if it is reading a file, we need to check that the file
      //  has not changed.)
      dependency->recompute(this);
      assert(dependency->lastChecked == this->currentRevisionNumber);
    } else {
      // check the dependencies, transitively.
      bool canReuse = checkAndRecomputeDependencies(dependency);
      dependencyMet = dependencyMet && canReuse;
    }

    if (dependency->lastChanged > resultEntry->lastChanged) {
      dependencyMet = false;
    }

    if (dependencyMet == false) {
      break;
    }
  }
  if (dependencyMet) {
    if (this->currentRevisionNumber==this->lastPrepareToGCRevisionNumber) {
      //printf("CALLING MARK UNIQUE STRINGS ON RESULT %s\n",
      //        resultEntry->parentQueryMap->queryName);
      // mark unique strings in the reused result
      resultEntry->markUniqueStrings(this);
    }
    resultEntry->lastChecked = this->currentRevisionNumber;
  }

  return dependencyMet;
}

bool Context::queryCanUseSavedResultAndPushIfNot(
                   const void* queryFunction,
                   const QueryMapResultBase* resultEntry) {

  bool useSaved = false;

  if (resultEntry == nullptr) {
    // If there was no result, we can't reuse it
    useSaved = false;
  } else {
    useSaved = checkAndRecomputeDependencies(resultEntry);

    // be sure to re-run input queries
    if (resultEntry->parentQueryMap->isInputQuery &&
        this->currentRevisionNumber != resultEntry->lastChecked) {
      useSaved = false;
    }
  }

  if (useSaved == false) {
    // Since the result cannot be reused, the query will be evaluated.
    // So, push something to queryDeps
    queryDeps.push_back(QueryDepsEntry(queryFunction));

    size_t n = queryDeps.size();
    if (n > 10000) {
      fprintf(stderr, "Error: query depth is too high - is there a cycle?\n");
      exit(1);
      // TODO - do something smarter; for example, could
      // check for cycles every time n reaches a power of two
    }
  }

  return useSaved;
}

void Context::saveDependenciesInParent(const QueryMapResultBase* resultEntry) {
  // Record that the parent query depends upon this one.
  //
  // we haven't pushed the query beginning yet, so the
  // parent query is at queryDeps.back()
  if (queryDeps.size() > 0) {
    queryDeps.back().dependencies.push_back(resultEntry);
  }
}
void Context::endQueryHandleDependency(const QueryMapResultBase* result) {
  // queryDeps.back() is the dependency vector for this query
  // which is now ending. So, replace result->dependencies with it.
  {
    QueryDepsEntry& back = queryDeps.back();
    result->dependencies.swap(back.dependencies);
    result->errors.swap(back.errors);
  }
  queryDeps.pop_back();
  // additionally, we've run a query and there might well be
  // a parent query. In that event, we should update the dependency
  // vector for the parent query.
  saveDependenciesInParent(result);
}

void Context::queryNoteError(ErrorMessage error) {
  assert(queryDeps.size() > 0);
  queryDeps.back().errors.push_back(std::move(error));
}


namespace querydetail {


template<>
std::size_t queryArgsHash<>(const std::tuple<>& tuple) {
  return 0;
}

template<>
bool queryArgsEquals<>(const std::tuple<>& lhs,
                       const std::tuple<>& rhs) {
  return true;
}

template<>
void queryArgsPrint<>(const std::tuple<>& tuple) {
}

void queryArgsPrintSep() {
  printf(", ");
}

void queryArgsPrintUnknown() {
  printf("?");
}

void queryArgsPrintOne(const ID& v) {
  printf("ID(%s@%i)", v.symbolPath().c_str(), v.postOrderId());
}
void queryArgsPrintOne(const UniqueString& v) {
  printf("\"%s\"", v.c_str());
}

QueryMapResultBase::~QueryMapResultBase() {
}

QueryMapBase::~QueryMapBase() {
}


} // end namespace querydetail
} // end namespace chpl
