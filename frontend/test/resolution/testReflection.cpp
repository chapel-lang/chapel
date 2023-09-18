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
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

static void ensureParamInt(const QualifiedType& type, int64_t expectedValue) {
  assert(type.kind() == QualifiedType::PARAM);
  assert(type.type() != nullptr);
  assert(type.type()->isIntType());
  assert(type.param() != nullptr);
  assert(type.param()->isIntParam());
  assert(type.param()->toIntParam()->value() == expectedValue);
}

static void ensureParamBool(const QualifiedType& type, bool expectedValue) {
  assert(type.kind() == QualifiedType::PARAM);
  assert(type.type() != nullptr);
  assert(type.type()->isBoolType());
  assert(type.param() != nullptr);
  assert(type.param()->isBoolParam());
  assert(type.param()->toBoolParam()->value() == expectedValue);
}

static void ensureParamString(const QualifiedType& type, const std::string& expectedValue) {
  assert(type.kind() == QualifiedType::PARAM);
  assert(type.type() != nullptr);
  assert(type.type()->isStringType());
  assert(type.param() != nullptr);
  assert(type.param()->isStringParam());
  assert(type.param()->toStringParam()->value() == expectedValue);
}

static void ensureErroneousType(const QualifiedType& type) {
  assert(type.type() != nullptr);
  assert(type.type()->isErroneousType());
}

// test num fields and field num to name
static void test1() {
  Context context;
  auto variables = resolveTypesOfVariables(&context,
      R"""(
      record R {
         var a, b: int;
         var c: int;
      }
      param r1 = __primitive("num fields", R);
      param r2 = __primitive("field num to name", R, 1);
      param r3 = __primitive("field num to name", R, 2);
      param r4 = __primitive("field num to name", R, 3);
      param r5 = __primitive("field num to name", R, 4);
      param r6 = __primitive("field num to name", R, "hi");
      param r7 = __primitive("field num to name", R);
      param r8 = __primitive("field num to name", R, 1, 1);
      )""", { "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8" });
  ensureParamInt(variables.at("r1"), 3);
  ensureParamString(variables.at("r2"), "a");
  ensureParamString(variables.at("r3"), "b");
  ensureParamString(variables.at("r4"), "c");
  ensureErroneousType(variables.at("r5"));
  ensureErroneousType(variables.at("r6"));
  ensureErroneousType(variables.at("r7"));
  ensureErroneousType(variables.at("r8"));
}
//
// test field num to name
static void test2() {
  Context context;
  auto variables = resolveTypesOfVariables(&context,
      R"""(
      record R {
         var a, b: int;
         var c: int;
      }
      param r1 = __primitive("field name to num", R, "a");
      param r2 = __primitive("field name to num", R, "b");
      param r3 = __primitive("field name to num", R, "c");
      param r4 = __primitive("field name to num", R, "d");
      param r5 = __primitive("field name to num", R, 1);
      param r6 = __primitive("field name to num", R);
      param r7 = __primitive("field name to num", R, "a", "b");
      )""", { "r1", "r2", "r3", "r4", "r5", "r6", "r7" });
  ensureParamInt(variables.at("r1"), 1);
  ensureParamInt(variables.at("r2"), 2);
  ensureParamInt(variables.at("r3"), 3);
  ensureErroneousType(variables.at("r4"));
  ensureErroneousType(variables.at("r5"));
  ensureErroneousType(variables.at("r6"));
  ensureErroneousType(variables.at("r7"));
}

// Test field by num
static void test3() {
  Context context;
  auto variables = resolveTypesOfVariables(&context,
      R"""(
      record R {
         var a, b: int;
         var c: string;
      }
      var r: R;
      var r1 = __primitive("field by num", r, 1);
      var r2 = __primitive("field by num", r, 2);
      var r3 = __primitive("field by num", r, 3);
      var r4 = __primitive("field by num", r, "hi");
      var r5 = __primitive("field by num", r);
      var r6 = __primitive("field by num", r, 1, 2);
      var r7 = __primitive("field by num", R, 1);
      )""", { "r1", "r2", "r3", "r4", "r5", "r6", "r7" });
  assert(variables.at("r1").type()->isIntType());
  assert(variables.at("r2").type()->isIntType());
  assert(variables.at("r3").type()->isStringType());
  ensureErroneousType(variables.at("r4"));
  ensureErroneousType(variables.at("r5"));
  ensureErroneousType(variables.at("r6"));
  ensureErroneousType(variables.at("r7"));
}

// Test is bound
static void test4() {
  Context context;
  auto variables = resolveTypesOfVariables(&context,
      R"""(
      record R {
         var a, b: int;
         type c = bool;
      }
      var r: R;
      param r1 = __primitive("is bound", R, "a");
      param r2 = __primitive("is bound", R, "b");
      param r3 = __primitive("is bound", R, "c");
      param r4 = __primitive("is bound", R(?), "a");
      param r5 = __primitive("is bound", R(?), "b");
      param r6 = __primitive("is bound", R(?), "c");
      param r7 = __primitive("is bound", R(integral), "c");
      param r8 = __primitive("is bound", r, "a");
      param r9 = __primitive("is bound", R, 1);
      param r10 = __primitive("is bound", R);
      param r11 = __primitive("is bound", R, "a", "a");
      )""", { "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11" });
  ensureParamBool(variables.at("r1"), true);
  ensureParamBool(variables.at("r2"), true);
  ensureParamBool(variables.at("r3"), true);
  ensureParamBool(variables.at("r4"), true);
  ensureParamBool(variables.at("r5"), true);
  ensureParamBool(variables.at("r6"), false);
  ensureParamBool(variables.at("r7"), false);
  ensureErroneousType(variables.at("r8"));
  ensureErroneousType(variables.at("r9"));
  ensureErroneousType(variables.at("r10"));
  ensureErroneousType(variables.at("r11"));
}

