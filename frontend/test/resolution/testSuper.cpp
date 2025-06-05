/*
 * Copyright 2023-2025 Hewlett Packard Enterprise Development LP
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

static void superTest1() {
  testCall("superTest1.chpl",
         R""""(
            module M {
              class Parent {
                proc primary() { return 1; }
              }
              class C : Parent {
                override proc primary() { return super.primary(); }
              }
            }
         )"""",
         "M.C.primary",
         "M.C.primary@3",
         "M.Parent.primary");
}

static void superTest2() {
  testCall("superTest2.chpl",
         R""""(
            module M {
              class Parent {
                proc primary() { return 1; }
              }
            }
            module N {
              import M.{Parent};
              class C : Parent {
                override proc primary() { return super.primary(); }
              }
            }
         )"""",
         "N.C.primary",
         "N.C.primary@3",
         "M.Parent.primary");
}

static void superTest3() {
  testCall("superTest3.chpl",
         R""""(
            module M {
              class Parent {
                proc primary() { return 1; }
              }
            }
            module N {
              import M.{Parent};
              class C : Parent {
              }
              proc C.secondary() { return super.primary(); }
            }
         )"""",
         "N.secondary",
         "N.secondary@4",
         "M.Parent.primary");
}

static void superTest3a() {
  testCall("superTest3a.chpl",
         R""""(
            module M {
              class Parent {
              }
              proc Parent.secondary() { return 1; }
            }
            module N {
              import M.{Parent};
              class C : Parent {
              }
              proc C.secondary() { return super.secondary(); }
            }
         )"""",
         "N.secondary",
         "N.secondary@4",
         "M.secondary");
}

static void superTest3b() {
  testCall("superTest3b.chpl",
         R""""(
            module M {
              class Parent {
              }
            }
            module N {
              import M.{Parent};
              class C : Parent {
              }
              proc Parent.tertiary() { return 1; }
              proc C.secondary() { return super.tertiary(); }
            }
         )"""",
         "N.secondary",
         "N.secondary@4",
         "N.tertiary");
}

static void superTest4() {
  testCall("superTest4.chpl",
         R""""(
            module M {
              class A {
                proc primary() { return 1; }
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
                proc primary() { return super.primary(); }
              }
            }
         )"""",
         "O.C.primary",
         "O.C.primary@3",
         "M.A.primary");
}

static void superTest5() {
  testCall("superTest5.chpl",
         R""""(
            module M {
              class Base {
                proc bar() { return 0; }
              }
              class Outer {
                var x: int;
                class Nested : Base {
                  proc bar() {
                    return super.bar();
                  }
                }
              }
            }
         )"""",
         "M.Outer.Nested.bar",
         "M.Outer.Nested.bar@3",
         "M.Base.bar");
}

static void superTest6() {
  testCall("superTest6.chpl",
         R""""(
            module M {
              class Parent {
                proc bar() {
                  return 1;
                }
              }

              class Other {
                proc bar() {
                  return 1;
                }
              }

              class Child : Parent {
                proc foo() {
                  return super.bar();
                }
              }
            }
         )"""",
         "M.Child.foo",
         "M.Child.foo@3",
         "M.Parent.bar");
}

int main() {
  superTest1();
  superTest2();
  superTest3();
  superTest3a();
  superTest3b();
  superTest4();
  superTest5();
  superTest6();
}
