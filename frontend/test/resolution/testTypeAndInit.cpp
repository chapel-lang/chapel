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
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

static void test1() {
  Context ctx;
  auto context = &ctx;
  auto qt = resolveQualifiedTypeOfX(context,
                             R""""(
                               var x: bool = true;
                             )"""");
  qt.dump();
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type()->isBoolType());
}

static void test2() {
  Context ctx;
  auto context = &ctx;
  auto qt = resolveQualifiedTypeOfX(context,
                                    R""""(
                                      proc foo(arg: uint = 0) do return arg;
                                      var x = foo();
                                    )"""");
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type()->isUintType());
}

int main() {
    test1();
    test2();
    return 0;
}
