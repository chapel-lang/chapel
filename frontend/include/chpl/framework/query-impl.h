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

#ifndef CHPL_QUERIES_QUERY_IMPL_H
#define CHPL_QUERIES_QUERY_IMPL_H

#include "chpl/framework/Context-detail.h"
#include "chpl/framework/Context.h"
#include "chpl/framework/mark-functions.h"
#include "chpl/framework/update-functions.h"
#include "chpl/framework/stringify-functions.h"

#ifndef CHPL_QUERY_TIMING_AND_TRACE_ENABLED
#define CHPL_QUERY_TIMING_AND_TRACE_ENABLED 1
#endif

/**
  This file should be included by .cpp files implementing queries.

  This comment describes how the query framework works. For how to use it, see
  the comment describing the Context class.

  Queries are just functions that call special macros at the start and end of
  the functions. The query framework uses these macros to cache the results of
  queries and to recompute these results when needed according to a dependency
  graph.

  What do these functions do, in more detail?

  At the start of a query:

   * if the query has been run before, check to see if any dependencies have
     changed.

     * this is done in a recursive manner; the dependencies of a query are
       processed before that query itself
     * input queries are recomputed if the current revision number has changed.
       This is so that e.g. file contents on disk are read again and
       checked for changes.
     * after a dependency is recomputed, it is checked whether or not it has
       changed. If it has not changed, it is marked as checked (lastChecked is
       set) and the lastChanged value is what it was before. The result is that
       queries that depend on it do not need to be recomputed. If it has
       changed, lastChanged is updated, which will cause queries that depend on
       it to, in turn, run again.

   * if any of the dependencies have changed, or if this is an input query and
     it hasn't been run in the current revision number, or if there is no cached
     result for this query, the query is run.
     Otherwise, the cached result is reused (and the function returns early,
     without running the main body of the query).

  When reusing the cached result of a query:

   * the function returns early, after the dependency checking, with
     the old result
   * the `mark` function is called for the old result, to mark any UniqueStrings
     stored in the result so that they are not garbage collected. This is not
     necessary when recomputing the result since the UniqueString build function
     also marks the UniqueStrings.

  When running the body of the query:

   * any query that is called by it will be recorded automatically in the list
     of dependencies.

   * at the end of the function, the result of the query is compared with the
     old result. If it changed, the 'lastChanged' value is updated. If it did
     not change, the old result is kept in the maps and the new result is
     discarded.

 */

