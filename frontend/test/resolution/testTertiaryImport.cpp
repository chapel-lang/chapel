/*
 * Copyright 2025 Hewlett Packard Enterprise Development LP
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

#include "test-common.h"
#include "test-resolution.h"

#include "chpl/uast/all-uast.h"

// test general type-based imports
static void test1() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
        module Lib {
          record R {}
          record S {}
        }
        module LibUsingLib {
          use Lib;

          proc R.foo() { return 0; }
          proc S.foo() { return 0.0; }
        }
        module MainMod {
          use Lib;
          use LibUsingLib only R, S;

          var x = (new R()).foo();
          var y = (new S()).foo();

          proc foo() {
            return ((new R()).foo(), (new S()).foo());
          }

          var (z, w) = foo();
        }
      )""", {"x", "y", "z", "w"});

  assert(!vars.at("x").isUnknownOrErroneous() && vars.at("x").type()->isIntType());
  assert(!vars.at("y").isUnknownOrErroneous() && vars.at("y").type()->isRealType());
  assert(!vars.at("z").isUnknownOrErroneous() && vars.at("z").type()->isIntType());
  assert(!vars.at("w").isUnknownOrErroneous() && vars.at("w").type()->isRealType());
}

// test that type-based imports only bring in methods on the type imported
static void test2() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
        module Lib {
          record R {}
          record S {}
        }
        module LibUsingLib {
          use Lib;

          proc R.foo() { return 0; }
          proc S.foo() { return 0.0; }
        }
        module MainMod {
          use Lib;
          use LibUsingLib only R;

          var x = (new R()).foo();
          var y = (new S()).foo();

          proc foo() {
            use LibUsingLib only R, S;
            return ((new R()).foo(), (new S()).foo());
          }

          var (z, w) = foo();
        }
      )""", {"x", "y", "z", "w"});

  assert(!vars.at("x").isUnknownOrErroneous() && vars.at("x").type()->isIntType());
  assert(vars.at("y").isUnknownOrErroneous());
  assert(!vars.at("z").isUnknownOrErroneous() && vars.at("z").type()->isIntType());
  assert(!vars.at("w").isUnknownOrErroneous() && vars.at("w").type()->isRealType());

  assert(guard.realizeErrors());
}

// test that type-based imports works with instantiations
static void test3() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
        module Lib {
          record R { type arg; }
          record S { type arg; }
        }
        module LibUsingLib {
          use Lib;

          proc R.foo() { return 0; }
          proc S.foo() { return 0.0; }
        }
        module MainMod {
          use Lib;
          use LibUsingLib only R, S;

          var x = (new R(int)).foo();
          var y = (new S(int)).foo();

          proc foo() {
            return ((new R(int)).foo(), (new S(int)).foo());
          }

          var (z, w) = foo();
        }
      )""", {"x", "y", "z", "w"});

  assert(!vars.at("x").isUnknownOrErroneous() && vars.at("x").type()->isIntType());
  assert(!vars.at("y").isUnknownOrErroneous() && vars.at("y").type()->isRealType());
  assert(!vars.at("z").isUnknownOrErroneous() && vars.at("z").type()->isIntType());
  assert(!vars.at("w").isUnknownOrErroneous() && vars.at("w").type()->isRealType());
}

// test that type-based imports works with basic aliases
static void test4() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
        module Lib {
          record R { type arg; }
          record S { type arg; }
        }
        module LibUsingLib {
          use Lib;

          type MyR = R(int);
          type MyS = S(int);

          proc R.foo() { return 0; }
          proc S.foo() { return 0.0; }
        }
        module MainMod {
          use Lib;
          use LibUsingLib only MyR, MyS;

          var x = (new MyR()).foo();
          var y = (new MyS()).foo();

          proc foo() {
            return ((new MyR()).foo(), (new MyS()).foo());
          }

          var (z, w) = foo();
        }
      )""", {"x", "y", "z", "w"});

  assert(!vars.at("x").isUnknownOrErroneous() && vars.at("x").type()->isIntType());
  assert(!vars.at("y").isUnknownOrErroneous() && vars.at("y").type()->isRealType());
  assert(!vars.at("z").isUnknownOrErroneous() && vars.at("z").type()->isIntType());
  assert(!vars.at("w").isUnknownOrErroneous() && vars.at("w").type()->isRealType());
}

// test that type-based imports works with aliases even if names mismatch
static void test5() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
        module Lib {
          record R { type arg; }
          record S { type arg; }
        }
        module LibUsingLib {
          use Lib;

          type MyR = R(int);
          type MyS = S(int);

          proc R.foo() { return 0; }
          proc S.foo() { return 0.0; }
        }
        module MainMod {
          use Lib;
          use LibUsingLib only MyR, MyS;

          var x = (new R(int)).foo();
          var y = (new S(int)).foo();

          proc foo() {
            return ((new R(int)).foo(), (new S(int)).foo());
          }

          var (z, w) = foo();
        }
      )""", {"x", "y", "z", "w"});

  assert(!vars.at("x").isUnknownOrErroneous() && vars.at("x").type()->isIntType());
  assert(!vars.at("y").isUnknownOrErroneous() && vars.at("y").type()->isRealType());
  assert(!vars.at("z").isUnknownOrErroneous() && vars.at("z").type()->isIntType());
  assert(!vars.at("w").isUnknownOrErroneous() && vars.at("w").type()->isRealType());
}

// test that type-based imports works with complex receiver expressions
static void test6() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
        module Lib {
          record R {}
          record S {}
        }
        module LibUsingLib {
          use Lib;

          proc wrap(type arg) type do return arg;

          proc (wrap(R)).foo() { return 0; }
          proc (wrap(S)).foo() { return 0.0; }
        }
        module MainMod {
          use Lib;
          use LibUsingLib only R, S;

          var x = (new R()).foo();
          var y = (new S()).foo();

          proc foo() {
            return ((new R()).foo(), (new S()).foo());
          }

          var (z, w) = foo();
        }
      )""", {"x", "y", "z", "w"});

  assert(!vars.at("x").isUnknownOrErroneous() && vars.at("x").type()->isIntType());
  assert(!vars.at("y").isUnknownOrErroneous() && vars.at("y").type()->isRealType());
  assert(!vars.at("z").isUnknownOrErroneous() && vars.at("z").type()->isIntType());
  assert(!vars.at("w").isUnknownOrErroneous() && vars.at("w").type()->isRealType());
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  return 0;
}

