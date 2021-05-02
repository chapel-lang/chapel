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

#ifndef CHPL_QUERIES_QUERYIMPL_H
#define CHPL_QUERIES_QUERYIMPL_H

#include "chpl/Queries/Context.h"
#include "chpl/Queries/ContextDetail.h"

/**
  This file should be included by .cpp files implementing queries.
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
void QueryMapResult<ResultType,ArgTs...>::recompute(Context* context) const
{
  QueryMap<ResultType, ArgTs...>* queryMap =
    (QueryMap<ResultType, ArgTs...>*) this->parentQueryMap;

  const std::tuple<ArgTs...>& args = this->tupleOfArgs;
  helpRecompute(context, queryMap, args, std::index_sequence_for<ArgTs...> {});
}

template<typename... ArgTs>
void Context::queryBeginTrace(const char* traceQueryName,
                              const std::tuple<ArgTs...>& tupleOfArg) {
  printf("QUERY BEGIN     %s (", traceQueryName);
  queryArgsPrint(tupleOfArg);
  printf(")\n");
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
  auto iter =
    this->queryDB.emplace_hint(
      search,
      std::make_pair(queryFuncV,
                     toOwned(new QueryMap<ResultType, ArgTs...>(traceQueryName,
                                                                isInputQuery,
                                                                queryFunc))));
  // and return a pointer to the newly inserted map
  QueryMapBase* newBase = iter->second.get();
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

  return getMap(queryFunc, tupleOfArgs, traceQueryName, isInputQuery);
}

template<typename ResultType,
         typename... ArgTs>
const QueryMapResult<ResultType, ArgTs...>*
Context::getResult(QueryMap<ResultType, ArgTs...>* queryMap,
          const std::tuple<ArgTs...>& tupleOfArgs) {

  // The below call default-initializes the result in QueryMapResult
  // in order to construct something we can use as the key in the find call.
  // This default-initialization could be avoided with some hacks to
  // the current implementation. However, C++20 enables a principled
  // solution to the problem using is_transparent and transparent_key_equal.
  auto search = queryMap->map.find(
                     QueryMapResult<ResultType, ArgTs...>(tupleOfArgs));
  const QueryMapResult<ResultType, ArgTs...>* ret = nullptr;
  if (search != queryMap->map.end()) {
    ret = &(*search);
  }
  return ret;
}

template<typename ResultType,
         typename... ArgTs>
const QueryMapResult<ResultType, ArgTs...>*
Context::queryBeginGetResult(QueryMap<ResultType, ArgTs...>* queryMap,
                             const std::tuple<ArgTs...>& tupleOfArgs) {
  return getResult(queryMap, tupleOfArgs);
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
  if (useSaved) {
    printf("QUERY END       %s (...) REUSING BASED ON DEPS\n", traceQueryName);
  } else {
    printf("QUERY COMPUTING %s (...)\n", traceQueryName);
  }
  return useSaved;
}

template<typename ResultType, typename... ArgTs>
const ResultType&
Context::queryGetSaved(const QueryMapResult<ResultType, ArgTs...>* r) {
  this->saveDependenciesInParent(r);
  return r->result;
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

  // At this point, queryDeps is the dependency vector for this query
  assert(queryDeps.size() > 0);
  assert(queryDeps.back().queryFunction == queryFunction);

  const QueryMapResult<ResultType, ArgTs...>* ret =
    this->updateResultForQueryMapR(queryMap, r, tupleOfArgs, std::move(result));

  {
    bool changed = ret->lastChanged == this->currentRevisionNumber;
    printf("QUERY END       %s (", traceQueryName);
    queryArgsPrint(tupleOfArgs);
    printf(") %s\n", changed?"UPDATED":"NO CHANGE");
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
                                  ResultType result) {
  if (r != nullptr) {
    // If we already have found a result, use that.
    //
    // Call a chpl::update function. That leaves the result in the 1st argument
    // and returns whether or not it needed to change. The 2nd argument contains
    // junk. If we wait until a garbageCollect call to free the junk, we can
    // avoid certain cases where a pointer could be allocated, freed, and then
    // allocated; leading to a sort of ABA issue.
    chpl::update<ResultType> combiner;
    bool changed = combiner(r->result, result);
    // now 'r->result' is updated and 'result' is garbage for collection
    queryMap->oldResults.push_back(std::move(result));
    // TODO: also check if the errors have changed
    auto currentRevision = this->currentRevisionNumber;
    r->lastChecked = currentRevision;
    if (changed) {
      r->lastChanged  = currentRevision;
    }
    return r;

  } else {
    // Otherwise, store a new result entry in the map.
    auto currentRevision = this->currentRevisionNumber;
    auto iv = queryMap->map.insert(
                    QueryMapResult<ResultType, ArgTs...>(currentRevision,
                                                         currentRevision,
                                                         queryMap,
                                                         tupleOfArgs,
                                                         std::move(result)));
    const QueryMapResult<ResultType, ArgTs...>* newResult = &(*iv.first);
    assert(iv.second && "expected insertion to take place");
    return const_cast<QueryMapResult<ResultType, ArgTs...>*>(newResult);
  }
}

template<typename ResultType,
         typename... ArgTs>
const QueryMapResult<ResultType, ArgTs...>*
Context::updateResultForQueryMap(QueryMap<ResultType, ArgTs...>* queryMap,
                                 const std::tuple<ArgTs...>& tupleOfArgs,
                                 ResultType result) {
  // Look up the current entry
  const QueryMapResult<ResultType, ArgTs...>* r
    = getResult(queryMap, tupleOfArgs);
 
  // Run the version of the function accepting the map and result entry
  return updateResultForQueryMapR(queryMap, r, tupleOfArgs, std::move(result));
}

template<typename ResultType,
         typename... ArgTs>
const QueryMapResult<ResultType, ArgTs...>*
Context::updateResultForQuery(
                const ResultType& (*queryFunction)(Context* context, ArgTs...),
                const std::tuple<ArgTs...>& tupleOfArgs,
                ResultType result,
                const char* traceQueryName,
                bool isInputQuery) {
  // Look up the map entry for this query name
  QueryMap<ResultType, ArgTs...>* queryMap =
    getMap(queryFunction, tupleOfArgs, traceQueryName, isInputQuery);

  // Run the version of the function accepting the map
  return updateResultForQueryMap(queryMap, tupleOfArgs, std::move(result));
}

/*
template<typename ResultType,
         typename... ArgTs>
void
Context::queryRecomputedUpdateResult(QueryMap<ResultType, ArgTs...>* queryMap,
                                     const std::tuple<ArgTs...>& tupleOfArgs,
                                     ResultType result) {
  updateResultForQueryMap(queryMap, tupleOfArgs, std::move(result));
}*/

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
                       traceQueryName, isInputQuery);
}

} // end namespace chpl