/// \cond DO_NOT_DOCUMENT
namespace chpl {

using namespace chpl::querydetail;


template<typename ResultType, typename... ArgTs, size_t... Is>
static void helpRecompute(Context* context,
                          QueryMap<ResultType, ArgTs...>* queryMap,
                          const std::tuple<ArgTs...>& t,
                          std::index_sequence<Is...>) {
  // Run the query function. It will update the saved result
  // when it runs QUERY_END.
  queryMap->queryFunction(context, std::get<Is>(t)...);
}

template<typename ResultType, typename... ArgTs>
void
QueryMapResult<ResultType,ArgTs...>::recompute(Context* context) const {
  QueryMap<ResultType, ArgTs...>* queryMap =
    (QueryMap<ResultType, ArgTs...>*) this->parentQueryMap;

  const std::tuple<ArgTs...>& args = this->tupleOfArgs;
  helpRecompute(context, queryMap, args, std::index_sequence_for<ArgTs...> {});
}

template<typename ResultType, typename... ArgTs>
void
QueryMapResult<ResultType,ArgTs...>::markUniqueStringsInResult(Context* context) const {
  chpl::mark<ResultType> marker;
  marker(context, this->result);
}

template<typename... ArgTs>
void Context::queryBeginTrace(const char* traceQueryName,
                              const std::tuple<ArgTs...>& tupleOfArg) {

  if ((breakSet || enableDebugTrace)
      && std::find(queryTraceIgnoreQueries.begin(),
                   queryTraceIgnoreQueries.end(),
                   traceQueryName) == queryTraceIgnoreQueries.end()) {
    auto args = queryArgsToStrings(tupleOfArg);
    size_t queryAndArgsHash = hash_combine(hash(traceQueryName), hash(args));
    if (enableDebugTrace) {
      queryTraceDepth++;
      // QUERY BEGIN
      setQueryDepthColor(queryTraceDepth, std::cout);
      std::cout << queryTraceDepth
                << " { ";
      clearTerminalColor(std::cout);
      std::cout << traceQueryName << " (";
      queryArgsPrint(tupleOfArg);
      std::cout << ") ";
      setTerminalColor(CYAN, std::cout);
      std::cout <<"hash: 0x"
                << std::hex << queryAndArgsHash;
      clearTerminalColor(std::cout);
      std::cout << std::endl;
    }
    if (breakSet && queryAndArgsHash == breakOnHash) {
      debuggerBreakHere();
    }
  }
}

template<typename ResultType,
         typename... ArgTs>
QueryMap<ResultType, ArgTs...>*
Context::getMap(const ResultType& (*queryFunc)(Context* context, ArgTs...),
                const std::tuple<ArgTs...>& tupleOfArgs,
                const char* traceQueryName,
                bool isInputQuery) {

  const void* queryFuncV = (const void*) queryFunc;

  // Look up the map entry for this query
  auto search = this->queryDB.find(queryFuncV);
  if (search != this->queryDB.end()) {
    // found an entry for this query
    QueryMapBase* base = search->second.get();
    // return the inner map
    return (QueryMap<ResultType, ArgTs...>*)base;
  }

  // Otherwise, create the QueryMap entry for this query
  // and add it to the map
  auto res =
    this->queryDB.try_emplace(queryFuncV,
                     toOwned(new QueryMap<ResultType, ArgTs...>(traceQueryName,
                                                                isInputQuery,
                                                                queryFunc)));
  // and return a pointer to the newly inserted map
  QueryMapBase* newBase = res.first->second.get();
  return (QueryMap<ResultType, ArgTs...>*)newBase;
}


template<typename ResultType,
         typename... ArgTs>
QueryMap<ResultType, ArgTs...>*
Context::queryBeginGetMap(
    const ResultType& (*queryFunc)(Context* context, ArgTs...),
    const std::tuple<ArgTs...>& tupleOfArgs,
    const char* traceQueryName,
    bool isInputQuery) {

#if CHPL_QUERY_TIMING_AND_TRACE_ENABLED
  auto stopwatch = makePlainQueryTimingStopwatch(enableQueryTiming);
#endif

  auto ret = getMap(queryFunc, tupleOfArgs, traceQueryName, isInputQuery);

#if CHPL_QUERY_TIMING_AND_TRACE_ENABLED
  if (enableQueryTiming) {
    ret->timings.systemGetMap.update(stopwatch.elapsed());
  }
#endif

  return ret;
}

template<typename ResultType,
         typename... ArgTs>
const QueryMapResult<ResultType, ArgTs...>*
Context::getResult(QueryMap<ResultType, ArgTs...>* queryMap,
                   const std::tuple<ArgTs...>& tupleOfArgs) {

  // Run the constructor QueryMapResult(queryMap, tupleOfArgs)
  // and insert the result into the map if it is not already present.
  auto pair = queryMap->map.emplace(queryMap, tupleOfArgs);
  auto savedElement = &(*pair.first); // pointer to element in map (added/not)
  bool newElementWasAdded = pair.second;

  if (enableDebugTrace) {
    //if (newElementWasAdded)
    //  printf("Added new result %p %s\n", savedElement, queryMap->queryName);
    //else
    //  printf("Found result %p %s\n", savedElement, queryMap->queryName);
  }

  if (newElementWasAdded == false && savedElement->lastChecked == -1) {
    haltForRecursiveQuery(savedElement);
  }

  return savedElement;
}

template<typename ResultType,
         typename... ArgTs>
const QueryMapResult<ResultType, ArgTs...>*
Context::queryBeginGetResult(QueryMap<ResultType, ArgTs...>* queryMap,
                             const std::tuple<ArgTs...>& tupleOfArgs) {

#if CHPL_QUERY_TIMING_AND_TRACE_ENABLED
  auto stopwatch = makePlainQueryTimingStopwatch(enableQueryTiming);
#endif

  auto ret = getResult(queryMap, tupleOfArgs);

#if CHPL_QUERY_TIMING_AND_TRACE_ENABLED
  if (enableQueryTiming) {
    queryMap->timings.systemGetResult.update(stopwatch.elapsed());
  }
#endif

  return ret;
}

template<typename ResultType,
         typename... ArgTs>
bool
Context::queryUseSaved(
       const ResultType& (*queryFunction)(Context* context, ArgTs...),
       const QueryMapResult<ResultType, ArgTs...>* r,
       const char* traceQueryName) {
  const void* queryFuncV = (const void*) queryFunction;
  bool useSaved = queryCanUseSavedResultAndPushIfNot(queryFuncV, r);

  if (enableDebugTrace) {
    if (useSaved) {
      //printf("QUERY END       %s (...) REUSING BASED ON DEPS %p\n",
      //       traceQueryName, r);
      CHPL_ASSERT(r->lastChecked == this->currentRevisionNumber);
    } else {
      //printf("QUERY COMPUTING %s (...) %p\n", traceQueryName, r);
    }
  }

  return useSaved;
}

template<typename ResultType, typename... ArgTs>
const ResultType&
Context::queryGetSaved(const QueryMapResult<ResultType, ArgTs...>* r) {
  this->saveDependencyInParent(r);

  if (errorCollectionStack.size() != 0) {
    // Errors are being collected. Since we're using a cached result, but
    // want to check if errors occurred, copy the already-saved errors
    // into the collector.
    storeErrorsFor(r);
  }

  return r->result;
}

template<typename ResultType,
        typename... ArgTs>
Context::QueryStatus Context::queryStatus(
             const ResultType& (*queryFunction)(Context* context, ArgTs...),
             const std::tuple<ArgTs...>& tupleOfArgs) {
  // Look up the map entry for this query name
  const void* queryFuncV = (const void*) queryFunction;
  // Look up the map entry for this query
  auto search = this->queryDB.find(queryFuncV);
  if (search == this->queryDB.end()) {
    return NOT_CHECKED_NOT_CHANGED;
  }

  // found an entry for this query
  QueryMapBase* base = search->second.get();
  auto queryMap = (QueryMap<ResultType, ArgTs...>*)base;
  QueryMapResult<ResultType, ArgTs...> key(queryMap, tupleOfArgs);
  auto search2 = queryMap->map.find(key);
  if (search2 == queryMap->map.end()) {
    return NOT_CHECKED_NOT_CHANGED;
  }

  if (search2->lastChanged == this->currentRevisionNumber) {
    return CHANGED;
  } else if (search2->lastChecked == this->currentRevisionNumber) {
    return REUSED;
  } else {
    return NOT_CHECKED_NOT_CHANGED;
  }
}

template<typename ResultType,
         typename... ArgTs>
const ResultType&
Context::queryEnd(
              const ResultType& (*queryFunction)(Context* context, ArgTs...),
              QueryMap<ResultType, ArgTs...>* queryMap,
              const QueryMapResult<ResultType, ArgTs...>* r,
              const std::tuple<ArgTs...>& tupleOfArgs,
              ResultType result,
              const char* traceQueryName) {
  // must be in a query to be running one!
  CHPL_ASSERT(queryStack.size() > 0);

  const QueryMapResult<ResultType, ArgTs...>* ret =
    this->updateResultForQueryMapR(queryMap, r, tupleOfArgs,
                                   std::move(result), /* forSetter */ false);

  if (enableDebugTrace
      && std::find(queryTraceIgnoreQueries.begin(),
                   queryTraceIgnoreQueries.end(),
                   traceQueryName) == queryTraceIgnoreQueries.end()) {
    // QUERY END
    setQueryDepthColor(queryTraceDepth, std::cout);
    std::cout << queryTraceDepth;
    clearTerminalColor(std::cout);
    std::cout << "   " << traceQueryName
              << " ";
    if (ret->lastChanged == this->currentRevisionNumber) {
      setTerminalColor(YELLOW, std::cout);
      std::cout << "UPDATED";
    } else {
      setTerminalColor(GREEN, std::cout);
      std::cout << "unchanged";
    }
    setQueryDepthColor(queryTraceDepth, std::cout);
    std::cout << " } ";
    clearTerminalColor(std::cout);
    std::cout << std::endl;
    queryTraceDepth--;
    CHPL_ASSERT(r->lastChecked == this->currentRevisionNumber);
    //for (auto dep : r->dependencies) {
    //  printf("  with dependency %p %s\n", dep, dep->parentQueryMap->queryName);
    //}
  }

  endQueryHandleDependency(ret);

  return ret->result;
}

template<typename ResultType,
         typename... ArgTs>
const QueryMapResult<ResultType, ArgTs...>*
Context::updateResultForQueryMapR(QueryMap<ResultType, ArgTs...>* queryMap,
                                  const QueryMapResult<ResultType, ArgTs...>* r,
                                  const std::tuple<ArgTs...>& tupleOfArgs,
                                  ResultType result,
                                  bool forSetter) {
  CHPL_ASSERT(r != nullptr);

  // If we already have found a result, use that.
  //
  // Call a chpl::update function. That leaves the result in the 1st argument
  // and returns whether or not it needed to change. The 2nd argument contains
  // junk. If we wait until a garbageCollect call to free the junk, we can
  // avoid certain cases where a pointer could be allocated, freed, and then
  // allocated; leading to a sort of ABA issue.
  bool changed = false;
  bool initialResult = (r->lastChanged == -1);
  auto currentRevision = this->currentRevisionNumber;

  // For setter queries, only run the combiner if the last
  // time the query was checked was an earlier revision.
  // If the combiner is skipped, 'changed' is left as false.
  if (forSetter == false || r->lastChecked != currentRevision) {
    chpl::update<ResultType> combiner;
    changed = combiner(r->result, result);
    // now 'r->result' is updated and 'result' is garbage for collection
  }
#ifndef NDEBUG
  // In release mode, the only thing that we care about marking are the
  // unique strings, and those are already marked when they are retrieved
  // with ::get. In debug mode, though, we do checking to make sure the
  // pointers are valid, which means we need to run the below code to make sure
  // that we mark pointers that are not automatically marked when you
  // get them.
  //
  // This initially came up with uast AstNode pointers that were nested
  // within the uAST.
  if(this->currentRevisionNumber == this->lastPrepareToGCRevisionNumber) {
    // Maybe this is a good idea, but maybe not. Make sure query results
    // are marked after being recomputed.
    // Maybe we can avoid doing this if assertions are off.
    r->markUniqueStringsInResult(this);
  }
#endif

  // save old result when appropriate
  // no need to save old result 1st time running this query
  // no need to save new result when old one is used due to no changes
  if (initialResult || changed==false) {
    // no need to save old result
  } else {
    queryMap->oldResults.push_back(std::move(result));
  }

  r->emittedErrors = errorCollectionStack.empty();
  r->lastChecked = currentRevision;
  if (changed || initialResult) {
    r->lastChanged  = currentRevision;
  }
  return r;
}

template<typename ResultType,
         typename... ArgTs>
const QueryMapResult<ResultType, ArgTs...>*
Context::updateResultForQueryMap(QueryMap<ResultType, ArgTs...>* queryMap,
                                 const std::tuple<ArgTs...>& tupleOfArgs,
                                 ResultType result,
                                 bool forSetter) {
  // Look up the current entry
  const QueryMapResult<ResultType, ArgTs...>* r
    = getResult(queryMap, tupleOfArgs);

  // Run the version of the function accepting the map and result entry
  return updateResultForQueryMapR(queryMap, r, tupleOfArgs,
                                  std::move(result), forSetter);
}

template<typename ResultType,
         typename... ArgTs>
const QueryMapResult<ResultType, ArgTs...>*
Context::updateResultForQuery(
                const ResultType& (*queryFunction)(Context* context, ArgTs...),
                const std::tuple<ArgTs...>& tupleOfArgs,
                ResultType result,
                const char* traceQueryName,
                bool isInputQuery,
                bool forSetter) {
  // Look up the map entry for this query name
  QueryMap<ResultType, ArgTs...>* queryMap =
    getMap(queryFunction, tupleOfArgs, traceQueryName, isInputQuery);

  // Run the version of the function accepting the map
  return updateResultForQueryMap(queryMap, tupleOfArgs,
                                 std::move(result), forSetter);
}

template<typename ResultType,
         typename... ArgTs>
bool
Context::hasCurrentResultForQuery(
     const ResultType& (*queryFunction)(Context* context, ArgTs...),
     const std::tuple<ArgTs...>& tupleOfArgs) {

  // Look up the map entry for this query name
  const void* queryFuncV = (const void*) queryFunction;
  // Look up the map entry for this query
  auto search = this->queryDB.find(queryFuncV);
  if (search == this->queryDB.end()) {
    return false;
  }

  // found an entry for this query
  QueryMapBase* base = search->second.get();
  auto queryMap = (QueryMap<ResultType, ArgTs...>*)base;
  QueryMapResult<ResultType, ArgTs...> key(queryMap, tupleOfArgs);
  auto search2 = queryMap->map.find(key);
  if (search2 == queryMap->map.end()) {
    return false;
  }

  return queryCanUseSavedResult(queryFuncV, &(*search2));
}

template<typename ResultType,
         typename... ArgTs>
bool
Context::isQueryRunning(
     const ResultType& (*queryFunction)(Context* context, ArgTs...),
     const std::tuple<ArgTs...>& tupleOfArgs) {
  // Look up the map entry for this query name
  const void* queryFuncV = (const void*) queryFunction;
  // Look up the map entry for this query
  auto search = this->queryDB.find(queryFuncV);
  if (search == this->queryDB.end()) {
    return false;
  }

  // found an entry for this query
  QueryMapBase* base = search->second.get();
  auto queryMap = (QueryMap<ResultType, ArgTs...>*)base;
  QueryMapResult<ResultType, ArgTs...> key(queryMap, tupleOfArgs);
  auto search2 = queryMap->map.find(key);
  if (search2 == queryMap->map.end()) {
    return false;
  }

  return search2->lastChecked == -1;
}


template<typename ResultType,
         typename... ArgTs>
void
Context::querySetterUpdateResult(
    const ResultType& (*queryFunction)(Context* context, ArgTs...),
    const std::tuple<ArgTs...>& tupleOfArgs,
    ResultType result,
    const char* traceQueryName,
    bool isInputQuery) {
  updateResultForQuery(queryFunction, tupleOfArgs, std::move(result),
                       traceQueryName, isInputQuery, /* forSetter */ true);
}

} // end namespace chpl

