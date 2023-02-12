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
#include "chpl/uast/Block.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Include.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"

// helper functions

/* Check that, when resolving (or scope resolving) program,
   identId within methodId resolves to refer to to fieldId.
   */
static void testIt(const char* testName,
                   const char* program,
                   const char* methodIdStr,
                   const char* identIdStr,
                   const char* fieldIdStr,
                   bool scopeResolveOnly=false) {
  printf("test %s\n", testName);
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, testName);
  std::string contents = program;
  setFileText(context, path, contents);
  // parse it so that Context knowns about the IDs
  const ModuleVec& vec = parseToplevel(context, path);

  for (auto m : vec) {
    m->dump();
  }

  ID methodId = ID::fromString(context, methodIdStr);
  ID identId = ID::fromString(context, identIdStr);
  ID fieldId = ID::fromString(context, fieldIdStr);

  auto methodAst = parsing::idToAst(context, methodId);
  assert(methodAst);
  assert(methodAst->isFunction());
  auto identAst = parsing::idToAst(context, identId);
  assert(identAst);
  assert(identAst->isIdentifier());
  auto fieldAst = parsing::idToAst(context, fieldId);
  assert(fieldAst);
  assert(fieldAst->isVariable() || fieldAst->isFormal() ||
         fieldAst->isAggregateDecl());

  // check the scope resolver
  const ResolvedFunction* sr = scopeResolveFunction(context, methodId);
  assert(sr != nullptr);

  assert(sr->byId(identAst->id()).toId() == fieldAst->id());

  // check the full resolver
  if (!scopeResolveOnly) {
    const ResolvedFunction* r = resolveConcreteFunction(context, methodId);
    assert(r != nullptr);

    assert(r->byId(identAst->id()).toId() == fieldAst->id());
  }
}

static void test1r() {
  testIt("test1r.chpl",
         R""""(
            module M {
              record R {
                var field: int;
                proc primary() { return field; }
              }
            }
         )"""",
         "M.R.primary",
         "M.R.primary@1",
         "M.R@1");
}
static void test1c() {
  testIt("test1c.chpl",
         R""""(
            module M {
              class C {
                var field: int;
                proc primary() { return field; }
              }
            }
         )"""",
         "M.C.primary",
         "M.C.primary@1",
         "M.C@1");
}


static void test2r() {
  testIt("test2r.chpl",
         R""""(
            module M {
              record R {
                var field: int;
              }
              proc R.secondary() { return field; }
            }
         )"""",
         "M.secondary",
         "M.secondary@2",
         "M.R@1");
}
static void test2c() {
  testIt("test2c.chpl",
         R""""(
            module M {
              class C {
                var field: int;
              }
              proc C.secondary() { return field; }
            }
         )"""",
         "M.secondary",
         "M.secondary@2",
         "M.C@1");
}


static void test3r() {
  testIt("test3r.chpl",
         R""""(
            module M {
              record R {
                var field: int;
              }
            }
            module N {
              import M.{R};
              proc R.tertiary() { return field; }
            }
         )"""",
         "N.tertiary",
         "N.tertiary@2",
         "M.R@1");
}
static void test3c() {
  testIt("test3c.chpl",
         R""""(
            module M {
              class C {
                var field: int;
              }
            }
            module N {
              import M.{C};
              proc C.tertiary() { return field; }
            }
         )"""",
         "N.tertiary",
         "N.tertiary@2",
         "M.C@1");
}


// test with class inheritance & a primary method
static void test4p() {
  testIt("test4p.chpl",
         R""""(
            module M {
              class Parent {
                var field: int;
              }
              class C : Parent {
                proc primary() { return field; }
              }
            }
         )"""",
         "M.C.primary",
         "M.C.primary@1",
         "M.Parent@1");
}
static void test4p2() {
  testIt("test4p2.chpl",
         R""""(
            module M {
              class Parent {
                var field: int;
              }
            }
            module N {
              import M.{Parent};
              class C : Parent {
                proc primary() { return field; }
              }
            }
         )"""",
         "N.C.primary",
         "N.C.primary@1",
         "M.Parent@1");
}
// test with class inheritance & a secondary method
static void test4s() {
  testIt("test4s.chpl",
         R""""(
            module M {
              class Parent {
                var field: int;
              }
            }
            module N {
              import M.{Parent};
              class C : Parent {
              }
              proc C.secondary() { return field; }
            }
         )"""",
         "N.secondary",
         "N.secondary@2",
         "M.Parent@1");
}

