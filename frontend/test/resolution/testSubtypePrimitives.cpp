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

static constexpr int shouldReturnTrue = 0;
static constexpr int shouldReturnFalse = 1;
static constexpr int shouldError = 2;

static void testPrimitive(std::string primitive,
                          std::vector<std::tuple<const char*, const char*, int>> args) {
  Context ctx;
  auto context = &ctx;
  ErrorGuard guard(context);

  std::stringstream ps;
  std::vector<std::string> variables;
  std::vector<int> expectedResults;

  ps << "class Parent {}" << std::endl;
  ps << "class Child : Parent {}" << std::endl;

  int counter = 0;
  int expectedErrorCount = 0;
  for (auto triple : args) {
    const char* parentType = std::get<0>(triple);
    const char* subType = std::get<1>(triple);
    int expectedResult = std::get<2>(triple);

    if (expectedResult == shouldError) expectedErrorCount += 1;

    std::string variableName = std::string("x") + std::to_string(counter++);
    variables.push_back(variableName);
    expectedResults.push_back(expectedResult);

    ps << "param " << variableName << " = " << "__primitive(\"" << primitive << "\", " << parentType;
    ps << ", " << subType << ");" << std::endl;
  }

  std::cout << "--- program ---" << std::endl;
  std::cout << ps.str() << std::endl;

  auto varTypes = resolveTypesOfVariables(context, ps.str(), variables);

  for (size_t i = 0; i < varTypes.size(); i++) {
    std::cout << "Checking " << variables[i] << std::endl;

    if (expectedResults[i] == shouldReturnTrue) {
      assert(varTypes.at(variables[i]).isParamTrue());
    } else if (expectedResults[i] == shouldReturnFalse) {
      assert(varTypes.at(variables[i]).isParamFalse());
    } else {
      assert(varTypes.at(variables[i]).isErroneousType());
    }
  }

  assert(guard.realizeErrors() == expectedErrorCount);
}

static void test1() {
  testPrimitive("is_subtype", {
    { "Parent", "Parent", shouldReturnTrue },
    { "Parent?", "Parent", shouldReturnTrue },
    { "Parent", "Parent?", shouldReturnFalse },
    { "Parent?", "Parent?", shouldReturnTrue },
    { "Parent", "owned Parent", shouldReturnTrue },
    { "Parent?", "owned Parent", shouldReturnTrue },
    { "Parent", "owned Parent?", shouldReturnFalse },
    { "Parent?", "owned Parent?", shouldReturnTrue },
    { "Parent", "shared Parent", shouldReturnTrue },
    { "Parent?", "shared Parent", shouldReturnTrue },
    { "Parent", "shared Parent?", shouldReturnFalse },
    { "Parent?", "shared Parent?", shouldReturnTrue },
    { "owned Parent", "owned Parent", shouldReturnTrue },
    { "owned Parent?", "owned Parent", shouldReturnTrue },
    { "owned Parent", "owned Parent?", shouldReturnFalse },
    { "owned Parent?", "owned Parent?", shouldReturnTrue },
    { "owned Parent", "shared Parent", shouldReturnFalse },
    { "owned Parent?", "shared Parent", shouldReturnFalse },
    { "owned Parent", "shared Parent?", shouldReturnFalse },
    { "owned Parent?", "shared Parent?", shouldReturnFalse },
    { "shared Parent", "shared Parent", shouldReturnTrue },
    { "shared Parent?", "shared Parent", shouldReturnTrue },
    { "shared Parent", "shared Parent?", shouldReturnFalse },
    { "shared Parent?", "shared Parent?", shouldReturnTrue },
    /* borrow cases follow */
    { "borrowed Parent", "owned Parent", shouldReturnTrue },
    { "borrowed Parent?", "owned Parent", shouldReturnTrue },
    { "borrowed Parent?", "owned Parent?", shouldReturnTrue },
    { "owned Parent", "borrowed Parent", shouldReturnFalse },
    { "owned Parent?", "borrowed Parent", shouldReturnFalse },
    { "owned Parent?", "borrowed Parent?", shouldReturnFalse },
    { "borrowed Parent", "shared Parent", shouldReturnTrue },
    { "borrowed Parent?", "shared Parent?", shouldReturnTrue },
    { "shared Parent", "borrowed Parent", shouldReturnFalse },
    { "shared Parent?", "borrowed Parent", shouldReturnFalse },
    { "shared Parent?", "borrowed Parent?", shouldReturnFalse },
    { "borrowed Parent", "unmanaged Parent", shouldReturnTrue },
    { "borrowed Parent?", "unmanaged Parent", shouldReturnTrue },
    { "borrowed Parent?", "unmanaged Parent?", shouldReturnTrue },
    { "unmanaged Parent", "borrowed Parent", shouldReturnFalse },
    { "unmanaged Parent?", "borrowed Parent?", shouldReturnFalse },
  });
}

