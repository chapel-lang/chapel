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

#include "chpl/framework/Context.h"

#include "chpl/framework/query-impl.h"
#include "chpl/framework/global-strings.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/stringify-functions.h"

#include <chrono>
#include <fstream>
#include <iomanip>

#include <cassert>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include "../util/my_aligned_alloc.h" // assumes size_t defined

namespace chpl {

  namespace detail {
    GlobalStrings globalStrings;
    Context rootContext;

    static void initGlobalStrings() {
#define X(field, str) globalStrings.field = UniqueString::get(&rootContext, str);
#include "chpl/framework/all-global-strings.h"
#undef X
    }
  } // namespace detail

  Context::Context(std::string chplHome,
                   std::unordered_map<std::string, std::string> chplEnvOverrides)
    : chplHome(std::move(chplHome)),
      chplEnvOverrides(std::move(chplEnvOverrides)) {
    if (this == &detail::rootContext) {
      detail::initGlobalStrings();
      for (auto& v : uniqueStringsTable) {
        doNotCollectUniqueCString(v.str);
      }
    } else {
      for (const auto& v : detail::rootContext.uniqueStringsTable) {
        uniqueStringsTable.insert(v);
      }
    }
  }

using namespace chpl::querydetail;


llvm::ErrorOr<const ChplEnvMap&> Context::getChplEnv() {
  if (chplHome.empty() || computedChplEnv) return chplEnv;
  auto chplEnvResult = ::chpl::getChplEnv(chplEnvOverrides, chplHome.c_str());
  if (auto err = chplEnvResult.getError()) {
    // forward error to caller
    return err;
  }
  chplEnv = std::move(chplEnvResult.get());
  computedChplEnv = true;
  return chplEnv;
}

static void defaultReportErrorPrintDetail(Context* context,
                                          const ErrorMessage& err,
                                          const char* prefix,
                                          const char* kind) {
  Location loc = err.location(context);
  const char* path = loc.path().c_str();
  int lineno = loc.line();
  bool validPath = (path != nullptr && path[0] != '\0');

  if (validPath && lineno > 0) {
    printf("%s%s:%i: %s: %s\n",
           prefix, path, lineno, kind, err.message().c_str());
  } else if (validPath) {
    printf("%s%s: %s: %s\n",
           prefix, path, kind, err.message().c_str());
  } else {
    printf("%s%s: %s\n",
           prefix, kind, err.message().c_str());
  }

  for (const auto& detail : err.details()) {
    defaultReportErrorPrintDetail(context, detail, "  ", "note");
  }
}

void Context::defaultReportError(Context* context, const ErrorMessage& err) {
  // if the err came with a kind, use it, otherwise just call it an "error"
  std::string errKindString;
  switch (err.kind()) {
    case ErrorMessage::Kind::WARNING:
      errKindString = "warning";
      break;
    case ErrorMessage::Kind::NOTE:
      errKindString = "note";
      break;
    case ErrorMessage::Kind::SYNTAX:
      errKindString = "syntax";
      break;
    default:
      errKindString = "error";
  }

  defaultReportErrorPrintDetail(context, err, "", errKindString.c_str());
}

// unique'd strings are preceded by 4 bytes of length, gcMark and doNotCollectMark
// this number must be even
#define UNIQUED_STRING_METADATA_BYTES 6
#define UNIQUED_STRING_METADATA_LEN 4

Context::~Context() {
  // free all the unique'd strings
  for (auto& item: uniqueStringsTable) {
    char* buf = (char*) item.str;
    buf -= UNIQUED_STRING_METADATA_BYTES;
    char doNotCollect = buf[UNIQUED_STRING_METADATA_LEN + 1];
    // Root context  : Free all strings
    // Other contexts: Free all that aren't doNotCollect
    if (this == &detail::rootContext || !doNotCollect) {
      free(buf);
    }
  }
}

#define ALIGN_DN(i, size)  ((i) & ~((size) - 1))
#define ALIGN_UP(i, size)  ALIGN_DN((i) + (size) - 1, size)

static char* allocateEvenAligned(size_t amt) {
  char* buf = (char*) malloc(amt);
  // UNIQUED_STRING_METADATA_BYTES must be even
  static_assert((UNIQUED_STRING_METADATA_BYTES & 1) == 0, "UniquedString metadata bytes not even");
  // Normally, malloc returns something that is aligned to 16 bytes,
  // but it's technically possible that a platform library
  // could not do so. So, here we check.
  // We only need even alignment here.
  if ((((uintptr_t)buf) & 1) != 0) {
    free(buf);
    // try again with an aligned allocation
    size_t alignment = sizeof(void *);
    size_t align_up_len = ALIGN_UP(amt, sizeof(void*));
    buf = (char*) my_aligned_alloc(alignment, align_up_len);
  }
  assert(buf);
  assert((((uintptr_t)buf) & 1) == 0);
  return buf;
}

// Assumes buf starts with UNIQUED_STRING_METADATA_BYTES of metadata and has
// room for len and then a null terminator.
// Assumes that buf is allocated with even alignment
// Returns the pointer to where to copy the string and null terminator.
char* Context::setupStringMetadata(char* buf, size_t len) {
  char gcMark = this->gcCounter & 0xff;

  assert(len <= INT32_MAX);

  int32_t len32 = len;
  // these assert should fail if the below code needs to change
  static_assert(sizeof(len32) + 2 == UNIQUED_STRING_METADATA_BYTES, "Size mismatch");
  static_assert(sizeof(len32) == UNIQUED_STRING_METADATA_LEN, "Size mismatch");

  // copy the length
  memcpy(buf, &len32, sizeof(len32));
  buf += sizeof(len32);
  // set the GC mark
  *buf = gcMark;
  buf++;
  // set the doNotCollectMark
  *buf = 0;
  buf++;
  return buf;
}

// allocates new storage for the string if it was not found
const char* Context::getOrCreateUniqueString(const char* str, size_t len) {
  chpl::detail::StringAndLength key = {str, len};
  auto search = this->uniqueStringsTable.find(key);
  if (search != this->uniqueStringsTable.end()) {
    const char* ret = search->str;
    // update the GC mark
    this->markUniqueCString(ret);
    return ret;
  }

  size_t allocLen = UNIQUED_STRING_METADATA_BYTES+len+1; // metadata, len, null
  char* buf = allocateEvenAligned(allocLen);
  // setup metadata
  char* s = setupStringMetadata(buf, len);
  // copy string data and null terminator
  memcpy(s, str, len);
  // null terminate
  s[len] = 0x0;
  // Add it to the table
  chpl::detail::StringAndLength ret = {s, len};
  this->uniqueStringsTable.insert(search, ret);
  return s;
}

const char* Context::uniqueCString(const char* str, size_t len) {
  if (len == 0 || str == nullptr) str = "";
  return this->getOrCreateUniqueString(str, len);
}

const char* Context::uniqueCString(const char* str) {
  if (str == nullptr) str = "";
  return this->getOrCreateUniqueString(str, strlen(str));
}

const char* Context::uniqueCStringConcatLen(const char* s1, size_t len1,
                                            const char* s2, size_t len2,
                                            const char* s3, size_t len3,
                                            const char* s4, size_t len4,
                                            const char* s5, size_t len5,
                                            const char* s6, size_t len6,
                                            const char* s7, size_t len7,
                                            const char* s8, size_t len8,
                                            const char* s9, size_t len9) {
  size_t len = len1 + len2 + len3 + len4 + len5 + len6 + len7 + len8 + len9;

  size_t allocLen = UNIQUED_STRING_METADATA_BYTES+len+1; // metadata, len, null
  char* buf = allocateEvenAligned(allocLen);
  // setup metadata
  char* s = setupStringMetadata(buf, len);

  len = 0;

  // copy each non-empty string
  if (len1 > 0) {
    memcpy(&s[len], s1, len1);
    len += len1;
  }
  if (len2 > 0) {
    memcpy(&s[len], s2, len2);
    len += len2;
  }
  if (len3 > 0) {
    memcpy(&s[len], s3, len3);
    len += len3;
  }
  if (len4 > 0) {
    memcpy(&s[len], s4, len4);
    len += len4;
  }
  if (len5 > 0) {
    memcpy(&s[len], s5, len5);
    len += len5;
  }
  if (len6 > 0) {
    memcpy(&s[len], s6, len6);
    len += len6;
  }
  if (len7 > 0) {
    memcpy(&s[len], s7, len7);
    len += len7;
  }
  if (len8 > 0) {
    memcpy(&s[len], s8, len8);
    len += len8;
  }
  if (len9 > 0) {
    memcpy(&s[len], s9, len9);
    len += len9;
  }

  // null terminate
  s[len] = '\0';

  // Check for it in the table
  chpl::detail::StringAndLength key = {s, len};
  auto search = this->uniqueStringsTable.find(key);
  if (search != this->uniqueStringsTable.end()) {
    const char* ret = search->str;
    // update the GC mark
    this->markUniqueCString(ret);
    free(buf);
    return ret;
  }

  // Add it to the table
  chpl::detail::StringAndLength ret = {s, len};
  this->uniqueStringsTable.insert(search, ret);
  return s;
}

const char* Context::uniqueCStringConcat(const char* s1,
                                         const char* s2,
                                         const char* s3,
                                         const char* s4,
                                         const char* s5,
                                         const char* s6,
                                         const char* s7,
                                         const char* s8,
                                         const char* s9) {
  size_t len1 = 0;
  size_t len2 = 0;
  size_t len3 = 0;
  size_t len4 = 0;
  size_t len5 = 0;
  size_t len6 = 0;
  size_t len7 = 0;
  size_t len8 = 0;
  size_t len9 = 0;

  if (s1 != nullptr) len1 = strlen(s1);
  if (s2 != nullptr) len2 = strlen(s2);
  if (s3 != nullptr) len3 = strlen(s3);
  if (s4 != nullptr) len4 = strlen(s4);
  if (s5 != nullptr) len5 = strlen(s5);
  if (s6 != nullptr) len6 = strlen(s6);
  if (s7 != nullptr) len7 = strlen(s7);
  if (s8 != nullptr) len8 = strlen(s8);
  if (s9 != nullptr) len9 = strlen(s9);

  return uniqueCStringConcatLen(s1, len1, s2, len2,
                                s3, len3, s4, len4, s5, len5, s6, len6,
                                s7, len7, s8, len8, s9, len9);
}

void Context::markUniqueCString(const char* s) {
  if (s == nullptr) {
    return; // nothing to do
  }

  bool checkMarked = false;
  bool doMark = (currentRevisionNumber == lastPrepareToGCRevisionNumber);
  char gcMark = this->gcCounter & 0xff;
  char* buf = (char*) s;
  buf -= UNIQUED_STRING_METADATA_BYTES; // find start of metadata
  buf += UNIQUED_STRING_METADATA_LEN;   // pass the length

  #ifndef NDEBUG
    // assertions are enabled, so consider logic about
    // whether or not current values should already be marked
    checkMarked = checkStringsAlreadyMarked;
  #endif

  if (checkMarked) {
    assert(buf[0] == gcMark && "string should already be marked");
  }

  // write the mark if needed
  if (doMark) {
    buf[0] = gcMark;
  }

  assert(0 <= buf[1] && buf[1] <= 1);   // doNotCollectMark bit is 0 or 1
}

void Context::doNotCollectUniqueCString(const char* s) {
  char* buf = (char*)s;
  buf -= UNIQUED_STRING_METADATA_BYTES; // find start of metadata
  buf += UNIQUED_STRING_METADATA_LEN;   // pass the length
  buf += 1;                             // pass the gc mark
  *buf = 1;                             // set doNotCollectMark
}

size_t Context::lengthForUniqueString(const char* s) {
  const char* buf = (char*) s;
  buf -= UNIQUED_STRING_METADATA_BYTES; // find start of metadata
  int32_t len32 = 0;
  memcpy(&len32, buf, sizeof(len32));
  assert(len32 >= 0);
  return len32;
}

bool Context::shouldMarkUnownedPointer(const void* ptr) {
  // don't bother for nullptr
  if (ptr == nullptr)
    return false;

  // shouldn't run any mark code if the revision is not doing GC
  assert(this->currentRevisionNumber == this->lastPrepareToGCRevisionNumber);

  // check that the unowned pointer refers to an owned
  // pointer that we have already marked
  assert(ownedPtrsForThisRevision.count(ptr) != 0);

  // add the pointer to the map
  auto pair = ptrsMarkedThisRevision.insert(ptr);
  // pair.second is 'true' if the insertion took place
  // or 'false' if there already was an element
  return pair.second;
}
bool Context::shouldMarkOwnedPointer(const void* ptr) {
  // don't bother for nullptr
  if (ptr == nullptr)
    return false;

  #ifndef NDEBUG
    // note the pointer value for checking with markUnownedPointer
    ownedPtrsForThisRevision.insert(ptr);
  #endif

  return true;
}

static
const UniqueString& filePathForModuleIdSymbolPathQuery(Context* context,
                                                       UniqueString modIdSymP) {
  QUERY_BEGIN(filePathForModuleIdSymbolPathQuery, context, modIdSymP);

  // return the empty string if it wasn't already set
  // in setFilePathForModuleID.
  UniqueString result;

  return QUERY_END(result);
}

bool Context::filePathForId(ID id,
                            UniqueString& pathOut,
                            UniqueString& parentSymbolPathOut) {
  UniqueString symbolPath = id.symbolPath();

  while (!symbolPath.isEmpty()) {
    auto tupleOfArgs = std::make_tuple(symbolPath);

    bool got = hasCurrentResultForQuery(filePathForModuleIdSymbolPathQuery,
                                        tupleOfArgs);

    if (got) {
      pathOut = filePathForModuleIdSymbolPathQuery(this, symbolPath);
      parentSymbolPathOut = ID::parentSymbolPath(this, symbolPath);
      return true;
    }

    // remove the last path component, e.g. M.N -> M
    symbolPath = ID::parentSymbolPath(this, symbolPath);
  }

  pathOut = UniqueString::get(this, "<unknown file path>");
  parentSymbolPathOut = UniqueString();
  return false;
}

void Context::setFilePathForModuleId(ID moduleID, UniqueString path) {
  UniqueString moduleIdSymbolPath = moduleID.symbolPath();
  auto tupleOfArgs = std::make_tuple(moduleIdSymbolPath);

  updateResultForQuery(filePathForModuleIdSymbolPathQuery,
                       tupleOfArgs, path,
                       "filePathForModuleIdSymbolPathQuery",
                       /* isInputQuery */ false,
                       /* forSetter */ true);

  if (enableDebugTrace) {
    printf("%i SETTING FILE PATH FOR MODULE %s -> %s\n", queryTraceDepth,
           moduleIdSymbolPath.c_str(), path.c_str());
  }
  #ifndef NDEBUG
    UniqueString gotPath;
    UniqueString gotParentSymbolPath;
    bool ok = filePathForId(moduleID, gotPath, gotParentSymbolPath);
    assert(ok && path == gotPath);
  #endif
}

void Context::advanceToNextRevision(bool prepareToGC) {
  this->currentRevisionNumber++;
  this->numQueriesRunThisRevision_ = 0;
  ptrsMarkedThisRevision.clear();
  ownedPtrsForThisRevision.clear();

  if (prepareToGC) {
    this->lastPrepareToGCRevisionNumber = this->currentRevisionNumber;
    gcCounter++;
  }
  if (enableDebugTrace) {
    printf("%i CURRENT REVISION NUMBER IS NOW %i %s\n", queryTraceDepth,
           (int) this->currentRevisionNumber,
           prepareToGC?"PREPARING GC":"");
  }
}

void Context::setDebugTraceFlag(bool enable)  {
  enableDebugTrace = enable;
}

void Context::setBreakOnHash(size_t hashVal) {
  breakSet = true;
  breakOnHash = hashVal;
}

void Context::collectGarbage() {
  // if there are no parent queries, collect some garbage
  assert(queryStack.size() == 0);

  if (enableDebugTrace) {
    printf("%i COLLECTING GARBAGE\n", queryTraceDepth);
  }

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
      const char* key = e.str;
      char* buf = (char*)key;
      buf -= UNIQUED_STRING_METADATA_BYTES; // find start of allocation
      char* allocation = buf;
      buf += UNIQUED_STRING_METADATA_LEN; // pass the length
      // buf[1] is the doNotCollectMark
      if (buf[1] || buf[0] == gcMark) {
        newTable.insert(e);
        if (enableDebugTrace) {
          printf("%i COPYING OVER UNIQUESTRING %s\n", queryTraceDepth, key);
        }
      } else {
        toFree.push_back(allocation);
        if (enableDebugTrace) {
          printf("%i WILL FREE UNIQUESTRING %s\n", queryTraceDepth, key);
        }
      }
    }
    for (char* allocation: toFree) {
      free(allocation);
    }
    uniqueStringsTable.swap(newTable);

