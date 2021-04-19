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

static const Module* parseOneModule(Context* ctx, UniqueString filepath) {
  const FrontendQueries::ModuleDeclVec& v =
    FrontendQueries::parse(ctx, filepath);
  assert(v.size() == 1);
  return v[0]->module();
}

static void test2() {
  printf("test2\n");
  auto context = Context::build();
  Context* ctx = context.get();

  ctx->advanceToNextRevision(true);
  auto modOnePath = UniqueString::build(ctx, "modOne.chpl");
  std::string modOneContents = "/* this is a test */\n"
                               "a;\n";
  ctx->setFileText(modOnePath, modOneContents);
  auto modTwoPath = UniqueString::build(ctx, "modTwo.chpl");
  std::string modTwoContents = "/* this is a another test */"
                               "a;\n";
  ctx->setFileText(modTwoPath, modTwoContents);
 
  const Module* moduleOne = nullptr;
  const Module* moduleTwo = nullptr;

  moduleOne = parseOneModule(ctx, modOnePath);
  moduleTwo = parseOneModule(ctx, modTwoPath);
  assert(moduleOne->numStmts() == 2);
  assert(moduleTwo->numStmts() == 2);
  BaseAST::dump(moduleOne);
  BaseAST::dump(moduleTwo);

  const Module* oldModuleOne = moduleOne;
  const Module* oldModuleTwo = moduleTwo;
  ctx->collectGarbage();

  printf("test2 changing whitespace in modOne.chpl\n");
  ctx->advanceToNextRevision(true);

  modOneContents = "/* this is a test */\n"
                   "\n"
                   "\n"
                   "a;\n";
  ctx->setFileText(modOnePath, modOneContents);
  ctx->setFileText(modTwoPath, modTwoContents);

  printf("test2 parsing modOne.chpl\n");
  moduleOne = parseOneModule(ctx, modOnePath);
  printf("test2 parsing modTwo.chpl\n");
  moduleTwo = parseOneModule(ctx, modTwoPath);
  assert(moduleOne->numStmts() == 2);
  assert(moduleTwo->numStmts() == 2);
  BaseAST::dump(moduleOne);
  BaseAST::dump(moduleTwo);

  // Check that the pointer values didn't change
  // (they shouldn't because we should have reused the modules).
  assert(moduleOne == oldModuleOne);
  assert(moduleTwo == oldModuleTwo);

  ctx->collectGarbage();
}

static void test3() {
  printf("test3\n");
  auto context = Context::build();
  Context* ctx = context.get();

  auto modulePath = UniqueString::build(ctx, "MyModule.chpl");
  const Module* module = nullptr;
  const Comment* comment = nullptr;
  const Identifier* identifierA = nullptr;
  const Identifier* identifierB = nullptr;
  const BlockStmt* block = nullptr;

  std::string moduleContents;

  moduleContents = "/* this is a test */\n"
                   "a;\n"
                   "b;\n";
  ctx->advanceToNextRevision(true);
  ctx->setFileText(modulePath, moduleContents);
  module = parseOneModule(ctx, modulePath);
  ctx->collectGarbage();

  BaseAST::dump(module);
  assert(module->numStmts() == 3);
  comment = module->stmt(0)->toComment();
  identifierA = module->stmt(1)->toIdentifier();
  identifierB = module->stmt(2)->toIdentifier();
  assert(comment);
  assert(identifierA);
  assert(identifierB);
  const Comment* oldComment = comment;
  const Identifier* oldIdentifierA = identifierA;
  const Identifier* oldIdentifierB = identifierB;

  printf("test3 adding BlockStmts\n");
  moduleContents = "/* this is a test */\n"
                   "{ x; }\n"
                   "a;\n"
                   "{ y; }\n"
                   "b;\n";
  ctx->advanceToNextRevision(true);
  ctx->setFileText(modulePath, moduleContents);
  module = parseOneModule(ctx, modulePath);
  ctx->collectGarbage();

  BaseAST::dump(module);

  // Check that the pointer values didn't change
  // (they shouldn't because we should have reused the parts that didn't change)
  assert(module->numStmts() == 5);
  comment = module->stmt(0)->toComment();
  block = module->stmt(1)->toBlockStmt();
  identifierA = module->stmt(2)->toIdentifier();
  identifierB = module->stmt(4)->toIdentifier();
  assert(comment);
  assert(identifierA);
  assert(identifierB);
  assert(comment == oldComment);
  assert(identifierA == oldIdentifierA);
  assert(identifierB == oldIdentifierB);
  const BlockStmt* oldBlock = block;

  printf("test3 changing Identifier in Blocks\n");
  moduleContents = "/* this is a test */\n"
                   "{ xx; }\n"
                   "a;\n"
                   "{ yy; }\n"
                   "b;\n";
  ctx->advanceToNextRevision(true);
  ctx->setFileText(modulePath, moduleContents);
  module = parseOneModule(ctx, modulePath);
  ctx->collectGarbage();

  BaseAST::dump(module);

  // Check that the block and identifiers match
  assert(module->numStmts() == 5);
  comment = module->stmt(0)->toComment();
  block = module->stmt(1)->toBlockStmt();
  identifierA = module->stmt(2)->toIdentifier();
  identifierB = module->stmt(4)->toIdentifier();
  assert(comment);
  assert(identifierA);
  assert(identifierB);
  assert(comment == oldComment);
  assert(block == oldBlock);
  assert(identifierA == oldIdentifierA);
  assert(identifierB == oldIdentifierB);
  oldBlock = nullptr; // it will be invalid after this point

  printf("test3 removing the Blocks\n");
  moduleContents = "/* this is a test */\n"
                   "a;\n"
                   "b;\n";
  ctx->advanceToNextRevision(true);
  ctx->setFileText(modulePath, moduleContents);
  module = parseOneModule(ctx, modulePath);
  ctx->collectGarbage();

  BaseAST::dump(module);

  // Check that the comment and identifiers match
  assert(module->numStmts() == 3);
  comment = module->stmt(0)->toComment();
  identifierA = module->stmt(1)->toIdentifier();
  identifierB = module->stmt(2)->toIdentifier();
  assert(comment);
  assert(identifierA);
  assert(identifierB);
  assert(comment == oldComment);
  assert(identifierA == oldIdentifierA);
  assert(identifierB == oldIdentifierB);
  oldIdentifierA = nullptr; // it will be invalid after this point

  printf("test3 replacing first Identifier\n");
  moduleContents = "/* this is a test */\n"
                   "aa;\n"
                   "b;\n";
  ctx->advanceToNextRevision(true);
  ctx->setFileText(modulePath, moduleContents);
  module = parseOneModule(ctx, modulePath);
  ctx->collectGarbage();

  BaseAST::dump(module);

  // Check that the comment and identifiers match
  assert(module->numStmts() == 3);
  comment = module->stmt(0)->toComment();
  identifierA = module->stmt(1)->toIdentifier();
  identifierB = module->stmt(2)->toIdentifier();
  assert(comment);
  assert(identifierA);
  assert(identifierB);
  assert(comment == oldComment);
  assert(identifierB == oldIdentifierB);
}

// TODO: test locate

int main() {
  test0();
  test1();
  test2();
  test3();

  return 0;
}
