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
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

static std::string buildControlFlowProgram(std::string existingProgram, std::string boolVal, std::string typeVal1, std::string typeVal2) {
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
static std::string buildIfExpressionParam(std::string boolVal, std::string typeVal1, std::string typeVal2) {
  std::string program = "";
  program += "var x = if ";
  program += boolVal;
  program += " then 0:";
  program += typeVal1;
  program += " else 0:";
  program += typeVal2;
  program += ";\n";
  return program;
}

static std::string buildIfExpressionNoParam(std::string boolVal, std::string typeVal1, std::string typeVal2) {
  std::string program = "";
  program += "var zero:uint(8);\n";
  program += "var x = if ";
  program += boolVal;
  program += " then zero:";
  program += typeVal1;
  program += " else zero:";
  program += typeVal2;
  program += ";\n";
  return program;
}

enum class ProgramTestType {
  TestGeneric,
  TestGenericCast,
  TestParamGenericCast,
  TestExpression,
  TestParamExpression
};

static void testIntegralReturn(std::string baseProgram, std::string boolVal,
                              std::string typeVal1, std::string typeVal2,
                              const chpl::types::PrimitiveType* expectedType,
                              ProgramTestType testType) {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  std::string program;
  std::string lineOut = "";
  if (testType == ProgramTestType::TestGeneric) {
    program = buildControlFlowProgram(baseProgram, boolVal, typeVal1, typeVal2);
    lineOut += "testGeneric: when ";
  } else if (testType == ProgramTestType::TestGenericCast) {
    program = buildControlFlowProgram(baseProgram, boolVal, typeVal1, typeVal2);
    lineOut += "testGenericCast: when ";
  } else if (testType == ProgramTestType::TestParamGenericCast) {
    program = buildControlFlowProgram(baseProgram, boolVal, typeVal1, typeVal2);
    lineOut += "testParamGenericCast: when ";
  } else if (testType == ProgramTestType::TestExpression) {
    program = buildIfExpressionNoParam(boolVal, typeVal1, typeVal2);
    lineOut += "testExpression: when ";
  } else if (testType == ProgramTestType::TestParamExpression) {
    program = buildIfExpressionParam(boolVal, typeVal1, typeVal2);
    lineOut += "testParamExpression: when ";
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
  std::string testGeneric =
    R"""(
    proc f(arg: bool, type t, type tt) {
      var i: t = 0;
      var u: tt = 0;
      if arg then return i; else return u;
    }
    )""";
  std::string testGenericCast =
    R"""(
    proc f(arg: bool, type t, type tt) {
      var zero: uint(8);
      if arg then return zero:t; else return zero:tt;
    }
    )""";
  std::string testParamGenericCast =
    R"""(
    proc f(arg: bool, type t, type tt) {
      if arg then return 0:t; else return 0:tt;
    }
    )""";

  testIfExpressionIntegralTypesHelper(testGeneric, "true", ProgramTestType::TestGeneric);
  testIfExpressionIntegralTypesHelper(testGeneric, "false", ProgramTestType::TestGeneric);
  testIfExpressionIntegralTypesHelper(testParamGenericCast, "true", ProgramTestType::TestParamGenericCast);
  testIfExpressionIntegralTypesHelper(testParamGenericCast, "false", ProgramTestType::TestParamGenericCast);
  testIfExpressionIntegralTypesHelper(testGenericCast, "true", ProgramTestType::TestGenericCast);
  testIfExpressionIntegralTypesHelper(testGenericCast, "false", ProgramTestType::TestGenericCast);
  testIfExpressionIntegralTypesHelper("", "true", ProgramTestType::TestExpression);
  testIfExpressionIntegralTypesHelper("", "false", ProgramTestType::TestExpression);
  testIfExpressionIntegralTypesHelper("", "true", ProgramTestType::TestParamExpression);
  testIfExpressionIntegralTypesHelper("", "false", ProgramTestType::TestParamExpression);
}

int main() {
  testConditionalIntegralTypes();
  return 0;
}