    if (enableDebugTrace) {
      size_t nUniqueStringsAfter = uniqueStringsTable.size();
      printf("%i COLLECTED %i UniqueStrings\n", queryTraceDepth,
             (int)(nUniqueStringsBefore-nUniqueStringsAfter));
    }
  }
}

void Context::report(ErrorMessage error) {
  if (queryStack.size() > 0) {
    queryStack.back()->errors.push_back(std::move(error));
    reportError(this, queryStack.back()->errors.back());
  } else {
    reportError(this, error);
  }
}

static void logErrorInContext(Context* context,
                              ErrorMessage::Kind kind,
                              Location loc,
                              const char* fmt,
                              va_list vl) {
  auto err = ErrorMessage::vbuild(kind, loc, fmt, vl);
  context->report(std::move(err));
}

static void logErrorInContext(Context* context,
                              ErrorMessage::Kind kind,
                              ID id,
                              const char* fmt,
                              va_list vl) {
  auto err = ErrorMessage::vbuild(kind, id, fmt, vl);
  context->report(std::move(err));
}

static void logErrorInContext(Context* context,
                              ErrorMessage::Kind kind,
                              const uast::AstNode* ast,
                              const char* fmt,
                              va_list vl) {
  auto err = ErrorMessage::vbuild(kind, ast->id(), fmt, vl);
  context->report(std::move(err));
}

