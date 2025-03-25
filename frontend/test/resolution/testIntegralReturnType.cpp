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

#include <iostream>
#include <sstream>

#include "test-resolution.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"


static std::string generateTestGenericCastZero(const std::string& type1, const std::string& type2) {
  std::ostringstream oss;
  oss << "proc f(arg: bool) {\n";
  oss << "  var i: " << type1 << ";\n";
  oss << "  var u: " << type2 << ";\n";
  oss << "  if arg then return 0:"<<type1<<"; else return 0:"<<type2<<";\n";
  oss << "}\n";
  return oss.str();
}

static std::string addCallToBoolProgram(std::string existingProgram, std::string boolVal) {
  std::string program = existingProgram;
  program += "\nvar x = f(";
  program += boolVal;
  program += ");\n";
  return program;
}

// takes an existing definition for f(bool, t, tt) and adds a call to it with
// the given values for bool, t, and tt:
// var x = f(boolVal, typeVal1, typeVal2);
static std::string addCallToProgram(std::string existingProgram, std::string boolVal, std::string typeVal1, std::string typeVal2) {
  std::string program = existingProgram;
  program += "\nvar x = f(";
  program += boolVal;
  program += ", ";
  program += typeVal1;
  program += ", ";
  program += typeVal2;
  program += ");\n";
  return program;
}

// need param and non-param variants
static std::string buildIfExpressionParamMax(std::string boolVal, std::string typeVal1, std::string typeVal2) {
  std::string program = "param b: bool =";
  program += boolVal;
  program += ";\n";
  program += "param x = if b then max(";
  program += typeVal1;
  program += "):";
  program += typeVal1;
  program += " else max(";
  program += typeVal2;
  program += "):";
  program += typeVal2;
  program += ";\n";
  return program;
}

// need param and non-param variants
static std::string buildIfExpressionParamZero(std::string boolVal, std::string typeVal1, std::string typeVal2) {
  std::string program = "param b: bool =";
  program += boolVal;
  program += ";\n";
  program += "param x = if b then 0:";
  program += typeVal1;
  program += " else 0:";
  program += typeVal2;
  program += ";\n";
  return program;
}

static std::string buildIfExpressionNoParamMax(std::string boolVal, std::string typeVal1, std::string typeVal2) {
  std::string program = "var b: bool =";
  program += boolVal;
  program += ";\n";
  program += "var zero = max(uint(8)):uint(8);\n";
  program += "var x = if b then zero:";
  program += typeVal1;
  program += " else zero:";
  program += typeVal2;
  program += ";\n";
  return program;
}

// no difference if zero is inited to 0 or not, e.g. var zero = 0:uint(8);
static std::string buildIfExpressionNoParamZero(std::string boolVal, std::string typeVal1, std::string typeVal2) {
  std::string program = "var b: bool =";
  program += boolVal;
  program += ";\n";
  // program += "var zero :uint(8);\n";
  program += "var x = if b then 0:";
  program += typeVal1;
  program += " else 0:";
  program += typeVal2;
  program += ";\n";
  return program;
}

enum class ProgramTestType {
  TestGenericMax,
  TestGenericZero,
  TestParamGenericMax,
  TestParamGenericZero,
  TestGenericCastMax,
  TestGenericCastZero,
  TestParamGenericCastMax,
  TestParamGenericCastZero,
  TestExpressionMax,
  TestExpressionZero,
  TestParamExpressionMax,
  TestParamExpressionZero
};

