/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#include "chpl/parsing/Parser.h"
#include "chpl/queries/Context.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/Call.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Dot.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/OpCall.h"
#include "chpl/uast/Variable.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace uast;
using namespace parsing;

static void test0(Parser* parser) {
  auto parseResult = parser->parseString("test0.chpl", "");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->name().compare("test0") == 0);
  assert(module->numStmts() == 0);
}

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl", "x;");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->name().compare("test1") == 0);
  assert(module->numStmts() == 1);
  assert(module->stmt(0)->isIdentifier());
  auto identifier = module->stmt(0)->toIdentifier();
  assert(identifier->name().compare("x") == 0);
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl", "x; y;");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->name().compare("test2") == 0);
  assert(module->numStmts() == 2);
  assert(module->stmt(0)->isIdentifier());
  assert(module->stmt(1)->isIdentifier());
}

static void test3(Parser* parser) {
  auto parseResult = parser->parseString("test3.chpl", "/* hi */ y;");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->name().compare("test3") == 0);
  assert(module->numStmts() == 2);
  assert(module->stmt(0)->isComment());
  assert(module->stmt(1)->isIdentifier());
}

static void test4(Parser* parser) {
  auto parseResult = parser->parseString("test4.chpl", "/* hi */ y; /* bye */");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
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
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 3);
  assert(module->stmt(0)->isComment());
  assert(module->stmt(1)->isIdentifier());
  assert(module->stmt(2)->isComment());
}

static void test6(Parser* parser) {
  auto parseResult = parser->parseString("test6.chpl",
                                         "{ }");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  assert(module->stmt(0)->isBlock());
}

static void test7(Parser* parser) {
  auto parseResult = parser->parseString("test7.chpl",
                                         "{ a; }");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  assert(module->stmt(0)->isBlock());
  const Block* block = module->stmt(0)->toBlock();
  assert(block->numStmts()==1);
  assert(block->stmt(0)->isIdentifier());
}

static void test8(Parser* parser) {
  auto parseResult = parser->parseString("t.chpl", "aVeryLongIdentifierName;");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  assert(module->stmt(0)->isIdentifier());
}

static void test9(Parser* parser) {
  auto parseResult = parser->parseString("test9.chpl",
                                         "{ /* this is a comment */ }");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  assert(module->stmt(0)->isBlock());
  const Block* block = module->stmt(0)->toBlock();
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
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  assert(module->stmt(0)->isBlock());
  const Block* block = module->stmt(0)->toBlock();
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
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 3);
  assert(module->stmt(0)->isComment());
  assert(module->stmt(1)->isBlock());
  assert(module->stmt(2)->isComment());
  const Block* block = module->stmt(1)->toBlock();
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
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 5);
  assert(module->stmt(0)->isComment());
  assert(module->stmt(1)->isComment());
  assert(module->stmt(2)->isBlock());
  assert(module->stmt(3)->isComment());
  assert(module->stmt(4)->isComment());
  const Block* block = module->stmt(2)->toBlock();
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
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 2);
  assert(module->stmt(0)->isVariable());
  assert(module->stmt(1)->isIdentifier());
}

