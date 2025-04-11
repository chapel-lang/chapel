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

#include <sstream>

#include "test-resolution.h"


static void readyMap(Context* context);

// stores the mapping of test name, bool val, type1, type2, and expected result
std::map<std::tuple<std::string, std::string, std::string, std::string>, std::string> testExpectationsMap;

// given a string representing a type, returns the corresponding PrimitiveType
static const chpl::types::PrimitiveType* stringNameToType(Context* context, std::string name) {
  if (name == "int(8)") {
    return IntType::get(context, 8);
  } else if (name == "int(16)") {
    return IntType::get(context, 16);
  } else if (name == "int(32)") {
    return IntType::get(context, 32);
  } else if (name == "int(64)") {
    return IntType::get(context, 64);
  } else if (name == "uint(8)") {
    return UintType::get(context, 8);
  } else if (name == "uint(16)") {
    return UintType::get(context, 16);
  } else if (name == "uint(32)") {
    return UintType::get(context, 32);
  } else if (name == "uint(64)") {
    return UintType::get(context, 64);
  } else {
    assert(false && "Unexpected type name");
  }
}

// given an existing block of code defining a procedure f(arg: bool),
// add a call to f() with the given boolean value
static std::string addCallToProcedure(std::string& existingProcedure,
                              std::string boolVal, bool isParam=false) {
  std::ostringstream oss;
  oss << existingProcedure;
  oss << (isParam ? "param " : "var ");
  oss << "x = f(" << boolVal << ");\n";
  return oss.str();
}

static std::string addBoolToExpression(std::string& existingExpression,
                                std::string boolVal, bool isParam=false) {
  std::ostringstream oss;
  oss << (isParam ? "param " : "var ");
  oss << "b: bool = " << boolVal << ";\n";
  oss << existingExpression;
  return oss.str();
}

static void
buildProcedure(std::ostringstream& oss, std::string typeVal1, std::string typeVal2,
              bool isParam=false, bool isMax=false, bool defVars=false) {
  oss << "proc f(";
  oss << (isParam ? "param " : "");
  oss << "arg: bool) ";
  oss << (isParam ? "param " : "");
  oss << "{\n";
  if (defVars) {
    oss << (isParam ? "param " : "var ");
    oss << "  i: ";
    oss << typeVal1;
    oss << " = ";
    oss << (isMax ? "max(" + typeVal1 + ");\n" : "0;\n");
    oss << (isParam ? "param " : "var ");
    oss << "  u: " << typeVal2 << " = ";
    oss << (isMax ? "max(" + typeVal2 + ");\n" : "0;\n");
    oss << "  if arg then return i; else return u;\n";
  } else {
    oss << "  if arg then return ";
    oss << (isMax ? "max(" + typeVal1 + "):" : "0:");
    oss << typeVal1 << "; else return " << (isMax ? "max(" + typeVal2 + "):" : "0:");
    oss << typeVal2 << ";\n";
  }
  oss << "}\n";
}

static void
buildIfExpression(std::ostringstream& oss, std::string typeVal1,
                  std::string typeVal2, bool isParam=false, bool isMax=false) {
  oss << (isParam ? "param " : "var ");
  oss << "x = if b then " << (isMax ? "max(" + typeVal1 + "):" : "0:");
  oss << typeVal1 << " else " << (isMax ? "max(" + typeVal2 + "):" : "0:");
  oss << typeVal2 << ";\n";
}

struct IntegralTest {
  const bool isParam;
  const bool isMax;
  const bool defVars;
  const bool isProcedure;
  const std::string type1;
  const std::string type2;
  const std::string testName;
  std::string body;
};

static void evaluateTestResult(IntegralTest& test, std::string boolVal) {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  std::string program;
  if (test.isProcedure) {
    program = addCallToProcedure(test.body, boolVal, test.isParam);
  } else {
    program = addBoolToExpression(test.body, boolVal, test.isParam);
  }
  auto returnType = resolveTypeOfXInit(context, program, true);
  assert(returnType.type());
  // these lines can be used to debug or collect updated values for the testExpectationsMap
  // std::cout << test.testName << ": when " << boolVal << " " << test.type1 << " " << test.type2 << " then ";
  // returnType.type()->dump();
  auto expectedTypeName = testExpectationsMap[std::make_tuple(test.testName + ":", boolVal, test.type1, test.type2)];
  assert(expectedTypeName != "");
  auto expectedType = stringNameToType(context, expectedTypeName);
  assert(expectedType);
  assert(returnType.type() == expectedType);
}

