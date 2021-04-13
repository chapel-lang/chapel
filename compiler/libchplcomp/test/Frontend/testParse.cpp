// g++ -g -I compiler/libchplcomp/include/ compiler/libchplcomp/lib/AST/*.cpp compiler/libchplcomp/lib/Frontend/*.cpp compiler/libchplcomp/lib/Frontend/Parser/{bison-chapel,flex-chapel}.cpp compiler/libchplcomp/lib/Util/*.cpp compiler/libchplcomp/test/Frontend/testParse.cpp

#include "chpl/AST/BlockStmt.h"
#include "chpl/AST/Context.h"
#include "chpl/AST/Expr.h"
#include "chpl/Frontend/Parser.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace ast;

static void test0(Parser* parser) {
  auto parseResult = parser->parseString("<test>", "");
  assert(parseResult.topLevelExprs.size() == 0);
  assert(parseResult.parseErrors.size() == 0);
}

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("<test>", "x;");
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isIdentifier());
  assert(parseResult.parseErrors.size() == 0);
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("<test>", "x; y;");
  assert(parseResult.topLevelExprs.size() == 2);
  assert(parseResult.topLevelExprs[0]->isIdentifier());
  assert(parseResult.topLevelExprs[1]->isIdentifier());
  assert(parseResult.parseErrors.size() == 0);
}

static void test3(Parser* parser) {
  auto parseResult = parser->parseString("<test>", "/* hi */ y;");
  assert(parseResult.topLevelExprs.size() == 2);
  assert(parseResult.topLevelExprs[0]->isComment());
  assert(parseResult.topLevelExprs[1]->isIdentifier());
  assert(parseResult.parseErrors.size() == 0);
}

static void test4(Parser* parser) {
  auto parseResult = parser->parseString("<test>", "/* hi */ y; /* bye */");
  assert(parseResult.topLevelExprs.size() == 3);
  assert(parseResult.topLevelExprs[0]->isComment());
  assert(parseResult.topLevelExprs[1]->isIdentifier());
  assert(parseResult.topLevelExprs[2]->isComment());
  assert(parseResult.parseErrors.size() == 0);
}

static void test5(Parser* parser) {
  auto parseResult = parser->parseString("<test>",
                                         "// hi\n"
                                         "a;\n"
                                         "// bye\n");
  assert(parseResult.topLevelExprs.size() == 3);
  assert(parseResult.topLevelExprs[0]->isComment());
  assert(parseResult.topLevelExprs[1]->isIdentifier());
  assert(parseResult.topLevelExprs[2]->isComment());
  assert(parseResult.parseErrors.size() == 0);
}

static void test6(Parser* parser) { 
  auto parseResult = parser->parseString("<test>",
                                         "{ }");
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isBlockStmt());
  assert(parseResult.parseErrors.size() == 0);
}

static void test7(Parser* parser) {
  auto parseResult = parser->parseString("<test>",
                                         "{ a; }");
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isBlockStmt());
  assert(parseResult.parseErrors.size() == 0);
  BlockStmt* block = parseResult.topLevelExprs[0]->toBlockStmt();
  assert(block->numStmts()==1);
  assert(block->stmt(0)->isIdentifier());
}

static void test8(Parser* parser) {
  auto parseResult = parser->parseString("<test>", "aVeryLongIdentifierName;");
  assert(parseResult.topLevelExprs.size() == 1);
  assert(parseResult.topLevelExprs[0]->isIdentifier());
  assert(parseResult.parseErrors.size() == 0);
}

static void test9(Parser* parser) {
  auto parseResult = parser->parseString("<test>",
                                         "/* this is comment 1 */\n"
                                         "{\n"
                                         "/* this is comment 2 */\n"
                                         "aVeryLongIdentifierName;\n"
                                         "/* this is comment 3 */\n"
                                         "}\n"
                                         "/* this is comment 4 */");
  assert(parseResult.topLevelExprs.size() == 3);
  assert(parseResult.topLevelExprs[0]->isComment());
  assert(parseResult.topLevelExprs[1]->isBlockStmt());
  assert(parseResult.topLevelExprs[2]->isComment());
  assert(parseResult.parseErrors.size() == 0);
  BlockStmt* block = parseResult.topLevelExprs[1]->toBlockStmt();
  assert(block->numStmts()==3);
  assert(block->stmt(0)->isComment());
  assert(block->stmt(1)->isIdentifier());
  assert(block->stmt(2)->isComment());
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

  return 0;
}
