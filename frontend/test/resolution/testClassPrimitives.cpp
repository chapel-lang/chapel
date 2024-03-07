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
#include "chpl/resolution/scope-queries.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

static constexpr bool testType = true;
static constexpr bool testExact = false;

static void testPrimitive(std::string primitive, std::vector<std::tuple<const char*, const char*, bool>> args) {
  Context ctx;
  auto context = &ctx;
  ErrorGuard guard(context);

  std::stringstream ps;
  std::vector<std::string> variables;
  std::vector<bool> expectedValidities;

  ps << "class C {}" << std::endl;

  int counter = 0;
  int expectedErrorCount = 0;
  for (auto triple : args) {
    const char* expr = std::get<0>(triple);
    const char* expectedType = std::get<1>(triple);
    bool callDotType = std::get<2>(triple);

    std::string variableName = std::string("x") + std::to_string(counter++);
    variables.push_back(variableName);
    expectedValidities.push_back(expectedType != nullptr);

    if (expectedType != nullptr) {
      ps << "param " << variableName << " = " << "__primitive(\"" << primitive << "\", " << expr << ")";
      if (callDotType) ps << ".type ";
      ps << "== " << expectedType << ";" << std::endl;
    } else {
      expectedErrorCount += 1;

      ps << "type " << variableName << " = " << "__primitive(\"" << primitive << "\", " << expr << ")";
      if (callDotType) ps << ".type";
      ps << ";" << std::endl;
    }
  }

  std::cout << "--- program ---" << std::endl;
  std::cout << ps.str() << std::endl;

  auto varTypes = resolveTypesOfVariables(context, ps.str(), variables);

  for (size_t i = 0; i < varTypes.size(); i++) {
    if (expectedValidities[i]) {
      assert(varTypes.at(variables[i]).isParamTrue());
    } else {
      assert(varTypes.at(variables[i]).isErroneousType());
    }
  }

  for (auto& error : guard.errors()) {
    assert(error->type() == ErrorType::InvalidClassCast);
  }

  assert(guard.realizeErrors() == expectedErrorCount);
}

static void test1() {
  testPrimitive("to nilable class", {
    { "new owned C()", "owned C?", testType },
    { "new owned C?()", "owned C?", testType },
    { "new shared C()", "shared C?", testType },
    { "new shared C?()", "shared C?", testType },
    { "new unmanaged C()", "unmanaged C?", testType },
    { "new unmanaged C?()", "unmanaged C?", testType },

    { "owned C", "owned C?", testExact },
    { "owned C?", "owned C?", testExact },
    { "shared C", "shared C?", testExact },
    { "shared C?", "shared C?", testExact },
    { "unmanaged C", "unmanaged C?", testExact },
    { "unmanaged C?", "unmanaged C?", testExact },

    { "owned class", "owned class?", testExact },
    { "owned class", "owned class?", testExact },
    { "shared class", "shared class?", testExact },
    { "shared class", "shared class?", testExact },
    { "unmanaged class", "unmanaged class?", testExact },
    { "unmanaged class", "unmanaged class?", testExact },

    { "42", "int(64)", testType },
    { "int(64)", "int(64)", testExact },
    { "\"hello\"", "string", testType },
    { "string", "string", testExact },
  });
}

static void test2() {
  testPrimitive("to borrowed class", {
    { "new shared C()", "borrowed C", testType },
    { "new shared C?()", "borrowed C?", testType },
    { "new owned C()", "borrowed C" , testType },
    { "new owned C?()", "borrowed C?", testType },
    { "new unmanaged C()", "borrowed C", testType },
    { "new unmanaged C?()", "borrowed C?", testType },

    { "shared C", "borrowed C", testExact },
    { "shared C?", "borrowed C?", testExact },
    { "owned C", "borrowed C" , testExact },
    { "owned C?", "borrowed C?", testExact },
    { "unmanaged C", "borrowed C", testExact },
    { "unmanaged C?", "borrowed C?", testExact },

    { "shared class", "borrowed class", testExact },
    { "shared class?", "borrowed class?", testExact },
    { "owned class", "borrowed class" , testExact },
    { "owned class?", "borrowed class?", testExact },
    { "unmanaged class", "borrowed class", testExact },
    { "unmanaged class?", "borrowed class?", testExact },

    { "42", "int(64)", testType },
    { "int(64)", "int(64)", testExact },
    { "\"hello\"", "string", testType },
    { "string", "string", testExact },
  });
}

static void test3() {
  testPrimitive("to unmanaged class", {
    { "new shared C()", "unmanaged C", testType },
    { "new shared C?()", "unmanaged C?", testType },
    { "new owned C()", "unmanaged C" , testType },
    { "new owned C?()", "unmanaged C?", testType },
    { "new unmanaged C()", "unmanaged C", testType },
    { "new unmanaged C?()", "unmanaged C?", testType },

    { "shared C", "unmanaged C", testExact },
    { "shared C?", "unmanaged C?", testExact },
    { "owned C", "unmanaged C" , testExact },
    { "owned C?", "unmanaged C?", testExact },
    { "unmanaged C", "unmanaged C", testExact },
    { "unmanaged C?", "unmanaged C?", testExact },

    { "shared class", "unmanaged class", testExact },
    { "shared class?", "unmanaged class?", testExact },
    { "owned class", "unmanaged class" , testExact },
    { "owned class?", "unmanaged class?", testExact },
    { "unmanaged class", "unmanaged class", testExact },
    { "unmanaged class?", "unmanaged class?", testExact },

    { "42", "int(64)", testType },
    { "int(64)", "int(64)", testExact },
    { "\"hello\"", "string", testType },
    { "string", "string", testExact },
  });
}

