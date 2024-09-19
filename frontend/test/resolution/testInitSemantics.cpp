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

  assert(guard.numErrors(/* countWarnings */ false) == 6);

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

  assert(guard.numErrors(/* countWarnings */ false) == 1);

  // Check the error (which comes last) to see if it lines up.
  auto& msg = guard.errors().back();
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

    assert(guard.numErrors(/* countWarnings */ false) == 1);

    // Check the error (which comes last) to see if it lines up.
    auto& msg = guard.errors().back();
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

// Replaces a placeholder with possible values to help test more programs.
// If multuple placeholders are present, tests all combinations.
static std::vector<std::string> getVersionsWithTypes(const std::string& prog,
                                                     const std::string& placeholder,
                                                     const std::vector<std::string> types) {
  std::vector<std::vector<std::string>> variants;

  std::vector<size_t> occurrences;
  size_t start = 0;
  while ((start = prog.find(placeholder, start)) != std::string::npos) {
    occurrences.push_back(start);
    start += placeholder.length();
  }

  std::reverse(occurrences.begin(), occurrences.end());
  std::vector<std::string> programs = { prog };
  std::vector<std::string> programsNext;

  for (auto occurrence : occurrences) {
    for (auto& program : programs) {
      for (auto& type : types) {
        std::string newProg = program;
        newProg.replace(occurrence, placeholder.length(), type);
        programsNext.push_back(newProg);
      }
    }
    programs = std::move(programsNext);
  }

  return programs;
}

static std::vector<std::string> getAllVersions(const std::string& prog) {
  // Note, const checker currently balks at non-'this' calls to 'init'.
  std::vector<std::string> initProgs =
    getVersionsWithTypes(prog, "INIT", { "this.init", "init" });

  std::vector<std::string> allProgs;
  for (auto initProg : initProgs) {
    auto versions = getVersionsWithTypes(initProg, "AGGREGATE", { "record", "class" });
    allProgs.insert(allProgs.end(), versions.begin(), versions.end());
  }

  return allProgs;
}

static void testInitFromInit(void) {
  std::string prog =
      R"""(
      AGGREGATE pair {
        type fst;
        type snd;

        proc init(type fst, type snd) {
          this.fst = fst;
          this.snd = snd;
        }

        proc init(type both) {
          INIT(both, (both, both));
        }
      }

      var x = new pair(int);
      )""";

  for (auto version : getAllVersions(prog)) {
    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);
    auto qt = resolveTypeOfXInit(context, version);

    assert(qt.type());
    auto recType = qt.type()->getCompositeType();
    assert(recType);
    assert(recType->name() == "pair");

    auto fields = fieldsForTypeDecl(context, recType, DefaultsPolicy::IGNORE_DEFAULTS);

    assert(fields.fieldName(0) == "fst");
    assert(fields.fieldName(1) == "snd");

    auto fstType = fields.fieldType(0);
    assert(fstType.type());
    assert(fstType.type()->isIntType());

    auto sndType = fields.fieldType(1);
    assert(sndType.type());
    auto sndTypeTup = sndType.type()->toTupleType();
    assert(sndTypeTup);

    auto fstTupEltType = sndTypeTup->elementType(0);
    assert(fstTupEltType.type());
    assert(fstTupEltType.type()->isIntType());
    auto sndTupEltType = sndTypeTup->elementType(1);
    assert(sndTupEltType.type());
    assert(sndTupEltType.type()->isIntType());
  }
}