static void test2() {
  testPrimitive("is_subtype", {
    { "Parent", "Child", shouldReturnTrue },
    { "Parent?", "Child", shouldReturnTrue },
    { "Parent", "Child?", shouldReturnFalse },
    { "Parent?", "Child?", shouldReturnTrue },
    { "Parent", "owned Child", shouldReturnTrue },
    { "Parent?", "owned Child", shouldReturnTrue },
    { "Parent", "owned Child?", shouldReturnFalse },
    { "Parent?", "owned Child?", shouldReturnTrue },
    { "Parent", "shared Child", shouldReturnTrue },
    { "Parent?", "shared Child", shouldReturnTrue },
    { "Parent", "shared Child?", shouldReturnFalse },
    { "Parent?", "shared Child?", shouldReturnTrue },
    { "owned Parent", "owned Child", shouldReturnTrue },
    { "owned Parent?", "owned Child", shouldReturnTrue },
    { "owned Parent", "owned Child?", shouldReturnFalse },
    { "owned Parent?", "owned Child?", shouldReturnTrue },
    { "owned Parent", "shared Child", shouldReturnFalse },
    { "owned Parent?", "shared Child", shouldReturnFalse },
    { "owned Parent", "shared Child?", shouldReturnFalse },
    { "owned Parent?", "shared Child?", shouldReturnFalse },
    { "shared Parent", "shared Child", shouldReturnTrue },
    { "shared Parent?", "shared Child", shouldReturnTrue },
    { "shared Parent", "shared Child?", shouldReturnFalse },
    { "shared Parent?", "shared Child?", shouldReturnTrue },
    /* borrow cases follow */
    { "borrowed Parent", "owned Child", shouldReturnTrue },
    { "borrowed Parent?", "owned Child", shouldReturnTrue },
    { "borrowed Parent", "owned Child?", shouldReturnFalse },
    { "borrowed Parent?", "owned Child?", shouldReturnTrue },
    { "borrowed Parent", "shared Child", shouldReturnTrue },
    { "borrowed Parent?", "shared Child", shouldReturnTrue },
    { "borrowed Parent", "shared Child?", shouldReturnFalse },
    { "borrowed Parent?", "shared Child?", shouldReturnTrue },
  });
}

static void test3() {
  testPrimitive("is_subtype", {
    { "Child", "Parent", shouldReturnFalse },
    { "Child?", "Parent", shouldReturnFalse },
    { "Child", "Parent?", shouldReturnFalse },
    { "Child?", "Parent?", shouldReturnFalse },
    { "Child", "owned Parent", shouldReturnFalse },
    { "Child?", "owned Parent", shouldReturnFalse },
    { "Child", "owned Parent?", shouldReturnFalse },
    { "Child?", "owned Parent?", shouldReturnFalse },
    { "Child", "shared Parent", shouldReturnFalse },
    { "Child?", "shared Parent", shouldReturnFalse },
    { "Child", "shared Parent?", shouldReturnFalse },
    { "Child?", "shared Parent?", shouldReturnFalse },
    { "owned Child", "owned Parent", shouldReturnFalse },
    { "owned Child?", "owned Parent", shouldReturnFalse },
    { "owned Child", "owned Parent?", shouldReturnFalse },
    { "owned Child?", "owned Parent?", shouldReturnFalse },
    { "owned Child", "shared Parent", shouldReturnFalse },
    { "owned Child?", "shared Parent", shouldReturnFalse },
    { "owned Child", "shared Parent?", shouldReturnFalse },
    { "owned Child?", "shared Parent?", shouldReturnFalse },
    { "shared Child", "shared Parent", shouldReturnFalse },
    { "shared Child?", "shared Parent", shouldReturnFalse },
    { "shared Child", "shared Parent?", shouldReturnFalse },
    { "shared Child?", "shared Parent?", shouldReturnFalse },
  });
}

