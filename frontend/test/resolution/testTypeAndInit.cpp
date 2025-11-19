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

#include "test-resolution.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

#include "./ErrorGuard.h"

static void test1() {
  Context ctx;
  auto context = &ctx;
  auto qt = resolveQualifiedTypeOfX(context,
                             R""""(
                               var x: bool = true;
                             )"""");
  qt.dump();
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type()->isBoolType());
}

static void test2() {
  Context ctx;
  auto context = &ctx;
  auto qt = resolveQualifiedTypeOfX(context,
                                    R""""(
                                      proc foo(arg: uint = 0) do return arg;
                                      var x = foo();
                                    )"""");
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type()->isUintType());
}

// Test split-init with bytes type and string literals
// Should allow init= conversion from string to bytes
static void testBytesStringInitConversion() {
  printf("testBytesStringInitConversion\n");

  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto program = R"""(
      module M {
        proc test(flag: bool) {
          var x: bytes;
          if flag {
            x = b"hello";
          } else {
            x = "world";  // string literal, should convert via init=
          }
        }
      }
      )""";

  auto path = UniqueString::get(context, "testBytesStringInitConversion.chpl");
  setFileText(context, path, program);
  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* M = vec[0]->toModule();
  assert(M);
  assert(M->numStmts() == 1);

  const Function* func = M->stmt(0)->toFunction();
  assert(func);

  const ResolvedFunction* r = resolveConcreteFunction(context, func->id());
  assert(r);

  // Should compile without errors (init= handles string to bytes conversion)
  // Note: there will be unimplemented warnings but no errors
  assert(guard.numErrors() == 0);
}

// Test incompatible type assignment to primitive type with explicit type
static void testPrimitiveTypeIncompatibleInit() {
  printf("testPrimitiveTypeIncompatibleInit\n");

  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto program = R"""(
      module M {
        proc test() {
          var x: int;
          x = 3.5;  // Should error: can't assign real to int
        }
      }
      )""";

  auto path = UniqueString::get(context, "testPrimitiveTypeIncompatibleInit.chpl");
  setFileText(context, path, program);
  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* M = vec[0]->toModule();
  assert(M);
  assert(M->numStmts() == 1);

  const Function* func = M->stmt(0)->toFunction();
  assert(func);

  const ResolvedFunction* r = resolveConcreteFunction(context, func->id());
  assert(r);

  // Should have error for incompatible type
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->type() == ErrorType::IncompatibleTypeAndInit);

  guard.realizeErrors();
}

// Test incompatible type in split-init to primitive type
static void testPrimitiveTypeIncompatibleSplitInit() {
  printf("testPrimitiveTypeIncompatibleSplitInit\n");

  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto program = R"""(
      module M {
        proc test(flag: bool) {
          var x: int;
          if flag {
            x = 1;
          } else {
            x = 2.5;  // Should error: can't assign real to int
          }
        }
      }
      )""";

  auto path = UniqueString::get(context, "testPrimitiveTypeIncompatibleSplitInit.chpl");
  setFileText(context, path, program);
  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* M = vec[0]->toModule();
  assert(M);
  assert(M->numStmts() == 1);

  const Function* func = M->stmt(0)->toFunction();
  assert(func);

  const ResolvedFunction* r = resolveConcreteFunction(context, func->id());
  assert(r);

  // Should have error for incompatible type
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->type() == ErrorType::IncompatibleTypeAndInit);

  guard.realizeErrors();
}

static void testDefaultArgRefCoercion() {
  // even though `0` is a uint and ref doesn't allow coercions,
  // it should work in formals. Use 'const' to not care about mutability of '0'.
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);
    auto qt = resolveQualifiedTypeOfX(context,
                                      R""""(
                                        proc foo(const ref arg: uint = 0) do return arg;
                                        var x = foo();
                                      )"""");
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type()->isUintType());
  }
  // Same as before but with a global variable to disentangle this from
  // param narrowing.
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);
    auto qt = resolveQualifiedTypeOfX(context,
                                      R""""(
                                        var glob: int = 0;
                                        proc foo(const ref arg: uint = glob) do return arg;
                                        var x = foo();
                                      )"""");
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type()->isUintType());
  }

  // neither of the above cases should work for variables.
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);
    auto qt = resolveQualifiedTypeOfX(context,
                                      R""""(
                                        const ref x: uint = 0;
                                      )"""");
    assert(qt.isUnknownOrErroneous());
    assert(guard.realizeErrors() == 1);
  }
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);
    auto qt = resolveQualifiedTypeOfX(context,
                                      R""""(
                                        var glob: int = 0;
                                        const ref x: uint = glob;
                                      )"""");
    assert(qt.isUnknownOrErroneous());
    assert(guard.realizeErrors() == 1);
  }
}

int main() {
  test1();
  test2();
  testBytesStringInitConversion();
  testPrimitiveTypeIncompatibleInit();
  testPrimitiveTypeIncompatibleSplitInit();
  testDefaultArgRefCoercion();
  return 0;
}
