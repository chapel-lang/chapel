/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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
#include "chpl/resolution/can-pass.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

static constexpr bool testType = true;
static constexpr bool testExact = !testType;

// args: Vector of argument tuples like (prim, expr, expectedValue,
// callDotType). callDotType means expr will have '.type' appended.
// preamble: Code to insert before the generated test code, for declarations.
static void testPrimitive(
    std::string preamble,
    std::vector<std::tuple<const char*, const char*, bool, bool>> args) {
  auto context = buildStdContext();
  /* ErrorGuard guard(context); */

  std::stringstream ps;
  std::vector<std::string> variables;
  std::vector<bool> expectedValues;

  ps << preamble << std::endl;

  int counter = 0;
  for (auto arg : args) {
    const char* prim = std::get<0>(arg);
    const char* expr = std::get<1>(arg);
    bool expectedValue = std::get<2>(arg);
    bool callDotType = std::get<3>(arg);

    std::string variableName = std::string("x") + std::to_string(counter++);
    variables.push_back(variableName);
    expectedValues.push_back(expectedValue);

    // Generate prim call like:
    // param x3 = __primitive("PRIM_IS_CONST_ASSIGNABLE", (myFoo).type);
    ps << "param " << variableName << " = " << "__primitive(\"" << prim << "\", (" << expr << ")";
    if (callDotType) ps << ".type ";
    ps << ");" << std::endl;

    // TODO: test the implementation of the primitives
    // (getCopyOrAssignableInfo) directly
  }

  std::cout << "--- program ---" << std::endl;
  std::cout << ps.str() << std::endl;

  auto varTypes = resolveTypesOfVariables(context, ps.str(), variables);

  /* assert(guard.realizeErrors() == 0); */

  for (size_t i = 0; i < varTypes.size(); i++) {
    if (expectedValues[i]) {
      assert(varTypes.at(variables[i]).isParamTrue());
    } else {
      assert(varTypes.at(variables[i]).isParamFalse());
    }
  }
}

static void testExprCopyAndAssignability(const char* preamble, const char* expr,
                                         bool expectRefCopyable,
                                         bool expectConstCopyable,
                                         bool expectRefAssignable,
                                         bool expectConstAssignable) {
  testPrimitive(preamble,
      {{"is copyable type", expr, expectRefCopyable, testExact},
       {"is const copyable type", expr, expectConstCopyable, testExact},
       {"is assignable type", expr, expectRefAssignable, testExact},
       {"is const assignable type", expr, expectConstAssignable, testExact}});
}

static constexpr int all = 0;
static constexpr int refOnly = 1;
static constexpr int none = 2;

static void testCases(const char* preamble,
                      std::vector<std::tuple<const char*, int>> cases) {
  for (auto testCase : cases) {
    const char* expr = std::get<0>(testCase);
    int kind = std::get<1>(testCase);
    switch (kind) {
      case all:
        testExprCopyAndAssignability(preamble, expr, true, true, true, true);
        break;
      case refOnly:
        testExprCopyAndAssignability(preamble, expr, true, false, true, false);
        break;
      case none:
        testExprCopyAndAssignability(preamble, expr, false, false, false,
                                     false);
        break;
      default:
        assert(false && "unhandled case, bug in test");
        break;
    }
  }
}


// Class types with different management
static void test1() {
  testCases(
      R"""(
      class C { }
      )""",
      {
          {"owned C", none},
          {"owned C?", refOnly},
          {"shared C", all},
          {"shared C?", all},
          {"borrowed C", all},
          {"borrowed C?", all},
          {"unmanaged C", all},
          {"unmanaged C?", all},
      });
}

// Record types with different contents
static void test2() {
  testCases(
      R"""(
      class C { }

      record PlainRecord { }

      record RecordWithNilableOwned {
        var x: owned C?;
      }

      record RecordWithNonNilableOwned {
        var x: owned C;
      }

      record RecordWithNilableOwnedIndirect {
        var x: RecordWithNilableOwned;
      }

      record RecordWithNonNilableOwnedIndirect {
        var x: RecordWithNonNilableOwned;
      }

      record CustomRecordWithNilableOwned {
        var x: owned C?;
        proc init=(other) {
          this.x = new owned C();
        }
      }
      operator =(ref lhs: CustomRecordWithNilableOwned,
              const ref rhs: CustomRecordWithNilableOwned) {
        lhs.x = new owned C();
      }
      record CustomRecordWithNonNilableOwned {
        var x: owned C;
        proc init=(other) {
          this.x = new owned C();
        }
      }
      operator =(ref lhs: CustomRecordWithNonNilableOwned,
              const ref rhs: CustomRecordWithNonNilableOwned) {
        lhs.x = new owned C();
      }

      record CustomRecordWithNilableOwnedOp {
        var x: owned C?;
        proc init=(other) {
          this.x = new owned C();
        }
      }
      operator CustomRecordWithNilableOwnedOp.=(
              ref lhs: CustomRecordWithNilableOwnedOp,
              const ref rhs: CustomRecordWithNilableOwnedOp) {
        lhs.x = new owned C();
      }

      record CustomRecordWithNonNilableOwnedOp {
        var x: owned C;
        proc init=(other) {
          this.x = new owned C();
        }
      }
      operator CustomRecordWithNonNilableOwnedOp.=(
              ref lhs: CustomRecordWithNonNilableOwnedOp,
              const ref rhs: CustomRecordWithNonNilableOwnedOp) {
        lhs.x = new owned C();
      }
      )""",
      {
          {"PlainRecord", all},
          {"RecordWithNilableOwned", refOnly},
          {"RecordWithNonNilableOwned", none},
          {"RecordWithNilableOwnedIndirect", refOnly},
          {"RecordWithNonNilableOwnedIndirect", none},
          {"CustomRecordWithNilableOwned", all},
          {"CustomRecordWithNonNilableOwned", all},
          {"CustomRecordWithNilableOwnedOp", all},
          {"CustomRecordWithNonNilableOwnedOp", all},
      });
}

// Tuple and enum types
static void test3() {
  testCases(
      R"""(
      class C { }
      enum E { x }
      )""",
      {
          {"E", all},
          {"(int, string)", all},
          {"(int, owned C)", none},
          {"(int, owned C?)", refOnly},
      });
}

// Plain old primitive types
static void test4() {
  testCases(
      R"""(
      )""",
      {
          {"int", all},
          {"int(32)", all},
          {"string", all},
          {"real", all},
          {"bool", all},
          {"complex", all},
      });
}

// Atomic type
/* static void test5() { */
/*   testCases( */
/*       R"""( */
/*       )""", */
/*       { */
/*           {"atomic int", all}, */
/*       }); */
/* } */

// Sync/single types (special case)
/* static void test6() { */
/*   testCases( */
/*       R"""( */
/*       )""", */
/*       { */
/*           {"sync string", all}, */
/*           {"single string", all}, */
/*       }); */
/* } */

// Generic types
static void test7() {
  testCases(
      R"""(
      record GenericRecord {
        type t;
        var x : t;
      }

      record GenericRecordWithDefault {
        type t = int;
        var x : t;
      }
      )""",
      {
          {"GenericRecord", none},
          {"GenericRecord(int)", all},
          {"GenericRecordWithDefault", all},
          {"GenericRecordWithDefault(real)", all},
          {"integral", all}, // generic but not composite
      });
}

int main() {
  test1();
  test2();
  test3();
  test4();
  // can't test since we don't resolve atomics yet
  /* test5(); */
  // can't test since we don't resolve syncs and singles yet
  /* test6(); */
  test7();
}
