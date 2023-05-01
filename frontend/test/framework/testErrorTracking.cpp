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

#include "test-common.h"

#include "chpl/framework/Context.h"
#include "chpl/framework/query-impl.h"
#include "ErrorGuard.h"

#include <cstdlib>

using namespace chpl;


static const std::string& inputQuery(Context* context) {
  QUERY_BEGIN_INPUT(inputQuery, context);
  std::string result = "This is from input";
  return QUERY_END(result);
}

static const std::string& queryThatErrors(Context* context) {
  QUERY_BEGIN(queryThatErrors, context);
  context->error(ID(), "An error!");
  std::string result = "I returned a value but I also reported an error";
  return QUERY_END(result);
}

static const std::string& queryThatCapturesErrors(Context* context) {
  QUERY_BEGIN(queryThatCapturesErrors, context);
  std::string output = "";
  auto fineResult = context->runAndTrackErrors([](Context* ctx) {
    return inputQuery(ctx);
  });
  output += fineResult.ranWithoutErrors() ? "no errors" : "errors";
  output += " on the first go; ";
  auto badResult = context->runAndTrackErrors([](Context* ctx) {
    return queryThatErrors(ctx);
  });
  output += badResult.ranWithoutErrors() ? "no errors" : "errors";
  output += " on the second go";

  context->error(ID(), "Besides the errors I captured, I also had my own error.");
  return QUERY_END(output);
}

static const std::string& queryThatCapturesCapturingErrors(Context* context) {
  QUERY_BEGIN(queryThatCapturesCapturingErrors, context);
  std::string output = "";
  auto result = context->runAndTrackErrors([](Context* ctx) {
    return queryThatCapturesErrors(ctx);
  });
  output += "[" + result.result() + "] ";
  output += "error count: " + std::to_string(result.errors().size());
  return QUERY_END(output);
}

static const std::string& queryThatRunsQueryThatCapturesErrors(Context* context) {
  QUERY_BEGIN(queryThatRunsQueryThatCapturesErrors, context);
  std::string output = "(" + queryThatCapturesErrors(context) + ")";
  return QUERY_END(output);
}

static const std::string& queryThatRunsQueryThatErrors(Context* context) {
  QUERY_BEGIN(queryThatRunsQueryThatErrors, context);
  std::string output = "[" + queryThatErrors(context) + "]";
  return QUERY_END(output);
}

// What happens if a query first hides the errors, then another query doesn't?
static void test1() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // Doesn't have errors at the top level; no errors should make it to us.
  assert(queryThatCapturesCapturingErrors(context) == "[no errors on the first go; errors on the second go] error count: 1");
  assert(guard.errors().size() == 0);

  // Does have an error that was captured by the previous call, and captures
  // other errors but shouldn't report them.
  assert(queryThatCapturesErrors(context) == "no errors on the first go; errors on the second go");
  assert(guard.errors().size() == 1);

  // Now the hidden errors have been re-reported; no new errors should be reporterd
  // after the second call.
  assert(queryThatCapturesErrors(context) == "no errors on the first go; errors on the second go");
  assert(guard.errors().size() == 1);

  // This query had an error captured before, but now it should be reported.
  assert(queryThatErrors(context) == "I returned a value but I also reported an error");
  assert(guard.errors().size() == 2);

  assert(queryThatErrors(context) == "I returned a value but I also reported an error");
  assert(guard.errors().size() == 2);

  guard.realizeErrors();
}

// Same as test 1, but the un-hidden queries are invoked from other queries
static void test2() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // Doesn't have errors at the top level; no errors should make it to us.
  assert(queryThatCapturesCapturingErrors(context) == "[no errors on the first go; errors on the second go] error count: 1");
  assert(guard.errors().size() == 0);

  // Does have an error that was captured by the previous call, and captures
  // other errors but shouldn't report them.
  assert(queryThatRunsQueryThatCapturesErrors(context) == "(no errors on the first go; errors on the second go)");
  assert(guard.errors().size() == 1);

  // Now the hidden errors have been re-reported; no new errors should be reporterd
  // after the second call.
  assert(queryThatRunsQueryThatCapturesErrors(context) == "(no errors on the first go; errors on the second go)");
  assert(guard.errors().size() == 1);

  // This query had an error captured before, but now it should be reported.
  assert(queryThatRunsQueryThatErrors(context) == "[I returned a value but I also reported an error]");
  assert(guard.errors().size() == 2);

  assert(queryThatRunsQueryThatErrors(context) == "[I returned a value but I also reported an error]");
  assert(guard.errors().size() == 2);

  guard.realizeErrors();
}

