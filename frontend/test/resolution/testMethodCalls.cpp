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
#include "chpl/uast/all-uast.h"

#include <functional>

// Test resolving a simple primary and secondary method in defining scope.
static void test1() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "test1.chpl");
  std::string contents =
    R""""(
      record r {
        proc doPrimary() {}
      }
      proc r.doSecondary() {}
      var obj: r;
      obj.doPrimary();
      obj.doSecondary();
    )"""";

  setFileText(context, path, contents);

  // Get the module.
  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);

  // Unpack all the uAST we need for the test.
  assert(m->numStmts() == 5);
  auto r = m->stmt(0)->toRecord();
  assert(r);
  assert(r->numDeclOrComments() == 1);
  auto fnPrimary = r->declOrComment(0)->toFunction();
  assert(fnPrimary);
  auto fnSecondary = m->stmt(1)->toFunction();
  assert(fnSecondary);
  auto obj = m->stmt(2)->toVariable();
  assert(obj);
  auto callPrimary = m->stmt(3)->toFnCall();
  assert(callPrimary);
  auto callSecondary = m->stmt(4)->toFnCall();
  assert(callSecondary);

  // Resolve the module.
  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  // Get the type of 'r'.
  auto& qtR = typeForModuleLevelSymbol(context, r->id());

  // Assert some things about the primary call.
  auto& reCallPrimary = rr.byAst(callPrimary);
  auto& qtCallPrimary = reCallPrimary.type();
  assert(qtCallPrimary.type()->isVoidType());
  auto mscCallPrimary = reCallPrimary.mostSpecific().only();
  assert(mscCallPrimary);
  auto tfsCallPrimary = mscCallPrimary.fn();

  // Check the primary call receiver.
  assert(tfsCallPrimary->id() == fnPrimary->id());
  assert(tfsCallPrimary->numFormals() == 1);
  assert(tfsCallPrimary->formalName(0) == "this");
  assert(tfsCallPrimary->formalType(0).type() == qtR.type());

  // Assert some things about the secondary call.
  auto& reCallSecondary = rr.byAst(callSecondary);
  auto& qtCallSecondary = reCallSecondary.type();
  assert(qtCallSecondary.type()->isVoidType());
  auto mscCallSecondary = reCallSecondary.mostSpecific().only();
  assert(mscCallSecondary);
  auto tfsCallSecondary = mscCallSecondary.fn();

  // Check the secondary call receiver.
  assert(tfsCallSecondary->id() == fnSecondary->id());
  assert(tfsCallSecondary->numFormals() == 1);
  assert(tfsCallSecondary->formalName(0) == "this");
  assert(tfsCallSecondary->formalType(0).type() == qtR.type());
}

// Similar test but for parenless methods.
static void test2() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "test2.chpl");
  std::string contents =
    R""""(
      record r {
        proc primary { }
      }
      proc r.secondary { }
      var obj: r;
      obj.primary;
      obj.secondary;
    )"""";
  setFileText(context, path, contents);

  // Get the module.
  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);

  // Unpack all the uAST we need for the test.
  assert(m->numStmts() == 5);
  auto r = m->stmt(0)->toRecord();
  assert(r);
  assert(r->numDeclOrComments() == 1);
  auto fnPrimary = r->declOrComment(0)->toFunction();
  assert(fnPrimary);
  auto fnSecondary = m->stmt(1)->toFunction();
  assert(fnSecondary);
  auto obj = m->stmt(2)->toVariable();
  assert(obj);
  auto callPrimary = m->stmt(3)->toDot();
  assert(callPrimary);
  auto callSecondary = m->stmt(4)->toDot();
  assert(callSecondary);

  // Resolve the module.
  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  // Get the type of 'r'.
  auto& qtR = typeForModuleLevelSymbol(context, r->id());

  // Assert some things about the primary call.
  auto& reCallPrimary = rr.byAst(callPrimary);
  auto& qtCallPrimary = reCallPrimary.type();
  assert(qtCallPrimary.type()->isVoidType());
  auto mscCallPrimary = reCallPrimary.mostSpecific().only();
  assert(mscCallPrimary);
  auto tfsCallPrimary = mscCallPrimary.fn();

  // Check the primary call receiver.
  assert(tfsCallPrimary->id() == fnPrimary->id());
  assert(tfsCallPrimary->numFormals() == 1);
  assert(tfsCallPrimary->formalName(0) == "this");
  assert(tfsCallPrimary->formalType(0).type() == qtR.type());

  // Assert some things about the secondary call.
  auto& reCallSecondary = rr.byAst(callSecondary);
  auto& qtCallSecondary = reCallSecondary.type();
  assert(qtCallSecondary.type()->isVoidType());
  auto mscCallSecondary = reCallSecondary.mostSpecific().only();
  assert(mscCallSecondary);
  auto tfsCallSecondary = mscCallSecondary.fn();

  // Check the secondary call receiver.
  assert(tfsCallSecondary->id() == fnSecondary->id());
  assert(tfsCallSecondary->numFormals() == 1);
  assert(tfsCallSecondary->formalName(0) == "this");
  assert(tfsCallSecondary->formalType(0).type() == qtR.type());
}

