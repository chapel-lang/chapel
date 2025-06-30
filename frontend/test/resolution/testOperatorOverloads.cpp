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
#include "test-resolution.h"

// basic definition tests with cast operator
static void test1() {
  Context ctx;
  auto context = &ctx;

  // no declaration (expect cast to fail)
  QualifiedType qt1 = resolveTypeOfXInit(context,
    R""""(
      record R {
        var field: int;
      }
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt1.kind() == QualifiedType::UNKNOWN);
  assert(qt1.type() && qt1.type()->isErroneousType());
  ctx.advanceToNextRevision(false);

  // primary operator method
  QualifiedType qt2 = resolveTypeOfXInit(context,
    R""""(
      operator =(ref lhs: int, const rhs: int) {}
      record R {
        var field: int;
        operator :(z: R, type t: int) { return z.field; }
      }
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt2.type() && qt2.type()->isIntType());
  assert(qt2.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);

  // secondary operator method
  QualifiedType qt3 = resolveTypeOfXInit(context,
    R""""(
      operator =(ref lhs: int, const rhs: int) {}
      record R {
        var field: int;
      }
      operator R.:(z: R, type t: int) { return z.field; }
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt3.type() && qt3.type()->isIntType());
  assert(qt3.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);

  // non-method operator
  QualifiedType qt4 = resolveTypeOfXInit(context,
    R""""(
      operator =(ref lhs: int, const rhs: int) {}
      record R {
        var field: int;
      }
      operator :(z: R, type t: int) { return z.field; }
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt4.type() && qt4.type()->isIntType());
  assert(qt4.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);
}

// scope issue tests with cast operator
static void test2() {
  Context ctx;
  auto context = &ctx;

  // method and function operator definitions in the same scope should conflict (ambiguous call)
  QualifiedType qt1 = resolveTypeOfXInit(context,
    R""""(
      operator =(ref lhs: int, const rhs: int) {}
      record R {
        var field: int;
      }
      operator R.:(z: R, type t: int) { return z.field; }
      operator :(z: R, type t: int) { return z.field; }
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt1.kind() == QualifiedType::UNKNOWN);
  assert(qt1.type() && qt1.type()->isErroneousType());
  ctx.advanceToNextRevision(false);

  // access to R should implicitly grant access to its method operators
  QualifiedType qt2 = resolveTypeOfXInit(context,
    R""""(
      operator =(ref lhs: int, const rhs: int) {}
      module M {
        record R {
          var field: int;
        }
        operator R.:(z: R, type t: int) { return z.field; }
      }

      import this.M.R;
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt2.type() && qt2.type()->isIntType());
  assert(qt2.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);
}

// various additional tests with other operators
static void test3() {
  Context ctx;
  auto context = &ctx;

  // incorrectly defined unary operator overload
  QualifiedType qt1 = resolveTypeOfXInit(context,
    R""""(
      operator =(ref lhs: int, const rhs: int) {}
      record R {
        var field: int;
        operator !(z: R, type t: int) { return z.field; }
      }
      var myR: R;
      var x = !myR;
    )""""
  );
  assert(qt1.kind() == QualifiedType::UNKNOWN);
  assert(qt1.type() && qt1.type()->isErroneousType());
  ctx.advanceToNextRevision(false);

  // correct unary operator overload
  QualifiedType qt2 = resolveTypeOfXInit(context,
    R""""(
      operator =(ref lhs: int, const rhs: int) {}
      record R {
        var field: int;
        operator !(z: R) { return z.field; }
      }
      var myR: R;
      var x = !myR;
    )""""
  );
  assert(qt2.type() && qt2.type()->isIntType());
  assert(qt2.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);

  // overloading more operators
  QualifiedType qt3 = resolveTypeOfXInit(context,
    R""""(
      operator =(ref lhs: int, const rhs: int) {}
      record R {
        var field: int;
        operator +(z: R, t: int) { return z.field; }
      }
      var myR: R;
      var x = myR + 3;
    )""""
  );
  assert(qt3.type() && qt3.type()->isIntType());
  assert(qt3.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);

  QualifiedType qt4 = resolveTypeOfXInit(context,
    R""""(
      operator =(ref lhs: int, const rhs: int) {}
      record R {
        var field: int;
        operator >(z: R, t: complex) { return true; }
      }
      var myR: R;
      var x = myR > 2;
    )""""
  );
  assert(qt4.type() && qt4.type()->isBoolType());
  assert(qt4.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);

  QualifiedType qt5 = resolveTypeOfXInit(context,
    R""""(
      operator =(ref lhs: int, const rhs: int) {}
      record R {
        var field: int;
      }
      operator <(z: R, otherZ: R) { return 3; }
      var myR: R;
      var myOtherR: R;
      var x = myR < myOtherR;
    )""""
  );
  assert(qt5.type() && qt5.type()->isIntType());
  assert(qt5.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);

  // overloading operator for non-compound types
  QualifiedType qt6 = resolveTypeOfXInit(context,
    R""""(
      operator =(ref lhs: int, const rhs: int) {}
      operator *(a: int, b: int) { return true; }
      var a: int = 2;
      var b: int = 3;
      var x = a * b;
    )""""
  );
  assert(qt6.type() && qt6.type()->isBoolType());
  assert(qt6.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);
}

