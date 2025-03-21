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

struct ReturnVariant {
  std::string intent;
  std::string type;
  std::string value;

  ReturnVariant(std::string intent, std::string type, std::string value)
    : intent(std::move(intent)), type(std::move(type)), value(std::move(value)) {}
};

static ReturnVariant type(std::string value) {
  return ReturnVariant("type", "", value);
}

static ReturnVariant lit(std::string value) {
  return ReturnVariant("param", "", value);
}

static ReturnVariant decl(std::string intent, std::string type) {
  return ReturnVariant(intent, type, "");
}

static ReturnVariant ref(bool isConst = false) {
  return ReturnVariant(isConst ? "const ref" : "ref", "r", "temp");
}

static bool hasReference(const std::vector<ReturnVariant>& variants) {
  for (auto& variant : variants) {
    if (variant.intent.find("ref") != std::string::npos) {
      return true;
    }
  }
  return false;
}

static std::string buildProgram(const std::vector<ReturnVariant>& variants) {
  std::ostringstream oss;
  if (hasReference(variants)) {
    // references need something to refer to, so create a temporary
    // of a dummy record type.
    oss << "record r {}" << std::endl;
    oss << "var temp: r;" << std::endl;
  }
  for (size_t i = 0; i < variants.size(); i++) {
    const auto& variant = variants[i];
    oss << variant.intent << " arg" << i;
    if (!variant.type.empty()) oss << ": " << variant.type;
    if (!variant.value.empty()) oss << " = " << variant.value;
    oss << ";" << std::endl;
  }

  return oss.str();
}

static std::vector<QualifiedType> extractDefinedTypes(Context* context,
                                                      const char* program,
                                                      size_t startAt) {
  std::vector<QualifiedType> types;
  auto m = parseModule(context, program);
  assert(m->numStmts() > 0);
  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  for (int i = startAt; i < m->numStmts(); i++) {
    auto argName = "arg" + std::to_string(i-startAt);
    auto stmt = m->stmt(i)->toVariable();
    assert(stmt && stmt->name().str() == argName);
    types.push_back(rr.byAst(stmt).type());
  }
  return types;
}

template <typename F>
void testProgram(const std::vector<ReturnVariant>& variants, F func,
                 QualifiedType::Kind kind = QualifiedType::DEFAULT_INTENT) {
  auto context = buildStdContext();
  auto program = buildProgram(variants);
  std::cout << "--- test program ---" << std::endl;
  std::cout << program.c_str() << std::endl;
  // If any references are there the first two statements are not variable
  // declarations we want to include in the commonType test, so ignore them.
  size_t offset = hasReference(variants) ? 2 : 0;
  auto types = extractDefinedTypes(context, program.c_str(), offset);
  KindRequirement requiredKind;
  if (kind != QualifiedType::UNKNOWN) {
    requiredKind = kind;
  }
  auto commonTypeResult = chpl::resolution::commonType(context, types, requiredKind);
#if LLVM_VERSION_MAJOR >= 15
  auto qt = commonTypeResult.value_or(QualifiedType());
#else
  auto qt = commonTypeResult.getValueOr(QualifiedType());
#endif
  std::cout << "return type:" << std::endl;
  qt.dump();
  std::cout << std::endl;
  func((bool) commonTypeResult, qt);
}

static std::string buildControlFlowProgram(std::string controlFlow) {
  std::string program = "";
  program += "proc f() {\n  // Inserted control flow\n";
  program += controlFlow;
  program += "\n  // End inserted control flow\n  return \"hello\";\n}";
  program += "\nvar x = f();";
  return program;
}

enum class ControlFlowResult {
  AllPathsReturn,
  SomePathsReturn,
  FallsThrough,
};

static void testControlFlow(std::string controlFlow, ControlFlowResult expectedResult) {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  auto program = buildControlFlowProgram(controlFlow);
  std::cout << "--- test program ---" << std::endl;
  std::cout << program.c_str() << std::endl;

  auto returnType = resolveTypeOfXInit(context, program,
                                       /* requireTypeKnown */ expectedResult != ControlFlowResult::SomePathsReturn);

  if (expectedResult == ControlFlowResult::AllPathsReturn) {
    // No errors should be emitted.
    // Error guard will emit any unexpected errors when we go out of scope.
    assert(returnType.type());
    assert(returnType.type()->isIntType());
  } else if (expectedResult == ControlFlowResult::FallsThrough) {
    // No errors should be emitted.
    // Error guard will emit any unexpected errors when we go out of scope.
    assert(returnType.type());
    assert(returnType.type()->isStringType());
  } else {
    assert(guard.realizeErrors() > 0);
  }
}

