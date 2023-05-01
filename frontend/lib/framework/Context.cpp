/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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
#include "chpl/framework/ErrorWriter.h"
#include "chpl/framework/ErrorBase.h"
#include "chpl/framework/compiler-configuration.h"
#include "chpl/util/filesystem.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/Config/llvm-config.h"
#include "llvm/Support/FileSystem.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <utility>

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


using namespace chpl::querydetail;

void Context::Configuration::swap(Context::Configuration& other) {
  std::swap(chplHome, other.chplHome);
  std::swap(chplEnvOverrides, other.chplEnvOverrides);
  std::swap(tmpDir, other.tmpDir);
  std::swap(keepTmpDir, other.keepTmpDir);
  std::swap(toolName, other.toolName);
}

void Context::setupGlobalStrings() {
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

void Context::swap(Context& other) {
  config_.swap(other.config_);
  std::swap(handler_, other.handler_);
  std::swap(computedChplEnv, other.computedChplEnv);
  std::swap(chplEnv, other.chplEnv);
  std::swap(detailedErrors, other.detailedErrors);
  std::swap(uniqueStringsTable, other.uniqueStringsTable);
  std::swap(queryDB, other.queryDB);
  std::swap(modNameToFilepath, other.modNameToFilepath);
  std::swap(queryStack, other.queryStack);
  std::swap(ptrsMarkedThisRevision, other.ptrsMarkedThisRevision);
  std::swap(ownedPtrsForThisRevision, other.ownedPtrsForThisRevision);
  std::swap(currentRevisionNumber, other.currentRevisionNumber);
  std::swap(checkStringsAlreadyMarked, other.checkStringsAlreadyMarked);
  std::swap(enableDebugTrace, other.enableDebugTrace);
  std::swap(enableQueryTiming, other.enableQueryTiming);
  std::swap(enableQueryTimingTrace, other.enableQueryTimingTrace);
  std::swap(currentTerminalSupportsColor_, other.currentTerminalSupportsColor_);
  std::swap(breakSet, other.breakSet);
  std::swap(breakOnHash, other.breakOnHash);
  std::swap(numQueriesRunThisRevision_, other.numQueriesRunThisRevision_);
  std::swap(queryTraceDepth, other.queryTraceDepth);
  std::swap(queryTraceIgnoreQueries, other.queryTraceIgnoreQueries);
  std::swap(queryDepthColor, other.queryDepthColor);
  std::swap(queryTimingTraceOutput, other.queryTimingTraceOutput);
  std::swap(lastPrepareToGCRevisionNumber, other.lastPrepareToGCRevisionNumber);
  std::swap(gcCounter, other.gcCounter);
}

Context::Context() {
  setupGlobalStrings();
}
Context::Context(Configuration config) {
  // swap the configuration settings in to place
  config_.swap(config);

  setupGlobalStrings();
}
Context::Context(Context& consumeContext, Configuration newConfig) {
  // swap all fields in to place from consumeContext
  this->swap(consumeContext);

  // set consumeContext not to delete the temp dir when it is deleted
  // since this context will do so if that is needed.
  consumeContext.config_.keepTmpDir = true;

  // now set the new configuration information
  config_.swap(newConfig);
}

Context::RunResultBase::RunResultBase() = default;

Context::RunResultBase::~RunResultBase() = default;

Context::RunResultBase::RunResultBase(const Context::RunResultBase& other) {
  for (auto& err : other.errors()) {
      errors_.push_back(err->clone());
  }
}

bool Context::RunResultBase::ranWithoutErrors() const {
  for (auto& error : errors_) {
    auto kind = error->kind();
    if (kind == ErrorBase::ERROR || kind == ErrorBase::SYNTAX) {
      return false;
    }
  }
  return true;
}

Context::ErrorCollectionEntry
Context::ErrorCollectionEntry::createForTrackingQuery(
    std::vector<owned<ErrorBase>>* storeInto,
    const QueryMapResultBase* trackingQuery) {
  return Context::ErrorCollectionEntry(storeInto, trackingQuery);
}

Context::ErrorCollectionEntry
Context::ErrorCollectionEntry::createForRecomputing(
    const querydetail::QueryMapResultBase* trackingQuery) {
  return Context::ErrorCollectionEntry(nullptr, trackingQuery);
}

void Context::ErrorCollectionEntry::storeError(owned<ErrorBase> toStore) const {
  if (storeInto_) {
    storeInto_->push_back(std::move(toStore));
  }
}

void Context::reportError(Context* context, const ErrorBase* err) {
  handler_->report(context, err);
}

const std::string& Context::chplHome() const {
  return config_.chplHome;
}

const std::string& Context::tmpDir() {
  if (tmpDir_.empty()) {
    if (!config_.tmpDir.empty()) {
      // if a temp dir was configured, use that
      tmpDir_ = config_.tmpDir;
    } else {
      // otherwise, generate a temp directory
      std::string dir;
      auto err = makeTempDir(config_.toolName + "-", dir);

      if (err) {
        this->error(Location(), "Could not create temp directory");
      } else {
        tmpDir_ = dir;
        tmpDirExists_ = true;
      }
    }
  }

  if (!tmpDirExists_) {
    auto err = llvm::sys::fs::create_directories(tmpDir_);
    if (err) {
      this->error(Location(), "Could not create temp directory %s",
                  tmpDir_.c_str());
    } else {
      tmpDirExists_ = true;
    }
  }

  return tmpDir_;
}

bool Context::shouldSaveTmpDirFiles() const {
  return config_.keepTmpDir;
}

std::string Context::tmpDirAnchorFile() {
  std::string path = tmpDir();
  if (!path.empty() && !tmpDirAnchorCreated_) {
    path += "/anchor";

    std::string data = "anchor\n";
    std::error_code err = writeFile(path.c_str(), data);
    if (err) {
      this->error(Location(), "Could not update anchor file %s: %s",
                  path.c_str(), err.message().c_str());
    } else {
      tmpDirAnchorCreated_ = true;
    }
  }
  return path;
}

void Context::cleanupTmpDirIfNeeded() {
  if (!tmpDir_.empty() && fileExists(tmpDir_.c_str()) && !config_.keepTmpDir) {
    // delete the tmp dir
    deleteDir(tmpDir_);
  }
}

void Context::setDetailedErrorOutput(bool detailedErrors) {
  this->detailedErrors = detailedErrors;
}

llvm::ErrorOr<const ChplEnvMap&> Context::getChplEnv() {
  if (config_.chplHome.empty() || computedChplEnv) return chplEnv;
  auto chplEnvResult = ::chpl::getChplEnv(config_.chplEnvOverrides,
                                          config_.chplHome.c_str());
  if (auto err = chplEnvResult.getError()) {
    // forward error to caller
    return err;
  }
  chplEnv = std::move(chplEnvResult.get());
  computedChplEnv = true;
  return chplEnv;
}

void Context::defaultReportError(Context* context, const ErrorBase* err) {
  ErrorWriter ew(context, std::cerr,
                 context->detailedErrors ?
                   ErrorWriter::DETAILED :
                   ErrorWriter::BRIEF,
                 context->currentTerminalSupportsColor_);
  err->write(ew);
  if (context->detailedErrors) {
    // Print an extra error separator
    std::cerr << std::endl;
  }
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

  // delete the tmp dir
  cleanupTmpDirIfNeeded();
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
  CHPL_ASSERT(buf);
  CHPL_ASSERT((((uintptr_t)buf) & 1) == 0);
  return buf;
}

// Assumes buf starts with UNIQUED_STRING_METADATA_BYTES of metadata and has
// room for len and then a null terminator.
// Assumes that buf is allocated with even alignment
// Returns the pointer to where to copy the string and null terminator.
char* Context::setupStringMetadata(char* buf, size_t len) {
  char gcMark = this->gcCounter & 0xff;

  CHPL_ASSERT(len <= INT32_MAX);

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
    CHPL_ASSERT(buf[0] == gcMark && "string should already be marked");
  }

  // write the mark if needed
  if (doMark) {
    buf[0] = gcMark;
  }

  CHPL_ASSERT(0 <= buf[1] && buf[1] <= 1);   // doNotCollectMark bit is 0 or 1
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
  CHPL_ASSERT(len32 >= 0);
  return len32;
}