static void test4() {
  testPrimitive("is_subtype", {
    { "class", "Parent", shouldReturnTrue },
    { "class?", "Parent", shouldReturnTrue },
    { "class", "Parent?", shouldReturnFalse },
    { "class?", "Parent?", shouldReturnTrue },
    { "class", "owned Parent", shouldReturnTrue },
    { "class?", "owned Parent", shouldReturnTrue },
    { "class", "owned Parent?", shouldReturnFalse },
    { "class?", "owned Parent?", shouldReturnTrue },
    { "class", "shared Parent", shouldReturnTrue },
    { "class?", "shared Parent", shouldReturnTrue },
    { "class", "shared Parent?", shouldReturnFalse },
    { "class?", "shared Parent?", shouldReturnTrue },
    { "owned class", "owned Parent", shouldReturnTrue },
    { "owned class?", "owned Parent", shouldReturnTrue },
    { "owned class", "owned Parent?", shouldReturnFalse },
    { "owned class?", "owned Parent?", shouldReturnTrue },
    { "owned class", "shared Parent", shouldReturnFalse },
    { "owned class?", "shared Parent", shouldReturnFalse },
    { "owned class", "shared Parent?", shouldReturnFalse },
    { "owned class?", "shared Parent?", shouldReturnFalse },
    { "shared class", "shared Parent", shouldReturnTrue },
    { "shared class?", "shared Parent", shouldReturnTrue },
    { "shared class", "shared Parent?", shouldReturnFalse },
    { "shared class?", "shared Parent?", shouldReturnTrue },
  });
}

static void test5() {
  testPrimitive("is_subtype", {
    { "Child", "int(64)", shouldReturnFalse },
    { "Child?", "int(64)", shouldReturnFalse },
    { "Child", "int(64)", shouldReturnFalse },
    { "Child?", "int(64)", shouldReturnFalse },
    { "Child", "int(64)", shouldReturnFalse },
    { "Child?", "int(64)", shouldReturnFalse },
    { "Child", "int(64)", shouldReturnFalse },
    { "Child?", "int(64)", shouldReturnFalse },
    { "Child", "int(64)", shouldReturnFalse },
    { "Child?", "int(64)", shouldReturnFalse },
    { "Child", "int(64)", shouldReturnFalse },
    { "Child?", "int(64)", shouldReturnFalse },
    { "owned Child", "int(64)", shouldReturnFalse },
    { "owned Child?", "int(64)", shouldReturnFalse },
    { "owned Child", "int(64)", shouldReturnFalse },
    { "owned Child?", "int(64)", shouldReturnFalse },
    { "owned Child", "int(64)", shouldReturnFalse },
    { "owned Child?", "int(64)", shouldReturnFalse },
    { "owned Child", "int(64)", shouldReturnFalse },
    { "owned Child?", "int(64)", shouldReturnFalse },
    { "shared Child", "int(64)", shouldReturnFalse },
    { "shared Child?", "int(64)", shouldReturnFalse },
    { "shared Child", "int(64)", shouldReturnFalse },
    { "shared Child?", "int(64)", shouldReturnFalse },
  });
}

