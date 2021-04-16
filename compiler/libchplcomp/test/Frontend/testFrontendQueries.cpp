#include "chpl/Frontend/FrontendQueries.h"

#include "chpl/AST/ModuleDecl.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace ast;

static void test0() {
  printf("test0\n");
  auto context = Context::build();
  Context* ctx = context.get();

  auto path = UniqueString::build(ctx, "input.chpl");
  std::string contents = "/* this is a test */";
  ctx->setFileText(path, contents);
 
  std::string gotContents = FrontendQueries::fileText(ctx, path);
  assert(gotContents == contents);
}

static void test1() {
  printf("test1\n");
  auto context = Context::build();
  Context* ctx = context.get();

  auto path = UniqueString::build(ctx, "input.chpl");
  std::string contents = "/* this is a test */";
  ctx->setFileText(path, contents);
 
  FrontendQueries::parse(ctx, path);
}

static void test2() {
  printf("test2\n");
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
 
  const ast::Builder::Result* p;
  p = FrontendQueries::parse(ctx, modOnePath);
  assert(p->topLevelExprs.size() == 1);
  assert(p->topLevelExprs[0]->isModuleDecl());
  auto moduleOne = p->topLevelExprs[0]->toModuleDecl()->module();
  assert(moduleOne->numStmts() == 2);
  p = FrontendQueries::parse(ctx, modTwoPath);
  assert(p->topLevelExprs.size() == 1);
  assert(p->topLevelExprs[0]->isModuleDecl());
  auto moduleTwo = p->topLevelExprs[0]->toModuleDecl()->module();
  assert(moduleTwo->numStmts() == 2);

  printf("test2 changing whitespace in modOne.chpl\n");

  modOneContents = "/* this is a test */\n"
                   "\n"
                   "\n"
                   "a;\n";
  ctx->setFileText(modOnePath, modOneContents);
  ctx->setFileText(modTwoPath, modTwoContents);

  p = FrontendQueries::parse(ctx, modOnePath);
  assert(p->topLevelExprs.size() == 1);
  assert(p->topLevelExprs[0]->isModuleDecl());
  moduleOne = p->topLevelExprs[0]->toModuleDecl()->module();
  assert(moduleOne->numStmts() == 2);
  p = FrontendQueries::parse(ctx, modTwoPath);
  assert(p->topLevelExprs.size() == 1);
  assert(p->topLevelExprs[0]->isModuleDecl());
  moduleTwo = p->topLevelExprs[0]->toModuleDecl()->module();
  assert(moduleTwo->numStmts() == 2);
}


int main() {
  test0();
  test1();
  test2();

  return 0;
}
