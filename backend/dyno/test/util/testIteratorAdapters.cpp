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

#include "chpl/util/iteration.h"

#include <cassert>
#include <cstddef>

#include <vector>
#include <unordered_set>

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

using namespace chpl;

// Iteration over an expanding vector works
static void test1() {
  std::vector<int> xs = {};

  // empty iteration works
  for (int i : expandingIterator(xs)) {
    (void)i;
    assert(false);
  }

  xs.push_back(0);
  for (int i : expandingIterator(xs)) {
    if (i < 5) xs.push_back(i + 1);
  }
  assert(xs == std::vector<int>({0, 1, 2, 3, 4, 5}));
}

// Iteration over an expanding vector works even for large sizes (where we expect reallocation to occur)
static void test2() {
  std::vector<int> xs = {0};
  const int n = 1024;
  for (int i : expandingIterator(xs)) {
    if (i < n) xs.push_back(i + 1);
  }
  assert(xs.size() == (size_t)n + 1);
}

static void test3Check(const std::vector<int*>& xs, const std::vector<int*>& expected) {
  std::vector<int*> acc;
  for (int* i : chpl::uptoNullptrIterator(xs)) {
    acc.push_back(i);
  }
  assert(acc == expected);
}
// Iteration stops at a nullptr for uptoNullPtrIterator
// Check for both vectors and sets
static void test3() {
  int a, b, c;

  test3Check({}, {});
  test3Check({nullptr}, {});
  test3Check({&a, &b, &c}, {&a, &b, &c});
  test3Check({&a, &b, nullptr, &c}, {&a, &b});
  test3Check({&a, &b, &c, nullptr}, {&a, &b, &c});
}

int main(int argc, char** argv) {
  test1();
  test2();
  test3();
  return 0;
}
