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

#include "chpl/resolution/call-graph.h"
#include "test-resolution.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"
#include "./ErrorGuard.h"

#define ADVANCE_PRESERVING_STANDARD_MODULES_(ctx__) \
  do { \
    ctx__->advanceToNextRevision(false); \
    setupModuleSearchPaths(ctx__, false, false, {}, {}); \
  } while (0)

static Context*
turnOnWarnUnstable(Context* ctx) {
  // TODO: Turn on after infinite cycle bug in 'recomputeIfNeeded' is fixed,
  // see private issue #6721.
  /*
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  CompilerFlags flags;
  flags.set(CompilerFlags::WARN_UNSTABLE, true);
  setCompilerFlags(ctx, std::move(flags));
  assert(isCompilerFlagSet(ctx, CompilerFlags::WARN_UNSTABLE));
  */
  return ctx;
}

// This test demonstrates that it is safe to resolve a nested function like
// normal if the function does not refer to outer variables, regardless of
// whether or not the function is generic or concrete.
static void test0(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  // This snippet is a mockup of code from the internal modules.
  std::string program =
    R""""(
    proc isEnumType(type t) param {
      proc isEnumHelp(type t: enum) param do return true;
      proc isEnumHelp(type t) param do return false;
      return isEnumHelp(t);
    }

    enum colors { red, green, blue }

    proc foo(arg) {
      if isEnumType(arg.type) {
        return 42;
      } else {
        return "hello";
      }
    }

    var color = colors.red;
    var x = foo(color);
    )"""";

  auto qt = resolveQualifiedTypeOfX(ctx, program);
  assert(!guard.realizeErrors());
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type() && qt.type()->isIntType());
}

// Very simple test exercising nested functions with outer variables.
static void test1(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R""""(
    proc foo() {
      const a: real;
      proc bar() { return a; }
      return bar();
    }
    var x = foo();
    )"""";

  auto qt = resolveQualifiedTypeOfX(ctx, program);
  assert(!guard.realizeErrors());
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type() && qt.type()->isRealType());
}

// We should be able to call the sibling nested function 'ding' within the
// child nested function 'baz'. The nested functions use outer variables in
// their bodies _and_ their signatures.
static void test2(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R""""(
    proc foo(param x) {
      param y = 16;
      type z = x.type;
      var a: int = 0;

      proc ding(b: z) { x; y; z; a; b; return b; }

      proc bar() {
        var b: z;
        var c = "foo";

        proc baz(param g: b.type, h: c.type) where y == 16 {
          x; y; z; a; b; c; g; h;
          return ding(b);
        }

        return baz(x, "foo");
      }

      return bar();
    }

    enum e { a, b, c }

    var x0 = foo(8);
    var x1 = foo(8.0);
    var x2 = foo("8");
    var x3 = foo(e.b);
    )"""";

  auto m = resolveTypesOfVariables(ctx, program, { "x0", "x1", "x2", "x3" });
  assert(!guard.realizeErrors());

  auto& x0 = m["x0"];
  assert(x0.kind() == QualifiedType::VAR);
  assert(x0.type() && x0.type()->isIntType());
  auto& x1 = m["x1"];
  assert(x1.kind() == QualifiedType::VAR);
  assert(x1.type() && x1.type()->isRealType());
  auto& x2 = m["x2"];
  assert(x2.kind() == QualifiedType::VAR);
  assert(x2.type() && x2.type()->isStringType());
  auto& x3 = m["x3"];
  assert(x3.kind() == QualifiedType::VAR);
  assert(x3.type() && x3.type()->isEnumType());
}