static void test6() {
  testPrimitive("is_subtype", {
    { "int(64)", "Parent", shouldReturnFalse },
    { "int(64)", "Parent", shouldReturnFalse },
    { "int(64)", "Parent?", shouldReturnFalse },
    { "int(64)", "Parent?", shouldReturnFalse },
    { "int(64)", "owned Parent", shouldReturnFalse },
    { "int(64)", "owned Parent", shouldReturnFalse },
    { "int(64)", "owned Parent?", shouldReturnFalse },
    { "int(64)", "owned Parent?", shouldReturnFalse },
    { "int(64)", "shared Parent", shouldReturnFalse },
    { "int(64)", "shared Parent", shouldReturnFalse },
    { "int(64)", "shared Parent?", shouldReturnFalse },
    { "int(64)", "shared Parent?", shouldReturnFalse },
    { "int(64)", "owned Parent", shouldReturnFalse },
    { "int(64)", "owned Parent", shouldReturnFalse },
    { "int(64)", "owned Parent?", shouldReturnFalse },
    { "int(64)", "owned Parent?", shouldReturnFalse },
    { "int(64)", "shared Parent", shouldReturnFalse },
    { "int(64)", "shared Parent", shouldReturnFalse },
    { "int(64)", "shared Parent?", shouldReturnFalse },
    { "int(64)", "shared Parent?", shouldReturnFalse },
    { "int(64)", "shared Parent", shouldReturnFalse },
    { "int(64)", "shared Parent", shouldReturnFalse },
    { "int(64)", "shared Parent?", shouldReturnFalse },
    { "int(64)", "shared Parent?", shouldReturnFalse },
  });
}

static void test7() {
  testPrimitive("is_subtype", {
    { "Child", "42", shouldError },
    { "Child?", "42", shouldError },
    { "Child", "42", shouldError },
    { "Child?", "42", shouldError },
    { "Child", "42", shouldError },
    { "Child?", "42", shouldError },
    { "Child", "42", shouldError },
    { "Child?", "42", shouldError },
    { "Child", "42", shouldError },
    { "Child?", "42", shouldError },
    { "Child", "42", shouldError },
    { "Child?", "42", shouldError },
    { "owned Child", "42", shouldError },
    { "owned Child?", "42", shouldError },
    { "owned Child", "42", shouldError },
    { "owned Child?", "42", shouldError },
    { "owned Child", "42", shouldError },
    { "owned Child?", "42", shouldError },
    { "owned Child", "42", shouldError },
    { "owned Child?", "42", shouldError },
    { "shared Child", "42", shouldError },
    { "shared Child?", "42", shouldError },
    { "shared Child", "42", shouldError },
    { "shared Child?", "42", shouldError },
  });
}

static void test8() {
  testPrimitive("is_subtype", {
    { "42", "Parent", shouldError },
    { "42", "Parent", shouldError },
    { "42", "Parent?", shouldError },
    { "42", "Parent?", shouldError },
    { "42", "owned Parent", shouldError },
    { "42", "owned Parent", shouldError },
    { "42", "owned Parent?", shouldError },
    { "42", "owned Parent?", shouldError },
    { "42", "shared Parent", shouldError },
    { "42", "shared Parent", shouldError },
    { "42", "shared Parent?", shouldError },
    { "42", "shared Parent?", shouldError },
    { "42", "owned Parent", shouldError },
    { "42", "owned Parent", shouldError },
    { "42", "owned Parent?", shouldError },
    { "42", "owned Parent?", shouldError },
    { "42", "shared Parent", shouldError },
    { "42", "shared Parent", shouldError },
    { "42", "shared Parent?", shouldError },
    { "42", "shared Parent?", shouldError },
    { "42", "shared Parent", shouldError },
    { "42", "shared Parent", shouldError },
    { "42", "shared Parent?", shouldError },
    { "42", "shared Parent?", shouldError },
  });
}

