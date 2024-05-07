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
checkMentionedModules(Context* ctx, ID idMod, ...) {
  auto& v = findMentionedModules(ctx, idMod);

  for (const auto& id : v) {
    std::cout << "  " << id.str() << "\n";
  }

  va_list args;
  va_start(args, idMod);

  for (const auto& id : v) {
    assert(!id.isEmpty());

    auto ast = parsing::idToAst(ctx, id);
    auto mod = ast ? ast->toModule() : nullptr;
    assert(ast && mod);

    std::string expect = va_arg(args, const char*);
    std::string got = id.isEmpty() ? "<>" : mod->name().c_str();
    std::cout << got << " == " << expect << std::endl;
    assert(got == expect);
  }
}


static void testFindSimple(void) {
  Context::Configuration config;
  config.chplHome = chplHome();
  Context context(config);
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
    module M1 {
      use M2;
    }
    module M2 { }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 2);
  auto m1 = br.topLevelExpression(0)->toModule();
  assert(m1);

  // check that we find the correct list of mentioned modules
  checkMentionedModules(ctx, m1->id(), "M2");

  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

static void testFindImport(void) {
  Context::Configuration config;
  config.chplHome = chplHome();
  Context context(config);
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
    module M1 {
      import M2.Sub;
    }
    module M2 {
      module Sub { }
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 2);
  auto m1 = br.topLevelExpression(0)->toModule();
  assert(m1);

  // check that we find the correct list of mentioned modules
  checkMentionedModules(ctx, m1->id(), "Sub");

  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

static void testFindMention(void) {
  Context::Configuration config;
  config.chplHome = chplHome();
  Context context(config);
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
    module M1 {
      Sub1.Sub2.foo();
      module Sub1 {
        module Sub2 {
          proc foo() { }
        }
      }
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto m1 = br.topLevelExpression(0)->toModule();
  assert(m1);

  // check that we find the correct list of mentioned modules
  checkMentionedModules(ctx, m1->id(), "Sub1", "Sub2");

  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

static void testFindMentionFields(void) {
  Context::Configuration config;
  config.chplHome = chplHome();
  Context context(config);
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
      module Program {
        module M {
          var M: real;
        }

        record r1 { var M: int; }
        record r2 { var M: r1; }

        proc r2.foo() {
          return M.M;
        }

        proc main() {
          var x: r2;
          var y = x.foo();
        }
      }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto m1 = br.topLevelExpression(0)->toModule();
  assert(m1);

  // check that we find the correct list of mentioned modules
  checkMentionedModules(ctx, m1->id());

  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

static void testFindMentionNotFields(void) {
  Context::Configuration config;
  config.chplHome = chplHome();
  Context context(config);
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
      module Program {
        module M {
          var M: real;
        }

        record r1 { var N: int; }
        record r2 { var N: r1; }

        proc r2.foo() {
          return M.M;
        }

        proc main() {
          var x: r2;
          var y = x.foo();
        }
      }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto m1 = br.topLevelExpression(0)->toModule();
  assert(m1);

  // check that we find the correct list of mentioned modules
  checkMentionedModules(ctx, m1->id(), "M");

  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

static void
checkModuleInitOrder(Context* ctx, ID idMod, ...) {
  auto& v = moduleInitializationOrder(ctx, idMod, {});

  for (const auto& id : v) {
    std::cout << "  " << id.str() << "\n";
  }

  va_list args;
  va_start(args, idMod);

  for (const auto& id : v) {
    assert(!id.isEmpty());

    auto ast = !id.isEmpty() ? parsing::idToAst(ctx, id) : nullptr;
    auto mod = ast ? ast->toModule() : nullptr;
    assert(ast && mod);

    std::string expect = va_arg(args, const char*);
    std::string got = id.isEmpty() ? "<>" : mod->name().c_str();
    std::cout << got << " == " << expect << std::endl;
    assert(got == expect);
  }
}

static void testSpec(void) {
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

  // check to make sure the initialization order is correct.
  checkModuleInitOrder(ctx, m1->id(), "M4", "M2", "M3", "M1");


  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

// Make sure module initialization order can handle circular dependencies.
static void testCircular(void) {
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

  // check to make sure the initialization order is correct.
  checkModuleInitOrder(ctx, m1->id(), "M3", "M2", "M1");

  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

static void testImportSub(void) {
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

  // test that dead modules / submodules do not contribute
  // to the initialization ordering.
  std::string contents =
    R""""(
    module Main {
      import this.Library.Submodule;
      proc main() { }
      module Library {
        module Submodule { }
      }
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto m1 = br.topLevelExpression(0)->toModule();
  assert(m1);

  // check to make sure the initialization order is correct.
  checkModuleInitOrder(ctx, m1->id(), "Library", "Submodule", "Main");

  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

static void testMentionedSubmodule(void) {
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

  // test that dead modules / submodules do not contribute
  // to the initialization ordering.
  std::string contents =
    R""""(
    module Main {
      module Submodule {
        proc foo() { }
      }

      proc main() {
        Submodule.foo();
      }
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto m1 = br.topLevelExpression(0)->toModule();
  assert(m1);

  // check to make sure the initialization order is correct.
  checkModuleInitOrder(ctx, m1->id(), "Submodule", "Main");

  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

static void testMentionedLibrarySub(void) {
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

  // test that dead modules / submodules do not contribute
  // to the initialization ordering.
  std::string contents =
    R""""(
    module Main {
      import Library;
      proc main() {
        Library.Submodule.foo();
      }
    }
    module Library {
      module Submodule {
        proc foo() { }
      }
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 2);
  auto m1 = br.topLevelExpression(0)->toModule();
  assert(m1);

  // check to make sure the initialization order is correct.
  checkModuleInitOrder(ctx, m1->id(), "Library", "Submodule", "Main");

  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}


static void testDeadModule(void) {
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

  // test that dead modules / submodules do not contribute
  // to the initialization ordering.
  std::string contents =
    R""""(
    module Main {
      module UnusedSubmodule { }
      proc main() { a(); }
    }

    module UnusedToplevel {
      module UnusedTwo {
        use UnusedToplevel;
      }
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 2);
  auto m1 = br.topLevelExpression(0)->toModule();
  assert(m1);

  // check to make sure the initialization order is correct.
  checkModuleInitOrder(ctx, m1->id(), "Main");


  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

static void testBundled(void) {
  // this test just prints out the module init order for
  // the bundled/internal module. As a test, it makes
  // sure that this completes without error; however it might
  // have use in manual testing.

  Context::Configuration config;
  config.chplHome = chplHome();
  Context context(config);
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  setupModuleSearchPaths(ctx, false, false, {}, {});

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
    module Main {
      proc main() { }
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto m1 = br.topLevelExpression(0)->toModule();
  assert(m1);

  auto& v = moduleInitializationOrder(ctx, m1->id(), {});

  printf("Module initialization order:\n");
  for (const auto& id : v) {
    printf(" %s\n", id.str().c_str());
  }

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}


int main() {
  // tests of findMentionedModules
  testFindSimple();
  testFindImport();
  testFindMention();
  testFindMentionFields();
  testFindMentionNotFields();

  // tests of moduleInitializationOrder
  testSpec();
  testCircular();
  testImportSub();
  testMentionedSubmodule();
  testMentionedLibrarySub();
  testDeadModule();
  testBundled();
  return 0;
}
