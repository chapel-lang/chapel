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
#include "chpl/types/BasicClassType.h"
#include "chpl/types/ClassType.h"
#include "chpl/types/IntType.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/uast/Class.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"


/*
 ============= TODO ============
 These tests are here because we currently don't use the Dyno resolver
 in production, and thus can't use the usual .good-.chpl structure to
 lock down error message output. It is definitely suboptimal to include
 strings of error message output in this file, and once these errors can
 be triggered using `chpl`, this file should be replaced in favor of the usual
 .good-based tests.
 */

static const char* progExactMatch = R"""(
class Parent {}
class Child : Parent {}

proc f(ref x: owned Parent) {}

var x: owned Child;
f(x);
)""";

static const char* errorExactMatch = R"""(
─── error in file.chpl:7 [NoMatchingCandidates] ───
  Unable to resolve call to 'f': no matching candidates.
      |
    7 | f(x);
      |
  
  The following candidate didn't match because an actual couldn't be passed to a formal:
      |
    4 | proc f(ref x: owned Parent) {}
      |        ⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺
      |
  The formal 'x' expects a value of type 'owned Parent', but the actual was a value of type 'owned Child'.
      |
    7 | f(x);
      |   ⎺
      |
  The 'ref' intent requires the formal and actual types to match exactly.
)""";

static const char* progExpectedSubType = R"""(
proc f(type x: string) {}

f(int);
)""";

static const char* errorExpectedSubType = R"""(
─── error in file.chpl:3 [NoMatchingCandidates] ───
  Unable to resolve call to 'f': no matching candidates.
      |
    3 | f(int);
      |
  
  The following candidate didn't match because an actual couldn't be passed to a formal:
      |
    1 | proc f(type x: string) {}
      |        ⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺
      |
  The formal 'x' expects the type 'string', but the actual was the type 'int(64)'.
      |
    3 | f(int);
      |   ⎺⎺⎺
      |
  Formals with kind 'type' expect the actual to be a subtype, but 'int(64)' is not a subtype of 'string'.
)""";

static const char* progIncompatibleMgr = R"""(
class C {}

proc f(x: owned C) {}

var x: shared C;
f(x);
)""";

static const char* errorIncompatibleMgr = R"""(
─── error in file.chpl:6 [NoMatchingCandidates] ───
  Unable to resolve call to 'f': no matching candidates.
      |
    6 | f(x);
      |
  
  The following candidate didn't match because an actual couldn't be passed to a formal:
      |
    3 | proc f(x: owned C) {}
      |        ⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺
      |
  The formal 'x' expects a value of type 'owned C', but the actual was a value of type 'shared C'.
      |
    6 | f(x);
      |   ⎺
      |
  A class with 'shared' management cannot be passed to a formal with 'owned' management.
)""";

static const char* progIncompatibleNilability = R"""(
class C {}

proc f(x: owned C) {}

var arg: owned C?;
f(arg);
)""";

static const char* errorIncompatibleNilability = R"""(
─── error in file.chpl:6 [NoMatchingCandidates] ───
  Unable to resolve call to 'f': no matching candidates.
      |
    6 | f(arg);
      |
  
  The following candidate didn't match because an actual couldn't be passed to a formal:
      |
    3 | proc f(x: owned C) {}
      |        ⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺
      |
  The formal 'x' expects a value of type 'owned C', but the actual was a value of type 'owned C?'.
      |
    6 | f(arg);
      |   ⎺⎺⎺
      |
  The formal expects a non-nilable class, but the actual is nilable.
)""";


static const char* progTupleSize = R"""(
proc f(x: (?, ?, ?)) {}

f((1, 2));
)""";

static const char* errorTupleSize = R"""(
─── error in file.chpl:3 [NoMatchingCandidates] ───
  Unable to resolve call to 'f': no matching candidates.
      |
    3 | f((1, 2));
      |
  
  The following candidate didn't match because an actual couldn't be passed to a formal:
      |
    1 | proc f(x: (?, ?, ?)) {}
      |        ⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺
      |
  The formal 'x' expects a value of type '(?, ?, ?)', but the actual was a value of type '(int(64), int(64))'.
      |
    3 | f((1, 2));
      |   ⎺⎺⎺⎺⎺⎺
      |
  A tuple with 2 elements cannot be passed to a tuple formal with 3 elements.
)""";

static const char* progStarVsNoStar = R"""(
proc f(in x: 3*real) {}

f((1.0, 1.0, true));
)""";

static const char* errorStarVsNotStar = R"""(
─── error in file.chpl:3 [NoMatchingCandidates] ───
  Unable to resolve call to 'f': no matching candidates.
      |
    3 | f((1.0, 1.0, true));
      |
  
  The following candidate didn't match because an actual couldn't be passed to a formal:
      |
    1 | proc f(in x: 3*real) {}
      |        ⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺
      |
  The formal 'x' expects a value of type '(real(64), real(64), real(64))', but the actual was a value of type '(real(64), real(64), bool)'.
      |
    3 | f((1.0, 1.0, true));
      |   ⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺
      |
  A formal that is a star tuple cannot accept an actual actual that is not.
)""";