#define CHPL_CONTEXT_LOG_ERROR_HELPER(context__, kind__, pin__, fmt__) \
  do { \
    va_list vl; \
    va_start(vl, fmt__); \
    logErrorInContext(context__, kind__, pin__, fmt__, vl); \
    va_end(vl); \
  } while (0)

// TODO: Similar overloads for NOTE, WARN, etc.
void Context::error(Location loc, const char* fmt, ...) {
  CHPL_CONTEXT_LOG_ERROR_HELPER(this, ErrorMessage::ERROR, loc, fmt);
}

void Context::error(ID id, const char* fmt, ...) {
  CHPL_CONTEXT_LOG_ERROR_HELPER(this, ErrorMessage::ERROR, id, fmt);
}

void Context::error(const uast::AstNode* ast, const char* fmt, ...) {
  CHPL_CONTEXT_LOG_ERROR_HELPER(this, ErrorMessage::ERROR, ast, fmt);
}

void Context::error(const resolution::TypedFnSignature* inFn,
                    const uast::AstNode* ast,
                    const char* fmt, ...) {
  CHPL_CONTEXT_LOG_ERROR_HELPER(this, ErrorMessage::ERROR, ast, fmt);
  // TODO: add note about instantiation & POI stack
}

#undef CHPL_CONTEXT_LOG_ERROR_HELPER