#define QUERY_BEGIN_INNER(isInput, func, context, ...) \
  auto* BEGIN_QUERY_FUNCTION = func; \
  Context* BEGIN_QUERY_CONTEXT = context; \
  const char* BEGIN_QUERY_FUNC_NAME = #func; \
  CHPL_ASSERT(0 == strcmp(BEGIN_QUERY_FUNC_NAME, __func__)); \
  auto BEGIN_QUERY_ARGS = std::make_tuple(__VA_ARGS__); \
  auto BEGIN_QUERY_MAP = context->queryBeginGetMap(BEGIN_QUERY_FUNCTION, \
                                                   BEGIN_QUERY_ARGS, \
                                                   BEGIN_QUERY_FUNC_NAME, \
                                                   isInput); \
  auto BEGIN_QUERY_FOUND = context->queryBeginGetResult(BEGIN_QUERY_MAP, \
                                                        BEGIN_QUERY_ARGS); \

#define QUERY_USE_SAVED() \
  (BEGIN_QUERY_CONTEXT->queryUseSaved(BEGIN_QUERY_FUNCTION, \
                                      BEGIN_QUERY_FOUND, \
                                      BEGIN_QUERY_FUNC_NAME))

#define QUERY_GET_SAVED() \
  (BEGIN_QUERY_CONTEXT->queryGetSaved(BEGIN_QUERY_FOUND))

