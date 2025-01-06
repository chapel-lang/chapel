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
  Context context;
  auto ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME_FROM_FN_NAME(ctx);
  std::string contents =
    R""""(
    do {
      var x = true;
    } while x;
    )"""";
  setFileText(ctx, path, contents);

  auto& br = parseAndReportErrors(ctx, path);
  auto mod = br.singleModule();
  assert(mod);

  assert(mod->numStmts() == 1 && mod->stmt(0)->isDoWhile());
  auto doWhile = mod->stmt(0)->toDoWhile();
  auto decl = doWhile->stmt(0)->toVariable();
  auto condition = doWhile->condition()->toIdentifier();

  auto& rr = resolveModule(ctx, mod->id());
  assert(rr.byAst(condition).toId() == decl->id());
}

static void test2() {
  Context context;
  auto ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME_FROM_FN_NAME(ctx);
  std::string contents =
    R""""(
    var x = 42;
    do {
      var x = true;
    } while x;
    )"""";
  setFileText(ctx, path, contents);

  auto& br = parseAndReportErrors(ctx, path);
  auto mod = br.singleModule();
  assert(mod);

  assert(mod->numStmts() == 2 && mod->stmt(1)->isDoWhile());
  auto doWhile = mod->stmt(1)->toDoWhile();
  auto decl = doWhile->stmt(0)->toVariable();
  auto condition = doWhile->condition()->toIdentifier();

  auto& rr = resolveModule(ctx, mod->id());
  assert(rr.byAst(condition).toId() == decl->id());
}

static void test3() {
  Context context;
  auto ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME_FROM_FN_NAME(ctx);
  std::string contents =
    R""""(
    do class C {

    } while C != int;
    )"""";
  setFileText(ctx, path, contents);

  auto& br = parseAndReportErrors(ctx, path);
  auto mod = br.singleModule();
  assert(mod);

  assert(mod->numStmts() == 1 && mod->stmt(0)->isDoWhile());
  auto doWhile = mod->stmt(0)->toDoWhile();
  auto decl = doWhile->stmt(0)->toClass();
  auto ident = doWhile->condition()->toOpCall()->child(0)->toIdentifier();

  auto& rr = resolveModule(ctx, mod->id());
  assert(rr.byAst(ident).toId() == decl->id());
}

static void testScopeForId1() {
  Context context;
  auto ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME_FROM_FN_NAME(ctx);
  std::string contents =
    R""""(
    do class C {

    } while C.type != int;
    )"""";
  setFileText(ctx, path, contents);

  auto& br = parseAndReportErrors(ctx, path);
  auto mod = br.singleModule();
  assert(mod);

  assert(mod->numStmts() == 1 && mod->stmt(0)->isDoWhile());
  auto doWhile = mod->stmt(0)->toDoWhile();
  auto block = doWhile->body();
  auto cond = doWhile->condition();

  assert(resolution::scopeForId(ctx, cond->id())->id() == block->id());
}

int main() {
  test1();
  test2();
  test3();

  testScopeForId1();
}