void Context::recomputeIfNeeded(const QueryMapResultBase* resultEntry) {

  //if (enableDebugTrace) {
  //  printf("RECOMPUTING IF NEEDED FOR %p %s\n", resultEntry,
  //         resultEntry->parentQueryMap->queryName);
  //}

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
      // No need to check the dependency again; already did, and it was OK
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
    if (enableDebugTrace) {
      printf("%i DONE RECOMPUTING IF NEEDED -- RECOMPUTED FOR %s\n",
             queryTraceDepth,
             resultEntry->parentQueryMap->queryName);
    }
    return;
  } else {
    updateForReuse(resultEntry);
    if (enableDebugTrace) {
      printf("%i DONE RECOMPUTING IF NEEDED -- REUSED FOR %s\n",
             queryTraceDepth,
             resultEntry->parentQueryMap->queryName);
    }
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
      err.mark(this);
    }
  }
  resultEntry->lastChecked = this->currentRevisionNumber;

  // Update error locations if needed and re-report the error
  for (auto& err: resultEntry->errors) {
    reportError(this, err);
  }
}

bool Context::queryCanUseSavedResult(
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

  return useSaved;
}

bool Context::queryCanUseSavedResultAndPushIfNot(
                   const void* queryFunction,
                   const QueryMapResultBase* resultEntry) {

  bool useSaved = queryCanUseSavedResult(queryFunction, resultEntry);

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
    // increment the number of queries run in this revision
    numQueriesRunThisRevision_++;
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
  // we are trying to compute it when a recursive call was made. In that event
  // it is a severe error with the compiler implementation.
  // This is a severe internal error and compilation cannot proceed.
  // This uses 'exit' so that it can be tested but in the future we could
  // make it call an internal error function that also exits.
  // If this happens, the solution is to fix the query not to recurse.
  gdbShouldBreakHere();
  fprintf(stderr, "Error: recursion encountered in query %s\n",
          r->parentQueryMap->queryName);
  exit(-1);
}

