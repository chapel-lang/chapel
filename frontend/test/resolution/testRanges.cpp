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

static std::tuple<QualifiedType, std::string, std::string>
getRangeInfo(Context* context, const RecordType* r) {
  assert(r->name() == "_range");
  auto rc = createDummyRC(context);
  auto fields = fieldsForTypeDecl(&rc, r, DefaultsPolicy::IGNORE_DEFAULTS);

  assert(fields.fieldName(0) == "idxType");
  assert(fields.fieldName(1) == "bounds");
  assert(fields.fieldName(2) == "strides");

  auto bounded = fields.fieldType(1);
  assert(bounded.kind() == QualifiedType::PARAM);
  assert(bounded.type()->isEnumType());
  assert(bounded.param() != nullptr);
  auto boundedValue = bounded.param()->toEnumParam();
  auto id = boundedValue->value().id;
  auto astNode = idToAst(context, id)->toNamedDecl();
  assert(astNode != nullptr);
  std::string boundTypeStr = astNode->name().str();

  auto stridable = fields.fieldType(2);
  assert(stridable.kind() == QualifiedType::PARAM);
  assert(stridable.type()->isEnumType());
  assert(stridable.param() != nullptr);
  auto stridableValue = stridable.param()->toEnumParam();
  auto idS = stridableValue->value().id;
  auto astNodeS = idToAst(context, idS)->toNamedDecl();
  assert(astNodeS != nullptr);
  std::string stridesStr = astNodeS->name().str();

  return std::make_tuple(fields.fieldType(0), boundTypeStr, stridesStr);
}

static QualifiedType getRangeIndexType(Context* context, const RecordType* r, const std::string& ensureBoundedType) {
  auto info = getRangeInfo(context, r);
  assert(std::get<1>(info) == ensureBoundedType);
  assert(std::get<2>(info) == "one");
  return std::get<0>(info);
}

static void test1() {
  // if the stdlib is not used, ranges should resolve to having
  // unknown types.
  Context context;
  QualifiedType qt =  resolveTypeOfXInit(&context,
                         R""""(
                         var y : int;
                         var x = 1..5;
                         )"""", false);
  assert(qt.isUnknown());
}


static void test2(Context* context) {
  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         var y : int;
                         var x = 1..5;
                         )"""", true);
  assert(qt.type() != nullptr);
  auto rangeType = qt.type()->toRecordType();
  assert(rangeType != nullptr);
  auto idxType = getRangeIndexType(context, rangeType, "both");
  assert(idxType.type() != nullptr);
  auto idxTypeInt = idxType.type()->toIntType();
  assert(idxTypeInt->bitwidth() == 64);
}

static void test3(Context* context) {
  // test coercion
  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         var y : int;
                         var lower: int(32);
                         var upper: int(16);
                         var x = lower..upper;
                         )"""", true);
  assert(qt.type() != nullptr);
  auto rangeType = qt.type()->toRecordType();
  assert(rangeType != nullptr);
  auto idxType = getRangeIndexType(context, rangeType, "both");
  assert(idxType.type() != nullptr);
  auto idxTypeInt = idxType.type()->toIntType();
  assert(idxTypeInt->bitwidth() == 32);
}

static void test4(Context* context) {
  // test range without upper bound
  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         var y : int;
                         var lower: int(32);
                         var x = lower..;
                         )"""", true);
  assert(qt.type() != nullptr);
  auto rangeType = qt.type()->toRecordType();
  assert(rangeType != nullptr);
  auto idxType = getRangeIndexType(context, rangeType, "low");
  assert(idxType.type() != nullptr);
  auto idxTypeInt = idxType.type()->toIntType();
  assert(idxTypeInt->bitwidth() == 32);
}

static void test5(Context* context) {
  // test range without lower bound
  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         var y : int;
                         var upper: int(16);
                         var x = ..upper;
                         )"""", true);
  assert(qt.type() != nullptr);
  auto rangeType = qt.type()->toRecordType();
  assert(rangeType != nullptr);
  auto idxType = getRangeIndexType(context, rangeType, "high");
  assert(idxType.type() != nullptr);
  auto idxTypeInt = idxType.type()->toIntType();
  assert(idxTypeInt->bitwidth() == 16);
}

static void test6(Context* context) {
  // test range without bound
  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         var y : int;
                         var x = ..;
                         )"""", true);
  assert(qt.type() != nullptr);
  auto rangeType = qt.type()->toRecordType();
  assert(rangeType != nullptr);
  auto idxType = getRangeIndexType(context, rangeType, "neither");
  assert(idxType.type() != nullptr);
  auto idxTypeInt = idxType.type()->toIntType();
  assert(idxTypeInt->bitwidth() == 64);
}

static void test7(Context* context) {
  // test range without bound
  ErrorGuard guard(context);
  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         proc f(r: range(?)) do return 42;
                         var x = f(1..10);
                         )"""", true);
  assert(qt.type() != nullptr);
  assert(qt.type()->isIntType());
  assert(qt.type()->toIntType()->isDefaultWidth());
}

