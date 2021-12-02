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

#include "chpl/resolution/can-pass.h"
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

static bool passesNumeric(CanPassResult r) {
  return r.passes() &&
         !r.instantiates() &&
         !r.promotes() &&
         r.converts() &&
         r.conversionKind() == CanPassResult::NUMERIC;
}

static bool passesParamNarrowing(CanPassResult r) {
  return r.passes() &&
         !r.instantiates() &&
         !r.promotes() &&
         r.converts() &&
         r.conversionKind() == CanPassResult::PARAM_NARROWING;
}

static bool passesSubtype(CanPassResult r) {
  return r.passes() &&
         !r.instantiates() &&
         !r.promotes() &&
         r.converts() &&
         r.conversionKind() == CanPassResult::SUBTYPE;
}

static bool passesOther(CanPassResult r) {
  return r.passes() &&
         !r.instantiates() &&
         !r.promotes() &&
         r.converts() &&
         r.conversionKind() == CanPassResult::OTHER;
}

static bool doesNotPass(CanPassResult r) {
  return !r.passes();
}

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  QualifiedType int0(QualifiedType::VAR, IntType::get(context, 0));
  QualifiedType int8(QualifiedType::VAR, IntType::get(context, 8));
  QualifiedType int16(QualifiedType::VAR, IntType::get(context, 16));
  QualifiedType int32(QualifiedType::VAR, IntType::get(context, 32));
  QualifiedType int64(QualifiedType::VAR, IntType::get(context, 64));
  QualifiedType real0(QualifiedType::VAR, RealType::get(context, 0));

  CanPassResult r;
  r = canPass(int0, int0); assert(passesAsIs(r));
  r = canPass(int8, int8); assert(passesAsIs(r));
  r = canPass(int16, int16); assert(passesAsIs(r));
  r = canPass(int32, int32); assert(passesAsIs(r));
  r = canPass(int64, int64); assert(passesAsIs(r));
  r = canPass(int0, int64); assert(passesAsIs(r));

  r = canPass(int8, int0); assert(passesNumeric(r));
  r = canPass(int8, int16); assert(passesNumeric(r));
  r = canPass(int8, int32); assert(passesNumeric(r));
  r = canPass(int8, int64); assert(passesNumeric(r));

  r = canPass(int16, int0); assert(passesNumeric(r));
  r = canPass(int16, int8); assert(doesNotPass(r));
  r = canPass(int16, int32); assert(passesNumeric(r));
  r = canPass(int16, int64); assert(passesNumeric(r));

  r = canPass(int32, int0); assert(passesNumeric(r));
  r = canPass(int32, int8); assert(doesNotPass(r));
  r = canPass(int32, int16); assert(doesNotPass(r));
  r = canPass(int32, int64); assert(passesNumeric(r));

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

  QualifiedType int0(QualifiedType::VAR, IntType::get(context, 0));
  QualifiedType int8(QualifiedType::VAR, IntType::get(context, 8));
  QualifiedType int16(QualifiedType::VAR, IntType::get(context, 16));
  QualifiedType int32(QualifiedType::VAR, IntType::get(context, 32));
  QualifiedType int64(QualifiedType::VAR, IntType::get(context, 64));
  QualifiedType real0(QualifiedType::VAR, RealType::get(context, 0));

  CanPassResult r;
  r = canPass(int0, real0); assert(passesNumeric(r));
  r = canPass(int8, real0); assert(passesNumeric(r));
  r = canPass(int16, real0); assert(passesNumeric(r));
  r = canPass(int32, real0); assert(passesNumeric(r));
  r = canPass(int64, real0); assert(passesNumeric(r));
}

