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
#include "chpl/types/IntType.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"
#include "chpl/util/version-info.h"

static bool primary = true;

struct IterableType {
  std::string typeName;
  std::vector<std::string> body;
  std::vector<std::string> nearby;

  IterableType(std::string typeName) : typeName(std::move(typeName)) {}

  void pushSignature(const char* fnType, std::string signature) {
    if (primary) {
      body.push_back(std::string(fnType) + " " + signature);
    } else {
      nearby.push_back(std::string(fnType) + " " + typeName + "." + signature);
    }
  }

  IterableType& definePromotionType(const char* type) {
    auto sig = std::string("chpl__promotionType() type do return ") + type + ";";
    pushSignature("proc", std::move(sig));
    return *this;
  }

  IterableType& defineSerialIterator(const char* yieldValue) {
    auto sig = std::string("these() do yield ") + yieldValue + ";";
    pushSignature("iter", std::move(sig));
    return *this;
  }

  IterableType& defineStandaloneIterator(const char* yieldValue) {
    auto sig = std::string("these(param tag) where tag == iterKind.standalone do yield ") + yieldValue + ";";
    pushSignature("iter", std::move(sig));
    return *this;
  }

  IterableType& defineLeaderIterator(const char* yieldValue) {
    auto sig = std::string("these(param tag) where tag == iterKind.leader do yield ") + yieldValue + ";";
    pushSignature("iter", std::move(sig));
    return *this;
  }

  IterableType& defineFollowerIterator(const char* followThisType, const char* yieldValue) {
    auto sig =
      std::string("these(param tag, followThis: ") + followThisType +
      ") where tag == iterKind.follower do yield " + yieldValue + ";";
    pushSignature("iter", std::move(sig));
    return *this;
  }

  std::vector<std::string> strs() {
    std::vector<std::string> result;

    // New:
    result.push_back("record " + typeName + " {");
    for (const auto& line : body)
      result.push_back("  " + line);
    result.push_back("}");
    for (const auto& line : nearby)
      result.push_back(line);

    return result;
  }
};

template <typename... Types, typename F>
static void runProgram(std::vector<const char*> prog, F&& f, Types... types) {
  Context ctx;
  auto context = &ctx;
  ErrorGuard guard(context);

  std::string program = R"""(
module ChapelBase {
  enum iterKind { standalone, leader, follower };
  operator =(ref lhs:int, const rhs:int) {}
  operator =(ref lhs:real, const rhs:real) {}
}
)""";

  auto addType = [&](auto arg) {
    for (const auto& line : arg.strs())
      program += "  " + line + "\n";
  };

  program += "module Main {\n  use ChapelBase;\n";
  (addType(types), ...);
  for (const auto& line : prog)
    program += "  " + std::string(line) + "\n";
  program += "\n}\n";

  printf("\n--- Program ---\n");
  printf("%s", program.c_str());
  printf("\n\n");

  auto vars = resolveTypesOfVariables(context, program, { "i" });
  f(guard, vars.at("i"));
}

template <typename... Types, typename F>
static void runProgram(const char* prog, F&& f, Types... types) {
  runProgram(std::vector<const char*>{prog}, std::forward<F>(f), types...);
}

static void test0() {
  // Promotion gets triggered, but we still need 'these' to make it iterable.
  runProgram(
      { "proc foo(x: int) {}",
        "for i in foo(new R()) {}" },
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
      { "proc foo(x: int) do return x;",
        "for i in foo(new R()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isIntType());
      },
      IterableType("R").definePromotionType("int").defineSerialIterator("1"));
}

static void test2() {
  // Promotion doesn't get triggered even if there is a 'these'
  runProgram(
      { "proc foo(x: int) {}",
        "for i in foo(new R()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(t.isUnknownOrErroneous());
        guard.realizeErrors();
      },
      IterableType("R").defineSerialIterator("1"));
}

static void test3() {
  // The yield type depends on the return type of the function
  runProgram(
      { "proc foo(x: int) do return true;",
        "for i in foo(new R()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isBoolType());
      },
      IterableType("R").definePromotionType("int").defineSerialIterator("1"));
}

static void test4() {
  // Promoting twice works
  runProgram(
      { "proc foo(x: int) do return true;",
        "proc bar(x: bool) do return \"hello\";",
        "for i in bar(foo(new R())) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isStringType());
      },
      IterableType("R").definePromotionType("int").defineSerialIterator("1"));
}

static void test5() {
  // Promoting three times works (for good measure)
  runProgram(
      { "proc foo(x: int) do return true;",
        "proc bar(x: bool) do return \"hello\";",
        "proc baz(x: string) do return 1.0;",
        "for i in baz(bar(foo(new R()))) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isRealType());
      },
      IterableType("R").definePromotionType("int").defineSerialIterator("1"));
}

