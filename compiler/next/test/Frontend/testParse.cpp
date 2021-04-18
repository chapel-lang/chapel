#include "chpl/AST/BlockStmt.h"
#include "chpl/AST/Expr.h"
#include "chpl/AST/Identifier.h"
#include "chpl/AST/ModuleDecl.h"
#include "chpl/Frontend/Parser.h"
#include "chpl/Queries/Context.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace ast;

static void test0(Parser* parser) {
  auto parseResult = parser->parseString("test0.chpl", "");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isModuleDecl());
  auto module = parseResult.topLevelExprs[0]->toModuleDecl()->module();
  assert(module->name().compare("test0") == 0);
  assert(module->numStmts() == 0);
}

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl", "x;");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isModuleDecl());
  auto module = parseResult.topLevelExprs[0]->toModuleDecl()->module();
  assert(module->name().compare("test1") == 0);
  assert(module->numStmts() == 1);
  assert(module->stmt(0)->isIdentifier());
  auto identifier = module->stmt(0)->toIdentifier();
  assert(identifier->name().compare("x") == 0);
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl", "x; y;");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isModuleDecl());
  auto module = parseResult.topLevelExprs[0]->toModuleDecl()->module();
  assert(module->name().compare("test2") == 0);
  assert(module->numStmts() == 2);
  assert(module->stmt(0)->isIdentifier());
  assert(module->stmt(1)->isIdentifier());
}

static void test3(Parser* parser) {
  auto parseResult = parser->parseString("test3.chpl", "/* hi */ y;");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isModuleDecl());
  auto module = parseResult.topLevelExprs[0]->toModuleDecl()->module();
  assert(module->name().compare("test3") == 0);
  assert(module->numStmts() == 2);
  assert(module->stmt(0)->isComment());
  assert(module->stmt(1)->isIdentifier());
}

static void test4(Parser* parser) {
  auto parseResult = parser->parseString("test4.chpl", "/* hi */ y; /* bye */");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isModuleDecl());
  auto module = parseResult.topLevelExprs[0]->toModuleDecl()->module();
  assert(module->name().compare("test4") == 0);
  assert(module->numStmts() == 3);
  assert(module->stmt(0)->isComment());
  assert(module->stmt(1)->isIdentifier());
  assert(module->stmt(2)->isComment());
}

static void test5(Parser* parser) {
  auto parseResult = parser->parseString("test5.chpl",
                                         "// hi\n"
                                         "a;\n"
                                         "// bye\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isModuleDecl());
  auto module = parseResult.topLevelExprs[0]->toModuleDecl()->module();
  assert(module->numStmts() == 3);
  assert(module->stmt(0)->isComment());
  assert(module->stmt(1)->isIdentifier());
  assert(module->stmt(2)->isComment());
}

static void test6(Parser* parser) { 
  auto parseResult = parser->parseString("test6.chpl",
                                         "{ }");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isModuleDecl());
  auto module = parseResult.topLevelExprs[0]->toModuleDecl()->module();
  assert(module->numStmts() == 1);
  assert(module->stmt(0)->isBlockStmt());
}

static void test7(Parser* parser) {
  auto parseResult = parser->parseString("test7.chpl",
                                         "{ a; }");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isModuleDecl());
  auto module = parseResult.topLevelExprs[0]->toModuleDecl()->module();
  assert(module->numStmts() == 1);
  assert(module->stmt(0)->isBlockStmt());
  const BlockStmt* block = module->stmt(0)->toBlockStmt();
  assert(block->numStmts()==1);
  assert(block->stmt(0)->isIdentifier());
}

static void test8(Parser* parser) {
  auto parseResult = parser->parseString("t.chpl", "aVeryLongIdentifierName;");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isModuleDecl());
  auto module = parseResult.topLevelExprs[0]->toModuleDecl()->module();
  assert(module->numStmts() == 1);
  assert(module->stmt(0)->isIdentifier());
}

