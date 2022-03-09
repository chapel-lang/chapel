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
#include <vector>

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
class PassManager {
 private:
  // Run pass over many and return it's results (if any)
  template <typename T, typename R, typename Container>
  R runPass(PassT<T, R>& pass, const Container& xs) {
    for (auto& x : xs) {
      pass.run(x);
    }
    return pass.getResult();
  }

 public:
  // We take pass by && in these for the common case where the caller
  // sinks their argument; like runPass(myPassName(), ...);

  // Run pass on a single item and return it's results (if any)
  template <typename T, typename R>
  R runPass(PassT<T, R>&& pass, T x) {
    return runPass(pass, {x});
  }

  // Run pass on many items and return it's results (if any)
  template <typename T, typename R, typename Container>
  R runPass(PassT<T, R>&& pass, const Container& xs) {
    return runPass(pass, xs);
  }

  // Run multiple passes on many items, pass at a time. Does not return results
  template <typename T, typename Container>
  void runPass(PassTList<T>&& passes, const Container& xs) {
    for (auto& p : passes) {
      runPass(*p, xs);
    }
  }

  // Run multiple passes over many, node at a time. Does not return results
  // NOTE: switching between runPass and runPassChained cannot be done without
  // manual reasoning of whether passes can commute
  template <typename T, typename Container>
  void runPassChained(PassTList<T>&& passes, Container& xs) {
    for (auto& x : xs) {
      for (auto& p : passes) {
        p.run(x);
      }
    }
  }
};

#endif
