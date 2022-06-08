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

#ifndef CHPL_QUERIES_CONTEXT_H
#define CHPL_QUERIES_CONTEXT_H

#include "chpl/queries/Context-detail.h"
#include "chpl/queries/ID.h"
#include "chpl/queries/CommentID.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/util/memory.h"
#include "chpl/util/hash.h"
#include "chpl/util/break.h"

#include <memory>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace chpl {

namespace uast {
  class AstNode;
}

namespace resolution {
  class TypedFnSignature;
}


/**

\rst

This Context class stores the compilation-wide context. Another name for this
compilation-wide context is *program database*. It handles unique'd strings and
also stores the results of queries (so that they are are memoized). It tracks
dependencies of queries in order to update them appropriately when a dependency
changes.

Please see :ref:`Chapter-dyno-chpl-queries` for more information about how to
implement queries and how the query framework functions.

\endrst

 */
class Context {
 private:
  // map that supports uniqueCString / UniqueString
  using UniqueStringsTableType = std::unordered_set<chpl::detail::StringAndLength, chpl::detail::UniqueStrHash, chpl::detail::UniqueStrEqual>;
  UniqueStringsTableType uniqueStringsTable;

  // Map from a query function pointer to appropriate QueryMap object.
  // Maps to an 'owned' heap-allocated thing to manage having subclasses
  // without slicing.
  // It assumes that the query name is already unique.
  std::unordered_map<const void*, owned<querydetail::QueryMapBase>> queryDB;

  // Since IDs include module names but not file paths, use this
  // map to go from module name to file path.
  // (If this proves too restrictive for some reason, we could
  //  start including file path in IDs).
  std::unordered_map<UniqueString, UniqueString> modNameToFilepath;

  std::vector<const querydetail::QueryMapResultBase*> queryStack;

  // for avoiding infinite loops in markPointer assertion checking
  std::unordered_set<const void*> ptrsMarkedThisRevision;

  // queries can return an owned pointer and then other queries
  // can depend on the value. this set tracks owned pointers returned
  // by queries. markPointer can check, when asserting is on, that
  // its argument is always in this set.
  // (otherwise, it would be potentially a memory error, because
  //  the owned pointer could be deleted if it is not expressed as
  //  a dependency).
  std::unordered_set<const void*> ownedPtrsForThisRevision;

  querydetail::RevisionNumber currentRevisionNumber = 1;
  bool checkStringsAlreadyMarked = false;
  bool enableDebugTrace = false;
  bool enableQueryTiming = false;
  bool enableQueryTimingTrace = false;
  bool breakSet = false;
  size_t breakOnHash = 0;
  int numQueriesRunThisRevision_ = 0;

  owned<std::ostream> queryTimingTraceOutput = nullptr;

  static void defaultReportError(const ErrorMessage& err);
  void (*reportError)(const ErrorMessage& err) = defaultReportError;

  // The following are only used for UniqueString garbage collection
  querydetail::RevisionNumber lastPrepareToGCRevisionNumber = 0;
  querydetail::RevisionNumber gcCounter = 1;

  char* setupStringMetadata(char* buf, size_t len);
  const char* getOrCreateUniqueStringWithAllocation(char* buf,
                                                    const char* str,
                                                    size_t len);
  const char* getOrCreateUniqueString(const char* str, size_t len);

  bool shouldMarkUnownedPointer(const void* ptr);
  bool shouldMarkOwnedPointer(const void* ptr);

  // saves the dependency in the parent query, which is assumed
  // to be at queryStack.back().
  void saveDependencyInParent(const querydetail::QueryMapResultBase* resultEntry);
  void endQueryHandleDependency(const querydetail::QueryMapResultBase* result);

  template<typename ResultType,
           typename... ArgTs>
  querydetail::QueryMap<ResultType, ArgTs...>*
  getMap(const ResultType& (*queryFunction)(Context* context, ArgTs...),
         const std::tuple<ArgTs...>& tupleOfArgs,
         const char* traceQueryName,
         bool isInputQuery);

