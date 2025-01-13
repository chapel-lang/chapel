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

int main() {
  testBasic();
  testResourceByVar();
  return 0;
}
