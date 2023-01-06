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

static void testDeprecationWarningsForTypes(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents = 
    R""""(
    module testDeprecationWarningsForTypes {

      deprecated
      record r1 { var x: int; }
      deprecated "- The record r2 is deprecated"
      record r2 { var y: int; }

      deprecated
      class c1 { var a: int; }
      deprecated "- The class c2 is deprecated"
      class c2 { var b: int; }

      deprecated
      union u1 { var f: int; }
      deprecated "- The union u2 is deprecated"
      union u2 { var g: int; }

      deprecated
      config const foo1 = 0;
      deprecated "- The config const foo2 is deprecated"
      config const foo2 = 0;

      deprecated
      config param bar1 = 0;
      deprecated "- The config param bar2 is deprecated"
      config param bar2 = 0;

      deprecated
      param baz1 = 0;
      deprecated "- The param baz2 is deprecated"
      param baz2 = 0;

      deprecated
      module mod1 {}
      deprecated "- The module mod2 is deprecated"
      module mod2 {}

      enum e1 {
        k1,
        deprecated
        k2,
        deprecated "- The enum element k3 is deprecated"
        k3,
        k4
      }

      deprecated
      enum e2 { k1 }

      deprecated "- The enum e3 is deprecated"
      enum e3 { k1 }

      var v1 = new r1();
      var v2 = new r2();
      var v3 = new c1();
      var v4 = new c2();
      var v5 = new u1();
      var v6 = new u2();
      var v7 = foo1;
      var v8 = foo2;
      var v9 = bar1;
      var v10 = bar2;
      var v11 = baz1;
      var v12 = baz2;

      {
        use mod1;
        use mod2;
      }

      {
        import mod1;
        import mod2;
      }

      var v13 = e1.k1;  // OK
      var v14 = e1.k2;
      var v15 = e1.k3;
      var v16 = e1.k4;  // OK

      var v17 = e2.k1;
      var v18 = e3.k1;
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Scope-resolve the module.
  std::ignore = scopeResolveModule(ctx, mod->id());

  // TODO: Helpers to make sure the errors are correct.
  assert(guard.realizeErrors());

  std::cout << std::endl;
}

static void testDeprecationWarningsForUseImport(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents = 
    R""""(
    deprecated "- The module foo is deprecated"
    module foo {
      deprecated
      var x: int;
      deprecated "The variable y is deprecated"
      var y: int;
      var z: int;
    }

    module bar {
      deprecated "The variable a is deprecated"
      var a: int;
    }

    module baz {
      deprecated "The proc p is deprecated"
      proc p() {}
      deprecated "The parenless proc f is deprecated"
      proc f { return 0; }
    }

    module ding {
      deprecated "The proc p(x: int) is deprecated"
      proc p(x: int) {}
      deprecated "The proc p(x: uint) is deprecated"
      proc p(x: uint) {}
      deprecated "The proc p(x: real) is deprecated"
      proc p(x: real) {}
    }

    module testDeprecationWarningsForUseImport {

      // Deprecated entries in 'only' lists.
      {
        use foo only x, y, z;
        use bar only a;
      }

      // Deprecated entries in import 'braces' lists.
      {
        import foo.{x, y, z};
        import bar.{a};
      }

      // Non-overloaded function and parenless should emit warning.
      {
        use baz only p, f;
        import baz.{p, f};
      }

      // Overloaded function should emit no warning.
      {
        use ding only p;
        import ding.{p};
      }
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 5);
  auto mod = br.topLevelExpression(4)->toModule();
  assert(mod);

  // Scope-resolve the module.
  std::ignore = scopeResolveModule(ctx, mod->id());

  // TODO: Helpers to make sure the errors are correct.
  assert(guard.realizeErrors());

  std::cout << std::endl;
}

int main() {
  testDeprecationWarningsForTypes();
  testDeprecationWarningsForUseImport();
  return 0;
}

