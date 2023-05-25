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
  const auto& sre = sr->byId(identAst->id());
  printf("scope resolved to %s\n", sre.toId().str().c_str());
  assert(sre.toId() == fieldAst->id());

  // check the full resolver
  if (!scopeResolveOnly) {
    const ResolvedFunction* r = resolveConcreteFunction(context, methodId);
    assert(r != nullptr);
    const auto& re = r->byId(identAst->id());
    printf("full resolved to %s\n", re.toId().str().c_str());
    assert(re.toId() == fieldAst->id());
  }
  printf("\n");
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
         "M.Base@1");
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
         "M.Base@1");
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
         "M.outerClass.aClass" /* the nested class */);
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
         "M.outerClass.aClass" /* the nested class */);
}

// test with parent scopes that should find a local not a field
static void test11p() {
  testIt("test11p.chpl",
         R""""(
            module M {
              record rec {
                var x: int;
                proc foo() {
                  var x: real;
                  {
                    var foo: string;
                    {
                      var bar: string;
                      {
                        x;
                      }
                    }
                  }
                }
              }
            }
         )"""",
         "M.rec.foo",
         "M.rec.foo@7",
         "M.rec.foo@2" /* the local variable */);
}
static void test11s() {
  testIt("test11s.chpl",
         R""""(
            module M {
              record rec {
                var x: int;
              }
              proc rec.foo() {
                var x: real;
                {
                  var foo: string;
                  {
                    var bar: string;
                    {
                      x;
                    }
                  }
                }
              }
            }
         )"""",
         "M.foo",
         "M.foo@8",
         "M.foo@3" /* the local variable */);
}

// same as above but with a formal rather than a local variable
static void test12p() {
  testIt("test12p.chpl",
         R""""(
            module M {
              record rec {
                var x: int;
                proc foo(x: int) {
                  var y: real;
                  {
                    var foo: string;
                    {
                      var bar: string;
                      {
                        x;
                      }
                    }
                  }
                }
              }
            }
         )"""",
         "M.rec.foo",
         "M.rec.foo@9",
         "M.rec.foo@2" /* the formal */);
}
static void test12s() {
  testIt("test12s.chpl",
         R""""(
            module M {
              record rec {
                var x: int;
              }
              proc rec.foo(x: int) {
                var y: real;
                {
                  var foo: string;
                  {
                    var bar: string;
                    {
                      x;
                    }
                  }
                }
              }
            }
         )"""",
         "M.foo",
         "M.foo@10",
         "M.foo@3" /* the formal */);
}

// field access vs parent module field
static void test13p() {
  testIt("test13p.chpl",
         R""""(
              module M {
                record mat {
                  var m, n: int;
                  proc foo() {
                    var b = 1;
                    {
                      var c = 2;
                      n;
                    }
                  }
                }

                var n: real;
              }
         )"""",
         "M.mat.foo",
         "M.mat.foo@5",
         "M.mat@2" /* the field */);
}
static void test13s() {
  testIt("test13s.chpl",
         R""""(
            module M {
              record mat {
                var m, n: int;
              }

              proc mat.foo() {
                var b = 1;
                {
                  var c = 2;
                  n;
                }
              }

              var n: real;
            }
         )"""",
         "M.foo",
         "M.foo@6",
         "M.mat@2" /* the field */);
}

// The following series of tests is from issue #21668
static void testExample2() {
  testCall("example2.chpl",
           R""""(
              module M {
                record R { }
                proc R.bar(arg: real) { }
                proc bar(arg: int) { }
                proc R.test() {
                  var x = 32;
                  bar(x); // does it refer to M.R.bar() or M.bar() ?
                          // 1.29 prefers M.R.bar().
                }
              }
           )"""",
           "M.test",
           "M.test@6",
           "M.bar" /* the selected method */);
  // TODO: perhaps this case should prefer the 'int' version
  // because both should participate in disambiguation.
  // See issue #21668.
}

static void testExample3() {
  testCall("example3.chpl",
           R""""(
              module M {
                record R { }
                proc R.bar(arg: string, arg2: real) { }
                proc bar(arg: int) { }
                proc R.test() {
                  var x = 32;
                  bar(x); // does it refer to M.R.bar() or M.bar ?
                          // 1.29 compiler prefers M.R.bar()
                          // (so it fails to compile)
                }
              }
           )"""",
           "M.test",
           "M.test@6",
           "M.bar#1" /* the applicable non-method function */);
}