static void test9() {
  testPrimitive("is_proper_subtype", {
    { "Parent", "Parent", shouldReturnFalse },
    { "Parent?", "Parent", shouldReturnTrue },
    { "Parent", "Parent?", shouldReturnFalse },
    { "Parent?", "Parent?", shouldReturnFalse },
    { "Parent", "owned Parent", shouldReturnTrue },
    { "Parent?", "owned Parent", shouldReturnTrue },
    { "Parent", "owned Parent?", shouldReturnFalse },
    { "Parent?", "owned Parent?", shouldReturnTrue },
    { "Parent", "shared Parent", shouldReturnTrue },
    { "Parent?", "shared Parent", shouldReturnTrue },
    { "Parent", "shared Parent?", shouldReturnFalse },
    { "Parent?", "shared Parent?", shouldReturnTrue },
    { "owned Parent", "owned Parent", shouldReturnFalse },
    { "owned Parent?", "owned Parent", shouldReturnTrue },
    { "owned Parent", "owned Parent?", shouldReturnFalse },
    { "owned Parent?", "owned Parent?", shouldReturnFalse },
    { "owned Parent", "shared Parent", shouldReturnFalse },
    { "owned Parent?", "shared Parent", shouldReturnFalse },
    { "owned Parent", "shared Parent?", shouldReturnFalse },
    { "owned Parent?", "shared Parent?", shouldReturnFalse },
    { "shared Parent", "shared Parent", shouldReturnFalse },
    { "shared Parent?", "shared Parent", shouldReturnTrue },
    { "shared Parent", "shared Parent?", shouldReturnFalse },
    { "shared Parent?", "shared Parent?", shouldReturnFalse },
  });
}

static void test10() {
  testPrimitive("is_proper_subtype", {
    { "Parent", "Child", shouldReturnTrue },
    { "Parent?", "Child", shouldReturnTrue },
    { "Parent", "Child?", shouldReturnFalse },
    { "Parent?", "Child?", shouldReturnTrue },
    { "Parent", "owned Child", shouldReturnTrue },
    { "Parent?", "owned Child", shouldReturnTrue },
    { "Parent", "owned Child?", shouldReturnFalse },
    { "Parent?", "owned Child?", shouldReturnTrue },
    { "Parent", "shared Child", shouldReturnTrue },
    { "Parent?", "shared Child", shouldReturnTrue },
    { "Parent", "shared Child?", shouldReturnFalse },
    { "Parent?", "shared Child?", shouldReturnTrue },
    { "owned Parent", "owned Child", shouldReturnTrue },
    { "owned Parent?", "owned Child", shouldReturnTrue },
    { "owned Parent", "owned Child?", shouldReturnFalse },
    { "owned Parent?", "owned Child?", shouldReturnTrue },
    { "owned Parent", "shared Child", shouldReturnFalse },
    { "owned Parent?", "shared Child", shouldReturnFalse },
    { "owned Parent", "shared Child?", shouldReturnFalse },
    { "owned Parent?", "shared Child?", shouldReturnFalse },
    { "shared Parent", "shared Child", shouldReturnTrue },
    { "shared Parent?", "shared Child", shouldReturnTrue },
    { "shared Parent", "shared Child?", shouldReturnFalse },
    { "shared Parent?", "shared Child?", shouldReturnTrue },
  });
}

static void test11() {
  testPrimitive("is_proper_subtype", {
    { "Child", "Parent", shouldReturnFalse },
    { "Child?", "Parent", shouldReturnFalse },
    { "Child", "Parent?", shouldReturnFalse },
    { "Child?", "Parent?", shouldReturnFalse },
    { "Child", "owned Parent", shouldReturnFalse },
    { "Child?", "owned Parent", shouldReturnFalse },
    { "Child", "owned Parent?", shouldReturnFalse },
    { "Child?", "owned Parent?", shouldReturnFalse },
    { "Child", "shared Parent", shouldReturnFalse },
    { "Child?", "shared Parent", shouldReturnFalse },
    { "Child", "shared Parent?", shouldReturnFalse },
    { "Child?", "shared Parent?", shouldReturnFalse },
    { "owned Child", "owned Parent", shouldReturnFalse },
    { "owned Child?", "owned Parent", shouldReturnFalse },
    { "owned Child", "owned Parent?", shouldReturnFalse },
    { "owned Child?", "owned Parent?", shouldReturnFalse },
    { "owned Child", "shared Parent", shouldReturnFalse },
    { "owned Child?", "shared Parent", shouldReturnFalse },
    { "owned Child", "shared Parent?", shouldReturnFalse },
    { "owned Child?", "shared Parent?", shouldReturnFalse },
    { "shared Child", "shared Parent", shouldReturnFalse },
    { "shared Child?", "shared Parent", shouldReturnFalse },
    { "shared Child", "shared Parent?", shouldReturnFalse },
    { "shared Child?", "shared Parent?", shouldReturnFalse },
  });
}