static void testInitInParamBranchFromInit(void) {
  // test calling 'this.init' from another initializer.
  std::string prog =
      R"""(
      AGGREGATE pair {
        type fst;
        type snd;

        proc init(type fst, type snd) {
          this.fst = fst;
          this.snd = snd;
        }

        proc init(param cond) {
          if cond {
            INIT(int, (int, int));
          } else {
            this.fst = bool;
            this.snd = bool;
          }
        }
      }

      var x = new pair(true);
      var y = new pair(false);
      )""";


  for (auto version : getAllVersions(prog)) {
    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);

    auto qts = resolveTypesOfVariables(context, version, {"x", "y"});


    {
      auto qt = qts.at("x");
      assert(qt.type());
      auto recType = qt.type()->getCompositeType();
      assert(recType);
      assert(recType->name() == "pair");

      auto fields = fieldsForTypeDecl(context, recType, DefaultsPolicy::IGNORE_DEFAULTS);

      assert(fields.fieldName(0) == "fst");
      assert(fields.fieldName(1) == "snd");

      auto fstType = fields.fieldType(0);
      assert(fstType.type());
      assert(fstType.type()->isIntType());

      auto sndType = fields.fieldType(1);
      assert(sndType.type());
      auto sndTypeTup = sndType.type()->toTupleType();
      assert(sndTypeTup);

      auto fstTupEltType = sndTypeTup->elementType(0);
      assert(fstTupEltType.type());
      assert(fstTupEltType.type()->isIntType());
      auto sndTupEltType = sndTypeTup->elementType(1);
      assert(sndTupEltType.type());
      assert(sndTupEltType.type()->isIntType());
    }
    {
      auto qt = qts.at("y");
      assert(qt.type());
      auto recType = qt.type()->getCompositeType();
      assert(recType);
      assert(recType->name() == "pair");

      auto fields = fieldsForTypeDecl(context, recType, DefaultsPolicy::IGNORE_DEFAULTS);

      assert(fields.fieldName(0) == "fst");
      assert(fields.fieldName(1) == "snd");

      auto fstType = fields.fieldType(0);
      assert(fstType.type());
      assert(fstType.type()->isBoolType());
      auto sndType = fields.fieldType(1);
      assert(sndType.type());
      assert(sndType.type()->isBoolType());
    }
  }
}

static void testInitInBranchFromInit(void) {
  std::string prog =
      R"""(
      AGGREGATE pair {
        type fst;
        type snd;

        proc init(type fst, type snd) {
          this.fst = fst;
          this.snd = snd;
        }

        proc init(cond: bool) {
          if cond {
            INIT(int, (int, int));
          } else {
            this.fst = int;
            this.snd = (int, int);
          }
        }
      }

      var x = new pair(true);
      var y = new pair(false);
      )""";

  for (auto version : getAllVersions(prog)) {
    // test calling 'this.init' from another initializer.
    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);

    auto qts = resolveTypesOfVariables(context, version, {"x", "y"});


    {
      auto qt = qts.at("x");
      assert(qt.type());
      auto recType = qt.type()->getCompositeType();
      assert(recType);
      assert(recType->name() == "pair");

      auto fields = fieldsForTypeDecl(context, recType, DefaultsPolicy::IGNORE_DEFAULTS);

      assert(fields.fieldName(0) == "fst");
      assert(fields.fieldName(1) == "snd");

      auto fstType = fields.fieldType(0);
      assert(fstType.type());
      assert(fstType.type()->isIntType());

      auto sndType = fields.fieldType(1);
      assert(sndType.type());
      auto sndTypeTup = sndType.type()->toTupleType();
      assert(sndTypeTup);

      auto fstTupEltType = sndTypeTup->elementType(0);
      assert(fstTupEltType.type());
      assert(fstTupEltType.type()->isIntType());
      auto sndTupEltType = sndTypeTup->elementType(1);
      assert(sndTupEltType.type());
      assert(sndTupEltType.type()->isIntType());
    }
    {
      auto qt = qts.at("y");
      assert(qt.type());
      auto recType = qt.type()->getCompositeType();
      assert(recType);
      assert(recType->name() == "pair");

      auto fields = fieldsForTypeDecl(context, recType, DefaultsPolicy::IGNORE_DEFAULTS);

      assert(fields.fieldName(0) == "fst");
      assert(fields.fieldName(1) == "snd");

      auto fstType = fields.fieldType(0);
      assert(fstType.type());
      assert(fstType.type()->isIntType());

      auto sndType = fields.fieldType(1);
      assert(sndType.type());
      auto sndTypeTup = sndType.type()->toTupleType();
      assert(sndTypeTup);

      auto fstTupEltType = sndTypeTup->elementType(0);
      assert(fstTupEltType.type());
      assert(fstTupEltType.type()->isIntType());
      auto sndTupEltType = sndTypeTup->elementType(1);
      assert(sndTupEltType.type());
      assert(sndTupEltType.type()->isIntType());
    }
  }
}

