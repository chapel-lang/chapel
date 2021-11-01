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

static bool passesParam(CanPassResult r) {
  return r.passes() &&
         !r.instantiates() &&
         !r.promotes() &&
         r.converts() &&
         r.conversionKind() == CanPassResult::PARAM;
}

static bool passesParamNarrowing(CanPassResult r) {
  return r.passes() &&
         !r.instantiates() &&
         !r.promotes() &&
         r.converts() &&
         r.conversionKind() == CanPassResult::PARAM_NARROWING;
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

  r = canPass(int8, int0); assert(passesParam(r));
  r = canPass(int8, int16); assert(passesParam(r));
  r = canPass(int8, int32); assert(passesParam(r));
  r = canPass(int8, int64); assert(passesParam(r));

  r = canPass(int16, int0); assert(passesParam(r));
  r = canPass(int16, int8); assert(doesNotPass(r));
  r = canPass(int16, int32); assert(passesParam(r));
  r = canPass(int16, int64); assert(passesParam(r));

  r = canPass(int32, int0); assert(passesParam(r));
  r = canPass(int32, int8); assert(doesNotPass(r));
  r = canPass(int32, int16); assert(doesNotPass(r));
  r = canPass(int32, int64); assert(passesParam(r));

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
  r = canPass(int0, real0); assert(passesParam(r));
  r = canPass(int8, real0); assert(passesParam(r));
  r = canPass(int16, real0); assert(passesParam(r));
  r = canPass(int32, real0); assert(passesParam(r));
  r = canPass(int64, real0); assert(passesParam(r));
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

  r = canPass(bool0, bool8); assert(passesParam(r));
  r = canPass(bool0, bool16); assert(passesParam(r));
  r = canPass(bool0, bool32); assert(passesParam(r));
  r = canPass(bool0, bool64); assert(passesParam(r));

  r = canPass(bool8, bool0); assert(passesParam(r));
  r = canPass(bool8, bool16); assert(passesParam(r));
  r = canPass(bool8, bool32); assert(passesParam(r));
  r = canPass(bool8, bool64); assert(passesParam(r));

  r = canPass(bool16, bool0); assert(passesParam(r));
  r = canPass(bool16, bool8); assert(passesParam(r));
  r = canPass(bool16, bool32); assert(passesParam(r));
  r = canPass(bool16, bool64); assert(passesParam(r));

  r = canPass(bool32, bool0); assert(passesParam(r));
  r = canPass(bool32, bool8); assert(passesParam(r));
  r = canPass(bool32, bool16); assert(passesParam(r));
  r = canPass(bool32, bool64); assert(passesParam(r));

  r = canPass(bool64, bool0); assert(passesParam(r));
  r = canPass(bool64, bool8); assert(passesParam(r));
  r = canPass(bool64, bool16); assert(passesParam(r));
  r = canPass(bool64, bool32); assert(passesParam(r));
}

static void test4() {
  printf("test4\n");
  Context ctx;
  Context* context = &ctx;

  auto oneParam = IntParam::get(context, 1);
  auto negParam = IntParam::get(context, -1);

  QualifiedType oneInt0(QualifiedType::PARAM,
                        IntType::get(context, 0),
                        oneParam);
  QualifiedType oneInt8(QualifiedType::PARAM,
                        IntType::get(context, 8),
                        oneParam);
  QualifiedType oneInt16(QualifiedType::PARAM,
                         IntType::get(context, 16),
                         oneParam);
  QualifiedType oneInt32(QualifiedType::PARAM,
                         IntType::get(context, 32),
                         oneParam);
  QualifiedType oneInt64(QualifiedType::PARAM,
                         IntType::get(context, 64),
                         oneParam);

  QualifiedType negInt0(QualifiedType::PARAM,
                        IntType::get(context, 0),
                        negParam);
  QualifiedType negInt8(QualifiedType::PARAM,
                        IntType::get(context, 8),
                        negParam);
  QualifiedType negInt16(QualifiedType::PARAM,
                         IntType::get(context, 16),
                         negParam);
  QualifiedType negInt32(QualifiedType::PARAM,
                         IntType::get(context, 32),
                         negParam);
  QualifiedType negInt64(QualifiedType::PARAM,
                         IntType::get(context, 64),
                         negParam);

  QualifiedType oneUint0(QualifiedType::PARAM,
                         UintType::get(context, 0),
                         oneParam);
  QualifiedType oneUint8(QualifiedType::PARAM,
                         UintType::get(context, 8),
                         oneParam);
  QualifiedType oneUint16(QualifiedType::PARAM,
                          UintType::get(context, 16),
                          oneParam);
  QualifiedType oneUint32(QualifiedType::PARAM,
                          UintType::get(context, 32),
                          oneParam);
  QualifiedType oneUint64(QualifiedType::PARAM,
                          UintType::get(context, 64),
                          oneParam);

  QualifiedType int8(QualifiedType::VALUE, IntType::get(context, 8));
  QualifiedType uint8(QualifiedType::VALUE, UintType::get(context, 8));


  CanPassResult r;
  r = canPass(oneInt0,  int8); assert(passesParamNarrowing(r));
  r = canPass(oneInt8,  int8); assert(passesAsIs(r));
  r = canPass(oneInt16, int8); assert(passesParamNarrowing(r));
  r = canPass(oneInt32, int8); assert(passesParamNarrowing(r));
  r = canPass(oneInt64, int8); assert(passesParamNarrowing(r));

  r = canPass(oneUint0,  int8); assert(passesParamNarrowing(r));
  r = canPass(oneUint8,  int8); assert(passesParamNarrowing(r));
  r = canPass(oneUint16, int8); assert(passesParamNarrowing(r));
  r = canPass(oneUint32, int8); assert(passesParamNarrowing(r));
  r = canPass(oneUint64, int8); assert(passesParamNarrowing(r));

  r = canPass(oneInt0,  uint8); assert(passesParamNarrowing(r));
  r = canPass(oneInt8,  uint8); assert(passesParamNarrowing(r));
  r = canPass(oneInt16, uint8); assert(passesParamNarrowing(r));
  r = canPass(oneInt32, uint8); assert(passesParamNarrowing(r));
  r = canPass(oneInt64, uint8); assert(passesParamNarrowing(r));

  r = canPass(oneUint0,  uint8); assert(passesParamNarrowing(r));
  r = canPass(oneUint8,  uint8); assert(passesAsIs(r));
  r = canPass(oneUint16, uint8); assert(passesParamNarrowing(r));
  r = canPass(oneUint32, uint8); assert(passesParamNarrowing(r));
  r = canPass(oneUint64, uint8); assert(passesParamNarrowing(r));

  r = canPass(negInt0,  int8); assert(passesParamNarrowing(r));
  r = canPass(negInt8,  int8); assert(passesAsIs(r));
  r = canPass(negInt16, int8); assert(passesParamNarrowing(r));
  r = canPass(negInt32, int8); assert(passesParamNarrowing(r));
  r = canPass(negInt64, int8); assert(passesParamNarrowing(r));

  r = canPass(negInt0,  uint8); assert(doesNotPass(r));
  r = canPass(negInt8,  uint8); assert(doesNotPass(r));
  r = canPass(negInt16, uint8); assert(doesNotPass(r));
  r = canPass(negInt32, uint8); assert(doesNotPass(r));
  r = canPass(negInt64, uint8); assert(doesNotPass(r));
}

