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

/*
static bool passesOther(CanPassResult r) {
  return r.passes() &&
         !r.instantiates() &&
         !r.promotes() &&
         r.converts() &&
         r.conversionKind() == CanPassResult::OTHER;
}
*/

static bool doesNotPass(CanPassResult r) {
  return !r.passes();
}

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;
  Context* c = context;

  QualifiedType int0(QualifiedType::VAR, IntType::get(context, 0));
  QualifiedType int8(QualifiedType::VAR, IntType::get(context, 8));
  QualifiedType int16(QualifiedType::VAR, IntType::get(context, 16));
  QualifiedType int32(QualifiedType::VAR, IntType::get(context, 32));
  QualifiedType int64(QualifiedType::VAR, IntType::get(context, 64));
  QualifiedType real0(QualifiedType::VAR, RealType::get(context, 0));

  CanPassResult r;
  r = canPass(c, int0, int0); assert(passesAsIs(r));
  r = canPass(c, int8, int8); assert(passesAsIs(r));
  r = canPass(c, int16, int16); assert(passesAsIs(r));
  r = canPass(c, int32, int32); assert(passesAsIs(r));
  r = canPass(c, int64, int64); assert(passesAsIs(r));
  r = canPass(c, int0, int64); assert(passesAsIs(r));

  r = canPass(c, int8, int0); assert(passesNumeric(r));
  r = canPass(c, int8, int16); assert(passesNumeric(r));
  r = canPass(c, int8, int32); assert(passesNumeric(r));
  r = canPass(c, int8, int64); assert(passesNumeric(r));

  r = canPass(c, int16, int0); assert(passesNumeric(r));
  r = canPass(c, int16, int8); assert(doesNotPass(r));
  r = canPass(c, int16, int32); assert(passesNumeric(r));
  r = canPass(c, int16, int64); assert(passesNumeric(r));

  r = canPass(c, int32, int0); assert(passesNumeric(r));
  r = canPass(c, int32, int8); assert(doesNotPass(r));
  r = canPass(c, int32, int16); assert(doesNotPass(r));
  r = canPass(c, int32, int64); assert(passesNumeric(r));

  r = canPass(c, int64, int0); assert(passesAsIs(r));
  r = canPass(c, int64, int8); assert(doesNotPass(r));
  r = canPass(c, int64, int16); assert(doesNotPass(r));
  r = canPass(c, int64, int32); assert(doesNotPass(r));

  r = canPass(c, int0, int8); assert(doesNotPass(r));
  r = canPass(c, int0, int16); assert(doesNotPass(r));
  r = canPass(c, int0, int32); assert(doesNotPass(r));
  r = canPass(c, int0, int64); assert(passesAsIs(r));
}

static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;
  Context* c = context;

  QualifiedType int0(QualifiedType::VAR, IntType::get(context, 0));
  QualifiedType int8(QualifiedType::VAR, IntType::get(context, 8));
  QualifiedType int16(QualifiedType::VAR, IntType::get(context, 16));
  QualifiedType int32(QualifiedType::VAR, IntType::get(context, 32));
  QualifiedType int64(QualifiedType::VAR, IntType::get(context, 64));
  QualifiedType real0(QualifiedType::VAR, RealType::get(context, 0));

  CanPassResult r;
  r = canPass(c, int0, real0); assert(passesNumeric(r));
  r = canPass(c, int8, real0); assert(passesNumeric(r));
  r = canPass(c, int16, real0); assert(passesNumeric(r));
  r = canPass(c, int32, real0); assert(passesNumeric(r));
  r = canPass(c, int64, real0); assert(passesNumeric(r));
}

