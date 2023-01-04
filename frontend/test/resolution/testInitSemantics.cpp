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

#include "test-resolution.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"
#include "./ErrorGuard.h"

#define TEST_NAME(ctx__)\
  chpl::UniqueString::getConcat(ctx__, __FUNCTION__, ".chpl")

std::string opEquals = R"""(
    operator =(ref lhs: int, rhs: int) {
      __primitive("=", lhs, rhs);
    }
    )""";

static const BuilderResult&
parseAndReportErrors(Context* ctx, UniqueString path) {
  auto& ret = parseFileToBuilderResult(ctx, path, UniqueString());
  for (auto& err : ret.errors()) ctx->report(err);
  return ret;
}


static void testFieldUseBeforeInit1(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + R""""(
    record r {
      var x: int;
    }
    proc foo(x) return;
    proc r.init() {
      foo(x);
      var doNotFold: bool;
      if doNotFold then foo(x); else foo(x);
      for i in 1..7 do foo(x);
      forall i in 1..7 do foo(x);
      foreach i in 1..7 do foo(x);
      this.x = 16;
      foo(x);
    }
    var obj = new r();
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(ctx, mod->id());

  assert(guard.errors().size() == 6);

  // Check the first error to see if it lines up.
  auto& msg = guard.errors()[0];
  assert(msg->message() == "'x' is used before it is initialized");
  assert(msg->location(ctx).firstLine() == 11);
  assert(guard.realizeErrors());
}

static void testInitReturnVoid(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + R""""(
    record r {
      var x: int;
    }
    proc foo(x) return;
    proc r.init() {
      this.x = 5;
      return 1;
    }
    var obj = new r();
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(ctx, mod->id());

  assert(guard.errors().size() == 1);

  // Check the first error to see if it lines up.
  auto& msg = guard.errors()[0];
  assert(msg->message() == "initializers can only return 'void'");
  assert(msg->location(ctx).firstLine() == 12);
  assert(guard.realizeErrors());
}

static void testInitReturnEarly(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + R""""(
    record r {
      var x: int;
    }
    proc foo(x) return;
    proc r.init() {
      return;
      this.x = 5;
    }
    var obj = new r();
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(ctx, mod->id());

  assert(guard.errors().size() == 1);

  // Check the first error to see if it lines up.
  auto& msg = guard.errors()[0];
  assert(msg->message() == "cannot return from initializer before initialization is complete");
  assert(msg->location(ctx).firstLine() == 11);
  assert(guard.realizeErrors());
}

static void testInitThrow(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + R""""(
    record r {
      var x: int;
    }
    proc foo(x) return;
    proc r.init() {
      this.x = 5;
      throw "test!"; // TODO: Fix this once error handling comes online
    }
    var obj = new r();
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(ctx, mod->id());

  assert(guard.errors().size() == 1);

  // Check the first error to see if it lines up.
  auto& msg = guard.errors()[0];
  assert(msg->message() == "initializers are not yet allowed to throw errors");
  assert(msg->location(ctx).firstLine() == 12);
  assert(guard.realizeErrors());
}

static void testInitTryBang(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + R""""(
    record r {
      var x: int;
    }
    proc foo(x) return;
    proc r.init() {
      this.x = 5;
      try! { foo(x); }
      catch { }
    }
    var obj = new r();
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(ctx, mod->id());

  assert(guard.errors().size() == 1);

  // Check the first error to see if it lines up.
  auto& msg = guard.errors()[0];
  assert(msg->message() == "Only catch-less try! statements are allowed in initializers for now");
  assert(msg->location(ctx).firstLine() == 12);
  assert(guard.realizeErrors());
}

static void testInitInsideLoops(void) {
  std::vector<std::string> inits = {"for i in 1..10 do",
    "forall i in 1..10 do", "coforall i in 1..10 do", "while true do",
    "[i in 1..10]", "foreach i in 1..10 do"};
  for (auto& s : inits) {
    s += " this.x = 5;\n";
  }
  inits.push_back("do { this.x = 5; } while true;\n");

  std::vector<std::string> messages(inits.size(), "cannot initialize fields inside of loops");

  inits.push_back("begin this.x = 5;");
  messages.push_back("cannot initialize fields inside begin statements");

  inits.push_back("cobegin { this.x = 5; }");
  messages.push_back("cannot initialize fields inside cobegin statements");

  for (size_t i = 0; i < inits.size(); i++) {
    auto& loop = inits[i];
    auto& message = messages[i];
    Context context;
    Context* ctx = &context;
    ErrorGuard guard(ctx);

    auto path = TEST_NAME(ctx);
    std::string contents = opEquals + R""""(
      record r {
        var x: int;
      }
      proc foo(x) return;
      proc r.init() {
      )"""" + loop + R""""(
      }
      var obj = new r();
      )"""";

    setFileText(ctx, path, contents);

    // Get the module.
    auto& br = parseAndReportErrors(ctx, path);
    assert(br.numTopLevelExpressions() == 1);
    auto mod = br.topLevelExpression(0)->toModule();
    assert(mod);

    // Resolve the module.
    std::ignore = resolveModule(ctx, mod->id());

    assert(guard.errors().size() == 1);

    // Check the first error to see if it lines up.
    auto& msg = guard.errors()[0];
    assert(msg->message() == message);
    assert(msg->location(ctx).firstLine() == 11);
    assert(guard.realizeErrors());
  }
}

int main() {
  testFieldUseBeforeInit1();
  testInitReturnVoid();
  testInitReturnEarly();
  testInitThrow();
  testInitTryBang();
  testInitInsideLoops();

  return 0;
}

