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

#include <cstdarg>

#define TEST_NAME(ctx__)\
  chpl::UniqueString::getConcat(ctx__, __FUNCTION__, ".chpl")

static std::string debugDeclName = "";

static std::string chplHome() {
  const char* chpl_home_env = getenv("CHPL_HOME");
  CHPL_ASSERT(chpl_home_env && "This test requires $CHPL_HOME!");
  auto ret = std::string(chpl_home_env);
  return ret;
}

static void 
checkModuleInitOrder(Context* ctx, bool isCheckForOrigin, ID idMod, ...) {
  auto& v = moduleInitializationOrder(ctx, idMod);
  va_list args;
  va_start(args, idMod);

  for (auto& p : v) {
    auto id = isCheckForOrigin ? p.second : p.first;
    assert(isCheckForOrigin || !id.isEmpty());

    auto ast = !id.isEmpty() ? parsing::idToAst(ctx, id) : nullptr;
    auto mod = ast ? ast->toModule() : nullptr;
    assert(isCheckForOrigin || (ast && mod));

    std::string expect = va_arg(args, const char*);
    std::string got = id.isEmpty() ? "<>" : mod->name().c_str();
    std::cout << got << " == " << expect << std::endl;
    assert(got == expect);
  }
}

static void test0(void) {
  Context::Configuration config;
  config.chplHome = chplHome();
  Context context(config);
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  // Do NOT set the module search paths for this test. Do not want to
  // compute the standard/internal modules in this ordering.
  /** setupModuleSearchPaths(ctx, false, false, {}, {}); */

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  // Remove the functions, and replace their invocations with 'writeln',
  // and this test is pretty much identical to the one given in the spec
  // section on module initialization order. Note that the function calls
  // are needed regardless to ensure that each module is considered
  // "non-trivial". Future iterations of the algorithm may elide trivial
  // modules.
  std::string contents = 
    R""""(
    module M1 {
      use M2.M3;
      use M2;
      proc a() {}
      proc main() { a(); }
    }

    module M2 {
      use M4;
      proc b() {}
      b();

      module M3 {
        proc c() {}
        c();
      }
    }

    module M4 {
      proc d() {}
      d();
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 3);
  auto m1 = br.topLevelExpression(0)->toModule();
  assert(m1);

  const bool CHECK_FOR_ORIGIN = true;

  // First check to make sure the initialization order is correct.
  checkModuleInitOrder(ctx, !CHECK_FOR_ORIGIN, m1->id(),
                       "M4", "M2", "M3", "M1");

  // Now check to make sure the trigger order is correct.
  checkModuleInitOrder(ctx, CHECK_FOR_ORIGIN, m1->id(),
                       "M2", "M3", "M1", "<>");

  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

// Make sure module initialization order can handle circular dependencies.
static void test1(void) {
  Context::Configuration config;
  config.chplHome = chplHome();
  Context context(config);
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  // Do NOT set the module search paths for this test. Do not want to
  // compute the standard/internal modules in this ordering.
  /** setupModuleSearchPaths(ctx, false, false, {}, {}); */

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents = 
    R""""(
    module M1 {
      use M2;
      proc foo() {}
      foo();
    }

    module M2 {
      use M1;
      use M3;
      proc foo() {}
      foo();
    }

    module M3 {
      use M1;
      use M2;
      proc foo() {}
      foo();
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 3);
  auto m1 = br.topLevelExpression(0)->toModule();
  assert(m1);

  const bool CHECK_FOR_ORIGIN = true;

  // First check to make sure the initialization order is correct.
  checkModuleInitOrder(ctx, !CHECK_FOR_ORIGIN, m1->id(),
                       "M3", "M2", "M1");

  // Now check to make sure the trigger order is correct.
  checkModuleInitOrder(ctx, CHECK_FOR_ORIGIN, m1->id(),
                       "M2", "M1", "<>");

  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

int main() {
  test0();
  test1();
  return 0;
}
