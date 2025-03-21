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

static void testBasic() {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  std::string program = R"""(
    var x = let a = 4 in a * a;
  )""";

  auto qt = resolveTypeOfXInit(context, program);
  assert(qt.type());
  assert(qt.type()->isIntType());
}

static void testTwoVars() {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  std::string program = R"""(
    var x = let a = 4, b = 4 in a * b;
  )""";

  auto qt = resolveTypeOfXInit(context, program);
  assert(qt.type());
  assert(qt.type()->isIntType());
}

static void testRedefinition() {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  std::string program = R"""(
    var x = let a = 4, a = 4 in a * a;
  )""";

  resolveTypeOfXInit(context, program, /* requireType */ false);
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->type() == ErrorType::Redefinition);
  guard.realizeErrors();
}

static void testOutOfOrder() {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  std::string program = R"""(
    var x = let a = b, b = 4 in a * a;
  )""";

  resolveTypeOfXInit(context, program, /* requireType */ false);
  assert(guard.numErrors() == 2);
  assert(guard.error(0)->type() == ErrorType::UseOfLaterVariable);
  assert(guard.error(1)->type() == ErrorType::NoMatchingCandidates);
  guard.realizeErrors();
}

static void testVoid() {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  std::string program = R"""(
    proc foo(x) {}
    var x = let a = 4 in foo(a);
  )""";

  auto qt = resolveTypeOfXInit(context, program);
  assert(qt.type()->isVoidType());
  assert(guard.numErrors() == 1);
  guard.realizeErrors();
}

int main() {
  testBasic();
  testTwoVars();
  testRedefinition();
  testOutOfOrder();
  testVoid();
}
