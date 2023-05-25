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

#include "test-common.h"

#include "chpl/framework/Context.h"
#include "chpl/framework/query-impl.h"

#include <cstdlib>

using namespace chpl;

// This test uses global variables so that the queries
// have side effects. That's not allowed in the framework
// and if this test becomes problematic then it should be updated.
const char* inputString = "no input";
int nInputQueryRuns = 0;
int nQueryOneRuns = 0;
int nQueryTwoRuns = 0;
int nQueryThreeRuns = 0;
int nQueryFourRuns = 0;
int nQueryFiveRuns = 0;
int nQuerySixRuns = 0;
int nQuerySevenRuns = 0;

static const std::string& inputQuery(Context* context, int unused) {
  QUERY_BEGIN_INPUT(inputQuery, context, unused);

  std::string result = inputString;
  nInputQueryRuns++;
  printf("inputQuery returning %s\n", result.c_str());

  return QUERY_END(result);
}
static const std::string& queryOne(Context* context, int unused) {
  QUERY_BEGIN(queryOne, context, unused);

  const std::string& input = inputQuery(context, 0);
  std::string result = input;
  nQueryOneRuns++;
  printf("queryOne returning %s\n", result.c_str());

  return QUERY_END(result);
}
static const std::string& queryTwo(Context* context, int unused) {
  QUERY_BEGIN(queryTwo, context, unused);

  const std::string& one = queryOne(context, 0);
  // resize to half the string size
  std::string result = one;
  result.resize(result.size() / 2);
  nQueryTwoRuns++;
  printf("queryTwo returning %s\n", result.c_str());

  return QUERY_END(result);
}

static const int& queryThree(Context* context, int unused) {
  QUERY_BEGIN(queryThree, context, unused);

  const std::string& one = queryOne(context, 0);
  int result = (one.size() % 2);
  nQueryThreeRuns++;
  printf("queryThree returning %i\n", result);

  return QUERY_END(result);
}

static const std::string& queryFour(Context* context, int unused) {
  QUERY_BEGIN(queryFour, context, unused);

  const std::string& one = queryOne(context, 0);
  std::string result = one + one;
  nQueryFourRuns++;
  printf("queryFour returning %s\n", result.c_str());

  return QUERY_END(result);
}

static const std::string& queryFive(Context* context, int unused) {
  QUERY_BEGIN(queryFive, context, unused);

  const std::string& one = queryOne(context, 0);
  std::string result = one + "..";
  nQueryFiveRuns++;
  printf("queryFive returning %s\n", result.c_str());

  return QUERY_END(result);
}

static const std::string& querySix(Context* context, int unused) {
  QUERY_BEGIN(querySix, context, unused);

  int isQueryOneOddLength = queryThree(context, 0);
  std::string result;
  // if the size is odd, use queryFour to get an even size
  if (isQueryOneOddLength != 0) {
    result = queryFour(context, 0);
  } else {
    result = queryFive(context, 0);
  }
  nQuerySixRuns++;
  printf("querySix returning %s\n", result.c_str());

  return QUERY_END(result);
}

static const int& querySeven(Context* context, int unused) {
  QUERY_BEGIN(querySeven, context, unused);

  int result = queryThree(context, 0);
  result *= 2;
  nQuerySevenRuns++;
  printf("querySeven returning %d\n", result);

  return QUERY_END(result);
}


static void test0() {
  printf("test0\n");

  Context ctx;
  Context* context = &ctx;

  printf("part 1\n");
  // test that an input query is run exactly once when requested
  context->advanceToNextRevision(false);
  inputString = "hello there giant world";
  nInputQueryRuns = 0;
  std::string ii = inputQuery(context, 0);
  assert(nInputQueryRuns == 1);
  assert(ii == inputString);

  printf("part 2\n");
  // and that if we repeat the query, it's still run again
  // (because it might need to read a file etc)
  context->advanceToNextRevision(false);
  ii = inputQuery(context, 0);
  assert(nInputQueryRuns == 2);
  assert(ii == inputString);
}

