#ifndef CHPL_QUERIES_CONTEXT_DETAIL_H
#define CHPL_QUERIES_CONTEXT_DETAIL_H

#include "chpl/AST/UniqueString.h"
#include "chpl/AST/ErrorMessage.h"
#include "chpl/Util/memory.h"

#include <cstring>
#include <unordered_map>
#include <vector>

/// \cond DO_NOT_DOCUMENT
namespace chpl {
namespace detail {

struct UniqueStrEqual final {
  bool operator()(const char* lhs, const char* rhs) const {
    // pass the 2 metadata bytes
    return strcmp(lhs, rhs) == 0;
  }
};

struct UniqueStrHash final {
  std::size_t operator()(const char* s) const {
    // this hash is from StringHashFns in the old map.h
    unsigned int h = 0;
    while (*s) h = h * 27 + (unsigned char)*s++;
    return h;
  }
};

}

namespace querydetail {

typedef int64_t RevisionNumber;
class QueryMapResultBase;

// define a hash function for std::tuple since the standard doesn't
// include one.
template<typename T>
static std::size_t queryArgsHashOne(const T& v) {
  std::hash<T> hasher;
  return hasher(v);
}

template<size_t I, typename... Ts>
static std::size_t queryArgsHashImpl(const std::tuple<Ts...>& tuple) {
  size_t myhash = queryArgsHashOne(std::get<I>(tuple));
  if (I + 1 < sizeof...(Ts)) {
    size_t resthash =
      queryArgsHashImpl<(I + 1 < sizeof... (Ts) ? I + 1 : I)>(tuple);
    return chpl::hash_combine(myhash, resthash);
  } else {
    return myhash;
  }
}

template<typename... Ts>
static std::size_t queryArgsHash(const std::tuple<Ts...>& tuple) {
  return queryArgsHashImpl<0>(tuple);
}
template<>
std::size_t queryArgsHash<>(const std::tuple<>& tuple);

// define an equality function for std::tuple. the standard will call ==
// but we want to always compare our keys with equal_to 
template<typename T>
static bool queryArgsEqualsOne(const T& lhs, const T& rhs) {
  std::equal_to<T> eq;
  return eq(lhs, rhs);
}

template<size_t I, typename... Ts>
static bool queryArgsEqualsImpl(const std::tuple<Ts...>& lhs,
                                const std::tuple<Ts...>& rhs) {
  bool myeq = queryArgsEqualsOne(std::get<I>(lhs), std::get<I>(rhs));
  if (myeq == false)
    return false;

  if (I + 1 < sizeof...(Ts)) {
    return queryArgsEqualsImpl<(I + 1 < sizeof... (Ts) ? I + 1 : I)>(lhs, rhs);
  } else {
    return true;
  }
}

template<typename... Ts>
static bool queryArgsEquals(const std::tuple<Ts...>& lhs,
                            const std::tuple<Ts...>& rhs) {
  return queryArgsEqualsImpl<0>(lhs, rhs);
}

template<>
bool queryArgsEquals<>(const std::tuple<>& lhs,
                       const std::tuple<>& rhs);

template<typename... Ts>
struct QueryMapArgTupleHash final {
  std::size_t operator()(const std::tuple<Ts...>& x) const {
    return queryArgsHash(x);
  }
};


template<typename... Ts>
struct QueryMapArgTupleEqual final {
  bool operator()(const std::tuple<Ts...>& lhs,
                  const std::tuple<Ts...>& rhs) const {
    return queryArgsEquals(lhs, rhs);
  }
};

// define a way to debug-print out a tuple
void queryArgsPrintSep();
void queryArgsPrintUnknown();

template<typename T>
static void queryArgsPrintOne(const T& v) {
  queryArgsPrintUnknown();
}

void queryArgsPrintOne(const ID& v);
void queryArgsPrintOne(const UniqueString& v);

template<size_t I, typename... Ts>
static void queryArgsPrintImpl(const std::tuple<Ts...>& tuple) {
  queryArgsPrintOne(std::get<I>(tuple));
  if (I + 1 < sizeof...(Ts)) {
    queryArgsPrintSep();
    queryArgsPrintImpl<(I + 1 < sizeof... (Ts) ? I + 1 : I)>(tuple);
  }
}

template<typename... Ts>
static void queryArgsPrint(const std::tuple<Ts...>& tuple) {
  queryArgsPrintImpl<0>(tuple);
}
template<>
void queryArgsPrint<>(const std::tuple<>& tuple);

typedef std::vector<QueryMapResultBase*> QueryDependencyVec;
typedef std::vector<ErrorMessage> QueryErrorVec;

class QueryMapResultBase {
 public:
  QueryDependencyVec dependencies;
  RevisionNumber lastCheckedAndReused;
  RevisionNumber lastComputed;
  RevisionNumber lastChanged;
  // errors includes errors from dependencies, transitively,
  // so that the dependencies can be forgotten and errors still reported.
  QueryErrorVec errors;