// This goofy test tries to make sure that nested functions in instantiations
// behave properly. The outermost function 'foo' has multiple instantiations,
// which 'baz' is indirectly dependent on (uses of the outer variable 'a').
// Then 'baz' is used to instantiate the nested function 'bar'.
static void test3(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R""""(
    proc foo(type a) {
      const v0 = 0;
      const v1 = 0.0;
      const v2 = "0";
      const v3 = new shared C?();
      const v4 = new r();
      const v5 = e.a;
      proc bar(param n) {
        var tup: (v0.type, v1.type, v2.type, v3.type, v4.type, v5.type);
        return tup[n];
      }
      proc baz() {
        if a == v0.type then return bar(0);
        else if a == v1.type then return bar(1);
        else if a == v2.type then return bar(2);
        else if a == v3.type then return bar(3);
        else if a == v4.type then return bar(4);
        else if a == v5.type then return bar(5);
        else return 8000;
      }
      return baz();
    }
    enum e { a, b, c }
    record r {}
    class C {}
    var x0 = foo(int);
    var x1 = foo(real);
    var x2 = foo(string);
    var x3 = foo(shared C?);
    var x4 = foo(r);
    var x5 = foo(e);
    )"""";

  auto m = resolveTypesOfVariables(ctx, program, { "x0", "x1", "x2", "x3", "x4", "x5"});
  assert(!guard.realizeErrors());
  auto& x0 = m["x0"];
  assert(x0.kind() == QualifiedType::VAR);
  assert(x0.type() && x0.type()->isIntType());
  auto& x1 = m["x1"];
  assert(x1.kind() == QualifiedType::VAR);
  assert(x1.type() && x1.type()->isRealType());
  auto& x2 = m["x2"];
  assert(x2.kind() == QualifiedType::VAR);
  assert(x2.type() && x2.type()->isStringType());
  auto& x3 = m["x3"];
  assert(x3.kind() == QualifiedType::VAR);
  assert(x3.type() && x3.type()->isClassType());
  auto& x4 = m["x4"];
  assert(x4.kind() == QualifiedType::VAR);
  assert(x4.type() && x4.type()->isRecordType());
  auto& x5 = m["x5"];
  assert(x5.kind() == QualifiedType::VAR);
  assert(x5.type() && x5.type()->isEnumType());
}

// Interaction between outer variables and generic constraints.
static void test4(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R""""(
    // Toggle to constrain.
    proc foo(a, param x: bool) {
      proc bar(b: if x then integral else ?) { return b; }
      return bar(a);
    }
    var x0 = foo(0, false);
    var x1 = foo("0", false);
    var x2 = foo(0, true);
    var x3 = foo("0", true);
    )"""";

  auto m = resolveTypesOfVariables(ctx, program, { "x0", "x1", "x2", "x3" });

  assert(guard.numErrors() == 2);
  auto& e = guard.errors();
  assert(e[0]->type() == chpl::NoMatchingCandidates);
  assert(e[1]->message() == "Cannot establish type for call expression");
  guard.clearErrors();

  auto& x0 = m["x0"];
  assert(x0.kind() == QualifiedType::VAR);
  assert(x0.type() && x0.type()->isIntType());
  auto& x1 = m["x1"];
  assert(x1.kind() == QualifiedType::VAR);
  assert(x1.type() && x1.type()->isStringType());
  auto& x2 = m["x2"];
  assert(x2.kind() == QualifiedType::VAR);
  assert(x2.type() && x2.type()->isIntType());
  auto& x3 = m["x3"];
  assert(x3.kind() == QualifiedType::VAR);
  assert(x3.type() && x3.type()->isErroneousType());
}

// This is private issue #6022. It tests a case where a nested function uses
// a field accessible through a outer method's receiver.
static void test5a(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R""""(
    record R {
      type T;
      var x : T;

      proc foobar() {
        proc helper(arg: T) { // Error for 'T' !
        }

        helper(x);
      }
    }

    var r : R(int);
    r.foobar();
    )"""";

  auto m = resolveTypesOfVariables(ctx, program, {"r"});
  assert(!guard.realizeErrors());
  assert(m["r"].type()->isRecordType());
}

static void test5b(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R""""(
    record R {
      type T;
      var x : T;

      proc foobar() {
        proc helper(arg: T) { // Error for 'T' !
          var y: x.type;
          return y;
        }
        return helper(x);
      }
    }

    var r : R(int);
    var x = r.foobar();
    )"""";

  auto qt = resolveQualifiedTypeOfX(ctx, program);
  assert(!guard.realizeErrors());
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type() && qt.type()->isIntType());
}

static void test5c(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R""""(
    proc test(ref arg: ?t) {
      proc helper(ref x: t) {
        if t == string {
          return "test";
        } else {
          return 5;
        }
      }

      return helper(arg);
    }

    var x = 5;
    var y = test(x);
    var z = test("foo");
    )"""";

  auto qt = resolveTypesOfVariables(ctx, program, { "y", "z" });
  assert(!guard.realizeErrors());
  assert(qt["y"].kind() == QualifiedType::VAR);
  assert(qt["y"].type() && qt["y"].type()->isIntType());
  assert(qt["z"].kind() == QualifiedType::VAR);
  assert(qt["z"].type() && qt["z"].type()->isStringType());
}

