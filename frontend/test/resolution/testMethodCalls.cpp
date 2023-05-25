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

// Test resolving a simple primary and secondary method in defining scope.
static void test1() {
  Context ctx;
  Context* context = &ctx;
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
  auto tfsCallPrimary = reCallPrimary.mostSpecific().only();
  assert(tfsCallPrimary);

  // Check the primary call receiver.
  assert(tfsCallPrimary->id() == fnPrimary->id());
  assert(tfsCallPrimary->numFormals() == 1);
  assert(tfsCallPrimary->formalName(0) == "this");
  assert(tfsCallPrimary->formalType(0).type() == qtR.type());

  // Assert some things about the secondary call.
  auto& reCallSecondary = rr.byAst(callSecondary);
  auto& qtCallSecondary = reCallSecondary.type();
  assert(qtCallSecondary.type()->isVoidType());
  auto tfsCallSecondary = reCallSecondary.mostSpecific().only();
  assert(tfsCallSecondary);

  // Check the secondary call receiver.
  assert(tfsCallSecondary->id() == fnSecondary->id());
  assert(tfsCallSecondary->numFormals() == 1);
  assert(tfsCallSecondary->formalName(0) == "this");
  assert(tfsCallSecondary->formalType(0).type() == qtR.type());
}

// Similar test but for parenless methods.
static void test2() {
  Context ctx;
  Context* context = &ctx;
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
  auto tfsCallPrimary = reCallPrimary.mostSpecific().only();
  assert(tfsCallPrimary);

  // Check the primary call receiver.
  assert(tfsCallPrimary->id() == fnPrimary->id());
  assert(tfsCallPrimary->numFormals() == 1);
  assert(tfsCallPrimary->formalName(0) == "this");
  assert(tfsCallPrimary->formalType(0).type() == qtR.type());

  // Assert some things about the secondary call.
  auto& reCallSecondary = rr.byAst(callSecondary);
  auto& qtCallSecondary = reCallSecondary.type();
  assert(qtCallSecondary.type()->isVoidType());
  auto tfsCallSecondary = reCallSecondary.mostSpecific().only();
  assert(tfsCallSecondary);

  // Check the secondary call receiver.
  assert(tfsCallSecondary->id() == fnSecondary->id());
  assert(tfsCallSecondary->numFormals() == 1);
  assert(tfsCallSecondary->formalName(0) == "this");
  assert(tfsCallSecondary->formalType(0).type() == qtR.type());
}

// test case to lock in correct behavior w.r.t. T being both a
// field and a formal.
static void test3() {
  Context ctx;
  Context* context = &ctx;
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
  assert(qt.type()->isRealType()); // and not real
}

static void test4() {
  Context ctx;
  Context* context = &ctx;
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
  Context ctx;
  Context* context = &ctx;
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
  auto tfsCallPrimary = reCallPrimary.mostSpecific().only();
  assert(tfsCallPrimary);

  // Check the primary call receiver.
  assert(tfsCallPrimary->id() == fnPrimary->id());
  assert(tfsCallPrimary->numFormals() == 1);
  assert(tfsCallPrimary->formalName(0) == "this");
  assert(tfsCallPrimary->formalType(0).type() == qtR.type());
}

static void test6() {
  Context ctx;
  Context* context = &ctx;
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
    Context ctx;
    Context* context = &ctx;
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
    Context ctx;
    Context* context = &ctx;
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


int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();

  return 0;
}

