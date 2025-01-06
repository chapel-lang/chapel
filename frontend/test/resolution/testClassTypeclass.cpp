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

struct ArgumentType {
  enum class Nilability {
    Nilable, NotNilable
  };

  std::string management;
  Nilability nilability;

  ArgumentType(std::string management, Nilability nilability)
    : management(std::move(management)), nilability(nilability) {}

  std::string toDecl(std::string variableName, std::string className) {
    std::stringstream ss;
    ss << "var " << variableName << ": " << management << " " << className;
    if (nilability == Nilability::Nilable) {
      ss << "?";
    }
    ss << " = new " << management << " " << className << "();" << std::endl;
    return ss.str();
  }
};

static const std::vector<std::string> classNames = { "C1" , "C2", "C3" };

static ArgumentType nilable(std::string management) {
  return ArgumentType(std::move(management), ArgumentType::Nilability::Nilable);
}

static ArgumentType nonNil(std::string management) {
  return ArgumentType(std::move(management), ArgumentType::Nilability::NotNilable);
}

bool shouldPass = true;
bool shouldNotPass = false;

static void testProgram(std::string intent, std::string management, std::vector<std::pair<ArgumentType, bool>> args) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  // Build the program.
  std::stringstream ps;
  std::vector<std::string> variables;
  std::vector<std::string> results;
  int counter = 0;

  // Declare all the classes
  for (auto& className : classNames) {
    ps << "class " << className << " {}" << std::endl;
  }

  // Declare the function
  ps << "proc f(" << intent << " arg: " << management << ") { return true; }" << std::endl;

  // Declare all the variables
  for (auto& className : classNames) {
    for (auto& argument : args) {
      std::string variableName = std::string("x") + std::to_string(counter++);
      ps << argument.first.toDecl(variableName, className);
      variables.push_back(std::move(variableName));
    }
  }

  // Create all the function calls
  for (auto variable : variables) {
    std::string resultName = variable + "Result";
    ps << "var " << resultName << " = f(" << variable << ");" << std::endl;
    results.push_back(std::move(resultName));
  }

  std::cout << "--- program ---" << std::endl;
  std::cout << ps.str() << std::endl;

  auto varTypes = resolveTypesOfVariables(context, ps.str(), results);
  int expectedErrorCount = 0;
  for (size_t i = 0; i < variables.size(); i++) {
    std::cout << "Testing: " << variables[i] << std::endl;
    bool expectSuccess = args[i % args.size()].second;
    auto& type = varTypes[results[i]];
    if (expectSuccess) {
      assert(type.type());
      assert(type.type()->isBoolType());
    } else {
      assert(type.isErroneousType());
      expectedErrorCount++;
    }
  }
  for (auto varType : varTypes) {
  }


  assert(guard.realizeErrors() == expectedErrorCount);
}

// For just a basic 'class', every management type should pass, as long
// as it's not nil.
static void test1() {
  testProgram("ref", "class", {
    { nonNil("owned"), shouldPass },
    { nonNil("shared"), shouldPass },
    { nonNil("unmanaged"), shouldPass },

    { nilable("owned"), shouldNotPass },
    { nilable("shared"), shouldNotPass },
    { nilable("unmanaged"), shouldNotPass },
  });
}

// If the class is nilable, only nilable things should pass without conversion.
static void test2() {
  testProgram("ref", "class?", {
    { nonNil("owned"), shouldNotPass },
    { nonNil("shared"), shouldNotPass },
    { nonNil("unmanaged"), shouldNotPass },

    { nilable("owned"), shouldPass },
    { nilable("shared"), shouldPass },
    { nilable("unmanaged"), shouldPass },
  });
}

static void test3() {
  testProgram("ref", "owned class", {
    { nonNil("owned"), shouldPass },
    { nonNil("shared"), shouldNotPass },
    { nonNil("unmanaged"), shouldNotPass },

    { nilable("owned"), shouldNotPass },
    { nilable("shared"), shouldNotPass },
    { nilable("unmanaged"), shouldNotPass },
  });
}

static void test4() {
  testProgram("ref", "owned class?", {
    { nonNil("owned"), shouldNotPass },
    { nonNil("shared"), shouldNotPass },
    { nonNil("unmanaged"), shouldNotPass },

    { nilable("owned"), shouldPass },
    { nilable("shared"), shouldNotPass },
    { nilable("unmanaged"), shouldNotPass },
  });
}

static void test5() {
  testProgram("ref", "shared class", {
    { nonNil("owned"), shouldNotPass },
    { nonNil("shared"), shouldPass },
    { nonNil("unmanaged"), shouldNotPass },

    { nilable("owned"), shouldNotPass },
    { nilable("shared"), shouldNotPass },
    { nilable("unmanaged"), shouldNotPass },
  });
}

static void test6() {
  testProgram("ref", "shared class?", {
    { nonNil("owned"), shouldNotPass },
    { nonNil("shared"), shouldNotPass },
    { nonNil("unmanaged"), shouldNotPass },

    { nilable("owned"), shouldNotPass },
    { nilable("shared"), shouldPass },
    { nilable("unmanaged"), shouldNotPass },
  });
}

static void test7() {
  testProgram("ref", "unmanaged class", {
    { nonNil("owned"), shouldNotPass },
    { nonNil("shared"), shouldNotPass },
    { nonNil("unmanaged"), shouldPass },

    { nilable("owned"), shouldNotPass },
    { nilable("shared"), shouldNotPass },
    { nilable("unmanaged"), shouldNotPass },
  });
}

