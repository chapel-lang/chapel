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

static Context*
turnOnWarnUnstable(Context* ctx) {
  CompilerFlags flags;
  flags.set(CompilerFlags::WARN_UNSTABLE, true);
  setCompilerFlags(ctx, std::move(flags));
  assert(isCompilerFlagSet(ctx, CompilerFlags::WARN_UNSTABLE));
  return ctx;
}

static void test0(void) {
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

  // Get the 'foo' function and its declared variables.
  auto fooFn = mod->stmt(1)->toFunction();
  assert(fooFn && fooFn->numStmts() == 5);
  auto xVar = fooFn->stmt(0)->toVariable();
  auto yVar = fooFn->stmt(1)->toVariable();
  auto zVar = fooFn->stmt(2)->toVariable();
  assert(xVar && yVar && zVar);

  // Get the 'bar' function and its declared variables.
  auto barFn = fooFn->stmt(3)->toFunction();
  assert(barFn && barFn->numStmts() == 7);
  auto aVar = barFn->stmt(0)->toVariable();
  auto bVar = barFn->stmt(1)->toVariable();

  // Get the 'baz' function.
  auto bazFn = barFn->stmt(5)->toFunction();
  assert(bazFn && bazFn->numStmts() == 13);

  // The 'foo' function should have no outer variables.
  assert(computeOuterVariables(ctx, fooFn->id()) == nullptr);

  auto ovsBarPtr = computeOuterVariables(ctx, barFn->id());
  assert(ovsBarPtr);
  auto& ovsBar = *ovsBarPtr;
  assert(ovsBar.numVariables() == 3);
  assert(ovsBar.numMentions() == 3);

  assert(ovsBar.numMentions(xVar->id()) == 1);
  assert(ovsBar.firstMention(xVar->id()) == barFn->stmt(2)->id());

  assert(ovsBar.numMentions(yVar->id()) == 1);
  assert(ovsBar.firstMention(yVar->id()) == barFn->stmt(3)->id());

  assert(ovsBar.numMentions(zVar->id()) == 1);
  assert(ovsBar.firstMention(zVar->id()) == barFn->stmt(4)->id());

  // Not reaching as declared directly in parent.
  assert(!ovsBar.isReachingVariable(xVar->id()));
  assert(!ovsBar.isReachingVariable(yVar->id()));
  assert(!ovsBar.isReachingVariable(zVar->id()));

  auto ovsBazPtr = computeOuterVariables(ctx, bazFn->id());
  assert(ovsBazPtr);
  auto& ovsBaz = *ovsBazPtr;
  assert(ovsBaz.numVariables() == 5);
  assert(ovsBaz.numMentions() == 10);

  assert(ovsBaz.variable(0) == aVar->id());
  assert(ovsBaz.numMentions(aVar->id()) == 2);
  assert(ovsBaz.mention(aVar->id(), 0) == bazFn->stmt(0)->id());
  assert(ovsBaz.mention(aVar->id(), 1) == bazFn->stmt(6)->id());

  assert(ovsBaz.variable(1) == bVar->id());
  assert(ovsBaz.numMentions(bVar->id()) == 2);
  assert(ovsBaz.mention(bVar->id(), 0) == bazFn->stmt(1)->id());
  assert(ovsBaz.mention(bVar->id(), 1) == bazFn->stmt(7)->id());

  assert(ovsBaz.variable(2) == xVar->id());
  assert(ovsBaz.numMentions(xVar->id()) == 2);
  assert(ovsBaz.mention(xVar->id(), 0) == bazFn->stmt(2)->id());
  assert(ovsBaz.mention(xVar->id(), 1) == bazFn->stmt(8)->id());

  assert(ovsBaz.variable(3) == yVar->id());
  assert(ovsBaz.numMentions(yVar->id()) == 2);
  assert(ovsBaz.mention(yVar->id(), 0) == bazFn->stmt(3)->id());
  assert(ovsBaz.mention(yVar->id(), 1) == bazFn->stmt(9)->id());

  assert(ovsBaz.variable(4) == zVar->id());
  assert(ovsBaz.numMentions(zVar->id()) == 2);
  assert(ovsBaz.mention(zVar->id(), 0) == bazFn->stmt(4)->id());
  assert(ovsBaz.mention(zVar->id(), 1) == bazFn->stmt(10)->id());

  // Reaching because 'baz' has to reach across 'bar' into 'foo'.
  assert(ovsBaz.isReachingVariable(xVar->id()));
  assert(ovsBaz.isReachingVariable(yVar->id()));
  assert(ovsBaz.isReachingVariable(zVar->id()));

  // Module variables are not recorded as outer variables at the moment.
  assert(!ovsBaz.mentions(globalVar->id()));
  assert(ovsBaz.numMentions(globalVar->id()) == 0);
  assert(!guard.realizeErrors());
}

// This test demonstrates that it is safe to resolve a nested function like
// normal if the function does not refer to outer variables, regardless of
// whether or not the function is generic or concrete.
static void test1(void) {
  Context context;
  Context* ctx = turnOnWarnUnstable(&context);
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  // This snippet is a mockup of code from the internal modules.
  std::string program =
    R""""(
    proc isEnumType(type t) param {
      proc isEnumHelp(type t: enum) param do return true;
      proc isEnumHelp(type t) param do return false;
      return isEnumHelp(t);
    }

    enum colors { red, green, blue }

    proc foo(arg) {
      if isEnumType(arg.type) {
        return 42;
      } else {
        return "hello";
      }
    }

    var color = colors.red;
    var x = foo(color);
    )"""";

  auto qt = resolveQualifiedTypeOfX(ctx, program);
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type() && qt.type()->isIntType());
}

int main() {
  test0();
  test1();
  return 0;
}
