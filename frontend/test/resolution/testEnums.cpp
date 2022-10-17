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
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

static void test1() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         enum color {
                           red, green, blue
                         }

                         var x = color.red;
                         )"""");
  assert(qt.kind() == QualifiedType::PARAM);
  assert(qt.type() && qt.type()->isEnumType());
  assert(qt.param() && qt.param()->isEnumParam());

  auto et = qt.type()->toEnumType();
  auto ep = qt.param()->toEnumParam();
  assert(et->id().contains(ep->value()));
  auto enumAst = parsing::idToAst(context, et->id());
  assert(enumAst && enumAst->isEnum());
  auto elemAst = parsing::idToAst(context, ep->value());
  assert(elemAst && elemAst->isEnumElement());
}

static void test2() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         enum color {
                           red, red, blue
                         }

                         var x = color.red;
                         )"""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type() && qt.type()->isEnumType());
}

static void test3() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         enum color {
                           green, blue
                         }

                         var x = color.red;
                         )"""");
  assert(qt.kind() == QualifiedType::UNKNOWN);
  assert(qt.type() && qt.type()->isErroneousType());
}

int main() {
  test1();
  test2();
  test3();
  return 0;
}
