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
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/New.h"
#include "chpl/uast/OpCall.h"
#include "chpl/uast/Variable.h"

#include <iostream>

static void test0(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test0.chpl",
      "/* comment 1 */\n"
      "x = new r();\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isOpCall());
  const OpCall* assign = mod->stmt(1)->toOpCall();
  assert(assign);
  assert(assign->isBinaryOp());
  const FnCall* fnCall = assign->actual(1)->toFnCall();
  assert(fnCall);
  assert(fnCall->numActuals() == 0);
  assert(fnCall->calledExpression());
  const New* newExpr = fnCall->calledExpression()->toNew();
  assert(newExpr->management() == New::DEFAULT_MANAGEMENT);
  assert(newExpr->typeExpression()->isIdentifier());
}

static void test1(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test1.chpl",
      "/* comment 1 */\n"
      "x = new r(a=b, c);\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isOpCall());
  const OpCall* assign = mod->stmt(1)->toOpCall();
  assert(assign);
  assert(assign->isBinaryOp());
  const FnCall* fnCall = assign->actual(1)->toFnCall();
  assert(fnCall);
  assert(fnCall->numActuals() == 2);
  assert(fnCall->isNamedActual(0));
  assert(!fnCall->isNamedActual(1));
  assert(fnCall->calledExpression());
  const New* newExpr = fnCall->calledExpression()->toNew();
  assert(newExpr->management() == New::DEFAULT_MANAGEMENT);
  assert(newExpr->typeExpression()->isIdentifier());
}

static void test2(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test2.chpl",
      "/* comment 1 */\n"
      "x = new owned C(a=b, c);\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isOpCall());
  const OpCall* assign = mod->stmt(1)->toOpCall();
  assert(assign);
  assert(assign->isBinaryOp());
  const FnCall* fnCall = assign->actual(1)->toFnCall();
  assert(fnCall);
  assert(fnCall->numActuals() == 2);
  assert(fnCall->isNamedActual(0));
  assert(!fnCall->isNamedActual(1));
  assert(fnCall->calledExpression());
  const New* newExpr = fnCall->calledExpression()->toNew();
  assert(newExpr->management() == New::OWNED);
  assert(newExpr->typeExpression()->isIdentifier());
}

static void test3(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test3.chpl",
      "/* comment 1 */\n"
      "x = new shared C(a=b, c);\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isOpCall());
  const OpCall* assign = mod->stmt(1)->toOpCall();
  assert(assign);
  assert(assign->isBinaryOp());
  const FnCall* fnCall = assign->actual(1)->toFnCall();
  assert(fnCall);
  assert(fnCall->numActuals() == 2);
  assert(fnCall->isNamedActual(0));
  assert(!fnCall->isNamedActual(1));
  assert(fnCall->calledExpression());
  const New* newExpr = fnCall->calledExpression()->toNew();
  assert(newExpr->management() == New::SHARED);
  assert(newExpr->typeExpression()->isIdentifier());
}

static void test4(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test4.chpl",
      "/* comment 1 */\n"
      "x = new borrowed C(a=b, c);\n");
  assert(guard.realizeErrors() == 1);
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isOpCall());
  const OpCall* assign = mod->stmt(1)->toOpCall();
  assert(assign);
  assert(assign->isBinaryOp());
  const FnCall* fnCall = assign->actual(1)->toFnCall();
  assert(fnCall);
  assert(fnCall->numActuals() == 2);
  assert(fnCall->isNamedActual(0));
  assert(!fnCall->isNamedActual(1));
  assert(fnCall->calledExpression());
  const New* newExpr = fnCall->calledExpression()->toNew();
  assert(newExpr->management() == New::BORROWED);
  assert(newExpr->typeExpression()->isIdentifier());
}

static void test5(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test5.chpl",
      "/* comment 1 */\n"
      "x = new unmanaged C(a=b, c);\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isOpCall());
  const OpCall* assign = mod->stmt(1)->toOpCall();
  assert(assign);
  assert(assign->isBinaryOp());
  const FnCall* fnCall = assign->actual(1)->toFnCall();
  assert(fnCall);
  assert(fnCall->numActuals() == 2);
  assert(fnCall->isNamedActual(0));
  assert(!fnCall->isNamedActual(1));
  assert(fnCall->calledExpression());
  const New* newExpr = fnCall->calledExpression()->toNew();
  assert(newExpr->management() == New::UNMANAGED);
  assert(newExpr->typeExpression()->isIdentifier());
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

  return 0;
}