// test case to lock in correct behavior w.r.t. T being both a
// field and a formal.
static void test3() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  const char* contents = R""""(
                              module M {
                                record R {
                                  type T;
                                  proc foo(T: int) type {
                                    return this.T;
                                  }
                                }
                                var z: R(real);
                                var arg: int;
                                var x: z.foo(arg);
                              }
                         )"""";

  auto qt = resolveQualifiedTypeOfX(context, contents);
  assert(qt.type()->isRealType()); // and not int
}

static void test4() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "test4.chpl");
  std::string contents =
    R""""(
    module A {
      record r {}
    }
    module B {
      use A;
      proc r.foo() {}
      var x: r;
      x.foo();
    }
    )"""";
  setFileText(context, path, contents);

  // Get the modules.
  auto& br = parseFileToBuilderResult(context, path, UniqueString());
  assert(!guard.realizeErrors());
  assert(br.numTopLevelExpressions() == 2);
  auto modA = br.topLevelExpression(0)->toModule();
  assert(modA);
  auto modB = br.topLevelExpression(1)->toModule();
  assert(modB);

  // Get the record from module 'A'.
  assert(modA->numStmts() == 1);
  auto rec = modA->stmt(0)->toRecord();
  assert(rec);

  // Get the tertiary method, variable, and call from module 'B'.
  assert(modB->numStmts() == 4);
  auto tert = modB->stmt(1)->toFunction();
  assert(tert);
  auto x = modB->stmt(2)->toVariable();
  assert(x && !x->initExpression() && x->typeExpression());
  auto typeExpr = x->typeExpression()->toIdentifier();
  assert(typeExpr);
  auto call = modB->stmt(3)->toFnCall();
  assert(call);

  auto& rr = resolveModule(context, modB->id());
  assert(!guard.realizeErrors());

  auto& reX = rr.byAst(x);
  assert(reX.type().kind() == QualifiedType::VAR);
  assert(!reX.type().isUnknown());
  assert(!reX.type().isErroneousType());
  assert(reX.type().type()->isRecordType());

  // TODO: Confirm other things.
  (void) typeExpr;
  (void) call;
  (void) tert;
}

// Test a field being named the same as the record.
static void test5() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "test5.chpl");
  std::string contents =
    R""""(
      record r {
        var r = 1;
        proc doPrimary() {}
      }
      var obj: r;
      obj.doPrimary();
    )"""";

  setFileText(context, path, contents);

  // Get the module.
  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);

  // Unpack all the uAST we need for the test.
  assert(m->numStmts() == 3);
  auto r = m->stmt(0)->toRecord();
  assert(r);
  assert(r->numDeclOrComments() == 2);
  auto fnPrimary = r->declOrComment(1)->toFunction();
  assert(fnPrimary);
  auto callPrimary = m->stmt(2)->toFnCall();
  assert(callPrimary);

  // Resolve the module.
  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  // Get the type of 'r'.
  auto& qtR = typeForModuleLevelSymbol(context, r->id());

  // Assert some things about the primary call.
  auto& reCallPrimary = rr.byAst(callPrimary);
  auto& qtCallPrimary = reCallPrimary.type();
  assert(qtCallPrimary.type()->isVoidType());
  auto mscCallPrimary = reCallPrimary.mostSpecific().only();
  assert(mscCallPrimary);
  auto tfsCallPrimary = mscCallPrimary.fn();

  // Check the primary call receiver.
  assert(tfsCallPrimary->id() == fnPrimary->id());
  assert(tfsCallPrimary->numFormals() == 1);
  assert(tfsCallPrimary->formalName(0) == "this");
  assert(tfsCallPrimary->formalType(0).type() == qtR.type());
}