// Ok, what happens if a query was already run, its errors reported, and THEN
// we ask to track how many errors it had?
static void test3() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // First, an error is immediately logged (nothing is hiding it)
  assert(queryThatErrors(context) == "I returned a value but I also reported an error");
  assert(guard.errors().size() == 1);

  // Not logged again, we cache etc.
  assert(queryThatErrors(context) == "I returned a value but I also reported an error");
  assert(guard.errors().size() == 1);

  // Okay, now run a query that tracks errors from here. It should detect errors ("errors on the second go"),
  // and also it reports an error of its own, so the error count goes to two.
  assert(queryThatCapturesErrors(context) == "no errors on the first go; errors on the second go");
  assert(guard.errors().size() == 2);

  // One more time.
  assert(queryThatCapturesErrors(context) == "no errors on the first go; errors on the second go");
  assert(guard.errors().size() == 2);

  // Okay, now run a query that tracks errors emitted by the query that tracks errors...
  // No errors should be re-emitted, and it should still detect the one error reported
  // by queryThatCapturesErrors.
  assert(queryThatCapturesCapturingErrors(context) == "[no errors on the first go; errors on the second go] error count: 1");
  assert(guard.errors().size() == 2);

  // One more time.
  assert(queryThatCapturesCapturingErrors(context) == "[no errors on the first go; errors on the second go] error count: 1");
  assert(guard.errors().size() == 2);

  guard.realizeErrors();
}

// Same as test 3, but the un-hidden queries are invoked from other queries
static void test4() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // First, an error is immediately logged (nothing is hiding it)
  assert(queryThatRunsQueryThatErrors(context) == "[I returned a value but I also reported an error]");
  assert(guard.errors().size() == 1);

  // Not logged again, we cache etc.
  assert(queryThatRunsQueryThatErrors(context) == "[I returned a value but I also reported an error]");
  assert(guard.errors().size() == 1);

  // Okay, now run a query that tracks errors from here. It should detect errors ("errors on the second go"),
  // and also it reports an error of its own, so the error count goes to two.
  assert(queryThatRunsQueryThatCapturesErrors(context) == "(no errors on the first go; errors on the second go)");
  assert(guard.errors().size() == 2);

  // One more time.
  assert(queryThatRunsQueryThatCapturesErrors(context) == "(no errors on the first go; errors on the second go)");
  assert(guard.errors().size() == 2);

  // Okay, now run a query that tracks errors emitted by the query that tracks errors...
  // No errors should be re-emitted, and it should still detect the one error reported
  // by queryThatCapturesErrors.
  assert(queryThatCapturesCapturingErrors(context) == "[no errors on the first go; errors on the second go] error count: 1");
  assert(guard.errors().size() == 2);

  // One more time.
  assert(queryThatCapturesCapturingErrors(context) == "[no errors on the first go; errors on the second go] error count: 1");
  assert(guard.errors().size() == 2);

  guard.realizeErrors();
}


static void test5() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // ================
  //   Generation 1
  // ================
  // First, an error is immediately logged (nothing is hiding it)
  assert(queryThatErrors(context) == "I returned a value but I also reported an error");
  assert(guard.errors().size() == 1);

  // Not logged again, we cache etc.
  assert(queryThatErrors(context) == "I returned a value but I also reported an error");
  assert(guard.errors().size() == 1);

  // Okay, now run a query that tracks errors from here. It should detect errors ("errors on the second go"),
  // and also it reports an error of its own, so the error count goes to two.
  assert(queryThatCapturesErrors(context) == "no errors on the first go; errors on the second go");
  assert(guard.errors().size() == 2);

  // One more time.
  assert(queryThatCapturesErrors(context) == "no errors on the first go; errors on the second go");
  assert(guard.errors().size() == 2);

  // Errors aren't safe to store between generations.
  assert(guard.realizeErrors() == 2);
  context->advanceToNextRevision(false);

  // ================
  //   Generation 2
  // ================
  // Now, run the query that tracks errors. It should capture the inner error, and report one itself.
  // This tests marking a query that emitted its errors in the previous generation
  // as needing to report them this time.
  assert(queryThatCapturesErrors(context) == "no errors on the first go; errors on the second go");
  assert(guard.errors().size() == 1);

  // One more time.
  assert(queryThatCapturesErrors(context) == "no errors on the first go; errors on the second go");
  assert(guard.errors().size() == 1);

  // Now run the error that had already reported an error last time, but hasn't this time.
  assert(queryThatErrors(context) == "I returned a value but I also reported an error");
  assert(guard.errors().size() == 2);

  // One more time.
  assert(queryThatErrors(context) == "I returned a value but I also reported an error");
  assert(guard.realizeErrors() == 2);
}

static void test6() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // ================
  //   Generation 1
  // ================
  // Run a query that tracks errors. It should detect errors ("errors on the second go"),
  // and also it reports an error of its own, so the error count goes to one.
  assert(queryThatCapturesErrors(context) == "no errors on the first go; errors on the second go");
  assert(guard.errors().size() == 1);

  // One more time.
  assert(queryThatCapturesErrors(context) == "no errors on the first go; errors on the second go");
  assert(guard.errors().size() == 1);

  // Errors aren't safe to store between generations.
  assert(guard.realizeErrors() == 1);
  context->advanceToNextRevision(false);

  // ================
  //   Generation 2
  // ================
  // Just runthe previous code again, make sure a single error is re-reported.
  // Run a query that tracks errors. It should detect errors ("errors on the second go"),
  // and also it reports an error of its own, so the error count goes to one.
  assert(queryThatCapturesErrors(context) == "no errors on the first go; errors on the second go");
  assert(guard.errors().size() == 1);

  // One more time.
  assert(queryThatCapturesErrors(context) == "no errors on the first go; errors on the second go");
  assert(guard.errors().size() == 1);

  // Errors aren't safe to store between generations.
  assert(guard.realizeErrors() == 1);
  context->advanceToNextRevision(false);
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
}