#if CHPL_QUERY_TIMING_AND_TRACE_ENABLED

#define QUERY_BEGIN_TIMING() \
  context->queryBeginTrace(BEGIN_QUERY_FUNC_NAME, BEGIN_QUERY_ARGS); \
  auto QUERY_STOPWATCH = context->makeQueryTimingStopwatch(BEGIN_QUERY_MAP)

#else

#define QUERY_BEGIN_TIMING()

#endif


/**
  Use QUERY_BEGIN at the start of the implementation of a particular query.
  It checks to see if an earlier result can be used and in that event returns
  it.

  Pass the name of the enclosing function as func, context is the
  class Context, and then pass any arguments to the query.
 */
#define QUERY_BEGIN(func, context, ...) \
  QUERY_BEGIN_INNER(false, func, context, __VA_ARGS__); \
  if (QUERY_USE_SAVED()) { \
    return QUERY_GET_SAVED(); \
  } \
  auto QUERY_RECOMPUTATION_MARKER = context->markRecomputing(false); \
  QUERY_BEGIN_TIMING();

/**
  QUERY_BEGIN_INPUT is like QUERY_BEGIN but should be used
  for input queries.
 */
#define QUERY_BEGIN_INPUT(func, context, ...) \
  QUERY_BEGIN_INNER(true, func, context, __VA_ARGS__) \
  if (QUERY_USE_SAVED()) { \
    return QUERY_GET_SAVED(); \
  } \
  auto QUERY_RECOMPUTATION_MARKER = context->markRecomputing(false); \
  QUERY_BEGIN_TIMING();