// try with two inherited parent types
static void test5() {
  testIt("test5.chpl",
         R""""(
            module M {
              class A {
                var field: int;
              }
            }
            module N {
              import M.{A};
              class B : A {
              }
            }
            module O {
              import N.{B};
              class C : B {
                proc primary() { return field; }
              }
            }
         )"""",
         "O.C.primary",
         "O.C.primary@1",
         "M.A@1");
}


// test with a nested class
static void test6() {
  testIt("test6.chpl",
         R""""(
            module M {
              class Base {
                var x: int;
              }
              class Outer {
                var x: int;
                class Nested : Base {
                  proc bar() {
                    x;
                  }
                }
              }
            }
         )"""",
         "M.Outer.Nested.bar",
         "M.Outer.Nested.bar@1",
         "M.Base@1",
         /* scope resolve only to avoid errors today */ true);
  // TODO get the above case working with the full resolver
}

// test with an outer variable vs a parent class field
static void test7() {
  testIt("test7.chpl",
         R""""(
            module M {
              class Base {
                var x: int;
              }

              class C : Base {
              }

              {
                var x: int;

                proc C.secondary() {
                  x;
                }
              }
            }
         )"""",
         "M.secondary",
         "M.secondary@2",
         "M.Base@1",
         /* scope resolve only to avoid errors today */ true);
  // TODO get the above case working with the full resolver
}

// test with a formal vs a parent class field
static void test8() {
  testIt("test8.chpl",
         R""""(
            module M {
              class Base {
                var x: int;
              }

              class C : Base {
              }

              proc C.secondary(x: real) {
                x;
              }
            }
         )"""",
         "M.secondary",
         "M.secondary@4",
         "M.secondary@3" /* the formal */ );
}

// test with a formal vs a class field
// this version uses a primary method
static void test9p() {
  testIt("test9p.chpl",
         R""""(
            module M {
              record R {
                var tz:int;
                proc init() { }
                proc foo(tzinfo: real) { }
                proc now(tz:real = 1.4) {
                  var x:R;
                  x.foo(tz);
                }
              }
            }
         )"""",
         "M.R.now",
         "M.R.now@8",
         "M.R.now@3" /* the formal */ );
}
// same but with a secondary method
static void test9s() {
  testIt("test9s.chpl",
         R""""(
            module M {
              record R {
                var tz:int;
                proc init() { }
                proc foo(tzinfo: real) { }
              }

              proc R.now(tz:real = 1.4) {
                var x:R;
                x.foo(tz);
              }
            }
         )"""",
         "M.now",
         "M.now@9",
         "M.now@4" /* the formal */ );
}


// make sure that we can find nested classes through method visibility rules
// this is inspired by the test 'test_no_name_collision.chpl'.
// this version uses a primary method
static void test10p() {
  testIt("test10p.chpl",
         R""""(
            module M {
              class outerClass {
                class aClass {
                  var a: int;
                }
                proc foo() {
                  aClass;
                }
              }

              class aClass {
                var b: int;
              }
            }
         )"""",
         "M.outerClass.foo",
         "M.outerClass.foo@1",
         "M.outerClass.aClass" /* the nested class */,
         /* scope resolve only to avoid errors today */ true);
  // TODO get the above case working with the full resolver
}
// similar to test10, but with a secondary method
static void test10s() {
  testIt("test10s.chpl",
         R""""(
            module M {
              class outerClass {
                class aClass {
                  var a: int;
                }
              }

              proc outerClass.foo() {
                aClass;
              }

              class aClass {
                var b: int;
              }
            }
         )"""",
         "M.foo",
         "M.foo@2",
         "M.outerClass.aClass" /* the nested class */,
         /* scope resolve only to avoid errors today */ true);
  // TODO get the above case working with the full resolver
}


int main() {
  test1r();
  test1c();
  test2r();
  test2c();
  test3r();
  test3c();

  test4p();
  test4p2();
  test4s();

  test5();

  test6();
  test7();

  test8();

  test9p();
  test9s();

  test10p();
  test10s();

  return 0;
}