bool Context::shouldMarkUnownedPointer(const void* ptr) {
  // don't bother for nullptr
  if (ptr == nullptr)
    return false;

  // shouldn't run any mark code if the revision is not doing GC
  CHPL_ASSERT(this->currentRevisionNumber == this->lastPrepareToGCRevisionNumber);

  // check that the unowned pointer refers to an owned
  // pointer that we have already marked
  CHPL_ASSERT(ownedPtrsForThisRevision.count(ptr) != 0);

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
    // check that querying the module ID works...
    UniqueString gotPath;
    UniqueString gotParentSymbolPath;
    bool ok = filePathForId(moduleID, gotPath, gotParentSymbolPath);
    CHPL_ASSERT(ok);

    // ... and gives the same path

    // Note: if this check causes problems in the future, it could
    // be removed, or we could wire up setFileText used in tests
    // to work with the LLVM VirtualFilesystem
#if LLVM_VERSION_MAJOR <= 11
    llvm::SmallVector<char, 64> realPath, realGotPath;
#else
    llvm::SmallVector<char> realPath, realGotPath;
#endif
    std::error_code errPath;
    std::error_code errGotPath;
    errPath = llvm::sys::fs::real_path(path.str(), realPath);
    errGotPath = llvm::sys::fs::real_path(gotPath.str(), realGotPath);
    if (errPath || errGotPath) {
      // ignore the check if there were errors
    } else {
      CHPL_ASSERT(realPath == realGotPath);
    }
  #endif
}