static void testBadInitInBranchFromInit(void) {
  std::string prog =
      R"""(
      AGGREGATE pair {
        type fst;
        type snd;

        proc init(type fst, type snd) {
          this.fst = fst;
          this.snd = snd;
        }

        proc init(cond: bool) {
          if cond {
            INIT(bool, (bool, bool));
          } else {
            this.fst = int;
            this.snd = (int, int);
          }
        }
      }

      var x = new pair(true);
      )""";

  for (auto version : getAllVersions(prog)) {
    // test calling 'this.init' from another initializer.
    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);

    std::ignore = resolveTypeOfXInit(context, version);

    // The above is invalid, since the two branches result in a different type.
    assert(guard.realizeErrors() == 1);
  }
}

static void testAssignThenInit(void) {
  std::string prog =
      R"""(
      AGGREGATE pair {
        type fst;
        type snd;

        proc init(type fst, type snd) {
          this.fst = fst;
          this.snd = snd;
        }

        proc init(type both) {
          this.fst = both;
          INIT(both, (both, both));
        }
      }

      var x = new pair(int);
      )""";

  for (auto version : getAllVersions(prog)) {
    // test calling 'this.init' from another initializer.
    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);

    std::ignore = resolveTypeOfXInit(context, version);


    // Can't initialize 'fst' then call initializer. Due to
    //
    //  https://github.com/chapel-lang/chapel/issues/24900
    //
    // The errors are currently issued twice.
    assert(guard.realizeErrors() == 1);
  }
}

static void testInitEqOther(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  std::string program = R"""(
    operator =(ref lhs: numeric, const in rhs: numeric) {
      __primitive("=", lhs, rhs);
    }
    record R {
      type T;
      var field : T;
    }
    proc R.init(type T, field = 0) {
      this.T = T;
      this.field = field;
    }
    proc R.init=(other: ?) {
      this.T = other.type;
      this.field = other;
    }
    var x:R(?) = 4;
    var y:R(?) = 42.0;
  )""";

  auto results = resolveTypesOfVariables(ctx, program, {"x", "y"});
  auto xt = results["x"];
  assert(xt.type()->isRecordType());
  std::stringstream ss;
  xt.type()->stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
  assert(ss.str() == "R(int(64))");

  ss.str(""); ss.clear();

  auto yt = results["y"];
  assert(yt.type()->isRecordType());
  yt.type()->stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
  assert(ss.str() == "R(real(64))");
}

