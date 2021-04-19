#ifndef CHPL_QUERIES_QUERYIMPL_H
#define CHPL_QUERIES_QUERYIMPL_H

#include "chpl/Queries/Context.h"
#include "chpl/Queries/ContextDetail.h"

#ifndef QUERY_MAP_USE_PRETTYFUNC
  #if defined(__clang__)
    #define QUERY_MAP_USE_PRETTYFUNC
  #elif defined(__GNUC__)
    #define QUERY_MAP_USE_PRETTYFUNC
  #endif
#endif

/**
  This file should be included by .cpp files implementing queries.
 */

/// \cond DO_NOT_DOCUMENT
namespace chpl {

using namespace chpl::querydetail;

template<typename... ArgTs>
void Context::queryTraceBegin(UniqueString queryName, const char* func,
                     const std::tuple<ArgTs...>& tupleOfArg) {
  printf("QUERY BEGIN     %s (", func);
  queryArgsPrint(tupleOfArg);
  printf(")\n");
}

template<typename... ArgTs>
void Context::queryTraceEnd(UniqueString queryName, const char* func,
                   const std::tuple<ArgTs...>& tupleOfArg,
                   bool changed) {
  printf("QUERY END       %s (", func);
  queryArgsPrint(tupleOfArg);
  printf(") %s\n", changed?"UPDATED":"NO CHANGE");
}

template<typename ResultType, typename... ArgTs>
QueryMap<ResultType,ArgTs...>* Context::queryGetMap(UniqueString queryName, const std::tuple<ArgTs...>& tupleOfArgs) {

  const char* prettyFunc = "";
#ifdef QUERY_MAP_USE_PRETTYFUNC
    prettyFunc = __PRETTY_FUNCTION__;
#endif

  // Look up the map entry for this query name
  auto search = this->queryDB.find(queryName);
  if (search != this->queryDB.end()) {
    // found an entry for this query name
    QueryMapBase* base = search->second.get();
    // check that the arg/result types match
    assert(0 == strcmp(base->prettyFunc, prettyFunc));
    // return the inner map
    return (QueryMap<ResultType,ArgTs...>*)base;
  }

  // Otherwise, create the QueryMap entry for this query name
  // and add it to the map
  auto iter =
    this->queryDB.emplace_hint(
      search,
      std::make_pair(queryName,
                     toOwned(new QueryMap<ResultType,ArgTs...>(queryName,
                                                               prettyFunc))));
  // and return a pointer to the newly inserted map
  QueryMapBase* newBase = iter->second.get();
  return (QueryMap<ResultType,ArgTs...>*)newBase;
}

template<typename ResultType, typename... ArgTs>
QueryMapResult<ResultType>*
Context::updateResultForQuery(const std::tuple<ArgTs...>& tupleOfArgs,
                              ResultType result,
                              bool& changedOut,
                              QueryMap<ResultType,ArgTs...>* queryMap) {
  // Set the map element
  auto search = queryMap->map.find(tupleOfArgs);
  if (search != queryMap->map.end()) {
   QueryMapResult<ResultType>* storedResult = &(search->second);

   // Call a chpl::combine function. That leaves the result in the 1st argument
   // and returns whether or not it needed to change. The 2nd argument contains
   // junk. If we wait until a garbageCollect call to free the junk, we can
   // avoid certain cases where a pointer could be allocated, freed, and then
   // allocated; leading to a sort of ABA issue.
   chpl::combine<ResultType> combiner;
   bool match = combiner(storedResult->result, result);
   // now storedResult is the new result
   queryMap->oldResults.push_back(std::move(result));
   if (match) {
     changedOut = false;
     storedResult->lastComputed = this->currentRevisionNumber;
     return storedResult;
   } else {
     changedOut = true;
     auto currentRevision = this->currentRevisionNumber;
     storedResult->lastComputed = currentRevision;
     storedResult->lastChanged  = currentRevision;
     return storedResult;
   }
  } else {
    auto iter =
      queryMap->map.emplace_hint(
        search,
        std::make_pair(tupleOfArgs,
                       QueryMapResult<ResultType>(QueryDependencyVec(),
                                                  0,
                                                  this->currentRevisionNumber,
                                                  this->currentRevisionNumber,
                                                  QueryErrorVec(),
                                                  std::move(result))));
    QueryMapResult<ResultType>* newResult = &(iter->second);
    changedOut = true;
    return newResult;
  }
}

template<typename ResultType, typename... ArgTs>
QueryMapResult<ResultType>*
Context::updateResultForQuery(const std::tuple<ArgTs...>& tupleOfArgs,
                              ResultType result,
                              bool& changedOut,
                              UniqueString queryName) {
  // Look up the map entry for this query name
  QueryMap<ResultType,ArgTs...>* queryMap =
    queryGetMap<ResultType>(queryName, tupleOfArgs);
  // Run the version of the function accepting the map
  return
    updateResultForQuery(tupleOfArgs, std::move(result), changedOut, queryMap);
}

template<typename ResultType>
const ResultType& Context::queryGetSavedResult(QueryMapResult<ResultType>* resultEntry) {
  this->saveDependenciesAndErrorsInParent(resultEntry);
  return resultEntry->result;
}

template<typename ResultType, typename... ArgTs>
ResultType& Context::queryEnd(UniqueString queryName, const char* func,
                    ResultType result,
                    const std::tuple<ArgTs...>& tupleOfArgs,
                    QueryMap<ResultType,ArgTs...>* queryMap) {

  // At this point, queryDeps is the dependency vector for this query
  assert(queryDeps.size() > 0);
  assert(queryDeps.back().queryName == queryName);

  bool changed = false;
  QueryMapResult<ResultType>* ret =
    this->updateResultForQuery(tupleOfArgs, std::move(result),
                               changed, queryMap);

  queryTraceEnd(queryName, func, tupleOfArgs, changed);

  endQueryHandleDependency(ret);

  auto currentRevision = this->currentRevisionNumber;
  if (changed == false) {
    ret->lastComputed = currentRevision;
  } else {
    ret->lastComputed = currentRevision;
    ret->lastChanged  = currentRevision;
  }
  return ret->result;
}

} // end namespace chpl

