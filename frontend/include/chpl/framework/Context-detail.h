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

#ifndef CHPL_QUERIES_CONTEXT_DETAIL_H
#define CHPL_QUERIES_CONTEXT_DETAIL_H

#include "chpl/framework/UniqueString.h"
#include "chpl/util/memory.h"
#include "chpl/util/hash.h"

#include <chrono>
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

class ErrorBase;

namespace detail {

/**
  Helper type that represents a string and a length.
  An alternative strategy for storing null bytes in unique'd strings
  would be to store the length before the allocation -- but
  then we'd have to allocate such a special buffer in order to query it.
 */
struct StringAndLength {
  const char* str;
  size_t len;
};

struct UniqueStrEqual final {
  bool operator()(StringAndLength lhs, StringAndLength rhs) const {
    if (lhs.len != rhs.len) return false;
    return 0 == memcmp(lhs.str, rhs.str, lhs.len);
  }
};

struct UniqueStrHash final {
  size_t operator()(StringAndLength key) const {
    return chpl::hash(key.str, key.len);
  }
};

}

namespace querydetail {

using RevisionNumber = int64_t;

using QueryTimingClock = std::chrono::steady_clock;
using QueryTimingDuration = QueryTimingClock::duration;

struct QueryTimingStat {
  QueryTimingDuration elapsed = QueryTimingDuration::zero();
  size_t count = 0;

  void update(QueryTimingDuration d) {
    count += 1;
    elapsed += d;
  }
};

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
  (void) dummy; // avoid unused variable warning
  return ret;
}