static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;
  Context* c = context;

  QualifiedType bool0(QualifiedType::VAR, BoolType::get(context, 0));
  QualifiedType bool8(QualifiedType::VAR, BoolType::get(context, 8));
  QualifiedType bool16(QualifiedType::VAR, BoolType::get(context, 16));
  QualifiedType bool32(QualifiedType::VAR, BoolType::get(context, 32));
  QualifiedType bool64(QualifiedType::VAR, BoolType::get(context, 64));

  CanPassResult r;
  r = canPass(c, bool0, bool0); assert(passesAsIs(r));
  r = canPass(c, bool8, bool8); assert(passesAsIs(r));
  r = canPass(c, bool16, bool16); assert(passesAsIs(r));
  r = canPass(c, bool32, bool32); assert(passesAsIs(r));
  r = canPass(c, bool64, bool64); assert(passesAsIs(r));

  r = canPass(c, bool0, bool8); assert(passesNumeric(r));
  r = canPass(c, bool0, bool16); assert(passesNumeric(r));
  r = canPass(c, bool0, bool32); assert(passesNumeric(r));
  r = canPass(c, bool0, bool64); assert(passesNumeric(r));

  r = canPass(c, bool8, bool0); assert(passesNumeric(r));
  r = canPass(c, bool8, bool16); assert(passesNumeric(r));
  r = canPass(c, bool8, bool32); assert(passesNumeric(r));
  r = canPass(c, bool8, bool64); assert(passesNumeric(r));

  r = canPass(c, bool16, bool0); assert(passesNumeric(r));
  r = canPass(c, bool16, bool8); assert(passesNumeric(r));
  r = canPass(c, bool16, bool32); assert(passesNumeric(r));
  r = canPass(c, bool16, bool64); assert(passesNumeric(r));

  r = canPass(c, bool32, bool0); assert(passesNumeric(r));
  r = canPass(c, bool32, bool8); assert(passesNumeric(r));
  r = canPass(c, bool32, bool16); assert(passesNumeric(r));
  r = canPass(c, bool32, bool64); assert(passesNumeric(r));

  r = canPass(c, bool64, bool0); assert(passesNumeric(r));
  r = canPass(c, bool64, bool8); assert(passesNumeric(r));
  r = canPass(c, bool64, bool16); assert(passesNumeric(r));
  r = canPass(c, bool64, bool32); assert(passesNumeric(r));
}

static void test4() {
  printf("test4\n");
  Context ctx;
  Context* context = &ctx;
  Context* c = context;

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
  r = canPass(c, oneInt0,  int8); assert(passesParamNarrowing(r));
  r = canPass(c, oneInt8,  int8); assert(passesAsIs(r));
  r = canPass(c, oneInt16, int8); assert(passesParamNarrowing(r));
  r = canPass(c, oneInt32, int8); assert(passesParamNarrowing(r));
  r = canPass(c, oneInt64, int8); assert(passesParamNarrowing(r));

  r = canPass(c, oneUint0,  int8); assert(passesParamNarrowing(r));
  r = canPass(c, oneUint8,  int8); assert(passesParamNarrowing(r));
  r = canPass(c, oneUint16, int8); assert(passesParamNarrowing(r));
  r = canPass(c, oneUint32, int8); assert(passesParamNarrowing(r));
  r = canPass(c, oneUint64, int8); assert(passesParamNarrowing(r));

  r = canPass(c, oneInt0,  uint8); assert(passesParamNarrowing(r));
  r = canPass(c, oneInt8,  uint8); assert(passesParamNarrowing(r));
  r = canPass(c, oneInt16, uint8); assert(passesParamNarrowing(r));
  r = canPass(c, oneInt32, uint8); assert(passesParamNarrowing(r));
  r = canPass(c, oneInt64, uint8); assert(passesParamNarrowing(r));

  r = canPass(c, oneUint0,  uint8); assert(passesParamNarrowing(r));
  r = canPass(c, oneUint8,  uint8); assert(passesAsIs(r));
  r = canPass(c, oneUint16, uint8); assert(passesParamNarrowing(r));
  r = canPass(c, oneUint32, uint8); assert(passesParamNarrowing(r));
  r = canPass(c, oneUint64, uint8); assert(passesParamNarrowing(r));

  r = canPass(c, negInt0,  int8); assert(passesParamNarrowing(r));
  r = canPass(c, negInt8,  int8); assert(passesAsIs(r));
  r = canPass(c, negInt16, int8); assert(passesParamNarrowing(r));
  r = canPass(c, negInt32, int8); assert(passesParamNarrowing(r));
  r = canPass(c, negInt64, int8); assert(passesParamNarrowing(r));

  r = canPass(c, negInt0,  uint8); assert(doesNotPass(r));
  r = canPass(c, negInt8,  uint8); assert(doesNotPass(r));
  r = canPass(c, negInt16, uint8); assert(doesNotPass(r));
  r = canPass(c, negInt32, uint8); assert(doesNotPass(r));
  r = canPass(c, negInt64, uint8); assert(doesNotPass(r));
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
  Context* c = context;

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
  r = canPass(c, m5,  real32); assert(passesParamNarrowing(r));
  r = canPass(c, m4,  real32); assert(passesParamNarrowing(r));
  r = canPass(c, m3,  real32); assert(passesParamNarrowing(r));
  r = canPass(c, m2,  real32); assert(passesParamNarrowing(r));
  r = canPass(c, m1,  real32); assert(passesParamNarrowing(r));
  r = canPass(c, p0,  real32); assert(passesParamNarrowing(r));
  r = canPass(c, p1,  real32); assert(doesNotPass(r));
  r = canPass(c, p2,  real32); assert(doesNotPass(r));
  r = canPass(c, p3,  real32); assert(doesNotPass(r));
  r = canPass(c, p4,  real32); assert(doesNotPass(r));
  r = canPass(c, p5,  real32); assert(doesNotPass(r));

  r = canPass(c, n_m5,  real32); assert(doesNotPass(r));
  r = canPass(c, n_m4,  real32); assert(doesNotPass(r));
  r = canPass(c, n_m3,  real32); assert(doesNotPass(r));
  r = canPass(c, n_m2,  real32); assert(doesNotPass(r));
  r = canPass(c, n_m1,  real32); assert(doesNotPass(r));
  r = canPass(c, n_p0,  real32); assert(passesParamNarrowing(r));
  r = canPass(c, n_p1,  real32); assert(passesParamNarrowing(r));
  r = canPass(c, n_p2,  real32); assert(passesParamNarrowing(r));
  r = canPass(c, n_p3,  real32); assert(passesParamNarrowing(r));
  r = canPass(c, n_p4,  real32); assert(passesParamNarrowing(r));
  r = canPass(c, n_p5,  real32); assert(passesParamNarrowing(r));
}

