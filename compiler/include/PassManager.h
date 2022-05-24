/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef _PASS_MANAGER_H_
#define _PASS_MANAGER_H_

#include <memory>
#include <unordered_set>
#include <vector>
#include "vec.h"

//
// A PassT is a Pass that runs on things of type T and can return results of ResultType
//
// The default run behavior provides a common pattern that many passes
// exhibit, along with providing a place for an idempotence check to
// be used in the future. A PassT processes a single item of type T so
// that the caller can drive the execution.
//
// There might be a useful base class for this that has more general `run` method
// for instance, but for now having it templated over its input type is too useful.
//
// Run could equally be spelled operator() but I find the name more clear.
// I expect most of our uses today to be de-virtualized (thinking of perf).
//
// Some passes accumulate some result after the whole pass is done,
// which can be returned with getResult. This might be storing some
// info in a map for instance. Future work could extend a pass to be
// combinable (operator+ maybe) so that parallel execution could
// divide, conquer, and combine the results.
//
//
template <typename T, typename ResultType = void> class PassT {
 public:
  virtual ~PassT() = default;

  virtual bool shouldProcess(T x) {
    (void)x;
    return true;
  }

  virtual void process(T x) {
    // do nothing
  }

  // main driver method, provides
  virtual void run(T x) {
    if (shouldProcess(x)) {
      process(x);
    }
  };

  virtual ResultType getResult() {
    // return nothing
  }

  virtual bool hasNext() {
    return false;
  }

  virtual void processNext() {
    // do nothing
  }

  virtual bool alreadyProcessed(T x) {
    auto it = processed_.find(x);
    bool ret = it != processed_.end();
    processed_.emplace_hint(it, x);
    return ret;
  }

 private:
  std::unordered_set<T> processed_;
};

//
// A PassTU is a PassT that is concerned with processing things of type T and U
//
// A common example of this two type pass is FnSymbol and CallExpr and the
// following documentation uses it as a running example.
//
// There are two overloads of the `process` method that should be overridden:
//      general             | example
//   1) process(T x)        | process(FnSymbol* fn)
//   2) process(T x, U, y)  | process(FnSymbol* fn, CallExpr* call)
//
// The first overload serves to process the "primary" unit of work and can
// modify itself, but if/when it discovers more work to be done, it should use
// the appropriate overload of `enqueue`:
//      general             | example
//   1) enqueue(T x)        | enqueue(FnSymbol* fn)
//   2) enqueue(T x, U, y)  | enqueue(FnSymbol* fn, CallExpr* call)
//
// Each `enqueue` overload corresponds to a `process` overload and is in effect
// a delayed function call.
//
// The second overload is called to process a `call` in its enclosing `fn`
//
template <typename T, typename U, typename ResultType = void>
class PassTU : public PassT<T, ResultType> {
 public:

  // We need this to get an overload of an inherited method
  using PassT<T, ResultType>::process;

  virtual void process(T x, U y) {
    // do nothing
  }

  void enqueue(T x) { TQueue_.push_back(x); }

  void enqueue(T x, U y) { TUQueue_.push_back({x, y}); }

  bool hasNext() override {
    return !TQueue_.empty() || !TUQueue_.empty();
  }

  void processNext() override {
    if (!TQueue_.empty()) {
      auto x = TQueue_.back();
      TQueue_.pop_back();
      process(x);

    } else if (!TUQueue_.empty()) {
      auto pair = TUQueue_.back();
      TUQueue_.pop_back();
      process(pair.first, pair.second);
    }
  }

 private:
  // This might be different for different ordering guarantees
  template<typename QT>
  using Queue = std::vector<QT>;

  Queue<T> TQueue_;
  Queue<std::pair<T, U>> TUQueue_;

  // another queueing method is something like
  // std::unordered_map<T, std::vector<U>>
  // which might give you better locality and/or visibility
  // into whether there are remaining tasks for a specific T
};

