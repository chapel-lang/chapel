/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "test-parsing.h"

#include "chpl/parsing/Parser.h"
#include "chpl/framework/Context.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/BoolLiteral.h"
#include "chpl/uast/Call.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Dot.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/OpCall.h"
#include "chpl/uast/PrimCall.h"
#include "chpl/uast/Variable.h"

static void test0(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test0.chpl", "");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->name().compare("test0") == 0);
  assert(mod->numStmts() == 0);
}

static void test1(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test1.chpl", "x;");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->name().compare("test1") == 0);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isIdentifier());
  auto identifier = mod->stmt(0)->toIdentifier();
  assert(identifier->name().compare("x") == 0);
}

static void test2(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test2.chpl", "x; y;");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->name().compare("test2") == 0);
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isIdentifier());
  assert(mod->stmt(1)->isIdentifier());
}

static void test3(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test3.chpl", "/* hi */ y;");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->name().compare("test3") == 0);
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isIdentifier());
}

static void test4(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test4.chpl", "/* hi */ y; /* bye */");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->name().compare("test4") == 0);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isIdentifier());
  assert(mod->stmt(2)->isComment());
}

static void test5(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test5.chpl",
                                         "// hi\n"
                                         "a;\n"
                                         "// bye\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isIdentifier());
  assert(mod->stmt(2)->isComment());
}

static void test6(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test6.chpl",
                                         "{ }");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isBlock());
}

static void test7(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test7.chpl",
                                         "{ a; }");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isBlock());
  const Block* block = mod->stmt(0)->toBlock();
  assert(block->numStmts()==1);
  assert(block->stmt(0)->isIdentifier());
}

static void test8(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "t.chpl", "aVeryLongIdentifierName;");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isIdentifier());
}

static void test9(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test9.chpl",
                                         "{ /* this is a comment */ }");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isBlock());
  const Block* block = mod->stmt(0)->toBlock();
  assert(block->numStmts()==1);
  assert(block->stmt(0)->isComment());
}

static void test10(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test10.chpl",
                                         "{\n"
                                         "/* this is comment 2 */\n"
                                         "aVeryLongIdentifierName;\n"
                                         "/* this is comment 3 */\n"
                                         "}\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isBlock());
  const Block* block = mod->stmt(0)->toBlock();
  assert(block->numStmts()==3);
  assert(block->stmt(0)->isComment());
  assert(block->stmt(1)->isIdentifier());
  assert(block->stmt(2)->isComment());
}


static void test11(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test11.chpl",
                                         "/* this is comment 1 */\n"
                                         "{\n"
                                         "/* this is comment 2 */\n"
                                         "aVeryLongIdentifierName;\n"
                                         "/* this is comment 3 */\n"
                                         "}\n"
                                         "/* this is comment 4 */");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isBlock());
  assert(mod->stmt(2)->isComment());
  const Block* block = mod->stmt(1)->toBlock();
  assert(block->numStmts()==3);
  assert(block->stmt(0)->isComment());
  assert(block->stmt(1)->isIdentifier());
  assert(block->stmt(2)->isComment());
}

static void test12(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test12.chpl",
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
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 5);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isComment());
  assert(mod->stmt(2)->isBlock());
  assert(mod->stmt(3)->isComment());
  assert(mod->stmt(4)->isComment());
  const Block* block = mod->stmt(2)->toBlock();
  assert(block->numStmts()==5);
  assert(block->stmt(0)->isComment());
  assert(block->stmt(1)->isComment());
  assert(block->stmt(2)->isIdentifier());
  assert(block->stmt(3)->isComment());
  assert(block->stmt(4)->isComment());
}

static void test13(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test13.chpl",
                                         "var a;\n"
                                         "a;");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isVariable());
  assert(mod->stmt(1)->isIdentifier());
}

static void test14(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test14.chpl",
                                         "{\n"
                                         " //a\n"
                                         " //b\n"
                                         "}\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isBlock());
  auto block = mod->stmt(0)->toBlock();
  assert(block->numStmts() == 2);
  assert(block->stmt(0)->isComment());
  assert(block->stmt(1)->isComment());
}

