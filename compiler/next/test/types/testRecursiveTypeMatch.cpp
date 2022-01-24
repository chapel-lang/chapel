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
//   class D { var a; var b; }

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
                                   const Type* bType) {
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

  auto ret = toOwned(new BasicClassType(emptyId, name, fields,
                                        /* instantiatedFrom */ nullptr,
                                        CompositeType::SubstitutionsMap()));

  ret->setParentType(BasicClassType::getObjectType(context));
  ret->setFieldType(0, QualifiedType(QualifiedType::VAR, aType));
  ret->setFieldType(1, QualifiedType(QualifiedType::VAR, bType));
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
  auto d1 = makeD(context, IntType::get(context, 0), RealType::get(context, 0));
  auto d2 = makeD(context, RealType::get(context, 0), IntType::get(context, 0));

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




int main(int argc, char** argv) {

  test1();
  test2();
  test3();
  test4();

  return 0;
}
