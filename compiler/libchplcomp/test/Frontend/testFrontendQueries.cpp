#include "chpl/Frontend/FrontendQueries.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace ast;

static void test0() {
  auto context = Context::build();
  Context* ctx = context.get();

  auto path = UniqueString::build(ctx, "input.chpl");
  std::string contents = "/* this is a test */";
  ctx->setFileText(path, contents);
 
  std::string gotContents = FrontendQueries::fileText(ctx, path);
  assert(gotContents == contents);
}

static void test1() {
  auto context = Context::build();
  Context* ctx = context.get();

  auto modOne = UniqueString::build(ctx, "modOne.chpl");
  std::string modOneContents = "/* this is a test */";
  ctx->setFileText(modOne, modOneContents);
  auto modTwo = UniqueString::build(ctx, "modTwo.chpl");
  std::string modTwoContents = "/* this is a another test */";
  ctx->setFileText(modTwo, modTwoContents);
 
  std::string gotContentsOne = FrontendQueries::fileText(ctx, modOne);
  assert(gotContentsOne == modOneContents);
  std::string gotContentsTwo = FrontendQueries::fileText(ctx, modTwo);
  assert(gotContentsTwo == modTwoContents);
}


int main() {
  test0();
  test1();

  return 0;
}