static void test5d(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R""""(
    record R {
      type T;
      var x : T;

      proc foo() {
        proc bar(): T { return x; }
        return bar();
      }
    }

    var r : R(int);
    var x = r.foo();
    )"""";

  auto qt = resolveQualifiedTypeOfX(ctx, program);
  assert(!guard.realizeErrors());
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type() && qt.type()->isIntType());
}

// Similar to the other 5x cases, but with a secondary method
static void test5e(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  {
    ErrorGuard guard(ctx);

    std::string program =
      R"""(
      record R {
        type T;
        var x : T;
      }

      proc R.foobar() {
        proc helper(arg: T) {
          return arg;
        }

        return helper(x);
      }

      var r : R(int);
      var x = r.foobar();
      )""";

    auto qt = resolveQualifiedTypeOfX(ctx, program);
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isIntType());
  }
  {
    ErrorGuard guard(ctx);

    std::string program =
      R"""(
      record R {
        type T;
        var x : T;
      }

      proc R.foobar() {
        // generic case
        proc helper(arg: T, other: ?) {
          return __primitive("+", arg, other);
        }

        return helper(x, 5);
      }

      var r : R(int);
      var x = r.foobar();

      )""";

    auto qt = resolveQualifiedTypeOfX(ctx, program);
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isIntType());
  }
}

// Same as test5 but with a nested method instead of a nested function.
static void test6(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R""""(
    record R {
      type T;
      var x : T;

      proc foobar() {
        proc R.helper(arg: T) { var y: T; return y; }
        return helper(x);
      }
    }

    var r : R(int);
    var x = r.foobar();
    )"""";

  auto qt = resolveQualifiedTypeOfX(ctx, program);
  assert(!guard.realizeErrors());
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type() && qt.type()->isIntType());
}

// This is not legal, but we should still perform the correct name resolution.
// TODO: Right now, mentions of 'T' in 'helper' are not resolved to the field.
/**
static void test7(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R""""(
    record R {
      type T;
      var x : T;

      proc foobar() {
        record S {
          proc helper(arg: T) { var y: T; return y; }
        }
        var v : S;
        return v.helper(x);
      }
    }

    var r : R(int);
    var x = r.foobar();
    )"""";

  auto qt = resolveQualifiedTypeOfX(ctx, program);
  assert(guard.numErrors() == 2);
  assert(guard.errors()[0]->type() == chpl::NestedClassFieldRef);
  assert(guard.errors()[1]->type() == chpl::NestedClassFieldRef);
  guard.clearErrors();
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type() && qt.type()->isIntType());
}
*/

// TODO: It is illegal to use 'x' in the definition of 'S.y', but should
// this program still type as though it would work? Right now it can't
// be typed because the lookup fails.
/*
static void test8(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R""""(
    record R {
      type T;
      var x : T;

      proc foobar() {
        record S {
          var y: x.type;
          proc helper() { return y; }
        }
        var v : S;
        return v.helper();
      }
    }

    var r : R(int);
    var x = r.foobar();
    )"""";

  auto qt = resolveQualifiedTypeOfX(ctx, program);
  assert(!guard.realizeErrors());
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type() && qt.type()->isIntType());
}
*/

static void test9(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R""""(
    proc helper(type T) param: string {
      return T:string + "_hello";
    }

    proc foo(type T) {
      extern helper(T) proc foobar(obj: int): int; // should rename to "bool_hello" at codegen
      var x: int;
      return foobar(x);
    }

    var x = foo(bool);
    )"""";

  auto qt = resolveQualifiedTypeOfX(ctx, program);
  assert(!guard.realizeErrors());
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type() && qt.type()->isIntType());

  auto m = parseModule(ctx, std::move(program));
  CalledFnsSet called;
  std::ignore = gatherTransitiveFnsCalledByModInit(ctx, m->id(), called);

  bool found = false;
  for (auto [fn, order] : called) {
    if (fn->signature()->untyped()->name() != "foobar") continue;

    assert(fn->linkageName() == "bool_hello");
    found = true;
  }
  assert(found);
}

// This is private issue #6123.
static void test10(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R""""(
    proc externT(type T) type { return int; }

    record R {
      type valType;
      var val : valType;

      proc foo() {
        extern proc helper(arg: externT(valType)) : real;

        var dummy : int;
        return helper(dummy);
      }
    }

    var r : R(int);
    var x = r.foo();
    )"""";

  auto qt = resolveQualifiedTypeOfX(ctx, program);
  assert(!guard.realizeErrors());
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type() && qt.type()->isRealType());
}


