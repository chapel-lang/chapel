#ifndef CHPL_QUERIES_QUERYIMPL_H
#define CHPL_QUERIES_QUERYIMPL_H

#include "chpl/Queries/Context.h"
#include "chpl/Queries/ContextDetail.h"

/**
  This file should be included by .cpp files implementing queries.
 */

namespace chpl {

using namespace chpl::querydetail;

template<typename ResultType, typename... ArgTs>
QueryMap<ResultType,ArgTs...>* Context::queryBeginGetMap(UniqueString queryName, const std::tuple<ArgTs...>& tupleOfArgs) {
  // Look up the map entry for this query name
  auto search = this->queryDB.find(queryName);
  if (search != this->queryDB.end()) {
    // found an entry for this query name
    QueryMapBase* base = search->second.get();
    // return the inner map
    return (QueryMap<ResultType,ArgTs...>*)base;
  }
  // Otherwise, create the QueryMap entry for this query name
  // and add it to the map
  auto iter =
    this->queryDB.emplace_hint(
      search,
      std::make_pair(queryName,
                     toOwned(new QueryMap<ResultType,ArgTs...>(queryName))));
  // and return a pointer to the newly inserted map
  QueryMapBase* newBase = iter->second.get();
  return (QueryMap<ResultType,ArgTs...>*)newBase;
}


bool Context::queryCanUseSavedResultAndPushIfNot(UniqueString queryName, QueryMapResultBase* resultEntry) {
  bool ret = this->queryCanUseSavedResult(resultEntry);
  if (ret == false) {
    // since the result cannot be used, the query will be evaluated
    // so push something to queryDeps
    queryDeps.push_back(QueryDepsEntry(queryName));
  }
  return ret;
}

template<typename ResultType>
const ResultType& Context::queryGetSavedResult(QueryMapResult<ResultType>* resultEntry) {
  this->saveDependenciesAndErrorsInParent(resultEntry);
  return resultEntry->result;
}

template<typename ResultType, typename... ArgTs>
ResultType& Context::queryEnd(UniqueString queryName,
                    ResultType result,
                    const std::tuple<ArgTs...>& tupleOfArgs,
                    QueryMap<ResultType,ArgTs...>* queryMap) {

  // At this point, queryDeps is the dependency vector for this query
  assert(queryDeps.size() > 0);
  assert(queryDeps.back().queryName == queryName);

  auto search = queryMap->map.find(tupleOfArgs);
  if (search != queryMap->map.end()) {
    QueryMapResult<ResultType>* oldResult = &(search->second);
    endQueryHandleDependency(oldResult);

    std::equal_to<ResultType> equal;
    if (equal(oldResult->result, result)) {
      // no change to the result
      // so update "last checked" but don't change "last changed".
      oldResult->lastComputed = this->currentRevisionNumber;
      return oldResult->result;
    } else {
      // change to the result
      // so update "last changed" and "last checked"
      // and store the new result in the map
      oldResult->lastComputed = this->currentRevisionNumber;
      oldResult->lastChanged = this->currentRevisionNumber;
      oldResult->result.swap(result);
      return oldResult->result;
    }
  }
  // result not found in map
  // so update "last changed" and "last checked"
  // and store the new result in the map
  auto iter =
    queryMap->map.emplace_hint(
      search,
      std::make_pair(tupleOfArgs,
                     QueryMapResult<ResultType>(QueryDependencyVec(),
                                                this->currentRevisionNumber,
                                                this->currentRevisionNumber,
                                                this->currentRevisionNumber,
                                                QueryErrorVec(),
                                                std::move(result))));
  // update the dependencies with the result we have
  QueryMapResult<ResultType>* newResult = &(iter->second);
  endQueryHandleDependency(newResult);
  return newResult->result;
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

 */
#define QUERY_BEGIN(context, ResultType, ...) \
  const char* BEGIN_QUERY_FILE = __FILE__; \
  int BEGIN_QUERY_LINE = __LINE__; \
  const char* BEGIN_QUERY_FUNC = __func__; \
  const char* BEGIN_QUERY_FILE_LINE = __FILE__ STRINGIZE_LINE(__LINE__); \
  Context* BEGIN_QUERY_CONTEXT = context; \
  UniqueString BEGIN_QUERY_NAME = UniqueString::build(context, BEGIN_QUERY_FILE_LINE); \
  auto BEGIN_QUERY_ARGS = std::make_tuple(__VA_ARGS__); \
  auto BEGIN_QUERY_MAP = \
    context->queryBeginGetMap<ResultType>(BEGIN_QUERY_NAME, \
                                          BEGIN_QUERY_ARGS); \
  auto BEGIN_QUERY_SEARCH1 = BEGIN_QUERY_MAP->map.find(BEGIN_QUERY_ARGS);


#define QUERY_USE_SAVED() \
  (BEGIN_QUERY_SEARCH1 != BEGIN_QUERY_MAP->map.end() && \
   BEGIN_QUERY_CONTEXT->queryCanUseSavedResultAndPushIfNot(BEGIN_QUERY_NAME, &(BEGIN_QUERY_SEARCH1->second)))

#define QUERY_GET_SAVED() \
  (BEGIN_QUERY_CONTEXT->queryGetSavedResult(&(BEGIN_QUERY_SEARCH1->second)))

#define QUERY_ERROR(error) \
  BEGIN_QUERY_CONTEXT->queryNoteError(error)

#define QUERY_END(result) \
  /* must not use BEGIN_QUERY_SEARCH1 (iterator could be invalidated) */ \
  (BEGIN_QUERY_CONTEXT->queryEnd(BEGIN_QUERY_NAME, result, BEGIN_QUERY_ARGS, BEGIN_QUERY_MAP))

#endif