static void test1() {
  printf("test1\n");

  Context ctx;
  Context* context = &ctx;

  printf("part 1\n");
  // test that the queries are run just once
  context->advanceToNextRevision(false);
  inputString = "hello there giant world";
  nInputQueryRuns = 0;
  nQueryOneRuns = 0;
  std::string q1 = queryOne(context, 0);
  assert(nInputQueryRuns == 1);
  assert(nQueryOneRuns == 1);
  assert(q1 == inputString);

  printf("part 2\n");
  // and test what happens when we repeat the query
  context->advanceToNextRevision(false);
  q1 = queryOne(context, 0);
  // input query should run again, but the result doesn't change,
  // so queryOne should not be run again.
  assert(nInputQueryRuns == 2);
  assert(nQueryOneRuns == 1);
  assert(q1 == inputString);

  printf("part 3\n");
  // test what happens when we repeat the query with the input changing
  context->advanceToNextRevision(false);
  inputString = "hello again there";
  q1 = queryOne(context, 0);
  // input query should have run again, and then the result changed,
  // so queryOne should have run again as well.
  assert(nInputQueryRuns == 3);
  assert(nQueryOneRuns == 2);
  assert(q1 == "hello again there");
}

static void test2() {
  printf("test2\n");

  Context ctx;
  Context* context = &ctx;

  context->advanceToNextRevision(false);
  inputString = "hello there giant world";
  // run the dependencies first
  nInputQueryRuns = 0;
  nQueryOneRuns = 0;
  nQueryTwoRuns = 0;
  std::string ii = inputQuery(context, 0);
  std::string q1 = queryOne(context, 0);
  assert(nInputQueryRuns == 1);
  assert(nQueryOneRuns == 1);
  assert(ii == inputString);
  assert(q1 == inputString);
  // then run the query. the dependencies should not be rerun
  // test that the queries are run just once
  std::string q2 = queryTwo(context, 0);
  assert(nInputQueryRuns == 1);
  assert(nQueryOneRuns == 1);
  assert(nQueryTwoRuns == 1);
  assert(q2 == "hello there");
}

static void test3() {
  printf("test3\n");

  Context ctx;
  Context* context = &ctx;

  // get a baseline
  printf("part 1\n");
  context->advanceToNextRevision(false);
  inputString = "hello there giant world";
  nInputQueryRuns = 0;
  nQueryOneRuns = 0;
  nQueryTwoRuns = 0;
  std::string q2 = queryTwo(context, 0);
  assert(nInputQueryRuns == 1);
  assert(nQueryOneRuns == 1);
  assert(nQueryTwoRuns == 1);
  assert(q2 == "hello there");

  // now change the 2nd half of the input
  printf("part 2\n");
  context->advanceToNextRevision(false);
  inputString = "hello there giant worl ";
  q2 = queryTwo(context, 0);
  assert(nInputQueryRuns == 2);
  assert(nQueryOneRuns == 2);
  assert(nQueryTwoRuns == 2);
  assert(q2 == "hello there");

  // now change the 1st half of the input
  printf("part 3\n");
  context->advanceToNextRevision(false);
  inputString = " ello there giant worl ";
  q2 = queryTwo(context, 0);
  assert(nInputQueryRuns == 3);
  assert(nQueryOneRuns == 3);
  assert(nQueryTwoRuns == 3);
  assert(q2 == " ello there");
}

