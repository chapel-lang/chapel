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
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

static void test1() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveQualifiedTypeOfX(context,
                         R""""(
                         proc id(type t) type return t;

                         record r {
                           type typeWithDefault = int;
                           var nonGenericField : id(typeWithDefault);
                         }

                         var x : r;
                         )"""");
  assert(qt.type() && qt.type()->isRecordType());
  auto ct = qt.type()->toRecordType();
  auto baseCt = ct->instantiatedFrom();
  assert(baseCt != nullptr);
  auto& fields = chpl::resolution::fieldsForTypeDecl(context, baseCt,
      chpl::resolution::DefaultsPolicy::IGNORE_DEFAULTS);
  assert(fields.numFields() == 2);
  assert(fields.fieldName(0) == "typeWithDefault");
  auto firstFieldType = fields.fieldType(0);
  assert(firstFieldType.kind() == QualifiedType::TYPE);
  assert(firstFieldType.type() && firstFieldType.type()->isAnyType());
  assert(fields.fieldName(1) == "nonGenericField");
  auto secondFieldType = fields.fieldType(1);
  assert(secondFieldType.kind() == QualifiedType::VAR);
  assert(secondFieldType.isUnknown());
  auto& subs = ct->substitutions();
  assert(subs.size() == 1);
  auto subIt = subs.find(fields.fieldDeclId(0));
  assert(subIt != subs.end());
  assert(subIt->second.kind() == QualifiedType::TYPE);
  assert(subIt->second.type()->isIntType());
}

static void test2() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveQualifiedTypeOfX(context,
                         R""""(
                         proc id(type t) type return t;

                         record r {
                           type typeWithDefault;
                           var nonGenericField : id(typeWithDefault);
                         }

                         var x : r;
                         )"""");
  assert(qt.type() && qt.type()->isRecordType());
  auto ct = qt.type()->toRecordType();
  auto baseCt = ct->instantiatedFrom();
  assert(baseCt == nullptr);
  auto& fields = chpl::resolution::fieldsForTypeDecl(context, ct,
      chpl::resolution::DefaultsPolicy::IGNORE_DEFAULTS);
  assert(fields.numFields() == 2);
  assert(fields.fieldName(0) == "typeWithDefault");
  auto firstFieldType = fields.fieldType(0);
  assert(firstFieldType.kind() == QualifiedType::TYPE);
  assert(firstFieldType.type() && firstFieldType.type()->isAnyType());
  assert(fields.fieldName(1) == "nonGenericField");
  auto secondFieldType = fields.fieldType(1);
  assert(secondFieldType.kind() == QualifiedType::VAR);
  assert(secondFieldType.isUnknown());
  auto& subs = ct->substitutions();
  assert(subs.size() == 0);
}

static void test3() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveQualifiedTypeOfX(context,
                         R""""(
                         proc id(type t) type return enum;

                         record r {
                           type typeWithDefault = int;
                           var nonGenericField : id(typeWithDefault);
                         }

                         var x : r;
                         )"""");
  assert(qt.type() && qt.type()->isRecordType());
  auto ct = qt.type()->toRecordType();
  auto baseCt = ct->instantiatedFrom();
  assert(baseCt == nullptr);
  auto& fields = chpl::resolution::fieldsForTypeDecl(context, ct,
      chpl::resolution::DefaultsPolicy::IGNORE_DEFAULTS);
  assert(fields.numFields() == 2);
  assert(fields.fieldName(0) == "typeWithDefault");
  auto firstFieldType = fields.fieldType(0);
  assert(firstFieldType.kind() == QualifiedType::TYPE);
  assert(firstFieldType.type() && firstFieldType.type()->isAnyType());
  assert(fields.fieldName(1) == "nonGenericField");
  auto secondFieldType = fields.fieldType(1);
  assert(secondFieldType.kind() == QualifiedType::VAR);
  assert(secondFieldType.isUnknown());
  auto& subs = ct->substitutions();
  assert(subs.size() == 0);
}

static void test4() {
  {
    Context ctx;
    auto context = &ctx;
    QualifiedType qt =  resolveQualifiedTypeOfX(context,
                           R""""(
                           proc foo(type t = int) type do return t;

                           var x : foo();
                           )"""");
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type()->isIntType());
  }
  {
    Context ctx;
    auto context = &ctx;
    QualifiedType qt =  resolveQualifiedTypeOfX(context,
                           R""""(
                           proc foo(type t = int) type do return t;

                           var x : foo(string);
                           )"""");
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type()->isStringType());
  }
}

int main() {
  test1();
  test2();
  test3();
  test4();
  return 0;
}
