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

#include "test-resolution.h"

static void testCoerceToDesync() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
      var tmp1: sync int;
      var tmp2: sync real;
      var tmp3: sync bool;

      var x = tmp1;
      var y = tmp2;
      var z = tmp3;
      )""", {"x", "y", "z"});

  assert(vars.at("x").type()->isIntType());
  assert(vars.at("y").type()->isRealType());
  assert(vars.at("z").type()->isBoolType());
}

static void testExplicitCoerce() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
      var tmp1: sync int;
      var tmp2: sync bool;

      var x: real = tmp1;
      var y: int = tmp2;
      )""", {"x", "y"});

  assert(vars.at("x").type()->isRealType());
  assert(vars.at("y").type()->isIntType());
}

static void testCoerceInCall() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
      var tmp1: sync int;
      var tmp2: sync real;
      var tmp3: sync bool;

      proc foo1(x: int) { return x; }
      proc foo2(x: real) { return x; }
      proc foo3(x: bool) { return x; }

      var x = foo1(tmp1);
      var y = foo2(tmp2);
      var z = foo3(tmp3);
      )""", {"x", "y", "z"});

  assert(vars.at("x").type()->isIntType());
  assert(vars.at("y").type()->isRealType());
  assert(vars.at("z").type()->isBoolType());

  assert(guard.numErrors() == 3);
  for (auto& err : guard.errors()) {
    assert(err->type() == ErrorType::DeprecatedSyncRead);
  }
  guard.realizeErrors();
}


int main() {
  testCoerceToDesync();
  testExplicitCoerce();
  testCoerceInCall();
}