static void test1() {
  // test returning a single value from value-returning function
  testProgram({
      lit("1")
  }, [](bool found, auto& qt) {
    assert(found);
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isIntType());
  });
}

static void test2() {
  // test returning multiple of the same type of values from
  // value-returning function
  testProgram({
      lit("1"),
      lit("2")
  }, [](bool found, auto& qt) {
    assert(found);
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isIntType());
  });
}

static void test3() {
  // test returning a param from a param-returning function
  testProgram({
      lit("1")
  }, [](bool found, auto& qt) {
    assert(found);
    assert(qt.type() && qt.type()->isIntType());
    assert(qt.kind() == QualifiedType::PARAM && qt.param());
    assert(qt.param()->toIntParam()->value() == 1);
  }, QualifiedType::PARAM);
}

static void test4() {
  // test returning a param from an ambiguous param-returning function
  // non-ambiguous tests are in testParamIf.
  testProgram({
      lit("1"),
      lit("2")
  }, [](bool found, auto& qt) {
    assert(!found);
    assert(qt.isUnknown());
  }, QualifiedType::PARAM);
}

static void test5() {
  // test allowed coercions
  testProgram({
      decl("const", "int(32)"),
      decl("const", "int(16)")
  }, [](bool found, auto& qt) {
    assert(found);
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isIntType());
    assert(qt.type()->toIntType()->bitwidth() == 32);
  });
}

static void test6() {
  // test disallowed coercions
  testProgram({
      decl("const", "int(32)"),
      decl("const", "string")
  }, [](bool found, auto& qt) {
    assert(!found);
    assert(qt.isUnknown());
  });
}

static void test7() {
  // test param decaying to var
  testProgram({
      decl("const", "int(32)"),
      lit("1")
  }, [](bool found, auto& qt) {
    assert(found);
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isIntType());
  });
}

static void test8() {
  // test param decaying to var, but for strings
  testProgram({
      decl("const", "string"),
      lit("\"hello\"")
  }, [](bool found, auto& qt) {
    assert(found);
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isStringType());
  });
}

static void test9() {
  // test multiple branches (not just two)
  testProgram({
      decl("const", "int(16)"),
      lit("1"),
      decl("const", "int"),
      decl("const", "int(32)"),
  }, [](bool found, auto& qt) {
    assert(found);
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isIntType());
    assert(qt.type()->toIntType()->bitwidth() == 64);
  });
}

static void test10() {
  // returning types from compile-time unknown function
  testProgram({
      type("int"),
      type("int"),
  }, [](bool found, auto& qt) {
    assert(found);
    assert(qt.kind() == QualifiedType::TYPE);
    assert(qt.type() && qt.type()->isIntType());
    assert(qt.type()->toIntType()->bitwidth() == 64);
  }, QualifiedType::TYPE);
  testProgram({
      type("int"),
      type("bool"),
  }, [](bool found, auto& qt) {
    assert(!found);
    assert(qt.isUnknown());
  }, QualifiedType::TYPE);
}

static void test11() {
  // test mixing types and values
  testProgram({
      type("int"),
      lit("1"),
  }, [](bool found, auto& qt) {
    assert(!found);
    assert(qt.isUnknown());
  }, QualifiedType::TYPE);
  testProgram({
      type("int"),
      lit("1"),
  }, [](bool found, auto& qt) {
    assert(!found);
    assert(qt.isUnknown());
  });
}

static void test12() {
  // test returning non-param from param-intent procedure
  testProgram({
      decl("const", "int"),
      lit("1"),
  }, [](bool found, auto& qt) {
    assert(!found);
    assert(qt.isUnknown());
  }, QualifiedType::PARAM);
}

// ================================================================
// All tests after this point don't assume a specified return intent.
// This is useful for testing commonType for things other than function
// return values.
// ================================================================

