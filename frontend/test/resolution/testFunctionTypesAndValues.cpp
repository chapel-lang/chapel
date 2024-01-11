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

int main() {
  test0();
  return 0;
}