static void testInheritance() {
  std::string parentChild = R"""(
    class Parent {
      var x : int;
    }

    class Child : Parent {
      var y : string;
    }
  )""";

  std::string grandparent = R"""(
    class A {
      var x : int;
    }
    class B : A {
      var y : real;
    }
    class C : B {
      var z : string;
    }
  )""";

  // basic usage
  {
    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);

    std::string program = parentChild + grandparent + R"""(
      var a = new Child(1, "hello");
      var b = new Child(1);
      var c = new Child();

      var d = new C(1, 42.0, "hello");
      var e = new C(1, 42.0);
      var f = new C(1);
      var g = new C();
    )""";

    auto m = parseModule(context, std::move(program));
    std::ignore = resolveModule(context, m->id());
  }

  // named arguments
  {
    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);

    std::string program = parentChild + grandparent + R"""(
      var a = new Child("hello", x=1);
      var b = new Child(x=1);

      var c = new C("hello", x=1, y=42.0);
      var d = new C(z="hello", x=1);
      var e = new C(z="hello", y=42.0);
      var f = new C(x=1);
      var g = new C(y=42.0);
    )""";

    auto m = parseModule(context, std::move(program));
    std::ignore = resolveModule(context, m->id());
  }

  // user-defined parent initializer
  {
    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);

    std::string program = parentChild + R"""(
    proc Parent.init() { this.x = 1; }

    var a = new Child("hello");
    var b = new Child();
    )""";

    auto m = parseModule(context, std::move(program));
    std::ignore = resolveModule(context, m->id());
  }
  {
    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);

    std::string program = grandparent + R"""(
    proc A.init() { this.x = 1; }

    var a = new C(42.0, "hello");
    var b = new C(42.0);
    var c = new C();
    )""";

    auto m = parseModule(context, std::move(program));
    std::ignore = resolveModule(context, m->id());
  }
  {
    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);

    std::string program = grandparent + R"""(
    proc B.init() { this.y = 42.0; }

    var a = new C("hello");
    var b = new C();
    )""";

    auto m = parseModule(context, std::move(program));
    std::ignore = resolveModule(context, m->id());
  }

  // super.init example
  {
    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);

    std::string program = grandparent + R"""(
      proc C.init(x: int = 0, y: real = 0.0, z: string = "") {
        super.init(x, y);
        this.z = z;
      }
      var a = new C(5, 42.0, "test");
    )""";

    auto m = parseModule(context, std::move(program));
    std::ignore = resolveModule(context, m->id());
  }

  // Basic generic case
  {
    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);

    std::string program = R"""(
      class Parent {
        type A;
      }

      class Child : Parent {
        type B;

        proc init(type A, type B) {
          super.init(A);
          this.B = B;
        }

        proc helper() { return "test"; }
      }

      var x = new Child(int, string);
      var y = x.helper();
    )""";

    auto vars = resolveTypesOfVariables(context, program, {"x", "y"});
    auto x = vars["x"];
    auto y = vars["y"];

    std::stringstream ss;
    x.type()->stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
    assert(ss.str() == "owned Child(int(64), string)");

    assert(y.type()->isStringType());
  }

  // Generic parent, concrete child
  {
    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);

    std::string program = R"""(
      class Parent {
        type A;
      }

      class Child : Parent {

        proc init(type A) {
          super.init(A);
        }

        proc doNothing() {}
      }

      var x = new Child(int);

      // ensure we can call a method on this receiver type after init
      x.doNothing();
    )""";

    auto vars = resolveTypesOfVariables(context, program, {"x"});
    auto x = vars["x"];

    std::stringstream ss;
    x.type()->stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
    assert(ss.str() == "owned Child(int(64))");
  }

  // Generic grandparent, concrete parent, concrete child
  {
    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);

    std::string program = R"""(
      class Grandparent {
        type A;
      }

      class Parent : Grandparent {
        proc init(type A) {
          super.init(A);
        }
      }

      class Child : Parent {

        proc init(type A) {
          super.init(A);
        }

        proc doNothing() {}
      }

      var x = new Child(int);

      // ensure we can call a method on this receiver type after init
      x.doNothing();
    )""";

    auto vars = resolveTypesOfVariables(context, program, {"x"});
    auto x = vars["x"];

    std::stringstream ss;
    x.type()->stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
    assert(ss.str() == "owned Child(int(64))");
  }
}

static void testInitGenericAfterConcrete() {
  // With generic var initialized properly
  {
    std::string program = R"""(
      record Foo {
        var a:int;
        var b;
        proc init() {
          this.a = 1;
          this.b = 2;
        }
      }

      var myFoo = new Foo();
      var x = myFoo.b;
    )""";

    Context ctx;
    Context* context = &ctx;
    auto t = resolveTypeOfX(context, program);

    assert(t);
    assert(t->isIntType());
  }

  // With generic var not initialized, so not enough info
  {
    std::string program = R"""(
      record Foo {
        var a:int;
        var b;
        proc init() {
          this.a = 1;
        }
      }

      var myFoo = new Foo();
      var x = myFoo.b;
    )""";

    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);
    auto t = resolveTypeOfX(context, program);

    assert(t);
    assert(t->isUnknownType());

    assert(guard.errors().size() == 1);
    assert(guard.error(0)->message() ==
           "unable to instantiate generic type from initializer");
    assert(guard.realizeErrors());
  }
}

// TODO:
// - test using defaults for types and params
//   - also in conditionals
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

  testInitFromInit();
  testInitInParamBranchFromInit();
  testInitInBranchFromInit();
  testBadInitInBranchFromInit();
  testAssignThenInit();

  testInitEqOther();

  testInheritance();

  testInitGenericAfterConcrete();

  return 0;
}

