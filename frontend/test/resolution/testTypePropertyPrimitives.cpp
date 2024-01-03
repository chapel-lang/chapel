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

struct Test {
  enum Output {
    TRUE,
    FALSE,
    STRING,
    ERROR 
  };

  struct PrimitiveCalls {
    std::vector<const char*> actuals;
    Output expected;
    std::string str;

    PrimitiveCalls(std::vector<const char*> actuals, Output expected,
                   std::string str="")
      : actuals(std::move(actuals)),
        expected(expected),
        str(std::move(str)) {
      if (expected != STRING) assert(this->str.empty());
    }
  };

  std::string testName;
  bool isChplHomeRequired = false;
  std::string prelude;
  chpl::uast::primtags::PrimitiveTag primitive;
  std::vector<PrimitiveCalls> calls;
};

static void testPrimitive(Test tpg) {
  Context::Configuration config;
  if (tpg.isChplHomeRequired) {
    if (const char* chplHomeEnv = getenv("CHPL_HOME")) {
      config.chplHome = chplHomeEnv;
    } else {
      std::cout << "CHPL_HOME must be set!" << std::endl;
      exit(1);
    }
  }
  Context ctx(config);
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::stringstream ps;
  int counter = 0;
  int expectedErrorCount = 0;
  std::vector<std::string> variables;
  std::vector<Test::Output> expected;
  const auto tagStr = chpl::uast::primtags::primTagToName(tpg.primitive);

  ps << tpg.prelude << std::endl;

  for (auto& call : tpg.calls) {
    if (call.expected == Test::ERROR) expectedErrorCount += 1;

    auto var = std::string("x_") + std::to_string(counter++);
    variables.push_back(var);
    expected.push_back(call.expected);

    ps << "param " << var << " = " << "__primitive(\"";
    ps << tagStr << "\", ";

    for (int i = 0; i < call.actuals.size(); i++) {
      ps << call.actuals[i];
      bool last = (i+1) == call.actuals.size();
      if (!last) ps << ", ";
    }
    ps << ");" << std::endl;
  }

  auto text = ps.str();
  std::cout << "--- program " << tpg.testName << " (" << tagStr << ") ---";
  std::cout << std::endl << text << "---" << std::endl;
  auto varTypes = resolveTypesOfVariables(context, text, variables);

  for (size_t i = 0; i < varTypes.size(); i++) {
    auto& var = variables[i];
    auto& type = varTypes.at(var);
    std::cout << "Checking " << var << " -> ";

    switch (expected[i]) {
      case Test::TRUE:
        assert(type.isParamTrue());
        std::cout << "true";
        break;
      case Test::FALSE:
        assert(type.isParamFalse());
        std::cout << "false";
        break;
      case Test::ERROR:
        assert(type.isErroneousType());
        std::cout << "error";
        break;
      default: assert(false);
        break;
    }
    std::cout << std::endl;
  }

  assert(guard.realizeErrors() == expectedErrorCount);
}

static void test0() {
  Test tpg {
    .testName = __FUNCTION__,
    .prelude = R"""(
               record r1 { var x: int; }
               record r2 { type T; var x: T; }
               record r3 { type T=int; var x: T; }
               class c1 { var x: int; }
               class c2 { type T; var x: T; }
               class c3 { type T=int; var x: T; }
               )""",
    .primitive = chpl::uast::primtags::PRIM_IS_GENERIC_TYPE,
    .calls = {
      { {"r1"}, Test::FALSE },
      { {"r2"}, Test::TRUE },
      // Types with default values are still considered generic.
      { {"r3"}, Test::TRUE },
      { {"r3(?)"}, Test::TRUE },
      // Concrete class without management is generic.
      { {"c1"}, Test::TRUE },
      { {"c2"}, Test::TRUE },
      { {"c3"}, Test::TRUE },
      { {"c3(?)"}, Test::TRUE },
      { {"owned c1"}, Test::FALSE },
      { {"owned c2"}, Test::TRUE },
      { {"owned c3"}, Test::TRUE },
      { {"owned c3(?)"}, Test::TRUE },
      { {"shared c1"}, Test::FALSE },
      { {"shared c2"}, Test::TRUE },
      { {"shared c3"}, Test::TRUE },
      { {"shared c3(?)"}, Test::TRUE },
      { {"borrowed c1"}, Test::FALSE },
      { {"borrowed c2"}, Test::TRUE },
      { {"borrowed c3"}, Test::TRUE },
      { {"borrowed c3(?)"}, Test::TRUE },
      { {"unmanaged c1"}, Test::FALSE },
      { {"unmanaged c2"}, Test::TRUE },
      { {"unmanaged c3"}, Test::TRUE },
      { {"unmanaged c3(?)"}, Test::TRUE },
      // As above but nilable.
      { {"c1?"}, Test::TRUE },
      { {"c2?"}, Test::TRUE },
      { {"c3?"}, Test::TRUE },
      { {"c3(?)?"}, Test::TRUE },
      { {"owned c1?"}, Test::FALSE },
      { {"owned c2?"}, Test::TRUE },
      { {"owned c3?"}, Test::TRUE },
      { {"owned c3(?)?"}, Test::TRUE },
      { {"shared c1?"}, Test::FALSE },
      { {"shared c2?"}, Test::TRUE },
      { {"shared c3?"}, Test::TRUE },
      { {"shared c3(?)?"}, Test::TRUE },
      { {"borrowed c1?"}, Test::FALSE },
      { {"borrowed c2?"}, Test::TRUE },
      { {"borrowed c3?"}, Test::TRUE },
      { {"borrowed c3(?)?"}, Test::TRUE },
      { {"unmanaged c1?"}, Test::FALSE },
      { {"unmanaged c2?"}, Test::TRUE },
      { {"unmanaged c3?"}, Test::TRUE },
      { {"unmanaged c3(?)?"}, Test::TRUE },
      { {"int"}, Test::FALSE },
      { {"integral"}, Test::TRUE },
     },
  };
  testPrimitive(tpg);
}

