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

#include "chpl/types/all-types.h"

#include <cassert>

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

using namespace chpl;
using namespace types;

// We suppose we have
//   class C { var field; }
// and
//   class D { var a; var b; var c; }

static owned<BasicClassType> makeC(Context* context,
                                   const Type* fieldType) {
  ID emptyId;

  auto name = UniqueString::build(context, "C");
  std::vector<CompositeType::FieldDetail> fields;

  fields.push_back(CompositeType::FieldDetail(
                       UniqueString::build(context, "field"),
                       /* hasDefaultValue */ false,
                       /* decl */ nullptr,
                       QualifiedType()));

  auto ret = toOwned(new BasicClassType(emptyId, name, fields,
                                        /* instantiatedFrom */ nullptr,
                                        CompositeType::SubstitutionsMap()));

  ret->setParentType(BasicClassType::getObjectType(context));
  ret->setFieldType(0, QualifiedType(QualifiedType::VAR, fieldType));
  ret->finalizeFieldTypes();

  return ret;
}

static owned<BasicClassType> makeD(Context* context,
                                   const Type* aType,
                                   const Type* bType,
                                   const Type* cType) {
  ID emptyId;

  auto name = UniqueString::build(context, "D");
  std::vector<CompositeType::FieldDetail> fields;

  fields.push_back(CompositeType::FieldDetail(
                       UniqueString::build(context, "a"),
                       /* hasDefaultValue */ false,
                       /* decl */ nullptr,
                       QualifiedType()));
  fields.push_back(CompositeType::FieldDetail(
                       UniqueString::build(context, "b"),
                       /* hasDefaultValue */ false,
                       /* decl */ nullptr,
                       QualifiedType()));
  fields.push_back(CompositeType::FieldDetail(
                       UniqueString::build(context, "c"),
                       /* hasDefaultValue */ false,
                       /* decl */ nullptr,
                       QualifiedType()));

  auto ret = toOwned(new BasicClassType(emptyId, name, fields,
                                        /* instantiatedFrom */ nullptr,
                                        CompositeType::SubstitutionsMap()));

  ret->setParentType(BasicClassType::getObjectType(context));
  ret->setFieldType(0, QualifiedType(QualifiedType::VAR, aType));
  ret->setFieldType(1, QualifiedType(QualifiedType::VAR, bType));
  ret->setFieldType(2, QualifiedType(QualifiedType::VAR, cType));
  ret->finalizeFieldTypes();

  return ret;
}


static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  // Construct two C types containing 'int'
  auto c1 = makeC(context, IntType::get(context, 0));
  auto c2 = makeC(context, IntType::get(context, 0));

  // Check to see if these match
  assert(c1->completeMatch(c2.get()));
  assert(c2->completeMatch(c1.get()));
}

static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  // Construct two C types, one with 'int' and another with 'real'
  auto c1 = makeC(context, IntType::get(context, 0));
  auto c2 = makeC(context, RealType::get(context, 0));

  // Check to see if these match
  assert(!c1->completeMatch(c2.get()));
  assert(!c2->completeMatch(c1.get()));
}

static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  // Construct two D types, one with 'int','real' and the
  // other with 'real','int'. They should not match.
  auto d1 = makeD(context,
                  IntType::get(context, 0),
                  RealType::get(context, 0),
                  IntType::get(context, 0));
  auto d2 = makeD(context,
                  RealType::get(context, 0),
                  IntType::get(context, 0),
                  IntType::get(context, 0));

  // Check to see if these match
  assert(!d1->completeMatch(d2.get()));
  assert(!d2->completeMatch(d1.get()));
}

static void test4() {
  printf("test4\n");
  Context ctx;
  Context* context = &ctx;

  // Construct two C types that refer to themselves
  // by adjusting fields after construction.
  auto c1 = makeC(context, IntType::get(context, 0));
  c1->setFieldType(0, QualifiedType(QualifiedType::VAR, c1.get()));
  c1->finalizeFieldTypes();

  auto c2 = makeC(context, IntType::get(context, 0));
  c2->setFieldType(0, QualifiedType(QualifiedType::VAR, c2.get()));
  c2->finalizeFieldTypes();

  // Check to see if these two match
  assert(c1->completeMatch(c2.get()));
  assert(c2->completeMatch(c1.get()));
}