static void test15(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test15.chpl",
                                         "{\n"
                                         " x;\n"
                                         " //a\n"
                                         " //b\n"
                                         "}\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isBlock());
  auto block = mod->stmt(0)->toBlock();
  assert(block->numStmts() == 3);
  assert(block->stmt(0)->isIdentifier());
  assert(block->stmt(1)->isComment());
  assert(block->stmt(2)->isComment());
}

static void testCalls1(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testCalls1.chpl",
                                         "f();\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto fnCall = mod->stmt(0)->toFnCall();
  assert(fnCall);
  assert(fnCall->callUsedSquareBrackets() == false);
  auto baseExpr = fnCall->calledExpression();
  assert(baseExpr);
  auto baseExprIdent = baseExpr->toIdentifier();
  assert(0 == baseExprIdent->name().compare("f"));
  assert(fnCall->numActuals() == 0);
}

static void testCalls2(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testCalls2.chpl",
                                         "f[];\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto fnCall = mod->stmt(0)->toFnCall();
  assert(fnCall);
  assert(fnCall->callUsedSquareBrackets() == true);
  auto baseExpr = fnCall->calledExpression();
  assert(baseExpr);
  auto baseExprIdent = baseExpr->toIdentifier();
  assert(0 == baseExprIdent->name().compare("f"));
  assert(fnCall->numActuals() == 0);
}

static void testCalls3(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testCalls3.chpl",
                                         "f(x);\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto fnCall = mod->stmt(0)->toFnCall();
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
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testCalls3a.chpl",
                                         "f(g(x));\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto fnCall = mod->stmt(0)->toFnCall();
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
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testCalls4.chpl",
                                         "f[x];\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto fnCall = mod->stmt(0)->toFnCall();
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
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testCalls5.chpl",
                                         "f(a,b,c);\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto fnCall = mod->stmt(0)->toFnCall();
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
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testCalls6.chpl",
                                         "f(a=aa,b=bb,c=cc);\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto fnCall = mod->stmt(0)->toFnCall();
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
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testCalls6.chpl",
                                         "f(aa,b=bb,cc);\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto fnCall = mod->stmt(0)->toFnCall();
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
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testOp1.chpl",
                                         "a=b;\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto opCall = mod->stmt(0)->toOpCall();
  assert(opCall);
  assert(0 == opCall->op().compare("="));
  assert(opCall->isBinaryOp());
  assert(!opCall->isUnaryOp());
  assert(0 == opCall->actual(0)->toIdentifier()->name().compare("a"));
  assert(0 == opCall->actual(1)->toIdentifier()->name().compare("b"));
}

static void testOp2(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testOp1.chpl",
                                         "a=b+c;\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto opCall = mod->stmt(0)->toOpCall();
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

static void testDot1(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testDot1.chpl",
                                         "a.b;\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto dotExpr = mod->stmt(0)->toDot();
  assert(dotExpr);
  auto receiverIdent = dotExpr->receiver()->toIdentifier();
  assert(receiverIdent);

  assert(0 == receiverIdent->name().compare("a"));
  assert(0 == dotExpr->field().compare("b"));
}

static void testDot2(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testDot2.chpl",
                                         "a.type;\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto dotExpr = mod->stmt(0)->toDot();
  assert(dotExpr);
  auto receiverIdent = dotExpr->receiver()->toIdentifier();
  assert(receiverIdent);

  assert(0 == receiverIdent->name().compare("a"));
  assert(0 == dotExpr->field().compare("type"));
}

static void testDot3(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testDot3.chpl",
                                         "a.f(b=c);\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto fnCall = mod->stmt(0)->toFnCall();
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
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testComment1.chpl",
                                         "// bla\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto comment = mod->stmt(0)->toComment();
  assert(comment);
  assert(0 == strcmp(comment->c_str(), "// bla"));
}

static void testComment2(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testComment2.chpl",
                                         "// bla");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto comment = mod->stmt(0)->toComment();
  assert(comment);
  assert(0 == strcmp(comment->c_str(), "// bla"));
}

static void testComment3(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testComment3.chpl",
                                         "/* bla */");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto comment = mod->stmt(0)->toComment();
  assert(comment);
  assert(0 == strcmp(comment->c_str(), "/* bla */"));
}

static void testComment4(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testComment4.chpl",
                                         "/* bla");
  assert(guard.realizeErrors() >= 1);
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto comment = mod->stmt(0)->toComment();
  assert(comment);
  assert(0 == strcmp(comment->c_str(), "/* bla"));
}