// build a bunch of tests using different integral types in both procedures and expressions
// then add calls to the procedures and evaluate the expressions with different boolean values
// and ensure the return type is as expected
static void testConditionalIntegralTypes() {
  auto context = buildStdContext();
  readyMap(context);
  std::vector<std::string> integralTypes = {
    "int(8)", "int(16)", "int(32)", "int(64)",
    "uint(8)", "uint(16)", "uint(32)", "uint(64)"
  };

  std::vector<std::string> boolVals = {
    "true", "false"
  };
  std::vector<IntegralTest> chapelTests;
  std::ostringstream oss;
  for (const auto& type1 : integralTypes) {
    for (const auto& type2 : integralTypes) {
      chapelTests.push_back({false, false, false, true, type1, type2, "testProcCastZero", ""});
      chapelTests.push_back({false, false, true, true, type1, type2, "testProcZero", ""});
      chapelTests.push_back({false, true, false, true, type1, type2, "testProcCastMax", ""});
      chapelTests.push_back({false, true, true, true, type1, type2, "testProcMax",""});
      chapelTests.push_back({true, false, false, true, type1, type2, "testParamProcCastZero",""});
      chapelTests.push_back({true, false, true, true, type1, type2, "testParamProcZero",""});
      chapelTests.push_back({true, true, false, true, type1, type2, "testParamProcCastMax",""});
      chapelTests.push_back({true, true, true, true, type1, type2, "testParamProcMax",""});
      chapelTests.push_back({false, false, false, false, type1, type2, "testExpressionZero",""});
      chapelTests.push_back({false, true, false, false, type1, type2, "testExpressionMax",""});
      chapelTests.push_back({true, false, false, false, type1, type2, "testParamExpressionZero",""});
      chapelTests.push_back({true, true, false, false, type1, type2, "testParamExpressionMax",""});
    }
  }

  for (auto& test : chapelTests) {
    oss.str("");
    oss.clear();
    if (test.isProcedure) {
      buildProcedure(oss, test.type1, test.type2, test.isParam, test.isMax, test.defVars);
    } else {
      buildIfExpression(oss, test.type1, test.type2, test.isParam, test.isMax);
    }
    test.body = oss.str();
  }

  for (auto& test : chapelTests) {
    for (auto& bVal : boolVals) {
      evaluateTestResult(test, bVal);
    }
  }
}

int main() {
  testConditionalIntegralTypes();
  return 0;
}

