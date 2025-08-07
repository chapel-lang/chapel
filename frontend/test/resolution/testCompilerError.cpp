/*
 * Copyright 2025 Hewlett Packard Enterprise Development LP
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
#include "test-resolution.h"

static void ensureErrorOnLine(Context* context, const std::vector<owned<ErrorBase>>& errors, ErrorType type, int line, const char* message, bool allowOthers = false) {
  for (auto& error : errors) {
    if (error->type() != type) continue;
    bool matches = error->location(context).firstLine() == line &&
                   error->message() == message;
    if (!matches && !allowOthers) {
      assert(false && "error found on a different line or with different message");
    }
    if (matches) return;
  }
  assert(false && "error not found");
}

static void ensureErrorInErrorsModule(Context* context, const std::vector<owned<ErrorBase>>& errors, ErrorType type) {
  for (auto& error : errors) {
    if (error->type() != type) continue;
    assert(error->location(context).path().endsWith("modules/standard/Errors.chpl"));
    return;
  }
  assert(false && "error not found");
}

static void testDirect() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    proc foo(x) {
      compilerError("some error");
      return x;
    }
    var x = foo(42);
    )""";

  resolveTypesOfVariables(ctx, program, {"x"});
  ensureErrorInErrorsModule(ctx, guard.errors(), ErrorType::UserDiagnosticEncounterError);
  ensureErrorOnLine(ctx, guard.errors(), ErrorType::UserDiagnosticEmitError, 6, "some error");
  guard.realizeErrors();
}

static void testDirectWarn() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    proc foo(x) {
      compilerWarning("some error");
      return x;
    }
    var x = foo(42);
    )""";

  resolveTypesOfVariables(ctx, program, {"x"});
  ensureErrorInErrorsModule(ctx, guard.errors(), ErrorType::UserDiagnosticEncounterWarning);
  ensureErrorOnLine(ctx, guard.errors(), ErrorType::UserDiagnosticEmitWarning, 6, "some error");
  guard.realizeErrors();
}

static void testDepth2() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    proc foo(x) {
      compilerError("some error", 2);
      return x;
    }
    proc bar(x) do return foo(x);
    var x = bar(42);
    )""";

  resolveTypesOfVariables(ctx, program, {"x"});
  ensureErrorInErrorsModule(ctx, guard.errors(), ErrorType::UserDiagnosticEncounterError);
  ensureErrorOnLine(ctx, guard.errors(), ErrorType::UserDiagnosticEmitError, 7, "some error");
  guard.realizeErrors();
}

static void testDepth3() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    proc foo(x) {
      compilerError("some error", 3);
      return x;
    }
    proc bar(x) do return foo(x);
    proc baz(x) do return bar(x);
    var x = baz(42);
    )""";

  resolveTypesOfVariables(ctx, program, {"x"});
  ensureErrorInErrorsModule(ctx, guard.errors(), ErrorType::UserDiagnosticEncounterError);
  ensureErrorOnLine(ctx, guard.errors(), ErrorType::UserDiagnosticEmitError, 8, "some error");
  guard.realizeErrors();
}

static void testTooDeep() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    proc foo(x) {
      compilerError("some error", 1000);
      return x;
    }
    var x = foo(42);
    )""";

  resolveTypesOfVariables(ctx, program, {"x"});
  ensureErrorInErrorsModule(ctx, guard.errors(), ErrorType::UserDiagnosticEncounterError);
  ensureErrorOnLine(ctx, guard.errors(), ErrorType::UserDiagnosticEmitError, 6, "some error");
  guard.realizeErrors();
}

static void testVarArgs() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    proc foo(x) {
      compilerError("this ", "is ", "some ", "error ", "message");
      return x;
    }
    var x = foo(42);
    )""";

  resolveTypesOfVariables(ctx, program, {"x"});
  ensureErrorInErrorsModule(ctx, guard.errors(), ErrorType::UserDiagnosticEncounterError);
  ensureErrorOnLine(ctx, guard.errors(), ErrorType::UserDiagnosticEmitError, 6, "this is some error message");
  guard.realizeErrors();
}

static void testTwoErrors() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    proc foo(): int do compilerError("no", 2);
    proc first() do return foo();
    proc second() do return foo();
    proc third() do return second();

    var x = first();
    var y = third();
    )""";

  resolveTypesOfVariables(ctx, program, {"x", "y"});
  ensureErrorOnLine(ctx, guard.errors(), ErrorType::UserDiagnosticEncounterError, 644, "no", /* allowOthers = */ true);
  ensureErrorOnLine(ctx, guard.errors(), ErrorType::UserDiagnosticEmitError, 5, "no", /* allowOthers = */ true);
  ensureErrorOnLine(ctx, guard.errors(), ErrorType::UserDiagnosticEmitError, 7, "no", /* allowOthers = */ true);
  guard.realizeErrors();
}

static void testRunAndTrackErrors() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    proc foo() {
      compilerError("some error");
      return 42;
    }
    )""";

  auto file = UniqueString::get(ctx, "test.chpl");
  setFileText(ctx, file, program);
  auto modules = parse(ctx, file, UniqueString());
  assert(modules.size() == 1);
  assert(modules[0]->numStmts() == 1);

  auto result = ctx->runAndCaptureErrors([&](Context* ctx) {
    return resolveConcreteFunction(ctx, modules[0]->stmt(0)->id());
  });
  assert(!result.ranWithoutErrors());
  assert(result.errors().size() == 2);
  ensureErrorInErrorsModule(ctx, result.errors(), ErrorType::UserDiagnosticEncounterError);
}

static void testInfiniteRecursion() {
  // in nested functions, 'isQueryRunning' is not sufficient to avoid infinite
  // recursion while gathering diagnostics. Regression test for
  // https://github.com/Cray/chapel-private/issues/7096.
  //

  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
    proc foo() {
      proc bar(x: int): int {
        return bar(x);
      }
      return bar(1);
    }
    var x = foo();
    )""";

  auto x = resolveTypeOfX(ctx, program);
  assert(x->isIntType());
}

static void testEarlyReturn() {
  // ensure that code after compilerError() is not resolved.
  // below, the bar function (which emits a compilerError) should not
  // be resolved past the call to compilerError, so the "inside bar"
  // compilerWarning should not be emitted. However, foo() merely
  // uses the result of bar(), so it should be resolved.

  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);
  std::string program =
    R"""(
      proc foo() {
        bar();
        compilerWarning("inside foo, after call to bar");
        return 42;
      }

      proc bar() {
        compilerError("Hello");
        compilerWarnning("inside bar, after call to compilerError");
      }

      var x = foo();
  )""";

  auto x = resolveTypeOfX(ctx, program);
  assert(x->isIntType());
  ensureErrorOnLine(ctx, guard.errors(), ErrorType::UserDiagnosticEmitError, 3, "Hello", /* allowOthers = */ true);
  ensureErrorOnLine(ctx, guard.errors(), ErrorType::UserDiagnosticEmitWarning, 13, "inside foo, after call to bar", /* allowOthers = */ true);
  assert(guard.realizeErrors() == 4); /* two more for UserDiagnosticEncounter{Error,Warning} */
}

int main() {
  testDirect();
  testDirectWarn();
  testDepth2();
  testDepth3();
  testTooDeep();
  testVarArgs();
  testTwoErrors();
  testRunAndTrackErrors();
  testInfiniteRecursion();
  testEarlyReturn();
}