// test that we get a compiler generated record method for `==` when none exist
static void test4() {
  // generated field comparison uses chpl_field_neq, which is part of the stdib.
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
      operator =(ref lhs: int, const rhs: int) {}
      record R {
        var x : int;
      }

      proc foo() {
        const a : R;
        const b : R;
        return a == b;
      }
      var x = foo();
    )"""";

  QualifiedType initType = resolveTypeOfXInit(context, program);
  assert(initType.type()->isBoolType());
  assert(initType.kind() == QualifiedType::CONST_VAR);
}

// test that we don't get a compiler generated record method for `==`
// when one exists as a method
static void test5() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::string program =
    R""""(
      operator =(ref lhs: real, const rhs: real) {}
      operator =(ref lhs: int, const rhs: int) {}
      record R {
        var x : int;
      }
      operator R.==(a: R, b: R) { return 4.2; }

      var a : R;
      var b : R;

      var x = a == b;
    )"""";

  QualifiedType initType = resolveTypeOfXInit(context, program);
  assert(initType.type()->isRealType());
  assert(initType.kind() == QualifiedType::CONST_VAR);
}

// test that we don't get a compiler generated record method for `==`
// when one exists as a standalone procedure
static void test6() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::string program =
    R""""(
      operator =(ref lhs: int, const rhs: int) {}
      record R {
        var x : int;
      }
      operator ==(a: R, b: R) { return 1; }

      var a : R;
      var b : R;

      var x = a == b;
    )"""";

  QualifiedType initType = resolveTypeOfXInit(context, program);
  assert(initType.type()->isIntType());
  assert(initType.kind() == QualifiedType::CONST_VAR);
}

// test that we do get a compiler generated record method for `==`
// when other operators exist
static void test7() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
      operator =(ref lhs: int, const rhs: int) {}
      record R {
        var x : int;
      }
      operator +=(a: R, b: R) { return 1; }
      operator R.-(a:R,b:R) { return 2; }

      var a : R;
      var b : R;

      var x = a == b;
    )"""";

  QualifiedType initType = resolveTypeOfXInit(context, program);
  assert(initType.type()->isBoolType());
  assert(initType.kind() == QualifiedType::CONST_VAR);
}

// test that we do get a compiler generated record method for `==`
// even when == exists for R and another type (this matches production).
static void test7b() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
      record R {
        var x : int;
      }
      operator R.==(a:R,b:int) { return 2; }

      var a : R;
      var b : R;

      var x = a == b;
    )"""";

  QualifiedType initType = resolveTypeOfXInit(context, program);
  assert(initType.type()->isBoolType());
  assert(initType.kind() == QualifiedType::CONST_VAR);
}

// for generic types, test that we get a compiler generated method for `==`
// even if a specialized `==` exists for different types.
static void test7c() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
      record R {
        var field;
      }
      operator R.==(a:R(int), b:R(int)) { return 2; }

      var a = new R(0);
      var b = new R(0.0);

      var x = a == a;
      var y = b == b;
    )"""";

  auto vars = resolveTypesOfVariables(context, program, {"x", "y"});
  assert(vars.at("x").type() && vars.at("x").type()->isIntType());
  assert(vars.at("y").type() && vars.at("y").type()->isBoolType());
}

