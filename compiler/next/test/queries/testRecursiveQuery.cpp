/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#include "chpl/queries/Context.h"
#include "chpl/queries/query-impl.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <cstdlib>

using namespace chpl;

// query with
//  arg 0 calls arg 1
//  arg 1 calls arg 0

static const int& recursiveQuery(Context* context, int arg) {
  printf("recursiveQuery(%i)\n", arg);
  QUERY_BEGIN(recursiveQuery, context, arg);

  // this should just be true always since we're in the query body
  assert(QUERY_RUNNING_PARTIAL_RESULT(recursiveQuery, context, arg) != nullptr);

  int result = -1;
  // update the partial result in the map in case of recursion
  QUERY_CURRENT_RESULT = result;

  int newArg = (int)(arg == 0);
  printf("checking %i\n", newArg);
  const int* r = QUERY_RUNNING_PARTIAL_RESULT(recursiveQuery, context, newArg);
  if (r == nullptr) {
    printf("no partial result for %i\n", newArg);
    // not recursing so just run the recursive query as usual
    result = recursiveQuery(context, newArg) + 1;
  } else {
    printf("found partial result for %i\n", newArg);
    // recursing would cause a fatal error so avoid it
    assert(*r == -1);
    result = 0;
  }

  return QUERY_END(result);
}

static void test0() {
  Context ctx;
  Context* context = &ctx;

  printf("test 0\n");
  int r0 = recursiveQuery(context, 0);
  assert(r0 == 1);
}

static void test1() {
  Context ctx;
  Context* context = &ctx;

  printf("test 1\n");
  int r1 = recursiveQuery(context, 1);
  assert(r1 == 1);
}

int main() {
  test0();
  test1();
  return 0;
}