static void test6() {
  // If the scalar type doesn't match, we don't promote
  runProgram(
      { "proc foo(x: string) do return true;",
        "for i in foo(new R()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(t.isUnknownOrErroneous());
        guard.realizeErrors();
      },
      IterableType("R").definePromotionType("int").defineSerialIterator("1"));
}

static void test7() {
  // If the scalar type matches, but the yield type doesn't, we don't promote.
  runProgram(
      { "proc foo(x: int) do return true;",
        "for i in foo(new R()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(t.isUnknownOrErroneous());
        assert(guard.numErrors() == 1);
        assert(guard.error(0)->type() == ErrorType::NonIterable);
        guard.realizeErrors();
      },
      IterableType("R").definePromotionType("int").defineSerialIterator("1.0"));
}

static void test8() {
  // Promotion respects parallelism allowed by loops (can't invoke parallel loop here)
  runProgram(
      { "proc foo(x: int) do return true;",
        "for i in foo(new R()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(t.isUnknownOrErroneous());
        assert(guard.numErrors() == 1);
        assert(guard.error(0)->type() == ErrorType::NonIterable);
        guard.realizeErrors();
      },
      IterableType("R").definePromotionType("int")
        .defineLeaderIterator("1.0")
        .defineFollowerIterator("real", "1"));
}

static void test9() {
  // Promotion respects parallelism allowed by loops (invoking parallel iterator)
  runProgram(
      { "proc foo(x: int) do return true;",
        "forall i in foo(new R()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isBoolType());
      },
      IterableType("R").definePromotionType("int")
        .defineStandaloneIterator("1"));
}

static void test10() {
  // Promotion respects parallelism allowed by loops (invoking leader/follower iterator)
  runProgram(
      { "proc foo(x: int) do return true;",
        "forall i in foo(new R()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isBoolType());
      },
      IterableType("R").definePromotionType("int")
        .defineLeaderIterator("1.0")
        .defineFollowerIterator("real", "1"));
}

static void test11() {
  // promotion on multiple arguments works (serially)
  runProgram(
      { "proc foo(x: int, y: int) do return true;",
        "for i in foo(new R(), new R()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isBoolType());
      },
      IterableType("R").definePromotionType("int")
        .defineSerialIterator("1"));
}

static void test12() {
  // promotion on multiple arguments works (in parallel)
  runProgram(
      { "proc foo(x: int, y: int) do return true;",
        "forall i in foo(new R(), new R()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isBoolType());
      },
      IterableType("R").definePromotionType("int")
        .defineLeaderIterator("1.0")
        .defineFollowerIterator("real", "1"));
}

static void test13() {
  // promotion on multiple (distinct) arguments works (serially)
  runProgram(
      { "proc foo(x: int, y: real) do return true;",
        "for i in foo(new R(), new S()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isBoolType());
      },
      IterableType("R").definePromotionType("int")
        .defineSerialIterator("1"),
      IterableType("S").definePromotionType("real")
        .defineSerialIterator("1.0"));
}

static void test14() {
  // promotion on multiple (distinct) arguments works (in parallel)
  runProgram(
      { "proc foo(x: int, y: real) do return true;",
        "forall i in foo(new R(), new S()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isBoolType());
      },
      IterableType("R").definePromotionType("int")
        .defineLeaderIterator("\"hello\"")
        .defineFollowerIterator("string", "1"),
      IterableType("S").definePromotionType("real")
        .defineLeaderIterator("\"hello\"")
        .defineFollowerIterator("string", "1.0"));
}

static void test15() {
  // Promotion works when instantiating generics.
  runProgram(
      { "proc foo(x: integral) do return x;",
        "for i in foo(new R()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isIntType());
      },
      IterableType("R").definePromotionType("int").defineSerialIterator("1"));

}

static void test16() {
  // Promotion works when converting
  runProgram(
      { "proc foo(x: int) do return x;",
        "for i in foo(new R()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isIntType());
      },
      IterableType("R").definePromotionType("int(8)").defineSerialIterator("1 : int(8)"));

}

static void test17() {
  // Instantiations from promotion are the scalar type, not the promoted type
  runProgram(
      { "proc foo(x: integral, y: x.type) do return y;",
        "for i in foo(new R(), 12) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isIntType());
      },
      IterableType("R").definePromotionType("int").defineSerialIterator("1"));

}

static void test18() {
  // Two-element promotion instantiation works
  runProgram(
      { "proc foo(x: integral, y: integral) do return (x, y);",
        "for i in foo(new R(), new S()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isTupleType());
        assert(t.type()->toTupleType()->elementType(0).type()->isIntType());
        assert(t.type()->toTupleType()->elementType(1).type()->isIntType());
      },
      IterableType("R").definePromotionType("int").defineSerialIterator("1"),
      IterableType("S").definePromotionType("int").defineSerialIterator("1")
      );

}

