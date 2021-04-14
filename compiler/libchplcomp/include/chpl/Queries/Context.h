#ifndef CHPL_QUERIES_CONTEXT_H
#define CHPL_QUERIES_CONTEXT_H

#include "chpl/AST/UniqueString.h"
#include "chpl/Queries/ContextDetail.h"
#include "chpl/Util/memory.h"

#include <unordered_map>

namespace chpl {

// Allow ast::UniqueString to be just called UniqueString
// TODO: Should it even be ast::UniqueString at all?
using chpl::ast::UniqueString;

/**
  This class stores the compilation-wide context.
  In particular it handles unique'd strings.
 */
class Context {
 private:
  // map that supports uniqueCString / UniqueString
  std::unordered_map<const char*, const char*, detail::UniqueStrHash, detail::UniqueStrEqual> uniqueStringsTable;

  // map from a query name to appropriate QueryMap object
  std::unordered_map<UniqueString, owned<chpl::detail::QueryMapBase>> queryDB;

  // this is used to compute the dependencies
  std::vector<std::pair<UniqueString,chpl::detail::DependencyVec>> queryDeps;

  chpl::detail::RevisionNumber currentRevisionNumber;

  const char* getOrCreateUniqueString(const char* s);
  Context();

  // Future Work: support marking used strings and garbage collecting the rest
  // Could store an atomic uint_8 just after the string for the mark.

  // Future Work: make the context thread-safe

  // Future Work: allow moving some AST to a different context
  //              (or, at least, that can handle the unique strings)

 public:
  /**
    Create a new AST Context.
   */
  static owned<Context> build();
  ~Context();

  /**
    Get or create a unique string for a NULL-terminated C string
    and return it as a C string. If the passed string is NULL,
    this function will return an empty string.

    Strings returned by this function will always be aligned to 2 bytes.

    The function `UniqueString::build` returns such a string
    with a wrapper type. It should be preferred for type safety
    and to reduce redundant checks.
   */
  const char* uniqueCString(const char* s);


  template<typename ResultType, typename ArgTupleType>
  chpl::detail::QueryMap<ResultType,ArgTupleType>*
    beginQueryGetMap(UniqueString queryName, const ArgTupleType& tupleOfArgs) {
    // Look up the map entry for this query name
    auto search = this->queryDB.find(queryName);
    if (search != this->queryDB.end()) {
      // found an entry for this query name
      chpl::detail::QueryMapBase* base = search->second.get();
      // return the inner map
      return (chpl::detail::QueryMap<ResultType,ArgTupleType>*)base;
    }
    // Otherwise, create the QueryMap entry for this query name
    chpl::detail::QueryMap<ResultType,ArgTupleType>* newMap =
      new chpl::detail::QueryMap<ResultType,ArgTupleType>();
    // and add it to the map
    this->queryDB.insert(search, {queryName, toOwned(newMap)});
    // and return it
    return newMap;
  }
  bool beginQueryCanUseResult(chpl::detail::QueryMapResultBase* result) {
    // If we already checked this query in this revision,
    // we can use this result
    if (result->lastComputed == this->currentRevisionNumber) {
      return true;
    }

    // Otherwise, check the dependencies. Have any of them
    // changed since the last revision in which we computed this?
    for (chpl::detail::QueryMapResultBase* dependency : result->dependencies) {
      if (dependency->lastChanged > result->lastComputed) {
        return false;
      }
    }

    // Otherwise, all of the inputs have not changed
    // since this result was last computed.
    return true;
  }
  bool beginQueryCanUseResultDeps(chpl::detail::QueryMapResultBase* result) {
    bool retval = this->beginQueryCanUseResult(result);
    if (retval) {
      // we haven't pushed the query beginning yet, so the
      // parent query is at queryDeps.back()
      if (queryDeps.size() > 0) {
        queryDeps.back().second.push_back(result);
      }
    }
    return retval;
  }

