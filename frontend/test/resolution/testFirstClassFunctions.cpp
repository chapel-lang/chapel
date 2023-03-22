/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

static std::string debugDeclName = "";

static void testDebuggingBreakpoint() {}

static Context*
turnOnWarnUnstable(Context* ctx) {
  testDebuggingBreakpoint();
  CompilerFlags flags;
  flags.set(CompilerFlags::WARN_UNSTABLE, true);
  setCompilerFlags(ctx, std::move(flags));
  assert(isCompilerFlagSet(ctx, CompilerFlags::WARN_UNSTABLE));
  return ctx;
}

// Check the type of a simple function type.
static void test0(void) {
  Context context;
  Context* ctx = turnOnWarnUnstable(&context);
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
    proc main() {
      type T = proc(_: int, _: int): int;
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the top module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Force resolve 'main' since we may not always do that yet.
  assert(mod->numStmts() == 1);
  const Function* mainFn = mod->stmt(0)->toFunction();
  auto rf = resolveConcreteFunction(ctx, mainFn->id());

  assert(mainFn->numStmts() == 1);
  auto tv = mainFn->stmt(0)->toVariable();
  assert(tv);
  auto procType = tv->initExpression();
  assert(procType && procType->isFunctionSignature());
  auto& re = rf->byAst(procType);

  re.type().dump();

  assert(!guard.realizeErrors());
}

// Compute outer variables. Notice that we just call 'scopeResolve', since
// getting the types of outer variables is a complicated process that the
// compiler can't quite handle yet.
static void test1(void) {
  Context context;
  Context* ctx = turnOnWarnUnstable(&context);
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
    var global = 0;
    proc foo() {
      var x = 0; var y = 0; var z = 0;
      proc bar() {
        var a = 0; var b = 0;
        x; y; z;
        proc baz() {
          a; b; x; y; z; global;
          return 0;
        }
        return baz();
      }
      return bar();
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the top module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod && mod->numStmts() == 2);
  auto globalVar = mod->stmt(0)->toVariable();
  
  auto fooFn = mod->stmt(1)->toFunction();
  assert(fooFn && fooFn->numStmts() == 5);
  auto xVar = fooFn->stmt(0)->toVariable();
  auto yVar = fooFn->stmt(1)->toVariable();
  auto zVar = fooFn->stmt(2)->toVariable();
  assert(xVar && yVar && zVar);

  auto barFn = fooFn->stmt(3)->toFunction();
  assert(barFn && barFn->numStmts() == 7);
  auto aVar = barFn->stmt(0)->toVariable();
  auto bVar = barFn->stmt(1)->toVariable();

  auto bazFn = barFn->stmt(5)->toFunction();
  assert(bazFn && bazFn->numStmts() == 7);

  auto rfFoo = scopeResolveFunction(ctx, fooFn->id());
  assert(rfFoo->outerVariableList().empty());

  auto rfBar = scopeResolveFunction(ctx, barFn->id());
  assert(rfBar->outerVariableList().size() == 3);
  assert(rfBar->outerVariableList()[0].first == xVar->id());
  assert(rfBar->outerVariableList()[0].second == barFn->stmt(2)->id());
  assert(rfBar->outerVariableList()[1].first == yVar->id());
  assert(rfBar->outerVariableList()[1].second == barFn->stmt(3)->id());
  assert(rfBar->outerVariableList()[2].first == zVar->id());
  assert(rfBar->outerVariableList()[2].second == barFn->stmt(4)->id());

  auto rfBaz = scopeResolveFunction(ctx, bazFn->id());
  assert(rfBaz->outerVariableList().size() == 6);
  assert(rfBaz->outerVariableList()[0].first == aVar->id());
  assert(rfBaz->outerVariableList()[0].second == bazFn->stmt(0)->id());
  assert(rfBaz->outerVariableList()[1].first == bVar->id());
  assert(rfBaz->outerVariableList()[1].second == bazFn->stmt(1)->id());
  assert(rfBaz->outerVariableList()[2].first == xVar->id());
  assert(rfBaz->outerVariableList()[2].second == bazFn->stmt(2)->id());
  assert(rfBaz->outerVariableList()[3].first == yVar->id());
  assert(rfBaz->outerVariableList()[3].second == bazFn->stmt(3)->id());
  assert(rfBaz->outerVariableList()[4].first == zVar->id());
  assert(rfBaz->outerVariableList()[4].second == bazFn->stmt(4)->id());
  assert(rfBaz->outerVariableList()[5].first == globalVar->id());
  assert(rfBaz->outerVariableList()[5].second == bazFn->stmt(5)->id());

  assert(!guard.realizeErrors());
}

int main() {
  test0();
  test1();
  return 0;
}