static void testIntegralReturn(std::string baseProgram, std::string boolVal,
                              std::string typeVal1, std::string typeVal2,
                              const chpl::types::PrimitiveType* expectedType,
                              ProgramTestType testType) {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  std::string program;
  std::string lineOut = "";
  if (testType == ProgramTestType::TestGenericMax) {
    program = addCallToProgram(baseProgram, boolVal, typeVal1, typeVal2);
    lineOut += "testGenericMax: when ";
  } else if (testType == ProgramTestType::TestGenericZero) {
    program = addCallToProgram(baseProgram, boolVal, typeVal1, typeVal2);
    lineOut += "testGenericZero: when ";
  } else if (testType == ProgramTestType::TestParamGenericMax) {
    program = addCallToProgram(baseProgram, boolVal, typeVal1, typeVal2);
    lineOut += "testParamGenericMax: when ";
  } else if (testType == ProgramTestType::TestParamGenericZero) {
    program = addCallToProgram(baseProgram, boolVal, typeVal1, typeVal2);
    lineOut += "testParamGenericZero: when ";
  } else if (testType == ProgramTestType::TestGenericCastMax) {
    program = addCallToProgram(baseProgram, boolVal, typeVal1, typeVal2);
    lineOut += "testGenericCastMax: when ";
  } else if (testType == ProgramTestType::TestGenericCastZero) {
    auto newBaseProgram = generateTestGenericCastZero(typeVal1, typeVal2);
    program = addCallToBoolProgram(newBaseProgram, boolVal);
    lineOut += "testGenericCastZero: when ";
  } else if (testType == ProgramTestType::TestParamGenericCastMax) {
    program = addCallToProgram(baseProgram, boolVal, typeVal1, typeVal2);
    lineOut += "testParamGenericCastMax: when ";
  } else if (testType == ProgramTestType::TestParamGenericCastZero) {
    program = addCallToProgram(baseProgram, boolVal, typeVal1, typeVal2);
    lineOut += "testParamGenericCastZero: when ";
  } else if (testType == ProgramTestType::TestExpressionMax) {
    program = buildIfExpressionNoParamMax(boolVal, typeVal1, typeVal2);
    lineOut += "testExpressionMax: when ";
  } else if (testType == ProgramTestType::TestExpressionZero) {
    program = buildIfExpressionNoParamZero(boolVal, typeVal1, typeVal2);
    lineOut += "testExpressionZero: when ";
  } else if (testType == ProgramTestType::TestParamExpressionMax) {
    program = buildIfExpressionParamMax(boolVal, typeVal1, typeVal2);
    lineOut += "testParamExpressionMax: when ";
  } else if (testType == ProgramTestType::TestParamExpressionZero) {
    program = buildIfExpressionParamZero(boolVal, typeVal1, typeVal2);
    lineOut += "testParamExpressionZero: when ";
  } else {
    assert(false);
  }

  lineOut += boolVal;
  lineOut += " ";
  lineOut += typeVal1;
  lineOut += " ";
  lineOut += typeVal2;
  lineOut += " then ";
  auto returnType = resolveTypeOfXInit(context, program, true);
  assert(returnType.type());
  std::cout << lineOut;
  returnType.type()->dump();
  // re-enable this when we have the correct types for each call
  // assert(returnType.type() == expectedType);
}

