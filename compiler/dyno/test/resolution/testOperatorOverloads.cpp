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

#include "common.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

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

  // primary operator method (currently fails)
  /*
  QualifiedType qt2 = resolveTypeOfXInit(context,
    R""""(
      record R {
        var field: int;
        operator :(x: R, type t: int) { return x.field; }
      }
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt2.type() && qt2.type()->isIntType());
  assert(qt2.kind() == QualifiedType::VAR);
  ctx.advanceToNextRevision(false);
  */

  // secondary operator method (currently fails)
  /*
  QualifiedType qt3 = resolveTypeOfXInit(context,
    R""""(
      record R {
        var field: int;
      }
      operator R.:(x: R, type t: int) { return x.field; }
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt3.type() && qt3.type()->isIntType());
  assert(qt3.kind() == QualifiedType::VAR);
  ctx.advanceToNextRevision(false);
  */

  // non-method operator
  QualifiedType qt4 = resolveTypeOfXInit(context,
    R""""(
      record R {
      var field: int;
      }
      operator :(x: R, type t: int) { return x.field; }
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt4.type() && qt4.type()->isIntType());
  assert(qt4.kind() == QualifiedType::VAR);
  ctx.advanceToNextRevision(false);
}

static void test2() {
  Context ctx;
  auto context = &ctx;

  // method and function operator definitions should conflict (ambiguous call)
  QualifiedType qt1 = resolveTypeOfXInit(context,
    R""""(
      record R {
      var field: int;
        operator :(x: R, type t: int) { return x.field; }
      }
      operator R.:(x: R, type t: int) { return x.field; }
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt1.kind() == QualifiedType::UNKNOWN);
  assert(qt1.type() && qt1.type()->isErroneousType());
  ctx.advanceToNextRevision(false);

  // access to R should implicitly grant access to its method operators
  /*
  QualifiedType qt2 = resolveTypeOfXInit(context,
    R""""(
      module M {
        record R {
        var field: int;
          operator :(x: R, type t: int) { return x.field; }
        }
      }

      import M.R;
      var myR: R;
      var x = myR : int;
    )""""
  );
  assert(qt2.type() && qt2.type()->isIntType());
  assert(qt2.kind() == QualifiedType::VAR);
  ctx.advanceToNextRevision(false);
  */
}


int main() {
  test1();
  test2();
  return 0;
}