static void test4() {
  printf("test4\n");

  Context ctx;
  Context* context = &ctx;

  // get a baseline
  printf("part 1\n");
  context->advanceToNextRevision(false);
  inputString = "hello there giant world"; // note: odd length
  assert(strlen(inputString) % 2 == 1);
  nInputQueryRuns = 0;
  nQueryOneRuns = 0;
  nQueryTwoRuns = 0;
  nQueryThreeRuns = 0;
  nQueryFourRuns = 0;
  nQueryFiveRuns = 0;
  nQuerySixRuns = 0;
  nQuerySevenRuns = 0;
  std::string q6 = querySix(context, 0);
  assert(nInputQueryRuns == 1);
  assert(nQueryOneRuns == 1);
  assert(nQueryTwoRuns == 0);
  assert(nQueryThreeRuns == 1);
  assert(nQueryFourRuns == 1);
  assert(nQueryFiveRuns == 0);
  assert(nQuerySixRuns == 1);
  assert(nQuerySevenRuns == 0);
  assert(q6 == "hello there giant worldhello there giant world");

  // run it again with no changes
  printf("part 2\n");
  context->advanceToNextRevision(false);
  q6 = querySix(context, 0);
  assert(nInputQueryRuns == 2);
  assert(nQueryOneRuns == 1);
  assert(nQueryTwoRuns == 0);
  assert(nQueryThreeRuns == 1);
  assert(nQueryFourRuns == 1);
  assert(nQueryFiveRuns == 0);
  assert(nQuerySixRuns == 1);
  assert(nQuerySevenRuns == 0);
  assert(q6 == "hello there giant worldhello there giant world");

  // run it again with something that is still odd length
  printf("part 3\n");
  inputString = "hello there world";
  assert(strlen(inputString) % 2 == 1);
  context->advanceToNextRevision(false);
  q6 = querySix(context, 0);
  assert(nInputQueryRuns == 3);
  assert(nQueryOneRuns == 2);
  assert(nQueryTwoRuns == 0);
  assert(nQueryThreeRuns == 2);
  assert(nQueryFourRuns == 2);
  assert(nQueryFiveRuns == 0);
  assert(nQuerySixRuns == 2);
  assert(nQuerySevenRuns == 0);
  assert(q6 == "hello there worldhello there world");

  // now change to something that is of even length
  printf("part 4\n");
  inputString = "helloworld";
  assert(strlen(inputString) % 2 == 0);
  context->advanceToNextRevision(false);
  q6 = querySix(context, 0);
  assert(nInputQueryRuns == 4);
  assert(nQueryOneRuns == 3);
  assert(nQueryTwoRuns == 0);
  assert(nQueryThreeRuns == 3);
  assert(nQueryFourRuns == 2);
  assert(nQueryFiveRuns == 1);
  assert(nQuerySixRuns == 3);
  assert(nQuerySevenRuns == 0);
  assert(q6 == "helloworld..");
}

static void test5() {
  printf("test5\n");

  Context ctx;
  Context* context = &ctx;

  // get a baseline
  printf("part 1\n");
  context->advanceToNextRevision(false);
  inputString = "hello there giant world."; // even length
  assert(strlen(inputString) % 2 == 0);
  nInputQueryRuns = 0;
  nQueryOneRuns = 0;
  nQueryTwoRuns = 0;
  nQueryThreeRuns = 0;
  nQueryFourRuns = 0;
  nQueryFiveRuns = 0;
  nQuerySixRuns = 0;
  nQuerySevenRuns = 0;
  std::string q6 = querySix(context, 0);
  assert(nInputQueryRuns == 1);
  assert(nQueryOneRuns == 1);
  assert(nQueryTwoRuns == 0);
  assert(nQueryThreeRuns == 1);
  assert(nQueryFourRuns == 0);
  assert(nQueryFiveRuns == 1);
  assert(nQuerySixRuns == 1);
  assert(nQuerySevenRuns == 0);
  assert(q6 == "hello there giant world...");

  // run it again with no changes
  printf("part 2\n");
  context->advanceToNextRevision(false);
  q6 = querySix(context, 0);
  assert(nInputQueryRuns == 2);
  assert(nQueryOneRuns == 1);
  assert(nQueryTwoRuns == 0);
  assert(nQueryThreeRuns == 1);
  assert(nQueryFourRuns == 0);
  assert(nQueryFiveRuns == 1);
  assert(nQuerySixRuns == 1);
  assert(nQuerySevenRuns == 0);
  assert(q6 == "hello there giant world...");

  // run it again with something that is still even length
  printf("part 3\n");
  context->advanceToNextRevision(false);
  inputString = "hello there giant wrld"; // even length
  assert(strlen(inputString) % 2 == 0);
  q6 = querySix(context, 0);
  assert(nInputQueryRuns == 3);
  assert(nQueryOneRuns == 2);
  assert(nQueryTwoRuns == 0);
  assert(nQueryThreeRuns == 2);
  assert(nQueryFourRuns == 0);
  assert(nQueryFiveRuns == 2);
  assert(nQuerySixRuns == 2);
  assert(nQuerySevenRuns == 0);
  assert(q6 == "hello there giant wrld..");


  // run it again changing to odd length
  printf("part 4\n");
  context->advanceToNextRevision(false);
  inputString = "hello there giant world"; // odd length
  assert(strlen(inputString) % 2 == 1);
  q6 = querySix(context, 0);
  assert(nInputQueryRuns == 4);
  assert(nQueryOneRuns == 3);
  assert(nQueryTwoRuns == 0);
  assert(nQueryThreeRuns == 3);
  assert(nQueryFourRuns == 1);
  assert(nQueryFiveRuns == 2);
  assert(nQuerySixRuns == 3);
  assert(nQuerySevenRuns == 0);
  assert(q6 == "hello there giant worldhello there giant world");
}