#define QUERY_BEGIN_INNER(isInput, func, context, ...) \
  auto* BEGIN_QUERY_FUNCTION = func; \
  Context* BEGIN_QUERY_CONTEXT = context; \
  const char* BEGIN_QUERY_FUNC_NAME = #func; \
  const char* BEGIN_QUERY_IN_FUNC_NAME = __func__; \
  assert(0 == strcmp(BEGIN_QUERY_FUNC_NAME, BEGIN_QUERY_IN_FUNC_NAME)); \
  auto BEGIN_QUERY_ARGS = std::make_tuple(__VA_ARGS__); \
  context->queryBeginTrace(BEGIN_QUERY_FUNC_NAME, BEGIN_QUERY_ARGS); \
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

#define QUERY_BEGIN(func, context, ...) \
  QUERY_BEGIN_INNER(false, func, context, __VA_ARGS__); \
  if (QUERY_USE_SAVED()) { \
    return QUERY_GET_SAVED(); \
  }

#define QUERY_BEGIN_INPUT(func, context, ...) \
  QUERY_BEGIN_INNER(true, func, context, __VA_ARGS__) \
  if (QUERY_USE_SAVED()) { \
    return QUERY_GET_SAVED(); \
  }

#define QUERY_ERROR(error) \
  BEGIN_QUERY_CONTEXT->queryNoteError(error)

#define QUERY_DEPENDS_INPUT() \
  BEGIN_QUERY_CONTEXT->queryNoteInputDependency()

#define QUERY_END(result) \
  /* must not use BEGIN_QUERY_SEARCH1 (iterator could be invalidated) */ \
  (BEGIN_QUERY_CONTEXT->queryEnd(BEGIN_QUERY_FUNCTION, \
                                 BEGIN_QUERY_MAP, \
                                 BEGIN_QUERY_FOUND, \
                                 BEGIN_QUERY_ARGS, \
                                 std::move(result), \
                                 BEGIN_QUERY_FUNC_NAME))


/// \endcond

#endif
