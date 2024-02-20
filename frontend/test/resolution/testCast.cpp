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
// #include "chpl/types/Param.h"

static void test1() {
  printf("test1\n");
  Context ctx;

  QualifiedType qt = getTypeForFirstStmt(&ctx,
                                         "param x = 0x8000000000000000 : int;\n");
  assert(qt.hasTypePtr());
  assert(qt.hasParamPtr());

  assert(qt.type() == IntType::get(&ctx, 0));
  assert(qt.param() == IntParam::get(&ctx, 0x8000000000000000));
}

static void test2() {
  printf("test2\n");
  Context ctx;

  QualifiedType qt = getTypeForFirstStmt(&ctx,
                                         "param x = 0x7fffffffffffffff : int;\n");
  assert(qt.hasTypePtr());
  assert(qt.hasParamPtr());

  assert(qt.type() == IntType::get(&ctx, 0));
  assert(qt.param() == IntParam::get(&ctx, 0x7fffffffffffffff));
}

static void test3() {
  printf("test3\n");
  Context ctx;

  std::string program = R""""(
    proc min(type t) param  where t == int(64) do return 0x8000000000000000 : int;
    param x = min(int);
  )"""";

  QualifiedType qt = resolveQualifiedTypeOfX(&ctx, program);
  assert(qt.hasTypePtr());
  assert(qt.hasParamPtr());

  assert(qt.type() == IntType::get(&ctx, 0));
  assert(qt.param() == IntParam::get(&ctx, 0x8000000000000000));
}

static void test4() {
    printf("test4\n");
  Context ctx;

  std::string program = R""""(
    proc max(type t) param  where t == int(64) do return 0x7fffffffffffffff: t;
    param x = max(int);
  )"""";

  QualifiedType qt = resolveQualifiedTypeOfX(&ctx, program);
  assert(qt.hasTypePtr());
  assert(qt.hasParamPtr());

  assert(qt.type() == IntType::get(&ctx, 0));
  assert(qt.param() == IntParam::get(&ctx, 0x7fffffffffffffff));
}


int main() {
  test1();
  test2();
  test3();
  test4();

  return 0;
}
