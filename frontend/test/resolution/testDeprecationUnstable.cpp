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
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"
#include "./ErrorGuard.h"

#define TEST_NAME(ctx__)\
  chpl::UniqueString::getConcat(ctx__, __FUNCTION__, ".chpl")

static std::string debugDeclName = "";

static void testDebuggingBreakpoint() {}

static CompilerFlags warnUnstableFlags() {
  static auto flags = []() {
    CompilerFlags flags;
    flags.set(CompilerFlags::WARN_UNSTABLE, true);
    return flags;
  }();
  return flags;
}

static Context*
buildStdContextWithUnstableWarnings() {
  auto context = buildStdContext(warnUnstableFlags());
  assert(isCompilerFlagSet(context, CompilerFlags::WARN_UNSTABLE));
  return context;
}

static Context* turnOnWarnUnstable(Context* context) {
  setCompilerFlags(context, warnUnstableFlags());
  assert(isCompilerFlagSet(context, CompilerFlags::WARN_UNSTABLE));
  return context;
}

static const AstNode*
mentionAstFromExpression(const AstNode* ast, bool isReceiver) {
  if (ast == nullptr) return nullptr;

  if (auto ident = ast->toIdentifier()) {
    return ident;
  } else if (auto dot = ast->toDot()) {
    if (isReceiver) return dot->receiver();
    return dot;
  } else if (auto call = ast->toCall()) {
    return mentionAstFromExpression(call->calledExpression(), isReceiver);
  } else if (auto newExpr = ast->toNew()) {
    return mentionAstFromExpression(newExpr->typeExpression(), isReceiver);
  }

  return nullptr;
}

static UniqueString
mentionNameFromExpression(const AstNode* ast, bool isReceiver) {
  if (auto ident = ast->toIdentifier()) {
    return ident->name();
  } else if (auto dot = ast->toDot()) {
    if (isReceiver) {
      return mentionNameFromExpression(dot->receiver(), isReceiver);
    } else {
      return dot->field();
    }
  }
  return UniqueString();
}

static std::vector<const NamedDecl*> collectAllNamedDecls(const AstNode* ast) {
  std::vector<const NamedDecl*> ret;
  for (auto ast : ast->children()) {
    auto v = collectAllNamedDecls(ast);
    ret.insert(ret.end(), v.begin(), v.end());
  }
  if (auto nd = ast->toNamedDecl()) ret.push_back(nd);
  return ret;
}


static void
assertMatchesWarningPattern(const ErrorGuard& guard,
                            const Module* mod,
                            std::string declName,
                            std::string nameForAstContainingMention,
                            std::string warningLabel,
                            bool isDefaultMessage,
                            bool isMentionInUseImport) {
  if (declName == debugDeclName) testDebuggingBreakpoint();
  bool isReceiver = false;

  CHPL_ASSERT(declName.size() > 0);
  if (declName[declName.size() - 1] == '.') {
    declName.erase(declName.end() - 1);
    isReceiver = true;
  }

  const NamedDecl* named = nullptr;
  const AstNode* astContainingMention = nullptr;
  auto namedDecls = collectAllNamedDecls(mod);

  // Unpack the declaration.
  for (auto nd : namedDecls) {
    if (nd->name() == declName) {
      assert(named == nullptr);
      named = nd;
      break;
    }
  }

  // Maybe unpack the mention.
  for (auto nd : namedDecls) {
    if (nd->name() == nameForAstContainingMention) {
      assert(astContainingMention == nullptr);
      astContainingMention = nd;
      break;
    }
  }

  assert(named);
  assert(astContainingMention); // TODO: Remove for use/import.

  const AstNode* mention = nullptr;

  // Now hone in on the exact point of mention.
  if (astContainingMention->isUse() || astContainingMention->isImport()) {
    assert(false && "Not handled yet!");

  } else if (auto varLike = astContainingMention->toVarLikeDecl()) {
    auto initExpr = varLike->initExpression();
    assert(initExpr);

    // Make sure the mention point is correct.
    mention = mentionAstFromExpression(initExpr, isReceiver);

  } else {
    assert(false && "Not handled!");
  }

  assert(mention);

  // Confirm that the mention point matches.
  if (mention->isIdentifier() || mention->isDot()) {
    auto nameForMentionAst = mentionNameFromExpression(mention, isReceiver);
    assert(!nameForMentionAst.isEmpty());
    assert(nameForMentionAst == named->name());
  } else {
    assert(false && "Not handled!");
  }

  // Get the corresponding error.
  auto mentionLoc = locateAst(guard.context(), mention);
  const ErrorBase* err = nullptr;

  for (auto& e : guard.errors()) {
    auto loc = e->location(guard.context());
    if (loc == mentionLoc) {
      err = e.get();
      break;
    }
  }
  assert(err);

  // The expected default error message.
  std::string defaultMsg =
      std::string(named->name().c_str()) + " is " + warningLabel;

  // Check some things about the error.
  assert(err->message().size() != 0);
  if (err->message() == defaultMsg) {
    assert(isDefaultMessage);
  } else {
    assert(err->message()[0] == '-');
    assert(err->message().find(named->name().c_str()));
    assert(err->message().find(warningLabel));
  }
}