static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  QualifiedType bool0(QualifiedType::VAR, BoolType::get(context, 0));
  QualifiedType bool8(QualifiedType::VAR, BoolType::get(context, 8));
  QualifiedType bool16(QualifiedType::VAR, BoolType::get(context, 16));
  QualifiedType bool32(QualifiedType::VAR, BoolType::get(context, 32));
  QualifiedType bool64(QualifiedType::VAR, BoolType::get(context, 64));

  CanPassResult r;
  r = canPass(bool0, bool0); assert(passesAsIs(r));
  r = canPass(bool8, bool8); assert(passesAsIs(r));
  r = canPass(bool16, bool16); assert(passesAsIs(r));
  r = canPass(bool32, bool32); assert(passesAsIs(r));
  r = canPass(bool64, bool64); assert(passesAsIs(r));

  r = canPass(bool0, bool8); assert(passesNumeric(r));
  r = canPass(bool0, bool16); assert(passesNumeric(r));
  r = canPass(bool0, bool32); assert(passesNumeric(r));
  r = canPass(bool0, bool64); assert(passesNumeric(r));

  r = canPass(bool8, bool0); assert(passesNumeric(r));
  r = canPass(bool8, bool16); assert(passesNumeric(r));
  r = canPass(bool8, bool32); assert(passesNumeric(r));
  r = canPass(bool8, bool64); assert(passesNumeric(r));

  r = canPass(bool16, bool0); assert(passesNumeric(r));
  r = canPass(bool16, bool8); assert(passesNumeric(r));
  r = canPass(bool16, bool32); assert(passesNumeric(r));
  r = canPass(bool16, bool64); assert(passesNumeric(r));

  r = canPass(bool32, bool0); assert(passesNumeric(r));
  r = canPass(bool32, bool8); assert(passesNumeric(r));
  r = canPass(bool32, bool16); assert(passesNumeric(r));
  r = canPass(bool32, bool64); assert(passesNumeric(r));

  r = canPass(bool64, bool0); assert(passesNumeric(r));
  r = canPass(bool64, bool8); assert(passesNumeric(r));
  r = canPass(bool64, bool16); assert(passesNumeric(r));
  r = canPass(bool64, bool32); assert(passesNumeric(r));
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

  QualifiedType int8(QualifiedType::VAR, IntType::get(context, 8));
  QualifiedType uint8(QualifiedType::VAR, UintType::get(context, 8));


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

  QualifiedType real32(QualifiedType::VAR, RealType::get(context, 32));

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

static void test6() {
  printf("test6\n");
  Context ctx;
  Context* context = &ctx;

  // test that we can pass param string c_string
  // but we can't pass param string to bytes

  auto s = UniqueString::build(context, "hello");
  auto p = StringParam::get(context, s);
  auto paramString = QualifiedType(QualifiedType::PARAM,
                                   StringType::get(context),
                                   p);

  auto stringQT = QualifiedType(QualifiedType::VAR,
                                StringType::get(context));
  auto cStringQT = QualifiedType(QualifiedType::VAR,
                                 CStringType::get(context));
  auto bytesQT = QualifiedType(QualifiedType::VAR,
                               BytesType::get(context));
  CanPassResult r;
  r = canPass(paramString, stringQT); assert(passesAsIs(r));
  r = canPass(paramString, cStringQT); assert(passesParamNarrowing(r));
  r = canPass(paramString, bytesQT); assert(doesNotPass(r));
}

