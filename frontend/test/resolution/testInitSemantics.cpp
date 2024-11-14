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

#define TEST_NAME(context__) TEST_NAME_FROM_FN_NAME(context__)

static void testFieldUseBeforeInit1(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = TEST_NAME(context);
  std::string contents = R""""(
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

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(context, mod->id());

  assert(guard.numErrors(/* countWarnings */ false) == 6);

  // Check the first error to see if it lines up.
  auto& msg = guard.errors()[0];
  assert(msg->message() == "'x' is used before it is initialized");
  assert(msg->location(context).firstLine() == 7);
  assert(guard.realizeErrors());
}

static void testInitReturnVoid(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = TEST_NAME(context);
  std::string contents = R""""(
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

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(context, mod->id());

  assert(guard.numErrors(/* countWarnings */ false) == 1);

  // Check the error (which comes last) to see if it lines up.
  auto& msg = guard.errors().back();
  assert(msg->message() == "initializers can only return 'void'");
  assert(msg->location(context).firstLine() == 8);
  assert(guard.realizeErrors());
}

static void testInitReturnEarly(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = TEST_NAME(context);
  std::string contents = R""""(
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

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(context, mod->id());

  assert(guard.errors().size() == 1);

  // Check the first error to see if it lines up.
  auto& msg = guard.errors()[0];
  assert(msg->message() == "cannot return from initializer before initialization is complete");
  assert(msg->location(context).firstLine() == 7);
  assert(guard.realizeErrors());
}

static void testInitThrow(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = TEST_NAME(context);
  std::string contents = R""""(
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

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(context, mod->id());

  assert(guard.errors().size() == 1);

  // Check the first error to see if it lines up.
  auto& msg = guard.errors()[0];
  assert(msg->message() == "initializers are not yet allowed to throw errors");
  assert(msg->location(context).firstLine() == 8);
  assert(guard.realizeErrors());
}

static void testInitTryBang(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = TEST_NAME(context);
  std::string contents = R""""(
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

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(context, mod->id());

  assert(guard.errors().size() == 1);

  // Check the first error to see if it lines up.
  auto& msg = guard.errors()[0];
  assert(msg->message() == "Only catch-less try! statements are allowed in initializers for now");
  assert(msg->location(context).firstLine() == 8);
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
    auto context = buildStdContext();
    ErrorGuard guard(context);

    auto path = TEST_NAME(context);
    std::string contents = R""""(
      record r {
        var x: int;
      }
      proc foo(x) do return;
      proc r.init() {
      )"""" + loop + R""""(
      }
      var obj = new r();
      )"""";

    setFileText(context, path, contents);

    // Get the module.
    auto& br = parseAndReportErrors(context, path);
    assert(br.numTopLevelExpressions() == 1);
    auto mod = br.topLevelExpression(0)->toModule();
    assert(mod);

    // Resolve the module.
    std::ignore = resolveModule(context, mod->id());

    assert(guard.numErrors(/* countWarnings */ false) == 1);

    // Check the error (which comes last) to see if it lines up.
    auto& msg = guard.errors().back();
    assert(msg->message() == message);
    assert(msg->location(context).firstLine() == 7);
    assert(guard.realizeErrors());
  }
}

static void testThisComplete(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = TEST_NAME(context);
  std::string contents = R""""(
    record r {
      var x : int;
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

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(context, mod->id());

  assert(guard.errors().size() == 0);
}

static void testSecondAssign(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = TEST_NAME(context);
  std::string contents = R""""(
    record r {
      var x: int;
    }
    proc r.init() {
      this.x = 10;
      this.x = 5;
    }
    var obj = new r();
    )"""";

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(context, mod->id());

  assert(guard.errors().size() == 0);
}

static void testOutOfOrder(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = TEST_NAME(context);
  std::string contents = R""""(
    record r {
      var x, y, z: int;
    }
    proc r.init() {
      this.z = 10;
      this.x = 5;
    }
    var obj = new r();
    )"""";

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(context, mod->id());

  assert(guard.errors().size() == 1);

  auto& msg = guard.errors()[0];
  assert(msg->message() == "Field \"x\" initialized out of order");
  assert(msg->location(context).firstLine() == 7);
  assert(guard.realizeErrors());
}

