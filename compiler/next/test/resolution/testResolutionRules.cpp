/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/resolution-rules.h"
#include "chpl/types/all-types.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace resolution;
using namespace types;
using namespace uast;

static bool passesAsIs(CanPassResult r) {
  return r.passes() && 
         !r.instantiates() &&
         !r.promotes() &&
         !r.converts();
}

static bool passesWithParamConversion(CanPassResult r) {
  return r.passes() && 
         !r.instantiates() &&
         !r.promotes() &&
         r.converts() &&
         r.conversionKind() == CanPassResult::PARAM;
}

static bool doesNotPass(CanPassResult r) {
  return !r.passes();
}

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  QualifiedType int0(QualifiedType::VALUE, IntType::get(context, 0));
  QualifiedType int8(QualifiedType::VALUE, IntType::get(context, 8));
  QualifiedType int16(QualifiedType::VALUE, IntType::get(context, 16));
  QualifiedType int32(QualifiedType::VALUE, IntType::get(context, 32));
  QualifiedType int64(QualifiedType::VALUE, IntType::get(context, 64));
  QualifiedType real0(QualifiedType::VALUE, RealType::get(context, 0));

  CanPassResult r;
  r = canPass(int0, int0); assert(passesAsIs(r));
  r = canPass(int8, int8); assert(passesAsIs(r));
  r = canPass(int16, int16); assert(passesAsIs(r));
  r = canPass(int32, int32); assert(passesAsIs(r));
  r = canPass(int64, int64); assert(passesAsIs(r));
  r = canPass(int0, int64); assert(passesAsIs(r));

  r = canPass(int8, int0); assert(passesWithParamConversion(r));
  r = canPass(int8, int16); assert(passesWithParamConversion(r));
  r = canPass(int8, int32); assert(passesWithParamConversion(r));
  r = canPass(int8, int64); assert(passesWithParamConversion(r));

  r = canPass(int16, int0); assert(passesWithParamConversion(r));
  r = canPass(int16, int8); assert(doesNotPass(r));
  r = canPass(int16, int32); assert(passesWithParamConversion(r));
  r = canPass(int16, int64); assert(passesWithParamConversion(r));

  r = canPass(int32, int0); assert(passesWithParamConversion(r));
  r = canPass(int32, int8); assert(doesNotPass(r));
  r = canPass(int32, int16); assert(doesNotPass(r));
  r = canPass(int32, int64); assert(passesWithParamConversion(r));

  r = canPass(int64, int0); assert(passesAsIs(r));
  r = canPass(int64, int8); assert(doesNotPass(r));
  r = canPass(int64, int16); assert(doesNotPass(r));
  r = canPass(int64, int32); assert(doesNotPass(r));

  r = canPass(int0, int8); assert(doesNotPass(r));
  r = canPass(int0, int16); assert(doesNotPass(r));
  r = canPass(int0, int32); assert(doesNotPass(r));
  r = canPass(int0, int64); assert(passesAsIs(r));
}

static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  QualifiedType int0(QualifiedType::VALUE, IntType::get(context, 0));
  QualifiedType int8(QualifiedType::VALUE, IntType::get(context, 8));
  QualifiedType int16(QualifiedType::VALUE, IntType::get(context, 16));
  QualifiedType int32(QualifiedType::VALUE, IntType::get(context, 32));
  QualifiedType int64(QualifiedType::VALUE, IntType::get(context, 64));
  QualifiedType real0(QualifiedType::VALUE, RealType::get(context, 0));

  CanPassResult r;
  r = canPass(int0, real0); assert(passesWithParamConversion(r));
  r = canPass(int8, real0); assert(passesWithParamConversion(r));
  r = canPass(int16, real0); assert(passesWithParamConversion(r));
  r = canPass(int32, real0); assert(passesWithParamConversion(r));
  r = canPass(int64, real0); assert(passesWithParamConversion(r));
}

static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  QualifiedType bool0(QualifiedType::VALUE, BoolType::get(context, 0));
  QualifiedType bool8(QualifiedType::VALUE, BoolType::get(context, 8));
  QualifiedType bool16(QualifiedType::VALUE, BoolType::get(context, 16));
  QualifiedType bool32(QualifiedType::VALUE, BoolType::get(context, 32));
  QualifiedType bool64(QualifiedType::VALUE, BoolType::get(context, 64));

  CanPassResult r;
  r = canPass(bool0, bool0); assert(passesAsIs(r));
  r = canPass(bool8, bool8); assert(passesAsIs(r));
  r = canPass(bool16, bool16); assert(passesAsIs(r));
  r = canPass(bool32, bool32); assert(passesAsIs(r));
  r = canPass(bool64, bool64); assert(passesAsIs(r));

  r = canPass(bool0, bool8); assert(passesWithParamConversion(r));
  r = canPass(bool0, bool16); assert(passesWithParamConversion(r));
  r = canPass(bool0, bool32); assert(passesWithParamConversion(r));
  r = canPass(bool0, bool64); assert(passesWithParamConversion(r));

  r = canPass(bool8, bool0); assert(passesWithParamConversion(r));
  r = canPass(bool8, bool16); assert(passesWithParamConversion(r));
  r = canPass(bool8, bool32); assert(passesWithParamConversion(r));
  r = canPass(bool8, bool64); assert(passesWithParamConversion(r));

  r = canPass(bool16, bool0); assert(passesWithParamConversion(r));
  r = canPass(bool16, bool8); assert(passesWithParamConversion(r));
  r = canPass(bool16, bool32); assert(passesWithParamConversion(r));
  r = canPass(bool16, bool64); assert(passesWithParamConversion(r));

  r = canPass(bool32, bool0); assert(passesWithParamConversion(r));
  r = canPass(bool32, bool8); assert(passesWithParamConversion(r));
  r = canPass(bool32, bool16); assert(passesWithParamConversion(r));
  r = canPass(bool32, bool64); assert(passesWithParamConversion(r));

  r = canPass(bool64, bool0); assert(passesWithParamConversion(r));
  r = canPass(bool64, bool8); assert(passesWithParamConversion(r));
  r = canPass(bool64, bool16); assert(passesWithParamConversion(r));
  r = canPass(bool64, bool32); assert(passesWithParamConversion(r));
}



int main() {
  test1();
  test2();
  test3();

  return 0;
}