static void test7() {
  printf("test7\n");
  Context ctx;
  Context* context = &ctx;

  // test that we can pass a child class to its parent class type
  // but that we can't do the inverse.

  ID emptyId;
  std::vector<CompositeType::FieldDetail> emptyFields;

  auto parentName = UniqueString::build(context, "Parent");
  auto childName = UniqueString::build(context, "Child");
  auto basicObj = BasicClassType::getObjectType(context);
  auto basicParent = BasicClassType::get(context, emptyId, parentName,
                                         basicObj, emptyFields);
  auto basicChild = BasicClassType::get(context, emptyId, childName,
                                        basicParent, emptyFields);

  auto borrowed = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
  auto borrowedQ = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NILABLE);
  auto unmanaged = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
  auto unmanagedQ = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NILABLE);
  auto owned = ClassTypeDecorator(ClassTypeDecorator::MANAGED_NONNIL);
  auto ownedQ = ClassTypeDecorator(ClassTypeDecorator::MANAGED_NILABLE);

  auto borrowedParent = QualifiedType(QualifiedType::VAR,
                                      ClassType::get(context, basicParent,
                                                     nullptr, borrowed));
  auto borrowedChild = QualifiedType(QualifiedType::VAR,
                                     ClassType::get(context, basicChild,
                                                    nullptr, borrowed));
  auto borrowedParentQ = QualifiedType(QualifiedType::VAR,
                                       ClassType::get(context, basicParent,
                                        nullptr, borrowedQ));
  auto borrowedChildQ = QualifiedType(QualifiedType::VAR,
                                      ClassType::get(context, basicChild,
                                                     nullptr, borrowedQ));
  auto unmanagedParent = QualifiedType(QualifiedType::VAR,
                                       ClassType::get(context, basicParent,
                                                      nullptr, unmanaged));
  auto unmanagedChild = QualifiedType(QualifiedType::VAR,
                                      ClassType::get(context, basicChild,
                                                     nullptr, unmanaged));
  auto unmanagedParentQ = QualifiedType(QualifiedType::VAR,
                                        ClassType::get(context, basicParent,
                                                       nullptr, unmanagedQ));
  auto unmanagedChildQ = QualifiedType(QualifiedType::VAR,
                                       ClassType::get(context, basicChild,
                                                      nullptr, unmanagedQ));
  auto ownedParent = QualifiedType(QualifiedType::VAR,
                                   ClassType::get(context, basicParent,
                                                  AnyOwnedType::get(context),
                                                  owned));
  auto ownedChild = QualifiedType(QualifiedType::VAR,
                                  ClassType::get(context, basicChild,
                                                 AnyOwnedType::get(context),
                                                 owned));
  auto ownedParentQ = QualifiedType(QualifiedType::VAR,
                                    ClassType::get(context, basicParent,
                                                   AnyOwnedType::get(context),
                                                   ownedQ));
  auto ownedChildQ = QualifiedType(QualifiedType::VAR,
                                   ClassType::get(context, basicChild,
                                                  AnyOwnedType::get(context),
                                                  ownedQ));

  CanPassResult r;
  // first, check same type passes
  r = canPass(borrowedParent, borrowedParent); assert(passesAsIs(r));
  r = canPass(borrowedChild, borrowedChild); assert(passesAsIs(r));
  r = canPass(borrowedParentQ, borrowedParentQ); assert(passesAsIs(r));
  r = canPass(borrowedChildQ, borrowedChildQ); assert(passesAsIs(r));
  r = canPass(unmanagedParent, unmanagedParent); assert(passesAsIs(r));
  r = canPass(unmanagedChild, unmanagedChild); assert(passesAsIs(r));
  r = canPass(unmanagedParentQ, unmanagedParentQ); assert(passesAsIs(r));
  r = canPass(unmanagedChildQ, unmanagedChildQ); assert(passesAsIs(r));
  r = canPass(ownedParent, ownedParent); assert(passesAsIs(r));
  r = canPass(ownedChild, ownedChild); assert(passesAsIs(r));
  r = canPass(ownedParentQ, ownedParentQ); assert(passesAsIs(r));
  r = canPass(ownedChildQ, ownedChildQ); assert(passesAsIs(r));

  // next check parent/child toggling nilability within each group
  r = canPass(borrowedChild, borrowedParent); assert(passesSubtype(r));
  r = canPass(borrowedChild, borrowedParentQ); assert(passesOther(r));
  r = canPass(borrowedChildQ, borrowedParent); assert(doesNotPass(r));
  r = canPass(borrowedChildQ, borrowedParentQ); assert(passesSubtype(r));

  r = canPass(borrowedParent, borrowedChild); assert(doesNotPass(r));
  r = canPass(borrowedParent, borrowedChildQ); assert(doesNotPass(r));
  r = canPass(borrowedParentQ, borrowedChild); assert(doesNotPass(r));
  r = canPass(borrowedParentQ, borrowedChildQ); assert(doesNotPass(r));

  r = canPass(unmanagedChild, unmanagedParent); assert(passesSubtype(r));
  r = canPass(unmanagedChild, unmanagedParentQ); assert(passesOther(r));
  r = canPass(unmanagedChildQ, unmanagedParent); assert(doesNotPass(r));
  r = canPass(unmanagedChildQ, unmanagedParentQ); assert(passesSubtype(r));

  r = canPass(unmanagedParent, unmanagedChild); assert(doesNotPass(r));
  r = canPass(unmanagedParent, unmanagedChildQ); assert(doesNotPass(r));
  r = canPass(unmanagedParentQ, unmanagedChild); assert(doesNotPass(r));
  r = canPass(unmanagedParentQ, unmanagedChildQ); assert(doesNotPass(r));

  r = canPass(ownedChild, ownedParent); assert(passesSubtype(r));
  r = canPass(ownedChild, ownedParentQ); assert(passesOther(r));
  r = canPass(ownedChildQ, ownedParent); assert(doesNotPass(r));
  r = canPass(ownedChildQ, ownedParentQ); assert(passesSubtype(r));

  r = canPass(ownedParent, ownedChild); assert(doesNotPass(r));
  r = canPass(ownedParent, ownedChildQ); assert(doesNotPass(r));
  r = canPass(ownedParentQ, ownedChild); assert(doesNotPass(r));
  r = canPass(ownedParentQ, ownedChildQ); assert(doesNotPass(r));
}


int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();

  return 0;
}
