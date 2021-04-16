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

  auto path = UniqueString::build(ctx, "input.chpl");
  std::string contents = "/* this is a test */";
  ctx->setFileText(path, contents);
 
  auto* p = FrontendQueries::parse(ctx, path);
}

static void test2() {
  auto context = Context::build();
  Context* ctx = context.get();

  auto modOnePath = UniqueString::build(ctx, "modOne.chpl");
  std::string modOneContents = "/* this is a test */\n"
                               "a;\n";
  ctx->setFileText(modOnePath, modOneContents);
  auto modTwoPath = UniqueString::build(ctx, "modTwo.chpl");
  std::string modTwoContents = "/* this is a another test */"
                               "a;\n";
  ctx->setFileText(modTwoPath, modTwoContents);
 
  auto* p = FrontendQueries::parse(ctx, modOnePath);
  //const ast::Builder::Result* parse(Context* context, UniqueString path);

}


int main() {
  test0();
  test1();
  test2();

  return 0;
}