static void test6() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
      class A {
        var field: int;
        proc init() { }
      }
      class B : A {
        proc init() { }
      }
      class C : B {
        proc init() { }
      }

      extern proc foo(): unmanaged C;
      var obj = foo();
      var x = obj.field;
    )"""";

  QualifiedType initType = resolveTypeOfXInit(context, program);
  assert(initType.type()->isIntType());
}

static void test7() {
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program =
      R""""(
        record R {
          var x : int;

          proc type factory() do return 1;
        }

        var x = R.factory();
      )"""";

    QualifiedType initType = resolveTypeOfXInit(context, program);
    assert(initType.type()->isIntType());
  }
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program =
      R""""(
        record R {
          type T;
          var x : int;

          proc type factory() do return 1;
        }

        var x = R.factory();
      )"""";

    QualifiedType initType = resolveTypeOfXInit(context, program);
    assert(initType.type()->isIntType());
  }
}

static void runAndAssert(std::string program,
                         std::function<bool(QualifiedType)> fn) {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  QualifiedType initType = resolveTypeOfXInit(context, program);
  //assert(initType.type()->isStringType());
  assert(fn(initType));
}

//
// Test method signatures that use fields or methods in the same type.
//
static void test8() {
  std::string base =
  R"""(
    record R {
      param flag : bool;

      proc paramMethod() param : bool {
        return flag;
      }

      proc withDefaultField(arg = flag) {
        return "hello";
      }

      proc withDefault(arg = paramMethod()) {
        return "hello";
      }

      proc whereMethod() where paramMethod() {
        return "hello";
      }

      proc whereMethod() where !paramMethod() {
        return 5;
      }

      proc onlyFalse() where !paramMethod() {
        return 42.0;
      }

      proc whereField() where flag {
        return "hello";
      }

      proc whereField() where !flag {
        return 5;
      }
    }
  )""";

  auto isString = [](QualifiedType qt) { return qt.type()->isStringType(); };
  auto isInt    = [](QualifiedType qt) { return qt.type()->isIntType(); };

  // Resolve method using a sibling method as an argument's default
  runAndAssert(base + R""""(
    var r : R(false);
    var x = r.withDefault();
    )"""", isString);

  // Resolve method using a field as an argument's default value
  runAndAssert(base + R""""(
      var r : R(false);
      var x = r.withDefaultField();
    )"""", isString);

  // Resolve method using another method as the where-clause condition
  runAndAssert(base + R""""(
      var r : R(true);
      var x = r.whereMethod();
    )"""", isString);

  runAndAssert(base + R""""(
      var r : R(false);
      var x = r.whereMethod();
    )"""", isInt);

  // Resolve method using a field as the where-clause condition
  runAndAssert(base + R""""(
      var r : R(true);
      var x = r.whereField();
    )"""", isString);

  runAndAssert(base + R""""(
      var r : R(false);
      var x = r.whereField();
    )"""", isInt);

  // Ensure that methods whose where-clause always results in 'false' cannot
  // be called.
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = base + R""""(
      var r : R(true);
      var x = r.onlyFalse();
    )"""";

    QualifiedType initType = resolveTypeOfXInit(context, program);
    assert(guard.numErrors() == 1);
    assert(initType.type()->isErroneousType());
    assert(guard.error(0)->type() == chpl::NoMatchingCandidates);
    guard.realizeErrors();
  }
}

static void test9() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program = R"""(
    record R {
      type T;
      var field : T;
    }

    // Case 1: correctly call 'helper' in a where-clause when declared as a
    // secondary method on a generic record.
    proc R.helper() param do return field.type == int;
    proc R.foo() where helper() do return 5;
    proc R.foo() where !helper() do return "hello";

    // Case 2: correctly resolve the identifier 'T' implicitly referenced
    // within a where-clause of an instantiated method
    proc R.wrapper() param where T == int do return helper();
    proc R.baz() where wrapper() do return 5;
    proc R.baz() where !wrapper() do return "hello";

    var r : R(int);

    var x = r.foo();

    var y = r.baz();
    )""";

  auto results = resolveTypesOfVariables(context, program, {"x", "y"});
  assert(results["x"].type()->isIntType());
  assert(results["y"].type()->isIntType());
  assert(guard.numErrors() == 0);
}

static void test10() {
  // Ensure that secondary methods like 'proc x.myMethod()' are generic
  // even if 'x' is generic-with-defaults.
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program = R"""(
    record R {
      type T = int;
      var field : T;
    }

    proc R.myMethod(): T do return this.field;

    var r1: R(int);
    var r2: R(bool);

    var x1 = r1.myMethod();
    var x2 = r2.myMethod();
    )""";

  auto vars = resolveTypesOfVariables(context, program, { "x1", "x2" });

  auto t1 = vars.at("x1");
  assert(t1.type());
  assert(t1.type()->isIntType());
  assert(t1.type()->toIntType()->isDefaultWidth());

  auto t2 = vars.at("x2");
  assert(t2.type());
  assert(t2.type()->isBoolType());
}

static void test11() {
  // Type method calls are allowed on nilable classes; their receiver should
  // be generic over nilability and management.
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program = R"""(
    class C {
      proc type typeMethod() {
        return 42;
      }
    }
    var x1 = (C).typeMethod();
    var x2 = (owned C).typeMethod();
    var x3 = (borrowed C?).typeMethod();
  )""";

  auto vars = resolveTypesOfVariables(context, program, { "x1", "x2", "x3" });
  assert(!guard.realizeErrors());

  for (auto& [name, var] : vars) {
    std::ignore = name;
    assert(var.type());
    assert(var.type()->isIntType());
  }
}

static void test12() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program = R"""(
    class C {
      proc parenlessParam param {
        return 42;
      }
      proc parenlessType type {
        return this.type;
      }
    }

    var x: owned C? = new owned C?();
    param p1 = x.parenlessParam;
    type t1 = x.parenlessType;
  )""";

  auto vars = resolveTypesOfVariables(context, program, {"p1", "t1"});
  assert(!guard.realizeErrors());

  ensureParamInt(vars["p1"], 42);

  auto t1 = vars["t1"];
  assert(t1.type());
  assert(t1.type()->isClassType());
  assert(t1.type()->toClassType()->decorator().isNilable());
  assert(t1.type()->toClassType()->manager() == AnyOwnedType::get(context));
}

static void test13() {
  // Test fields that are deemed concrete by the initial type constructor
  // logic but are then discovered to be generic (this should produce errors).
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program = R"""(
    record r {
      var x: SecretlyGeneric;
      var y: poop(x.type);
      var z: SecretlyNotGeneric;
    }

    proc poop(type arg) type do return arg;
    proc SecretlyGeneric type do return owned class;
    proc SecretlyNotGeneric type do return int;

    var myR: r;
    var tmp = (myR.x, myR.y, myR.z);
    )""";

  auto vars = resolveTypesOfVariables(context, program, { "tmp" });

  assert(guard.numErrors() == 2);
  for (auto& err : guard.errors()) {
    assert(err->type() == ErrorType::SyntacticGenericityMismatch);
  }
  guard.realizeErrors();
}

static void test14() {
  // Test that the error from test13 is not spuriously emitted when inheritance
  // is in play.
  //
  // Original error in https://github.com/Cray/chapel-private/issues/6673,
  // where 'myFoo.asdf' would complain about asdf being a generic field.
  // It's not actually generic when the error is emitted; it's just that
  // fields of parent class Bar aren't instantiated yet. A robust check
  // (locked down by this issue) must ensure that all parent classes are
  // fully instantiated before issuing the error.
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program = R"""(
      enum strideKind { one }

      record Wrapper {
        param strides: strideKind = strideKind.one;
      }

      class Bar {
        param strides: strideKind;
      }

      class Foo: Bar(?) {
        var asdf : Wrapper(strides);

        proc init(param strides) {
          super.init(strides);
        }
      }

      var myFoo = new unmanaged Foo(strideKind.one);
      var myFooAsdf = myFoo.asdf;
      )""";

  auto vars = resolveTypesOfVariables(context, program, { "myFooAsdf" });
  assert(guard.realizeErrors() == 0);
}

static void test14b() {
  // Test that the error from test13 is correctly emitted when inheritance
  // is in play.
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program = R"""(
      enum strideKind { one }

      record Wrapper {
        param strides: strideKind = strideKind.one;
      }

      class Bar {
        param strides: strideKind;
      }

      class Foo: Bar(?) {
        var asdf : Wrapper(strides);
        var notConcrete: SecretlyGeneric;

        proc init(param strides) {
          super.init(strides);
        }
      }

      proc SecretlyGeneric type do return owned class;
      var myFoo = new unmanaged Foo(strideKind.one);
      var myFooAsdf = myFoo.asdf;
      var myNotConcrete = myFoo.notConcrete;
      )""";

  auto vars = resolveTypesOfVariables(context, program, { "myFooAsdf", "myNotConcrete" });
  assert(guard.realizeErrors() == 2);
}

static void test16() {
  // Test resolving 'this' call on variable that shadows field

  {
    // For automatic variable
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
        class Foo {
          var tup : 2*int;

          proc init() {}

          proc doSomething() {
            const tup = this.tup;
            return tup(0);
          }
        }

        var f = new Foo();
        var x = f.doSomething();
        )""";

    auto vars = resolveTypesOfVariables(context, program, { "x" });
    assert(guard.realizeErrors() == 0);
  }

  {
    // For formal
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
        class Foo {
          var tup : 2*int;

          proc init() {}

          proc doSomething(tup) {
            return tup(0);
          }
        }

        var f = new Foo();
        var anotherTup : 2*int;
        var x = f.doSomething(anotherTup);
        )""";

    auto vars = resolveTypesOfVariables(context, program, { "x" });
    assert(guard.realizeErrors() == 0);
  }
}

static void test17() {
  // Test resolving method calls from within param for loop.

  // Parenful
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
        class Foo {
          proc asdf() do return 3;

          proc doSomething() {
            for param i in 0..2 do
              return asdf();
          }
        }

        var f = new Foo();
        var x = f.doSomething();
        )""";

    QualifiedType initType = resolveTypeOfXInit(context, program);
    assert(initType.type());
    assert(initType.type()->isIntType());

    assert(guard.realizeErrors() == 0);
  }

  // Parenless
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
        class Foo {
          proc asdf do return 3;

          proc doSomething() {
            for param i in 0..2 do
              return asdf;
          }
        }

        var f = new Foo();
        var x = f.doSomething();
        )""";

    QualifiedType initType = resolveTypeOfXInit(context, program);
    assert(initType.type());
    assert(initType.type()->isIntType());

    assert(guard.realizeErrors() == 0);
  }
}

static void test18() {
  // test sync var method call isFull
  printf("test6\n");
  auto config = getConfigWithHome();
  Context ctx(config);
  Context* context = &ctx;
  ErrorGuard guard(context);
  setupModuleSearchPaths(context, false, false, {}, {});

  std::string program = R"""(
      var x : sync int;
      var y = x.isFull;
    )""";

  auto m = parseModule(context, std::move(program));
  auto results = resolveModule(context, m->id());
  auto var = findVariable(m, "y");
  auto init = var->initExpression();
  assert(init);
  auto qt = results.byAst(init).type();
  assert(qt.type()->isBoolType());
  assert(guard.numErrors() == 0);
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
  test9();
  test10();
  test11();
  test12();
  test13();
  test14();
  test14b();
  test16();
  test17();
  test18();

  return 0;
}