void Context::queryTimingReport(std::ostream& os) {
  auto elapsed = [](QueryTimingDuration d) {
    double ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
    return ms;
  };

  os << "query timings\n";

  auto w1 = 40;
  auto w2 = 14;

  os << std::setw(w1) << "name"  << std::setw(w2) << "query (ms)"
     << std::setw(w2) << "calls" << std::setw(w2) << "getMap (ms)"
     << std::setw(w2) << "calls" << std::setw(w2) << "getResult (ms)"
     << std::setw(w2) << "calls" << "\n";

  for (const auto& it : queryDB) {
    QueryMapBase* base = it.second.get();
    const auto& timings = base->timings;

    os << std::setw(w1) << base->queryName
      // query
       << std::setw(w2) << elapsed(timings.query.elapsed)
       << std::setw(w2) << timings.query.count
      // getMap
       << std::setw(w2) << elapsed(timings.systemGetMap.elapsed)
       << std::setw(w2) << timings.systemGetMap.count
      // getResult
       << std::setw(w2) << elapsed(timings.systemGetResult.elapsed)
       << std::setw(w2) << base->timings.systemGetResult.count << "\n";
    }
}

// TODO should these be ifdef'd away if !QUERY_TIMING_ENABLED? Or just warn?
void Context::beginQueryTimingTrace(const std::string& outname) {
  queryTimingTraceOutput = std::make_unique<std::ofstream>(outname);
  enableQueryTimingTrace = true;
}

void Context::endQueryTimingTrace() {
  queryTimingTraceOutput.reset();
  enableQueryTimingTrace = false;
}

namespace querydetail {


void queryArgsPrintSep() {
  printf(", ");
}

QueryMapResultBase::~QueryMapResultBase() {
}

QueryMapBase::~QueryMapBase() {
}


} // end namespace querydetail
} // end namespace chpl
