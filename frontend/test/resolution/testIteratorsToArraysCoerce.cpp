/*
 * Copyright 2025 Hewlett Packard Enterprise Development LP
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

#include "test-common.h"
#include "test-resolution.h"

using namespace chpl;
using namespace resolution;

// Note: matching array bounds are enforced at runtime, not compile time.
// The various strides etc. here serve to vary the type of the array (e.g.,
// strides=positive to strides=negative).
//
// Here, the striding etc. is determined by the array expression, so details
// of the initialization expression should be dropped.

static void testIterFn(Context* context) {
  testArrayCoerce(context,
                  "iter foo() do yield 1;",
                  ": [1..1] int",
                  "foo()",
                  1, "one", "ChapelArray.chpl__coerceMove#5");
}

static void testForeachRange(Context* context) {
  testArrayCoerce(context,
                  "",
                  ": [1..10] int",
                  "foreach i in 1..10 do i",
                  1, "one", "ChapelArray.chpl__coerceMove#5");
}

static void testForeachStridedRange(Context* context) {
  testArrayCoerce(context,
                  "",
                  ": [1..5] int",
                  "foreach i in 1..10 by 2 do i",
                  1, "one", "ChapelArray.chpl__coerceMove#5");
}

static void testForeachOverForeach(Context* context) {
  testArrayCoerce(context,
                  "",
                  ": [1..10] int",
                  "foreach i in (foreach j in 1..10 do j) do i",
                  1, "one", "ChapelArray.chpl__coerceMove#5");
}

static void testChangeDimension(Context* context) {
  testArrayCoerce(context,
                  "",
                  ": [1..5, 1..5] int",
                  "foreach i in 1..25 do i",
                  2, "one", "ChapelArray.chpl__coerceMove#5");
}

static void testPromotionRange(Context* context) {
  testArrayCoerce(context,
                  "proc foo(x) do return x : real;",
                  ": [1..10] real",
                  "foo(1..10)",
                  1, "one", "ChapelArray.chpl__coerceMove#5");
}

static void testPromotionStridedRange(Context* context) {
  testArrayCoerce(context,
                  "proc foo(x) do return x : real;",
                  ": [1..5] real",
                  "foo(1..10 by 2)",
                  1, "one", "ChapelArray.chpl__coerceMove#5");
}

static void testPromotionOverDomain(Context* context) {
  testArrayCoerce(context,
                  "proc foo(x) do return x : real;",
                  ": [1..10] real",
                  "foo({1..10})",
                  1, "one", "ChapelArray.chpl__coerceMove#5");
  testArrayCoerce(context,
                  "proc foo((x, y)) do return (x : real, y : real);",
                  ": [1..10, 1..10] (real, real)",
                  "foo({1..10, 1..10})",
                  2, "one", "ChapelArray.chpl__coerceMove#5");
}


int main() {
  auto context = buildStdContext();
  testIterFn(context);
  testForeachRange(context);
  testForeachStridedRange(context);
  testForeachOverForeach(context);
  testChangeDimension(context);
  testPromotionRange(context);
  testPromotionStridedRange(context);
  testPromotionOverDomain(context);
}
