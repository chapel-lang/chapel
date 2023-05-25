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

// basic definition tests with cast operator
static void test1() {
  Context ctx;
  auto context = &ctx;

  // no declaration (expect cast to fail)
  QualifiedType qt1 = resolveTypeOfXInit(context,
    R""""(
      record R {
        var field: int;
      }
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt1.kind() == QualifiedType::UNKNOWN);
  assert(qt1.type() && qt1.type()->isErroneousType());
  ctx.advanceToNextRevision(false);

  // primary operator method
  QualifiedType qt2 = resolveTypeOfXInit(context,
    R""""(
      record R {
        var field: int;
        operator :(z: R, type t: int) { return z.field; }
      }
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt2.type() && qt2.type()->isIntType());
  assert(qt2.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);

  // secondary operator method
  QualifiedType qt3 = resolveTypeOfXInit(context,
    R""""(
      record R {
        var field: int;
      }
      operator R.:(z: R, type t: int) { return z.field; }
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt3.type() && qt3.type()->isIntType());
  assert(qt3.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);

  // non-method operator
  QualifiedType qt4 = resolveTypeOfXInit(context,
    R""""(
      record R {
        var field: int;
      }
      operator :(z: R, type t: int) { return z.field; }
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt4.type() && qt4.type()->isIntType());
  assert(qt4.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);
}

// scope issue tests with cast operator
static void test2() {
  Context ctx;
  auto context = &ctx;

  // method and function operator definitions in the same scope should conflict (ambiguous call)
  QualifiedType qt1 = resolveTypeOfXInit(context,
    R""""(
      record R {
        var field: int;
      }
      operator R.:(z: R, type t: int) { return z.field; }
      operator :(z: R, type t: int) { return z.field; }
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt1.kind() == QualifiedType::UNKNOWN);
  assert(qt1.type() && qt1.type()->isErroneousType());
  ctx.advanceToNextRevision(false);

  // access to R should implicitly grant access to its method operators
  QualifiedType qt2 = resolveTypeOfXInit(context,
    R""""(
      module M {
        record R {
          var field: int;
        }
        operator R.:(z: R, type t: int) { return z.field; }
      }

      import this.M.R;
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt2.type() && qt2.type()->isIntType());
  assert(qt2.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);
}

// various additional tests with other operators
static void test3() {
  Context ctx;
  auto context = &ctx;

  // incorrectly defined unary operator overload
  QualifiedType qt1 = resolveTypeOfXInit(context,
    R""""(
      record R {
        var field: int;
        operator !(z: R, type t: int) { return z.field; }
      }
      var myR: R;
      var x = !myR;
    )""""
  );
  assert(qt1.kind() == QualifiedType::UNKNOWN);
  assert(qt1.type() && qt1.type()->isErroneousType());
  ctx.advanceToNextRevision(false);

  // correct unary operator overload
  QualifiedType qt2 = resolveTypeOfXInit(context,
    R""""(
      record R {
        var field: int;
        operator !(z: R) { return z.field; }
      }
      var myR: R;
      var x = !myR;
    )""""
  );
  assert(qt2.type() && qt2.type()->isIntType());
  assert(qt2.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);

  // overloading more operators
  QualifiedType qt3 = resolveTypeOfXInit(context,
    R""""(
      record R {
        var field: int;
        operator +(z: R, t: int) { return z.field; }
      }
      var myR: R;
      var x = myR + 3;
    )""""
  );
  assert(qt3.type() && qt3.type()->isIntType());
  assert(qt3.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);

  QualifiedType qt4 = resolveTypeOfXInit(context,
    R""""(
      record R {
        var field: int;
        operator >(z: R, t: complex) { return true; }
      }
      var myR: R;
      var x = myR > 2;
    )""""
  );
  assert(qt4.type() && qt4.type()->isBoolType());
  assert(qt4.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);

  QualifiedType qt5 = resolveTypeOfXInit(context,
    R""""(
      record R {
        var field: int;
      }
      operator <(z: R, otherZ: R) { return 3; }
      var myR: R;
      var myOtherR: R;
      var x = myR < myOtherR;
    )""""
  );
  assert(qt5.type() && qt5.type()->isIntType());
  assert(qt5.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);

  // overloading operator for non-compound types
  QualifiedType qt6 = resolveTypeOfXInit(context,
    R""""(
      operator *(a: int, b: int) { return true; }
      var a: int = 2;
      var b: int = 3;
      var x = a * b;
    )""""
  );
  assert(qt6.type() && qt6.type()->isBoolType());
  assert(qt6.kind() == QualifiedType::CONST_VAR);
  ctx.advanceToNextRevision(false);
}


int main() {
  test1();
  test2();
  test3();

  return 0;
}
