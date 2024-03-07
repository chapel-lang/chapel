/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#define TEST_NAME(ctx__) TEST_NAME_FROM_FN_NAME(ctx__)

std::string opEquals = R"""(
    operator =(ref lhs: int, rhs: int) {
      __primitive("=", lhs, rhs);
    }
    )""";

std::string otherOps = R"""(
    operator >(ref lhs: int, rhs: int) {
      return __primitive(">", lhs, rhs);
    }
    )""";

static void testFieldUseBeforeInit1(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + R""""(
    record r {
      var x: int;
    }
    proc foo(x) do return;
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
    proc foo(x) do return;
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
    proc foo(x) do return;
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
    proc foo(x) do return;
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
    proc foo(x) do return;
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
      proc foo(x) do return;
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

static void testThisComplete(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + R""""(
    record r {
      var x: int;
      var y : int;
      var z : int;
    }
    proc r.init() {
      this.y = 5;
      this.complete();
      this.x = 10;
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

  assert(guard.errors().size() == 0);
}

static void testSecondAssign(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + R""""(
    record r {
      var x: int;
    }
    proc r.init() {
      this.x = 10;
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

  assert(guard.errors().size() == 0);
}

static void testOutOfOrder(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + R""""(
    record r {
      var x, y, z: int;
    }
    proc r.init() {
      this.z = 10;
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

  auto& msg = guard.errors()[0];
  assert(msg->message() == "Field \"x\" initialized out of order");
  assert(msg->location(ctx).firstLine() == 11);
  assert(guard.realizeErrors());
}

static void testInitCondBasic(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + R""""(
    record r {
      var x: int;
      var y : int;
      var z : int;
    }
    proc r.init(cond: bool) {
      if cond {
        this.y = 5;
      } else {
        this.x = 10;
      }
    }
    var obj = new r(false);
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(ctx, mod->id());

  assert(guard.errors().size() == 0);

  // TODO: verify resolution knows which fields need to be initialized
  // in which branches. How to make that info accessible?
}

static void testInitCondBadOrder(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + R""""(
    record r {
      var x: int;
      var y : int;
      var z : int;
    }
    proc r.init(cond: bool) {
      this.z = 5;
      if cond {
        this.x = 10;
      } else {
        this.y = 42;
      }
    }
    var obj = new r(false);
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(ctx, mod->id());

  assert(guard.errors().size() == 2);

  {
    auto& msg = guard.errors()[0];
    assert(msg->message() == "Field \"x\" initialized out of order");
    assert(msg->location(ctx).firstLine() == 14);
  }
  {
    auto& msg = guard.errors()[1];
    assert(msg->message() == "Field \"y\" initialized out of order");
    assert(msg->location(ctx).firstLine() == 16);
  }
  assert(guard.realizeErrors());
}

static void testInitCondGenericDiff(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + R""""(
    operator >(const lhs : int, const rhs : int) {
      return __primitive(">", lhs, rhs);
    }

    record R {
      type T;
      param P : int;

      proc init(type T, val : int) {
	if val > 5 {
	  this.T = T;
	  this.P = 11;
	} else {
	  this.T = T;
	  this.P = 5;
	}
      }

      proc deinit() { }
    }
    var r = new R(int, 11);
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
  assert(msg->message() == "Initializer must compute the same type in each branch");
  assert(msg->location(ctx).firstLine() == 14);
  assert(guard.realizeErrors());
}

static void testInitCondGeneric(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + R""""(
    operator >(const lhs : int, const rhs : int) {
      return __primitive(">", lhs, rhs);
    }

    record R {
      type T;
      param P : int;

      proc init(type T, val : int) {
	if val > 5 {
	  this.T = T;
	  this.P = 5;
	} else {
	  this.T = T;
	  this.P = 5;
	}
      }

      proc deinit() { }
    }
    var r = new R(int, 11);
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(ctx, mod->id());

  assert(guard.errors().size() == 0);
}

static void testInitParamCondGeneric(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = UniqueString::get(ctx, "mod");
  std::string contents = opEquals + R""""(
    operator >(const lhs : int, const rhs : int) {
      return __primitive(">", lhs, rhs);
    }

    record R {
      type T;
      param P : int;

      proc init(param cond : bool) {
	if cond {
	  this.T = real;
	  this.P = 42;
	} else {
	  this.T = int;
	  this.P = 5;
	}
      }

      proc deinit() { }
    }
    var t = new R(true);
    var f = new R(false);

    type X = R(real, 42);
    type Y = R(int, 5);
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  const ResolutionResultByPostorderID& rr = resolveModule(ctx, mod->id());

  assert(guard.errors().size() == 0);

  {
    auto t = mod->stmt(3)->toVariable();
    auto tType = rr.byAst(t).type();
    auto X = mod->stmt(5)->toVariable();
    auto XType = rr.byAst(X).type();
    assert(tType.type() == XType.type());
  }
  {
    auto f = mod->stmt(4)->toVariable();
    auto fType = rr.byAst(f).type();
    auto Y = mod->stmt(6)->toVariable();
    auto YType = rr.byAst(Y).type();
    assert(fType.type() == YType.type());
  }
}

static void testNotThisDot(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + otherOps + R""""(
    record X {
      proc type foo() {
        return 5;
      }
    }

    record R {
      var i : int;

      proc init(i = 0) {
        if X.foo() > 0 {
          this.i = 1;
        } else {
          this.i = 0;
        }
      }
    }

    var r : R;
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(ctx, mod->id());
}

static void testRelevantInit(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  //
  // Based on behavior implemented by production compiler back in:
  //   https://github.com/chapel-lang/chapel/pull/9004
  //
  // This test exists to check dyno's ability to only try resolving candidate
  // initializers that are implemented for a particular type. Without this
  // capability, the program below would fail to compile while trying to
  // resolve 'X.init' and 'R.init' for the formal 'x' in 'R.init'.
  //

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + otherOps + R""""(
    operator =(ref lhs: int, const rhs: int) {
      __primitive("=", lhs, rhs);
    }

    record X {
      var val : int;
    }

    operator =(ref lhs: X, const rhs: X) {
      lhs.val = rhs.val;
    }

    record R {
      var x : X;

      proc init(x = new X(5)) {
        this.x = x;
      }
    }

    var r: R;
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(ctx, mod->id());
}

static void testOwnedUserInit(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  // Ensure we can resolve a user-defined initializer call for an owned class,
  // which requires an implicit borrowing conversion of the receiver from owned
  // to borrowed. In particular, test this for a class with a parent class that
  // also defines an init, to ensure implicit subtype conversion to the parent
  // class doesn't make it a candidate.

  auto path = TEST_NAME(ctx);
  std::string contents = opEquals + otherOps + R""""(
    class Parent {
      proc init() {}
    }
    class Child : Parent {
      proc init() {
        super.init();
      }
    }
    var c = new owned Child();
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(ctx, mod->id());
}

// TODO:
// - test using defaults for types and params
//   - also in conditionals
// - test this.init in branches
// - test super.init in branches
// - test this.complete in branches
// - test then-only case (must know to insert 'else' branch eventually)

int main() {
  testFieldUseBeforeInit1();
  testInitReturnVoid();
  testInitReturnEarly();
  testInitThrow();
  testInitTryBang();
  testInitInsideLoops();
  testThisComplete();
  testSecondAssign();
  testOutOfOrder();

  testInitCondBasic();
  testInitCondBadOrder();
  testInitCondGenericDiff();
  testInitCondGeneric();
  testInitParamCondGeneric();

  // Tests that track old InitResolver bugs
  testNotThisDot();

  testRelevantInit();
  testOwnedUserInit();

  return 0;
}