static void test13() {
  testProgram({
      ref(/* isConst */ true),
      decl("var", "r"),
  }, [](bool found, auto& qt) {
    assert(found);
    assert(qt.kind() == QualifiedType::CONST_VAR);
    assert(qt.type() && qt.type()->isRecordType());
  }, QualifiedType::UNKNOWN);
}

static void test14() {
  testProgram({
      ref(/* isConst */ true),
      ref(/* isConst */ false),
  }, [](bool found, auto& qt) {
    assert(found);
    assert(qt.kind() == QualifiedType::CONST_REF);
    assert(qt.type() && qt.type()->isRecordType());
  }, QualifiedType::UNKNOWN);
}

static void test15() {
  testProgram({
      ref(/* isConst */ false),
      decl("var", "r"),
  }, [](bool found, auto& qt) {
    assert(found);
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type() && qt.type()->isRecordType());
  }, QualifiedType::UNKNOWN);
}

static void test16() {
  testProgram({
      lit("1"),
      decl("var", "int"),
  }, [](bool found, auto& qt) {
    assert(found);
    assert(qt.kind() == QualifiedType::CONST_VAR);
    assert(qt.type() && qt.type()->isIntType());
  }, QualifiedType::UNKNOWN);
}

static void test17() {
  testProgram({
      lit("1"),
      lit("1"),
  }, [](bool found, auto& qt) {
    assert(found);
    assert(qt.kind() == QualifiedType::PARAM);
    assert(qt.type() && qt.type()->isIntType());
    assert(qt.param() && qt.param()->toIntParam()->value() == 1);
  }, QualifiedType::UNKNOWN);
}

static void test18() {
  testProgram({
      lit("1"),
      lit("2"),
  }, [](bool found, auto& qt) {
    assert(found);
    assert(qt.kind() == QualifiedType::CONST_VAR);
    assert(qt.type() && qt.type()->isIntType());
  }, QualifiedType::UNKNOWN);
}

// Subsequent tests execute some control flow before running `return "hello"`.
// The purpose of the tests is to ensure that we correctly identify instances
// where all paths return, and subsequent return statements (like the one
// with "hello") are ignored.

static void testControlFlow0() {
  testControlFlow(
      R"""(
      )"""
  , ControlFlowResult::FallsThrough);
}

static void testControlFlow1() {
  testControlFlow(
      R"""(
      return 1;
      )"""
  , ControlFlowResult::AllPathsReturn);
}

static void testControlFlow2() {
  testControlFlow(
      R"""(
      var x = true;
      if (x) {
          return 1;
      }
      )"""
  , ControlFlowResult::SomePathsReturn);
}

static void testControlFlow3() {
  testControlFlow(
      R"""(
      var x = true;
      if (x) {
          return 1;
      } else {
          return 2;
      }
      )"""
  , ControlFlowResult::AllPathsReturn);
}

static void testControlFlow4() {
  testControlFlow(
      R"""(
      var x, y = true;
      if (x) {
          if (y) {
              return 1;
          } else {

          }
      } else {
          return 3;
      }
      )"""
  , ControlFlowResult::SomePathsReturn);
}

static void testControlFlow5() {
  testControlFlow(
      R"""(
      var x, y = true;
      if (x) {
          if (y) {
              return 1;
          } else {
              return 2;
          }
      } else {
          return 3;
      }
      )"""
  , ControlFlowResult::AllPathsReturn);
}

static void testControlFlow6() {
  testControlFlow(
      R"""(
      try! {
          return 1;
      }
      )"""
  , ControlFlowResult::AllPathsReturn);
}

static void testControlFlow7() {
  testControlFlow(
      R"""(
      try {
          return 1;
      } catch {

      }
      )"""
  , ControlFlowResult::SomePathsReturn);
}

static void testControlFlow8() {
  testControlFlow(
      R"""(
      try {
          return 1;
      } catch {
          return 2;
      }
      )"""
  , ControlFlowResult::AllPathsReturn);
}

static void testControlFlow9() {
  testControlFlow(
      R"""(
      class MyError: Error {}
      try {
          return 1;
      } catch e : MyError {
          return 2;
      } catch {

      }
      )"""
  , ControlFlowResult::SomePathsReturn);
}

static void testControlFlow10() {
  testControlFlow(
      R"""(
      class MyError: Error {}
      try {
          return 1;
      } catch e : MyError {
          return 2;
      } catch {
          return 3;
      }
      )"""
  , ControlFlowResult::AllPathsReturn);
}

