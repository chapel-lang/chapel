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

static void defaultReportErrorPrintDetail(const ErrorMessage& err) {
  printf("  %s:%i: note: %s\n",
         err.path().c_str(), err.line(), err.message().c_str());
  for (const auto& detail : err.details()) {
    defaultReportErrorPrintDetail(detail);
  }
}
static void defaultReportError(const ErrorMessage& err) {
  printf("%s:%i: error: %s\n",
         err.path().c_str(), err.line(), err.message().c_str());
  for (const auto& detail : err.details()) {
    defaultReportErrorPrintDetail(detail);
  }
}

Context::Context()
  : uniqueStringsTable(), queryDB(), queryStack(),
    currentRevisionNumber(1),
    reportError(defaultReportError),
    lastPrepareToGCRevisionNumber(0), gcCounter(1) {
}

Context::~Context() {
  // free all of the unique'd strings
  for (auto& item: this->uniqueStringsTable) {
    const char* s = item.second;
    free((void*)s);
  }
}

void Context::setErrorHandler(void (*reportError)(const ErrorMessage& err)) {
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
  assert(queryStack.size() == 0);

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

void Context::setFilePathForModuleName(UniqueString modName, UniqueString path) {
  auto tupleOfArgs = std::make_tuple(modName);
  auto queryMapResult = updateResultForQuery(filePathForModuleNameQuery,
                                             tupleOfArgs, path,
                                             "filePathForModuleNameQuery",
                                             false);
  printf("SETTING FILE PATH FOR MODULE %s -> %s\n",
         modName.c_str(), path.c_str());
}

void Context::recomputeIfNeeded(const QueryMapResultBase* resultEntry) {

  printf("RECOMPUTING IF NEEDED FOR %p %s\n", resultEntry,
         resultEntry->parentQueryMap->queryName);

  if (this->currentRevisionNumber == resultEntry->lastChecked) {
    // No need to check the dependencies again.
    // We already know that we can reuse the result.
    return;
  }

  if (resultEntry->parentQueryMap->isInputQuery) {
    // For an input query, compute it once per revision, ignoring
    // dependencies (e.g. if it is reading a file, we need to check that the
    // file has not changed.)
    resultEntry->recompute(this);
    assert(resultEntry->lastChecked == this->currentRevisionNumber);
    return;
  }

  // Otherwise, check the dependencies. Have any of them
  // changed since the last revision in which we computed this?
  // If so, compute it again.
  bool useSaved = true;
  for (const QueryMapResultBase* dependency : resultEntry->dependencies) {
    if (dependency->lastChanged > resultEntry->lastChanged) {
      useSaved = false;
      break;
    } else if (this->currentRevisionNumber == dependency->lastChecked) {
      // No need to check the dependency again; already did and it was OK
    } else {
      recomputeIfNeeded(dependency);
      // we might have recomputed the dependency, so check its lastChanged
      if (dependency->lastChanged > resultEntry->lastChanged) {
        useSaved = false;
        break;
      }
    }
  }

  if (useSaved == false) {
    resultEntry->recompute(this);
    assert(resultEntry->lastChecked == this->currentRevisionNumber);
    printf("DONE RECOMPUTING IF NEEDED -- RECOMPUTED FOR %s\n",
           resultEntry->parentQueryMap->queryName);
    return;
  } else {
    printf("DONE RECOMPUTING IF NEEDED -- REUSED FOR %s\n",
           resultEntry->parentQueryMap->queryName);
    updateForReuse(resultEntry);
  }

}

// this should be called once each revision the first time
// the result is reused. It sets lastChecked = currentRevisionNumber
// and takes some steps to support garbage collection.
void Context::updateForReuse(const QueryMapResultBase* resultEntry) {
  // if we are GC'ing, mark unique strings
  if (this->currentRevisionNumber == this->lastPrepareToGCRevisionNumber) {
    resultEntry->markUniqueStringsInResult(this);
    // and also mark unique strings in the errors
    for (const auto& err: resultEntry->errors) {
      err.markUniqueStrings(this);
    }
  }
  resultEntry->lastChecked = this->currentRevisionNumber;

  // Re-report any errors in the query
  for (const auto& err: resultEntry->errors) {
    reportError(err);
  }
}

bool Context::queryCanUseSavedResultAndPushIfNot(
                   const void* queryFunction,
                   const QueryMapResultBase* resultEntry) {

  bool useSaved = false;

  assert(resultEntry != nullptr);

  if (resultEntry->lastChanged == -1) {
    // If it is a new entry, we can't reuse it
    useSaved = false;
  } else if (this->currentRevisionNumber == resultEntry->lastChecked) {
    // the query was already checked/run in this revision
    useSaved = true;
  } else if (resultEntry->parentQueryMap->isInputQuery) {
    // be sure to re-run input queries
    useSaved = false;
  } else {
    useSaved = true;
    for (const QueryMapResultBase* dependency : resultEntry->dependencies) {
      recomputeIfNeeded(dependency);
      assert(dependency->lastChecked == this->currentRevisionNumber);
      if (dependency->lastChanged > resultEntry->lastChanged) {
        useSaved = false;
        break;
      }
    }
    if (useSaved == true) {
      updateForReuse(resultEntry);
    }
  }

  if (useSaved == false) {
    // Since the result cannot be reused, the query will be evaluated.
    // So, push something to queryDeps
    queryStack.push_back(resultEntry);
    // Record that this query is being recomputed
    // (to enable detecting recursion)
    resultEntry->lastChecked = -1;
    // Clear out the dependencies and errors since these will be recomputed
    // by evaluating the query.
    resultEntry->dependencies.clear();
    resultEntry->errors.clear();
  }

  return useSaved;
}

void Context::saveDependencyInParent(const QueryMapResultBase* resultEntry) {
  // Record that the parent query depends upon this one.
  //
  // We haven't pushed the query beginning yet; on already popped it.
  // So, the parent query is at queryDeps.back().
  if (queryStack.size() > 0) {
    assert(queryStack.back() != resultEntry); // should be parent query
    queryStack.back()->dependencies.push_back(resultEntry);
  }
}
void Context::endQueryHandleDependency(const QueryMapResultBase* resultEntry) {
  // Remove the current query from the stack
  assert(queryStack.back() == resultEntry);
  queryStack.pop_back();

  // We've just the query represented by 'resultEntry'. If that query
  // was called from another query, we need to record that the calling
  // query depends on 'resultEntry'.
  saveDependencyInParent(resultEntry);
}

void Context::haltForRecursiveQuery(const querydetail::QueryMapResultBase* r) {
  // If an old element present has lastChecked == -1, that means that
  // we trying to compute it when a recursive call was made. In that event
  // it is a severe error with the compiler implementation.
  // This is a severe internal error and compilation cannot proceed.
  // This uses 'exit' so that it can be tested but in the future we could
  // make it call an internal error function that also exits.
  // If this happens, the solution is to fix the query not to recurse.
  fprintf(stderr, "Error: recursion encountered in query %s\n",
          r->parentQueryMap->queryName);
  exit(-1);
}

void Context::queryNoteError(ErrorMessage error) {
  assert(queryStack.size() > 0);
  queryStack.back()->errors.push_back(std::move(error));
  reportError(queryStack.back()->errors.back());
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
