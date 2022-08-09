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
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"
#include "common.h"

#include <cassert>

struct ReturnVariant {
  bool literal;
  std::string intent;
  std::string type;
  std::string value;
  bool generic;

  ReturnVariant(std::string intent, std::string type, bool generic=false)
    : literal(false), intent(std::move(intent)), type(std::move(type)),
      value(""), generic(generic) {}

  ReturnVariant(std::string value) : literal(true), intent(""), type(""), value(std::move(value)) {}
};

std::string buildProgram(std::string intent, std::string selectorValue, const std::vector<ReturnVariant>& variants) {
  std::ostringstream oss;
  oss << "proc f(";
  const char* trail = "";
  if (selectorValue.empty() && variants.size() > 1) {
    oss << "sel: int";
    trail = ", ";
  }
  for (size_t i = 0; i < variants.size(); i++) {
    const auto& variant = variants[i];
    if (variant.literal) continue;
    oss << trail << variant.intent << " arg" << i;
    if (!variant.generic) {
      oss << ":" << variant.type;
    }
    trail = ", ";
  }
  oss << ") " << intent << "{" << std::endl;

  if (variants.size() == 1) {
    // don't need selector if n = 1
    const auto& variant = variants[0];
    oss << "  return "
      << (variant.literal ? variant.value : "arg0") << ";";
  } else {
    if (!selectorValue.empty()) {
      oss << "  param sel = " << selectorValue << ";" << std::endl;
    }
    for (size_t i = 0; i < variants.size(); i++) {
      const auto& variant = variants[i];
      if (i != 0) {
        oss << " else ";
      } else {
        oss << "  ";
      }
      oss << "if " << "__primitive(\"==\", sel, " << i << ") {" << std::endl;
      oss << "    return ";
      if (variant.literal) {
        oss << variant.value;
      } else {
        oss << "arg" << i;
      }
      oss << ";" << std::endl;
      oss << "  }";
    }
  }
  oss << std::endl << "}" << std::endl;

  if (selectorValue.empty() && variants.size() > 1) {
    oss << "var sel : int;" << std::endl;
  }
  for (size_t i = 0; i < variants.size(); i++) {
    const auto& variant = variants[i];
    if (variant.literal) continue;
    oss << "var arg" << i << ": " << variant.type << ";" << std::endl;
  }
  oss << "var x = f(";
  trail = "";
  if (selectorValue.empty() && variants.size() > 1) {
    oss << "sel";
    trail = ", ";
  }
  for (size_t i = 0; i < variants.size(); i++) {
    const auto& variant = variants[i];
    if (variant.literal) continue;
    oss << trail << "arg" << i;
    trail = ", ";
  }
  oss << ");" << std::endl;
  return oss.str();
}

template <typename F>
void testProgram(std::string intent, std::string selectorValue,
                 const std::vector<ReturnVariant>& variants, F func) {
  Context ctx;
  auto context = &ctx;
  auto program = buildProgram(intent, selectorValue, variants);
  std::cout << "--- test program ---" << std::endl;
  std::cout << program.c_str() << std::endl;
  QualifiedType qt =  resolveTypeOfXInit(context, program.c_str());
  std::cout << "return type:" << std::endl;
  qt.dump();
  std::cout << std::endl;
  func(qt);
}

static void test1() {
  // test returning a single value from value-returning function
  testProgram("", "", {
      { "1" }
  }, [](auto& qt) {
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isIntType());
  });
}

static void test2() {
  // test returning multiple of the same type of values from
  // value-returning function
  testProgram("", "", {
      { "1" },
      { "2" }
  }, [](auto& qt) {
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isIntType());
  });
}

static void test3() {
  // test returning a param from a param-returning function
  testProgram("param", "", {
      { "1" }
  }, [](auto& qt) {
    assert(qt.type() && qt.type()->isIntType());
    assert(qt.kind() == QualifiedType::PARAM && qt.param());
    assert(qt.param()->toIntParam()->value() == 1);
  });
}

