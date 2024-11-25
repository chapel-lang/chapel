#include "test-common.h"

using namespace chpl;
using namespace chpl::parsing;


static void test(Context* ctx, const char* program, int numErrors, bool gc) {
  ErrorGuard guard(ctx);
  ctx->advanceToNextRevision(gc);

  auto path = UniqueString::get(ctx, "simple.chpl");
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  assert(vec.size() == 1);

  if (gc) ctx->collectGarbage();
  guard.printErrors();
  assert(guard.realizeErrors() == 2);
}

int main(int argc, char** argv) {

  const char* program1 = R""""(
    proc bar() {}
    module A {
      proc foo() {}
    }
    enum a {}
  )"""";
  const char* program2 = R""""(
    proc bar() {}
    module A {
      proc foo() {}
    }
    enum a {3}
  )"""";

  Context context;
  Context* ctx = &context;

  test(ctx, program1, 2, true);
  test(ctx, program2, 2, true);
  test(ctx, program1, 2, true);
  test(ctx, program2, 2, true);
  test(ctx, program2, 2, true);

  return 0;
}