static void testControlFlow11() {
  testControlFlow(
      R"""(
      while false {
          return 1;
      }
      )"""
  , ControlFlowResult::SomePathsReturn);
}

static void testControlFlow12() {
  testControlFlow(
      R"""(
      if true {
        return 1;
      }
      )"""
  , ControlFlowResult::AllPathsReturn);
}

static void testControlFlow13() {
  testControlFlow(
      R"""(
      if false {
        return "hello";
      } else {
        return 1;
      }
      )"""
  , ControlFlowResult::AllPathsReturn);
}

static void testControlFlow14() {
  testControlFlow(
      R"""(
      if false {
        return 1;
      }
      )"""
  , ControlFlowResult::FallsThrough);
}

static void testControlFlow15() {
  testControlFlow(
      R"""(
      if true {

      } else {
        return 1;
      }
      )"""
  , ControlFlowResult::FallsThrough);
}

static void testControlFlow16() {
  testControlFlow(
      R"""(
      if true then return 1;

      var b = false;
      if b then /* fall through to the default return */
      )"""
  , ControlFlowResult::AllPathsReturn);
}

static void testControlFlowYield1() {
  auto program = R"""(
    iter myIter() {
      yield 1;
      yield "hello";
    }
    )""";

  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto mod = parseModule(context, program);
  assert(mod);
  auto child = mod->stmt(0);
  assert(child);
  auto fn = child->toFunction();
  assert(fn);

  std::ignore = resolveConcreteFunction(context, fn->id());
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->message() == "could not determine return type for function");

  // Already checked expected errors above.
  guard.realizeErrors();
}

// returning without a value in an iterator is allowed.
static void testControlFlowYield2() {
  auto program = R"""(
    iter myIter() {
      yield 1;
      return;
      yield "hello";
    }
    )""";

  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto mod = parseModule(context, program);
  assert(mod);
  auto child = mod->stmt(0);
  assert(child);
  auto fn = child->toFunction();
  assert(fn);

  auto resolvedFunction = resolveConcreteFunction(context, fn->id());
  assert(resolvedFunction->returnType().type());
  assert(resolvedFunction->returnType().type()->isIntType());
}

// returning with a value is not allowed in an iterator.
static void testControlFlowYield3() {
  auto program = R"""(
    iter myIter() {
      yield 1;
      return 2;
      yield 3;
    }
    )""";

  Context ctx;
  auto context = &ctx;
  ErrorGuard guard(context);

  auto mod = parseModule(context, program);
  assert(mod);
  auto child = mod->stmt(0);
  assert(child);
  auto fn = child->toFunction();
  assert(fn);

  std::ignore = resolveConcreteFunction(context, fn->id());
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->type() == ErrorType::DisallowedControlFlow);

  // Already checked expected errors above.
  guard.realizeErrors();
}

static void testControlFlowYield4() {
  auto program = R"""(
    proc myIter() {
      yield 1;
    }
    )""";

  Context ctx;
  auto context = &ctx;
  ErrorGuard guard(context);

  auto mod = parseModule(context, program);
  assert(mod);
  auto child = mod->stmt(0);
  assert(child);
  auto fn = child->toFunction();
  assert(fn);

  std::ignore = resolveConcreteFunction(context, fn->id());
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->type() == ErrorType::DisallowedControlFlow);

  // Already checked expected errors above.
  guard.realizeErrors();
}