static void test4() {
  // test returning a param from an ambigous param-returning function
  // non-ambigous tests are in testParamIf.
  testProgram("param", "", {
      { "1" },
      { "2" }
  }, [](auto& qt) {
    assert(qt.isErroneousType());
  });
}

static void test5() {
  // test allowed coercions
  testProgram("", "", {
      { "", "int(32)" },
      { "", "int(16)" }
  }, [](auto& qt) {
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isIntType());
    assert(qt.type()->toIntType()->bitwidth() == 32);
  });
}

static void test6() {
  // test disallowed coercions
  testProgram("", "", {
      { "", "int(32)" },
      { "", "string" }
  }, [](auto& qt) {
    assert(qt.isErroneousType());
  });
}

static void test7() {
  // test param decaying to var
  testProgram("", "", {
      { "", "int(32)" },
      { "1" }
  }, [](auto& qt) {
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isIntType());
  });
}

static void test8() {
  // test param decaying to var, but for strings
  testProgram("", "", {
      { "", "string" },
      { "\"hello\"" }
  }, [](auto& qt) {
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isStringType());
  });
}

static void test9() {
  // test generic formals with return type resolution.
  // When the actual's type matches the fixed type in the
  // body, all is well
  testProgram("", "", {
      { "", "string", /* generic, actual is string */ true },
      { "", "string" }
  }, [](auto& qt) {
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isStringType());
  });
}

static void test10() {
  // test generic formals with return type resolution.
  // When the actual's type is mismatched from the fixed
  // type in the body, we error.
  testProgram("", "", {
      { "", "int", /* generic, actual is int */ true },
      { "", "string" }
  }, [](auto& qt) {
    assert(qt.isErroneousType());
  });
}

static void test11() {
  // test multiple branches (not just two)
  testProgram("", "", {
      { "", "int(16)" },
      { "1" },
      { "", "int" },
      { "", "int(32)" },
  }, [](auto& qt) {
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isIntType());
    assert(qt.type()->toIntType()->bitwidth() == 64);
  });
}

static void test12() {
  // returnign types from compile-time known function
  testProgram("type", "0", {
      { "int" },
      { "bool" },
  }, [](auto& qt) {
    assert(qt.kind() == QualifiedType::TYPE);
    assert(qt.type() && qt.type()->isIntType());
    assert(qt.type()->toIntType()->bitwidth() == 64);
  });
  testProgram("type", "1", {
      { "int" },
      { "bool" },
  }, [](auto& qt) {
    assert(qt.kind() == QualifiedType::TYPE);
    assert(qt.type() && qt.type()->isBoolType());
  });
}

static void test13() {
  // returning types from compile-time unknown function
  testProgram("type", "", {
      { "int" },
      { "int" },
  }, [](auto& qt) {
    assert(qt.kind() == QualifiedType::TYPE);
    assert(qt.type() && qt.type()->isIntType());
    assert(qt.type()->toIntType()->bitwidth() == 64);
  });
  testProgram("type", "", {
      { "int" },
      { "bool" },
  }, [](auto& qt) {
    assert(qt.isErroneousType());
  });
}

static void test14() {
  // test mixing types and values
  testProgram("type", "", {
      { "int" },
      { "1" },
  }, [](auto& qt) {
    assert(qt.isErroneousType());
  });
  testProgram("", "", {
      { "int" },
      { "1" },
  }, [](auto& qt) {
    assert(qt.isErroneousType());
  });
}

static void test15() {
  // test returning non-param from param-intent procedure
  testProgram("param", "", {
      { "1" },
      { "2" },
  }, [](auto& qt) {
    assert(qt.isErroneousType());
  });
}

// TODO: test ref-ness calculation. Can't do right now since
// we allow function return intents to override the computed ref-ness.

// TODO: test param coercion (param int(32) = 1 and param int(64) = 2)
// looks like canPass doesn't handle this very well.

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
  return 0;
}
