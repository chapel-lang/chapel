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
#include "chpl/types/BasicClassType.h"
#include "chpl/types/ClassType.h"
#include "chpl/types/IntType.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/uast/Class.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test1.chpl");
  std::string contents = R""""(
                           module M {
                             class C { }

                             proc C.method() { }

                             proc function(arg: C) { }
                          }
                        )"""";

  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 3);
  auto c = m->stmt(0)->toClass();
  assert(c);
  auto method = m->stmt(1)->toFunction();
  assert(method);
  auto function = m->stmt(2)->toFunction();
  assert(function);

  auto methodU = UntypedFnSignature::get(context, method);
  auto functionU = UntypedFnSignature::get(context, function);

  ResolutionContext rcval(context);
  auto methodT = typedSignatureInitial(&rcval, methodU);
  auto functionT = typedSignatureInitial(&rcval, functionU);

  auto it = initialTypeForTypeDecl(context, c->id());
  assert(it);
  auto bct = it->getCompositeType()->toBasicClassType();
  assert(bct);

  auto borrowedNonNil = ClassType::get(context, bct, nullptr,
                                       ClassTypeDecorator(
                                         ClassTypeDecorator::BORROWED_NONNIL));
  auto anyNonNil = ClassType::get(context, bct, nullptr,
                                  ClassTypeDecorator(
                                    ClassTypeDecorator::GENERIC_NONNIL));

  assert(methodT->formalName(0) == "this");
  assert(methodT->formalType(0).kind() == QualifiedType::CONST_IN);
  assert(methodT->formalType(0).type() == borrowedNonNil);

  assert(functionT->formalName(0) == "arg");
  assert(functionT->formalType(0).kind() == QualifiedType::DEFAULT_INTENT);
  assert(functionT->formalType(0).type() == anyNonNil);
}

static void test2() {
  printf("test2\n");

  // This test exists to ensure the compiler can correctly infer the type
  // of 'myParent' without resolving the field's initialization expression,
  // which would result in recursion.
  std::string program = R"""(
    var sentinelValue = new MyRecord();

    record MyRecord {
      var inst : unmanaged MyClass?;

      proc init() { }
    }

    class MyClass {
      const myParent : MyRecord = sentinelValue;
    }

    proc main() {
      var x = sentinelValue;
    }
  )""";

  Context ctx;
  auto context = &ctx;
  ErrorGuard guard(context);
  auto m = parseModule(context, program);
  auto results = resolveModule(context, m->id());

  auto var = findVariable(m, "x");
  auto rec = m->stmt(1)->toRecord();
  auto qt = results.byAst(var).type();
  assert(!qt.isErroneousType());
  assert(qt.type()->isRecordType());
  assert(qt.type()->toRecordType()->id() == rec->id());
}

// test that we can resolve the type of multi-decl fields properly when they
// they are later used as a return type of a method
static void test3() {
  printf("test3\n");

  std::string program = R"""(
    class C {
      var a, b : int;
    }

    proc C.getA() {
      return a;
    }

    var foo = new C(40, 2);
    var x = foo.getA();
    )""";
  auto context = buildStdContext();
  ErrorGuard guard(context);
  auto m = parseModule(context, program);
  auto results = resolveModule(context, m->id());
  auto var = findVariable(m, "x");
  auto qt = results.byAst(var).type();
  assert(qt.type()->isIntType());
  assert(qt.type()->toIntType()->bitwidth() == 64);
}

// Test resolving dependently-typed field usage in method body
static void test4() {
  printf("test4\n");

  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program = R"""(
    class Bar {
      param rank : int;
      var myTup : rank*int;

      proc getSomething() {
        return myTup;
      }
    }

    var b = new Bar(2);
    var x = b.getSomething();
    )""";

  auto m = parseModule(context, program);
  auto r = resolveModule(context, m->id());

  auto x = findVariable(m, "x");
  auto qt = r.byAst(x).type();
  assert(qt.type()->isTupleType());
  assert(qt.type()->toTupleType()->numElements() == 2);
  assert(qt.type()->toTupleType()->starType().type());
  assert(qt.type()->toTupleType()->starType().type()->isIntType());
}
// test that we can map _owned to owned
static void test5() {
  printf("test5\n");
  auto config = getConfigWithHome();
  Context ctx(config);
  Context* context = &ctx;
  ErrorGuard guard(context);
  setupModuleSearchPaths(context, false, false, {}, {});

  std::string program = R"""(
    class C { var y : int; }

    proc foo(arg: _owned) {
    }

    proc bar(arg: owned) {
    }

    proc baz(type arg: _owned(C)) {
    }

    proc buz(type arg: owned C) {
    }

    proc type (owned).doSomething(obj: owned) {

    }
    var objA = new C(1);
    owned.doSomething(objA);

    proc type _owned.doNothing(obj: owned) {

    }
    var objB = new C(1);
    owned.doNothing(objB);

    var obj = new C(5);
    var x = obj.borrow();

    var obj2 = new C(42);
    owned.release(obj2);

    var obj3 = new C(10);
    obj3 = nil;

    var obj4 = new C(42);
    foo(obj4);
    bar(obj4);
    baz(obj4.type);
    buz(obj4.type);

    )""";

  auto m = parseModule(context, std::move(program));
  auto results = resolveModule(context, m->id());
  auto var = findVariable(m, "x");
  auto init = var->initExpression();
  assert(init);
  auto qt = results.byAst(init).type();
  auto c = m->stmt(0)->toClass();
  assert(c);
  auto it = initialTypeForTypeDecl(context, c->id());
  auto bct = it->getCompositeType()->toBasicClassType();
  assert(bct);

  auto borrowedNonNil = ClassType::get(context, bct, nullptr,
                                       ClassTypeDecorator(
                                        ClassTypeDecorator::BORROWED_NONNIL));

  assert(qt.type() == borrowedNonNil);
  assert(guard.realizeErrors() == 0);
}