static void testInitCondBasic(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = TEST_NAME(context);
  std::string contents = R""""(
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

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(context, mod->id());

  assert(guard.errors().size() == 0);

  // TODO: verify resolution knows which fields need to be initialized
  // in which branches. How to make that info accessible?
}

static void testInitCondBadOrder(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = TEST_NAME(context);
  std::string contents = R""""(
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

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(context, mod->id());

  assert(guard.errors().size() == 2);

  {
    auto& msg = guard.errors()[0];
    assert(msg->message() == "Field \"x\" initialized out of order");
    assert(msg->location(context).firstLine() == 10);
  }
  {
    auto& msg = guard.errors()[1];
    assert(msg->message() == "Field \"y\" initialized out of order");
    assert(msg->location(context).firstLine() == 12);
  }
  assert(guard.realizeErrors());
}

static void testInitCondGenericDiff(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = TEST_NAME(context);
  std::string contents = R""""(
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

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(context, mod->id());

  assert(guard.errors().size() == 1);

  // Check the first error to see if it lines up.
  auto& msg = guard.errors()[0];
  assert(msg->message() == "Initializer must compute the same type in each branch");
  assert(msg->location(context).firstLine() == 6);
  assert(guard.realizeErrors());
}

static void testInitCondGeneric(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = TEST_NAME(context);
  std::string contents = R""""(
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

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(context, mod->id());

  assert(guard.errors().size() == 0);
}

static void testInitParamCondGeneric(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "mod");
  std::string contents = R""""(
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

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  const ResolutionResultByPostorderID& rr = resolveModule(context, mod->id());

  assert(guard.errors().size() == 0);

  {
    auto t = mod->stmt(1)->toVariable();
    auto tType = rr.byAst(t).type();
    auto X = mod->stmt(3)->toVariable();
    auto XType = rr.byAst(X).type();
    assert(tType.type() == XType.type());
  }
  {
    auto f = mod->stmt(2)->toVariable();
    auto fType = rr.byAst(f).type();
    auto Y = mod->stmt(4)->toVariable();
    auto YType = rr.byAst(Y).type();
    assert(fType.type() == YType.type());
  }
}

static void testNotThisDot(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = TEST_NAME(context);
  std::string contents = R""""(
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

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(context, mod->id());
}

static void testRelevantInit(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  //
  // Based on behavior implemented by production compiler back in:
  //   https://github.com/chapel-lang/chapel/pull/9004
  //
  // This test exists to check dyno's ability to only try resolving candidate
  // initializers that are implemented for a particular type. Without this
  // capability, the program below would fail to compile while trying to
  // resolve 'X.init' and 'R.init' for the formal 'x' in 'R.init'.
  //

  auto path = TEST_NAME(context);
  std::string contents = R""""(
    record X {
      var val : int;
    }

    record R {
      var x : X;

      proc init(x = new X(5)) {
        this.x = x;
      }
    }

    var r: R;
    )"""";

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(context, mod->id());
}

