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

static void test1() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveQualifiedTypeOfX(context,
                         R""""(
                         var x : if true then string else "not-a-type";
                         )"""");
  assert(qt.type() && qt.type()->isStringType());
}

static void test2() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveQualifiedTypeOfX(context,
                         R""""(
                         var x : if false then "not-a-type" else int;
                         )"""");
  assert(qt.type() && qt.type()->isIntType());
}

static void test3() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveQualifiedTypeOfX(context,
                         R""""(
                         var b : bool;
                         var x = if b then 0 else "string";
                         )"""");
  qt.dump();
  assert(qt.isErroneousType());
}

static void test4() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         var b : bool;
                         var x = if b then 1 else 0;
                         )"""");
  qt.dump();
  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type() && qt.type()->isIntType());
}

static void test5() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         record r {}
                         var temp: r;
                         const ref tempRef = temp;
                         var b : bool;
                         var x = if b then tempRef else new r();
                         )"""");
  qt.dump();
  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type() && qt.type()->isRecordType());
}

static void test6() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         param b : bool;
                         var x = if b then 1 else "hello";
                         )"""", false);
  qt.dump();
  assert(qt.kind() == QualifiedType::UNKNOWN);
}

static void testIfVarErrorUseInElseBranch() {
  Context context;
  auto ctx = &context;
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
  auto var = cond->condition()->toVariable();
  assert(var);
  auto elseCall = cond->elseStmt(0)->toFnCall();
  assert(elseCall && elseCall->numActuals() == 1);
  auto elseUse = elseCall->actual(0)->toIdentifier();
  assert(elseUse);

  auto& rr = resolveModule(ctx, mod->id());
  auto& reElseUse = rr.byAst(elseUse);
  assert(reElseUse.toId().isEmpty());
  assert(reElseUse.type().isUnknown());
}

static void testIfVarErrorNonClassType() {
  Context context;
  auto ctx = &context;
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
         "be a class, not a value of type 'int(64)'");
  auto& reVar = rr.byAst(var);
  assert(reVar.type().kind() == QualifiedType::VAR);
  assert(reVar.type().type() == IntType::get(ctx, 64));
  guard.realizeErrors();
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  testIfVarErrorUseInElseBranch();
  testIfVarErrorNonClassType();
  return 0;
}