static void test19() {
  // promotion on multiple arguments with default arguments
  runProgram(
      { "proc foo(x: int = 1, y: int = 2) do return true;",
        "for i in foo(1, new R()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isBoolType());
      },
      IterableType("R").definePromotionType("int")
        .defineSerialIterator("1"));

  runProgram(
      { "proc foo(x: int = 1, y: int = 2) do return true;",
        "for i in foo(x=new R()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isBoolType());
      },
      IterableType("R").definePromotionType("int")
        .defineSerialIterator("1"));

  runProgram(
      { "proc foo(x: int = 1, y: int = 2) do return true;",
        "for i in foo(y=new R()) {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isBoolType());
      },
      IterableType("R").definePromotionType("int")
        .defineSerialIterator("1"));
}

static void test20() {
  // check that we can promote a field access, as spec'ed.
  runProgram(
      {
        "record pair { var first: int; var second: real; }",
        "proc R.chpl__promotionType() type do return pair;",
        "for i in (new R()).second {}",
      },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isRealType());
      },
      IterableType("R").defineSerialIterator("new pair(0, 0.0)"));
}

static void test21() {
  // Promotion gets triggered for methods
  runProgram(
      { "proc int.foo() do return this;",
        "for i in (new R()).foo() {}" },
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(!t.isUnknownOrErroneous());
        assert(t.type()->isIntType());
      },
      IterableType("R").definePromotionType("int").defineSerialIterator("1"));
}

// You can invoke primary methods on a type even if they are not imported
// into the current scope, because the type's definition scope is considered.
// However, in production, this rule doesn't apply to promoted methods.
// Test that in Dyno, it does (which is more consistent).
//
// Tracking issue in prod: https://github.com/chapel-lang/chapel/issues/27578
static void testPromotedMethodNotImported() {
  auto prog = R"""(
    module M1 {
      record R {
        proc foo() do return 42.0;
      }
    }
    module M2 {
      use M1;

      record S {
        proc chpl__promotionType() type do return R;
        iter these() do yield new R();
      }

      var s = new S();
    }
    module M3 {
      import M2.s;

      var x = s.foo();
    }
  )""";

  auto context = buildStdContext();
  ErrorGuard guard(context);

  setFileText(context, "input.chpl", prog);
  auto& res = parseFileToBuilderResultAndCheck(context, UniqueString::get(context, "input.chpl"), UniqueString());
  assert(res.numTopLevelExpressions() == 3);
  auto M3 = res.topLevelExpression(2)->toModule();
  assert(M3->numStmts() == 2);
  auto xInit = M3->stmt(1)->toVariable()->initExpression();

  auto& rr = resolveModule(context, M3->id());
  auto& re = rr.byAst(xInit);
  assert(!re.type().isUnknownOrErroneous());
  assert(re.type().type()->isPromotionIteratorType());
  assert(re.type().type()->toPromotionIteratorType()->yieldType().type()->isRealType());
}

static void testFieldPromotionScoping() {
  // test that field access promotion works even if the field name itself
  // is not imported / in scope (it should be found in the receiver scopes).
  auto prog = R"""(
    module M1 {
      record point {
        var firstElt: real;
        var secondElt: real;
      }
      var A: [1..5] point;
    }

    module M2 {
      import M1.{A};
      var B = A.firstElt;

      proc main() {
        writeln(B);
      }
    }
  )""";

  auto ctx = buildStdContext();
  auto vars = resolveTypesOfVariables(ctx, prog, { "B" });

  assert(!vars.at("B").isUnknownOrErroneous());
  assert(vars.at("B").type()->isArrayType());
  assert(vars.at("B").type()->toArrayType()->eltType().type()->isRealType());
}

static void testTertiaryMethod() {
  // tertiary method definitions of chpl__promotionType
  // (defined in the scope of iteration) are not allowed.
  runProgram(
      { "{",
        "  proc R.chpl__promotionType() type do return int;",
        "  proc foo(x: int) do return x;",
        "  for i in foo(new R()) {}",
        "}"},
      [](ErrorGuard& guard, const QualifiedType& t) {
        assert(t.isUnknownOrErroneous());
        assert(guard.realizeErrors());
      },
      IterableType("R").defineSerialIterator("1"));

}

static void regressionTestRecursivePromotionTypeBug() {
  // previously, because we allowed promotion to be applied to functions
  // with the type and param intents, it was possible to trigger recursive
  // queries. This PR ensures that no longer happens.

  auto context = buildStdContext();

  auto var = resolveTypeOfX(context,
      R"""(
      use OS;
      var x = createSystemError(0);
      )""");

  assert(var);
  assert(var->isClassType());
  assert(var->toClassType()->basicClassType()->parentClassType() ==
         CompositeType::getErrorType(context)->basicClassType());
}

int main() {
  // Run tests with primary and secondary method definitions (expecting
  // the same results).
  for (auto primarySetting : { true, false }) {
    primary = primarySetting;

    test0();
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
    test21();
  }

  testPromotedMethodNotImported();

  testFieldPromotionScoping();

  testTertiaryMethod();

  regressionTestRecursivePromotionTypeBug();

  return 0;
}