static void test5() {
  printf("test5\n");
  Context ctx;
  Context* context = &ctx;

  // Construct two C types that refer to another C type
  // by adjusting fields after construction.
  // c -> cc -> c
  auto c1 = makeC(context, IntType::get(context, 0));
  auto cc1 = makeC(context, IntType::get(context, 0));

  c1->setFieldType(0, QualifiedType(QualifiedType::VAR, cc1.get()));
  c1->finalizeFieldTypes();
  cc1->setFieldType(0, QualifiedType(QualifiedType::VAR, c1.get()));
  cc1->finalizeFieldTypes();

  auto c2 = makeC(context, IntType::get(context, 0));
  auto cc2 = makeC(context, IntType::get(context, 0));

  c2->setFieldType(0, QualifiedType(QualifiedType::VAR, cc2.get()));
  c2->finalizeFieldTypes();
  cc2->setFieldType(0, QualifiedType(QualifiedType::VAR, c2.get()));
  cc2->finalizeFieldTypes();

  // Check to see if these two match
  assert(c1->completeMatch(c2.get()));
  assert(c2->completeMatch(c1.get()));
}

static void test6() {
  printf("test6\n");
  Context ctx;
  Context* context = &ctx;

  // Construct two C types that refer to another C type
  // by adjusting fields after construction.
  // 1: c -> cc -> c
  // 2: c -> c
  auto c1 = makeC(context, IntType::get(context, 0));
  auto cc1 = makeC(context, IntType::get(context, 0));

  c1->setFieldType(0, QualifiedType(QualifiedType::VAR, cc1.get()));
  c1->finalizeFieldTypes();
  cc1->setFieldType(0, QualifiedType(QualifiedType::VAR, c1.get()));
  cc1->finalizeFieldTypes();

  auto c2 = makeC(context, IntType::get(context, 0));

  c2->setFieldType(0, QualifiedType(QualifiedType::VAR, c2.get()));
  c2->finalizeFieldTypes();

  // Check to see if these two match
  assert(!c1->completeMatch(c2.get()));
  assert(!c2->completeMatch(c1.get()));
}

static void test7() {
  printf("test7\n");
  Context ctx;
  Context* context = &ctx;

  // Construct two C types that refer to another C type
  // by adjusting fields after construction.
  //    c -> d -> c
  //           -> c
  auto iType = IntType::get(context, 0);
  auto c1 = makeC(context, iType);
  auto d1 = makeD(context, iType, iType, iType);

  c1->setFieldType(0, QualifiedType(QualifiedType::VAR, d1.get()));
  c1->finalizeFieldTypes();
  d1->setFieldType(0, QualifiedType(QualifiedType::VAR, c1.get()));
  d1->setFieldType(1, QualifiedType(QualifiedType::VAR, c1.get()));
  d1->finalizeFieldTypes();

  auto c2 = makeC(context, iType);
  auto d2 = makeD(context, iType, iType, iType);

  c2->setFieldType(0, QualifiedType(QualifiedType::VAR, d2.get()));
  c2->finalizeFieldTypes();
  d2->setFieldType(0, QualifiedType(QualifiedType::VAR, c2.get()));
  d2->setFieldType(1, QualifiedType(QualifiedType::VAR, c2.get()));
  d2->finalizeFieldTypes();

  // Check to see if these two match
  assert(c1->completeMatch(c2.get()));
  assert(c2->completeMatch(c1.get()));
  assert(d1->completeMatch(d2.get()));
  assert(d2->completeMatch(d1.get()));
}

static void test8() {
  printf("test8\n");
  Context ctx;
  Context* context = &ctx;

  // Construct two C types that refer to another C type
  // by adjusting fields after construction.
  // 1: c -> d -> c
  //           -> c
  // 2: c -> d -> c
  //           -> c'
  auto iType = IntType::get(context, 0);
  auto c1 = makeC(context, iType);
  auto d1 = makeD(context, iType, iType, iType);

  c1->setFieldType(0, QualifiedType(QualifiedType::VAR, d1.get()));
  c1->finalizeFieldTypes();
  d1->setFieldType(0, QualifiedType(QualifiedType::VAR, c1.get()));
  d1->setFieldType(1, QualifiedType(QualifiedType::VAR, c1.get()));
  d1->finalizeFieldTypes();

  auto c2 = makeC(context, iType);
  auto c2p = makeC(context, iType);
  auto d2 = makeD(context, iType, iType, iType);

  c2->setFieldType(0, QualifiedType(QualifiedType::VAR, d2.get()));
  c2->finalizeFieldTypes();
  c2p->setFieldType(0, QualifiedType(QualifiedType::VAR, d2.get()));
  c2p->finalizeFieldTypes();
  d2->setFieldType(0, QualifiedType(QualifiedType::VAR, c2.get()));
  d2->setFieldType(1, QualifiedType(QualifiedType::VAR, c2p.get()));
  d2->finalizeFieldTypes();

  // Check to see if these two match
  assert(!c1->completeMatch(c2.get()));
  assert(!c2->completeMatch(c1.get()));
  assert(!d1->completeMatch(d2.get()));
  assert(!d2->completeMatch(d1.get()));
}

