/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"

// an initial example
static void test1() {
  printf("test1\n");

  Context* context = buildStdContext();
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      record Inner {
        var i: int;
        proc ref addOne() {
          i += 1;
          return 1.0;
        }
      }
      record Outer {
        forwarding var impl: Inner;
      }

      var rec: Outer;
      var x = rec.addOne();
    }
    )"""";

  auto qt = resolveQualifiedTypeOfX(context, contents);
  assert(qt.type()->isRealType());
}

// forwarding -> forwarding
static void test2() {
  printf("test2\n");

  auto context = buildStdContext();
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      record Inner {
        var i: int;
        proc ref addOne() {
          i += 1;
          return 1.0;
        }
      }
      record Middle {
        forwarding var impl: Inner;
      }
      record Outer {
        forwarding var impl: Middle;
      }

      var rec: Outer;
      var x = rec.addOne();
    }
    )"""";

  auto qt = resolveQualifiedTypeOfX(context, contents);
  assert(qt.type()->isRealType());
}

// error for cycle of forwarding
static void test3() {
  printf("test3\n");

  auto context = buildStdContext();
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      class C {
        forwarding var impl: unmanaged C;
      }

      var rec: borrowed C = new unmanaged C();
      var x = rec.addOne();
    }
    )"""";

  auto qt = resolveQualifiedTypeOfX(context, contents);

  size_t errCount = guard.realizeErrors(); // expecting no candidates error
  assert(errCount > 0);

  assert(qt.type()->isErroneousType());
}

// forwarding statement that isn't forwarding var
static void test4() {
  printf("test4\n");

  auto context = buildStdContext();
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      record Inner {
        var i: int;
        proc init() { this.i = 0; };
        proc ref addOne() {
          i += 1;
          return 1.0;
        }
      }
      proc foo() { var x: Inner; return x; }
      record Outer {
        forwarding foo();
      }

      var rec: Outer;
      var x = rec.addOne();
    }
    )"""";

  auto qt = resolveQualifiedTypeOfX(context, contents);
  assert(qt.type()->isRealType());
}

// two forwarding statements
static void test5a() {
  printf("test5a\n");

  auto context = buildStdContext();
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      record Inner1 {
        var i: int;
        proc init() { this.i = 0; };
        proc ref addOne() {
          i += 1;
          return 1.0;
        }
      }
      record Inner2 {
        proc init() { }
      }
      proc foo() { var x: Inner1; return x; }
      proc bar() { var x: Inner2; return x; }
      record Outer {
        forwarding foo();
        forwarding bar();
      }

      var rec: Outer;
      var x = rec.addOne();
    }
    )"""";

  auto qt = resolveQualifiedTypeOfX(context, contents);
  assert(qt.type()->isRealType());
}
// same but expecting ambiguity
static void test5b() {
  printf("test5b\n");

  auto context = buildStdContext();
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      record Inner1 {
        var i: int;
        proc init() { this.i = 0; };
        proc ref addOne() {
          i += 1;
          return 1.0;
        }
      }
      record Inner2 {
        var i: int;
        proc init() { this.i = 0; };
        proc ref addOne() {
          i += 1;
          return 1.0;
        }
      }
      proc foo() { var x: Inner1; return x; }
      proc bar() { var x: Inner2; return x; }
      record Outer {
        forwarding foo();
        forwarding bar();
      }

      var rec: Outer;
      var x = rec.addOne();
    }
    )"""";

  auto qt = resolveQualifiedTypeOfX(context, contents);

  size_t errCount = guard.realizeErrors(); // expecting ambiguity error
  assert(errCount > 0);

  assert(qt.type()->isErroneousType());
}

