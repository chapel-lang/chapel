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
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

static void test1() {
  auto context = buildStdContext();
  QualifiedType qt =  resolveQualifiedTypeOfX(context,
                         R""""(
                         var x : if true then string else "not-a-type";
                         )"""");
  assert(qt.type() && qt.type()->isStringType());
}

static void test2() {
  auto context = buildStdContext();
  QualifiedType qt =  resolveQualifiedTypeOfX(context,
                         R""""(
                         var x : if false then "not-a-type" else int;
                         )"""");
  assert(qt.type() && qt.type()->isIntType());
}

static void test3() {
  auto context = buildStdContext();
  QualifiedType qt =  resolveQualifiedTypeOfX(context,
                         R""""(
                         var b : bool;
                         var x = if b then 0 else "string";
                         )"""");
  qt.dump();
  std::cout << std::endl;
  assert(qt.isErroneousType());
}

static void test4() {
  auto context = buildStdContext();
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         var b : bool;
                         var x = if b then 1 else 0;
                         )"""");
  qt.dump();
  std::cout << std::endl;
  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type() && qt.type()->isIntType());
}

static void test5() {
  auto context = buildStdContext();
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         record r {}
                         var temp: r;
                         const ref tempRef = temp;
                         var b : bool;
                         var x = if b then tempRef else new r();
                         )"""");
  qt.dump();
  std::cout << std::endl;
  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type() && qt.type()->isRecordType());
}

static void test6() {
  auto context = buildStdContext();
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         param b : bool;
                         var x = if b then 1 else "hello";
                         )"""", false);
  qt.dump();
  std::cout << std::endl;
  assert(qt.kind() == QualifiedType::UNKNOWN);
}

static void testIfVarErrorUseInElseBranch1() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);

  auto path = TEST_NAME_FROM_FN_NAME(ctx);
  std::string contents =
    R""""(
    class C {}
    proc foo(x) {}
    if var x = new C() then foo(x); else foo(x);
    )"""";
  setFileText(ctx, path, contents);

  auto& br = parseAndReportErrors(ctx, path);
  auto mod = br.singleModule();
  assert(mod);

  assert(mod->numStmts() == 3 && mod->stmt(2)->isConditional());
  auto cond = mod->stmt(2)->toConditional();
  assert(cond->hasElseBlock());
  assert(cond->numThenStmts() == 1 && cond->numElseStmts() == 1);
  auto var = cond->condition()->toVariable();
  assert(var);

  auto ifCall = cond->thenStmt(0)->toFnCall();
  assert(ifCall && ifCall->numActuals() == 1);
  auto ifUse = ifCall->actual(0)->toIdentifier();
  assert(ifUse);

  auto elseCall = cond->elseStmt(0)->toFnCall();
  assert(elseCall && elseCall->numActuals() == 1);
  auto elseUse = elseCall->actual(0)->toIdentifier();
  assert(elseUse);

  auto& rr = resolveModule(ctx, mod->id());
  auto& reIfUse = rr.byAst(ifUse);
  auto& reElseUse = rr.byAst(elseUse);

  assert(reIfUse.toId() == var->id());
  assert(reElseUse.toId().isEmpty());
  assert(reElseUse.type().isUnknown());

  assert(guard.realizeErrors() > 0);
}

// In this variation the use is in a 'else-if'.
static void testIfVarErrorUseInElseBranch2() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);

  auto path = TEST_NAME_FROM_FN_NAME(ctx);
  std::string contents =
    R""""(
    class C {}
    proc foo(x) {}
    if var x = new C() {
      foo(x);
    } else if var y = new C() {
      foo(y);
      foo(x);
    }
    )"""";
  setFileText(ctx, path, contents);

  auto& br = parseAndReportErrors(ctx, path);
  auto mod = br.singleModule();
  assert(mod && mod->numStmts() == 3);
  auto cond = mod->stmt(2)->toConditional();
  assert(cond && cond->hasElseBlock() && cond->numElseStmts() == 1);
  auto var = cond->condition()->toVariable();
  assert(var);

  auto elseIf = cond->elseStmt(0)->toConditional();
  assert(elseIf && elseIf->numThenStmts() == 2);
  auto elseIfVar = elseIf->condition()->toVariable();
  assert(elseIfVar);
  auto elseIfCall1 = elseIf->thenStmt(0)->toFnCall();
  auto elseIfCall2 = elseIf->thenStmt(1)->toFnCall();
  assert(elseIfCall1 && elseIfCall1->numActuals() == 1);
  assert(elseIfCall2 && elseIfCall2->numActuals() == 1);
  auto useY = elseIfCall1->actual(0)->toIdentifier();
  auto useX = elseIfCall2->actual(0)->toIdentifier();
  assert(useY && useX);

  auto& rr = resolveModule(ctx, mod->id());
  auto& reUseY = rr.byAst(useY);
  auto& reUseX = rr.byAst(useX);

  assert(reUseY.toId() == elseIfVar->id());
  assert(reUseX.toId().isEmpty());
  assert(reUseX.type().isUnknown());

  assert(guard.numErrors() > 0);
  guard.realizeErrors();
}

// In this variation the use is in a deeply nested block.
static void testIfVarErrorUseInElseBranch3() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);

  auto path = TEST_NAME_FROM_FN_NAME(ctx);
  std::string contents =
    R""""(
    class C {}
    proc foo(x) {}
    if var x = new C() then foo(x); else { { { foo(x); } } }
    )"""";
  setFileText(ctx, path, contents);

  auto& br = parseAndReportErrors(ctx, path);
  auto mod = br.singleModule();
  assert(mod);

  assert(mod->numStmts() == 3 && mod->stmt(2)->isConditional());
  auto cond = mod->stmt(2)->toConditional();
  auto var = cond->condition()->toVariable();
  assert(var);

  auto elseBlock2 = cond->elseStmt(0)->toBlock();
  assert(elseBlock2 && elseBlock2->numStmts());
  auto elseBlock3 = elseBlock2->stmt(0)->toBlock();
  assert(elseBlock3 && elseBlock3->numStmts());

  auto elseCall = elseBlock3->stmt(0)->toFnCall();
  assert(elseCall);

  assert(elseCall && elseCall->numActuals() == 1);
  auto elseUse = elseCall->actual(0)->toIdentifier();
  assert(elseUse);

  auto& rr = resolveModule(ctx, mod->id());
  auto& reElseUse = rr.byAst(elseUse);
  assert(reElseUse.toId().isEmpty());
  assert(reElseUse.type().isUnknown());

  assert(guard.numErrors() > 0);
  guard.realizeErrors();
}

// Uses of two if-vars with the same name across multiple branches.
static void testIfVarErrorUseInElseBranch4() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);

  auto path = TEST_NAME_FROM_FN_NAME(ctx);
  std::string contents =
    R""""(
    class C {}
    proc foo(x) {}
    if var x = new C() {
      foo(x);
    } else if var x = new C() {
      foo(x);
    } else {
      x;
    }
    )"""";
  setFileText(ctx, path, contents);

  auto& br = parseAndReportErrors(ctx, path);
  auto mod = br.singleModule();
  assert(mod && mod->numStmts() == 3);

  auto cond = mod->stmt(2)->toConditional();
  assert(cond && cond->hasElseBlock() && cond->numElseStmts() == 1);
  assert(cond->numThenStmts() == 1);

  auto ifVar = cond->condition()->toVariable();
  assert(ifVar);

  auto ifCall = cond->thenStmt(0)->toFnCall();
  assert(ifCall->numActuals() == 1 && ifCall->actual(0)->isIdentifier());
  auto ifUseX = ifCall->actual(0)->toIdentifier();

  auto elseIf = cond->elseStmt(0)->toConditional();
  assert(elseIf && elseIf->numThenStmts() == 1);
  assert(elseIf->hasElseBlock());

  auto elseIfVar = elseIf->condition()->toVariable();
  assert(elseIfVar);

  auto elseIfCall = elseIf->thenStmt(0)->toFnCall();
  assert(elseIfCall && elseIfCall->numActuals() == 1);
  auto elseIfUseX = elseIfCall->actual(0)->toIdentifier();
  assert(elseIfUseX);

  assert(elseIf->numElseStmts() == 1);
  auto elseUseX = elseIf->elseStmt(0)->toIdentifier();
  assert(elseUseX);

  auto& rr = resolveModule(ctx, mod->id());
  auto& reIfVar = rr.byAst(ifVar);
  auto& reIfUseX = rr.byAst(ifUseX);
  auto& reElseIfVar = rr.byAst(elseIfVar);
  auto& reElseIfUseX = rr.byAst(elseIfUseX);
  auto& reElseUseX = rr.byAst(elseUseX);

  assert(reIfUseX.toId() == ifVar->id());
  assert(reIfUseX.type().type() == reIfVar.type().type());

  assert(reElseIfUseX.toId() == elseIfVar->id());
  assert(reElseIfUseX.type().type() == reElseIfVar.type().type());

  assert(reElseUseX.toId().isEmpty());
  assert(reElseUseX.type().isUnknown());

  assert(guard.numErrors() > 0);
  guard.realizeErrors();
}

static void testIfVarErrorNonClassType() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);

  auto path = TEST_NAME_FROM_FN_NAME(ctx);
  std::string contents =
    R""""(
    proc foo(x) {}
    if var x = 1 then foo(x); else;
    )"""";
  setFileText(ctx, path, contents);

  auto& br = parseAndReportErrors(ctx, path);
  auto mod = br.singleModule();
  assert(mod);

  assert(mod->numStmts() == 2 && mod->stmt(1)->isConditional());
  auto cond = mod->stmt(1)->toConditional();
  auto var = cond->condition()->toVariable();
  assert(var);

  auto& rr = resolveModule(ctx, mod->id());
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->message() ==
         "a variable declared in the condition of an if statement must "
         "be a class, but it has non-class type 'int(64)'");
  auto& reVar = rr.byAst(var);
  assert(reVar.type().kind() == QualifiedType::VAR);
  assert(reVar.type().type() == IntType::get(ctx, 64));
  guard.realizeErrors();
}

static void testIfVarNonNilInThen() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);

  auto path = TEST_NAME_FROM_FN_NAME(ctx);
  std::string contents =
    R""""(
    class Bar {
      var _msg : string;
      proc message():string {
        return _msg;
      }
    }

    proc foo() {
      var e : owned Bar? = new Bar("foo");
      return e;
    }

    if var err = foo() {
      var x = err.message();
    }
    )"""";
  setFileText(ctx, path, contents);

  auto& br = parseAndReportErrors(ctx, path);
  auto mod = br.singleModule();
  assert(mod);

  auto& rr = resolveModule(ctx, mod->id());
  (void)rr;
  assert(guard.numErrors() == 0);
  guard.realizeErrors();
}

static void testIfVarBorrow() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R"""(
    class C {
      var i : int;
    }

    proc retClass() : owned C? {
      return new C(5);
    }

    if const obj = retClass() {
      var x = obj.i;
    }
    )""";

  auto vars = resolveTypesOfVariables(context, program, {"x", "obj"});
  assert(vars["x"].type()->isIntType());
  auto obj = vars["obj"].type()->toClassType();
  assert(obj->decorator().isNonNilable());
  assert(obj->decorator().isBorrowed());
  assert(obj->basicClassType()->name() == "C");
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  testIfVarBorrow();
  testIfVarErrorUseInElseBranch1();
  testIfVarErrorUseInElseBranch2();
  testIfVarErrorUseInElseBranch3();
  testIfVarErrorUseInElseBranch4();
  testIfVarErrorNonClassType();
  testIfVarNonNilInThen();

  return 0;
}