static void testExample4() {
  testCall("example4.chpl",
           R""""(
              module M {
                import U;
                record R { }
              }

              module U {
                import M.R;
                proc R.bar(arg: string, arg2: real) { }
              }

              module N {
                import M.R;

                proc bar(arg: int) { }

                proc R.test() {
                  var x = 32;
                  bar(x); // does it refer to U.R.bar() or N.bar ?
                          // 1.29 tries to call this.bar(x)
                          // (and so fails to compile)
                }
              }
           )"""",
           "N.test",
           "N.test@6",
           "N.bar" /* the applicable non-method function */);
}

static void testExample4a() {
  testCall("example4a.chpl",
           R""""(
              module M {
                public use U;
                record R { }
              }

              module U {
                import M.R;
                proc R.bar(arg: int) { }
              }

              module N {
                import M.R;

                proc R.test() {
                  var x = 32;
                  bar(x); // is this a legal way to call U.R.bar()?
                          // it does work in 1.29 compiler
                }
              }
           )"""",
           "N.test",
           "N.test@6",
           "U.bar" /* find U.R.bar through M's public use U */);
}

static void testExample5() {
  testCall("example5.chpl",
           R""""(
              // Example 5
              module A {
                record r { }
                proc r.foo { }
              }
              module B {
                use A;
                var foo: int;

                proc r.test() {
                  foo; // is this A.foo or B.r.foo?
                       // 1.29 calls A.r.foo
                }
              }
           )"""",
           "B.test",
           "B.test@2",
           "A.foo" /* call the method; don't refer to the int */);
}

static void testExample5a() {
  testCall("example5a.chpl",
           R""""(
              module M {
                record r { }
                proc r.test() {
                  var foo: int;
                  proc r.foo { }
                  foo; // is this referring to the int or the parenless method?
                       // 1.29 refers to the local variable
                }
              }
           )"""",
           "M.test",
           "M.test@4",
           "" /* ambiguity */);
}

static void testExample6() {
  testCall("example6.chpl",
           R""""(
              module A {
                record r { }
                proc r.foo { }
              }
              module B {
                import A.r;

                var foo: int;

                proc r.test() {
                  foo; // is this A.r.foo or B.foo?
                       // 1.29 calls A.r.foo
                }
              }
           )"""",
           "B.test",
           "B.test@2",
           "A.foo" /* the method not the int */);
}

static void testExample7() {
  testCall("example7.chpl",
           R""""(
              module M {
                record recordA { }
                record recordB { var x: int; }
                proc recordA.outer() {
                  var x: real;
                  proc recordB.test() {
                    x; // the field or variable in outer() ?
                       // 1.29 prefers the field
                  }
                }
              }
           )"""",
           "M.outer.test",
           "M.outer.test@2",
           "M.recordB@1" /* the field */);
}

static void testExample8() {
  testCall("example8.chpl",
           R""""(
              module A {
                var foo: int;
                proc main() {
                  record r { }
                  proc r.foo { }
                  proc r.test() {
                    foo; // is this A.main.r.foo or A.foo ?
                         // 1.29 calls A.main.r.foo
                  }
                }
              }
           )"""",
           "A.main.test",
           "A.main.test@2",
           "A.main.foo" /* the method */);
}

static void testExample9() {
  testCall("example9.chpl",
           R""""(
              module A {
                proc main() {
                  record r { }
                  proc r.foo { }
                  var foo: int;
                  proc r.test() {
                    foo; // is this the method or the outer variable?
                         // 1.29 calls A.main.r.foo
                  }
                  var x:r;
                  x.method();
                }
              }
           )"""",
           "A.main.test",
           "A.main.test@2",
           "A.main.foo" /* the method */);
}

static void testExample10() {
  testCall("example10.chpl",
           R""""(
              module A {
                class Parent { }
                proc Parent.foo { }
              }
              module B {
                import A.Parent;

                class Child : Parent { }
              }

              module C {
                import B.Child;

                var foo : int;

                proc Child.test() {
                  foo; // is this the method on Parent or the outer int?
                }
              }
           )"""",
           "C.test",
           "C.test@2",
           "A.foo" /* the method */ );
}

static void testExample11() {
  testCall("example11.chpl",
           R""""(
              module A {
                class Parent { }
                proc Parent.foo { }
              }
              module B {
                import A.Parent;

                class Child : Parent { }
              }

              module C {
                import B.Child;

                override proc Child.foo { }

                proc Child.test() {
                  foo; // is this A's Parent.foo or C's Child.foo?
                       // 1.29 calls Child.foo
                }
              }
           )"""",
           "C.test",
           "C.test@2",
           "C.foo" /* the method Child.foo */ );
}

