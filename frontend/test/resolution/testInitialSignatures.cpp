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
#include "chpl/resolution/resolution-queries.h"

template <typename F>
static void testInitialFormalOfF(const char* prog, int formalIdx, F&& predicate) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto mod = parseModule(context, prog);
  const Function* fn = nullptr;
  for (auto decl : mod->children()) {
    if (auto f = decl->toFunction()) {
      if (f->name() == "f") {
        fn = f;
        break;
      }
    }
  }

  assert(fn != nullptr);
  auto rc = createDummyRC(context);
  auto initial = typedSignatureInitialForId(&rc, fn->id());
  assert(formalIdx < initial->numFormals());
  predicate(context, initial->formalType(formalIdx));
}

// "numeric" is well-known and won't change with instantiation, so initial
// signature should compute 'id' etc.
static void test1() {
  testInitialFormalOfF(R""""(
    proc id(type t) type do return t;
    proc f(x: id(numeric)) {}
  )"""", 0, [](Context* context, const QualifiedType& qt) {
    assert(qt.kind() == QualifiedType::DEFAULT_INTENT);
    assert(qt.type()->isAnyNumericType());
  });
}

// 'class' is well-known and won't change with instantiation, so initial
// signature should compute 'class?' etc. Also, 'class?' should work for
// other reasons (it's established syntax).
static void test2() {
  testInitialFormalOfF(R""""(
    proc f(x: class?) {}
  )"""", 0, [](Context* context, const QualifiedType& qt) {
    assert(qt.kind() == QualifiedType::DEFAULT_INTENT);
    assert(qt.type()->isClassType());
    auto ct = qt.type()->toClassType();
    assert(ct->decorator().isNilable());
    assert(ct->manageableType()->isAnyClassType());
  });
}

// same as test2, but nest two calls ('owned(?(class))')
static void test3() {
  testInitialFormalOfF(R""""(
    proc f(x: owned class?) {}
  )"""", 0, [](Context* context, const QualifiedType& qt) {
    assert(qt.type()->isClassType());
    auto ct = qt.type()->toClassType();
    assert(ct->decorator().isNilable());
    assert(ct->decorator().isManaged());
    assert(ct->manageableType()->isAnyClassType());
    assert(ct->manager() == AnyOwnedType::get(context));
  });
}

// type alias variables are not affected by susbtitutions and so won't change.
// initial signature should compute 'id(alias)' etc.
static void test4() {
  testInitialFormalOfF(R""""(
    type alias = numeric;
    proc id(type t) type do return t;
    proc f(x: id(alias)) {}
  )"""", 0, [](Context* context, const QualifiedType& qt) {
    assert(qt.kind() == QualifiedType::DEFAULT_INTENT);
    assert(qt.type()->isAnyNumericType());
  });
}

// type formals may be instantiated later, so initial signature cannot
// compute 'id(x)' etc. It must remain unknown.
static void test5() {
  testInitialFormalOfF(R""""(
    proc id(type t) type do return t;
    proc f(type x: numeric, y: id(x)) {}
  )"""", 1, [](Context* context, const QualifiedType& qt) {
    assert(qt.isUnknown());
  });
}

// same as test5, but make 'x' a value formal
static void test6() {
  testInitialFormalOfF(R""""(
    proc id(type t) type do return t;
    proc f(x: numeric, y: id(x.type)) {}
  )"""", 1, [](Context* context, const QualifiedType& qt) {
    assert(qt.isUnknown());
  });
}

// same as test1, but instead of a function, use a type constructor
static void test7() {
  testInitialFormalOfF(R""""(
    record wrapper { type wraps; }
    proc f(x: wrapper(numeric)) {}
  )"""", 0, [](Context* context, const QualifiedType& qt) {
    assert(qt.type()->isRecordType());
    ensureSubs(context, qt.type()->toRecordType(), {
      {"wraps", QualifiedType(QualifiedType::TYPE,AnyNumericType::get(context))}
    });
  });
}

// same as test4, but instead of a function, use a type constructor
static void test8() {
  testInitialFormalOfF(R""""(
    type alias = numeric;
    record wrapper { type wraps; }
    proc f(x: wrapper(alias)) {}
  )"""", 0, [](Context* context, const QualifiedType& qt) {
    assert(qt.type()->isRecordType());
    ensureSubs(context, qt.type()->toRecordType(), {
      {"wraps", QualifiedType(QualifiedType::TYPE,AnyNumericType::get(context))}
    });
  });
}

// same as test5, but instead of a function, use a type constructor
static void test9() {
  testInitialFormalOfF(R""""(
    record wrapper { type wraps; }
    proc f(type x: numeric, y: wrapper(x)) {}
  )"""", 1, [](Context* context, const QualifiedType& qt) {
    assert(qt.isUnknown());
  });
}

// same as test6, but instead of a function, use a type constructor
static void test10() {
  testInitialFormalOfF(R""""(
    record wrapper { type wraps; }
    proc f(x: numeric, y: wrapper(x.type)) {}
  )"""", 1, [](Context* context, const QualifiedType& qt) {
    assert(qt.isUnknown());
  });
}

// as a special case, we compute partial tuple types to get more info where possible
static void test11() {
  testInitialFormalOfF(R""""(
    proc f(type x, type y, z: (x, y)) {}
  )"""", 2, [](Context* context, const QualifiedType& qt) {
    assert(qt.type()->isTupleType());
    auto tt = qt.type()->toTupleType();
    assert(tt->numElements() == 2);
    assert(tt->elementType(0).type()->isAnyType());
    assert(tt->elementType(1).type()->isAnyType());
  });
}

// same as test11, but with nested tuples
static void test12() {
  testInitialFormalOfF(R""""(
    proc f(type x, type y, z: (x, (y,))) {}
  )"""", 2, [](Context* context, const QualifiedType& qt) {
    assert(qt.type()->isTupleType());
    auto tt = qt.type()->toTupleType();
    assert(tt->numElements() == 2);
    assert(tt->elementType(0).type()->isAnyType());
    assert(tt->elementType(1).type()->isTupleType());
    auto inner = tt->elementType(1).type()->toTupleType();
    assert(inner->numElements() == 1);
    assert(inner->elementType(0).type()->isAnyType());
  });
}

// even though we compute partial types, don't poison other, less well-behaved
// things.
static void test13() {
  testInitialFormalOfF(R""""(
    proc f(type x, type y, z: [1..10] (x, (y,))) {}
  )"""", 2, [](Context* context, const QualifiedType& qt) {
    assert(qt.type()->isArrayType());
    auto at = qt.type()->toArrayType();
    assert(at->isUninstancedArray());
  });
}

static void test14() {
  testInitialFormalOfF(R""""(
    proc id(type t) type do return t;
    proc f(type x, type y, z: id((x, y))) {}
  )"""", 2, [](Context* context, const QualifiedType& qt) {
    assert(qt.isUnknown());
  });
}

int main() {
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
  test14();
}