template <typename T> using PassTList = std::vector<std::unique_ptr<PassT<T>>>;

//
// PassManager runs passes over some collection of objects
//
// Today, we only have "immediate mode" execution of a pass or group
// of passes that operate on the same type. (as opposed to building up
// something like an execution graph that is later executed) because
// it still gives us the benefit of having the passmanager drive
// execution (so that sequential passes that operate over the same
// type can be chained (where possible)), but without the
// complexity. Part of the implementation difficulty with that
// approach today is that we have passes that operate on a load of
// different types and building up a typesafe graph of those is more
// trouble than worth at this point. One can imagine this would change
// when we have fewer pass types, like say Global, Module, Type, and
// Function. Its not clear to me at this point we can flip the switch
// today so that eg. a pass over CallExpr is actually a Function pass
// with an adapter to visit every CallExpr without a performance hit.
//
// When working over a "std::vector" or "Vec" type, the PassManager will
// only iterate over elements present when the pass started running, by
// noting the size at that time.
//
class PassManager {
 private:
  // Run pass over many and return it's results (if any). Specialization
  // for vector with fixed length iteration in order to avoid iterator
  // invalidation.
  template <typename T, typename R, typename Elt>
  R runPassImpl(PassT<T, R>& pass, const std::vector<Elt>& xs) {
    const int stop = xs.size();
    for (int i = 0; i < stop; i++) {
      pass.run(xs[i]);
    }

    while (pass.hasNext()) {
      pass.processNext();
    }

    return pass.getResult();
  }

  // Run pass over many and return it's results (if any). Specialization
  // for vector with fixed length iteration in order to avoid iterator
  // invalidation.
  template <typename T, typename R, typename Elt>
  R runPassImpl(PassT<T, R>& pass, const Vec<Elt>& xs) {
    const int stop = xs.size();
    for (int i = 0; i < stop; i++) {
      pass.run(xs.v[i]);
    }

    while (pass.hasNext()) {
      pass.processNext();
    }

    return pass.getResult();
  }

  // Run pass over many and return it's results (if any)
  template <typename T, typename R, typename Container>
  R runPassImpl(PassT<T, R>& pass, const Container& xs) {
    for (auto& x : xs) {
      pass.run(x);
    }

    while (pass.hasNext()) {
      pass.processNext();
    }

    return pass.getResult();
  }

 public:
  // We take pass by && in these for the common case where the caller
  // sinks their argument; like runPass(myPassName(), ...); Also
  // because a pass list should get moved in so we can own it

  template <typename T, typename R, typename Container>
  R runPass(PassT<T, R>& pass, const Container& xs) {
    return runPassImpl(pass, xs);
  }

  // Run pass on many items and return it's results (if any)
  template <typename T, typename R, typename Container>
  R runPass(PassT<T, R>&& pass, const Container& xs) {
    return runPassImpl(pass, xs);
  }

  // Run multiple passes on many items, pass at a time. Does not return results
  template <typename T, typename Container>
  void runPass(PassTList<T>&& passes, const Container& xs) {
    for (auto& p : passes) {
      runPassImpl(*p, xs);
    }
  }

  // Run multiple passes over many, node at a time. Does not return results
  // NOTE: switching between runPass and runPassChained cannot be done without
  // manual reasoning of whether passes can commute
  template <typename T, typename Container>
  void runPassChained(PassTList<T>&& passes, Container& xs) {
    for (auto& x : xs) {
      for (auto& p : passes) {
        // NOTE: these passes can't currently have leftovers:
        // In other words, it's not as easy to run multiple PassTU chained
        // together because we don't know for sure that at its legal to do
        //   p1.run(x); p2.run(x);
        // without
        //   p1.run(x); while (p1.hasNext()); p1.processNext(); p2.run(x)
        // and in the latter case, you're not guaranteed that another
        // call to
        //   p1.run(y)
        // for some y would have enqueued more work for x that should
        // be processed before p2.run(x).
        p.run(x);
      }
    }
  }
};

#endif
