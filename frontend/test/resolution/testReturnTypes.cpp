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
  Context ctx;
  auto context = &ctx;
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
  auto qt = commonTypeResult.getValueOr(QualifiedType());
  std::cout << "return type:" << std::endl;
  qt.dump();
  std::cout << std::endl;
  func(commonTypeResult.hasValue(), qt);
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
  // test returning a param from an ambigous param-returning function
  // non-ambigous tests are in testParamIf.
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
  return 0;
}