static void test7d() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
      record R {
        type t = int;
        var field : t;
      }
      operator R.==(a:R(int), b:R(int)) { return 2; }

      var a = new R();
      var b = new R(real, 0.0);

      var x = a == a;
      var y = b == b;
    )"""";

  auto vars = resolveTypesOfVariables(context, program, {"x", "y"});
  assert(vars.at("x").type() && vars.at("x").type()->isIntType());
  assert(vars.at("y").type() && vars.at("y").type()->isBoolType());
}

// test that we get compiler generated methods for = and == when inside a proc
static void test8() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(

      record T {
        var y: string;
      }

      // make sure these don't prevent us generating R.== and R.=
      operator ==(v:T, w:T) { return false; }
      operator =(v:T, w:T) { }

      record R {
        var x:int;
      }

      var r = new R(9);
      var q = new R(10);
      proc assign(ref a:R, b:R) {
        var p = new R(0);
        if (a == b) {
          a = p;
        }
      }
      assign(r, q);
      var x = r;
    )"""";

  QualifiedType initType = resolveTypeOfXInit(context, program);
  assert(initType.type()->isRecordType());
  assert(initType.kind() == QualifiedType::VAR);
}

// Ambiguous overloads with last resort.
static void test9() {
  Context ctx;
  auto context = &ctx;

  QualifiedType qt = resolveTypeOfXInit(context,
                                        R""""(
      record R {
        var field: int;
        operator :(z: R, type t: int) { return z.field; }
        pragma "last resort"
        operator :(z: R, type t: int) { return z.field + 1; }
      }
      var myR: R;
      var x = myR : int;
    )"""");
  assert(qt.type() && qt.type()->isIntType());
  assert(qt.kind() == QualifiedType::CONST_VAR);
}

static void helpTestGeneratedComparisons(std::string program, std::vector<std::string> trace) {
  std::string wrapper = R"""(
    record wrapper {
      var x;

      operator <(lhs: wrapper, rhs: wrapper) {
        compilerWarning("operator < with " + (lhs.x.type : string));
        return lhs.x < rhs.x;
      }

      operator >(lhs: wrapper, rhs: wrapper) {
        compilerWarning("operator > with " + (lhs.x.type : string));
        return lhs.x > rhs.x;
      }

      operator ==(lhs: wrapper, rhs: wrapper) {
        compilerWarning("operator == with " + (lhs.x.type : string));
        return lhs.x == rhs.x;
      }

      operator !=(lhs: wrapper, rhs: wrapper) {
        compilerWarning("operator != with " + (lhs.x.type : string));
        return lhs.x != rhs.x;
      }
    }
  )""";

  auto wholeProg = wrapper + program;

  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  auto result = resolveTypesOfVariables(ctx, wholeProg, {});

  int index = 0;
  assert(guard.numErrors() == trace.size() * 2);
  for (auto& op : trace) {
    assert(guard.error(index)->message().find(op) != std::string::npos);
    index += 2;
  }

  assert(guard.realizeErrors(/* countWarnings */ false) == 0);
}

// test compiler-generated comparison operators
static void test10() {
  helpTestGeneratedComparisons(
      R"""(
      record R {
        var x: wrapper(int);
      }
      var tmp = new R() == new R();
      )""", { "!= with int" });

  helpTestGeneratedComparisons(
      R"""(
      record R {
        var x: wrapper(int);
        var y: wrapper(bool);
      }
      var tmp = new R() == new R();
      )""", { "!= with int", "!= with bool" });

  helpTestGeneratedComparisons(
      R"""(
      record R {
        var x: wrapper(int);
        var y: wrapper(bool);
        var z: wrapper(real);
      }
      var tmp = new R() == new R();
      )""", { "!= with int", "!= with bool", "!= with real" });

  helpTestGeneratedComparisons(
      R"""(
      record R {
        var x: wrapper(int);
        var y: wrapper(bool);
        var z: wrapper(real);
      }
      var tmp = new R() < new R();
      )""", { "< with int", "> with int",
              "< with bool", "> with bool",
              "< with real", "> with real" });

  helpTestGeneratedComparisons(
      R"""(
      record R {
        var x: wrapper(int);
        var y: wrapper(bool);
        var z: wrapper(real);
      }
      var tmp = new R() > new R();
      )""", { "> with int", "< with int",
              "> with bool", "< with bool",
              "> with real", "< with real" });

  helpTestGeneratedComparisons(
      R"""(
      record R {
        var x: wrapper(int);
        var y: wrapper(bool);
        var z: wrapper(real);
      }
      var tmp = new R() <= new R();
      )""", { "< with int", "> with int",
              "< with bool", "> with bool",
              "< with real", "> with real" });

  helpTestGeneratedComparisons(
      R"""(
      record R {
        var x: wrapper(int);
        var y: wrapper(bool);
        var z: wrapper(real);
      }
      var tmp = new R() >= new R();
      )""", { "> with int", "< with int",
              "> with bool", "< with bool",
              "> with real", "< with real" });

  helpTestGeneratedComparisons(
      R"""(
      record R {
        type typeField = int;
        var x: wrapper(int);
        var y: wrapper(bool);
        var z: wrapper(real);
      }
      var tmp = new R() >= new R();
      )""", { "> with int", "< with int",
              "> with bool", "< with bool",
              "> with real", "< with real" });
}

// test that we generate enum-to-string casts if they are not present
static void test11() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
      enum E { A, B, C }
      var e = E.A;
      var x = e : string;
      var y = e : bytes;
    )"""";

  auto vars = resolveTypesOfVariables(context, program, {"x", "y"});
  assert(vars.at("x").type() && vars.at("x").type()->isStringType());
  assert(vars.at("y").type() && vars.at("y").type()->isBytesType());
}

// test that enum-to-string casts are skipped if user overloads are present
static void test12() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
      enum E { A, B, C }
      operator E.:(e: E, type t: string) { return 42; }
      operator E.:(e: E, type t: bytes) { return 42.0; }
      var e = E.A;
      var x = e : string;
      var y = e : bytes;
    )"""";

  auto vars = resolveTypesOfVariables(context, program, {"x", "y"});
  assert(vars.at("x").type() && vars.at("x").type()->isIntType());
  assert(vars.at("y").type() && vars.at("y").type()->isRealType());
}