static void
testIfExpressionIntegralTypesHelper(std::string program,
                                    std::string boolCondition,
                                    ProgramTestType testType) {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  // use only ints. 8, 16, 32, 64 bit. use each size with every other size.
  testIntegralReturn(program, boolCondition, "int(8)", "int(8)", IntType::get(context, 8), testType);
  testIntegralReturn(program, boolCondition, "int(8)", "int(16)", IntType::get(context, 16), testType);
  testIntegralReturn(program, boolCondition, "int(8)", "int(32)", IntType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "int(8)", "int(64)", IntType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "int(16)", "int(8)", IntType::get(context, 16), testType);
  testIntegralReturn(program, boolCondition, "int(16)", "int(16)", IntType::get(context, 16), testType);
  testIntegralReturn(program, boolCondition, "int(16)", "int(32)", IntType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "int(16)", "int(64)", IntType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "int(32)", "int(8)", IntType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "int(32)", "int(16)", IntType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "int(32)", "int(32)", IntType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "int(32)", "int(64)", IntType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "int(64)", "int(8)", IntType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "int(64)", "int(16)", IntType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "int(64)", "int(32)", IntType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "int(64)", "int(64)", IntType::get(context, 64), testType);
  // do the uint versions of all the above, true and false versions
  testIntegralReturn(program, boolCondition, "uint(8)", "uint(8)", UintType::get(context, 8), testType);
  testIntegralReturn(program, boolCondition, "uint(8)", "uint(16)", UintType::get(context, 16), testType);
  testIntegralReturn(program, boolCondition, "uint(8)", "uint(32)", UintType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "uint(8)", "uint(64)", UintType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "uint(16)", "uint(8)", UintType::get(context, 16), testType);
  testIntegralReturn(program, boolCondition, "uint(16)", "uint(16)", UintType::get(context, 16), testType);
  testIntegralReturn(program, boolCondition, "uint(16)", "uint(32)", UintType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "uint(16)", "uint(64)", UintType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "uint(32)", "uint(8)", UintType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "uint(32)", "uint(16)", UintType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "uint(32)", "uint(32)", UintType::get(context, 32), testType);;
  testIntegralReturn(program, boolCondition, "uint(32)", "uint(64)", UintType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "uint(64)", "uint(8)", UintType::get(context, 64), testType);;
  testIntegralReturn(program, boolCondition, "uint(64)", "uint(16)", UintType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "uint(64)", "uint(32)", UintType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "uint(64)", "uint(64)", UintType::get(context, 64), testType);
  // tests where the first type is int and the second is uint, do all sizes of ints and uints
  testIntegralReturn(program, boolCondition, "int(8)", "uint(8)", UintType::get(context, 8), testType);
  testIntegralReturn(program, boolCondition, "int(8)", "uint(16)", UintType::get(context, 16), testType);
  testIntegralReturn(program, boolCondition, "int(8)", "uint(32)", UintType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "int(8)", "uint(64)", UintType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "int(16)", "uint(8)", IntType::get(context, 16), testType);
  testIntegralReturn(program, boolCondition, "int(16)", "uint(16)", UintType::get(context, 16), testType);
  testIntegralReturn(program, boolCondition, "int(16)", "uint(32)", UintType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "int(16)", "uint(64)", UintType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "int(32)", "uint(8)", IntType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "int(32)", "uint(16)", IntType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "int(32)", "uint(32)", UintType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "int(32)", "uint(64)", UintType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "int(64)", "uint(8)", IntType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "int(64)", "uint(16)", IntType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "int(64)", "uint(32)", IntType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "int(64)", "uint(64)", UintType::get(context, 64), testType);
  // reverse the order of the types so that the first type is uint and the second is int
  testIntegralReturn(program, boolCondition, "uint(8)", "int(8)", UintType::get(context, 8), testType);
  testIntegralReturn(program, boolCondition, "uint(8)", "int(16)", IntType::get(context, 16), testType);
  testIntegralReturn(program, boolCondition, "uint(8)", "int(32)", IntType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "uint(8)", "int(64)", IntType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "uint(16)", "int(8)", UintType::get(context, 16), testType);
  testIntegralReturn(program, boolCondition, "uint(16)", "int(16)", UintType::get(context, 16), testType);
  testIntegralReturn(program, boolCondition, "uint(16)", "int(32)", IntType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "uint(16)", "int(64)", IntType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "uint(32)", "int(8)", UintType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "uint(32)", "int(16)", UintType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "uint(32)", "int(32)", UintType::get(context, 32), testType);
  testIntegralReturn(program, boolCondition, "uint(32)", "int(64)", IntType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "uint(64)", "int(8)", UintType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "uint(64)", "int(16)", UintType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "uint(64)", "int(32)", UintType::get(context, 64), testType);
  testIntegralReturn(program, boolCondition, "uint(64)", "int(64)", UintType::get(context, 64), testType);
}