// Pre-test for test6b
// query three's result doesn't change even when query one's result changes, so
// query seven is not recomputed.
static void test6a() {
  Context ctx;
  Context* context = &ctx;

  // get a baseline
  printf("part 1\n");
  inputString = "hello world";
  nInputQueryRuns = 0;
  nQueryOneRuns = 0;
  nQueryTwoRuns = 0;
  nQueryThreeRuns = 0;
  nQueryFourRuns = 0;
  nQueryFiveRuns = 0;
  nQuerySixRuns = 0;
  nQuerySevenRuns = 0;

  std::ignore = querySeven(context, /* unused */ 0);
  assert(nInputQueryRuns == 1);
  assert(nQueryOneRuns == 1);
  assert(nQueryTwoRuns == 0);
  assert(nQueryThreeRuns == 1);
  assert(nQueryFourRuns == 0);
  assert(nQueryFiveRuns == 0);
  assert(nQuerySixRuns == 0);
  assert(nQuerySevenRuns == 1);

  context->advanceToNextRevision(false);
  printf("part 2\n");
  inputString = "world hello"; /* Same parity as hello world, should not re-compute query 7. */
  std::ignore = querySeven(context, /* unused */ 0);
  assert(nInputQueryRuns == 2);
  assert(nQueryOneRuns == 2);
  assert(nQueryTwoRuns == 0);
  assert(nQueryThreeRuns == 2);
  assert(nQueryFourRuns == 0);
  assert(nQueryFiveRuns == 0);
  assert(nQuerySixRuns == 0);
  assert(nQuerySevenRuns == 1);
}

// This is pretty much the same as 6a, except the sub-computation of 3->1->i
// is computed separately before 7 is ever invoked. This is a regression test
// for a bug in the query system in which query 7 could be made to depend on
// query 1, thus making it be recomputed.
static void test6b() {
  Context ctx;
  Context* context = &ctx;

  // get a baseline
  printf("part 1\n");
  inputString = "hello world";
  nInputQueryRuns = 0;
  nQueryOneRuns = 0;
  nQueryTwoRuns = 0;
  nQueryThreeRuns = 0;
  nQueryFourRuns = 0;
  nQueryFiveRuns = 0;
  nQuerySixRuns = 0;
  nQuerySevenRuns = 0;

  std::ignore = queryThree(context, /* unused */ 0);
  assert(nInputQueryRuns == 1);
  assert(nQueryOneRuns == 1);
  assert(nQueryTwoRuns == 0);
  assert(nQueryThreeRuns == 1);
  assert(nQueryFourRuns == 0);
  assert(nQueryFiveRuns == 0);
  assert(nQuerySixRuns == 0);
  assert(nQuerySevenRuns == 0);

  context->advanceToNextRevision(false);
  printf("part 2\n");
  std::ignore = querySeven(context, /* unused */ 0);
  assert(nInputQueryRuns == 2);
  assert(nQueryOneRuns == 1);
  assert(nQueryTwoRuns == 0);
  assert(nQueryThreeRuns == 1);
  assert(nQueryFourRuns == 0);
  assert(nQueryFiveRuns == 0);
  assert(nQuerySixRuns == 0);
  assert(nQuerySevenRuns == 1);

  context->advanceToNextRevision(false);
  printf("part 3\n");
  inputString = "world hello"; /* Same parity as hello world, should not re-compute query 7. */
  std::ignore = querySeven(context, /* unused */ 0);
  assert(nInputQueryRuns == 3);
  assert(nQueryOneRuns == 2);
  assert(nQueryTwoRuns == 0);
  assert(nQueryThreeRuns == 2);
  assert(nQueryFourRuns == 0);
  assert(nQueryFiveRuns == 0);
  assert(nQuerySixRuns == 0);
  assert(nQuerySevenRuns == 1);
}

int main() {
  test0();
  test1();
  test2();
  test3();
  test4();
  test5();
  test6a();
  test6b();

  return 0;
}