static void test8() {
  testProgram("ref", "unmanaged class?", {
    { nonNil("owned"), shouldNotPass },
    { nonNil("shared"), shouldNotPass },
    { nonNil("unmanaged"), shouldNotPass },

    { nilable("owned"), shouldNotPass },
    { nilable("shared"), shouldNotPass },
    { nilable("unmanaged"), shouldPass },
  });
}

static void test9() {
  testProgram("ref", "owned", {
    { nonNil("owned"), shouldPass },
    { nonNil("shared"), shouldNotPass },
    { nonNil("unmanaged"), shouldNotPass },

    { nilable("owned"), shouldPass },
    { nilable("shared"), shouldNotPass },
    { nilable("unmanaged"), shouldNotPass },
  });
}

static void test10() {
  testProgram("ref", "shared", {
    { nonNil("owned"), shouldNotPass },
    { nonNil("shared"), shouldPass },
    { nonNil("unmanaged"), shouldNotPass },

    { nilable("owned"), shouldNotPass },
    { nilable("shared"), shouldPass },
    { nilable("unmanaged"), shouldNotPass },
  });
}

// In the following tests, relax the intent to the default one, allowing
// conversions. This means the type classes should behave as before, EXCEPT
// non-nilable classes will be passable to nilable formals.

static const char* defaultIntent = "";

static void test11() {
  testProgram(defaultIntent, "class", {
    { nonNil("owned"), shouldPass },
    { nonNil("shared"), shouldPass },
    { nonNil("unmanaged"), shouldPass },

    { nilable("owned"), shouldNotPass },
    { nilable("shared"), shouldNotPass },
    { nilable("unmanaged"), shouldNotPass },
  });
}

// If the class is nilable, only nilable things should pass without conversion.
// For the default intent (const ref), conversions are disallowed.
static void test12() {
  testProgram(defaultIntent, "class?", {
    { nonNil("owned"), shouldNotPass },
    { nonNil("shared"), shouldNotPass },
    { nonNil("unmanaged"), shouldPass },
    /* default intent is CONST_IN for unmanaged, not const ref, so coercion allowed. */

    { nilable("owned"), shouldPass },
    { nilable("shared"), shouldPass },
    { nilable("unmanaged"), shouldPass },
  });
}

static void test13() {
  testProgram(defaultIntent, "owned class", {
    { nonNil("owned"), shouldPass },
    { nonNil("shared"), shouldNotPass },
    { nonNil("unmanaged"), shouldNotPass },

    { nilable("owned"), shouldNotPass },
    { nilable("shared"), shouldNotPass },
    { nilable("unmanaged"), shouldNotPass },
  });
}

static void test14() {
  testProgram(defaultIntent, "owned class?", {
    { nonNil("owned"), shouldNotPass },
    { nonNil("shared"), shouldNotPass },
    { nonNil("unmanaged"), shouldNotPass },

    { nilable("owned"), shouldPass },
    { nilable("shared"), shouldNotPass },
    { nilable("unmanaged"), shouldNotPass },
  });
}

static void test15() {
  testProgram(defaultIntent, "shared class", {
    { nonNil("owned"), shouldNotPass },
    { nonNil("shared"), shouldPass },
    { nonNil("unmanaged"), shouldNotPass },

    { nilable("owned"), shouldNotPass },
    { nilable("shared"), shouldNotPass },
    { nilable("unmanaged"), shouldNotPass },
  });
}

static void test16() {
  testProgram(defaultIntent, "shared class?", {
    { nonNil("owned"), shouldNotPass },
    { nonNil("shared"), shouldNotPass },
    { nonNil("unmanaged"), shouldNotPass },

    { nilable("owned"), shouldNotPass },
    { nilable("shared"), shouldPass },
    { nilable("unmanaged"), shouldNotPass },
  });
}

static void test17() {
  testProgram(defaultIntent, "unmanaged class", {
    { nonNil("owned"), shouldNotPass },
    { nonNil("shared"), shouldNotPass },
    { nonNil("unmanaged"), shouldPass },

    { nilable("owned"), shouldNotPass },
    { nilable("shared"), shouldNotPass },
    { nilable("unmanaged"), shouldNotPass },
  });
}

static void test18() {
  testProgram(defaultIntent, "unmanaged class?", {
    { nonNil("owned"), shouldNotPass },
    { nonNil("shared"), shouldNotPass },
    { nonNil("unmanaged"), shouldPass },
    /* default intent is CONST_IN for unmanaged, not const ref, so coercion allowed. */

    { nilable("owned"), shouldNotPass },
    { nilable("shared"), shouldNotPass },
    { nilable("unmanaged"), shouldPass },
  });
}

static void test19() {
  testProgram(defaultIntent, "owned", {
    { nonNil("owned"), shouldPass },
    { nonNil("shared"), shouldNotPass },
    { nonNil("unmanaged"), shouldNotPass },

    { nilable("owned"), shouldPass },
    { nilable("shared"), shouldNotPass },
    { nilable("unmanaged"), shouldNotPass },
  });
}

static void test20() {
  testProgram(defaultIntent, "shared", {
    { nonNil("owned"), shouldNotPass },
    { nonNil("shared"), shouldPass },
    { nonNil("unmanaged"), shouldNotPass },

    { nilable("owned"), shouldNotPass },
    { nilable("shared"), shouldPass },
    { nilable("unmanaged"), shouldNotPass },
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