static void test8(Context* context) {
  // test range without bound
  ErrorGuard guard(context);
  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         var x = new range(int, 0, 10);
                         )"""", true);
  auto rangeType = qt.type()->toRecordType();
  assert(rangeType != nullptr);
  auto idxType = getRangeIndexType(context, rangeType, "both");
  assert(idxType.type() != nullptr);
  auto idxTypeInt = idxType.type()->toIntType();
  assert(idxTypeInt->bitwidth() == 64);
}

static void test9(Context* context) {
  // test the count operator on a bounded range
  ErrorGuard guard(context);
  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  auto qts =  resolveTypesOfVariables(context,
      R""""(
      var y : int;
      var lower: int(32);
      var x1 = lower..;
      var x2 = lower..#10;
      )"""", {"x1", "x2"});

  {
    // Check the first range
    auto qt = qts.at("x1");
    assert(qt.type() != nullptr);
    auto rangeType = qt.type()->toRecordType();
    assert(rangeType != nullptr);
    auto idxType = getRangeIndexType(context, rangeType, "low");
    assert(idxType.type() != nullptr);
    auto idxTypeInt = idxType.type()->toIntType();
    assert(idxTypeInt->bitwidth() == 32);
  }
  {
    // Check the counted range
    auto qt = qts.at("x2");
    assert(qt.type() != nullptr);
    auto rangeType = qt.type()->toRecordType();
    assert(rangeType != nullptr);
    auto idxType = getRangeIndexType(context, rangeType, "both");
    assert(idxType.type() != nullptr);
    auto idxTypeInt = idxType.type()->toIntType();
    assert(idxTypeInt->bitwidth() == 32);
  }
}

static void test10(Context* context) {
  // test the count operator on a bounded range
  ErrorGuard guard(context);
  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  auto qts =  resolveTypesOfVariables(context,
      R""""(
      var y : int;
      var higher: int(32);
      var x1 = ..higher;
      var x2 = ..higher#10;
      )"""", {"x1", "x2"});

  {
    // Check the first range
    auto qt = qts.at("x1");
    assert(qt.type() != nullptr);
    auto rangeType = qt.type()->toRecordType();
    assert(rangeType != nullptr);
    auto idxType = getRangeIndexType(context, rangeType, "high");
    assert(idxType.type() != nullptr);
    auto idxTypeInt = idxType.type()->toIntType();
    assert(idxTypeInt->bitwidth() == 32);
  }
  {
    // Check the counted range
    auto qt = qts.at("x2");
    assert(qt.type() != nullptr);
    auto rangeType = qt.type()->toRecordType();
    assert(rangeType != nullptr);
    auto idxType = getRangeIndexType(context, rangeType, "both");
    assert(idxType.type() != nullptr);
    auto idxTypeInt = idxType.type()->toIntType();
    assert(idxTypeInt->bitwidth() == 32);
  }
}

static void test11(Context* context) {
  // test the by operator on a bounded range
  ErrorGuard guard(context);
  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  auto qts =  resolveTypesOfVariables(context,
      R""""(
      var x1 = 1..10;
      var x2 = x1 by 2;
      var x3 = x1 by -1;
      var x4 = x1 by -2;

      var newStride = 10;
      var x5 = x1 by newStride;

      var y1 = 1..10;
      var y2 = y1 by -1;
      var y3 = y2 by -1;
      var y4 = y2 by 5;
      var y5 = y2 by -5;
      )"""", {"x1", "x2", "x3", "x4", "x5", "y1", "y2", "y3", "y4", "y5"});


  auto check = [&](const std::string& var, const std::string& stride) {
    auto qt = qts.at(var);
    assert(qt.type() != nullptr);
    auto rangeType = qt.type()->toRecordType();
    assert(rangeType != nullptr);
    auto info = getRangeInfo(context, rangeType);

    assert(std::get<0>(info).type() != nullptr);
    assert(std::get<0>(info).type()->isIntType());
    assert(std::get<1>(info) == "both");
    assert(std::get<2>(info) == stride);
  };

  check("x1", "one");
  check("x2", "positive");
  check("x3", "negOne");
  check("x4", "negative");
  check("x5", "any");

  check("y1", "one");
  check("y2", "negOne");
  check("y3", "one");
  check("y4", "negative");
  check("y5", "positive");
}

static void test12(Context* context) {
  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         proc foo(arg: range(?) = 0..) do return arg;
                         var x = foo(0..#10);
                         )"""", true);
  assert(qt.type() != nullptr);
  auto rangeType = qt.type()->toRecordType();
  assert(rangeType != nullptr);
  auto idxType = getRangeIndexType(context, rangeType, "both");
  assert(idxType.type() != nullptr);
  assert(idxType.type()->isIntType());

}

int main() {
  // first test runs without environment and stdlib.
  test1();

  Context::Configuration config;
  config.chplHome = getenv("CHPL_HOME");
  Context context(config);
  auto ctx = &context;
  test2(ctx);
  test3(ctx);
  test4(ctx);
  test5(ctx);
  test6(ctx);
  test7(ctx);
  test8(ctx);
  test9(ctx);
  test10(ctx);
  test11(ctx);
  test12(ctx);
  return 0;
}
