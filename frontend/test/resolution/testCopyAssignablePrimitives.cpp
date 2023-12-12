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
  Context ctx;
  auto context = &ctx;
  ErrorGuard guard(context);

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
  }

  std::cout << "--- program ---" << std::endl;
  std::cout << ps.str() << std::endl;

  auto varTypes = resolveTypesOfVariables(context, ps.str(), variables);

  assert(guard.realizeErrors() == 0);

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

static constexpr int normal = 0;
static constexpr int refOnly = 1;
static constexpr int none = 2;

static void testCases(const char* preamble,
                      std::vector<std::tuple<const char*, int>> cases) {
  for (auto testCase : cases) {
    const char* expr = std::get<0>(testCase);
    int kind = std::get<1>(testCase);
    switch (kind) {
      case normal:
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

static void test1() {
  testCases("class C {}", {
      {"owned C", none},
      {"owned C?", refOnly},
      {"shared C", normal},
      {"shared C?", normal},
  });
}

int main() {
  test1();
}