  void beginQueryComputation(UniqueString queryName) {
    queryDeps.push_back(std::make_pair(queryName, chpl::detail::DependencyVec()));
  }

  void endQueryHandleDependency(chpl::detail::QueryMapResultBase* result) {
    // queryDeps.back() is the dependency vector for this query
    // which was just run. So, replace result->dependencies with it.
    result->dependencies = queryDeps.back().second;
    queryDeps.pop_back();
    // additionally, we've run a query and there might well be
    // a parent query. In that event, we should update the dependency
    // vector for the parent query.
    if (queryDeps.size() > 0) {
      queryDeps.back().second.push_back(result);
    }
  }

  template<typename ArgTupleType, typename ResultType>
  ResultType endQuery(UniqueString queryName,
                      const ArgTupleType& tupleOfArgs,
                      ResultType result,
                      chpl::detail::QueryMap<ResultType,ArgTupleType>* queryMap) {

    // At this point, queryDeps is the dependency vector for this query
    assert(queryDeps.size() > 0);
    assert(queryDeps.back().first == queryName);

    auto search = queryMap->find(tupleOfArgs);
    if (search != queryMap->end()) {
      chpl::detail::QueryMapResult<ResultType>* oldResult = search->second->get();
      endQueryHandleDependency(oldResult);
      if (oldResult->equals(result)) {
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
        oldResult->result = result;
        return oldResult->result;
      }
    }
    // result not found in map
    // so update "last changed" and "last checked"
    // and store the new result in the map
    auto newResult = new chpl::detail::QueryMapResult<ResultType>(result);
    endQueryHandleDependency(newResult);
    newResult->lastComputed = this->currentRevisionNumber;
    newResult->lastChanged = this->currentRevisionNumber;
    queryMap->insert(search, {tupleOfArgs, toOwned(newResult)});
    return newResult->result;
  }

};

} // end namespace chpl

/**
 Should be called at the start of a query.
 */
#define STRINGIZE_LINE_DETAIL(x) #x
#define STRINGIZE_LINE(x) STRINGIZE_LINE_DETAIL(x)

/**
  Should be called at the start of a query function like this:

    BEGIN_QUERY(context, <ResultType>, <Args>)

  For example

    BEGIN_QUERY(context, double, 1);

  indicates that the result type is 'double' and that the arguments
  for the query are just the number 1.
 */
#define BEGIN_QUERY(context, ResultType, ...) \
  const char* BEGIN_QUERY_FILE = __FILE__; \
  int BEGIN_QUERY_LINE = __LINE__; \
  const char* BEGIN_QUERY_FUNC = __func__; \
  const char* BEGIN_QUERY_FILE_LINE = __FILE__ STRINGIZE_LINE(__LINE__); \
  UniqueString BEGIN_QUERY_NAME = UniqueString::build(BEGIN_QUERY_FILE_LINE); \
  auto BEGIN_QUERY_ARGS = std::make_tuple(__VA_ARGS__); \
  auto BEGIN_QUERY_MAP = \
    context->beginQueryGetMap<ResultType>(BEGIN_QUERY_NAME, \
                                          BEGIN_QUERY_ARGS); \
  { \
    auto BEGIN_QUERY_SEARCH = BEGIN_QUERY_MAP->find(BEGIN_QUERY_ARGS); \
    if (BEGIN_QUERY_SEARCH != BEGIN_QUERY_MAP->end()) { \
      auto BEGIN_QUERY_RESULT = BEGIN_QUERY_SEARCH->second->get(); \
      if (context->beginQueryCanUseResultDeps(BEGIN_QUERY_RESULT)) { \
        return BEGIN_QUERY_RESULT->result; \
      } \
    } \
  } \
  context->beginQueryComputation();

/**
 Should be called at the end of a query like this:

   return END_QUERY(context, result);
 */
#define END_QUERY(context, result) \
  endQuery(BEGIN_QUERY_NAME, BEGIN_QUERY_ARGS, result, BEGIN_QUERY_MAP);

#endif
