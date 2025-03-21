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
#include "./ErrorGuard.h"

#include <algorithm>
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
checkMentionedModules(Context* ctx, ID idMod, std::vector<const char*> expect) {
  auto& v = findMentionedModules(ctx, idMod);

  std::cout << "got mentioned modules:\n";
  for (const auto& id : v) {
    std::cout << "  " << id.str() << "\n";
  }

  assert(expect.size() == v.size());

  size_t i = 0;
  for (const auto& id : v) {
    assert(!id.isEmpty());

    auto ast = parsing::idToAst(ctx, id);
    assert(ast);
    auto mod = ast->toModule();
    assert(mod);

    assert(i < expect.size());
    std::string exp = expect[i];
    std::string got = id.isEmpty() ? "<>" : mod->name().c_str();
    std::cout << got << " == " << exp << std::endl;
    assert(got == exp);

    i++;
  }
}

static void testModulesNamedInUseOrImport(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
    module M1 {

    }
    module M2 {
      enum someEnum { red, green, blue }
    }
    module M3 {
      use M1;
      use M2;
      use someEnum;
    }
    )"""";

  setFileText(ctx, path, contents);

  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 3);
  auto M3 = br.topLevelExpression(2)->toModule();
  auto scope = scopeForId(ctx, M3->id());
  auto visStmts = resolveVisibilityStmts(ctx, scope);

  std::vector<ID> modulesMentioned;
  auto modulesMentionedIt = visStmts->modulesNamedInUseOrImport();
  std::copy(modulesMentionedIt.begin(), modulesMentionedIt.end(),
            std::back_inserter(modulesMentioned));
  assert(modulesMentioned.size() == 2);
  assert(modulesMentioned[0].str() == "M1");
  assert(modulesMentioned[1].str() == "M2");
}


static void testFindUse(void) {
  Context::Configuration config;
  config.chplHome = chplHome();
  Context context(config);
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
    module M0 {
      use Lib;
    }
    module M1 {
      use Lib as Foo;
    }
    module M2 {
      use Lib only Submodule;
    }
    module M3 {
      use Lib only Submodule as Bar;
    }
    module M4 {
      use Lib except Submodule;
    }
    module M5 {
      use Lib.Submodule;
    }

    module Lib {
      module Submodule { }
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 7);
  auto m0 = br.topLevelExpression(0)->toModule();
  assert(m0);
  auto m1 = br.topLevelExpression(1)->toModule();
  assert(m1);
  auto m2 = br.topLevelExpression(2)->toModule();
  assert(m2);
  auto m3 = br.topLevelExpression(3)->toModule();
  assert(m3);
  auto m4 = br.topLevelExpression(4)->toModule();
  assert(m4);
  auto m5 = br.topLevelExpression(5)->toModule();
  assert(m5);

  // check that we find the correct list of mentioned modules
  std::cout << "M0" << std::endl;
  checkMentionedModules(ctx, m0->id(), {"Lib"});
  std::cout << "M1" << std::endl;
  checkMentionedModules(ctx, m1->id(), {"Lib"});
  std::cout << "M2" << std::endl;
  checkMentionedModules(ctx, m2->id(), {"Lib", "Submodule"});
  std::cout << "M3" << std::endl;
  checkMentionedModules(ctx, m3->id(), {"Lib", "Submodule"});
  std::cout << "M4" << std::endl;
  checkMentionedModules(ctx, m4->id(), {"Lib"});
  std::cout << "M5" << std::endl;
  checkMentionedModules(ctx, m5->id(), {"Lib", "Submodule"});

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
    module M0 {
      import Lib;
    }
    module M1 {
      import Lib.Submodule;
    }
    module M2 {
      import Lib.{x, Submodule};
    }
    module M3 {
      import this.SubTwo.SubThree;
      module SubTwo {
        module SubThree { }
      }
    }

    module Lib {
      module Submodule { }
      var x: int;
    }

    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 5);
  auto m0 = br.topLevelExpression(0)->toModule();
  assert(m0);
  auto m1 = br.topLevelExpression(1)->toModule();
  assert(m1);
  auto m2 = br.topLevelExpression(2)->toModule();
  assert(m2);
  auto m3 = br.topLevelExpression(3)->toModule();
  assert(m3);

  // check that we find the correct list of mentioned modules
  std::cout << "M0" << std::endl;
  checkMentionedModules(ctx, m0->id(), {"Lib"});
  std::cout << "M1" << std::endl;
  checkMentionedModules(ctx, m1->id(), {"Lib", "Submodule"});
  std::cout << "M2" << std::endl;
  checkMentionedModules(ctx, m2->id(), {"Lib", "Submodule"});
  std::cout << "M3" << std::endl;
  checkMentionedModules(ctx, m3->id(), {"SubTwo", "SubThree"});

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
  checkMentionedModules(ctx, m1->id(), {"Sub1", "Sub2"});

  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

static void testFindMentionInherit(void) {
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
      class Child : Sub1.Sub2.Parent {}
      module Sub1 {
        module Sub2 {
          class Parent {}
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
  checkMentionedModules(ctx, m1->id(), {"Sub1", "Sub2"});

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
  checkMentionedModules(ctx, m1->id(), { });

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
  checkMentionedModules(ctx, m1->id(), {"M"});

  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

static void testFindImportSubmoduleIncluded(void) {
  Context::Configuration config;
  config.chplHome = chplHome();
  Context context(config);
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  std::cout << "testFindImportSubmoduleIncluded\n";

  setFileText(ctx, "TestMultipleModules.chpl",
    R""""(
      module TestMultipleModules {
        import MultipleModules;
        import MultipleModules.SubModule;
      }
    )"""");;

  setFileText(ctx, "MultipleModules.chpl",
    R""""(
      module MultipleModules {
        include module SubModule;
      }
    )"""");;

  setFileText(ctx, "MultipleModules/SubModule.chpl",
    R""""(
      module SubModule { }
    )"""");

  setModuleSearchPath(ctx, {UniqueString::get(ctx, ".")});

  // Get the module.
  auto& br = parseAndReportErrors(ctx, UniqueString::get(ctx, "TestMultipleModules.chpl"));
  assert(br.numTopLevelExpressions() == 1);
  auto m1 = br.topLevelExpression(0)->toModule();
  assert(m1);

  // check that we find the correct list of mentioned modules
  checkMentionedModules(ctx, m1->id(), {"MultipleModules", "SubModule"});

  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

static void testFindMentionSubmoduleIncluded(void) {
  Context::Configuration config;
  config.chplHome = chplHome();
  Context context(config);
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  std::cout << "testFindMentionSubmoduleIncluded\n";

  setFileText(ctx, "OuterModule.chpl",
    R""""(
      module OuterModule {
        include module SubModule;
        SubModule.foo();
      }
    )"""");;

  setFileText(ctx, "OuterModule/SubModule.chpl",
    R""""(
      module SubModule{
        proc foo() { }
      }
    )"""");

  setModuleSearchPath(ctx, {UniqueString::get(ctx, ".")});

  // Get the module.
  auto& br = parseAndReportErrors(ctx, UniqueString::get(ctx, "OuterModule.chpl"));
  assert(br.numTopLevelExpressions() == 1);
  auto m1 = br.topLevelExpression(0)->toModule();
  assert(m1);

  // check that we find the correct list of mentioned modules
  checkMentionedModules(ctx, m1->id(), {"SubModule"});

  std::cout << "---" << std::endl;

  assert(!guard.realizeErrors());
  std::cout << std::endl;
}

static void
checkModuleInitOrder(Context* ctx, ID idMod, std::vector<const char*> expect) {
  auto& v = moduleInitializationOrder(ctx, idMod, {});

  std::cout << "got module init order:\n";
  for (const auto& id : v) {
    std::cout << "  " << id.str() << "\n";
  }

  assert(expect.size() == v.size());

  size_t i = 0;
  for (const auto& id : v) {
    assert(!id.isEmpty());

    auto ast = parsing::idToAst(ctx, id);
    assert(ast);
    auto mod = ast->toModule();
    assert(mod);

    assert(i < expect.size());
    std::string exp = expect[i];
    std::string got = id.isEmpty() ? "<>" : mod->name().c_str();
    std::cout << got << " == " << exp << std::endl;
    assert(got == exp);

    i++;
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
  checkModuleInitOrder(ctx, m1->id(), {"M4", "M2", "M3", "M1"});


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
  checkModuleInitOrder(ctx, m1->id(), {"M3", "M2", "M1"});

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
  checkModuleInitOrder(ctx, m1->id(), {"Library", "Submodule", "Main"});

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
  checkModuleInitOrder(ctx, m1->id(), {"Submodule", "Main"});

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
  checkModuleInitOrder(ctx, m1->id(), {"Library", "Submodule", "Main"});

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
  checkModuleInitOrder(ctx, m1->id(), {"Main"});


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
  // test of the lower-level modulesNamedInUseOrImport
  testModulesNamedInUseOrImport();

  // tests of findMentionedModules
  testFindUse();
  testFindImport();
  testFindMention();
  testFindMentionInherit();
  testFindMentionFields();
  testFindMentionNotFields();
  testFindImportSubmoduleIncluded();
  testFindMentionSubmoduleIncluded();

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
