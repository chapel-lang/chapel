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

static void testPromotionRange(Context* context) {
  testArrayMaterialize(context,
                       "proc foo(x) do return x : real;",
                       "foo(1..10)",
                       1, "one", "ChapelArray.chpl__initCopy#5");
}

static void testPromotionStridedRange(Context* context) {
  testArrayMaterialize(context,
                       "proc foo(x) do return x : real;",
                       "foo(1..10 by 2)",
                       1, "positive", "ChapelArray.chpl__initCopy#5");
}

static void testPromotionReverseRange(Context* context) {
  testArrayMaterialize(context,
                       "proc foo(x) do return x : real;",
                       "foo(10..1 by -1)",
                       1, "negOne", "ChapelArray.chpl__initCopy#5");
}

static void testPromotionReverseStridedRange(Context* context) {
  testArrayMaterialize(context,
                       "proc foo(x) do return x : real;",
                       "foo(10..1 by -2)",
                       1, "negative", "ChapelArray.chpl__initCopy#5");
}

static void testPromotionOverPromotion(Context* context) {
  testArrayMaterialize(context,
                       "proc foo(x) do return x : real;",
                       "foo(foo(1..10))",
                       1, "one", "ChapelArray.chpl__initCopy#5");
}

static void testPromotionOverStridedPromotion(Context* context) {
  testArrayMaterialize(context,
                       "proc foo(x) do return x : real;",
                       "foo(foo(1..10 by 2))",
                       1, "positive", "ChapelArray.chpl__initCopy#5");
}

static void testPromotionOverDomain(Context* context) {
  testArrayMaterialize(context,
                       "proc foo(x) do return x : real;",
                       "foo({1..10})",
                       1, "one", "ChapelArray.chpl__initCopy#4");
  testArrayMaterialize(context,
                       "proc foo((x, y)) do return (x : real, y : real);",
                       "foo({1..10, 1..10})",
                       2, "one", "ChapelArray.chpl__initCopy#4");
  testArrayMaterialize(context,
                       "proc foo((x, y)) do return (x : real, y : real);",
                       "foo({1..10 by 2, 1..10})",
                       2, "positive", "ChapelArray.chpl__initCopy#4");
}

int main() {
  auto context = buildStdContext();
  testPromotionRange(context);
  testPromotionStridedRange(context);
  testPromotionReverseRange(context);
  testPromotionReverseStridedRange(context);
  testPromotionOverPromotion(context);
  testPromotionOverStridedPromotion(context);
  testPromotionOverDomain(context);
}