static void test12() {
  testPrimitive("is_proper_subtype", {
    { "class", "Parent", shouldReturnTrue },
    { "class?", "Parent", shouldReturnTrue },
    { "class", "Parent?", shouldReturnFalse },
    { "class?", "Parent?", shouldReturnTrue },
    { "class", "owned Parent", shouldReturnTrue },
    { "class?", "owned Parent", shouldReturnTrue },
    { "class", "owned Parent?", shouldReturnFalse },
    { "class?", "owned Parent?", shouldReturnTrue },
    { "class", "shared Parent", shouldReturnTrue },
    { "class?", "shared Parent", shouldReturnTrue },
    { "class", "shared Parent?", shouldReturnFalse },
    { "class?", "shared Parent?", shouldReturnTrue },
    { "owned class", "owned Parent", shouldReturnTrue },
    { "owned class?", "owned Parent", shouldReturnTrue },
    { "owned class", "owned Parent?", shouldReturnFalse },
    { "owned class?", "owned Parent?", shouldReturnTrue },
    { "owned class", "shared Parent", shouldReturnFalse },
    { "owned class?", "shared Parent", shouldReturnFalse },
    { "owned class", "shared Parent?", shouldReturnFalse },
    { "owned class?", "shared Parent?", shouldReturnFalse },
    { "shared class", "shared Parent", shouldReturnTrue },
    { "shared class?", "shared Parent", shouldReturnTrue },
    { "shared class", "shared Parent?", shouldReturnFalse },
    { "shared class?", "shared Parent?", shouldReturnTrue },
  });
}

static void test13() {
  testPrimitive("is_proper_subtype", {
    { "Child", "int(64)", shouldReturnFalse },
    { "Child?", "int(64)", shouldReturnFalse },
    { "Child", "int(64)", shouldReturnFalse },
    { "Child?", "int(64)", shouldReturnFalse },
    { "Child", "int(64)", shouldReturnFalse },
    { "Child?", "int(64)", shouldReturnFalse },
    { "Child", "int(64)", shouldReturnFalse },
    { "Child?", "int(64)", shouldReturnFalse },
    { "Child", "int(64)", shouldReturnFalse },
    { "Child?", "int(64)", shouldReturnFalse },
    { "Child", "int(64)", shouldReturnFalse },
    { "Child?", "int(64)", shouldReturnFalse },
    { "owned Child", "int(64)", shouldReturnFalse },
    { "owned Child?", "int(64)", shouldReturnFalse },
    { "owned Child", "int(64)", shouldReturnFalse },
    { "owned Child?", "int(64)", shouldReturnFalse },
    { "owned Child", "int(64)", shouldReturnFalse },
    { "owned Child?", "int(64)", shouldReturnFalse },
    { "owned Child", "int(64)", shouldReturnFalse },
    { "owned Child?", "int(64)", shouldReturnFalse },
    { "shared Child", "int(64)", shouldReturnFalse },
    { "shared Child?", "int(64)", shouldReturnFalse },
    { "shared Child", "int(64)", shouldReturnFalse },
    { "shared Child?", "int(64)", shouldReturnFalse },
  });
}

static void test14() {
  testPrimitive("is_proper_subtype", {
    { "int(64)", "Parent", shouldReturnFalse },
    { "int(64)", "Parent", shouldReturnFalse },
    { "int(64)", "Parent?", shouldReturnFalse },
    { "int(64)", "Parent?", shouldReturnFalse },
    { "int(64)", "owned Parent", shouldReturnFalse },
    { "int(64)", "owned Parent", shouldReturnFalse },
    { "int(64)", "owned Parent?", shouldReturnFalse },
    { "int(64)", "owned Parent?", shouldReturnFalse },
    { "int(64)", "shared Parent", shouldReturnFalse },
    { "int(64)", "shared Parent", shouldReturnFalse },
    { "int(64)", "shared Parent?", shouldReturnFalse },
    { "int(64)", "shared Parent?", shouldReturnFalse },
    { "int(64)", "owned Parent", shouldReturnFalse },
    { "int(64)", "owned Parent", shouldReturnFalse },
    { "int(64)", "owned Parent?", shouldReturnFalse },
    { "int(64)", "owned Parent?", shouldReturnFalse },
    { "int(64)", "shared Parent", shouldReturnFalse },
    { "int(64)", "shared Parent", shouldReturnFalse },
    { "int(64)", "shared Parent?", shouldReturnFalse },
    { "int(64)", "shared Parent?", shouldReturnFalse },
    { "int(64)", "shared Parent", shouldReturnFalse },
    { "int(64)", "shared Parent", shouldReturnFalse },
    { "int(64)", "shared Parent?", shouldReturnFalse },
    { "int(64)", "shared Parent?", shouldReturnFalse },
  });
}