static void test1() {
  Test tpg {
    .testName = __FUNCTION__,
    .isChplHomeRequired = false,
    .prelude = R"""(
             record r1 { var x: int; }
             class c1 { var x: int; }
             )""",
    .primitive = chpl::uast::primtags::PRIM_IS_CLASS_TYPE,
    .calls = {
      { {"r1"}, Test::FALSE },
      { {"c1"}, Test::TRUE },
      { {"owned c1"}, Test::TRUE },
      { {"int"}, Test::FALSE },
      { {"integral"}, Test::FALSE },
     },
  };
  testPrimitive(tpg);
}

static void test2() {
  Test tpg {
    .testName = __FUNCTION__,
    .isChplHomeRequired = false,
    .prelude = R"""(
             record r1 { var x: int; }
             class c1 { var x: int; }
             )""",
    .primitive = chpl::uast::primtags::PRIM_IS_NILABLE_CLASS_TYPE,
    .calls = {
      { {"r1"}, Test::FALSE },
      { {"owned c1"}, Test::FALSE },
      { {"int"}, Test::FALSE },
      { {"integral"}, Test::FALSE },
      { {"owned c1?"}, Test::TRUE },
      { {"c1?"}, Test::TRUE },
     },
  };
  testPrimitive(tpg);
}

static void test3() {
  Test tpg {
    .testName = __FUNCTION__,
    .isChplHomeRequired = false,
    .prelude = R"""(
             record r1 { var x: int; }
             class c1 { var x: int; }
             )""",
    .primitive = chpl::uast::primtags::PRIM_IS_NON_NILABLE_CLASS_TYPE,
    .calls = {
      { {"r1"}, Test::FALSE },
      { {"c1"}, Test::TRUE },
      { {"owned c1"}, Test::TRUE },
      { {"int"}, Test::FALSE },
      { {"integral"}, Test::FALSE },
      { {"owned c1?"}, Test::FALSE },
      { {"c1?"}, Test::FALSE },
     },
  };
  testPrimitive(tpg);
}

static void test4() {
  Test tpg {
    .testName = __FUNCTION__,
    .isChplHomeRequired = false,
    .prelude = R"""(
             record r1 { var x: int; }
             class c1 { var x: int; }
             )""",
    .primitive = chpl::uast::primtags::PRIM_IS_RECORD_TYPE,
    .calls = {
      { {"r1"}, Test::TRUE },
      { {"c1"}, Test::FALSE },
      { {"owned c1"}, Test::FALSE },
      { {"int"}, Test::FALSE },
      { {"integral"}, Test::FALSE },
      { {"owned c1?"}, Test::FALSE },
      { {"c1?"}, Test::FALSE },
     },
  };
  testPrimitive(tpg);
}

static void test5() {
  Test tpg {
    .testName = __FUNCTION__,
    .isChplHomeRequired = false,
    .prelude = R"""(
               record r1 { var x: int; }
               class c1 { var x: int; }
               union u1 { var x: int; }
               )""",
    .primitive = chpl::uast::primtags::PRIM_IS_UNION_TYPE,
    .calls = {
      { {"r1"}, Test::FALSE },
      { {"c1"}, Test::FALSE },
      { {"u1"}, Test::TRUE },
      { {"owned c1"}, Test::FALSE },
      { {"int"}, Test::FALSE },
      { {"integral"}, Test::FALSE },
      { {"owned c1?"}, Test::FALSE },
      { {"c1?"}, Test::FALSE },
     },
  };
  testPrimitive(tpg);
}