// Test call resolves
static void test5() {
  Context context;
  auto variables = resolveTypesOfVariables(&context,
      R"""(
      proc f(x: int) {}
      proc f(x: bool) {}
      proc f(x: string, y: string) {}
      proc f(x: int, y: int) {
          var x: int = "hello";
      }
      proc g() {}
      param r1 = __primitive("call resolves", "g");
      param r2 = __primitive("call resolves", "f", 1);
      param r3 = __primitive("call resolves", "f", true);
      param r4 = __primitive("call resolves", "f", "a", "b");
      param r5 = __primitive("call resolves", "f", "a");
      param r6 = __primitive("call resolves", "f");
      param r7 = __primitive("call resolves", "f", 1, 1);
      param r8 = __primitive("call resolves", "f", int);
      )""", { "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8" });
  ensureParamBool(variables.at("r1"), true);
  ensureParamBool(variables.at("r2"), true);
  ensureParamBool(variables.at("r3"), true);
  ensureParamBool(variables.at("r4"), true);
  ensureParamBool(variables.at("r5"), false);
  ensureParamBool(variables.at("r6"), false);
  ensureParamBool(variables.at("r7"), true);
  ensureParamBool(variables.at("r8"), false);
}

// Test call resolves
static void test6() {
  Context context;
  // Make sure no errors make it to the user, even though we will get errors.
  ErrorGuard guard(&context);
  auto variables = resolveTypesOfVariables(&context,
      R"""(
      proc f(x: int) {}
      proc f(x: bool) {}
      proc f(x: string, y: string) {}
      proc f(x: int, y: int) {
          var x: int = "hello";
      }
      proc g() {}
      param r1 = __primitive("call and fn resolves", "g");
      param r2 = __primitive("call and fn resolves", "f", 1);
      param r3 = __primitive("call and fn resolves", "f", true);
      param r4 = __primitive("call and fn resolves", "f", "a", "b");
      param r5 = __primitive("call and fn resolves", "f", "a");
      param r6 = __primitive("call and fn resolves", "f");
      param r7 = __primitive("call and fn resolves", "f", 1, 1);
      param r8 = __primitive("call and fn resolves", "f", int);
      )""", { "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8" });
  ensureParamBool(variables.at("r1"), true);
  ensureParamBool(variables.at("r2"), true);
  ensureParamBool(variables.at("r3"), true);
  ensureParamBool(variables.at("r4"), true);
  ensureParamBool(variables.at("r5"), false);
  ensureParamBool(variables.at("r6"), false);
  ensureParamBool(variables.at("r7"), false); // different from test5
  ensureParamBool(variables.at("r8"), false);
}
//
// Test call resolves
static void test7() {
  Context context;
  auto variables = resolveTypesOfVariables(&context,
      R"""(
      record R {
        proc f(x: int) {}
        proc f(x: bool) {}
        proc f(x: string, y: string) {}
        proc f(x: int, y: int) {
            var x: int = "hello";
        }
        proc g() {}
      }
      var r: R;
      param r1 = __primitive("method call resolves", r, "g");
      param r2 = __primitive("method call resolves", r, "f", 1);
      param r3 = __primitive("method call resolves", r, "f", true);
      param r4 = __primitive("method call resolves", r, "f", "a", "b");
      param r5 = __primitive("method call resolves", r, "f", "a");
      param r6 = __primitive("method call resolves", r, "f");
      param r7 = __primitive("method call resolves", r, "f", 1, 1);
      param r8 = __primitive("method call resolves", r, "f", int);
      )""", { "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8" });
  ensureParamBool(variables.at("r1"), true);
  ensureParamBool(variables.at("r2"), true);
  ensureParamBool(variables.at("r3"), true);
  ensureParamBool(variables.at("r4"), true);
  ensureParamBool(variables.at("r5"), false);
  ensureParamBool(variables.at("r6"), false);
  ensureParamBool(variables.at("r7"), true);
  ensureParamBool(variables.at("r8"), false);
}

// Test call resolves
static void test8() {
  Context context;
  // Make sure no errors make it to the user, even though we will get errors.
  ErrorGuard guard(&context);
  auto variables = resolveTypesOfVariables(&context,
      R"""(
      record R {
        proc f(x: int) {}
        proc f(x: bool) {}
        proc f(x: string, y: string) {}
        proc f(x: int, y: int) {
            var x: int = "hello";
        }
        proc g() {}
      }
      var r: R;
      param r1 = __primitive("method call and fn resolves", r, "g");
      param r2 = __primitive("method call and fn resolves", r, "f", 1);
      param r3 = __primitive("method call and fn resolves", r, "f", true);
      param r4 = __primitive("method call and fn resolves", r, "f", "a", "b");
      param r5 = __primitive("method call and fn resolves", r, "f", "a");
      param r6 = __primitive("method call and fn resolves", r, "f");
      param r7 = __primitive("method call and fn resolves", r, "f", 1, 1);
      param r8 = __primitive("method call and fn resolves", r, "f", int);
      )""", { "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8" });
  ensureParamBool(variables.at("r1"), true);
  ensureParamBool(variables.at("r2"), true);
  ensureParamBool(variables.at("r3"), true);
  ensureParamBool(variables.at("r4"), true);
  ensureParamBool(variables.at("r5"), false);
  ensureParamBool(variables.at("r6"), false);
  ensureParamBool(variables.at("r7"), false); // different from test5
  ensureParamBool(variables.at("r8"), false);
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  return 0;
}