static void testSelectVals() {
  {
    // Basic test case
    Context* context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
    proc foo(arg:int) {
      select arg {
        when 1 do return 1;
        when 2 do return 2;
        when 3 do return 3;
        otherwise do return 0;
      }
    }

    var x = foo(1);
    )""";
    QualifiedType qt = resolveTypeOfXInit(context,
                                         program);
    assert(qt.type()->isIntType());
  }
  {
    // Recognize that cases do not all return the same type
    Context* context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
    proc foo(arg:int) {
      select arg {
        when 1 do return 1;
        when 2 do return 2;
        when 3 do return "hello";
        otherwise do return 0;
      }
    }

    var x = foo(1);
    )""";
    QualifiedType qt = resolveTypeOfXInit(context,
                                         program);
    assert(qt.isErroneousType());
    assert(guard.numErrors() == 1);
    assert(guard.error(0)->message() == "could not determine return type for function");
    guard.realizeErrors();
  }
  {
    // Recognize that all cases return, so the final 'return' isn't considered.
    Context* context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
    proc foo(arg:int) {
      select arg {
        when 1 do return 1;
        when 2 do return 2;
        when 3 do return 3;
        otherwise do return 0;
      }

      return "hello";
    }

    var x = foo(1);
    )""";
    QualifiedType qt = resolveTypeOfXInit(context,
                                         program);
    assert(qt.type()->isIntType());
  }
  {
    // Without an 'otherwise', we should consider the final 'return'.
    Context* context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
    proc foo(arg:int) {
      select arg {
        when 1 do return 1;
        when 2 do return 2;
        when 3 do return 3;
      }

      return "hello";
    }

    var x = foo(1);
    )""";
    QualifiedType qt = resolveTypeOfXInit(context,
                                         program);
    assert(qt.isErroneousType());
    assert(guard.numErrors() == 1);
    assert(guard.error(0)->message() == "could not determine return type for function");
    guard.realizeErrors();
  }
}

using stringMap = std::map<std::string, std::string>;

static void testSelectCases(std::string base,
                            stringMap vals,
                            bool isType = true) {
  for (auto pair : vals) {
    std::string kind = isType ? "type" : "var";
    std::string program = base + kind + " x = foo(" + pair.first + ");";

    Context* context = buildStdContext();
    ErrorGuard guard(context);
    QualifiedType qt = resolveTypeOfXInit(context, program);
    std::stringstream ss;
    qt.type()->stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
    if (ss.str() != pair.second) {
      printf("[DEBUG] %s != %s\n", ss.str().c_str(), pair.second.c_str());
    }
    assert(ss.str() == pair.second);
  }
}

static void testSelectTypes() {
  {
    // basic type usage
    std::string fooFunc = R"""(
    proc foo(type T) type {
      select T {
        when int do return int;
        when real do return real;
        when string do return string;
        otherwise do return complex;
      }
    }
    )""";

    stringMap vals = {{"int", "int(64)"},
                      {"real", "real(64)"},
                      {"string", "string"},
                      {"uint", "complex(128)"}};

    testSelectCases(fooFunc, vals);
  }
  {
    // multiple cases in a single 'when'
    std::string fooFunc = R"""(
    proc foo(type T) type {
      select T {
        when int, uint, real do return int;
        when string, bytes do return string;
        otherwise do return nothing;
      }
    }
    )""";

    stringMap vals = {{"int", "int(64)"},
                      {"uint", "int(64)"},
                      {"real", "int(64)"},
                      {"string", "string"},
                      {"bytes", "string"},
                      {"bool", "nothing"}};

    testSelectCases(fooFunc, vals);
  }
  {
    // demonstrate that cases are ignored after first param-true
    std::string program = R"""(
    proc foo(type T) type {
      select T {
        when int do return int;
        when real {
          badCall(0); // should never attempt to resolve this
          return real;
        }
        otherwise {
          anotherBadCall(1);
          return complex;
        }
      }
    }
    type x = foo(int);
    )""";

    Context* context = buildStdContext();
    ErrorGuard guard(context);
    auto qt = resolveTypeOfXInit(context, program);
    assert(qt.type()->isIntType());
  }
  {
    // demonstrate that in the case of duplicates, the first is always chosen.
    std::string program = R"""(
    proc foo(type T) type {
      select T {
        when int do return int;
        when int do return real;
        when int do return string;
        otherwise do return nothing;
      }
    }
    type x = foo(int);
    )""";

    auto context = buildStdContext();
    ErrorGuard guard(context);
    auto qt = resolveTypeOfXInit(context, program);
    assert(qt.type()->isIntType());
  }
  {
    std::string fooFunc = R"""(
    proc foo(type T) {
      select T {
        when int do return 5;
        when real do return 42.0;
        when string do return "hello";
      }

      var x : T;
      return x;
    }
    )""";

    stringMap vals = {{"int", "int(64)"},
                      {"real", "real(64)"},
                      {"string", "string"},
                      {"uint", "uint(64)"}};

    testSelectCases(fooFunc, vals, /*isType=*/false);
  }
  {
    // demonstrate that when blocks can have multiple 
    // statements without otherwise
    std::string fooFunc = R"""(
    proc foo(type T) {
      var x : int;
      select T {
        when int {
          var x: int;
          return x;
        }
      }

      var y : T;
      return y;
    }
    )""";
    stringMap vals = {{"int", "int(64)"},
                      {"string", "string"}
                      };

    testSelectCases(fooFunc, vals, /*isType=*/false);
  }
  {
    // demonstrate that when blocks can have multiple 
    // statements with otherwise
    std::string fooFunc = R"""(
    proc foo(type T) {
      var x : int;
      select T {
        when int {
          var x: int;
          return x;
        }
        otherwise {}
      }
      var y : real;
      return y;
    }
    )""";
    stringMap vals = {{"int", "int(64)"},
                      {"string", "real(64)"}
                      };

    testSelectCases(fooFunc, vals, /*isType=*/false);
  }
}