static void test11(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R""""(
    record R { }

    proc R.test() {
      proc foobar(arg: int) { return arg; }
      proc foobar(arg: real) { return arg; }
      proc foobar(arg: string) { return arg; }

      return foobar("test");
    }

    var r : R;
    var x = r.test();
    )"""";

  auto vars = resolveTypesOfVariables(ctx, program, {"x"});
  auto x = vars["x"];
  assert(x.type()->isStringType());
}

static void test12(Context* ctx) {
  // Test ambiguity emitted between nested function and method.
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program = R"""(
      class Foo {
        proc init() {}

        proc asdf() {
          return "test";
        }

        proc doSomething() {
          proc asdf() do return 2;
          return asdf();
        }
      }

      var f = new Foo();
      var x = f.doSomething();
      )""";

  auto vars = resolveTypesOfVariables(ctx, program, { "x" });
  assert(guard.realizeErrors());
}

static void test12b(Context* ctx) {
  // Test ambiguity emitted between nested function and method.
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program = R"""(
    proc myProcWithAProcAndAClass() {
      proc asdf() do return 2;
      class Foo {
        proc init() {}
      
        proc asdf() {
          return "test";
        }
      
        proc doSomething() {
          return asdf();
        }
      }
      var f = new Foo();
      return f.doSomething();
    }
    var x = myProcWithAProcAndAClass();
    )""";

  auto vars = resolveTypesOfVariables(ctx, program, { "x" });
  assert(guard.realizeErrors());
}


static void test13(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R""""(
    class Foo {
      type valType = int;
      proc typeGetter(type t) type do return t;

      proc read() {
        proc nested(x : typeGetter(valType)) do return x;
        return nested(0);
      }
    }

    var foo = new Foo(int);
    var x = foo.read();
    )"""";

  auto qt = resolveQualifiedTypeOfX(ctx, program);
  assert(!guard.realizeErrors());
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type() && qt.type()->isIntType());
}

static void test14(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R"""(
    proc test() {
      extern type time_t;
      extern type suseconds_t;

      record timeval {
        var tv_sec: time_t;
        var tv_usec: suseconds_t;
      }

      var tv : timeval;
      var ret = __primitive("cast", int, tv.tv_sec);
      return ret;
    }

    var x = test();
    )""";

  std::ignore = resolveTypeOfX(ctx, program);
}

static void test15(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R"""(
    record R {
      type T;
      var x : int;
    }

    proc other(type T) {
      var ret : T;
      return ret;
    }

    proc helper(arg: R(?t), val: int) {
      return nested(val);

      proc nested(v: int) {
        return other(t);
      }
    }

    proc test() {
      var r = new R(int, 42);
      return helper(r, 5);
    }
    var x = test();
    )""";

  std::ignore = resolveTypeOfX(ctx, program);

  auto m = parseModule(ctx, std::move(program));
  CalledFnsSet called;
  std::ignore = gatherTransitiveFnsCalledByModInit(ctx, m->id(), called);
}

static void test16(Context* ctx) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(ctx);
  ErrorGuard guard(ctx);

  std::string program =
    R"""(
    proc helper(args) {
      proc nested() {
        var sum = 0;
        for param dim in 0..args.size-1 {
          if args(dim).type != int {
            sum += 1;
          }
        }
        return sum;
      }

      return nested();
    }

    var args = (1, 2, 3, 4);
    var x = helper(args);
    )""";

  std::ignore = resolveTypeOfX(ctx, program);

  auto m = parseModule(ctx, std::move(program));
  CalledFnsSet called;
  std::ignore = gatherTransitiveFnsCalledByModInit(ctx, m->id(), called);
}

int main() {
  auto context = buildStdContext();
  Context* ctx = turnOnWarnUnstable(context);

  test0(ctx);
  test1(ctx);
  test2(ctx);
  test3(ctx);
  test4(ctx);
  test5a(ctx);
  test5b(ctx);
  test5c(ctx);
  test5d(ctx);
  test5e(ctx);
  test6(ctx);
  // test7(ctx);
  // test8(ctx);
  test9(ctx);
  test10(ctx);
  test11(ctx);
  test12(ctx);
  test12b(ctx);
  test13(ctx);
  test14(ctx);
  test15(ctx);
  test16(ctx);

  return 0;
}
