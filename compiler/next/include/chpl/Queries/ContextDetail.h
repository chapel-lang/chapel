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

#ifndef CHPL_QUERIES_CONTEXT_DETAIL_H
#define CHPL_QUERIES_CONTEXT_DETAIL_H

#include "chpl/AST/UniqueString.h"
#include "chpl/AST/ErrorMessage.h"
#include "chpl/Util/memory.h"

#include <cstring>
#include <unordered_set>
#include <vector>

/// \cond DO_NOT_DOCUMENT
namespace chpl {

class Contex;

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

using RevisionNumber = int64_t;

// forward declare some types
class QueryMapResultBase;
template<typename ResultType, typename... ArgTs> class QueryMapResult;
class QueryMapBase;
template<typename ResultType, typename... ArgTs> class QueryMap;

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

template<typename ResultType, typename... ArgTs>
struct QueryMapArgTupleHash final {
  std::size_t operator()(const QueryMapResult<ResultType, ArgTs...>& r) const {
    return queryArgsHash(r.tupleOfArgs);
  }
};


template<typename ResultType, typename... ArgTs>
struct QueryMapArgTupleEqual final {
  bool operator()(const QueryMapResult<ResultType, ArgTs...>& lhs,
                  const QueryMapResult<ResultType, ArgTs...>& rhs) const {
    return queryArgsEquals(lhs.tupleOfArgs, rhs.tupleOfArgs);
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

using QueryDependencyVec = std::vector<const QueryMapResultBase*>;
using QueryErrorVec = std::vector<ErrorMessage>;

class QueryMapResultBase {
 public:

  // lastChecked indicates when the query result has last been checked
  // to be up-to-date by checking its dependencies. When running a query,
  // it has these meanings:
  //  * if it is a previous revision, dependencies need to be checked
  //    and re-run if they are out of date.
  //  * if it is the current revision, the result can be reused
  mutable RevisionNumber lastChecked;
  // lastChanged indicates the last revision in which the query result
  // has changed
  mutable RevisionNumber lastChanged;

  mutable QueryDependencyVec dependencies;
  mutable QueryErrorVec errors;

  QueryMapBase* parentQueryMap;

  QueryMapResultBase(RevisionNumber lastChecked,
                     RevisionNumber lastChanged,
                     QueryMapBase* parentQueryMap)
    : lastChecked(lastChecked),
      lastChanged(lastChanged),
      dependencies(),
      errors(),
      parentQueryMap(parentQueryMap) {
  }
  virtual ~QueryMapResultBase() = 0; // this is an abstract base class
  virtual void recompute(Context* context) const = 0;
};

template<typename ResultType, typename... ArgTs>
class QueryMapResult final : public QueryMapResultBase {
 public:
  std::tuple<ArgTs...> tupleOfArgs;
  mutable ResultType result;
  // This requires results be default-constructable but that's
  // not strictly necessary. This function is just used when
  // creating a value to use when querying the map.
  // This requirement can be removed with C++20 features or with
  // some hacky changes here.
  QueryMapResult(std::tuple<ArgTs...> tupleOfArgs)
    : QueryMapResultBase(-1, -1, nullptr),
      tupleOfArgs(std::move(tupleOfArgs)),
      result() {
  }
  QueryMapResult(RevisionNumber lastChecked,
                 RevisionNumber lastChanged,
                 QueryMap<ResultType, ArgTs...>* parentQueryMap,
                 std::tuple<ArgTs...> tupleOfArgs,
                 ResultType result)
    : QueryMapResultBase(lastChecked, lastChanged, parentQueryMap),
      tupleOfArgs(std::move(tupleOfArgs)),
      result(std::move(result)) {
  }
  void recompute(Context* context) const;
};

class QueryMapBase {
 public:
   const char* queryName;
   bool isInputQuery;

   QueryMapBase(const char* queryName, bool isInputQuery)
     : queryName(queryName), isInputQuery(isInputQuery) {
   }
   virtual ~QueryMapBase() = 0; // this is an abstract base class
   virtual void clearOldResults(RevisionNumber currentRevisionNumber) = 0;
};

template<typename ResultType,
         typename... ArgTs>
class QueryMap final : public QueryMapBase {
 public:
  using TheResultType = QueryMapResult<ResultType, ArgTs...>;
  using MapType = std::unordered_set<TheResultType,
                                     QueryMapArgTupleHash<ResultType,ArgTs...>,
                                     QueryMapArgTupleEqual<ResultType,ArgTs...>>;
  using QueryFunctionType = const ResultType& (*)(Context* context, ArgTs...);

  // the main map (which is actually a set since the result needs to
  // store the key (i.e. the args) in order to be recomputable
  MapType map;
  // old results stores replaced results long enough for dependent
  // queries to compare with them.
  std::vector<ResultType> oldResults;
  // the function to recompute the query.
  QueryFunctionType queryFunction;

  QueryMap(const char* queryName, bool isInputQuery, QueryFunctionType queryFunction)
     : QueryMapBase(queryName, isInputQuery),
       map(), oldResults(),
       queryFunction(queryFunction) {
  }

  ~QueryMap() = default;
  void clearOldResults(RevisionNumber currentRevisionNumber) override {
    // Performance: Would it be better to move everything to a new map
    // rather than modify it in place as is done here?
    auto iter = map.begin();
    while (iter != map.end()) {
      const TheResultType& result = *iter;
      if (result.lastChecked == currentRevisionNumber) {
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