static void testConditionalIntegralTypes() {
  std::string testGenericZero =
    R"""(
    proc f(arg: bool, type t, type tt) {
      var i: t = 0;
      var u: tt = 0;
      if arg then return i; else return u;
    }
    )""";

  std::string testGenericMax =
    R"""(
    proc f(arg: bool, type t, type tt) {
      var i: t = max(t);
      var u: tt = max(tt);
      if arg then return i; else return u;
    }
    )""";

  std::string testParamGenericZero =
    R"""(
    proc f(param arg: bool, type t, type tt) param {
      param i: t = 0;
      param u: tt = 0;
      if arg then return i; else return u;
    }
    )""";

  std::string testParamGenericMax =
    R"""(
    proc f(param arg: bool, type t, type tt) param {
      param i: t = max(t);
      param u: tt = max(tt);
      if arg then return i; else return u;
    }
    )""";

  // std::string testGenericCastZero =
  //   R"""(
  //   proc f(arg: bool, type t, type tt) {
  //     if arg then return 0:t; else return 0:tt;
  //   }
  //   )""";

  std::string testGenericCastMax =
    R"""(
    proc f(arg: bool, type t, type tt) {
      if arg then return max(t):t;
      else return max(tt):tt;
    }
    )""";

  std::string testParamGenericCastZero =
    R"""(
    proc f(param arg: bool, type t, type tt) param {
      if arg then return 0:t; else return 0:tt;
    }
    )""";

  std::string testParamGenericCastMax =
    R"""(
    proc f(param arg: bool, type t, type tt) param {
      if arg then return max(t):t; else return max(tt):tt;
    }
    )""";

  testIfExpressionIntegralTypesHelper(testGenericZero, "true", ProgramTestType::TestGenericZero);
  testIfExpressionIntegralTypesHelper(testGenericZero, "false", ProgramTestType::TestGenericZero);
  testIfExpressionIntegralTypesHelper(testGenericMax, "true", ProgramTestType::TestGenericMax);
  testIfExpressionIntegralTypesHelper(testGenericMax, "false", ProgramTestType::TestGenericMax);
  testIfExpressionIntegralTypesHelper(testParamGenericZero, "true", ProgramTestType::TestParamGenericZero);
  testIfExpressionIntegralTypesHelper(testParamGenericZero, "false", ProgramTestType::TestParamGenericZero);
  testIfExpressionIntegralTypesHelper(testParamGenericMax, "true", ProgramTestType::TestParamGenericMax);
  testIfExpressionIntegralTypesHelper(testParamGenericMax, "false", ProgramTestType::TestParamGenericMax);
  testIfExpressionIntegralTypesHelper(testParamGenericCastZero, "true", ProgramTestType::TestParamGenericCastZero);
  testIfExpressionIntegralTypesHelper(testParamGenericCastZero, "false", ProgramTestType::TestParamGenericCastZero);
  testIfExpressionIntegralTypesHelper(testParamGenericCastMax, "true", ProgramTestType::TestParamGenericCastMax);
  testIfExpressionIntegralTypesHelper(testParamGenericCastMax, "false", ProgramTestType::TestParamGenericCastMax);
  testIfExpressionIntegralTypesHelper("", "true", ProgramTestType::TestGenericCastZero);
  testIfExpressionIntegralTypesHelper("", "false", ProgramTestType::TestGenericCastZero);
  testIfExpressionIntegralTypesHelper(testGenericCastMax, "true", ProgramTestType::TestGenericCastMax);
  testIfExpressionIntegralTypesHelper(testGenericCastMax, "false", ProgramTestType::TestGenericCastMax);
  testIfExpressionIntegralTypesHelper("", "true", ProgramTestType::TestExpressionZero);
  testIfExpressionIntegralTypesHelper("", "false", ProgramTestType::TestExpressionZero);
  testIfExpressionIntegralTypesHelper("", "true", ProgramTestType::TestExpressionMax);
  testIfExpressionIntegralTypesHelper("", "false", ProgramTestType::TestExpressionMax);
  testIfExpressionIntegralTypesHelper("", "true", ProgramTestType::TestParamExpressionZero);
  testIfExpressionIntegralTypesHelper("", "false", ProgramTestType::TestParamExpressionZero);
  testIfExpressionIntegralTypesHelper("", "true", ProgramTestType::TestParamExpressionMax);
  testIfExpressionIntegralTypesHelper("", "false", ProgramTestType::TestParamExpressionMax);

  // std::vector<std::string> integral_types = {
  //   "int(8)", "int(16)", "int(32)", "int(64)",
  //   "uint(8)", "uint(16)", "uint(32)", "uint(64)"
  // };

  // for (const auto& type1 : integral_types) {
  //     for (const auto& type2 : integral_types) {
  //         std::string chapel_program = generateChapelProgram(type1, type2);

  //     }
  // }
}

int main() {
  testConditionalIntegralTypes();
  return 0;
}