static void test4() {
  testPrimitive("to nilable class from type", {
    { "new owned C()", nullptr, testType },
    { "new owned C?()", nullptr, testType },
    { "new shared C()", nullptr, testType },
    { "new shared C?()", nullptr, testType },
    { "new unmanaged C()", nullptr, testType },
    { "new unmanaged C?()", nullptr, testType },

    { "owned C", "owned C?", testExact },
    { "owned C?", "owned C?", testExact },
    { "shared C", "shared C?", testExact },
    { "shared C?", "shared C?", testExact },
    { "unmanaged C", "unmanaged C?", testExact },
    { "unmanaged C?", "unmanaged C?", testExact },

    { "owned class", "owned class?", testExact },
    { "owned class", "owned class?", testExact },
    { "shared class", "shared class?", testExact },
    { "shared class", "shared class?", testExact },
    { "unmanaged class", "unmanaged class?", testExact },
    { "unmanaged class", "unmanaged class?", testExact },

    { "42", nullptr, testType },
    { "int(64)", nullptr, testExact },
    { "\"hello\"", nullptr, testType },
    { "string", nullptr, testExact },
  });
}

static void test5() {
  testPrimitive("to borrowed class from unknown", {
    { "new shared C()", nullptr, testType },
    { "new shared C?()", nullptr, testType },
    { "new owned C()", nullptr , testType },
    { "new owned C?()", nullptr, testType },
    { "new unmanaged C()", nullptr, testType },
    { "new unmanaged C?()", nullptr, testType },

    { "shared C", "borrowed C", testExact },
    { "shared C?", "borrowed C?", testExact },
    { "owned C", "borrowed C" , testExact },
    { "owned C?", "borrowed C?", testExact },
    { "unmanaged C", "borrowed C", testExact },
    { "unmanaged C?", "borrowed C?", testExact },

    { "shared class", "borrowed class", testExact },
    { "shared class?", "borrowed class?", testExact },
    { "owned class", "borrowed class" , testExact },
    { "owned class?", "borrowed class?", testExact },
    { "unmanaged class", "borrowed class", testExact },
    { "unmanaged class?", "borrowed class?", testExact },

    { "42", nullptr, testType },
    { "int(64)", nullptr, testExact },
    { "\"hello\"", nullptr, testType },
    { "string", nullptr, testExact },
  });
}

static void test6() {
  testPrimitive("to unmanaged class from unknown", {
    { "new shared C()", nullptr, testType },
    { "new shared C?()", nullptr, testType },
    { "new owned C()", nullptr , testType },
    { "new owned C?()", nullptr, testType },
    { "new unmanaged C()", nullptr, testType },
    { "new unmanaged C?()", nullptr, testType },

    { "shared C", "unmanaged C", testExact },
    { "shared C?", "unmanaged C?", testExact },
    { "owned C", "unmanaged C" , testExact },
    { "owned C?", "unmanaged C?", testExact },
    { "unmanaged C", "unmanaged C", testExact },
    { "unmanaged C?", "unmanaged C?", testExact },

    { "shared class", "unmanaged class", testExact },
    { "shared class?", "unmanaged class?", testExact },
    { "owned class", "unmanaged class" , testExact },
    { "owned class?", "unmanaged class?", testExact },
    { "unmanaged class", "unmanaged class", testExact },
    { "unmanaged class?", "unmanaged class?", testExact },

    { "42", nullptr, testType },
    { "int(64)", nullptr, testExact },
    { "\"hello\"", nullptr, testType },
    { "string", nullptr, testExact },
  });
}

static void test7() {
  testPrimitive("to non nilable class", {
    { "new owned C()", "owned C", testType },
    { "new owned C?()", "owned C", testType },
    { "new shared C()", "shared C", testType },
    { "new shared C?()", "shared C", testType },
    { "new unmanaged C()", "unmanaged C", testType },
    { "new unmanaged C?()", "unmanaged C", testType },

    { "owned C", "owned C", testExact },
    { "owned C?", "owned C", testExact },
    { "shared C", "shared C", testExact },
    { "shared C?", "shared C", testExact },
    { "unmanaged C", "unmanaged C", testExact },
    { "unmanaged C?", "unmanaged C", testExact },

    { "owned class", "owned class", testExact },
    { "owned class", "owned class", testExact },
    { "shared class", "shared class", testExact },
    { "shared class", "shared class", testExact },
    { "unmanaged class", "unmanaged class", testExact },
    { "unmanaged class", "unmanaged class", testExact },

    { "42", "int(64)", testType },
    { "int(64)", "int(64)", testExact },
    { "\"hello\"", "string", testType },
    { "string", "string", testExact },
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
}
