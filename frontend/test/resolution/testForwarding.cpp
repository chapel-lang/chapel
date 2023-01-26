/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

static void test1() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      operator +=(ref lhs: int, rhs: int) { }
      record Inner {
        var i: int;
        proc ref addOne() {
          i += 1;
          return 1.0;
        }
      }
      record Outer {
        forwarding var impl: Inner;
      }

      var rec: Outer;
      var x = rec.addOne();
    }
    )"""";

  auto qt = resolveQualifiedTypeOfX(context, contents);
  assert(qt.type()->isRealType());
}

static void test2() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      operator +=(ref lhs: int, rhs: int) { }
      record Inner {
        var i: int;
        proc ref addOne() {
          i += 1;
          return 1.0;
        }
      }
      record Middle {
        forwarding var impl: Inner;
      }
      record Outer {
        forwarding var impl: Middle;
      }

      var rec: Outer;
      var x = rec.addOne();
    }
    )"""";

  auto qt = resolveQualifiedTypeOfX(context, contents);
  assert(qt.type()->isRealType());
}


int main() {
  test1();
  test2();

  return 0;
}