// test that we can map _shared to shared
static void test6() {
  printf("test6\n");
  auto config = getConfigWithHome();
  Context ctx(config);
  Context* context = &ctx;
  ErrorGuard guard(context);
  setupModuleSearchPaths(context, false, false, {}, {});

  std::string program = R"""(
    class C { var y : int; }
    proc foo(arg: _shared) {
    }

    proc bar(arg: shared) {
    }

    proc baz(type arg: _shared(C)) {
    }

    proc buz(type arg: shared C) {
    }

    proc type (shared).doSomething(obj: owned) {

    }
    var objA = new C(1);
    shared.doSomething(objA);

    proc type _shared.doNothing(obj: owned) {

    }
    var objB = new C(1);
    shared.doNothing(objB);

    var obj = new shared C(5);
    var x = obj.borrow();

    var obj2 = new shared C(42);

    var obj3 = new shared C(10);
    obj3 = obj2;

    var obj4 = new shared C(42);
    foo(obj4);
    bar(obj4);
    baz(obj4.type);
    buz(obj4.type);

    )""";

  auto m = parseModule(context, std::move(program));
  auto results = resolveModule(context, m->id());
  auto var = findVariable(m, "x");
  auto init = var->initExpression();
  assert(init);
  auto qt = results.byAst(init).type();
  auto c = m->stmt(0)->toClass();
  assert(c);
  auto it = initialTypeForTypeDecl(context, c->id());
  auto bct = it->getCompositeType()->toBasicClassType();
  assert(bct);

  auto borrowedNonNil = ClassType::get(context, bct, nullptr,
                                       ClassTypeDecorator(
                                        ClassTypeDecorator::BORROWED_NONNIL));
  assert(qt.type() == borrowedNonNil);

  assert(guard.numErrors() == 0);
}

static void test7() {
  printf("test7\n");

  std::string program = R"""(
    class Parent {
    }

    class Child : Parent {
    }

    proc test(type T: Parent) {
      var ret = new unmanaged T();
      return ret;
    }

    var x = test(Child);
  )""";
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto x = resolveQualifiedTypeOfX(context, program);
  auto t = x.type()->toClassType();
  assert(t);
  assert(t->decorator().isUnmanaged());
  assert(t->decorator().isNonNilable());
  assert(t->basicClassType()->name() == "Parent");
}

static void test8() {
  printf("test8\n");

  std::string program = R"""(
    class Parent {
    }

    class Child : Parent {
    }

    proc test(type T: Parent?) {
      var ret = new unmanaged T();
      return ret;
    }

    var x = test(Child);
  )""";
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto filename = UniqueString::get(context, "test8.chpl");
  setFileText(context, filename, program);
  auto m = parse(context, filename, UniqueString())[0];
  auto r = resolveModule(context, m->id());

  auto x = findVariable(m, "x");
  auto& xres = r.byAst(x->initExpression());

  assert(xres.mostSpecific().only());
  assert(!xres.mostSpecific().only().fn()->needsInstantiation());
}