static QualifiedType getint(Context* context, int val) {
  auto p = IntParam::get(context, val);
  return QualifiedType(QualifiedType::PARAM,
                       IntType::get(context, 0),
                       p);
}

// integers can implicitly convert to default-sized real,
// but integers can only convert to real(32) if there is no loss
// of precision.
static void test5() {
  printf("test5\n");
  Context ctx;
  Context* context = &ctx;

  auto m5 = getint(context, (1<<24) - 5);
  auto m4 = getint(context, (1<<24) - 4);
  auto m3 = getint(context, (1<<24) - 3);
  auto m2 = getint(context, (1<<24) - 2);
  auto m1 = getint(context, (1<<24) - 1);
  auto p0 = getint(context, (1<<24) + 0);
  auto p1 = getint(context, (1<<24) + 1);
  auto p2 = getint(context, (1<<24) + 2);
  auto p3 = getint(context, (1<<24) + 3);
  auto p4 = getint(context, (1<<24) + 4);
  auto p5 = getint(context, (1<<24) + 5);

  auto n_m5 = getint(context, -(1<<24) - 5);
  auto n_m4 = getint(context, -(1<<24) - 4);
  auto n_m3 = getint(context, -(1<<24) - 3);
  auto n_m2 = getint(context, -(1<<24) - 2);
  auto n_m1 = getint(context, -(1<<24) - 1);
  auto n_p0 = getint(context, -(1<<24) + 0);
  auto n_p1 = getint(context, -(1<<24) + 1);
  auto n_p2 = getint(context, -(1<<24) + 2);
  auto n_p3 = getint(context, -(1<<24) + 3);
  auto n_p4 = getint(context, -(1<<24) + 4);
  auto n_p5 = getint(context, -(1<<24) + 5);

  QualifiedType real32(QualifiedType::VALUE, RealType::get(context, 32));

  CanPassResult r;
  r = canPass(m5,  real32); assert(passesParamNarrowing(r));
  r = canPass(m4,  real32); assert(passesParamNarrowing(r));
  r = canPass(m3,  real32); assert(passesParamNarrowing(r));
  r = canPass(m2,  real32); assert(passesParamNarrowing(r));
  r = canPass(m1,  real32); assert(passesParamNarrowing(r));
  r = canPass(p0,  real32); assert(passesParamNarrowing(r));
  r = canPass(p1,  real32); assert(doesNotPass(r));
  r = canPass(p2,  real32); assert(doesNotPass(r));
  r = canPass(p3,  real32); assert(doesNotPass(r));
  r = canPass(p4,  real32); assert(doesNotPass(r));
  r = canPass(p5,  real32); assert(doesNotPass(r));

  r = canPass(n_m5,  real32); assert(doesNotPass(r));
  r = canPass(n_m4,  real32); assert(doesNotPass(r));
  r = canPass(n_m3,  real32); assert(doesNotPass(r));
  r = canPass(n_m2,  real32); assert(doesNotPass(r));
  r = canPass(n_m1,  real32); assert(doesNotPass(r));
  r = canPass(n_p0,  real32); assert(passesParamNarrowing(r));
  r = canPass(n_p1,  real32); assert(passesParamNarrowing(r));
  r = canPass(n_p2,  real32); assert(passesParamNarrowing(r));
  r = canPass(n_p3,  real32); assert(passesParamNarrowing(r));
  r = canPass(n_p4,  real32); assert(passesParamNarrowing(r));
  r = canPass(n_p5,  real32); assert(passesParamNarrowing(r));
}


int main() {
  test1();
  test2();
  test3();
  test4();
  test5();

  return 0;
}