static void testComment5(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testComment5.chpl",
                                         "/* /* bla */ */");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto comment = mod->stmt(0)->toComment();
  assert(comment);
  assert(0 == strcmp(comment->c_str(), "/* /* bla */ */"));
}

static void testComment6(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testComment6.chpl",
                                         "/* /* bla */");
  assert(guard.realizeErrors() >= 1);
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto comment = mod->stmt(0)->toComment();
  assert(comment);
  assert(0 == strcmp(comment->c_str(), "/* /* bla */"));
}

static void testBoolLiteral0(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testBoolLiteral0.chpl",
                                         "var f = false;\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto var = mod->stmt(0)->toVariable();
  assert(var);

  assert(!var->typeExpression() && var->initExpression());
  const BoolLiteral* b = var->initExpression()->toBoolLiteral();
  assert(b);
  assert(b->value() == false);
}

static void testBoolLiteral1(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testBoolLiteral1.chpl",
                                         "var f = true;\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto var = mod->stmt(0)->toVariable();
  assert(var);

  assert(!var->typeExpression() && var->initExpression());
  const BoolLiteral* b = var->initExpression()->toBoolLiteral();
  assert(b);
  assert(b->value() == true);
}

static void testPrimCall0(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testPrimCall0.chpl",
                                         "__primitive(\"u-\");\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto prim = mod->stmt(0)->toPrimCall();
  assert(prim);

  assert(prim->numActuals() == 0);
  assert(prim->calledExpression() == nullptr);
  assert(prim->prim() == PRIM_UNARY_MINUS);
}
static void testPrimCall1(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "testPrimCall1.chpl",
                                         "__primitive(\"u-\", x);\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto prim = mod->stmt(0)->toPrimCall();
  assert(prim);

  assert(prim->calledExpression() == nullptr);
  assert(prim->prim() == PRIM_UNARY_MINUS);
  assert(prim->numActuals() == 1);
  auto ident = prim->actual(0)->toIdentifier();
  assert(ident);
  assert(ident->name() == "x");
}

static void testAttributeOnClass(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parser->parseString("testAttributeOnClass.chpl",
                                         "@thekitchensink.inner.attribute(true, 'life', 42, 4.2, 1.28..)\n \
                                          class MyClass { \n }");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto cls = mod->stmt(0)->toClass();
  assert(cls);
  assert(cls->attributeGroupChildNum() > -1);

  auto attrGrp = cls->attributeGroup()->toAttributeGroup();
  assert(attrGrp);
  assert(attrGrp->numAttributes() == 1);
  auto ctx = parser->context();
  auto complexName = UniqueString::get(ctx, "thekitchensink.inner.attribute");
  auto attr = attrGrp->getAttributeNamed(complexName);
  assert(attr);
  auto attrNull = attrGrp->getAttributeNamed(UniqueString::get(ctx, "nodoc"));
  assert(attrNull==nullptr);
  assert(attr->fullyQualifiedAttributeName()=="thekitchensink.inner.attribute");
  assert(attr->numActuals() == 5);
  assert(attr->actual(0)->toBoolLiteral()->value());
  assert(attr->actual(1)->toStringLiteral()->value()=="life");
  assert(attr->actual(2)->toIntLiteral()->value()==42);
  assert(attr->actual(3)->toRealLiteral()->value()==4.2);
  auto range = attr->actual(4)->toRange();
  assert(range->lowerBound()->toRealLiteral()->value()==1.28);
  assert(range->upperBound()==nullptr);
}