static void testOwnedUserInit(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  // Ensure we can resolve a user-defined initializer call for an owned class,
  // which requires an implicit borrowing conversion of the receiver from owned
  // to borrowed. In particular, test this for a class with a parent class that
  // also defines an init, to ensure implicit subtype conversion to the parent
  // class doesn't make it a candidate.

  auto path = TEST_NAME(context);
  std::string contents = R""""(
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

  setFileText(context, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(context, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(context, mod->id());
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
    auto context = buildStdContext();
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
    auto context = buildStdContext();
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
    auto context = buildStdContext();
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
    auto context = buildStdContext();
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
    auto context = buildStdContext();
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

static void testUseAfterInit() {
  std::string program = R"""(
    class PointDoubleX {
      var a, b : real;

      proc init(a: real, b: real) {
        this.a = a;              // initialization

        this.a = 5.0;              // assignment

        var c = this.a * 2;      // use of initialized field

        this.b = b;              // initialization
      }
    }

    var x = new PointDoubleX(1.0, 2.0);
  )""";

  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::ignore = resolveTypeOfX(context, program);
}

static void testInitEqOther(void) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program = R"""(
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

  auto results = resolveTypesOfVariables(context, program, {"x", "y"});
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
    auto context = buildStdContext();
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
    auto context = buildStdContext();
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
    auto context = buildStdContext();
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
    auto context = buildStdContext();
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
    auto context = buildStdContext();
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
    auto context = buildStdContext();
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

  // Allow parent field access with implicit super.init
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
      class Parent { var x : int; }
      class Child : Parent { var y : real; }

      proc Child.init() {
        this.y = x * 42.0;
      }
      var a = new Child();
    )""";

    auto m = parseModule(context, std::move(program));
    std::ignore = resolveModule(context, m->id());
  }

  // Error for accessing parent field before super.init
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
      class Parent { var x : int; }
      class Child : Parent { var y : real; }

      proc Child.init() {
        this.x = 42;
        var dummy = this.x * 0.0;
        var other = x * 0.0;
        super.init(0);
        this.y = x * 42.0;
      }
      var a = new Child();
    )""";

    auto m = parseModule(context, std::move(program));
    std::ignore = resolveModule(context, m->id());

    assert(guard.numErrors() == 3);

    auto check = [&context] (const owned<ErrorBase>& err, std::string pid) {
      auto msg = R"""(Cannot access parent field "x" before super.init() or this.init())""";
      assert(err->message() == msg);
      assert(err->toErrorMessage(context).id().str() == pid);
    };

    check(guard.error(0), "input.init@3");
    check(guard.error(1), "input.init@7");
    check(guard.error(2), "input.init@11");

    guard.realizeErrors();
  }

  // Basic generic case
  {
    auto context = buildStdContext();
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
    auto context = buildStdContext();
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
    auto context = buildStdContext();
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

  // Default initializer
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
      class A {
        type TA;
        var a : TA;
      }
      class B : A(?) {
        type TB;
        var b : TB;
      }

      var x = new B(int, 1, real, 42.0);
      )""";

    auto xt = resolveTypeOfX(context, program);

    std::stringstream ss;
    xt->stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
    assert(ss.str() == "owned B(int(64), real(64))");
  }

  // Default initializer when parent has user-defined initializer
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
      class A {
        var x : int;

        proc init(x: int = 0) {
          this.x = x;
        }
      }

      class B : A {
        var y : string;
      }

      var b1 = new B();
      var b2 = new B("test");
      )""";

    auto vars = resolveTypesOfVariables(context, program, {"b1", "b2"});
    auto b1 = vars["b1"].type();
    auto b2 = vars["b2"].type();

    auto check = [] (const Type* type) {
      std::stringstream ss;
      type->stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
      assert(ss.str() == "owned B");
    };

    check(b1);
    check(b2);
  }

  // Default initializer when grandparent has user-defined initializer
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
      class X {
        var one : int;

        proc init(one: int = 0) {
          this.one = one;
        }
      }

      class Y : X {
        var two : real;
      }

      class Z : Y {
        var three : string;
      }

      var z1 = new Z();
      var z2 = new Z(42.0);
      var z3 = new Z(42.0, "test");
      )""";

    auto vars = resolveTypesOfVariables(context, program, {"z1", "z2", "z3"});
    auto z1 = vars["z1"].type();
    auto z2 = vars["z2"].type();
    auto z3 = vars["z3"].type();

    auto check = [] (const Type* type) {
      std::stringstream ss;
      type->stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
      assert(ss.str() == "owned Z");
    };

    check(z1);
    check(z2);
    check(z3);
  }

  // Make sure that existence of an interface in the inherit-exprs list
  // does not cause a super.init call to be generated.
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
      interface myInterface {}

      class C : myInterface {
        var x : string;
      }

      var c = new C();
      )""";

    auto m = parseModule(context, std::move(program));
    std::ignore = resolveModule(context, m->id());
  }
}