// test that where-based overloads are handled appropriately when deciding
// to generate defaults.
static void test13a() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
      enum E { A, B, C }
      operator E.:(e: E, type t) where isUintType(t) { return 42; }
      operator E.:(e: E, type t) where t == bytes { return 42.0; }
      var e = E.A;
      var x = e : string;
      var y = e : bytes;
    )"""";

  auto vars = resolveTypesOfVariables(context, program, {"x", "y"});
  assert(vars.at("x").type() && vars.at("x").type()->isStringType());
  assert(vars.at("y").type() && vars.at("y").type()->isRealType());
}

static void test13b() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
      enum E { A, B, C }
      operator :(e: ?t, type target: string) where isIntType(t) { return 42; }
      operator :(e: ?t, type target: bytes) where t == E { return 42.0; }
      var e = E.A;
      var x = e : string;
      var y = e : bytes;
    )"""";

  auto vars = resolveTypesOfVariables(context, program, {"x", "y"});
  assert(vars.at("x").type() && vars.at("x").type()->isStringType());
  assert(vars.at("y").type() && vars.at("y").type()->isRealType());
}

// test that we generate string-to-enum casts if they are not present
static void test14() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
      enum E { A, B, C }
      var es = "A";
      var eb = b"A";
      var x = es : E;
      var y = eb : E;
    )"""";

  auto vars = resolveTypesOfVariables(context, program, {"x", "y"});
  assert(vars.at("x").type() && vars.at("x").type()->isEnumType());
  assert(vars.at("y").type() && vars.at("y").type()->isEnumType());
}

// test that enum-to-string casts are skipped if user overloads are present
static void test15() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
      enum E { A, B, C }
      operator E.:(e: string, type t: E) { return 42; }
      operator E.:(e: bytes, type t: E) { return 42.0; }
      var es = "A";
      var eb = b"A";
      var x = es : E;
      var y = eb : E;
    )"""";

  auto vars = resolveTypesOfVariables(context, program, {"x", "y"});
  assert(vars.at("x").type() && vars.at("x").type()->isIntType());
  assert(vars.at("y").type() && vars.at("y").type()->isRealType());
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test7b();
  test7c();
  test7d();
  test8();
  test9();
  test10();
  test11();
  test12();
  test13a();
  test13b();
  test14();
  test15();

  return 0;
}