static void test9() {
  printf("test9\n");
  Context ctx;
  Context* context = &ctx;

  // Construct two C types that refer to another C type
  // by adjusting fields after construction.
  //    c -> c' -> c'' -> c
  auto iType = IntType::get(context, 0);
  auto c1 = makeC(context, iType);
  auto c1p = makeC(context, iType);
  auto c1pp = makeC(context, iType);

  c1->setFieldType(0, QualifiedType(QualifiedType::VAR, c1p.get()));
  c1->finalizeFieldTypes();
  c1p->setFieldType(0, QualifiedType(QualifiedType::VAR, c1pp.get()));
  c1p->finalizeFieldTypes();
  c1pp->setFieldType(0, QualifiedType(QualifiedType::VAR, c1.get()));
  c1pp->finalizeFieldTypes();

  auto c2 = makeC(context, iType);
  auto c2p = makeC(context, iType);
  auto c2pp = makeC(context, iType);

  c2->setFieldType(0, QualifiedType(QualifiedType::VAR, c2p.get()));
  c2->finalizeFieldTypes();
  c2p->setFieldType(0, QualifiedType(QualifiedType::VAR, c2pp.get()));
  c2p->finalizeFieldTypes();
  c2pp->setFieldType(0, QualifiedType(QualifiedType::VAR, c2.get()));
  c2pp->finalizeFieldTypes();

  // Check to see if these two match
  assert(c1->completeMatch(c2.get()));
  assert(c2->completeMatch(c1.get()));
  assert(c1p->completeMatch(c2p.get()));
  assert(c2p->completeMatch(c1p.get()));
  assert(c1pp->completeMatch(c2pp.get()));
  assert(c2pp->completeMatch(c1pp.get()));
}

static void test10() {
  printf("test10\n");
  Context ctx;
  Context* context = &ctx;

  // Construct two C types that refer to another C type
  // by adjusting fields after construction.
  // 1: c -> c' -> c'' -> c
  // 2: c -> c' -> c
  auto iType = IntType::get(context, 0);
  auto c1 = makeC(context, iType);
  auto c1p = makeC(context, iType);
  auto c1pp = makeC(context, iType);

  c1->setFieldType(0, QualifiedType(QualifiedType::VAR, c1p.get()));
  c1->finalizeFieldTypes();
  c1p->setFieldType(0, QualifiedType(QualifiedType::VAR, c1pp.get()));
  c1p->finalizeFieldTypes();
  c1pp->setFieldType(0, QualifiedType(QualifiedType::VAR, c1.get()));
  c1pp->finalizeFieldTypes();

  auto c2 = makeC(context, iType);
  auto c2p = makeC(context, iType);

  c2->setFieldType(0, QualifiedType(QualifiedType::VAR, c2p.get()));
  c2->finalizeFieldTypes();
  c2p->setFieldType(0, QualifiedType(QualifiedType::VAR, c2.get()));
  c2p->finalizeFieldTypes();

  // Check to see if these two match
  assert(!c1->completeMatch(c2.get()));
  assert(!c2->completeMatch(c1.get()));
  assert(!c1p->completeMatch(c2p.get()));
  assert(!c2p->completeMatch(c1p.get()));
}

static void test11() {
  printf("test11\n");
  Context ctx;
  Context* context = &ctx;

  // Construct two C types that refer to another C type
  // by adjusting fields after construction.
  // 1: d -> c -> c
  //      -> c' -> c'
  //      -> c -> c
  // 2: d -> c -> c
  //      -> c' -> c'
  //      -> c' -> c'
  auto iType = IntType::get(context, 0);
  auto c1 = makeC(context, iType);
  auto c1p = makeC(context, iType);
  auto d1 = makeD(context, iType, iType, iType);

  c1->setFieldType(0, QualifiedType(QualifiedType::VAR, c1.get()));
  c1->finalizeFieldTypes();
  c1p->setFieldType(0, QualifiedType(QualifiedType::VAR, c1p.get()));
  c1p->finalizeFieldTypes();
  d1->setFieldType(0, QualifiedType(QualifiedType::VAR, c1.get()));
  d1->setFieldType(1, QualifiedType(QualifiedType::VAR, c1p.get()));
  d1->setFieldType(2, QualifiedType(QualifiedType::VAR, c1.get()));
  d1->finalizeFieldTypes();

  auto c2 = makeC(context, iType);
  auto c2p = makeC(context, iType);
  auto d2 = makeD(context, iType, iType, iType);

  c2->setFieldType(0, QualifiedType(QualifiedType::VAR, c2.get()));
  c2->finalizeFieldTypes();
  c2p->setFieldType(0, QualifiedType(QualifiedType::VAR, c2p.get()));
  c2p->finalizeFieldTypes();
  d2->setFieldType(0, QualifiedType(QualifiedType::VAR, c2.get()));
  d2->setFieldType(1, QualifiedType(QualifiedType::VAR, c2p.get()));
  d2->setFieldType(2, QualifiedType(QualifiedType::VAR, c2p.get()));
  d2->finalizeFieldTypes();

  // Check to see if these two match
  assert(!d1->completeMatch(d2.get()));
  assert(!d2->completeMatch(d1.get()));
}