static void test6() {
  printf("test6\n");
  Context ctx;
  Context* context = &ctx;
  Context* c = context;

  // test that we can pass param string c_string
  // but we can't pass param string to bytes

  auto s = UniqueString::get(context, "hello");
  auto p = StringParam::get(context, s);
  auto paramString = QualifiedType(QualifiedType::PARAM,
                                   RecordType::getStringType(context),
                                   p);

  auto stringQT = QualifiedType(QualifiedType::VAR,
                                RecordType::getStringType(context));
  auto cStringQT = QualifiedType(QualifiedType::VAR,
                                 CStringType::get(context));
  auto bytesQT = QualifiedType(QualifiedType::VAR,
                               RecordType::getBytesType(context));
  CanPassResult r;
  r = canPass(c, paramString, stringQT); assert(passesAsIs(r));
  r = canPass(c, paramString, cStringQT); assert(passesParamNarrowing(r));
  r = canPass(c, paramString, bytesQT); assert(doesNotPass(r));
}

static void test7() {
  printf("test7\n");
  Context ctx;
  Context* context = &ctx;
  Context* c = context;

  // test that we can pass a child class to its parent class type
  // but that we can't do the inverse.

  ID emptyId;

  auto parentName = UniqueString::get(context, "Parent");
  auto childName = UniqueString::get(context, "Child");
  auto basicObj = BasicClassType::getObjectType(context);
  auto basicParent = BasicClassType::get(context, emptyId, parentName,
                                         basicObj,
                                        /* instantiatedFrom */ nullptr,
                                        SubstitutionsMap());

  auto basicChild = BasicClassType::get(context, emptyId, childName,
                                        basicParent,
                                        /* instantiatedFrom */ nullptr,
                                        SubstitutionsMap());

  auto borrowed = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
  auto borrowedQ = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NILABLE);
  auto unmanaged = ClassTypeDecorator(ClassTypeDecorator::UNMANAGED_NONNIL);
  auto unmanagedQ = ClassTypeDecorator(ClassTypeDecorator::UNMANAGED_NILABLE);
  auto owned = ClassTypeDecorator(ClassTypeDecorator::MANAGED_NONNIL);
  auto ownedQ = ClassTypeDecorator(ClassTypeDecorator::MANAGED_NILABLE);

  auto qt = [=](const BasicClassType* bctArg,
                const Type* mgmtArg,
                ClassTypeDecorator decArg) -> QualifiedType {
    return QualifiedType(QualifiedType::VAR,
                         ClassType::get(context, bctArg, mgmtArg, decArg));
  };

  auto ownedMgmt = AnyOwnedType::get(context);

  auto borrowedParent   = qt(basicParent, nullptr,   borrowed);
  auto borrowedChild    = qt(basicChild,  nullptr,   borrowed);
  auto borrowedParentQ  = qt(basicParent, nullptr,   borrowedQ);
  auto borrowedChildQ   = qt(basicChild,  nullptr,   borrowedQ);
  auto unmanagedParent  = qt(basicParent, nullptr,   unmanaged);
  auto unmanagedChild   = qt(basicChild,  nullptr,   unmanaged);
  auto unmanagedParentQ = qt(basicParent, nullptr,   unmanagedQ);
  auto unmanagedChildQ  = qt(basicChild,  nullptr,   unmanagedQ);
  auto ownedParent      = qt(basicParent, ownedMgmt, owned);
  auto ownedChild       = qt(basicChild,  ownedMgmt, owned);
  auto ownedParentQ     = qt(basicParent, ownedMgmt, ownedQ);
  auto ownedChildQ      = qt(basicChild,  ownedMgmt, ownedQ);

  CanPassResult r;

  // first, check same type passes
  r = canPass(c, borrowedParent,   borrowedParent);   assert(passesAsIs(r));
  r = canPass(c, borrowedChild,    borrowedChild);    assert(passesAsIs(r));
  r = canPass(c, borrowedParentQ,  borrowedParentQ);  assert(passesAsIs(r));
  r = canPass(c, borrowedChildQ,   borrowedChildQ);   assert(passesAsIs(r));
  r = canPass(c, unmanagedParent,  unmanagedParent);  assert(passesAsIs(r));
  r = canPass(c, unmanagedChild,   unmanagedChild);   assert(passesAsIs(r));
  r = canPass(c, unmanagedParentQ, unmanagedParentQ); assert(passesAsIs(r));
  r = canPass(c, unmanagedChildQ,  unmanagedChildQ);  assert(passesAsIs(r));
  r = canPass(c, ownedParent,      ownedParent);      assert(passesAsIs(r));
  r = canPass(c, ownedChild,       ownedChild);       assert(passesAsIs(r));
  r = canPass(c, ownedParentQ,     ownedParentQ);     assert(passesAsIs(r));
  r = canPass(c, ownedChildQ,      ownedChildQ);      assert(passesAsIs(r));

  // next check child-parent toggling nilability within each group
  r = canPass(c, borrowedChild,    borrowedParent);   assert(passesSubtype(r));
  r = canPass(c, borrowedChild,    borrowedParentQ);  assert(passesSubtype(r));
  r = canPass(c, borrowedChildQ,   borrowedParent);   assert(doesNotPass(r));
  r = canPass(c, borrowedChildQ,   borrowedParentQ);  assert(passesSubtype(r));

  r = canPass(c, borrowedParent,   borrowedChild);    assert(doesNotPass(r));
  r = canPass(c, borrowedParent,   borrowedChildQ);   assert(doesNotPass(r));
  r = canPass(c, borrowedParentQ,  borrowedChild);    assert(doesNotPass(r));
  r = canPass(c, borrowedParentQ,  borrowedChildQ);   assert(doesNotPass(r));

  r = canPass(c, unmanagedChild,   unmanagedParent);  assert(passesSubtype(r));
  r = canPass(c, unmanagedChild,   unmanagedParentQ); assert(passesSubtype(r));
  r = canPass(c, unmanagedChildQ,  unmanagedParent);  assert(doesNotPass(r));
  r = canPass(c, unmanagedChildQ,  unmanagedParentQ); assert(passesSubtype(r));

  r = canPass(c, unmanagedParent,  unmanagedChild);   assert(doesNotPass(r));
  r = canPass(c, unmanagedParent,  unmanagedChildQ);  assert(doesNotPass(r));
  r = canPass(c, unmanagedParentQ, unmanagedChild);   assert(doesNotPass(r));
  r = canPass(c, unmanagedParentQ, unmanagedChildQ);  assert(doesNotPass(r));

  r = canPass(c, ownedChild,       ownedParent);      assert(passesSubtype(r));
  r = canPass(c, ownedChild,       ownedParentQ);     assert(passesSubtype(r));
  r = canPass(c, ownedChildQ,      ownedParent);      assert(doesNotPass(r));
  r = canPass(c, ownedChildQ,      ownedParentQ);     assert(passesSubtype(r));

  r = canPass(c, ownedParent,      ownedChild);       assert(doesNotPass(r));
  r = canPass(c, ownedParent,      ownedChildQ);      assert(doesNotPass(r));
  r = canPass(c, ownedParentQ,     ownedChild);       assert(doesNotPass(r));
  r = canPass(c, ownedParentQ,     ownedChildQ);      assert(doesNotPass(r));

  // next, check cross-management with same type, toggling nilability

  // borrowed - unmanaged
  r = canPass(c, borrowedChild,    unmanagedChild);   assert(doesNotPass(r));
  r = canPass(c, borrowedChild,    unmanagedChildQ);  assert(doesNotPass(r));
  r = canPass(c, borrowedChildQ,   unmanagedChild);   assert(doesNotPass(r));
  r = canPass(c, borrowedChildQ,   unmanagedChildQ);  assert(doesNotPass(r));

  // borrowed - owned
  r = canPass(c, borrowedChild,    ownedChild);       assert(doesNotPass(r));
  r = canPass(c, borrowedChild,    ownedChildQ);      assert(doesNotPass(r));
  r = canPass(c, borrowedChildQ,   ownedChild);       assert(doesNotPass(r));
  r = canPass(c, borrowedChildQ,   ownedChildQ);      assert(doesNotPass(r));

  // unmanaged - borrowed
  r = canPass(c, unmanagedChild,   borrowedChild);    assert(passesSubtype(r));
  r = canPass(c, unmanagedChild,   borrowedChildQ);   assert(passesSubtype(r));
  r = canPass(c, unmanagedChildQ,  borrowedChild);    assert(doesNotPass(r));
  r = canPass(c, unmanagedChildQ,  borrowedChildQ);   assert(passesSubtype(r));

  // unamanaged - owned
  r = canPass(c, unmanagedChild,   ownedChild);       assert(doesNotPass(r));
  r = canPass(c, unmanagedChild,   ownedChildQ);      assert(doesNotPass(r));
  r = canPass(c, unmanagedChildQ,  ownedChild);       assert(doesNotPass(r));
  r = canPass(c, unmanagedChildQ,  ownedChildQ);      assert(doesNotPass(r));

  // owned - borrowed
  r = canPass(c, ownedChild,       borrowedChild);    assert(passesSubtype(r));
  r = canPass(c, ownedChild,       borrowedChildQ);   assert(passesSubtype(r));
  r = canPass(c, ownedChildQ,      borrowedChild);    assert(doesNotPass(r));
  r = canPass(c, ownedChildQ,      borrowedChildQ);   assert(passesSubtype(r));

  // owned - unmanaged
  r = canPass(c, ownedChild,       unmanagedChild);   assert(doesNotPass(r));
  r = canPass(c, ownedChild,       unmanagedChildQ);  assert(doesNotPass(r));
  r = canPass(c, ownedChildQ,      unmanagedChild);   assert(doesNotPass(r));
  r = canPass(c, ownedChildQ,      unmanagedChildQ);  assert(doesNotPass(r));

  // next, same as above, with Child and Parent
  // borrowed - unmanaged
  r = canPass(c, borrowedChild,    unmanagedParent);  assert(doesNotPass(r));
  r = canPass(c, borrowedChild,    unmanagedParentQ); assert(doesNotPass(r));
  r = canPass(c, borrowedChildQ,   unmanagedParent);  assert(doesNotPass(r));
  r = canPass(c, borrowedChildQ,   unmanagedParentQ); assert(doesNotPass(r));

  // borrowed - owned
  r = canPass(c, borrowedChild,    ownedParent);      assert(doesNotPass(r));
  r = canPass(c, borrowedChild,    ownedParentQ);     assert(doesNotPass(r));
  r = canPass(c, borrowedChildQ,   ownedParent);      assert(doesNotPass(r));
  r = canPass(c, borrowedChildQ,   ownedParentQ);     assert(doesNotPass(r));

  // unmanaged - borrowed
  r = canPass(c, unmanagedChild,   borrowedParent);   assert(passesSubtype(r));
  r = canPass(c, unmanagedChild,   borrowedParentQ);  assert(passesSubtype(r));
  r = canPass(c, unmanagedChildQ,  borrowedParent);   assert(doesNotPass(r));
  r = canPass(c, unmanagedChildQ,  borrowedParentQ);  assert(passesSubtype(r));

  // unamanaged - owned
  r = canPass(c, unmanagedChild,   ownedParent);      assert(doesNotPass(r));
  r = canPass(c, unmanagedChild,   ownedParentQ);     assert(doesNotPass(r));
  r = canPass(c, unmanagedChildQ,  ownedParent);      assert(doesNotPass(r));
  r = canPass(c, unmanagedChildQ,  ownedParentQ);     assert(doesNotPass(r));

  // owned - borrowed
  r = canPass(c, ownedChild,       borrowedParent);   assert(passesSubtype(r));
  r = canPass(c, ownedChild,       borrowedParentQ);  assert(passesSubtype(r));
  r = canPass(c, ownedChildQ,      borrowedParent);   assert(doesNotPass(r));
  r = canPass(c, ownedChildQ,      borrowedParentQ);  assert(passesSubtype(r));

  // owned - unmanaged
  r = canPass(c, ownedChild,       unmanagedParent);  assert(doesNotPass(r));
  r = canPass(c, ownedChild,       unmanagedParentQ); assert(doesNotPass(r));
  r = canPass(c, ownedChildQ,      unmanagedParent);  assert(doesNotPass(r));
  r = canPass(c, ownedChildQ,      unmanagedParentQ); assert(doesNotPass(r));
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
