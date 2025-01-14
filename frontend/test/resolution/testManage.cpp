/*
 * Copyright 2024-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "test-resolution.h"

static void testBasic() {
  // just a good old manage statement without saving the resource.
  // stick 'x' in the body just to give the resolver something to do.

  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    record R : contextManager {
      proc type contextReturnType type do return int;

      proc enterContext() {
        return 42;
      }
      proc exitContext(in error: owned Error?) {}
    }
    manage new R() {
      var x = 42;
    }
    )""";

  auto x = resolveTypesOfVariables(ctx, program, {"x"}).at("x");
  assert(x.type()->isIntType());
}

static void testResourceByVar() {
  // test saving the resource into a variable.
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    record R : contextManager {
      proc type contextReturnType type do return int;

      proc enterContext() {
        return 42;
      }
      proc exitContext(in error: owned Error?) {}
    }
    manage new R() as res {
      var x = res;
    }
    )""";

  auto x = resolveTypesOfVariables(ctx, program, {"x"}).at("x");
  assert(x.type()->isIntType());
}

static void testResourceByRef() {
  // test saving the resource into a variable.
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    var global: int;
    record R : contextManager {
      proc type contextReturnType type do return int;

      proc enterContext() ref {
        return global;
      }
      proc exitContext(in error: owned Error?) {}
    }
    manage new R() as ref res {

    }
    )""";

  auto x = resolveTypesOfVariables(ctx, program, {"res"}).at("res");
  assert(!x.isUnknownOrErroneous());
  assert(x.type()->isIntType());
  assert(x.isRef());
}

static void testResourceByConstRef() {
  // test saving the resource into a variable.
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    var global: int;
    record R : contextManager {
      proc type contextReturnType type do return int;

      proc enterContext() ref {
        return global;
      }
      proc exitContext(in error: owned Error?) {}
    }
    manage new R() as const ref res {

    }
    )""";

  auto x = resolveTypesOfVariables(ctx, program, {"res"}).at("res");
  assert(!x.isUnknownOrErroneous());
  assert(x.type()->isIntType());
  assert(x.isConst() && x.isRef());
}

static void testInferReturn() {
  // test saving the resource into a variable.
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    record R : contextManager {
      proc enterContext() {
        return 42;
      }
      proc exitContext(in error: owned Error?) {}
    }
    manage new R() as res {}
    )""";

  auto x = resolveTypesOfVariables(ctx, program, {"res"}).at("res");
  assert(x.type()->isIntType());
}

static void testReturnIntentOverload() {
  // test saving the resource into a variable.
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    module M {
      var global: int;
      record R : contextManager {
        proc type contextReturnType type do return int;

        proc enterContext() {
          return global;
        }
        proc enterContext() ref {
          return global;
        }
        proc enterContext() const ref {
          return global;
        }
        proc exitContext(in error: owned Error?) {}
      }
      manage new R() as byVal, new R() as ref byRef, new R() as const ref byConstRef {

      }
    }
    )""";

  setFileText(ctx, "test.chpl", program);
  auto& modules = parsing::parse(ctx, UniqueString::get(ctx, "test.chpl"), UniqueString());
  assert(modules.size() == 1);
  auto& rr = resolveModule(ctx, modules[0]->id());

  bool found;

  auto byVal = findVariable(modules[0], "byVal");
  auto byValParent = parsing::parentAst(ctx, byVal)->toAs()->symbol();
  assert(byValParent);
  assert(rr.hasAst(byValParent));
  found = false;
  for (auto aa : rr.byAst(byValParent).associatedActions()) {
    if (aa.action() == AssociatedAction::ENTER_CONTEXT) {
      found = true;
      assert(aa.fn()->id().symbolPath() == "M.R.enterContext");
    }
  }
  assert(found);

  auto byRef = findVariable(modules[0], "byRef");
  auto byRefParent = parsing::parentAst(ctx, byRef)->toAs()->symbol();
  assert(byRefParent);
  assert(rr.hasAst(byRefParent));
  found = false;
  for (auto aa : rr.byAst(byRefParent).associatedActions()) {
    if (aa.action() == AssociatedAction::ENTER_CONTEXT) {
      found = true;
      assert(aa.fn()->id().symbolPath() == "M.R.enterContext#1");
    }
  }
  assert(found);

  auto byConstRef = findVariable(modules[0], "byConstRef");
  auto byConstRefParent = parsing::parentAst(ctx, byConstRef)->toAs()->symbol();
  assert(byConstRefParent);
  assert(rr.hasAst(byConstRefParent));
  found = false;
  for (auto aa : rr.byAst(byConstRefParent).associatedActions()) {
    if (aa.action() == AssociatedAction::ENTER_CONTEXT) {
      found = true;
      assert(aa.fn()->id().symbolPath() == "M.R.enterContext#2");
    }
  }
  assert(found);
}

static void testNoExplicitImplements() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    record R {
      proc enterContext() {
        return 42;
      }
      proc exitContext(in error: owned Error?) {}
    }
    manage new R() as res {}
    )""";

  auto x = resolveTypesOfVariables(ctx, program, {"res"}).at("res");
  assert(x.type()->isIntType());
}

static void testWithoutInterfaceMatch() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    record R {
      proc enterContext() {
        return 42;
      }
      proc exitContext(in err: owned Error?) {} /* wrong name of 'err' */
    }
    manage new R() as res {}
    )""";

  auto x = resolveTypesOfVariables(ctx, program, {"res"}).at("res");
  assert(x.isUnknownOrErroneous());
  assert(guard.realizeErrors());
}

int main() {
  testBasic();
  testResourceByVar();
  testResourceByRef();
  testResourceByConstRef();
  testInferReturn();
  testReturnIntentOverload();
  testNoExplicitImplements();
  testWithoutInterfaceMatch();
  return 0;
}