  // if the result is present in the map, getResult returns it.
  // if not, it adds a new default-constructed result to the map
  // which will have lastChecked and lastChanged set to -1.
  template<typename ResultType,
           typename... ArgTs>
  const querydetail::QueryMapResult<ResultType, ArgTs...>*
  getResult(querydetail::QueryMap<ResultType, ArgTs...>* queryMap,
            const std::tuple<ArgTs...>& tupleOfArgs);

  void haltForRecursiveQuery(const querydetail::QueryMapResultBase* r);

  template<typename ResultType,
           typename... ArgTs>
  const querydetail::QueryMapResult<ResultType, ArgTs...>*
  updateResultForQueryMapR(
      querydetail::QueryMap<ResultType, ArgTs...>* queryMap,
      const querydetail::QueryMapResult<ResultType, ArgTs...>* r,
      const std::tuple<ArgTs...>& tupleOfArgs,
      ResultType result,
      bool forSetter);

  template<typename ResultType,
           typename... ArgTs>
  const querydetail::QueryMapResult<ResultType, ArgTs...>*
  updateResultForQueryMap(
      querydetail::QueryMap<ResultType, ArgTs...>* queryMap,
      const std::tuple<ArgTs...>& tupleOfArgs,
      ResultType result,
      bool forSetter);

  template<typename ResultType,
           typename... ArgTs>
  const querydetail::QueryMapResult<ResultType, ArgTs...>*
  updateResultForQuery(
       const ResultType& (*queryFunction)(Context* context, ArgTs...),
       const std::tuple<ArgTs...>& tupleOfArgs,
       ResultType result,
       const char* traceQueryName,
       bool isInputQuery,
       bool forSetter);

  void recomputeIfNeeded(const querydetail::QueryMapResultBase* resultEntry);
  void updateForReuse(const querydetail::QueryMapResultBase* resultEntry);

  // Checks to see if the current result exists and can be reused.
  // This can run queries that it depended on in the previous revision again
  // and it can update the query's lastChecked value.
  bool queryCanUseSavedResult(
            const void* queryFunction,
            const querydetail::QueryMapResultBase* resultEntry);

  // In addition to the steps in queryCanUseSavedResult, if the result
  // cannot be reused, adds the query to the stack of currently executing
  // queries
  bool queryCanUseSavedResultAndPushIfNot(
            const void* queryFunction,
            const querydetail::QueryMapResultBase* resultEntry);

  void doNotCollectUniqueCString(const char *s);

  // Future Work: make the context thread-safe

  // Future Work: allow moving some AST to a different context
  //              (or, at least, that can handle the unique strings)

  // Performance: Add fine-grained timing to measure
  //  * the total time spent in each query
  //  * the time spent in each query in Context functions
  //    (i.e. hashtable manipulations)
  //  * the time spent in each query in other queries
  //  * the time spent in each query in other query code

  // Performance: How can we arrange for better locality of reference/
  // cache reuse for the maps from IDs? The IDs within a function could
  // be just stored in a vector, but that would add an indirection to
  // the hashtable. Is there a way to adjust the hashing function and
  // tune the hashtable bucket size, or something? Do we need a custom
  // hashtable?

 public:
  /**
    Create a new AST Context.
   */
  Context();
  ~Context();

  /**
   Set the error handling function
   */
  void setErrorHandler(void (*reportError)(const ErrorMessage& err))
  {
    this->reportError = reportError;
  }