static void test14(Parser* parser) {
  auto parseResult = parser->parseString("test14.chpl",
                                         "{\n"
                                         " //a\n"
                                         " //b\n"
                                         "}\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  assert(module->stmt(0)->isBlock());
  auto block = module->stmt(0)->toBlock();
  assert(block->numStmts() == 2);
  assert(block->stmt(0)->isComment());
  assert(block->stmt(1)->isComment());
}

static void test15(Parser* parser) {
  auto parseResult = parser->parseString("test15.chpl",
                                         "{\n"
                                         " x;\n"
                                         " //a\n"
                                         " //b\n"
                                         "}\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  assert(module->stmt(0)->isBlock());
  auto block = module->stmt(0)->toBlock();
  assert(block->numStmts() == 3);
  assert(block->stmt(0)->isIdentifier());
  assert(block->stmt(1)->isComment());
  assert(block->stmt(2)->isComment());
}

static void testCalls1(Parser* parser) {
  auto parseResult = parser->parseString("testCalls1.chpl",
                                         "f();\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto fnCall = module->stmt(0)->toFnCall();
  assert(fnCall);
  assert(fnCall->callUsedSquareBrackets() == false);
  auto baseExpr = fnCall->calledExpression();
  assert(baseExpr);
  auto baseExprIdent = baseExpr->toIdentifier();
  assert(0 == baseExprIdent->name().compare("f"));
  assert(fnCall->numActuals() == 0);
}

static void testCalls2(Parser* parser) {
  auto parseResult = parser->parseString("testCalls2.chpl",
                                         "f[];\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto fnCall = module->stmt(0)->toFnCall();
  assert(fnCall);
  assert(fnCall->callUsedSquareBrackets() == true);
  auto baseExpr = fnCall->calledExpression();
  assert(baseExpr);
  auto baseExprIdent = baseExpr->toIdentifier();
  assert(0 == baseExprIdent->name().compare("f"));
  assert(fnCall->numActuals() == 0);
}

static void testCalls3(Parser* parser) {
  auto parseResult = parser->parseString("testCalls3.chpl",
                                         "f(x);\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto fnCall = module->stmt(0)->toFnCall();
  assert(fnCall);
  assert(fnCall->callUsedSquareBrackets() == false);
  auto baseExpr = fnCall->calledExpression();
  assert(baseExpr);
  auto baseExprIdent = baseExpr->toIdentifier();
  assert(0 == baseExprIdent->name().compare("f"));
  assert(fnCall->numActuals() == 1);
  auto actualExprIdent = fnCall->actual(0)->toIdentifier();
  assert(0 == actualExprIdent->name().compare("x"));
}

static void testCalls3a(Parser* parser) {
  auto parseResult = parser->parseString("testCalls3a.chpl",
                                         "f(g(x));\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto fnCall = module->stmt(0)->toFnCall();
  assert(fnCall);
  assert(fnCall->callUsedSquareBrackets() == false);
  auto baseExpr = fnCall->calledExpression();
  assert(baseExpr);
  auto baseExprIdent = baseExpr->toIdentifier();
  assert(0 == baseExprIdent->name().compare("f"));
  assert(fnCall->numActuals() == 1);
  auto subCall = fnCall->actual(0)->toFnCall();
  assert(subCall);
  assert(subCall->numActuals() == 1);
  auto subBaseExpr = subCall->calledExpression();
  assert(subBaseExpr);
  auto subBaseExprIdent = subBaseExpr->toIdentifier();
  assert(0 == subBaseExprIdent->name().compare("g"));
  auto subActualExprIdent = subCall->actual(0)->toIdentifier();
  assert(0 == subActualExprIdent->name().compare("x"));
}

static void testCalls4(Parser* parser) {
  auto parseResult = parser->parseString("testCalls4.chpl",
                                         "f[x];\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto fnCall = module->stmt(0)->toFnCall();
  assert(fnCall);
  assert(fnCall->callUsedSquareBrackets() == true);
  auto baseExpr = fnCall->calledExpression();
  assert(baseExpr);
  auto baseExprIdent = baseExpr->toIdentifier();
  assert(0 == baseExprIdent->name().compare("f"));
  assert(fnCall->numActuals() == 1);
  auto actualExprIdent = fnCall->actual(0)->toIdentifier();
  assert(0 == actualExprIdent->name().compare("x"));
}

static void testCalls5(Parser* parser) {
  auto parseResult = parser->parseString("testCalls5.chpl",
                                         "f(a,b,c);\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto fnCall = module->stmt(0)->toFnCall();
  assert(fnCall);
  assert(fnCall->callUsedSquareBrackets() == false);
  auto baseExpr = fnCall->calledExpression();
  assert(baseExpr);
  auto baseExprIdent = baseExpr->toIdentifier();
  assert(0 == baseExprIdent->name().compare("f"));
  assert(fnCall->numActuals() == 3);
  assert(!fnCall->isNamedActual(0));
  assert(!fnCall->isNamedActual(1));
  assert(!fnCall->isNamedActual(2));
  UniqueString emptyStr;
  assert(fnCall->actualName(0) == emptyStr);
  assert(fnCall->actualName(1) == emptyStr);
  assert(fnCall->actualName(2) == emptyStr);
  assert(0 == fnCall->actual(0)->toIdentifier()->name().compare("a"));
  assert(0 == fnCall->actual(1)->toIdentifier()->name().compare("b"));
  assert(0 == fnCall->actual(2)->toIdentifier()->name().compare("c"));
}

static void testCalls6(Parser* parser) {
  auto parseResult = parser->parseString("testCalls6.chpl",
                                         "f(a=aa,b=bb,c=cc);\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto fnCall = module->stmt(0)->toFnCall();
  assert(fnCall);
  assert(fnCall->callUsedSquareBrackets() == false);
  auto baseExpr = fnCall->calledExpression();
  assert(baseExpr);
  auto baseExprIdent = baseExpr->toIdentifier();
  assert(0 == baseExprIdent->name().compare("f"));
  assert(fnCall->numActuals() == 3);
  assert(fnCall->isNamedActual(0));
  assert(fnCall->isNamedActual(1));
  assert(fnCall->isNamedActual(2));
  assert(0 == fnCall->actualName(0).compare("a"));
  assert(0 == fnCall->actualName(1).compare("b"));
  assert(0 == fnCall->actualName(2).compare("c"));
  assert(0 == fnCall->actual(0)->toIdentifier()->name().compare("aa"));
  assert(0 == fnCall->actual(1)->toIdentifier()->name().compare("bb"));
  assert(0 == fnCall->actual(2)->toIdentifier()->name().compare("cc"));
}

static void testCalls7(Parser* parser) {
  auto parseResult = parser->parseString("testCalls6.chpl",
                                         "f(aa,b=bb,cc);\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto fnCall = module->stmt(0)->toFnCall();
  assert(fnCall);
  assert(fnCall->callUsedSquareBrackets() == false);
  auto baseExpr = fnCall->calledExpression();
  assert(baseExpr);
  auto baseExprIdent = baseExpr->toIdentifier();
  assert(0 == baseExprIdent->name().compare("f"));
  assert(fnCall->numActuals() == 3);
  assert(!fnCall->isNamedActual(0));
  assert(fnCall->isNamedActual(1));
  assert(!fnCall->isNamedActual(2));
  assert(0 == fnCall->actualName(0).compare(""));
  assert(0 == fnCall->actualName(1).compare("b"));
  assert(0 == fnCall->actualName(2).compare(""));
  assert(0 == fnCall->actual(0)->toIdentifier()->name().compare("aa"));
  assert(0 == fnCall->actual(1)->toIdentifier()->name().compare("bb"));
  assert(0 == fnCall->actual(2)->toIdentifier()->name().compare("cc"));
}

static void testOp1(Parser* parser) {
  auto parseResult = parser->parseString("testOp1.chpl",
                                         "a=b;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto opCall = module->stmt(0)->toOpCall();
  assert(opCall);
  assert(0 == opCall->op().compare("="));
  assert(opCall->isBinaryOp());
  assert(!opCall->isUnaryOp());
  assert(0 == opCall->actual(0)->toIdentifier()->name().compare("a"));
  assert(0 == opCall->actual(1)->toIdentifier()->name().compare("b"));
}

static void testOp2(Parser* parser) {
  auto parseResult = parser->parseString("testOp1.chpl",
                                         "a=b+c;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto opCall = module->stmt(0)->toOpCall();
  assert(opCall);
  assert(0 == opCall->op().compare("="));
  assert(opCall->isBinaryOp());
  assert(!opCall->isUnaryOp());
  assert(0 == opCall->actual(0)->toIdentifier()->name().compare("a"));
  auto subOpCall = opCall->actual(1)->toOpCall();
  assert(subOpCall);
  assert(0 == subOpCall->op().compare("+"));
  assert(subOpCall->isBinaryOp());
  assert(!subOpCall->isUnaryOp());
  assert(0 == subOpCall->actual(0)->toIdentifier()->name().compare("b"));
  assert(0 == subOpCall->actual(1)->toIdentifier()->name().compare("c"));
}

static void testVarDecl1(Parser* parser) {
  auto parseResult = parser->parseString("testVarDecl1.chpl",
                                         "var a: int;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto varDecl = module->stmt(0)->toVariable();
  assert(varDecl);

  assert(varDecl->name() == "a");
  assert(!varDecl->isConfig());

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  assert(varDecl->initExpression() == nullptr);
}

static void testVarDecl2(Parser* parser) {
  auto parseResult = parser->parseString("testVarDecl2.chpl",
                                         "var a = b;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto varDecl = module->stmt(0)->toVariable();
  assert(varDecl);

  assert(varDecl->name() == "a");
  assert(!varDecl->isConfig());

  assert(varDecl->typeExpression() == nullptr);

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void testVarDecl3(Parser* parser) {
  auto parseResult = parser->parseString("testVarDecl3.chpl",
                                         "var a: int = b;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto varDecl = module->stmt(0)->toVariable();
  assert(varDecl);

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void testVarDecl3a(Parser* parser) {
  auto parseResult = parser->parseString("testVarDecl3a.chpl",
                                         "var /* comment */ a: int = b;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto varDecl = module->stmt(0)->toVariable();
  assert(varDecl);

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void testVarDecl3b(Parser* parser) {
  auto parseResult = parser->parseString("testVarDecl3b.chpl",
                                         "var a /* comment */ : int = b;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto varDecl = module->stmt(0)->toVariable();
  assert(varDecl);

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void testVarDecl3c(Parser* parser) {
  auto parseResult = parser->parseString("testVarDecl3c.chpl",
                                         "var a : /* comment */ int = b;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto varDecl = module->stmt(0)->toVariable();
  assert(varDecl);

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void testVarDecl3d(Parser* parser) {
  auto parseResult = parser->parseString("testVarDecl3d.chpl",
                                         "var a : int /* comment */ = b;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto varDecl = module->stmt(0)->toVariable();
  assert(varDecl);

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void testVarDecl3e(Parser* parser) {
  auto parseResult = parser->parseString("testVarDecl3e.chpl",
                                         "var a : int = /* comment */ b;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto varDecl = module->stmt(0)->toVariable();
  assert(varDecl);

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void testVarDecl3f(Parser* parser) {
  auto parseResult = parser->parseString("testVarDecl3f.chpl",
                                         "var a : int = b /* comment */;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto varDecl = module->stmt(0)->toVariable();
  assert(varDecl);

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void testVarDecl3g(Parser* parser) {
  auto parseResult = parser->parseString("testVarDecl3g.chpl",
      "config var a : int = b /* comment */;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto varDecl = module->stmt(0)->toVariable();
  assert(varDecl);

  assert(varDecl->isConfig());
  assert(varDecl->name() == "a");

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}
static void testDot1(Parser* parser) {
  auto parseResult = parser->parseString("testDot1.chpl",
                                         "a.b;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto dotExpr = module->stmt(0)->toDot();
  assert(dotExpr);
  auto receiverIdent = dotExpr->receiver()->toIdentifier();
  assert(receiverIdent);

  assert(0 == receiverIdent->name().compare("a"));
  assert(0 == dotExpr->field().compare("b"));
}

static void testDot2(Parser* parser) {
  auto parseResult = parser->parseString("testDot2.chpl",
                                         "a.type;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto dotExpr = module->stmt(0)->toDot();
  assert(dotExpr);
  auto receiverIdent = dotExpr->receiver()->toIdentifier();
  assert(receiverIdent);

  assert(0 == receiverIdent->name().compare("a"));
  assert(0 == dotExpr->field().compare("type"));
}

static void testDot3(Parser* parser) {
  auto parseResult = parser->parseString("testDot3.chpl",
                                         "a.f(b=c);\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto fnCall = module->stmt(0)->toFnCall();
  assert(fnCall);

  auto dotExpr = fnCall->calledExpression()->toDot();
  assert(dotExpr);
  auto receiverIdent = dotExpr->receiver()->toIdentifier();
  assert(receiverIdent);

  assert(0 == receiverIdent->name().compare("a"));
  assert(0 == dotExpr->field().compare("f"));
  assert(1 == fnCall->numActuals());
  auto actualIdent = fnCall->actual(0)->toIdentifier();
  assert(actualIdent);
  assert(0 == fnCall->actualName(0).compare("b"));
  assert(0 == actualIdent->name().compare("c"));
}

static void testComment1(Parser* parser) {
  auto parseResult = parser->parseString("testComment1.chpl",
                                         "// bla\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto comment = module->stmt(0)->toComment();
  assert(comment);
  assert(0 == strcmp(comment->c_str(), "// bla"));
}

static void testComment2(Parser* parser) {
  auto parseResult = parser->parseString("testComment2.chpl",
                                         "// bla");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto comment = module->stmt(0)->toComment();
  assert(comment);
  assert(0 == strcmp(comment->c_str(), "// bla"));
}

static void testComment3(Parser* parser) {
  auto parseResult = parser->parseString("testComment3.chpl",
                                         "/* bla */");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto comment = module->stmt(0)->toComment();
  assert(comment);
  assert(0 == strcmp(comment->c_str(), "/* bla */"));
}

static void testComment4(Parser* parser) {
  auto parseResult = parser->parseString("testComment4.chpl",
                                         "/* bla");
  assert(parseResult.errors.size() >= 1);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto comment = module->stmt(0)->toComment();
  assert(comment);
  assert(0 == strcmp(comment->c_str(), "/* bla"));
}

static void testComment5(Parser* parser) {
  auto parseResult = parser->parseString("testComment5.chpl",
                                         "/* /* bla */ */");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto comment = module->stmt(0)->toComment();
  assert(comment);
  assert(0 == strcmp(comment->c_str(), "/* /* bla */ */"));
}

static void testComment6(Parser* parser) {
  auto parseResult = parser->parseString("testComment6.chpl",
                                         "/* /* bla */");
  assert(parseResult.errors.size() >= 1);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);

  auto comment = module->stmt(0)->toComment();
  assert(comment);
  assert(0 == strcmp(comment->c_str(), "/* /* bla */"));
}

int main() {
  Context context;
  Context* ctx = &context;

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
  test14(p);
  test15(p);

  testCalls1(p);
  testCalls2(p);
  testCalls3(p);
  testCalls3a(p);
  testCalls4(p);
  testCalls5(p);
  testCalls6(p);
  testCalls7(p);

  testOp1(p);
  testOp2(p);

  testVarDecl1(p);
  testVarDecl2(p);
  testVarDecl3(p);
  testVarDecl3a(p);
  testVarDecl3b(p);
  testVarDecl3c(p);
  testVarDecl3d(p);
  testVarDecl3e(p);
  testVarDecl3f(p);
  testVarDecl3g(p);

  testDot1(p);
  testDot2(p);
  testDot3(p);

  testComment1(p);
  testComment2(p);
  testComment3(p);
  testComment4(p);
  testComment5(p);
  testComment6(p);

  return 0;
}