  QueryMapResultBase(QueryDependencyVec deps,
                     RevisionNumber lastCheckedAndReused,
                     RevisionNumber lastComputed,
                     RevisionNumber lastChanged,
                     QueryErrorVec errors)
    : dependencies(std::move(deps)),
      lastCheckedAndReused(lastCheckedAndReused),
      lastComputed(lastComputed),
      lastChanged(lastChanged),
      errors(std::move(errors)) {
  }
  virtual ~QueryMapResultBase() = 0; // this is an abstract base class
};
template<typename ResultType>
class QueryMapResult final : public QueryMapResultBase {
 public:
  ResultType result;
  QueryMapResult(QueryDependencyVec deps,
                 RevisionNumber lastCheckedAndReused,
                 RevisionNumber lastComputed,
                 RevisionNumber lastChanged,
                 QueryErrorVec errors,
                 ResultType result)
    : QueryMapResultBase(std::move(deps),
                         lastCheckedAndReused, lastComputed, lastChanged,
                         std::move(errors)),
      result(std::move(result)) {
  }
};

class QueryMapBase {
 public:
   UniqueString queryName;
   const char* prettyFunc;

   QueryMapBase(UniqueString queryName, const char* prettyFunc)
     : queryName(queryName), prettyFunc(prettyFunc) {
   }
   virtual ~QueryMapBase() = 0; // this is an abstract base class
   virtual void clearOldResults(RevisionNumber currentRevisionNumber) = 0;
};

template<typename ResultType, typename... ArgTs>
class QueryMap final : public QueryMapBase {
 public:
  typedef QueryMapResult<ResultType> TheResultType;
  typedef std::unordered_map<std::tuple<ArgTs...>,
                     TheResultType,
                     QueryMapArgTupleHash<ArgTs...>,
                     QueryMapArgTupleEqual<ArgTs...>> MapType;
  // the main map
  MapType map;
  // old results stores replaced results long enough for dependent
  // queries to compare with them.
  std::vector<ResultType> oldResults;

  QueryMap(UniqueString queryName, const char* prettyFunc)
     : QueryMapBase(queryName, prettyFunc), map(), oldResults() {
  }
  ~QueryMap() = default;
  void clearOldResults(RevisionNumber currentRevisionNumber) override {
    // Performance: Would it be better to move everything to a new map
    // rather than modify it in place as is done here?
    auto iter = map.begin();
    while (iter != map.end()) {
      TheResultType& result = iter->second;
      if (result.lastCheckedAndReused >= currentRevisionNumber ||
          result.lastComputed >= currentRevisionNumber ||
          result.lastChanged >= currentRevisionNumber) {
        // Keep the result
        ++iter;
      } else {
        // Remove the result
        iter = map.erase(iter);
      }
    }

    oldResults.clear();
  }
};

} // end namespace querydetail
} // end namespace chpl
/// \endcond

#endif