// two forwarding statements -> two forwarding statements
static void test6a() {
  printf("test6a\n");

  auto context = buildStdContext();
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      record Inner1 {
        var i: int;
        proc init() { this.i = 0; };
        proc ref addOne() {
          i += 1;
          return 1.0;
        }
      }
      record Inner2 {
        proc init() { }
      }
      record Inner3 {
        proc init() { }
      }
      record Inner4 {
        proc init() { }
      }

      record Middle1 {
        forwarding var field: Inner1;
        forwarding var field: Inner2;
      }
      record Middle2 {
        forwarding var field: Inner3;
        forwarding var field: Inner4;
      }

      record Outer {
        forwarding var impl: Middle1;
        forwarding var impl: Middle2;
      }

      var rec: Outer;
      var x = rec.addOne();
    }
    )"""";

  auto qt = resolveQualifiedTypeOfX(context, contents);
  assert(qt.type()->isRealType());
}
// same, but expecting ambiguity error
static void test6b() {
  printf("test6b\n");

  auto context = buildStdContext();
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      record Inner1 {
        var i: int;
        proc init() { this.i = 0; };
        proc ref addOne() {
          i += 1;
          return 1.0;
        }
      }
      record Inner2 {
        proc init() { }
      }
      record Inner3 {
        proc init() { }
      }
      record Inner4 {
        var i: int;
        proc init() { this.i = 0; };
        proc ref addOne() {
          i += 1;
          return 1.0;
        }
      }

      record Middle1 {
        forwarding var field: Inner1;
        forwarding var field: Inner2;
      }
      record Middle2 {
        forwarding var field: Inner3;
        forwarding var field: Inner4;
      }

      record Outer {
        forwarding var impl: Middle1;
        forwarding var impl: Middle2;
      }

      var rec: Outer;
      var x = rec.addOne();
    }
    )"""";

  auto qt = resolveQualifiedTypeOfX(context, contents);

  size_t errCount = guard.realizeErrors(); // expecting ambiguity error
  assert(errCount > 0);

  assert(qt.type()->isErroneousType());
}

// Test forwarding to an expression's result
static void testExpr() {
  printf("testExpr\n");

  auto context = buildStdContext();
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    record X {
      proc foo() {
        return 5;
      }
    }

    record R {
      var x : X;

      proc _value const ref {
        return x;
      }

      forwarding _value;
    }

    var r : R;
    var x = r.foo();
    )"""";

  auto qt = resolveQualifiedTypeOfX(context, contents);
  assert(qt.type()->isIntType());
}

static void forwardForwardHelper(std::string stmt, bool isVar = false) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string contents =
    R""""(
    record A {
      proc foo() {
        return 5;
      }
    }

    record B {
      var a : A;

      proc bar() {
        return a;
      }
    }

    proc ident(arg) { return arg; }

    record R {
      var b : B;

      forwarding b;
      )"""" + stmt + R""""(
    }

    var r : R;
    var x = r.foo();
    )"""";

  auto qt = resolveQualifiedTypeOfX(context, contents);
  assert(qt.type()->isErroneousType());

  unsigned int numExpected = isVar ? 3 : 2;
  assert(guard.numErrors() == numExpected);

  assert(guard.error(0)->type() == chpl::NoMatchingCandidates);

  if (isVar) {
    // TODO: Why the extra message in the var case?
    assert(guard.error(1)->type() == chpl::NoMatchingCandidates);
  }

  assert(guard.error(guard.numErrors()-1)->type() == chpl::NoMatchingCandidates);

  guard.realizeErrors();
}

// Test an attempt to forward to an expression that itself requires forwarding
static void testForwardForwardExpr() {
  printf("testForwardForwardExpr\n");

  forwardForwardHelper("forwarding bar();");
  forwardForwardHelper("forwarding ident(bar());");
  forwardForwardHelper("forwarding var x = bar();", /*isVar=*/true);
}

// Two different levels of forwarding->forwarding
static void test7() {
  printf("test7\n");

  auto context = buildStdContext();
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      record Inner {
        var i: int;
        pragma "last resort"
        proc ref addOne() {
          i += 1;
          return 1.0;
        }
      }
      record Middle {
        forwarding var impl: Inner;
      }
      record Outer {
        forwarding var impl: Middle;
        forwarding var anotherImpl: Inner;
      }

      var rec: Outer;
      var x = rec.addOne();
    }
    )"""";

  auto qt = resolveQualifiedTypeOfX(context, contents);
  assert(qt.type()->isRealType());

  guard.realizeErrors();
}

// Test that 'except' clause doesn't exclude other symbols.
// Doesn't test 'except' correct exclusion functionality.
static void test8() {
  printf("test8\n");

  auto context = buildStdContext();
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      record Foo {
        var _instance : owned Bar;

        proc init(value) {
          this._instance = value;
        }

        proc arbitrarySymbol do return 4;
        forwarding _instance except arbitrarySymbol;
      }

      class Bar {
        var asdf : int;
      }

      var myFoo = new Foo(new unmanaged Bar());
      var x = myFoo.asdf;
    }
    )"""";

  auto qt = resolveQualifiedTypeOfX(context, contents);
  assert(qt.type()->isIntType());

  guard.realizeErrors();
}


int main() {
  test1();
  test2();
  test3();
  test4();
  test5a();
  test5b();
  test6a();
  test6b();

  testExpr();
  testForwardForwardExpr();

  test7();

  // TODO: forwarding with only, except

  test8();

  return 0;
}