static void testMultiAttributesOnClass(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parser->parseString("testMultiAttributesOnClass.chpl",
                                         R""""(
                                          @attribute(true)
                                          @attribute1()
                                          @attribute2('words')
                                          @attribute3(42)
                                          @attribute4()
                                          class MyClass { })"""");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto cls = mod->stmt(0)->toClass();
  assert(cls);
  assert(cls->attributeGroupChildNum() > -1);
  auto attrGrp = cls->attributeGroup()->toAttributeGroup();
  assert(attrGrp);
  assert(attrGrp->numAttributes() == 5);
  auto ctx = parser->context();
  auto attr0 = attrGrp->getAttributeNamed(UniqueString::get(ctx, "attribute"));
  auto attr1 = attrGrp->getAttributeNamed(UniqueString::get(ctx, "attribute1"));
  auto attr2 = attrGrp->getAttributeNamed(UniqueString::get(ctx, "attribute2"));
  auto attr3 = attrGrp->getAttributeNamed(UniqueString::get(ctx, "attribute3"));
  auto attr4 = attrGrp->getAttributeNamed(UniqueString::get(ctx, "attribute4"));
  auto attrNull = attrGrp->getAttributeNamed(UniqueString::get(ctx, "attribute5"));
  assert(attrNull==nullptr);
  assert(attr0);
  assert(attr1);
  assert(attr2);
  assert(attr3);
  assert(attr4);
  assert(attr0->name() == "attribute");
  assert(attr1->name() == "attribute1");
  assert(attr2->name() == "attribute2");
  assert(attr3->name() == "attribute3");
  assert(attr4->name() == "attribute4");
}

static void testMultiAttributesAndPragma(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parser->parseString("testMultiAttributesAndPragma.chpl",
                                         R""""(
                                          pragma "do not RVF"
                                          @attribute(true)
                                          @attribute1()
                                          @attribute2('words')
                                          @attribute3(42)
                                          @attribute4()
                                          class MyClass { })"""");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto cls = mod->stmt(0)->toClass();
  assert(cls);
  assert(cls->attributeGroupChildNum() > -1);
  auto attrGrp = cls->attributeGroup()->toAttributeGroup();
  assert(attrGrp);
  assert(attrGrp->numAttributes() == 5);
  auto ctx = parser->context();
  auto attr0 = attrGrp->getAttributeNamed(UniqueString::get(ctx, "attribute"));
  auto attr1 = attrGrp->getAttributeNamed(UniqueString::get(ctx, "attribute1"));
  auto attr2 = attrGrp->getAttributeNamed(UniqueString::get(ctx, "attribute2"));
  auto attr3 = attrGrp->getAttributeNamed(UniqueString::get(ctx, "attribute3"));
  auto attr4 = attrGrp->getAttributeNamed(UniqueString::get(ctx, "attribute4"));
  assert(attrGrp->hasPragma(PragmaTag::PRAGMA_NO_RVF));
  assert(attr0);
  assert(attr1);
  assert(attr2);
  assert(attr3);
  assert(attr4);
  assert(attr0->name() == "attribute");
  assert(attr1->name() == "attribute1");
  assert(attr2->name() == "attribute2");
  assert(attr3->name() == "attribute3");
  assert(attr4->name() == "attribute4");
}


static void testAttributeAndPragmaLast(Parser* parser) {
  // TODO: this test fails, currently the pragma must be first, before any
  // attributes
  // ErrorGuard guard(parser->context());
  // auto parseResult = parser->parseString("testAttributeOnClass.chpl",
  //                                        R""""(
  //                                         @attribute(true)
  //                                         pragma "no doc"
  //                                         class MyClass { })"""");
  // assert(!guard.realizeErrors());
  // auto mod = parseResult.singleModule();
  // assert(mod);
  // assert(mod->numStmts() == 1);

  // auto cls = mod->stmt(0)->toClass();
  // assert(cls);
  // assert(cls->attributeGroupChildNum() > -1);
  // auto attrGrp = cls->attributeGroup()->toAttributeGroup();
  // assert(attrGrp);
  // assert(attrGrp->numAttributes() == 1);
  // auto attr0 = attrGrp->child(0)->toAttribute();
  // assert(attrGrp->hasPragma(PRAGMA_NO_DOC));
  // assert(attr0);
  // assert(attr0->name() == "attribute");
}


static void testAttributeAndUnstable(Parser* parser) {
 ErrorGuard guard(parser->context());
 auto parseResult = parser->parseString("testAttributeAndUnstable.chpl",
                                        R""""(
                                         @unstable("we're not sure about this")
                                         @attribute(true)
                                         class MyClass { })"""");
 assert(!guard.realizeErrors());
 auto mod = parseResult.singleModule();
 assert(mod);
 assert(mod->numStmts() == 1);

 auto cls = mod->stmt(0)->toClass();
 assert(cls);
 assert(cls->attributeGroupChildNum() > -1);
 auto attrGrp = cls->attributeGroup()->toAttributeGroup();
 assert(attrGrp);
 assert(attrGrp->numAttributes() == 2);
 auto attr0 = attrGrp->getAttributeNamed(UniqueString::get(parser->context(), "attribute"));
 auto attrUnstable = attrGrp->getAttributeNamed(USTR("unstable"));
 assert(attrUnstable);
 assert(attrGrp->isUnstable());
 assert(attr0);
 assert(attr0->name() == "attribute");
}


static void testAttributeAndUnstableLast(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parser->parseString("testAttributeAndUnstableLast.chpl",
                                         R""""(
                                         @attribute(true)
                                         @unstable
                                         class MyClass { })"""");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto cls = mod->stmt(0)->toClass();
  assert(cls);
  assert(cls->attributeGroupChildNum() > -1);
  auto attrGrp = cls->attributeGroup()->toAttributeGroup();
  assert(attrGrp);
  assert(attrGrp->isUnstable());
  assert(attrGrp->numAttributes() == 2);
  auto attr0 = attrGrp->getAttributeNamed(UniqueString::get(parser->context(), "attribute"));
  auto attrUnstable = attrGrp->getAttributeNamed(USTR("unstable"));
  assert(attrUnstable);
  assert(attr0);
  assert(attr0->name() == "attribute");
}


static void testAttributeAndUnstableMessage(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parser->parseString("testAttributeAndUnstableMessage.chpl",
                                         R""""(
                                         @attribute(true)
                                         @unstable("this will be changed later")
                                         class MyClass { })"""");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto ctx = parser->context();
  auto cls = mod->stmt(0)->toClass();
  assert(cls);
  assert(cls->attributeGroupChildNum() > -1);
  auto attrGrp = cls->attributeGroup()->toAttributeGroup();
  assert(attrGrp);
  assert(attrGrp->isUnstable());
  assert(attrGrp->unstableMessage() == UniqueString::get(ctx, "this will be changed later"));
  assert(attrGrp->numAttributes() == 2);
  auto attr0 = attrGrp->getAttributeNamed(UniqueString::get(ctx, "attribute"));
  auto attrUnstable = attrGrp->getAttributeNamed(USTR("unstable"));
  assert(attrUnstable);
  assert(attr0);
  assert(attr0->name() == "attribute");
  assert(attrUnstable->numActuals() == 1);
  assert(attrUnstable->getOnlyStringActualOrError(ctx) == UniqueString::get(ctx, "this will be changed later"));
}

static void testAttributeAndDeprecatedLast(Parser* parser) {
  // deprecated as a token, last, no arg
  ErrorGuard guard(parser->context());
  auto parseResult = parser->parseString("testAttributeAndDeprecatedLast.chpl",
                                         R""""(
                                         @attribute("word")
                                         @deprecated
                                         class MyClass { })"""");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto ctx = parser->context();
  auto cls = mod->stmt(0)->toClass();
  assert(cls);
  assert(cls->attributeGroupChildNum() > -1);
  auto attrGrp = cls->attributeGroup()->toAttributeGroup();
  assert(attrGrp);
  assert(attrGrp->isDeprecated());
  assert(attrGrp->numAttributes() == 2);
  auto attr0 = attrGrp->getAttributeNamed(UniqueString::get(ctx, "attribute"));
  auto attrDeprecated = attrGrp->getAttributeNamed(USTR("deprecated"));
  assert(attrDeprecated);
  assert(attr0);
  assert(attr0->name() == "attribute");
  assert(
      attr0->getOnlyStringActualOrError(ctx) == UniqueString::get(ctx, "word"));
}

static void testAttributeAndDeprecatedMessage(Parser* parser) {
  // deprecated as a token TDEPRECATED, last, with arg
  ErrorGuard guard(parser->context());
  auto parseResult = parser->parseString("testAttributeAndDeprecatedMessage.chpl",
                                         R""""(
                                         @attribute("word")
                                         @deprecated(notes="try using something else")
                                         class MyClass { })"""");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto ctx = parser->context();
  auto cls = mod->stmt(0)->toClass();
  assert(cls);
  assert(cls->attributeGroupChildNum() > -1);
  auto attrGrp = cls->attributeGroup()->toAttributeGroup();
  assert(attrGrp);
  assert(attrGrp->isDeprecated());
  assert(attrGrp->deprecationMessage() == UniqueString::get(ctx,"try using something else"));
  assert(attrGrp->numAttributes() == 2);
  auto attr0 = attrGrp->getAttributeNamed(UniqueString::get(ctx, "attribute"));
  auto attrDeprecated = attrGrp->getAttributeNamed(USTR("deprecated"));
  assert(attrDeprecated);
  assert(attr0);
  assert(attr0->name() == "attribute");
  assert(
      attr0->getOnlyStringActualOrError(ctx) == UniqueString::get(ctx, "word"));
  assert(attrDeprecated->getOnlyStringActualOrError(ctx) == UniqueString::get(ctx, "try using something else"));
}

static void testAttributeAndDeprecated(Parser* parser) {
  // deprecated as an attribute @deprecated, first, no arg
  ErrorGuard guard(parser->context());
  auto parseResult = parser->parseString("testAttributeAndDeprecated.chpl",
                                        R""""(
                                        @deprecated
                                        @attribute(true)
                                        class MyClass { })"""");
   assert(!guard.realizeErrors());
   auto mod = parseResult.singleModule();
   assert(mod);
   assert(mod->numStmts() == 1);

   auto cls = mod->stmt(0)->toClass();
   assert(cls);
   assert(cls->attributeGroupChildNum() > -1);
   auto attrGrp = cls->attributeGroup()->toAttributeGroup();
   assert(attrGrp);
   assert(attrGrp->isDeprecated());
   assert(attrGrp->numAttributes() == 2);
   auto attr0 = attrGrp->getAttributeNamed(UniqueString::get(parser->context(), "attribute"));
   auto attrDeprecated = attrGrp->getAttributeNamed(USTR("deprecated"));
   assert(attrDeprecated);
   assert(attr0);
   assert(attr0->name() == "attribute");
}

static void testAttributeNamedArgs(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto program = R""""(
    @myAttribute(reason="my rationale")
    @myAttribute2(id=1234, feature="foo")
    @myAttribute3(since=1.30, feature="bar", reason="my rationale", issue=5678)
    proc foo { }
  )"""";

  auto parseResult =
  parseStringAndReportErrors(parser, "testAttributeNamedArgs.chpl", program);
  assert(guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto p = mod->stmt(0)->toFunction();
  assert(p);
  auto attr = p->attributeGroup();
  assert(attr);
  assert(attr->numAttributes() == 3);
  auto attr1 = attr->getAttributeNamed(UniqueString::get(parser->context(), "myAttribute"));
  assert(attr1);
  assert(attr1->numActuals() == 1);
  auto actual = attr1->actual(0);
  assert(actual);
  assert(attr1->isNamedActual(0));
  assert(attr1->actualName(0) == UniqueString::get(parser->context(), "reason"));
  assert(actual->isStringLiteral());
  auto attr2 = attr->getAttributeNamed(UniqueString::get(parser->context(), "myAttribute2"));
  assert(attr2);
  assert(attr2->numActuals() == 2);
  assert(attr2->isNamedActual(0));
  assert(attr2->actualName(0) == UniqueString::get(parser->context(), "id"));
  assert(attr2->isNamedActual(1));
  assert(attr2->actualName(1) == UniqueString::get(parser->context(), "feature"));
  assert(attr2->actual(0)->isIntLiteral());
  assert(attr2->actual(1)->isStringLiteral());
  auto attr3 = attr->getAttributeNamed(UniqueString::get(parser->context(), "myAttribute3"));
  assert(attr3);
  assert(attr3->numActuals() == 4);
  assert(attr3->isNamedActual(0));
  assert(attr3->actualName(0) == UniqueString::get(parser->context(), "since"));
  assert(attr3->isNamedActual(1));
  assert(attr3->actualName(1) == UniqueString::get(parser->context(), "feature"));
  assert(attr3->isNamedActual(2));
  assert(attr3->actualName(2) == UniqueString::get(parser->context(), "reason"));
  assert(attr3->isNamedActual(3));
  assert(attr3->actualName(3) == UniqueString::get(parser->context(), "issue"));
  assert(attr3->actual(0)->isRealLiteral());
  assert(attr3->actual(1)->isStringLiteral());
  assert(attr3->actual(2)->isStringLiteral());
  assert(attr3->actual(3)->isIntLiteral());
}

static void testAttributeMixedNamedArgs(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto program = R""""(
    @myAttribute(1, fade=2, 3)
    proc foo { }
  )"""";

  auto parseResult = parseStringAndReportErrors(parser, "testAttributeMixedNamedArgs.chpl", program);
  assert(guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto p = mod->stmt(0)->toFunction();
  assert(p);
  auto attr = p->attributeGroup();
  assert(attr);
  assert(attr->numAttributes() == 1);
  auto attr1 = attr->getAttributeNamed(UniqueString::get(parser->context(), "myAttribute"));
  assert(attr1);
  assert(attr1->numActuals() == 3);
  assert(attr1->isNamedActual(1));
  assert(attr1->actualName(1) == UniqueString::get(parser->context(), "fade"));
  assert(attr1->actual(0)->isIntLiteral());
  assert(attr1->actual(1)->isIntLiteral());
  assert(attr1->actual(2)->isIntLiteral());
}

static void testAttributeOnLoop(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto program = R""""(
    @myAttribute
    proc foo(A, ref sum) {
      @myLoopAttribute("some arg here")
      for i in A.domain {
        sum += A[i];
      }
      @myLoopAttribute("some arg here")
      for i in A.domain {
        const ref a = A[i];
        sum += a;
      }
      @myLoopAttribute("some arg here")
      // comment in the way
      for i in A.domain {
        const ref a = A[i];
        sum += a;
      }
      @myLoopAttribute("some arg here")
      foreach i in A.domain {
        sum += A[i];
      }
      @myLoopAttribute("some arg here")
      foreach i in A.domain {
        const ref a = A[i];
        sum += a;
      }
      @myLoopAttribute("some arg here")
      // comment in the way
      foreach i in A.domain {
        const ref a = A[i];
        sum += a;
      }
      @myLoopAttribute("some arg here")
      forall i in A.domain {
        sum += A[i];
      }
      @myLoopAttribute("some arg here")
      forall i in A.domain {
        const ref a = A[i];
        sum += a;
      }
      @myLoopAttribute("some arg here")
      // comment in the way
      forall i in A.domain {
        const ref a = A[i];
        sum += a;
      }
      @myLoopAttribute("some arg here")
      coforall i in A.domain {
        sum += A[i];
      }
      @myLoopAttribute("some arg here")
      coforall i in A.domain {
        const ref a = A[i];
        sum += a;
      }
      @myLoopAttribute("some arg here")
      // comment in the way
      coforall i in A.domain {
        const ref a = A[i];
        sum += a;
      }
      @myLoopAttribute("some arg here")
      while true {
        sum += A[A.domain.low];
      }
      @myLoopAttribute("some arg here")
      while true {
        const ref a = A[A.domain.low];
        sum += a;
      }
      @myLoopAttribute("some arg here")
      // comment in the way
      while true {
        const ref a = A[A.domain.low];
        sum += a;
      }
      @myLoopAttribute("some arg here")
      do {
        sum += A[A.domain.low];
      } while false;
      @myLoopAttribute("some arg here")
      do {
        const ref a = A[A.domain.low];
        sum += a;
      } while false;
      @myLoopAttribute("some arg here")
      // comment in the way
      do {
        const ref a = A[A.domain.low];
        sum += a;
      } while false;
      @myLoopAttribute("some arg here")
      [i in A.domain] {
        sum += A[i];
      }
      @myLoopAttribute("some arg here")
      [i in A.domain] {
        const ref a = A[i];
        sum += a;
      }
      @myLoopAttribute("some arg here")
      // comment in the way
      [i in A.domain] {
        const ref a = A[i];
        sum += a;
      }
    }
  )"""";

  auto parseResult = parseStringAndReportErrors(parser, "testAttributeOnLoop.chpl", program);
  assert(guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto f = mod->stmt(0)->toFunction();
  assert(f);
  // 7 kinds of loops, 3 for each kind plus 1 comment
  assert(f->numStmts() == (7*(3+1)));
  for(auto s: f->stmts()) {
    assert(s);
    assert(s->isLoop() || s->isComment());
    if(s->isLoop()) {
      auto attr = s->attributeGroup();
      assert(attr->numAttributes() == 1);
      auto attr1 = attr->getAttributeNamed(UniqueString::get(parser->context(), "myLoopAttribute"));
      assert(attr1);
    }
  }
}

static void testAttributeAndNoAttributeOnLoop(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto program = R""""(
    proc foo(A, ref sum) {
      @loopAttr()
      foreach i in A.domain {
        sum += A[i];
      }
    }
    proc bar(A, ref sum) {
      for i in A.domain {
        sum += A[i];
      }
    }
  )"""";

  auto parseResult = parseStringAndReportErrors(parser, "testAttributeAndNoAttributeOnLoop.chpl", program);
  assert(guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 2);

  auto foo = mod->stmt(0)->toFunction();
  assert(foo);
  auto bar = mod->stmt(1)->toFunction();
  assert(bar);

  assert(foo->numStmts() == 1);
  assert(bar->numStmts() == 1);

  auto fooLoop = foo->stmt(0)->toForeach();
  assert(fooLoop);
  auto barLoop = bar->stmt(0)->toFor();
  assert(barLoop);

  // bar should have no attributes
  assert(!barLoop->attributeGroup());

  auto loopAttr = fooLoop->attributeGroup();
  assert(loopAttr);
  assert(loopAttr->getAttributeNamed(UniqueString::get(parser->context(), "loopAttr")));
}


/* a test of the parser's ability to parse a proc with an attribute that has
   3 named arguments
*/
static void testAttribute3NamedArgs(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto program = R""""(
    @myAttribute(reason="my rationale", feature="foo", issue=1234)
    proc foo() { }
  )"""";

  auto parseResult = parseStringAndReportErrors(parser, "testAttribute3NamedArgs.chpl", program);
  assert(guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto p = mod->stmt(0)->toFunction();
  assert(p);
  auto attr = p->attributeGroup();
  assert(attr);
  assert(attr->numAttributes() == 1);
  auto attr1 = attr->getAttributeNamed(UniqueString::get(parser->context(), "myAttribute"));
  assert(attr1);
  assert(attr1->numActuals() == 3);
  assert(attr1->isNamedActual(0));
  assert(attr1->actualName(0) == UniqueString::get(parser->context(), "reason"));
  assert(attr1->actual(0)->isStringLiteral());
  assert(attr1->isNamedActual(1));
  assert(attr1->actualName(1) == UniqueString::get(parser->context(), "feature"));
  assert(attr1->actual(1)->isStringLiteral());
  assert(attr1->isNamedActual(2));
  assert(attr1->actualName(2) == UniqueString::get(parser->context(), "issue"));
  assert(attr1->actual(2)->isIntLiteral());
}

/* a test of the pragma chpldoc ignore chpl prefix*/
static void testPragmaChpldocIgnoreChplPrefix(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto program = R""""(
    module M {
      pragma "chpldoc ignore chpl prefix"
      proc chpl_foo() { }
    }
  )"""";

  auto parseResult = parseStringAndReportErrors(parser, "testPragmaChpldocIgnoreChplPrefix.chpl", program);

  auto mod = parseResult.singleModule();
  assert(guard.numErrors()==0);
  assert(mod);
  assert(mod->numStmts() == 1);
  auto p = mod->stmt(0)->toFunction();
  assert(p);
  auto attr = p->attributeGroup();
  assert(attr);
  assert(attr->numAttributes() == 0);
  assert(attr->hasPragma(PragmaTag::PRAGMA_CHPLDOC_IGNORE_CHPL_PREFIX));
}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::createForTopLevelModule(ctx);
  Parser* p = &parser;

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

  testDot1(p);
  testDot2(p);
  testDot3(p);

  testComment1(p);
  testComment2(p);
  testComment3(p);
  testComment4(p);
  testComment5(p);
  testComment6(p);

  testBoolLiteral0(p);
  testBoolLiteral1(p);

  testPrimCall0(p);
  testPrimCall1(p);

  testAttributeOnClass(p);
  testMultiAttributesOnClass(p);
  testMultiAttributesAndPragma(p);
  testAttributeAndPragmaLast(p);
  testAttributeAndUnstable(p);
  testAttributeAndUnstableLast(p);
  testAttributeAndDeprecated(p);
  testAttributeAndDeprecatedLast(p);
  testAttributeAndDeprecatedMessage(p);
  testAttributeAndUnstableMessage(p);
  testAttributeNamedArgs(p);
  testAttributeMixedNamedArgs(p);
  testAttribute3NamedArgs(p);
  testAttributeOnLoop(p);
  testAttributeAndNoAttributeOnLoop(p);
  testPragmaChpldocIgnoreChplPrefix(p);

  return 0;
}