static void test12() {
  printf("test12\n");
  Context ctx;
  Context* context = &ctx;

  // Construct two C types that refer to another C type
  // by adjusting fields after construction.
  // 1: d -> c -> c
  //      -> c' -> c'
  //      -> c -> c
  // 2: d -> c -> c
  //      -> c' -> c
  //      -> c' -> c
  auto iType = IntType::get(context, 0);
  auto c1 = makeC(context, iType);
  auto c1p = makeC(context, iType);
  auto d1 = makeD(context, iType, iType, iType);

  c1->setFieldType(0, QualifiedType(QualifiedType::VAR, c1.get()));
  c1->finalizeFieldTypes();
  c1p->setFieldType(0, QualifiedType(QualifiedType::VAR, c1p.get()));
  c1p->finalizeFieldTypes();
  d1->setFieldType(0, QualifiedType(QualifiedType::VAR, c1.get()));
  d1->setFieldType(1, QualifiedType(QualifiedType::VAR, c1p.get()));
  d1->setFieldType(2, QualifiedType(QualifiedType::VAR, c1.get()));
  d1->finalizeFieldTypes();

  auto c2 = makeC(context, iType);
  auto c2p = makeC(context, iType);
  auto d2 = makeD(context, iType, iType, iType);

  c2->setFieldType(0, QualifiedType(QualifiedType::VAR, c2.get()));
  c2->finalizeFieldTypes();
  c2p->setFieldType(0, QualifiedType(QualifiedType::VAR, c2.get()));
  c2p->finalizeFieldTypes();
  d2->setFieldType(0, QualifiedType(QualifiedType::VAR, c2.get()));
  d2->setFieldType(1, QualifiedType(QualifiedType::VAR, c2p.get()));
  d2->setFieldType(2, QualifiedType(QualifiedType::VAR, c2p.get()));
  d2->finalizeFieldTypes();

  // Check to see if these two match
  assert(!d1->completeMatch(d2.get()));
  assert(!d2->completeMatch(d1.get()));
}

static void test13() {
  printf("test13\n");
  Context ctx;
  Context* context = &ctx;

  // Construct two C types that refer to another C type
  // by adjusting fields after construction.
  // 1: d -> c -> c' -> c
  //      -> c' -> c
  //      -> c -> c
  // 2: d -> c -> c' -> c
  //      -> c' -> c
  //      -> c' -> c
  auto iType = IntType::get(context, 0);
  auto c1 = makeC(context, iType);
  auto c1p = makeC(context, iType);
  auto d1 = makeD(context, iType, iType, iType);

  c1->setFieldType(0, QualifiedType(QualifiedType::VAR, c1p.get()));
  c1->finalizeFieldTypes();
  c1p->setFieldType(0, QualifiedType(QualifiedType::VAR, c1.get()));
  c1p->finalizeFieldTypes();
  d1->setFieldType(0, QualifiedType(QualifiedType::VAR, c1.get()));
  d1->setFieldType(1, QualifiedType(QualifiedType::VAR, c1p.get()));
  d1->setFieldType(2, QualifiedType(QualifiedType::VAR, c1.get()));
  d1->finalizeFieldTypes();

  auto c2 = makeC(context, iType);
  auto c2p = makeC(context, iType);
  auto d2 = makeD(context, iType, iType, iType);

  c2->setFieldType(0, QualifiedType(QualifiedType::VAR, c2p.get()));
  c2->finalizeFieldTypes();
  c2p->setFieldType(0, QualifiedType(QualifiedType::VAR, c2.get()));
  c2p->finalizeFieldTypes();
  d2->setFieldType(0, QualifiedType(QualifiedType::VAR, c2.get()));
  d2->setFieldType(1, QualifiedType(QualifiedType::VAR, c2p.get()));
  d2->setFieldType(2, QualifiedType(QualifiedType::VAR, c2p.get()));
  d2->finalizeFieldTypes();

  // Check to see if these two match
  assert(!d1->completeMatch(d2.get()));
  assert(!d2->completeMatch(d1.get()));
}


int main(int argc, char** argv) {

  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  test10();
  test11();
  test12();
  test13();

  return 0;
}