static
const UniqueString& pathHasLibraryQuery(Context* context,
                                        UniqueString filePath) {
  QUERY_BEGIN(pathHasLibraryQuery, context, filePath);

  UniqueString result;

  return QUERY_END(result);
}

bool Context::pathHasLibrary(const UniqueString& filePath,
                             UniqueString& pathOut) {
  auto tupleOfArgs = std::make_tuple(filePath);

  bool got = hasCurrentResultForQuery(pathHasLibraryQuery,
                                      tupleOfArgs);

  if (got) {
    pathOut = pathHasLibraryQuery(this, filePath);
    return true;
  }

  pathOut = UniqueString::get(this, "<unknown library path>");
  return false;
}

void Context::setLibraryForFilePath(const UniqueString& filePath, const UniqueString& libPath) {
  auto tupleOfArgs = std::make_tuple(filePath);

  updateResultForQuery(pathHasLibraryQuery,
                       tupleOfArgs, libPath,
                       "pathHasLibraryQuery",
                       /* isInputQuery */ false,
                       /* forSetter */ true);
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
  CHPL_ASSERT(queryStack.size() == 0);

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

void Context::report(owned<ErrorBase> error) {
  gdbShouldBreakHere();

  // If errorCollectionStack is not empty, errors are being collected, and
  // thus not reported to the handler. Stash the error in the top (back) of the
  // stack, but do not call `reportError`. Still store it into query
  // results (errors will be re-emitted if the cached query is invoked without
  // error collection).

  if (queryStack.size() > 0 && errorCollectionStack.size() > 0) {
    errorCollectionStack.back().storeError(error->clone());
    queryStack.back()->errors.push_back(std::move(error));
  } else if (queryStack.size() > 0) {
    queryStack.back()->errors.push_back(std::move(error));
    reportError(this, queryStack.back()->errors.back().get());
  } else if (errorCollectionStack.size() > 0) {
    errorCollectionStack.back().storeError(std::move(error));
  } else {
    reportError(this, error.get());
  }
}

static void logErrorInContext(Context* context,
                              ErrorBase::Kind kind,
                              Location loc,
                              const char* fmt,
                              va_list vl) {
  auto err = GeneralError::vbuild(kind, loc, fmt, vl);
  context->report(std::move(err));
}

static void logErrorInContext(Context* context,
                              ErrorBase::Kind kind,
                              ID id,
                              const char* fmt,
                              va_list vl) {
  auto err = GeneralError::vbuild(kind, id, fmt, vl);
  context->report(std::move(err));
}

static void logErrorInContext(Context* context,
                              ErrorBase::Kind kind,
                              const uast::AstNode* ast,
                              const char* fmt,
                              va_list vl) {
  auto err = GeneralError::vbuild(kind, ast->id(), fmt, vl);
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
  CHPL_CONTEXT_LOG_ERROR_HELPER(this, ErrorBase::ERROR, loc, fmt);
}

void Context::error(ID id, const char* fmt, ...) {
  CHPL_CONTEXT_LOG_ERROR_HELPER(this, ErrorBase::ERROR, id, fmt);
}

void Context::error(const uast::AstNode* ast, const char* fmt, ...) {
  CHPL_CONTEXT_LOG_ERROR_HELPER(this, ErrorBase::ERROR, ast, fmt);
}

void Context::error(const resolution::TypedFnSignature* inFn,
                    const uast::AstNode* ast,
                    const char* fmt, ...) {
  CHPL_CONTEXT_LOG_ERROR_HELPER(this, ErrorBase::ERROR, ast, fmt);
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
    auto marker = markRecomputing(true);
    resultEntry->recompute(this);
    CHPL_ASSERT(resultEntry->lastChecked == this->currentRevisionNumber);
    return;
  }

  // Otherwise, check the dependencies. Have any of them
  // changed since the last revision in which we computed this?
  // If so, compute it again.
  bool useSaved = true;
  for (auto& dependency : resultEntry->dependencies) {
    const QueryMapResultBase* dependencyQuery = dependency.query;
    if (dependencyQuery->lastChanged > resultEntry->lastChanged) {
      useSaved = false;
      break;
    } else if (this->currentRevisionNumber == dependencyQuery->lastChecked) {
      // No need to check the dependency again; already did, and it was OK
    } else {
      if (dependency.errorCollectionRoot) {
        errorCollectionStack.push_back(
            ErrorCollectionEntry::createForRecomputing(resultEntry));
      }
      recomputeIfNeeded(dependencyQuery);
      if (dependency.errorCollectionRoot) {
        errorCollectionStack.pop_back();
      }

      // we might have recomputed the dependency, so check its lastChanged
      if (dependencyQuery->lastChanged > resultEntry->lastChanged) {
        useSaved = false;
        break;
      }
    }
  }

  if (useSaved == false) {
    auto marker = markRecomputing(true);
    resultEntry->recompute(this);
    CHPL_ASSERT(resultEntry->lastChecked == this->currentRevisionNumber);
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
  }
  resultEntry->lastChecked = this->currentRevisionNumber;
  resultEntry->emittedErrors = errorCollectionStack.empty();

  // Update error locations if needed and re-report the error
  // Only re-report errors if they are not being silenced.
  if (errorCollectionStack.empty()) {
    for (auto& err: resultEntry->errors) {
      reportError(this, err.get());
    }
  }
}