// prepare the testExpectationsMap with the expected results.
// these values are based on previous runs of the tests, capturing the output
static void readyMap(Context* context) {
  // there are four distinct result tables that all the test combinations map to
  // here we define the expected results using one test name whose value definition
  // is in the long list of output
  using TestPair = std::pair<std::string, std::string>;
  using TestPairList = std::vector<TestPair>;
  using TestListMap = std::map<TestPair, TestPairList>;
  TestListMap testMap;
  testMap.emplace(TestPair("testExpressionZero:", "true"),
                  TestPairList{TestPair("testExpressionZero:", "false")});

  testMap.emplace(TestPair("testProcZero:", "true"),
                  TestPairList{
                    TestPair("testProcZero:", "false"),
                    TestPair("testProcMax:", "true"),
                    TestPair("testProcMax:", "false"),
                    TestPair("testProcCastZero:", "true"),
                    TestPair("testProcCastZero:", "false"),
                    TestPair("testProcMax:", "true"),
                    TestPair("testProcMax:", "false"),
                    TestPair("testProcCastMax:", "true"),
                    TestPair("testProcCastMax:", "false"),
                    TestPair("testExpressionMax:", "true"),
                    TestPair("testExpressionMax:", "false")});

  testMap.emplace(TestPair("testParamProcZero:", "true"),
                  TestPairList{
                    TestPair("testParamProcMax:", "true"),
                    TestPair("testParamExpressionZero:", "true"),
                    TestPair("testParamExpressionMax:", "true"),
                    TestPair("testParamProcCastZero:", "true"),
                    TestPair("testParamProcCastMax:", "true")});

  testMap.emplace(TestPair("testParamProcZero:", "false"),
                  TestPairList{
                    TestPair("testParamProcMax:", "false"),
                    TestPair("testParamExpressionZero:", "false"),
                    TestPair("testParamExpressionMax:", "false"),
                    TestPair("testParamProcCastZero:", "false"),
                    TestPair("testParamProcCastMax:", "false")});

  std::string input = R"(
  testProcZero: when true int(8) int(8) then int(8)
  testParamProcZero: when true int(8) int(8) then int(8)
  testParamProcZero: when false int(8) int(8) then int(8)
  testExpressionZero: when true int(8) int(8) then int(8)
  testProcZero: when true int(8) int(16) then int(16)
  testParamProcZero: when true int(8) int(16) then int(8)
  testParamProcZero: when false int(8) int(16) then int(16)
  testExpressionZero: when true int(8) int(16) then int(8)
  testProcZero: when true int(8) int(32) then int(32)
  testParamProcZero: when true int(8) int(32) then int(8)
  testParamProcZero: when false int(8) int(32) then int(32)
  testExpressionZero: when true int(8) int(32) then int(8)
  testProcZero: when true int(8) int(64) then int(64)
  testParamProcZero: when true int(8) int(64) then int(8)
  testParamProcZero: when false int(8) int(64) then int(64)
  testExpressionZero: when true int(8) int(64) then int(8)
  testProcZero: when true int(8) uint(8) then uint(8)
  testParamProcZero: when true int(8) uint(8) then int(8)
  testParamProcZero: when false int(8) uint(8) then uint(8)
  testExpressionZero: when true int(8) uint(8) then uint(8)
  testProcZero: when true int(8) uint(16) then uint(16)
  testParamProcZero: when true int(8) uint(16) then int(8)
  testParamProcZero: when false int(8) uint(16) then uint(16)
  testExpressionZero: when true int(8) uint(16) then uint(16)
  testProcZero: when true int(8) uint(32) then uint(32)
  testParamProcZero: when true int(8) uint(32) then int(8)
  testParamProcZero: when false int(8) uint(32) then uint(32)
  testExpressionZero: when true int(8) uint(32) then uint(32)
  testProcZero: when true int(8) uint(64) then uint(64)
  testParamProcZero: when true int(8) uint(64) then int(8)
  testParamProcZero: when false int(8) uint(64) then uint(64)
  testExpressionZero: when true int(8) uint(64) then uint(64)
  testProcZero: when true int(16) int(8) then int(16)
  testParamProcZero: when true int(16) int(8) then int(16)
  testParamProcZero: when false int(16) int(8) then int(8)
  testExpressionZero: when true int(16) int(8) then int(16)
  testProcZero: when true int(16) int(16) then int(16)
  testParamProcZero: when true int(16) int(16) then int(16)
  testParamProcZero: when false int(16) int(16) then int(16)
  testExpressionZero: when true int(16) int(16) then int(16)
  testProcZero: when true int(16) int(32) then int(32)
  testParamProcZero: when true int(16) int(32) then int(16)
  testParamProcZero: when false int(16) int(32) then int(32)
  testExpressionZero: when true int(16) int(32) then int(16)
  testProcZero: when true int(16) int(64) then int(64)
  testParamProcZero: when true int(16) int(64) then int(16)
  testParamProcZero: when false int(16) int(64) then int(64)
  testExpressionZero: when true int(16) int(64) then int(16)
  testProcZero: when true int(16) uint(8) then int(16)
  testParamProcZero: when true int(16) uint(8) then int(16)
  testParamProcZero: when false int(16) uint(8) then uint(8)
  testExpressionZero: when true int(16) uint(8) then int(16)
  testProcZero: when true int(16) uint(16) then uint(16)
  testParamProcZero: when true int(16) uint(16) then int(16)
  testParamProcZero: when false int(16) uint(16) then uint(16)
  testExpressionZero: when true int(16) uint(16) then uint(16)
  testProcZero: when true int(16) uint(32) then uint(32)
  testParamProcZero: when true int(16) uint(32) then int(16)
  testParamProcZero: when false int(16) uint(32) then uint(32)
  testExpressionZero: when true int(16) uint(32) then uint(32)
  testProcZero: when true int(16) uint(64) then uint(64)
  testParamProcZero: when true int(16) uint(64) then int(16)
  testParamProcZero: when false int(16) uint(64) then uint(64)
  testExpressionZero: when true int(16) uint(64) then uint(64)
  testProcZero: when true int(32) int(8) then int(32)
  testParamProcZero: when true int(32) int(8) then int(32)
  testParamProcZero: when false int(32) int(8) then int(8)
  testExpressionZero: when true int(32) int(8) then int(32)
  testProcZero: when true int(32) int(16) then int(32)
  testParamProcZero: when true int(32) int(16) then int(32)
  testParamProcZero: when false int(32) int(16) then int(16)
  testExpressionZero: when true int(32) int(16) then int(32)
  testProcZero: when true int(32) int(32) then int(32)
  testParamProcZero: when true int(32) int(32) then int(32)
  testParamProcZero: when false int(32) int(32) then int(32)
  testExpressionZero: when true int(32) int(32) then int(32)
  testProcZero: when true int(32) int(64) then int(64)
  testParamProcZero: when true int(32) int(64) then int(32)
  testParamProcZero: when false int(32) int(64) then int(64)
  testExpressionZero: when true int(32) int(64) then int(32)
  testProcZero: when true int(32) uint(8) then int(32)
  testParamProcZero: when true int(32) uint(8) then int(32)
  testParamProcZero: when false int(32) uint(8) then uint(8)
  testExpressionZero: when true int(32) uint(8) then int(32)
  testProcZero: when true int(32) uint(16) then int(32)
  testParamProcZero: when true int(32) uint(16) then int(32)
  testParamProcZero: when false int(32) uint(16) then uint(16)
  testExpressionZero: when true int(32) uint(16) then int(32)
  testProcZero: when true int(32) uint(32) then uint(32)
  testParamProcZero: when true int(32) uint(32) then int(32)
  testParamProcZero: when false int(32) uint(32) then uint(32)
  testExpressionZero: when true int(32) uint(32) then uint(32)
  testProcZero: when true int(32) uint(64) then uint(64)
  testParamProcZero: when true int(32) uint(64) then int(32)
  testParamProcZero: when false int(32) uint(64) then uint(64)
  testExpressionZero: when true int(32) uint(64) then uint(64)
  testProcZero: when true int(64) int(8) then int(64)
  testParamProcZero: when true int(64) int(8) then int(64)
  testParamProcZero: when false int(64) int(8) then int(8)
  testExpressionZero: when true int(64) int(8) then int(64)
  testProcZero: when true int(64) int(16) then int(64)
  testParamProcZero: when true int(64) int(16) then int(64)
  testParamProcZero: when false int(64) int(16) then int(16)
  testExpressionZero: when true int(64) int(16) then int(64)
  testProcZero: when true int(64) int(32) then int(64)
  testParamProcZero: when true int(64) int(32) then int(64)
  testParamProcZero: when false int(64) int(32) then int(32)
  testExpressionZero: when true int(64) int(32) then int(64)
  testProcZero: when true int(64) int(64) then int(64)
  testParamProcZero: when true int(64) int(64) then int(64)
  testParamProcZero: when false int(64) int(64) then int(64)
  testExpressionZero: when true int(64) int(64) then int(64)
  testProcZero: when true int(64) uint(8) then int(64)
  testParamProcZero: when true int(64) uint(8) then int(64)
  testParamProcZero: when false int(64) uint(8) then uint(8)
  testExpressionZero: when true int(64) uint(8) then int(64)
  testProcZero: when true int(64) uint(16) then int(64)
  testParamProcZero: when true int(64) uint(16) then int(64)
  testParamProcZero: when false int(64) uint(16) then uint(16)
  testExpressionZero: when true int(64) uint(16) then int(64)
  testProcZero: when true int(64) uint(32) then int(64)
  testParamProcZero: when true int(64) uint(32) then int(64)
  testParamProcZero: when false int(64) uint(32) then uint(32)
  testExpressionZero: when true int(64) uint(32) then int(64)
  testProcZero: when true int(64) uint(64) then uint(64)
  testParamProcZero: when true int(64) uint(64) then int(64)
  testParamProcZero: when false int(64) uint(64) then uint(64)
  testExpressionZero: when true int(64) uint(64) then uint(64)
  testProcZero: when true uint(8) int(8) then uint(8)
  testParamProcZero: when true uint(8) int(8) then uint(8)
  testParamProcZero: when false uint(8) int(8) then int(8)
  testExpressionZero: when true uint(8) int(8) then uint(8)
  testProcZero: when true uint(8) int(16) then int(16)
  testParamProcZero: when true uint(8) int(16) then uint(8)
  testParamProcZero: when false uint(8) int(16) then int(16)
  testExpressionZero: when true uint(8) int(16) then int(16)
  testProcZero: when true uint(8) int(32) then int(32)
  testParamProcZero: when true uint(8) int(32) then uint(8)
  testParamProcZero: when false uint(8) int(32) then int(32)
  testExpressionZero: when true uint(8) int(32) then int(32)
  testProcZero: when true uint(8) int(64) then int(64)
  testParamProcZero: when true uint(8) int(64) then uint(8)
  testParamProcZero: when false uint(8) int(64) then int(64)
  testExpressionZero: when true uint(8) int(64) then int(64)
  testProcZero: when true uint(8) uint(8) then uint(8)
  testParamProcZero: when true uint(8) uint(8) then uint(8)
  testParamProcZero: when false uint(8) uint(8) then uint(8)
  testExpressionZero: when true uint(8) uint(8) then uint(8)
  testProcZero: when true uint(8) uint(16) then uint(16)
  testParamProcZero: when true uint(8) uint(16) then uint(8)
  testParamProcZero: when false uint(8) uint(16) then uint(16)
  testExpressionZero: when true uint(8) uint(16) then uint(8)
  testProcZero: when true uint(8) uint(32) then uint(32)
  testParamProcZero: when true uint(8) uint(32) then uint(8)
  testParamProcZero: when false uint(8) uint(32) then uint(32)
  testExpressionZero: when true uint(8) uint(32) then uint(8)
  testProcZero: when true uint(8) uint(64) then uint(64)
  testParamProcZero: when true uint(8) uint(64) then uint(8)
  testParamProcZero: when false uint(8) uint(64) then uint(64)
  testExpressionZero: when true uint(8) uint(64) then uint(8)
  testProcZero: when true uint(16) int(8) then uint(16)
  testParamProcZero: when true uint(16) int(8) then uint(16)
  testParamProcZero: when false uint(16) int(8) then int(8)
  testExpressionZero: when true uint(16) int(8) then uint(16)
  testProcZero: when true uint(16) int(16) then uint(16)
  testParamProcZero: when true uint(16) int(16) then uint(16)
  testParamProcZero: when false uint(16) int(16) then int(16)
  testExpressionZero: when true uint(16) int(16) then uint(16)
  testProcZero: when true uint(16) int(32) then int(32)
  testParamProcZero: when true uint(16) int(32) then uint(16)
  testParamProcZero: when false uint(16) int(32) then int(32)
  testExpressionZero: when true uint(16) int(32) then int(32)
  testProcZero: when true uint(16) int(64) then int(64)
  testParamProcZero: when true uint(16) int(64) then uint(16)
  testParamProcZero: when false uint(16) int(64) then int(64)
  testExpressionZero: when true uint(16) int(64) then int(64)
  testProcZero: when true uint(16) uint(8) then uint(16)
  testParamProcZero: when true uint(16) uint(8) then uint(16)
  testParamProcZero: when false uint(16) uint(8) then uint(8)
  testExpressionZero: when true uint(16) uint(8) then uint(16)
  testProcZero: when true uint(16) uint(16) then uint(16)
  testParamProcZero: when true uint(16) uint(16) then uint(16)
  testParamProcZero: when false uint(16) uint(16) then uint(16)
  testExpressionZero: when true uint(16) uint(16) then uint(16)
  testProcZero: when true uint(16) uint(32) then uint(32)
  testParamProcZero: when true uint(16) uint(32) then uint(16)
  testParamProcZero: when false uint(16) uint(32) then uint(32)
  testExpressionZero: when true uint(16) uint(32) then uint(16)
  testProcZero: when true uint(16) uint(64) then uint(64)
  testParamProcZero: when true uint(16) uint(64) then uint(16)
  testParamProcZero: when false uint(16) uint(64) then uint(64)
  testExpressionZero: when true uint(16) uint(64) then uint(16)
  testProcZero: when true uint(32) int(8) then uint(32)
  testParamProcZero: when true uint(32) int(8) then uint(32)
  testParamProcZero: when false uint(32) int(8) then int(8)
  testExpressionZero: when true uint(32) int(8) then uint(32)
  testProcZero: when true uint(32) int(16) then uint(32)
  testParamProcZero: when true uint(32) int(16) then uint(32)
  testParamProcZero: when false uint(32) int(16) then int(16)
  testExpressionZero: when true uint(32) int(16) then uint(32)
  testProcZero: when true uint(32) int(32) then uint(32)
  testParamProcZero: when true uint(32) int(32) then uint(32)
  testParamProcZero: when false uint(32) int(32) then int(32)
  testExpressionZero: when true uint(32) int(32) then uint(32)
  testProcZero: when true uint(32) int(64) then int(64)
  testParamProcZero: when true uint(32) int(64) then uint(32)
  testParamProcZero: when false uint(32) int(64) then int(64)
  testExpressionZero: when true uint(32) int(64) then int(64)
  testProcZero: when true uint(32) uint(8) then uint(32)
  testParamProcZero: when true uint(32) uint(8) then uint(32)
  testParamProcZero: when false uint(32) uint(8) then uint(8)
  testExpressionZero: when true uint(32) uint(8) then uint(32)
  testProcZero: when true uint(32) uint(16) then uint(32)
  testParamProcZero: when true uint(32) uint(16) then uint(32)
  testParamProcZero: when false uint(32) uint(16) then uint(16)
  testExpressionZero: when true uint(32) uint(16) then uint(32)
  testProcZero: when true uint(32) uint(32) then uint(32)
  testParamProcZero: when true uint(32) uint(32) then uint(32)
  testParamProcZero: when false uint(32) uint(32) then uint(32)
  testExpressionZero: when true uint(32) uint(32) then uint(32)
  testProcZero: when true uint(32) uint(64) then uint(64)
  testParamProcZero: when true uint(32) uint(64) then uint(32)
  testParamProcZero: when false uint(32) uint(64) then uint(64)
  testExpressionZero: when true uint(32) uint(64) then uint(32)
  testProcZero: when true uint(64) int(8) then uint(64)
  testParamProcZero: when true uint(64) int(8) then uint(64)
  testParamProcZero: when false uint(64) int(8) then int(8)
  testExpressionZero: when true uint(64) int(8) then uint(64)
  testProcZero: when true uint(64) int(16) then uint(64)
  testParamProcZero: when true uint(64) int(16) then uint(64)
  testParamProcZero: when false uint(64) int(16) then int(16)
  testExpressionZero: when true uint(64) int(16) then uint(64)
  testProcZero: when true uint(64) int(32) then uint(64)
  testParamProcZero: when true uint(64) int(32) then uint(64)
  testParamProcZero: when false uint(64) int(32) then int(32)
  testExpressionZero: when true uint(64) int(32) then uint(64)
  testProcZero: when true uint(64) int(64) then uint(64)
  testParamProcZero: when true uint(64) int(64) then uint(64)
  testParamProcZero: when false uint(64) int(64) then int(64)
  testExpressionZero: when true uint(64) int(64) then uint(64)
  testProcZero: when true uint(64) uint(8) then uint(64)
  testParamProcZero: when true uint(64) uint(8) then uint(64)
  testParamProcZero: when false uint(64) uint(8) then uint(8)
  testExpressionZero: when true uint(64) uint(8) then uint(64)
  testProcZero: when true uint(64) uint(16) then uint(64)
  testParamProcZero: when true uint(64) uint(16) then uint(64)
  testParamProcZero: when false uint(64) uint(16) then uint(16)
  testExpressionZero: when true uint(64) uint(16) then uint(64)
  testProcZero: when true uint(64) uint(32) then uint(64)
  testParamProcZero: when true uint(64) uint(32) then uint(64)
  testParamProcZero: when false uint(64) uint(32) then uint(32)
  testExpressionZero: when true uint(64) uint(32) then uint(64)
  testProcZero: when true uint(64) uint(64) then uint(64)
  testParamProcZero: when true uint(64) uint(64) then uint(64)
  testParamProcZero: when false uint(64) uint(64) then uint(64)
  testExpressionZero: when true uint(64) uint(64) then uint(64)
  )";

  std::istringstream iss(input);
  std::string line;
  while (std::getline(iss, line)) {
    std::istringstream lineStream(line);
    std::string testName, when, condition, type1, type2, then, resultType;

    lineStream >> testName >> when >> condition >> type1 >> type2 >> then >> resultType;

    auto key = std::make_tuple(testName, condition, type1, type2);
    testExpectationsMap[key] = resultType;
    // look for other test names and condition pairs that map to the same set of outputs
    // this lets us avoid storing every individual result in this file
    auto otherSameTable = testMap.find(TestPair(testName, condition));
    if (otherSameTable != testMap.end()) {
      auto otherSameList = otherSameTable->second;
      for (const auto& otherSame : otherSameList) {
        auto otherKey = std::make_tuple(otherSame.first, otherSame.second, type1, type2);
        testExpectationsMap[otherKey] = resultType;
      }
    }

  }
}
