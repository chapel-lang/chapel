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
#include "./ErrorGuard.h"

#define TEST_NAME(ctx__)\
  chpl::UniqueString::getConcat(ctx__, __FUNCTION__, ".chpl")

static Context*
turnOnWarnUnstable(Context* ctx) {
  CompilerFlags flags;
  flags.set(CompilerFlags::WARN_UNSTABLE, true);
  setCompilerFlags(ctx, std::move(flags));
  assert(isCompilerFlagSet(ctx, CompilerFlags::WARN_UNSTABLE));
  return ctx;
}

// This test demonstrates that it is safe to resolve a nested function like
// normal if the function does not refer to outer variables, regardless of
// whether or not the function is generic or concrete.
static void test0(void) {
  Context context;
  Context* ctx = turnOnWarnUnstable(&context);
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
static void test1(void) {
  Context context;
  Context* ctx = turnOnWarnUnstable(&context);
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
static void test2(void) {
  Context context;
  Context* ctx = turnOnWarnUnstable(&context);
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
static void test3(void) {
  Context context;
  Context* ctx = turnOnWarnUnstable(&context);
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
static void test4(void) {
  Context context;
  Context* ctx = turnOnWarnUnstable(&context);
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

int main() {
  test0();
  test1();
  test2();
  test3();
  test4();
  return 0;
}