/**
  Write

    return QUERY_END(result);

  at the end of the implementation for a particular query. The result
  will be `std::move`d into the query.

  Runs chpl::update to update the result stored in the map with the passed one.
  Updates the query's last updated and last computed revisions as needed.
 */
#define QUERY_END(result) \
  /* Recomputation marker isn't out of scope yet, but we want queryEnd to get old state */ \
  (QUERY_RECOMPUTATION_MARKER.restore(), \
  /* must not use BEGIN_QUERY_SEARCH1 (iterator could be invalidated) */ \
   BEGIN_QUERY_CONTEXT->queryEnd(BEGIN_QUERY_FUNCTION, \
                                 BEGIN_QUERY_MAP, \
                                 BEGIN_QUERY_FOUND, \
                                 BEGIN_QUERY_ARGS, \
                                 std::move(result), \
                                 BEGIN_QUERY_FUNC_NAME))


/**
  Use QUERY_STORE_RESULT to implement a setter for a non-input query.
  Arguments are:
   * the query function to update
   * the context
   * the new result
   * any number of query arguments

  The result will be `std::move`d into the query.
  If called multiple times within the same revision, only the first
  stored result in that revision will be saved.
 */
#define QUERY_STORE_RESULT(func, context, result, ...) \
  context->querySetterUpdateResult(func, \
                                   std::make_tuple(__VA_ARGS__), \
                                   std::move(result), \
                                   #func, \
                                   false)


/**
  Use QUERY_STORE_INPUT_RESULT to implement a setter for an input query.
  This is especially useful for input queries (to e.g. set the file contents).
  Arguments are:
   * the query function to update
   * the context
   * the new result
   * any number of query arguments

  The result will be `std::move`d into the query.
  If called multiple times within the same revision, only the first
  stored result in that revision will be saved.
 */
#define QUERY_STORE_INPUT_RESULT(func, context, result, ...) \
  context->querySetterUpdateResult(func, \
                                   std::make_tuple(__VA_ARGS__), \
                                   std::move(result), \
                                   #func, \
                                   true)

/// \endcond

#endif