static void test9(Parser* parser) {
  auto parseResult = parser->parseString("test9.chpl",
                                         "{ /* this is a comment */ }");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isModuleDecl());
  auto module = parseResult.topLevelExprs[0]->toModuleDecl()->module();
  assert(module->numStmts() == 1);
  assert(module->stmt(0)->isBlockStmt());
  const BlockStmt* block = module->stmt(0)->toBlockStmt();
  assert(block->numStmts()==1);
  assert(block->stmt(0)->isComment());
}

static void test10(Parser* parser) {
  auto parseResult = parser->parseString("test10.chpl",
                                         "{\n"
                                         "/* this is comment 2 */\n"
                                         "aVeryLongIdentifierName;\n"
                                         "/* this is comment 3 */\n"
                                         "}\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isModuleDecl());
  auto module = parseResult.topLevelExprs[0]->toModuleDecl()->module();
  assert(module->numStmts() == 1);
  assert(module->stmt(0)->isBlockStmt());
  const BlockStmt* block = module->stmt(0)->toBlockStmt();
  assert(block->numStmts()==3);
  assert(block->stmt(0)->isComment());
  assert(block->stmt(1)->isIdentifier());
  assert(block->stmt(2)->isComment());
}


static void test11(Parser* parser) {
  auto parseResult = parser->parseString("test11.chpl",
                                         "/* this is comment 1 */\n"
                                         "{\n"
                                         "/* this is comment 2 */\n"
                                         "aVeryLongIdentifierName;\n"
                                         "/* this is comment 3 */\n"
                                         "}\n"
                                         "/* this is comment 4 */");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isModuleDecl());
  auto module = parseResult.topLevelExprs[0]->toModuleDecl()->module();
  assert(module->numStmts() == 3);
  assert(module->stmt(0)->isComment());
  assert(module->stmt(1)->isBlockStmt());
  assert(module->stmt(2)->isComment());
  const BlockStmt* block = module->stmt(1)->toBlockStmt();
  assert(block->numStmts()==3);
  assert(block->stmt(0)->isComment());
  assert(block->stmt(1)->isIdentifier());
  assert(block->stmt(2)->isComment());
}

static void test12(Parser* parser) {
  auto parseResult = parser->parseString("test12.chpl",
                                         "/* this is comment 1 */\n"
                                         "/* this is comment 2 */\n"
                                         "{\n"
                                         "/* this is comment 2 */\n"
                                         "/* this is comment 3 */\n"
                                         "aVeryLongIdentifierName;\n"
                                         "/* this is comment 3 */\n"
                                         "/* this is comment 4 */\n"
                                         "}\n"
                                         "/* this is comment 5 */\n"
                                         "/* this is comment 6 */");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isModuleDecl());
  auto module = parseResult.topLevelExprs[0]->toModuleDecl()->module();
  assert(module->numStmts() == 5);
  assert(module->stmt(0)->isComment());
  assert(module->stmt(1)->isComment());
  assert(module->stmt(2)->isBlockStmt());
  assert(module->stmt(3)->isComment());
  assert(module->stmt(4)->isComment());
  const BlockStmt* block = module->stmt(2)->toBlockStmt();
  assert(block->numStmts()==5);
  assert(block->stmt(0)->isComment());
  assert(block->stmt(1)->isComment());
  assert(block->stmt(2)->isIdentifier());
  assert(block->stmt(3)->isComment());
  assert(block->stmt(4)->isComment());
}

static void test13(Parser* parser) {
  auto parseResult = parser->parseString("test13.chpl",
                                         "var a;\n"
                                         "a;");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isModuleDecl());
  auto module = parseResult.topLevelExprs[0]->toModuleDecl()->module();
  assert(module->numStmts() == 2);
  assert(module->stmt(0)->isVariableDecl());
  assert(module->stmt(1)->isIdentifier());
}


int main() {
  auto context = Context::build();
  Context* ctx = context.get();
  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  test0(p);
  test1(p);
  test2(p);
  test3(p);
  test4(p);
  test5(p);
  test6(p);
  test7(p);
  test8(p);
  test9(p);
  test10(p);
  test11(p);
  test12(p);
  test13(p);

  return 0;
}