#define STRINGIZE_LINE_DETAIL(x) #x
#define STRINGIZE_LINE(x) STRINGIZE_LINE_DETAIL(x)

/**
  The following macros should be called within a query function in
  a particular stylized manner:

    const MyResultType& myQueryFunction(Context* context, myKey1, myKey2) {
      QUERY_BEGIN(context, MyResultType, myKey1, myKey2)
      if (QUERY_USE_SAVED()) {
        return QUERY_GET_SAVED();
      }
      // do steps to compute the result
      MyResultType result = ...;
      // if an error is encountered, it can be saved with QUERY_ERROR(error)

      return QUERY_END(result);
    }

  The above is appropriate for value types  (std::string, int, etc).
  For an object that should be managed as owned within the context,
  use this pattern:

    const MyClassType* myQueryFunction(Context* context, myKey1, myKey2) {
      QUERY_BEGIN(context, owned<MyClassType>, myKey1, myKey2)
      if (QUERY_USE_SAVED()) {
        return QUERY_GET_SAVED().get();
      }
      // do steps to compute the result
      owned<MyClassType> result = ...;
      // if an error is encountered, it can be saved with QUERY_ERROR(error)

      return QUERY_END(result);
    }

  QUERY_BEGIN_NAMED can be used to provide a name for the query.
  That is needed in order to allow Context to have a setter method
  for that query.
 */

#define QUERY_BEGIN_NAMED(context, ResultType, queryName, ...) \
  Context* BEGIN_QUERY_CONTEXT = context; \
  const char* BEGIN_QUERY_FUNC = __func__; \
  UniqueString BEGIN_QUERY_NAME = UniqueString::build(context, queryName); \
  auto BEGIN_QUERY_ARGS = std::make_tuple(__VA_ARGS__); \
  context->queryTraceBegin(BEGIN_QUERY_NAME, BEGIN_QUERY_FUNC, \
                           BEGIN_QUERY_ARGS); \
  auto BEGIN_QUERY_MAP = \
    context->queryGetMap<ResultType>(BEGIN_QUERY_NAME, \
                                     BEGIN_QUERY_ARGS); \
  auto BEGIN_QUERY_SEARCH1 = BEGIN_QUERY_MAP->map.find(BEGIN_QUERY_ARGS); \
  QueryMapResultBase* BEGIN_QUERY_FOUND1 = nullptr; \
  if (BEGIN_QUERY_SEARCH1 != BEGIN_QUERY_MAP->map.end()) { \
    BEGIN_QUERY_FOUND1 = &(BEGIN_QUERY_SEARCH1->second); \
  }

#define QUERY_BEGIN(context, ResultType, ...) \
  const char* BEGIN_QUERY_FILE = __FILE__; \
  int BEGIN_QUERY_LINE = __LINE__; \
  const char* BEGIN_QUERY_FILE_LINE = __FILE__ ":" STRINGIZE_LINE(__LINE__); \
  QUERY_BEGIN_NAMED(context, ResultType, BEGIN_QUERY_FILE_LINE, __VA_ARGS__);

#define QUERY_USE_SAVED() \
  (BEGIN_QUERY_CONTEXT->queryCanUseSavedResultAndPushIfNot(BEGIN_QUERY_NAME, BEGIN_QUERY_FUNC, BEGIN_QUERY_FOUND1))

#define QUERY_GET_SAVED() \
  (BEGIN_QUERY_CONTEXT->queryGetSavedResult(&(BEGIN_QUERY_SEARCH1->second)))

#define QUERY_ERROR(error) \
  BEGIN_QUERY_CONTEXT->queryNoteError(error)

#define QUERY_END(result) \
  /* must not use BEGIN_QUERY_SEARCH1 (iterator could be invalidated) */ \
  (BEGIN_QUERY_CONTEXT->queryEnd(BEGIN_QUERY_NAME, BEGIN_QUERY_FUNC, result, BEGIN_QUERY_ARGS, BEGIN_QUERY_MAP))

/// \endcond

#endif