template<typename... Ts>
bool queryArgsEquals(const std::tuple<Ts...>& lhs, const std::tuple<Ts...>& rhs)
{
  return queryArgsEqualsImpl(lhs, rhs, std::index_sequence_for<Ts...>{});
}
static inline bool queryArgsEquals(const std::tuple<>& lhs, const std::tuple<>& rhs)
{
  return true;
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

template<typename T>
static void queryArgsPrintOne(const T& v) {
  stringify<T> tString;
  std::ostringstream ss;
  tString(ss, chpl::StringifyKind::DEBUG_SUMMARY, v);
  printf("%s", ss.str().c_str() );
}


template<typename TUP, size_t... I>
static inline void queryArgsPrintImpl(const TUP& tuple,
                                      std::index_sequence<I...>) {
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
  (void) dummy; // avoid unused variable warning
}

// taken from https://codereview.stackexchange.com/questions/193420/apply-a-function-to-each-element-of-a-tuple-map-a-tuple
// when the queryArgsToStringsImpl is dropped we can remove these too
template <class F, typename Tuple, size_t... Is>
auto applyToEachImpl(Tuple t, F f, std::index_sequence<Is...>) {
  return std::make_tuple(f(std::get<Is>(t) )...
  );
}

template <class F, typename... Args>
auto applyToEach(F f, const std::tuple<Args...>& t) {
  return applyToEachImpl(t, f, std::make_index_sequence<sizeof...(Args)>{});
}

template<typename TUP, size_t... I>
static inline const auto queryArgsToStringsImpl(const TUP& tuple,
                                                std::index_sequence<I...>) {
  // lambda to convert
  auto convert = [](auto& elem) {
    chpl::stringify<std::decay_t<decltype(elem)>> stringifier;
    std::ostringstream ss;
    stringifier(ss, chpl::StringifyKind::DEBUG_DETAIL, elem);
    return ss.str();
  };

  return applyToEach(convert, tuple);
}

template<typename... Ts>
auto queryArgsToStrings(const std::tuple<Ts...>& tuple) {
  return queryArgsToStringsImpl(tuple, std::index_sequence_for<Ts...>{});
  // TODO: Should be able to replace the Impl with the code below, but it
  //  fails to compile as written
  // chpl::stringify<std::tuple<Ts...>> stringifier;
  // std::ostringstream ss;
  // stringifier(ss, chpl::StringifyKind::DEBUG_SUMMARY, tuple);
  // return ss.str();
}

template<typename... Ts>
void queryArgsPrint(const std::tuple<Ts...>& tuple) {
  queryArgsPrintImpl(tuple, std::index_sequence_for<Ts...>{});
}
static inline void queryArgsPrint(const std::tuple<>& tuple) {
}

// Performance: this struct only contains a pointer and an additional bit
// field. We could probably get away with storing `errorCollectionRoot`
// in the last bit of the result pointer, and and thus reduce the overhead
// of this struct.
struct QueryDependency {
  const QueryMapResultBase* query;
  bool errorCollectionRoot;

  QueryDependency(const QueryMapResultBase* query,
                  bool errorCollectionRoot) :
    query(query), errorCollectionRoot(errorCollectionRoot) {}
};

using QueryDependencyVec = std::vector<QueryDependency>;
using QueryErrorVec = std::vector<owned<ErrorBase>>;

class QueryMapResultBase {
 public:

  // lastChecked indicates when the query result has last been checked
  // to be up-to-date by checking its dependencies. When running a query,
  // it has these meanings:
  //  * if it is a previous revision, dependencies need to be checked
  //    and re-run if they are out of date.
  //  * if it is the current revision, the result can be reused
  mutable RevisionNumber lastChecked = -1;
  // lastChanged indicates the last revision in which the query result
  // has changed
  mutable RevisionNumber lastChanged = -1;

  mutable QueryDependencyVec dependencies;

  // Whether or not errors from this query result have been shown to the
  // user (they may not have been if some query checked for errors).
  mutable bool emittedErrors = false;
  mutable QueryErrorVec errors;

  QueryMapBase* parentQueryMap;

  QueryMapResultBase(RevisionNumber lastChecked,
                     RevisionNumber lastChanged,
                     bool emittedErrors,
                     QueryMapBase* parentQueryMap);
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
    : QueryMapResultBase(-1, -1, false, parentQueryMap),
      tupleOfArgs(std::move(tupleOfArgs)),
      result() {
  }
  QueryMapResult(RevisionNumber lastChecked,
                 RevisionNumber lastChanged,
                 bool emittedErrors,
                 QueryMap<ResultType, ArgTs...>* parentQueryMap,
                 std::tuple<ArgTs...> tupleOfArgs,
                 ResultType result)
    : QueryMapResultBase(lastChecked, lastChanged, emittedErrors, parentQueryMap),
      tupleOfArgs(std::move(tupleOfArgs)),
      result(std::move(result)) {
  }
  void recompute(Context* context) const override;
  void markUniqueStringsInResult(Context* context) const override;
};

class QueryMapBase {
 public:
   const char* queryName;
   bool isInputQuery;

  struct QueryStats {
    // NOTE: `system` here refers to what the query system is doing
    // `self` is confusing b/c a profiler uses `self` to mean, time in me - time in children

    // Time spent inside a query after a QUERY_BEGIN
    //   * Not incremented (time or count) when queryUseSaved is true
    QueryTimingStat query;

    // Time spent inside Context::queryBeginGetMap
    QueryTimingStat systemGetMap;

    // Time spent inside Context::queryBeginGetResult
    QueryTimingStat systemGetResult;
    // Other per-query timings can be added here as needed
  } timings;

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
        // Remove the result
        iter = map.erase(iter);
      }
    }

    oldResults.clear();
  }
};

// Stopwatch that conditionally starts based on `enabled` passed to the
// constructor.
// `onExit` is called with a reference to the object itself so that `elapsed()`
// can be conditionally called. This avoids us needing to store an enabled flag
template<typename F, typename Clock=QueryTimingClock>
struct QueryTimingStopwatch {
  F onExit_;
  typename Clock::time_point start_;

  QueryTimingStopwatch(bool enabled, F onExit)
      : onExit_(onExit) {
    if (enabled) {
      start_ = Clock::now();
    }
  }

  QueryTimingDuration elapsed() {
    auto stop = Clock::now();
    return stop - start_;
  }

  ~QueryTimingStopwatch() { onExit_(*this); }
};

// Helper function to sort out the templates over lambda's
template <typename F> QueryTimingStopwatch<F>
makeQueryTimingStopwatch(bool enabled, F onExit) {
  return QueryTimingStopwatch<F>(enabled, onExit);
}

inline auto
makePlainQueryTimingStopwatch(bool enabled) {
  return makeQueryTimingStopwatch(enabled, [](auto& arg){(void)arg;});
}

} // end namespace querydetail
} // end namespace chpl
/// \endcond

#endif