  /**

    Get or create a unique string and return it as a C string. If the passed
    string is NULL, this function will return an empty string.

    Unique strings are limited to 2**31 bytes.

    The returned string will store len bytes, even if there are interior
    NULL bytes. It will be NULL terminated either way.

    Strings returned by this function will always be aligned to 2 bytes.

    The function `UniqueString::get` returns such a string
    with a wrapper type. It should be preferred for type safety
    and to reduce redundant checks.
   */
  const char* uniqueCString(const char* s, size_t len);
  /**
    Calls uniqueCString with len=strlen(s). This simpler
    call can be used for C strings that don't contain zero bytes
    other than the terminator.
   */
  const char* uniqueCString(const char* s);
  /**
    Get or create a unique string by concatenating up to 9 strings
    with lengths.
   */
  const char* uniqueCStringConcatLen(const char* s1, size_t len1,
                                     const char* s2, size_t len2,
                                     const char* s3 = nullptr, size_t len3 = 0,
                                     const char* s4 = nullptr, size_t len4 = 0,
                                     const char* s5 = nullptr, size_t len5 = 0,
                                     const char* s6 = nullptr, size_t len6 = 0,
                                     const char* s7 = nullptr, size_t len7 = 0,
                                     const char* s8 = nullptr, size_t len8 = 0,
                                     const char* s9 = nullptr, size_t len9 = 0);
  /**
    Get or create a unique string by concatenating up to 9 strings.
   */
  const char* uniqueCStringConcat(const char* s1,
                                  const char* s2,
                                  const char* s3 = nullptr,
                                  const char* s4 = nullptr,
                                  const char* s5 = nullptr,
                                  const char* s6 = nullptr,
                                  const char* s7 = nullptr,
                                  const char* s8 = nullptr,
                                  const char* s9 = nullptr);



  /**
   When the context is configured to run with garbage collection
   enabled, unique strings that are reused need to be marked.
   This function does that for a C string stored in the map.
   It will cause program crashes if called on a string that
   is not the result of one of the uniqueCString calls.
   */
  void markUniqueCString(const char* s);

  /**
   For a unique string, return the length of the string when it was created.
   It will cause program crashes if called on a string that
   is not the result of one of the uniqueCString calls.
   */
  static size_t lengthForUniqueString(const char* s);

  /**

   This function can be called by a mark method to perform checking on
   pointers and any UniqueStrings that they contain when asserts are enabled.

   Pointers available to a mark method should have already
   been returned by previous queries as owned objects and
   so markOwnedPointer should have been called on them if
   they have been reused.
   */
  template<typename T>
  void markUnownedPointer(const T* ptr) {
    #ifndef NDEBUG
      if (shouldMarkUnownedPointer(ptr)) {
        // run mark on the pointer contents while checking
        // all unique strings are already marked
        auto saveCheckStringsAlreadyMarked = checkStringsAlreadyMarked;
        checkStringsAlreadyMarked = true;
        ptr->mark(this);
        // restore the previous setting for checkStringsAlreadyMarked
        checkStringsAlreadyMarked = saveCheckStringsAlreadyMarked;
      }
    #endif
  }

  /**
   This function can be called by a mark method to mark UniqueStrings
   within an owned pointer.
   */
  template<typename T>
  void markOwnedPointer(const T* ptr) {
    if (shouldMarkOwnedPointer(ptr)) {
      // run mark on the object
      ptr->mark(this);
    }
  }

  /**
   markPointer can be used to mark a pointer, where
   it is considered owned if the type is owned.

   This overload just calls markOwnedPointer.
   */
  template<typename T>
  void markPointer(const owned<T>& ptr) {
    markOwnedPointer(ptr.get());
  }

  /**
   markPointer can be used to mark a pointer, where
   it is considered owned if the type is owned.

   This overload just calls markPointer.
   */
  template<typename T>
  void markPointer(const T* ptr) {
    markUnownedPointer(ptr);
  }


  /**
    Return the file path for the file containing this ID.
   */
  UniqueString filePathForId(ID id);

  /**
    Returns true if filePathForId is already populated for
    this ID.
   */
  bool hasFilePathForId(ID id);

  /**
    Sets the file path for the given module ID. This
    is suitable to call from a parse query.
   */
  void setFilePathForModuleID(ID moduleID, UniqueString path);

  /**
    This function increments the current revision number stored
    in the context. After it is called, the setters below can
    be used to provide the input at that revision.

    If the prepareToGC argument is true, when processing queries
    in that revision, will prepare to garbage collect UniqueStrings
    (by marking elements appropriately).
   */
  void advanceToNextRevision(bool prepareToGC);

