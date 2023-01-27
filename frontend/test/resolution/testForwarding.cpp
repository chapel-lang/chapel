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
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"

// an initial example
static void test1() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      operator +=(ref lhs: int, rhs: int) { }
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
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      operator +=(ref lhs: int, rhs: int) { }
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
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      operator +=(ref lhs: int, rhs: int) { }

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
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      operator +=(ref lhs: int, rhs: int) { }
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
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      operator +=(ref lhs: int, rhs: int) { }
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
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      operator +=(ref lhs: int, rhs: int) { }
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
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      operator +=(ref lhs: int, rhs: int) { }
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
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const char* contents =
    R""""(
    module M {
      operator +=(ref lhs: int, rhs: int) { }
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


int main() {
  test1();
  test2();
  test3();
  test4();
  test5a();
  test5b();
  test6a();
  test6b();

  return 0;
}