static void testSelectParams() {
  {
    // basic param usage
    std::string fooFunc = R"""(
    proc foo(param p) type {
      select p {
        when 1 do return int;
        when 2 do return real;
        when 3 do return string;
        otherwise do return nothing;
      }
    }
    )""";

    stringMap vals = {{"1", "int(64)"},
                      {"2", "real(64)"},
                      {"3", "string"},
                      {"4", "nothing"},
                      {"0", "nothing"}};

    testSelectCases(fooFunc, vals);
  }
  {
    // multiple cases in a single 'when'
    std::string fooFunc = R"""(
    proc foo(param p) type {
      select p {
        when 1, 2, 3 do return int;
        when 4, 5, 6 do return real;
        otherwise do return nothing;
      }
    }
    )""";

    stringMap vals = {{"1", "int(64)"},
                      {"2", "int(64)"},
                      {"3", "int(64)"},
                      {"4", "real(64)"},
                      {"5", "real(64)"},
                      {"6", "real(64)"},
                      {"0", "nothing"}};

    testSelectCases(fooFunc, vals);
  }
  {
    // Show that 'otherwise' should still resolve when a param-true case is
    // not present.
    std::string program = R"""(
    var myGlobal = 100;
    proc foo(param p) type {
      select p {
        when 1 do return int; // always false for this test
        when myGlobal do return real;
        otherwise do return nothing;
      }
    }
    type x = foo(5);
    )""";

    Context* context = buildStdContext();
    ErrorGuard guard(context);
    auto qt = resolveTypeOfXInit(context, program);

    assert(qt.isErroneousType());
    assert(guard.numErrors() == 1);
    assert(guard.error(0)->message() == "could not determine return type for function");
    guard.realizeErrors();
  }
  {
    // Show that non-param cases *preceding* param-true cases should still
    // resolve, in case their value matches at execution time.
    std::string program = R"""(
    var myGlobal = 100;
    proc foo(param p) type {
      select p {
        when myGlobal do return int;
        when 1 do return string; // always true for this test
        otherwise do return nothing;
      }
    }
    type x = foo(1);
    )""";

    Context* context = buildStdContext();
    ErrorGuard guard(context);
    auto qt = resolveTypeOfXInit(context, program);

    assert(qt.isErroneousType());
    assert(guard.numErrors() == 1);
    assert(guard.error(0)->message() == "could not determine return type for function");
    guard.realizeErrors();
  }
}

// Assumes loop body will return param string "asdf"
static void paramLoopTestHelper(const char* loopBody,
                                const char* loopRange, bool returnedFromLoop,
                                bool useEmptyLoop = false) {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  // Construct test program
  std::ostringstream oss;
  oss << "proc foo() param {\n";
  oss << "  for param i in " << loopRange << " {\n";
  oss << loopBody;
  oss << "  }\n";
  oss << "  return true;\n";
  oss << "}\n";
  oss << "param x = foo();\n";
  std::string program = oss.str();

  std::cout << "Param loop test program:\n";
  std::cout << program.c_str() << "\n";

  QualifiedType qt = resolveTypeOfXInit(context,
                                       program);
  assert(!qt.isUnknownOrErroneous());
  if (returnedFromLoop) {
    ensureParamString(qt, "asdf");
  } else {
    ensureParamBool(qt, true);
  }

  assert(guard.realizeErrors() == 0);

  std::cout << "success\n";
}

