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
#include "chpl/types/IntType.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"
#include "chpl/util/version-info.h"

struct IterableType {
  std::string typeName;
  std::vector<std::string> body;
  std::vector<std::string> nearby;

  IterableType(std::string typeName) : typeName(std::move(typeName)) {}

  void pushSignature(const char* fnType, std::string signature, bool primary) {
    if (primary) {
      body.push_back(std::string(fnType) + " " + signature);
    } else {
      nearby.push_back(std::string(fnType) + " " + typeName + signature);
    }
  }

  IterableType& definePromotionType(const char* type, bool primary = true) {
    auto sig = std::string("chpl__promotionType() type do return ") + type + ";";
    pushSignature("proc", std::move(sig), primary);
    return *this;
  }

  IterableType& defineSerialIterator(const char* yieldValue) {
    auto sig = std::string("these() do yield ") + yieldValue + ";";
    pushSignature("iter", std::move(sig), true);
    return *this;
  }

  IterableType& defineStandaloneIterator(const char* yieldValue) {
    auto sig = std::string("these(param tag) where tag == iterKind.standalone do yield ") + yieldValue + ";";
    pushSignature("iter", std::move(sig), true);
    return *this;
  }

  IterableType& defineLeaderIterator(const char* yieldValue) {
    auto sig = std::string("these(param tag) where tag == iterKind.leader do yield ") + yieldValue + ";";
    pushSignature("iter", std::move(sig), true);
    return *this;
  }

  IterableType& defineFollowerIterator(const char* followThisType, const char* yieldValue) {
    auto sig =
      std::string("these(param tag, followThis: ") + followThisType +
      ") where tag == iterKind.follower do yield " + yieldValue + ";";
    pushSignature("iter", std::move(sig), true);
    return *this;
  }

  std::string str() {
    std::stringstream ss;
    ss << "record " << typeName << " {\n";
    for (const auto& line : body)
      ss << "  " << line << "\n";
    ss << "}\n";
    for (const auto& line : nearby)
      ss << line << "\n";
    return ss.str();
  }
};

template <typename... Types, typename F>
static void runProgram(const char* prog, F&& f, Types... types) {
  Context ctx;
  auto context = &ctx;
  ErrorGuard guard(context);

  std::string program = "enum iterKind { standalone, leader, follower };\n";
  (program += ... += types.str());
  program += prog;

  printf("\n--- Program ---\n");
  printf("%s", program.c_str());
  printf("\n\n");

  auto vars = resolveTypesOfVariables(context, program, { "i" });
  f(guard, vars.at("i"));
}

static void test0() {
  // Promotion gets triggered, but we still need 'these' to make it iterable.
  runProgram(
      "proc foo(x: int) {}\n"
      "for i in foo(new R()) {}",
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(guard.numErrors() == 1);
        assert(guard.error(0)->type() == ErrorType::NonIterable);
        assert(t.isUnknown());
        guard.realizeErrors();
      },
      IterableType("R").definePromotionType("int"));
}

static void test1() {
  // Promotion gets triggered and we can iterate over the result.
  runProgram(
      "proc foo(x: int) do return x;\n"
      "for i in foo(new R()) {}",
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isIntType());
      },
      IterableType("R").definePromotionType("int").defineSerialIterator("1"));
}

static void test2() {
  // Promotion doesn't get triggered even if there is a 'these'
  runProgram(
      "proc foo(x: int) {}\n"
      "for i in foo(new R()) {}",
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(t.isUnknownOrErroneous());
        guard.realizeErrors();
      },
      IterableType("R").defineSerialIterator("1"));
}

static void test3() {
  // The yield type depends on the return type of the function
  runProgram(
      "proc foo(x: int) do return true;\n"
      "for i in foo(new R()) {}",
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isBoolType());
      },
      IterableType("R").definePromotionType("int").defineSerialIterator("1"));
}

static void test4() {
  // Promoting twice works
  runProgram(
      "proc foo(x: int) do return true;\n"
      "proc bar(x: bool) do return \"hello\";\n"
      "for i in bar(foo(new R())) {}",
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isStringType());
      },
      IterableType("R").definePromotionType("int").defineSerialIterator("1"));
}

static void test5() {
  // Promoting three times works (for good measure)
  runProgram(
      "proc foo(x: int) do return true;\n"
      "proc bar(x: bool) do return \"hello\";\n"
      "proc baz(x: string) do return 1.0;\n"
      "for i in baz(bar(foo(new R()))) {}",
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isRealType());
      },
      IterableType("R").definePromotionType("int").defineSerialIterator("1"));
}

static void test6() {
  // If the scalar type doesn't match, we don't promote
  runProgram(
      "proc foo(x: string) do return true;\n"
      "for i in foo(new R()) {}",
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(t.isUnknownOrErroneous());
        guard.realizeErrors();
      },
      IterableType("R").definePromotionType("int").defineSerialIterator("1"));
}

int main() {
  test0();
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  return 0;
}