static void assertIsDeprecated(const ErrorGuard& guard,
                               const Module* mod,
                               std::string declName,
                               int varNum,
                               bool isDefaultMessage) {
  auto nameForAstContainingMention = std::string("v") + std::to_string(varNum);
  bool isMentionInUseImport = false;
  assertMatchesWarningPattern(guard, mod, declName,
                              nameForAstContainingMention,
                              "deprecated",
                              isDefaultMessage,
                              isMentionInUseImport);
}

// Test deprecation warning coverage across different symbol types.
static void test0(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
    module testDeprecationWarningsForTypes {

      @deprecated
      record r1 { var x: int; }
      @deprecated(notes="- The record r2 is deprecated")
      record r2 { var y: int; }

      @deprecated
      class c1 { var a: int; }
      @deprecated(notes="- The class c2 is deprecated")
      class c2 { var b: int; }

      @deprecated
      union u1 { var f: int; }
      @deprecated(notes="- The union u2 is deprecated")
      union u2 { var g: int; }

      @deprecated
      config const foo1 = 0;
      @deprecated(notes="- The config const foo2 is deprecated")
      config const foo2 = 0;

      @deprecated
      config param bar1 = 0;
      @deprecated(notes="- The config param bar2 is deprecated")
      config param bar2 = 0;

      @deprecated
      param baz1 = 0;
      @deprecated(notes="- The param baz2 is deprecated")
      param baz2 = 0;

      @deprecated
      module mod1 {}
      @deprecated(notes="- The module mod2 is deprecated")
      module mod2 {}

      enum e1 {
        e1k1,
        @deprecated
        e1k2,
        @deprecated(notes="- The enum element e1k3 is deprecated")
        e1k3,
        e1k4
      }

      @deprecated
      enum e2 { e2k1 }

      @deprecated(notes="- The enum e3 is deprecated")
      enum e3 { e3k1 }

      var v1 = new r1();
      var v2 = new r2();
      var v3 = new c1();
      var v4 = new c2();
      var v5 = new u1();
      var v6 = new u2();
      var v7 = foo1;
      var v8 = foo2;
      var v9 = bar1;
      var v10 = bar2;
      var v11 = baz1;
      var v12 = baz2;

      {
        use mod1;
        use mod2;
      }

      {
        import this.mod1;
        import this.mod2;
      }

      var v13 = e1.e1k1;  // OK
      var v14 = e1.e1k2;
      var v15 = e1.e1k3;
      var v16 = e1.e1k4;  // OK

      var v17 = e2.e2k1;
      var v18 = e3.e3k1;
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Scope-resolve the module.
  std::ignore = resolveModule(ctx, mod->id());

  // TODO: Helpers to make sure the errors are correct.
  assert(guard.numErrors() > 0);
  for (auto& err : guard.errors()) {
    assert(err->type() == ErrorType::Deprecation);
  }

  const bool isDefault = true;
  int vn = 1;

  assertIsDeprecated(guard, mod, "r1", vn++, isDefault);
  assertIsDeprecated(guard, mod, "r2", vn++, !isDefault);
  assertIsDeprecated(guard, mod, "c1", vn++, isDefault);
  assertIsDeprecated(guard, mod, "c2", vn++, !isDefault);
  assertIsDeprecated(guard, mod, "u1", vn++, isDefault);
  assertIsDeprecated(guard, mod, "u2", vn++, !isDefault);
  assertIsDeprecated(guard, mod, "foo1", vn++, isDefault);
  assertIsDeprecated(guard, mod, "foo2", vn++, !isDefault);
  assertIsDeprecated(guard, mod, "bar1", vn++, isDefault);
  assertIsDeprecated(guard, mod, "bar2", vn++, !isDefault);
  assertIsDeprecated(guard, mod, "baz1", vn++, isDefault);
  assertIsDeprecated(guard, mod, "baz2", vn++, !isDefault);

  vn++;   // Skip 'k1'
  assertIsDeprecated(guard, mod, "e1k2", vn++, isDefault);
  assertIsDeprecated(guard, mod, "e1k3", vn++, !isDefault);
  vn++;   // Skip 'k4'
  assertIsDeprecated(guard, mod, "e2.", vn++, isDefault);
  assertIsDeprecated(guard, mod, "e3.", vn++, !isDefault);

  assert(guard.realizeErrors());
  std::cout << std::endl;
}

// Test deprecation coverage within use/import statements.
static void test1(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
    @deprecated(notes="- The module foo is deprecated")
    module foo {
      @deprecated
      var x: int;
      @deprecated(notes="The variable y is deprecated")
      var y: int;
      var z: int;
    }

    module bar {
      @deprecated(notes="The variable a is deprecated")
      var a: int;
    }

    module baz {
      @deprecated(notes="The proc p is deprecated")
      proc p() {}
      @deprecated(notes="The parenless proc f is deprecated")
      proc f { return 0; }
    }

    module ding {
      @deprecated(notes="The proc p(x: int) is deprecated")
      proc p(x: int) {}
      @deprecated(notes="The proc p(x: uint) is deprecated")
      proc p(x: uint) {}
      @deprecated(notes="The proc p(x: real) is deprecated")
      proc p(x: real) {}
    }

    module testDeprecationWarningsForUseImport {

      // Deprecated entries in 'only' lists.
      {
        use foo only x, y, z;
        use bar only a;
      }

      // Deprecated entries in import 'braces' lists.
      {
        import foo.{x, y, z};
        import bar.{a};
      }

      // Non-overloaded function and parenless should emit warning.
      {
        use baz only p, f;
        import baz.{p, f};
      }

      // Overloaded function should emit no warning.
      {
        use ding only p;
        import ding.{p};
      }
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 5);
  auto mod = br.topLevelExpression(4)->toModule();
  assert(mod);

  // Scope-resolve the module.
  std::ignore = resolveModule(ctx, mod->id());

  // TODO: Helpers to make sure the errors are correct.
  assert(guard.numErrors() > 0);
  for (auto& err : guard.errors()) {
    assert(err->type() == ErrorType::Deprecation);
  }

  assert(guard.realizeErrors());

  std::cout << std::endl;
}

// Warnings should not be emitted for method receivers.
static void test2(void) {
  Context* ctx = buildStdContextWithUnstableWarnings();
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
    module M {
      @unstable("The class 'C' is unstable")
      class C {
        proc foo() {}   // Primary
      }
      proc C.bar() {}   // Secondary

      @unstable("The record 'r' is unstable")
      record r {
        proc foo() {}   // Primary
      }
      proc r.bar() {}   // Secondary
    }
    module N {
      use M;
      proc C.baz() {}   // Tertiary
      proc r.baz() {}   // Tertiary

      var a = new C();  // Unstable warning for this mention
      a.foo();
      a.bar();
      a.baz();

      var b = new r();  // Unstable warning for this mention
      b.foo();
      b.bar();
      b.baz();
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the 'N' module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 2);
  auto mod = br.topLevelExpression(1)->toModule();
  assert(mod);

  // Scope-resolve the module.
  std::ignore = resolveModule(ctx, mod->id());

  assert(guard.numErrors() == 2);
  for (auto& err : guard.errors()) {
    assert(err->type() == ErrorType::Unstable);
  }

  assert(guard.error(0)->message() == "The class 'C' is unstable");
  assert(guard.error(1)->message() == "The record 'r' is unstable");
  assert(guard.realizeErrors());
}

// Nested unstable warnings should not occur.
static void test3(void) {
  Context context;
  Context* ctx = turnOnWarnUnstable(&context);
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(

    module testNoWarningsForUnstableMentionsInUnstable {
      @unstable("this variable is unstable")
      var x: int = 0;
      var y: int = 1;

      proc foo(z) {}

      @unstable("this module is unstable")
      module TestUnstableModule2 {
        // checks deep uses
        module Deeper {
          proc bar() {
            use super.super;
            foo(x);
            return x;
          }

          // Even deeper!
          module Deepest {
            proc baz() {
              use super.super.super;
              foo(x);
              return x;
            }
          }
        }
      }

      proc main() {
        use this.TestUnstableModule2.Deeper;
        use this.TestUnstableModule2.Deeper.Deepest;

        bar();
        baz();
      }
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the top module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Force resolve 'main' since we may not always do that yet.
  assert(mod->numStmts() == 5);
  const Function* mainFn = mod->stmt(4)->toFunction();
  std::ignore = resolveConcreteFunction(ctx, mainFn->id());

  assert(guard.numErrors() == 2);
  for (auto& err : guard.errors()) {
    assert(err->type() == ErrorType::Unstable);
  }

  auto& e0 = guard.error(0);
  assert(e0->message() == "this module is unstable");
  assert(e0->location(ctx).line() == 32);
  auto& e1 = guard.error(1);
  assert(e1->message() == "this module is unstable");
  assert(e1->location(ctx).line() == 33);

  assert(guard.realizeErrors());
}

// Make sure that formal types are covered as well.
static void test4(ErrorType expectedError) {
  assert(expectedError == ErrorType::Unstable ||
         expectedError == ErrorType::Deprecation);

  std::string warningLabel = expectedError == ErrorType::Unstable
        ? "@unstable"
        : "@deprecated";

  Context* ctx = buildStdContextWithUnstableWarnings();
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);

  std::cout << path.c_str() << std::endl;

  std::string contents = "\n" + warningLabel +
    R""""(("warning message")
    class C {}

    proc foo(x: C) {}

    proc main() {
      var x = new C();
      foo(x);
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the top module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(!guard.realizeErrors());

  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Force resolve 'main' since we may not always do that yet.
  assert(mod->numStmts() == 3);
  const Function* mainFn = mod->stmt(2)->toFunction();
  std::ignore = resolveConcreteFunction(ctx, mainFn->id());

  assert(guard.numErrors() == 2);
  for (auto& err : guard.errors()) {
    assert(err->type() == expectedError);
  }

  assert(guard.realizeErrors());
}

// Unstable warnings are not emitted if the parent is compiler-generated,
// deprecated, or unstable. Deprecation warnings are not emitted if the
// parent is deprecated or compiler-generated (but are emitted in unstable
// things, since those will stick around longer).
static void test5(void) {
  Context context;
  Context* ctx = turnOnWarnUnstable(&context);
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
    @deprecated
    var x = 0;
    @unstable
    var y = 0;

    proc foo(x) {}

    // No mention for either.
    pragma "compiler generated"
    proc f1() {
      foo(x);
      foo(y);
    }

    // Unstable warning is not mentioned. Deprecation warning is.
    @unstable
    proc f2() {
      foo(x);
      foo(y);
    }

    // Neither unstable or deprecated is mentioned.
    @deprecated
    proc f3() {
      foo(x);
      foo(y);
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the top module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Force resolve 'main' since we may not always do that yet.
  assert(mod->numStmts() == 9);
  const Function* f1 = mod->stmt(4)->toFunction();
  std::ignore = resolveConcreteFunction(ctx, f1->id());
  const Function* f2 = mod->stmt(6)->toFunction();
  std::ignore = resolveConcreteFunction(ctx, f2->id());
  const Function* f3 = mod->stmt(8)->toFunction();
  std::ignore = resolveConcreteFunction(ctx, f3->id());


  assert(guard.numErrors() == 2);
  assert(guard.error(0)->kind() == ErrorBase::Kind::WARNING);
  assert(guard.error(1)->type() == ErrorType::Deprecation);
  assert(guard.realizeErrors());
}

int main() {
  test0();
  test1();
  test2();
  test3();
  test4(ErrorType::Unstable);
  test4(ErrorType::Deprecation);
  test5();
  return 0;
}