bool Context::queryCanUseSavedResult(
                   const void* queryFunction,
                   const QueryMapResultBase* resultEntry) {

  bool useSaved = false;

  CHPL_ASSERT(resultEntry != nullptr);

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
    for (auto& dependency: resultEntry->dependencies) {
      const QueryMapResultBase* dependencyQuery = dependency.query;

      if (dependency.errorCollectionRoot) {
        errorCollectionStack.push_back(
            ErrorCollectionEntry::createForRecomputing(resultEntry));
      }
      recomputeIfNeeded(dependencyQuery);
      if (dependency.errorCollectionRoot) {
        errorCollectionStack.pop_back();
      }

      CHPL_ASSERT(dependencyQuery->lastChecked == this->currentRevisionNumber);
      if (dependencyQuery->lastChanged > resultEntry->lastChanged) {
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

void Context::emitHiddenErrorsFor(const querydetail::QueryMapResultBase* result) {
  CHPL_ASSERT(!result->emittedErrors);
  for (auto& error : result->errors) {
    reportError(this, error.get());
  }
  result->emittedErrors = true;
  for (auto& dependency : result->dependencies) {
    if (!dependency.query->emittedErrors && !dependency.errorCollectionRoot) {
      emitHiddenErrorsFor(dependency.query);
    }
  }
}

static void storeErrorsForHelp(const querydetail::QueryMapResultBase* result,
                               std::unordered_set<const querydetail::QueryMapResultBase*>& visited,
                               Context::ErrorCollectionEntry& into) {
  auto insertResult = visited.insert(result);
  if (!insertResult.second) return;
  for (auto& error : result->errors) {
    into.storeError(error->clone());
  }
  for (auto& dependency : result->dependencies) {
    if (!dependency.errorCollectionRoot) {
      storeErrorsForHelp(dependency.query, visited, into);
    }
  }
}

void Context::storeErrorsFor(const querydetail::QueryMapResultBase* result) {
  CHPL_ASSERT(!errorCollectionStack.empty());
  auto& trackingEntry = errorCollectionStack.back();
  std::unordered_set<const querydetail::QueryMapResultBase*> visited;
  storeErrorsForHelp(result, visited, trackingEntry);
}

void Context::saveDependencyInParent(const QueryMapResultBase* resultEntry) {
  // Record that the parent query depends upon this one.
  //
  // We haven't pushed the query beginning yet; or already popped it.
  // So, the parent query is at queryDeps.back().
  if (isRecomputing) {
    // Do nothing; do not modify dependency graph if we're just checking
    // canUsedSavedResult or recomputeIfNeeded.
  } else if (queryStack.size() > 0) {
    auto parentQuery = queryStack.back();
    CHPL_ASSERT(parentQuery != resultEntry); // should be parent query
    bool errorCollectionRoot = !errorCollectionStack.empty() &&
                               errorCollectionStack.back().collectingQuery() == parentQuery;
    parentQuery->dependencies.push_back(QueryDependency(resultEntry, errorCollectionRoot));
  }

  // The resultEntry might have been a query that silences errors. However,
  // the new parent query might be a query that does not itself silence errors,
  // and thus errors might now need to be emitted.

  if (!resultEntry->emittedErrors && errorCollectionStack.empty()) {
    emitHiddenErrorsFor(resultEntry);
  }
}
void Context::endQueryHandleDependency(const QueryMapResultBase* resultEntry) {
  // Remove the current query from the stack
  CHPL_ASSERT(queryStack.back() == resultEntry);
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

QueryMapResultBase::QueryMapResultBase(RevisionNumber lastChecked,
                   RevisionNumber lastChanged,
                   bool emittedErrors,
                   QueryMapBase* parentQueryMap)
  : lastChecked(lastChecked),
    lastChanged(lastChanged),
    dependencies(),
    emittedErrors(emittedErrors),
    errors(),
    parentQueryMap(parentQueryMap) {
}

QueryMapResultBase::~QueryMapResultBase() {
}

QueryMapBase::~QueryMapBase() {
}


} // end namespace querydetail
} // end namespace chpl