// Test returns from within param for loops
static void testParamLoop() {
  // Basic case
  paramLoopTestHelper(R"""(
                      return "asdf";
                      )""",
                      "0..2",
                      true);

  // Return statement in else branch taken
  paramLoopTestHelper(R"""(
                      if i == 1 {
                        return true;
                      } else {
                        return "asdf";
                      }
                      )""",
                      "0..2",
                      true);

  // Return statement in param TRUE if
  paramLoopTestHelper(R"""(
                      if i == 1 {
                        return "asdf";
                      }
                      )""",
                      "0..2",
                      true);

  // Return statement in param FALSE if
  paramLoopTestHelper(R"""(
                      if i == 3 {
                        return "asdf";
                      }
                      )""",
                      "0..2",
                      false);

  // Return statement in a param loop with no iterations
  paramLoopTestHelper(R"""(
                      return "asdf";
                      )""",
                      "1..0",
                      false,
                      /* useEmptyLoop */ true);

  // Return statement in iteration after break
  paramLoopTestHelper(R"""(
                      break;
                      return "asdf";
                      )""",
                      "0..2",
                      false);

  // Return statement in iteration after continue
  paramLoopTestHelper(R"""(
                      continue;
                      return "asdf";
                      )""",
                      "0..2",
                      false);

  // Return statement in iteration after a conditional break, only after return
  paramLoopTestHelper(R"""(
                      if i == 1 {
                        break;
                      }
                      return "asdf";
                      )""",
                      "0..2",
                      true);

  // Return statement in iteration after a conditional break, before any return
  paramLoopTestHelper(R"""(
                      if i == 0 {
                        break;
                      }
                      return "asdf";
                      )""",
                      "0..2",
                      false);

  // Return statement in iteration after a conditional continue, in only some
  // iterations
  paramLoopTestHelper(R"""(
                      if i == 0 {
                        continue;
                      }
                      return "asdf";
                      )""",
                      "0..2",
                      true);

  // Return statement in iteration after a conditional continue, in all iterations
  paramLoopTestHelper(R"""(
                      if i != 3 {
                        continue;
                      }
                      return "asdf";
                      )""",
                      "0..2",
                      false);
}

// Test return from within non-param loop (shouldn't work)
static void testNonParamLoop() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program = R"""(
  proc foo() {
    for i in 0..2 {
      return "asdf";
    }
    return true;
  }
  var x = foo();
  )""";
  QualifiedType qt = resolveTypeOfXInit(context,
                                       program);

  assert(qt.isErroneousType());
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->message() == "could not determine return type for function");
  guard.realizeErrors();
}

static void testCPtrEltType() {
  { 
    //works for c_ptr
    std::string program = R"""(
    use CTypes;
    var y: c_ptr(uint(8));
    type x = y.eltType;
    )""";

    Context* context = buildStdContext();
    ErrorGuard guard(context);
    auto qt = resolveTypeOfXInit(context, program);
    assert(qt.type()->isUintType());
    assert(qt.type()->toUintType()->bitwidth() == 8);
  }
  return;
  { 
    //works for user-defined class 
    std::string program = R"""(
    use CTypes;
    class c_ptr2 {
      type eltType;
    }
    var y: c_ptr2(uint(8));
    type x = y.eltType;
    )""";

    Context* context = buildStdContext();
    ErrorGuard guard(context);
    auto qt = resolveTypeOfXInit(context, program);
    assert(qt.type()->isUintType());
    assert(qt.type()->toUintType()->bitwidth() == 8);
  }
}

