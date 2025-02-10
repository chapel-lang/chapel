#include "test-common.h"
#include "test-resolution.h"

static void testBasic() {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  std::string program = R"""(
    var x = let a = 4 in a * a;
  )""";

  auto qt = resolveTypeOfXInit(context, program);
  assert(qt.type());
  assert(qt.type()->isIntType());
}

static void testTwoVars() {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  std::string program = R"""(
    var x = let a = 4, b = 4 in a * b;
  )""";

  auto qt = resolveTypeOfXInit(context, program);
  assert(qt.type());
  assert(qt.type()->isIntType());
}

static void testRedefinition() {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  std::string program = R"""(
    var x = let a = 4, a = 4 in a * a;
  )""";

  resolveTypeOfXInit(context, program, /* requireType */ false);
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->type() == ErrorType::Redefinition);
  guard.realizeErrors();
}

static void testOutOfOrder() {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  std::string program = R"""(
    var x = let a = b, b = 4 in a * a;
  )""";

  resolveTypeOfXInit(context, program, /* requireType */ false);
  assert(guard.numErrors() == 2);
  assert(guard.error(0)->type() == ErrorType::UseOfLaterVariable);
  assert(guard.error(1)->type() == ErrorType::NoMatchingCandidates);
  guard.realizeErrors();
}

static void testVoid() {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  std::string program = R"""(
    proc foo(x) {}
    var x = let a = 4 in foo(a);
  )""";

  auto qt = resolveTypeOfXInit(context, program);
  assert(qt.type()->isVoidType());
  assert(guard.numErrors() == 1);
  guard.realizeErrors();
}

int main() {
  testBasic();
  testTwoVars();
  testRedefinition();
  testOutOfOrder();
  testVoid();
}