  /**
    Returns the number of query bodies executed in this revision.
   */
  int numQueriesRunThisRevision() const {
    return numQueriesRunThisRevision_;
  }

  /**
    This function runs garbage collection. It will collect UniqueStrings
    if the last call to advanceToNextRevision passed prepareToGC=true.

    It is an implementation error to call this function while a query
    is running.
   */
  void collectGarbage();

  /**
    Note an error for the currently running query and report it
    with the error handler set by setErrorHandler.

    If no query is currently running, it just reports the error.
   */
  void report(ErrorMessage error);

  /**
    Note an error for the currently running query.
    This is a convenience overload.
    This version takes in a Location and a printf-style format string.
   */
  void error(Location loc, const char* fmt, ...)
  #ifndef DOXYGEN
    // docs generator has trouble with the attribute applied to 'build'
    // so the above ifndef works around the issue.
    __attribute__ ((format (printf, 3, 4)))
  #endif
  ;

  /**
    Note an error for the currently running query.
    This is a convenience overload.
    This version takes in an ID and a printf-style format string.
    The ID is used to compute a Location using parsing::locateId.
   */
  void error(ID id, const char* fmt, ...)
  #ifndef DOXYGEN
    // docs generator has trouble with the attribute applied to 'build'
    // so the above ifndef works around the issue.
    __attribute__ ((format (printf, 3, 4)))
  #endif
  ;

  /**
    Note an error for the currently running query.
    This is a convenience overload.
    This version takes in an AST node and a printf-style format string.
    The AST node is used to compute a Location by using a parsing::locateAst.
   */
  void error(const uast::AstNode* ast, const char* fmt, ...)
  #ifndef DOXYGEN
    // docs generator has trouble with the attribute applied to 'build'
    // so the above ifndef works around the issue.
    __attribute__ ((format (printf, 3, 4)))
  #endif
  ;

  /**
    Note an error for the currently running query.
    This is a convenience overload.
    This version takes in a TypedFnSignature and an AST node and a
    printf-style format string.
    The AST node is used to compute a Location by using a parsing::locateAst.
    The TypedFnSignature is used to print out instantiation information.
   */
  void error(const resolution::TypedFnSignature* inFn,
             const uast::AstNode* ast,
             const char* fmt, ...)
  #ifndef DOXYGEN
    // docs generator has trouble with the attribute applied to 'build'
    // so the above ifndef works around the issue.
    __attribute__ ((format (printf, 4, 5)))
  #endif
  ;

  /**
    Sets the enableDebugTrace flag. This was needed because the context
    in main gets created before the arguments to the compiler are parsed.
  */
  void setDebugTraceFlag(const bool enable);

  /*
    Set the hash value of a query and its arguments to break on. Needed because
    context in main is created before arguments to the compiler are parsed.
  */
  void setBreakOnHash(const size_t hashVal);

  /** Enables/disables timing each query execution */
  void setQueryTimingFlag(bool enable) {
    enableQueryTiming = enable;
  }

  /** Begin query timing trace, sending the trace to outname */
  void beginQueryTimingTrace(const std::string& outname);

  /** End query timing trace, closes out stream */
  void endQueryTimingTrace();

  typedef enum {
    NOT_CHECKED_NOT_CHANGED = 0,
    REUSED = 1,
    CHANGED = 2
  } QueryStatus;

  /**
    Returns:
      0 if the query was not checked or changed in this revision
      1 if the query was checked but not changed in this revision
      2 if the query was changed in this revision

    This is intended only as a debugging aid.
   */
  template<typename ResultType,
           typename... ArgTs>
  QueryStatus queryStatus(
         const ResultType& (*queryFunction)(Context* context, ArgTs...),
         const std::tuple<ArgTs...>& tupleOfArgs);

  /**
    Returns 'true' if the system already has a result for the passed query
    in the current revision. This can be useful for certain input
    queries - e.g. one reading a file that can both have the contents
    set and can also read the data from the filesystem.
   */
  template<typename ResultType,
           typename... ArgTs>
  bool
  hasCurrentResultForQuery(
       const ResultType& (*queryFunction)(Context* context, ArgTs...),
       const std::tuple<ArgTs...>& tupleOfArgs);

