/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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
                         proc id(type t) type do return t;

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
                         proc id(type t) type do return t;

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
                         proc id(type t) type do return enum;

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

static void test5() {
  std::string common = R"""(
    record R {
      type T = int;
      var field : T;

      proc init(type T) {
        this.T = T;
        var val : T;
        this.field = val;
      }

      proc foobar() {
      }
    }
  )""";

  {
    // Test methods on generics with defaults
    auto context = buildStdContext();
    ErrorGuard guard(context);
    std::string program = common + R"""(
      proc blah(x: R(string)) {
        x.foobar();
      }

      var r : R(string);
      r.foobar();
      blah(r);
    )""";

    auto m = parseModule(context, program);
    resolveModule(context, m->id());
  }
  {
    // Test passing to generic arguments with types that are generic-with-defaults
    auto context = buildStdContext();
    ErrorGuard guard(context);
    std::string program = common + R"""(
      //
      // In this case, we have specified 'arg' to be generic, but the frontend
      // still technically recognizes it as generic-with-defaults (at least at
      // the time this test was created).
      //
      proc blah(arg: R(?)) {
        return arg.field;
      }

      var a : R(string);
      var x = blah(a);

      var b : R(int);
      var y = blah(b);
    )""";

    auto m = parseModule(context, program);
    auto r = resolveModule(context, m->id());

    auto x = findVariable(m, "x");
    assert(r.byAst(x).type().type()->isStringType());

    auto y = findVariable(m, "y");
    assert(r.byAst(y).type().type()->isIntType());
  }
  {
    // Test passing to generic arguments with types that are generic-with-defaults
    auto context = buildStdContext();
    ErrorGuard guard(context);
    std::string program = common + R"""(
      proc copy(arg) {
        return arg;
      }

      var a : R(string);
      var b = copy(a);
    )""";

    auto m = parseModule(context, program);
    auto r = resolveModule(context, m->id());

    auto a = findVariable(m, "a");
    auto aType = r.byAst(a).type();
    assert(aType.type()->isCompositeType());
    auto subs = aType.type()->toCompositeType()->substitutions();
    assert(subs.size() == 1);
    for (auto pair : subs) {
      assert(pair.second.type()->isStringType());
    }

    auto b = findVariable(m, "b");
    auto bType = r.byAst(b).type();
    assert(aType == bType);
  }
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();

  return 0;
}