static void testExample12() {
  testCall("example12.chpl",
           R""""(
              module M {
                record recordA { var x: int; }
                record recordB { }
                proc recordA.outer() {
                  proc recordB.test() {
                    x; // can this resolve to M.recordA.x?
                  }
                }
              }
           )"""",
           "M.outer.test",
           "M.outer.test@2",
           "" /* some sort of error */);
}

static void testExample13() {
  testCall("example13.chpl",
           R""""(
              module A {
                record r { }
                proc r.foo { }
              }
              module B {
                use A;
                proc r.foo { }

                proc r.test() {
                  foo; // is this A.r.foo or B.r.foo?
                       // 1.29 considers it an ambiguity
                }
              }
           )"""",
           "B.test",
           "B.test@2",
           "" /* ambiguity error */);
}

static void testExample14() {
  testCall("example14.chpl",
           R""""(
              module A {
                record r { }
                proc r.foo { }
              }
              module B {
                import A.r;

                proc r.foo { }

                proc r.test() {
                  foo; // is this A.r.foo or B.r.foo?
                       // 1.29 considers it ambiguous
                }
              }
           )"""",
           "B.test",
           "B.test@2",
           "" /* ambiguity error */);
}

static void testExample15() {
  testCall("example15.chpl",
           R""""(
              module M {
                record r { }

                proc r.foo { }

                proc r.test() {
                  proc r.foo { }
                  foo; // is this M.r.tertiary.r.foo or M.r.foo or ambiguity?
                       // 1.29 considers it ambiguous
                }
              }
           )"""",
           "M.test",
           "M.test@2",
           "" /* ambiguity error */);
}

static void testExample16() {
  testCall("example16.chpl",
           R""""(
              module M {
                proc foo { }

                proc main() {
                  proc foo { }
                  foo; // is this M.main.foo or M.foo or ambiguity?
                       // 1.29 chooses M.main.foo.
                       // Similarly, if instead of the nested `foo`,
                       // we had `var foo: int`,
                       // the variable would be preferred.
                }
              }
           )"""",
           "M.main",
           "M.main@0",
           "M.main.foo" /* the innermost proc foo */);
}

static void testExample17() {
  testCall("example17.chpl",
           R""""(
              module A {
                proc foo { }
              }

              module B {
                proc foo { }
              }

              module C {
                use A;
                proc main() {
                  use B;
                  foo; // is this A.foo or B.foo or ambiguity?
                       // 1.29 prefers B.foo
                }
              }
           )"""",
           "C.main",
           "C.main@3",
           "B.foo" /* proc foo in B */);
}

static void testExample18() {
  testCall("example18.chpl",
           R""""(
              module M {
                record r { var x: int; }

                proc main() {
                  proc r.x { return 4; }
                  var rr: r;
                  rr.x;
                  // this does not compile in 1.29
                  // "no candidates found"
                }
              }
           )"""",
           "M.main",
           "M.main@3",
           "" /* ambiguity */);
}

static void testExample19() {
  testCall("example19.chpl",
           R""""(
              module A {
                record r { }
                proc r.foo { }

                proc r.test() {
                  proc foo { }

                  foo; // is this A.r.foo or A.r.test.foo?
                       // 1.29 calls A.r.test.foo
                }
              }
           )"""",
           "A.test",
           "A.test@2",
           "A.test.foo" /* the inner parenless non-method */);
}

static void testExample20() {
  testCall("example20.chpl",
           R""""(
              module A {
                record r { }

                proc r.test() {
                  proc r.foo { }
                  proc foo { }

                  foo; // does it call the method or non-method?
                       // 1.29 calls A.r.method.foo
                       // (the non-method)
                }
              }
           )"""",
           "A.test",
           "A.test@2",
           "" /* ambiguity error */);
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

  test11p();
  test11s();

  test12p();
  test12s();

  test13p();
  test13s();

  testExample2();
  testExample3();
  testExample4();
  testExample4a();
  testExample5();
  testExample5a();
  testExample6();
  testExample7();
  testExample8();
  testExample9();
  testExample10();
  testExample11();
  testExample12();
  testExample13();
  testExample14();
  testExample15();
  testExample16();
  testExample17();
  testExample18();
  testExample19();
  testExample20();

  return 0;
}
