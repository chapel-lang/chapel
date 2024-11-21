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
#include "chpl/types/ComplexType.h"
#include "chpl/types/IntType.h"
#include "chpl/types/Param.h"
#include "chpl/types/RealType.h"
#include "chpl/uast/Module.h"

static void test1() {
  auto context = buildStdContext();
  QualifiedType qt1 =  resolveTypeOfXInit(context, "var x = int == int;");
  assert(qt1.isParamTrue());
  context->advanceToNextRevision(false);
  QualifiedType qt2 =  resolveTypeOfXInit(context, "var x = int != int;");
  assert(qt2.isParamFalse());
  context->advanceToNextRevision(false);
  QualifiedType qt3 =  resolveTypeOfXInit(context, "var x = bool == int;");
  assert(qt3.isParamFalse());
  context->advanceToNextRevision(false);
  QualifiedType qt4 =  resolveTypeOfXInit(context, "var x = bool != int;");
  assert(qt4.isParamTrue());
}

static void test2() {
  auto context = buildStdContext();
  QualifiedType qt1 =  resolveTypeOfXInit(context, "var x = 1 == 1;");
  assert(qt1.isParamTrue());
  context->advanceToNextRevision(false);
  QualifiedType qt2 =  resolveTypeOfXInit(context, "var x = 1 != 1;");
  assert(qt2.isParamFalse());
  context->advanceToNextRevision(false);
  QualifiedType qt3 =  resolveTypeOfXInit(context, "var x = 1 == 2;");
  assert(qt3.isParamFalse());
  context->advanceToNextRevision(false);
  QualifiedType qt4 =  resolveTypeOfXInit(context, "var x = 1 != 2;");
  assert(qt4.isParamTrue());
}

static void test3() {
  auto context = buildStdContext();
  QualifiedType qt1 =  resolveTypeOfXInit(context,
                         R""""(
                         param p : int = 0;
                         var x = p == 1;
                         )"""");
  assert(qt1.isParamFalse());
}

static void test4() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  QualifiedType qt1 =  resolveTypeOfXInit(context,
                         R""""(
                         record R {}

                         var y : R;
                         param x = y.type:string;
                         )"""");

  assert(qt1.isParam() && qt1.type()->isStringType());
  assert(qt1.param()->toStringParam()->value() == "R");
}

int main() {
  test1();
  test2();
  test3();
  test4();
  return 0;
}