static void test15() {
  testPrimitive("is_proper_subtype", {
    { "Child", "42", shouldError },
    { "Child?", "42", shouldError },
    { "Child", "42", shouldError },
    { "Child?", "42", shouldError },
    { "Child", "42", shouldError },
    { "Child?", "42", shouldError },
    { "Child", "42", shouldError },
    { "Child?", "42", shouldError },
    { "Child", "42", shouldError },
    { "Child?", "42", shouldError },
    { "Child", "42", shouldError },
    { "Child?", "42", shouldError },
    { "owned Child", "42", shouldError },
    { "owned Child?", "42", shouldError },
    { "owned Child", "42", shouldError },
    { "owned Child?", "42", shouldError },
    { "owned Child", "42", shouldError },
    { "owned Child?", "42", shouldError },
    { "owned Child", "42", shouldError },
    { "owned Child?", "42", shouldError },
    { "shared Child", "42", shouldError },
    { "shared Child?", "42", shouldError },
    { "shared Child", "42", shouldError },
    { "shared Child?", "42", shouldError },
  });
}

static void test16() {
  testPrimitive("is_proper_subtype", {
    { "42", "Parent", shouldError },
    { "42", "Parent", shouldError },
    { "42", "Parent?", shouldError },
    { "42", "Parent?", shouldError },
    { "42", "owned Parent", shouldError },
    { "42", "owned Parent", shouldError },
    { "42", "owned Parent?", shouldError },
    { "42", "owned Parent?", shouldError },
    { "42", "shared Parent", shouldError },
    { "42", "shared Parent", shouldError },
    { "42", "shared Parent?", shouldError },
    { "42", "shared Parent?", shouldError },
    { "42", "owned Parent", shouldError },
    { "42", "owned Parent", shouldError },
    { "42", "owned Parent?", shouldError },
    { "42", "owned Parent?", shouldError },
    { "42", "shared Parent", shouldError },
    { "42", "shared Parent", shouldError },
    { "42", "shared Parent?", shouldError },
    { "42", "shared Parent?", shouldError },
    { "42", "shared Parent", shouldError },
    { "42", "shared Parent", shouldError },
    { "42", "shared Parent?", shouldError },
    { "42", "shared Parent?", shouldError },
  });
}

// Currently, constructors for child class are unimplemented, so can't run
// these test with 'new Child()'.

static void test17() {
  testPrimitive("is_instantiation_allow_values", {
    { "Parent", "Parent", shouldReturnTrue },
    { "Parent?", "Parent", shouldReturnFalse },
    { "Parent", "Parent?", shouldReturnFalse },
    { "Parent?", "Parent?", shouldReturnTrue },
    { "Parent", "owned Parent", shouldReturnTrue },
    { "Parent?", "owned Parent", shouldReturnFalse },
    { "Parent", "owned Parent?", shouldReturnFalse },
    { "Parent?", "owned Parent?", shouldReturnTrue },
    { "Parent", "shared Parent", shouldReturnTrue },
    { "Parent?", "shared Parent", shouldReturnFalse },
    { "Parent", "shared Parent?", shouldReturnFalse },
    { "Parent?", "shared Parent?", shouldReturnTrue },
    { "owned Parent", "owned Parent", shouldReturnTrue },
    { "owned Parent?", "owned Parent", shouldReturnFalse },
    { "owned Parent", "owned Parent?", shouldReturnFalse },
    { "owned Parent?", "owned Parent?", shouldReturnTrue },
    { "owned Parent", "shared Parent", shouldReturnFalse },
    { "owned Parent?", "shared Parent", shouldReturnFalse },
    { "owned Parent", "shared Parent?", shouldReturnFalse },
    { "owned Parent?", "shared Parent?", shouldReturnFalse },
    { "shared Parent", "shared Parent", shouldReturnTrue },
    { "shared Parent?", "shared Parent", shouldReturnFalse },
    { "shared Parent", "shared Parent?", shouldReturnFalse },
    { "shared Parent?", "shared Parent?", shouldReturnTrue },
  });
}