/**
static void test6() {
  Test tpg {
    .testName=__FUNCTION__,
    .isChplHomeRequired = true,
    .prelude=R"""(
             record r1 { var x: int; }
             )""",
    .primitive=chpl::uast::primtags::PRIM_IS_ATOMIC_TYPE,
    .calls={
      { {"r1"}, Test::FALSE },
      { {"int"}, Test::FALSE },
      { {"integral"}, Test::FALSE },
      { {"atomic int"}, Test::TRUE },
      { {"atomic r1"}, Test::TRUE },
     },
  };
  testPrimitive(tpg);
}
*/

static void test7() {
  Test tpg {
    .testName = __FUNCTION__,
    .isChplHomeRequired = false,
    .prelude = R"""(
               record r1 { var x: int; }
               class c1 { var x: int; }
               )""",
    .primitive = chpl::uast::primtags::PRIM_IS_BORROWED_CLASS_TYPE,
    .calls = {
      { {"r1"}, Test::FALSE },
      { {"c1"}, Test::FALSE },
      { {"c1?"}, Test::FALSE },
      { {"owned c1"}, Test::FALSE },
      { {"owned c1?"}, Test::FALSE },
      { {"borrowed c1"}, Test::TRUE },
      { {"borrowed c1?"}, Test::TRUE },
      { {"int"}, Test::FALSE },
      { {"integral"}, Test::FALSE },
     },
  };
  testPrimitive(tpg);
}

static void test8() {
  Test tpg {
    .testName = __FUNCTION__,
    .isChplHomeRequired = false,
    .prelude = R"""(
             record r1 { var x: int; }
             class c1 { var x: int; }
             enum e1 { foo=1, bar=2 }       // concrete
             enum e2 { foo=1, bar  }        // concrete
             enum e3 { foo, bar }           // abs
             enum e4 { foo }                // abs
             enum e5 { foo, bar=2, baz }    // semi
             enum e6 { foo, bar, baz=2 }    // semi
             var x1: e1;
             var x2: e2;
             var x3: e3;
             var x4: e4;
             var x5: e5;
             var x6: e6;
             )""",
    .primitive = chpl::uast::primtags::PRIM_IS_ABS_ENUM_TYPE,
    .calls = {
      { {"r1"}, Test::FALSE },
      { {"c1"}, Test::FALSE },
      { {"c1?"}, Test::FALSE },
      { {"owned c1"}, Test::FALSE },
      { {"owned c1?"}, Test::FALSE },
      { {"int"}, Test::FALSE },
      { {"integral"}, Test::FALSE },
      { {"e1"}, Test::FALSE },
      { {"x1"}, Test::FALSE },
      { {"e2"}, Test::FALSE },
      { {"x2"}, Test::FALSE },
      { {"e3"}, Test::TRUE },
      { {"x3"}, Test::TRUE },
      { {"e4"}, Test::TRUE },
      { {"x4"}, Test::TRUE },
      { {"e5"}, Test::FALSE },
      { {"x5"}, Test::FALSE },
      { {"e6"}, Test::FALSE },
      { {"x6"}, Test::FALSE },
     },
  };
  testPrimitive(tpg);
}

static void test9() {
  Test tpg {
    .testName = __FUNCTION__,
    .isChplHomeRequired = false,
    .prelude = R"""(
               record r1 { var x: int; }
               class c1 { var x: int; }
               class d1 : c1 {}
               )""",
    .primitive = chpl::uast::primtags::PRIM_IS_COERCIBLE,
    .calls = {
      { {"int", "bool"}, Test::FALSE },
      { {"int(8)", "bool"}, Test::FALSE },
      { {"int(16)", "bool"}, Test::FALSE },
      { {"int(32)", "bool"}, Test::FALSE },
      { {"int(64)", "bool"}, Test::FALSE },
      { {"r1", "bool"}, Test::FALSE },
      { {"c1", "bool"}, Test::FALSE },
      { {"c1?", "bool"}, Test::FALSE },
      { {"owned c1", "bool"}, Test::FALSE },
      { {"owned c1?", "bool"}, Test::FALSE },
      { {"c1", "RootClass?"}, Test::TRUE },
      { {"c1", "RootClass"}, Test::TRUE },
      { {"c1?", "RootClass?"}, Test::TRUE },
      { {"c1?", "RootClass"}, Test::FALSE },
     },
  };
  testPrimitive(tpg);
}

int main() {
  test0();
  test1();
  test2();
  test3();
  test4();
  test5();
  // test6();
  test7();
  test8();
  test9();
}