static const char* progVarArgMismatch = R"""(
proc f(param numArgs: int, nums: int...numArgs) {

}
f(numArgs=3, 1, 2);
)""";

static const char* errorVarArgMismatch = R"""(
─── error in file.chpl:4 [NoMatchingCandidates] ───
  Unable to resolve call to 'f': no matching candidates.
      |
    4 | f(numArgs=3, 1, 2);
      |
  
  The following candidate didn't match because the number of varargs was incorrect:
      |
    1 | proc f(param numArgs: int, nums: int...numArgs) {
    2 | 
    3 | }
      |
)""";

static const char* progWhereClauseFalse = R"""(
proc f(type t) where t == int {}

f(string);
)""";

static const char* errorWhereClauseFalse = R"""(
─── error in file.chpl:3 [NoMatchingCandidates] ───
  Unable to resolve call to 'f': no matching candidates.
      |
    3 | f(string);
      |
  
  The following candidate didn't match because the 'where' clause evaluated to 'false':
      |
    1 | proc f(type t) where t == int {}
      |
)""";

static const char* progBasic = R"""(
proc f(x: int) {}

f(a=42);
)""";

static const char* errorBasic = R"""(
─── error in file.chpl:3 [NoMatchingCandidates] ───
  Unable to resolve call to 'f': no matching candidates.
      |
    3 | f(a=42);
      |
  
  The following candidate didn't match:
      |
    1 | proc f(x: int) {}
      |
)""";

static const char* progOther = R"""(
record R {
    var x: int;
}

var r: R;
r.x("hello");
)""";

static const char* errorOther = R"""(
─── error in file.chpl:6 [NoMatchingCandidates] ───
  Unable to resolve call to 'this': no matching candidates.
      |
    6 | r.x("hello");
      |
)""";

static const char* progManyCandidates = R"""(
proc f(x: int(8)) {}
proc f(x: int(16)) {}
proc f(x: int(32)) {}
proc f(x: int(64)) {}
proc f(x: uint(8)) {}
proc f(x: uint(16)) {}
proc f(x: uint(32)) {}
proc f(x: uint(64)) {}

f("hello");
)""";

static const char* errorManyCandidates = R"""(
─── error in file.chpl:10 [NoMatchingCandidates] ───
  Unable to resolve call to 'f': no matching candidates.
       |
    10 | f("hello");
       |
  
  The following candidate didn't match because an actual couldn't be passed to a formal:
      |
    1 | proc f(x: int(8)) {}
      |        ⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺
      |
  The formal 'x' expects a value of type 'int(8)', but the actual was a param of type 'string'.
       |
    10 | f("hello");
       |   ⎺⎺⎺⎺⎺⎺⎺
       |
  
  The following candidate didn't match because an actual couldn't be passed to a formal:
      |
    2 | proc f(x: int(16)) {}
      |        ⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺
      |
  The formal 'x' expects a value of type 'int(16)', but the actual was a param of type 'string'.
       |
    10 | f("hello");
       |   ⎺⎺⎺⎺⎺⎺⎺
       |
  
  Omitting 6 more candidates that didn't match.
)""";

static void testResolverError(const char* program, const char* error) {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  while (*error == '\n') error++;
  while (*program == '\n') program++;

  auto path = UniqueString::get(context, "file.chpl");
  setFileText(context, path, program);
  auto& parseResult = parseFileToBuilderResult(context, path, UniqueString());
  auto mod = parseResult.singleModule();
  auto resolutionResult = resolveModule(context, mod->id());

  assert(guard.numErrors() == 1);
  assert(guard.error(0)->type() == ErrorType::NoMatchingCandidates);

  std::ostringstream oss;
  ErrorWriter detailedWriter(context, oss, ErrorWriter::DETAILED, /* useColor */ false);
  guard.error(0)->write(detailedWriter);

  printf("Expected error:\n%s\n", error);
  printf("Actual error:\n%s\n", oss.str().c_str());
  assert(oss.str() == error);

  guard.clearErrors();
}

int main() {
  testResolverError(progExactMatch, errorExactMatch);
  testResolverError(progExpectedSubType, errorExpectedSubType);
  testResolverError(progIncompatibleMgr, errorIncompatibleMgr);
  testResolverError(progIncompatibleNilability, errorIncompatibleNilability);
  testResolverError(progTupleSize, errorTupleSize);
  testResolverError(progStarVsNoStar, errorStarVsNotStar);
  testResolverError(progVarArgMismatch, errorVarArgMismatch);
  testResolverError(progWhereClauseFalse, errorWhereClauseFalse);
  testResolverError(progBasic, errorBasic);
  testResolverError(progOther, errorOther);
  testResolverError(progManyCandidates, errorManyCandidates);

  return 0;
}