static void test18() {
  testPrimitive("is_instantiation_allow_values", {
    { "new owned Parent()", "owned Parent", shouldReturnTrue },
    { "new owned Parent?()", "owned Parent", shouldReturnFalse },
    { "new owned Parent()", "owned Parent?", shouldReturnFalse },
    { "new owned Parent?()", "owned Parent?", shouldReturnTrue },
    { "new owned Parent()", "shared Parent", shouldReturnFalse },
    { "new owned Parent?()", "shared Parent", shouldReturnFalse },
    { "new owned Parent()", "shared Parent?", shouldReturnFalse },
    { "new owned Parent?()", "shared Parent?", shouldReturnFalse },
    { "new shared Parent()", "shared Parent", shouldReturnTrue },
    { "new shared Parent?()", "shared Parent", shouldReturnFalse },
    { "new shared Parent()", "shared Parent?", shouldReturnFalse },
    { "new shared Parent?()", "shared Parent?", shouldReturnTrue },
  });
}

static void test19() {
  testPrimitive("is_instantiation_allow_values", {
    { "Parent", "new owned Parent()", shouldReturnTrue },
    { "Parent?", "new owned Parent()", shouldReturnFalse },
    { "Parent", "new owned Parent?()", shouldReturnFalse },
    { "Parent?", "new owned Parent?()", shouldReturnTrue },
    { "Parent", "new shared Parent()", shouldReturnTrue },
    { "Parent?", "new shared Parent()", shouldReturnFalse },
    { "Parent", "new shared Parent?()", shouldReturnFalse },
    { "Parent?", "new shared Parent?()", shouldReturnTrue },
    { "owned Parent", "new owned Parent()", shouldReturnTrue },
    { "owned Parent?", "new owned Parent()", shouldReturnFalse },
    { "owned Parent", "new owned Parent?()", shouldReturnFalse },
    { "owned Parent?", "new owned Parent?()", shouldReturnTrue },
    { "owned Parent", "new shared Parent()", shouldReturnFalse },
    { "owned Parent?", "new shared Parent()", shouldReturnFalse },
    { "owned Parent", "new shared Parent?()", shouldReturnFalse },
    { "owned Parent?", "new shared Parent?()", shouldReturnFalse },
    { "shared Parent", "new shared Parent()", shouldReturnTrue },
    { "shared Parent?", "new shared Parent()", shouldReturnFalse },
    { "shared Parent", "new shared Parent?()", shouldReturnFalse },
    { "shared Parent?", "new shared Parent?()", shouldReturnTrue },
  });
}

static void test20() {
  testPrimitive("is_instantiation_allow_values", {
    { "new owned Parent()", "new owned Parent()", shouldError },
    { "new owned Parent?()", "new owned Parent()", shouldError },
    { "new owned Parent()", "new owned Parent?()", shouldError },
    { "new owned Parent?()", "new owned Parent?()", shouldError },
    { "new owned Parent()", "new shared Parent()", shouldError },
    { "new owned Parent?()", "new shared Parent()", shouldError },
    { "new owned Parent()", "new shared Parent?()", shouldError },
    { "new owned Parent?()", "new shared Parent?()", shouldError },
    { "new shared Parent()", "new shared Parent()", shouldError },
    { "new shared Parent?()", "new shared Parent()", shouldError },
    { "new shared Parent()", "new shared Parent?()", shouldError },
    { "new shared Parent?()", "new shared Parent?()", shouldError },
  });
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  test10();
  test11();
  test12();
  test13();
  test14();
  test15();
  test16();
  test17();
  test18();
  test19();
  test20();
}