  /**
    Returns 'true' if the query in question is currently running.
    This can be useful for avoiding recursion in certain cases.
   */
  template<typename ResultType,
           typename... ArgTs>
  bool
  isQueryRunning(
       const ResultType& (*queryFunction)(Context* context, ArgTs...),
       const std::tuple<ArgTs...>& tupleOfArgs);


  // the following functions are called by the macros defined in QueryImpl.h
  // and should not be called directly

  /// \cond DO_NOT_DOCUMENT
  template<typename... ArgTs>
  void queryBeginTrace(const char* traceQueryName,
                       const std::tuple<ArgTs...>& tupleOfArg);

  template<typename ResultType,
           typename... ArgTs>
  querydetail::QueryMap<ResultType, ArgTs...>*
  queryBeginGetMap(
       const ResultType& (*queryFunction)(Context* context, ArgTs...),
       const std::tuple<ArgTs...>& tupleOfArgs,
       const char* traceQueryName,
       bool isInputQuery);

  template<typename ResultType,
           typename... ArgTs>
  const querydetail::QueryMapResult<ResultType, ArgTs...>*
  queryBeginGetResult(querydetail::QueryMap<ResultType, ArgTs...>* queryMap,
                      const std::tuple<ArgTs...>& tupleOfArgs);


  template<typename ResultType,
           typename... ArgTs>
  bool queryUseSaved(
         const ResultType& (*queryFunction)(Context* context, ArgTs...),
         const querydetail::QueryMapResult<ResultType, ArgTs...>* r,
         const char* traceQueryName);

  template<typename ResultType, typename... ArgTs>
  const ResultType&
  queryGetSaved(const querydetail::QueryMapResult<ResultType, ArgTs...>* r);

  template<typename ResultType,
           typename... ArgTs>
  const ResultType& queryEnd(
      const ResultType& (*queryFunction)(Context* context, ArgTs...),
      querydetail::QueryMap<ResultType, ArgTs...>* queryMap,
      const querydetail::QueryMapResult<ResultType, ArgTs...>* r,
      const std::tuple<ArgTs...>& tupleOfArgs,
      ResultType result,
      const char* traceQueryName);

  template<typename ResultType,
           typename... ArgTs>
  void querySetterUpdateResult(
      const ResultType& (*queryFunction)(Context* context, ArgTs...),
      const std::tuple<ArgTs...>& tupleOfArgs,
      ResultType result,
      const char* traceQueryName,
      bool isInputQuery);


  /**
     Output a timing report of the cumulative time each query spent
   */
  void queryTimingReport(std::ostream& os);

  // Used in the in QUERY_BEGIN_TIMING macro. Creates a stopwatch that starts
  // timing if we are enabled. And then on scope exit we conditionally stop the
  // timing and add it to the total or log it.
  // Semi-public method because we only expect it to be used in the macro
  auto makeQueryTimingStopwatch(querydetail::QueryMapBase* base) {
    size_t depth = queryStack.size();
    bool enabled = enableQueryTiming || enableQueryTimingTrace;

    return querydetail::makeQueryTimingStopwatch(
        enabled,
        // This lambda gets called when the stopwatch object (which lives on the
        // stack of the query function) is destructed
        [this, base, depth, enabled](auto& stopwatch) {
          querydetail::QueryTimingDuration elapsed;
          if (enabled) {
            elapsed = stopwatch.elapsed();
          }
          if (enableQueryTiming) {
            base->timings.query.update(elapsed);
          }
          if (enableQueryTimingTrace) {
            auto ticks = elapsed.count();
            auto os = queryTimingTraceOutput.get();
            assert(os != nullptr);
            *os << depth << ' ' << base->queryName << ' ' << ticks << '\n';
          }
        });
  }

  /// \endcond
};

} // end namespace chpl

#endif