// We treat 'owned C' and the underlying '_owned(C)' pretty much interchangeably.
// However, when performing instantiation, we don't want to naively instantiate
// _owned with owned. Rather, we should convert owned to _owned, and instantiate
// it that way.
static void testInstantiateManagerRecord() {
  printf("testInstantiateOwned\n");

  std::string program = R"""(
    class C {}

    proc _owned.foo() type {
      return this.type;
    }

    proc _shared.foo() type {
      return this.type;
    }


    type x = (new owned C()).foo();
    type y = (new shared C()).foo();
  )""";
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context, program, {"x", "y"});

  auto checkChplT = [context](const QualifiedType qt) {
    assert(!qt.isUnknownOrErroneous());
    assert(qt.type()->isRecordType());

    auto rc = createDummyRC(context);
    auto fields = fieldsForTypeDecl(&rc, qt.type()->toRecordType(), DefaultsPolicy::IGNORE_DEFAULTS);
    bool foundField = false;
    for (int i = 0; i < fields.numFields(); i++) {
      if (fields.fieldName(i) == "chpl_t") {
        foundField = true;
        assert(fields.fieldType(i).type()->isClassType());
        assert(fields.fieldType(i).type()->toClassType()->basicClassType());
        assert(fields.fieldType(i).type()->toClassType()->basicClassType()->name() == "C");
        break;
      }
    }
    assert(foundField);
  };
  checkChplT(vars.at("x"));
  checkChplT(vars.at("y"));
}

// same as testInstantiateManagerRecord, but in the other direction.
static void testInstantiateOutOfManagerRecord() {
  printf("testInstantiateOutOfManagerRecord\n");

  std::string program = R"""(
    class C {}

    proc (owned).foo() type {
      return this.type;
    }

    proc (shared).foo() type {
      return this.type;
    }


    type x = (new _owned(new unmanaged C())).foo();
    type y = (new _shared(new unmanaged C())).foo();
    type z = (new _owned(new unmanaged C?())).foo();
    type w = (new _shared(new unmanaged C?())).foo();
  )""";
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context, program, {"x", "y", "z", "w"});

  auto checkChplT = [](const QualifiedType qt, const Type* expectManager, bool expectNil) {
    assert(!qt.isUnknownOrErroneous());
    assert(qt.type()->isClassType());

    auto ct = qt.type()->toClassType();
    assert(ct->decorator().isManaged());
    assert(expectNil ? ct->decorator().isNilable() : ct->decorator().isNonNilable());
    assert(ct->manager() == expectManager);
    assert(ct->basicClassType());
    assert(ct->basicClassType()->name() == "C");
  };

  auto owned = AnyOwnedType::get(context);
  auto shared = AnySharedType::get(context);
  checkChplT(vars.at("x"), owned, false);
  checkChplT(vars.at("y"), shared, false);
  checkChplT(vars.at("z"), owned, true);
  checkChplT(vars.at("w"), shared, true);
}

// a trickier case is a 'borrowed' formal and an '_owned' actual. So, the
// actual needs to become 'owned', and then get borrowed.
static void testBorrowManagerRecord() {
  printf("testBorrowManagerRecord\n");

  std::string program = R"""(
    class C {}

    proc (borrowed).foo() type {
      return this.type;
    }

    type x = (new _owned(new unmanaged C())).foo();
    type y = (new _shared(new unmanaged C())).foo();
  )""";
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context, program, {"x", "y"});

  auto checkChplT = [](const QualifiedType qt) {
    assert(!qt.isUnknownOrErroneous());
    assert(qt.type()->isClassType());

    auto ct = qt.type()->toClassType();
    assert(ct->decorator().isBorrowed());
    assert(ct->basicClassType());
    assert(ct->basicClassType()->name() == "C");
  };

  checkChplT(vars.at("x"));
  checkChplT(vars.at("y"));

}

static void testInstantiateParentClass() {
  printf("testInstantiateParentClass\n");

  // test that there's no ambiguity between a parent method and a child
  // method (ensuring parent doesn't become child as part of instantiation).
  {
    std::string program = R"""(
      class parentCls {
        param rank: int;
      }


      proc parentCls.foo(): int {}

      class childCls : parentCls(?) {
      }

      override proc childCls.foo(): int {}

      var c = new unmanaged childCls(1);
      var x = c.foo();
    )""";
    auto context = buildStdContext();
    ErrorGuard guard(context);

    auto vars = resolveTypesOfVariables(context, program, {"x"});
    auto qt = vars.at("x");
    assert(!qt.isUnknownOrErroneous());
    assert(qt.type()->isIntType());
  }

  {
    std::string program = R"""(
      class parentCls {
        param rank: int;
      }

      class childCls : parentCls(?) {
        param otherThing: int;
      }

      proc parentCls.foo() type do return this.type;

      var c = new unmanaged childCls(1, 2);
      param x = c.foo() : string;
    )""";
    auto context = buildStdContext();
    ErrorGuard guard(context);

    auto vars = resolveTypesOfVariables(context, program, {"x"});
    auto qt = vars.at("x");
    assert(!qt.isUnknownOrErroneous());
    ensureParamString(qt, "borrowed parentCls(1)");
  }
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

  testInstantiateManagerRecord();
  testInstantiateOutOfManagerRecord();
  testBorrowManagerRecord();
  testInstantiateParentClass();

  return 0;
}
