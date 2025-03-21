/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "test-common.h"

#include "chpl/framework/Context.h"
#include "chpl/framework/query-impl.h"

#include <cstdlib>

using namespace chpl;

static const UniqueString& recursiveQuery(Context* context, UniqueString arg) {
  QUERY_BEGIN(recursiveQuery, context, arg);

  UniqueString result = recursiveQuery(context, arg);
  assert(result.isEmpty()); // Because recursion is guaranteed.

  return QUERY_END(result);
}

static const char* recHandlerMessage = "recursive query handler executed";
static const UniqueString& recursiveQueryWithHandler(Context* context, UniqueString arg) {
  QUERY_BEGIN(recursiveQueryWithHandler, context, arg);

  UniqueString result = recursiveQueryWithHandler(context, arg);
  assert(result.isEmpty()); // Because recursion is guaranteed.

  if (auto trace = context->recoverFromSelfRecursion()) {
    context->error(ID(), "%s", recHandlerMessage);
  }

  return QUERY_END(result);
}

static void test0() {
  // Test automatic recursion detection and reporting, without additional code.

  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto s = UniqueString::get(context, "Nice To See You Again World, Hello");
  assert(recursiveQuery(context, s).isEmpty());
  assert(std::any_of(guard.errors().begin(), guard.errors().end(), [](auto& error) {
    return error->type() == ErrorType::Recursion;
  }));
  guard.clearErrors();

  // Run again without bumping recursion to ensure consistent results.
  s = UniqueString::get(context, "Nice To See You Again World, Hello");
  assert(recursiveQuery(context, s).isEmpty());
  // We do not expect new errors since we already reported them.
  assert(guard.realizeErrors() == 0);

  // Bump revision and run again
  context->advanceToNextRevision(false);
  s = UniqueString::get(context, "Nice To See You Again World, Hello");
  assert(recursiveQuery(context, s).isEmpty());
  assert(std::any_of(guard.errors().begin(), guard.errors().end(), [](auto& error) {
    return error->type() == ErrorType::Recursion;
  }));
  guard.clearErrors();
}

static void test1() {
  // Test recursion detection where the query recovers from recursion.

  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto s = UniqueString::get(context, "Nice To See You Again World, Hello");
  assert(recursiveQueryWithHandler(context, s).isEmpty());
  assert(std::any_of(guard.errors().begin(), guard.errors().end(), [](auto& error) {
    return error->message() == recHandlerMessage;
  }));
  guard.clearErrors();

  // Run again without bumping recursion to ensure consistent results.
  s = UniqueString::get(context, "Nice To See You Again World, Hello");
  assert(recursiveQueryWithHandler(context, s).isEmpty());
  // We do not expect new errors since we already reported them.
  assert(guard.realizeErrors() == 0);

  // Bump revision and run again
  context->advanceToNextRevision(false);
  s = UniqueString::get(context, "Nice To See You Again World, Hello");
  assert(recursiveQueryWithHandler(context, s).isEmpty());
  assert(std::any_of(guard.errors().begin(), guard.errors().end(), [](auto& error) {
    return error->message() == recHandlerMessage;
  }));
  guard.clearErrors();
}

/**
  There can be two recursive queries at the same time. Consider this call graph:

                 --------
                 |      |
                 v      |
  .---.       .-----.   |
  | f | ----> |     |---| (first call)
  .---.       |  g  |
    ^         |     |---| (second call)
    |         .-----.   |
    |                   |
    ---------------------

  Both result in recursion errors. Even though 'g' can handle its own recursion,
  'f' should still be "in trouble" (i.e., poisoned), because it was also called
  recursively. Errors should be emitted for both functions.

  Of course, nothing precludes 'g' from calling 'f' first instead of itself.

    ---------------------
    |                   |
    v                   |
  .---.       .-----.   |
  | f | ----> |     |---| (first call)
  .---.       |  g  |
              |     |---| (second call)
              .-----.   |
                 ^      |
                 |      |
                 --------
 */

static bool const& queryG(Context* context, bool callFFirst, bool runHandler);