static void testChildClassesHelper(const char* decls,
                                   const std::vector<const char*>& classTypes,
                                   const char* commonParent,
                                   bool withNilable = false,
                                   const char* intent = "",
                                   int checkParam = -1) {
  // Construct test program
  std::ostringstream oss;
  oss << decls << "\n";
  oss << "proc test(x : int = 0) " << intent << " {\n";
  oss << "select x {\n";
  for (size_t i = 0; i < classTypes.size(); i++) {
    const char* classType = classTypes[i];
    // Skip adding parens to 'new' call if already present
    const char* parensPart = strchr(classType, '(') ? "" : "()";
    if (i < classTypes.size() - 1) {
      oss << "\twhen " << i << " do return new " << classType << parensPart
          << ";\n";
    } else {
      oss << "\totherwise do return new " << classType << parensPart
          << (withNilable ? "?" : "") << ";\n";
    }
  }
  oss << R"""(
  }
}
var x = test();
        )""";
  std::string program = oss.str();

  // Print test program for debugging purposes
  std::cout << "\nTest program:\n";
  std::cout << program << "\n";
  if (commonParent)
    std::cout << "Expecting common parent '" << commonParent << "'\n";
  else
    std::cout << "Expecting no common parent (error)\n";

  // Setup to test
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  // Resolve program and test expected results
  auto qt = resolveTypeOfXInit(context, program);
  if (commonParent) {
    auto ct = qt.type()->toClassType();
    assert(ct);
    assert(ct->manager() && ct->manager()->isAnyOwnedType());
    if (withNilable) {
      assert(ct->decorator().isNilable());
    } else {
      assert(ct->decorator().isNonNilable());
    }
    auto mt = ct->manageableType();
    assert(mt);
    auto bct = mt->toBasicClassType();
    assert(bct);
    assert(bct->name() == commonParent);
    if (checkParam >= 0)
      ensureParamInt(bct->sortedSubstitutions().front().second, checkParam);

    assert(guard.realizeErrors() == 0);
  } else {
    assert(qt.isErroneousType());

    assert(guard.realizeErrors() == 1);
  }

  std::cout << "success\n";
}

static void testChildClasses() {
  // Shared direct parent
  testChildClassesHelper(R"""(
                         class Parent {}
                         class A : Parent {}
                         class B : Parent {}
                         )""",
                         {"A", "B"}, "Parent");

  // No shared parent
  testChildClassesHelper(R"""(
                         class A {}
                         class B {}
                         )""",
                         {"A", "B"}, nullptr);

  // Some but not all shared parent
  testChildClassesHelper(R"""(
                         class Parent {}
                         class A : Parent {}
                         class B : Parent {}
                         class C {}
                         )""",
                         {"A", "B", "C"}, nullptr);

  // Shared ancestor at different depths in inheritance tree
  testChildClassesHelper(R"""(
                         class Grandparent {}
                         class Parent : Grandparent {}
                         class A : Parent {}
                         class B : Parent {}
                         class C : Grandparent {}
                         )""",
                         {"A", "B", "C"}, "Grandparent");

  // Multiple shared ancestors (should pick closest relation)
  testChildClassesHelper(R"""(
                         class Grandparent {}
                         class Parent : Grandparent {}
                         class A : Parent {}
                         class B : Parent {}
                         class C : Parent {}
                         )""",
                         {"A", "B", "C"}, "Parent");

  // Shared ancestor, but ref intent
  testChildClassesHelper(R"""(
                         class Parent {}
                         class A : Parent {}
                         class B : Parent {}
                         )""",
                         {"A", "B"}, nullptr, false, "ref");

  // Shared parent, need nilability
  testChildClassesHelper(R"""(
                         class Parent {}
                         class A : Parent {}
                         class B : Parent {}
                         )""",
                         {"A", "B"}, "Parent", true);

  // Shared generic parent, matching type
  testChildClassesHelper(R"""(
                         class Parent { param x : int; }
                         class A : Parent(?) {}
                         class B : Parent(?) {}
                         )""",
                         {"A(1)", "B(1)"}, "Parent", false, "", 1);

  // Shared generic parent, non matching type
  testChildClassesHelper(R"""(
                         class Parent { param x : int; }
                         class A : Parent(?) {}
                         class B : Parent(?) {}
                         )""",
                         {"A(1)", "B(2)"}, nullptr);
}

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
  test16();
  test17();
  test18();

  testControlFlow0();
  testControlFlow1();
  testControlFlow2();
  testControlFlow3();
  testControlFlow4();
  testControlFlow5();
  testControlFlow6();
  testControlFlow7();
  testControlFlow8();
  testControlFlow9();
  testControlFlow10();
  testControlFlow11();
  testControlFlow12();
  testControlFlow13();
  testControlFlow14();
  testControlFlow15();
  testControlFlow16();

  testControlFlowYield1();
  testControlFlowYield2();
  testControlFlowYield3();
  testControlFlowYield4();

  testSelectVals();
  testSelectTypes();
  testSelectParams();

  testParamLoop();
  testNonParamLoop();

  testCPtrEltType();

  testChildClasses();

  return 0;
}
