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

static QualifiedType getRangeIndexType(Context* context, const RecordType* r, const std::string& ensureBoundedType) {
  assert(r->name() == "range");
  auto fields = fieldsForTypeDecl(context, r, DefaultsPolicy::IGNORE_DEFAULTS);

  assert(fields.fieldName(0) == "idxType");
  assert(fields.fieldName(1) == "bounds");
  assert(fields.fieldName(2) == "stridable");

  auto bounded = fields.fieldType(1);
  assert(bounded.kind() == QualifiedType::PARAM);
  assert(bounded.type()->isEnumType());
  assert(bounded.param() != nullptr);
  auto boundedValue = bounded.param()->toEnumParam();
  auto id = boundedValue->value();
  auto astNode = idToAst(context, id)->toNamedDecl();
  assert(astNode != nullptr);
  assert(astNode->name().str() == ensureBoundedType);

  auto stridable = fields.fieldType(2);
  assert(stridable.isParamTrue() || stridable.isParamFalse());

  return fields.fieldType(0);
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
  return 0;
}