static void testImplicitSuperInit() {
  // Ensure we resolve the body of implicit super.init() calls
  {
    // use 'test' to ensure 'q' has the right type
    std::string program = R"""(
      proc test(arg: uint) {}

      class A {
        type T = string;
        var aa : T;

        proc init(type T = int) {
          this.T = uint;
        }
      }

      class B : A(?) {
        var bb : real;

        // implicit super.init through Dot
        proc init() {
          var q = this.aa;
          test(q);

          this.bb = 42.0;
        }

        // implicit super.init through Identifier
        proc init(dummy:string) {
          var q = aa;
          test(q);

          this.bb = 42.0;
        }
      }

      var x = new B();
      var y = new B("");
    )""";

    auto context = buildStdContext();
    ErrorGuard guard(context);

    auto vars = resolveTypesOfVariables(context, program, {"x", "y"});

    auto check = [] (QualifiedType qt) {
      auto t = qt.type();
      assert(t);
      assert(t->isClassType());

      std::stringstream ss;
      t->stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
      assert(ss.str() == "owned B(uint(64))");
    };

    check(vars["x"]);
    check(vars["y"]);
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

    auto context = buildStdContext();
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

    auto context = buildStdContext();
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

static std::string toString(QualifiedType type) {
  std::stringstream ss;
  type.type()->stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
  return ss.str();
}

static void testNilFieldInit() {
  std::string program = R"""(
    class C { var x: int; }
    record R {
      var x: unmanaged C?;
      proc init() {
        x = nil;
      }
    }

    // exists to work around current lack of default-init at module scope
    proc test() {
      var x: R;
      return x;
    }
    var a = test();
    var b = new R();
  )""";
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context, program, {"a", "b"});

  assert(toString(vars["a"]) == "R");
  assert(toString(vars["b"]) == "R");
}

static void testGenericFieldInit() {
  {
    std::string program = R"""(
      record R {
        var x : integral;

        proc init(arg) {
          this.x = arg;
        }
      }

      var a = new R(5);
      var b = new R(10:uint);
      var c = new R("test");
      )""";

    Context* context = buildStdContext();
    ErrorGuard guard(context);

    auto vars = resolveTypesOfVariables(context, program, {"a", "b", "c"});

    assert(guard.numErrors() == 1);
    auto& err = guard.error(0);
    assert(err->type() == ErrorType::IncompatibleTypeAndInit);
    assert(err->location(context).firstLine() == 6);

    // Note: These type strings are not stabilized
    assert(toString(vars["a"]) == "R(var int(64))");
    assert(toString(vars["b"]) == "R(var uint(64))");
    assert(toString(vars["c"]) == "R(var ErroneousType)");

    guard.realizeErrors();
  }
  {
    std::string program = R"""(
      class C {
        type typeField;
      }

      record R {
        var myC: owned C(?)?;

        proc init() {
          this.myC = nil;
        }
      }

      var r  = new R();
      )""";
    Context* context = buildStdContext();
    ErrorGuard guard(context);

    auto vars = resolveTypesOfVariables(context, program, {"r"});
    assert(toString(vars["r"]) == "R(var ErroneousType)");

    // TODO: error message here says 'owned C?' and 'nil' are incompatible,
    // but should be more specific about the genericity being the problem.
    assert(guard.numErrors() == 1);
    auto& err = guard.error(0);
    assert(err->type() == ErrorType::IncompatibleTypeAndInit);
    assert(err->location(context).firstLine() == 10);

    guard.realizeErrors();
  }
  {
    printf("one\n");
    std::string program = R"""(
      record G { type T; var x : T; }

      proc G.init=(other: this.type) {
        this.T = other.T;
        this.x = other.x;
      }

      proc G.init=(other: ?t) where t != int {
        this.T = other.type;
        this.x = other;
      }

      // just to demonstrate we can specify a concrete formal
      proc G.init=(other: int) {
        this.T = other.type;
        this.x = other;
      }

      record R {
        var x : G(?);

        proc init(arg) {
          this.x = arg;
        }
      }

      proc test(arg) {
        var x = new R(arg);
        return x;
      }

      var a = test(5);
      var b = test(10:uint);
      var c = test("test");
      )""";

    auto context = buildStdContext();
    ErrorGuard guard(context);

    auto vars = resolveTypesOfVariables(context, program, {"a", "b", "c"});

    // Note: These type strings are not stabilized
    assert(toString(vars["a"]) == "R(var G(int(64)))");
    assert(toString(vars["b"]) == "R(var G(uint(64)))");
    assert(toString(vars["c"]) == "R(var G(string))");
  }
  {
    std::string program = R"""(
      record G { type T; var x : T; }

      proc G.init=(other: this.type) {
        this.T = other.T;
        this.x = other.x;
      }

      proc G.init=(other: ?t) {
        this.T = other.type;
        this.x = other;
      }

      class C { var x : int; }

      record R {
        var x: G(?),
            y: unmanaged C?,
            z: G(?);

        proc init(arg) {
          this.x = arg;
          this.y = nil;
          this.z = 42.0;
        }
      }

      var x = new R(5);
    )""";

    Context ctx;
    Context* context = &ctx;
    ErrorGuard guard(context);

    auto x = resolveQualifiedTypeOfX(context, program);
    assert(toString(x) == "R(var G(int(64)), var G(real(64)))");
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

  testUseAfterInit();

  testInitEqOther();

  testInheritance();

  testImplicitSuperInit();

  testInitGenericAfterConcrete();

  testNilFieldInit();

  testGenericFieldInit();

  return 0;
}

