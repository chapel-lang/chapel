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

#include "chpl/queries/ErrorMessage.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/util/memory.h"
#include "chpl/util/hash.h"

#include <cstdint>
#include <cstring>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

/// \cond DO_NOT_DOCUMENT
namespace chpl {

class Context;
class ID;

namespace detail {

struct UniqueStrEqual final {
  bool operator()(const char* lhs, const char* rhs) const {
    // pass the 2 metadata bytes
    return strcmp(lhs, rhs) == 0;
  }
};

struct UniqueStrHash final {
  size_t operator()(const char* s) const {
    return chpl::hash(s);
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

template<typename TUP, size_t... I>
static inline bool queryArgsEqualsImpl(const TUP& lhs, const TUP& rhs, std::index_sequence<I...>)
{
  // Lambda wrapper to std::equal_to
  auto eq = [](const auto& x, const auto& y) {
    std::equal_to<std::decay_t<decltype(x)>> equal_to;
    return equal_to(x,y);
  };

  // TODO: C++17 fold expression
  // return(... && eq(std::get<I>(lhs), std::get<I>(rhs)));

  // C++14 code
  // This expands the ret = ret && std::get<I>(lhs) == std::get<I>(rhs)
  // assignments into a brace initializer list, evaluating the elements of
  // TUP in left-to-right order. The value of ret is true iff all elements
  // compare equal. The compiler optimizes the short-circuiting and
  // sometimes generates SSE instructions to compare multiple words at once.
  // The dummy variable is optimized away, as it is unused.
  bool ret = true;
  auto dummy = {(ret = ret && eq(std::get<I>(lhs), std::get<I>(rhs)))...};
  return ret;
}

template<typename... Ts>
bool queryArgsEquals(const std::tuple<Ts...>& lhs, const std::tuple<Ts...>& rhs)
{
  return queryArgsEqualsImpl(lhs, rhs, std::index_sequence_for<Ts...>{});
}

template<typename ResultType, typename... ArgTs>
struct QueryMapArgTupleHash final {
  size_t operator()(const QueryMapResult<ResultType, ArgTs...>& r) const {
    return chpl::hash(r.tupleOfArgs);
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

template<typename TUP, size_t... I>
static inline void queryArgsPrintImpl(const TUP& tuple, std::index_sequence<I...>) {
  // lambda to optionally print separator, then print element
  auto print = [](bool printsep, const auto& elem) {
      if(printsep)
        queryArgsPrintSep();
      queryArgsPrintOne(elem);
  };

  // TODO: C++17 comma fold expression
  // (... , print(I != 0, std::get<I>(tuple)));

  // C++14 code
  // This prints the elements in order, with a separator in-between.
  // The comma (, 0) is used to initialize a dummy initializer_list.
  // The compiler optimizes away the dummy variable and list of 0s.
  auto dummy = { (print(I != 0, std::get<I>(tuple)), 0) ... };
}

template<typename... Ts>
void queryArgsPrint(const std::tuple<Ts...>& tuple) {
  queryArgsPrintImpl(tuple, std::index_sequence_for<Ts...>{});
}

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
  virtual void markUniqueStringsInResult(Context* context) const = 0;
};

template<typename ResultType, typename... ArgTs>
class QueryMapResult final : public QueryMapResultBase {
 public:
  std::tuple<ArgTs...> tupleOfArgs;
  mutable ResultType result;

  // This constructor creates an entry with
  //  * lastChecked and lastChanged = -1
  //  * a default-constructed result
  QueryMapResult(QueryMap<ResultType, ArgTs...>* parentQueryMap,
                 std::tuple<ArgTs...> tupleOfArgs)
    : QueryMapResultBase(-1, -1, parentQueryMap),
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
  void markUniqueStringsInResult(Context* context) const;
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
                                     QueryMapArgTupleHash<ResultType, ArgTs...>,
                                     QueryMapArgTupleEqual<ResultType, ArgTs...>>;
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
        printf("Removing result %p %s\n", &result, queryName);
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