static bool const& queryF(Context* context, bool callFFirst, bool runHandler) {
  QUERY_BEGIN(queryF, context, callFFirst, runHandler);

  auto res = queryG(context, callFFirst, runHandler);

  if (runHandler) {
    if (auto err = context->recoverFromSelfRecursion()) {
      context->error(ID(), "f");
    }
  }

  return QUERY_END(res);
}

static bool const& queryG(Context* context, bool callFFirst, bool runHandler) {
  QUERY_BEGIN(queryG, context, callFFirst, runHandler);

  // Use bitwise & instead of logical && to avoid short-circuiting.
  // Some compilers warn for & with boolean operands, so cast to int to ignore
  // the warning.
  bool res;
  if (callFFirst) {
    res = (int)queryF(context, callFFirst, runHandler) &
          (int)queryG(context, callFFirst, runHandler);
  } else {
    res = (int)queryG(context, callFFirst, runHandler) &
          (int)queryF(context, callFFirst, runHandler);
  }

  if (runHandler) {
    if (auto err = context->recoverFromSelfRecursion()) {
      context->error(ID(), "g");
    }
  }

  return QUERY_END(res);
}

static void test2() {
  // Lock down the cases where the functions don't try to detect their own recursion.
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  assert(queryF(context, /* callFFirst */ true, /* runHandler */ false) == false);
  assert(std::count_if(guard.errors().begin(), guard.errors().end(), [](auto& error) {
    return error->type() == ErrorType::Recursion;
  }) == 2);
  guard.clearErrors();

  assert(queryF(context, /* callFFirst */ false, /* runHandler */ false) == false);
  assert(std::count_if(guard.errors().begin(), guard.errors().end(), [](auto& error) {
    return error->type() == ErrorType::Recursion;
  }) == 2);
  guard.clearErrors();
}

static void test3() {
  // Lock down the cases where the functions do detect their own recursion.
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  assert(queryF(context, /* callFFirst */ true, /* runHandler */ true) == false);
  assert(std::count_if(guard.errors().begin(), guard.errors().end(), [](auto& error) {
    return error->message() == "f";
  }) == 1);
  assert(std::count_if(guard.errors().begin(), guard.errors().end(), [](auto& error) {
    return error->message() == "g";
  }) == 1);
  guard.clearErrors();

  assert(queryF(context, /* callFFirst */ false, /* runHandler */ true) == false);
  assert(std::count_if(guard.errors().begin(), guard.errors().end(), [](auto& error) {
    return error->message() == "f";
  }) == 1);
  assert(std::count_if(guard.errors().begin(), guard.errors().end(), [](auto& error) {
    return error->message() == "g";
  }) == 1);
  guard.clearErrors();
}

/*
 Even when only a single function creates a dependency cycle, the 'handler'
 should be executed only for the 'outer' invocation of the call that 'first'
 caused the cycle. Example:

 .--------.      .--------.      .--------.
 |  h(1)  | ---> |  h(2)  | ---> |  h(1)  |
 .--------.      .--------.      .--------.
     (A)             (B)             (C)

 (C) will trigger recursion, and (B) will be the first to try recover from self-recursion.
 However, for the error message to be proper, (A) should be the one to report the error.
 Thus, (B)'s handler should not be executed (recoverFromSelfRecursion will
 return a falsey value for B).
 */

static bool const& queryH(Context* context, bool parity, bool runHandler) {
  QUERY_BEGIN(queryH, context, parity, runHandler);

  bool res = queryH(context, !parity, runHandler);

  if (runHandler) {
    if (auto err = context->recoverFromSelfRecursion()) {
      context->error(ID(), "h(%s)", parity ? "true" : "false");
    }
  }

  return QUERY_END(res);
}

static void test4() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  assert(queryH(context, /* parity */ true, /* runHandler */ false) == false);
  assert(std::count_if(guard.errors().begin(), guard.errors().end(), [](auto& error) {
    return error->type() == ErrorType::Recursion;
  }) == 1);
  guard.clearErrors();
}

static void test5() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  assert(queryH(context, /* parity */ true, /* runHandler */ true) == false);
  assert(std::count_if(guard.errors().begin(), guard.errors().end(), [](auto& error) {
    return error->message() == "h(true)";
  }) == 1);
  guard.clearErrors();
}

int main() {
  test0();
  test1();
  test2();
  test3();
  test4();
  test5();
}
