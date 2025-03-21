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

#include "chpl/resolution/resolution-queries.h"
#include "test-common.h"
#include "test-resolution.h"

using namespace chpl;
using namespace resolution;

static void testIterFn(Context* context) {
  testArrayMaterialize(context,
                       "iter foo() do yield 1;",
                       "foo()",
                       1, "one", "ChapelArray.chpl__initCopy#6");
}

static void testForeachRange(Context* context) {
  testArrayMaterialize(context,
                       "",
                       "foreach i in 1..10 do i",
                       1, "one", "ChapelArray.chpl__initCopy#5");
}

static void testForeachStridedRange(Context* context) {
  testArrayMaterialize(context,
                       "",
                       "foreach i in 1..10 by 2 do i",
                       1, "positive", "ChapelArray.chpl__initCopy#5");
}

static void testForeachReverseRange(Context* context) {
  testArrayMaterialize(context,
                       "",
                       "foreach i in 10..1 by -1 do i",
                       1, "negOne", "ChapelArray.chpl__initCopy#5");
}

static void testForeachReverseStridedRange(Context* context) {
  testArrayMaterialize(context,
                       "",
                       "foreach i in 10..1 by -2 do i",
                       1, "negative", "ChapelArray.chpl__initCopy#5");
}

static void testForeachOverForeach(Context* context) {
  testArrayMaterialize(context,
                       "",
                       "foreach i in (foreach j in 1..10 do j) do i",
                       1, "one", "ChapelArray.chpl__initCopy#5");
}

static void testForeachOverStridedForeach(Context* context) {
  testArrayMaterialize(context,
                       "",
                       "foreach i in (foreach j in 1..10 by 2 do j) do i",
                       1, "positive", "ChapelArray.chpl__initCopy#5");
}

static void testForeachOverDomain(Context* context) {
  testArrayMaterialize(context,
                       "",
                       "foreach i in {1..10} do i",
                       1, "one", "ChapelArray.chpl__initCopy#4");
  testArrayMaterialize(context,
                       "",
                       "foreach i in {1..10, 1..10} do i",
                       2, "one", "ChapelArray.chpl__initCopy#4");
  testArrayMaterialize(context,
                       "",
                       "foreach i in {1..10 by 2, 1..10} do i",
                       2, "positive", "ChapelArray.chpl__initCopy#4");
}

static void testForRange(Context* context) {
  testArrayMaterialize(context,
                       "",
                       "for i in 1..10 do i",
                       1, "one", "ChapelArray.chpl__initCopy#6");
}

static void testForStridedRange(Context* context) {
  testArrayMaterialize(context,
                       "",
                       "for i in 1..10 by 2 do i",
                       1, "positive", "ChapelArray.chpl__initCopy#6");
}

static void testForReverseRange(Context* context) {
  testArrayMaterialize(context,
                       "",
                       "for i in 10..1 by -1 do i",
                       1, "negOne", "ChapelArray.chpl__initCopy#6");
}

static void testForReverseStridedRange(Context* context) {
  testArrayMaterialize(context,
                       "",
                       "for i in 10..1 by -2 do i",
                       1, "negative", "ChapelArray.chpl__initCopy#6");
}

static void testForOverFor(Context* context) {
  testArrayMaterialize(context,
                       "",
                       "for i in (for j in 1..10 do j) do i",
                       1, "one", "ChapelArray.chpl__initCopy#6");
}

static void testForOverStridedFor(Context* context) {
  testArrayMaterialize(context,
                       "",
                       "for i in (for j in 1..10 by 2 do j) do i",
                       1, "positive", "ChapelArray.chpl__initCopy#6");
}

static void testForOverDomain(Context* context) {
  testArrayMaterialize(context,
                       "",
                       "for i in {1..10} do i",
                       1, "one", "ChapelArray.chpl__initCopy#4");
  testArrayMaterialize(context,
                       "",
                       "for i in {1..10, 1..10} do i",
                       2, "one", "ChapelArray.chpl__initCopy#4");
  testArrayMaterialize(context,
                       "",
                       "for i in {1..10 by 2, 1..10} do i",
                       2, "positive", "ChapelArray.chpl__initCopy#4");
}

int main() {
  auto context = buildStdContext();
  testIterFn(context);
  testForeachRange(context);
  testForeachStridedRange(context);
  testForeachReverseRange(context);
  testForeachReverseStridedRange(context);
  testForeachOverForeach(context);
  testForeachOverStridedForeach(context);
  testForeachOverDomain(context);
  testForRange(context);
  testForStridedRange(context);
  testForReverseRange(context);
  testForReverseStridedRange(context);
  testForOverFor(context);
  testForOverStridedFor(context);
  testForOverDomain(context);
}
