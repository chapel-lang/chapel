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
#include "chpl/uast/Coforall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/TaskVar.h"
#include "chpl/uast/WithClause.h"
#include "chpl/uast/Zip.h"

#include <iostream>

static void test0(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test0.chpl",
      "/* comment 1 */\n"
      "coforall x in foo do\n"
      "  /* comment 2 */\n"
      "  foo();\n"
      "/* comment 3 */\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isCoforall());
  assert(mod->stmt(2)->isComment());
  const Coforall* coforall = mod->stmt(1)->toCoforall();
  assert(coforall != nullptr);
  assert(coforall->index() != nullptr);
  assert(coforall->index()->isVariable());
  assert(coforall->iterand() != nullptr);
  assert(coforall->iterand()->isIdentifier());
  assert(coforall->withClause() == nullptr);
  assert(coforall->numStmts() == 2);
  assert(coforall->blockStyle() == BlockStyle::IMPLICIT);
  assert(!coforall->isExpressionLevel());
  assert(coforall->stmt(0)->isComment());
  assert(coforall->stmt(1)->isFnCall());
}

static void test1(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test1.chpl",
      "/* comment 1 */\n"
      "coforall x in foo with (ref thing) do {\n"
      "  /* comment 2 */\n"
      "  foo();\n"
      "  /* comment 3 */\n"
      "}\n"
      "/* comment 4 */\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isCoforall());
  assert(mod->stmt(2)->isComment());
  const Coforall* coforall = mod->stmt(1)->toCoforall();
  assert(coforall != nullptr);
  assert(coforall->index() != nullptr);
  assert(coforall->index()->isVariable());
  assert(coforall->iterand() != nullptr);
  assert(coforall->iterand()->isIdentifier());
  const WithClause* withClause = coforall->withClause();
  assert(withClause);
  assert(withClause->numExprs() == 1);
  const TaskVar* taskVar = withClause->expr(0)->toTaskVar();
  assert(taskVar);
  assert(!taskVar->typeExpression());
  assert(!taskVar->initExpression());
  assert(taskVar->intent() == TaskVar::REF);
  assert(coforall->blockStyle() == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK);
  assert(!coforall->isExpressionLevel());
  assert(coforall->numStmts() == 3);
  assert(coforall->stmt(0)->isComment());
  assert(coforall->stmt(1)->isFnCall());
  assert(coforall->stmt(2)->isComment());
}

static void test2(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test2.chpl",
      "/* comment 1 */\n"
      "coforall x in zip(a, b) {\n"
      "  foo();\n"
      "}\n"
      "/* comment 4 */\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isCoforall());
  assert(mod->stmt(2)->isComment());
  const Coforall* coforall = mod->stmt(1)->toCoforall();
  assert(coforall != nullptr);
  assert(coforall->index() != nullptr);
  assert(coforall->index()->isVariable());
  assert(coforall->iterand() != nullptr);
  const Zip* zip = coforall->iterand()->toZip();
  assert(zip);
  assert(zip->numActuals() == 2);
  assert(zip->actual(0)->isIdentifier());
  assert(zip->actual(1)->isIdentifier());
  assert(coforall->blockStyle() == BlockStyle::EXPLICIT);
  assert(!coforall->isExpressionLevel());
  assert(coforall->numStmts() == 1);
  assert(coforall->stmt(0)->isFnCall());
}

static void test3(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test3.chpl",
      "/* comment 1 */\n"
      "coforall x in zip(a, b, foo()) with (const ref thing1, in thing2=d) {\n"
      "  writeln(thing1);\n"
      "  thing2 = thing3;\n"
      "}\n"
      "/* comment 4 */\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isCoforall());
  assert(mod->stmt(2)->isComment());
  const Coforall* coforall = mod->stmt(1)->toCoforall();
  assert(coforall != nullptr);
  assert(coforall->index() != nullptr);
  assert(coforall->index()->isVariable());
  assert(coforall->iterand() != nullptr);
  const Zip* zip = coforall->iterand()->toZip();
  assert(zip);
  assert(zip->numActuals() == 3);
  assert(zip->actual(0)->isIdentifier());
  assert(zip->actual(1)->isIdentifier());
  assert(zip->actual(2)->isFnCall());
  const WithClause* withClause = coforall->withClause();
  assert(withClause);
  assert(withClause->numExprs() == 2);
  const TaskVar* taskVar0 = withClause->expr(0)->toTaskVar();
  assert(taskVar0);
  assert(!taskVar0->typeExpression());
  assert(!taskVar0->initExpression());
  assert(taskVar0->intent() == TaskVar::CONST_REF);
  const TaskVar* taskVar1 = withClause->expr(1)->toTaskVar();
  assert(taskVar1);
  assert(!taskVar1->typeExpression());
  assert(taskVar1->initExpression());
  assert(taskVar1->initExpression()->isIdentifier());
  assert(taskVar1->intent() == TaskVar::IN);
  assert(coforall->blockStyle() == BlockStyle::EXPLICIT);
  assert(!coforall->isExpressionLevel());
  assert(coforall->numStmts() == 2);
  assert(coforall->stmt(0)->isFnCall());
  assert(coforall->stmt(1)->isOpCall());
}

static void test4(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test4.chpl",
      "/* comment 1 */\n"
      "coforall foo() do\n"
      "  /* comment 2 */\n"
      "  bar();\n"
      "/* comment 3 */\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isCoforall());
  assert(mod->stmt(2)->isComment());
  const Coforall* coforall = mod->stmt(1)->toCoforall();
  assert(coforall != nullptr);
  assert(coforall->index() == nullptr);
  assert(coforall->iterand() != nullptr);
  assert(coforall->iterand()->isFnCall());
  assert(coforall->withClause() == nullptr);
  assert(coforall->numStmts() == 2);
  assert(coforall->blockStyle() == BlockStyle::IMPLICIT);
  assert(!coforall->isExpressionLevel());
  assert(coforall->stmt(0)->isComment());
  assert(coforall->stmt(1)->isFnCall());
}

static void test5(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test5.chpl",
      "coforall i in 1..10 with (re A) { }\n"
      "coforall i in 1..10 with () { }\n"
      "coforall i in 1..10 with ref A { }\n");
  auto numErrors = 5;
  assert(guard.errors().size() == (size_t) numErrors);
  assert("invalid intent expression in 'with' clause" == guard.error(1)->message());
  assert("'with' clause cannot be empty" == guard.error(2)->message());
  assert("missing parentheses around 'with' clause intents" == guard.error(4)->message());
  // The other errors are from the parser as "near ...".
  // It would be really nice to not have those be emitted at all.
  assert(guard.realizeErrors() == numErrors);
}

static void test6(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test6.chpl",
      "coforall a { }\n"
      "coforall zip(a,b) { }\n"
      "var c1 = coforall a do 1;\n"
      "var c2 = coforall zip(a,b) do 1;\n");
  auto numErrors = 2;
  assert(guard.errors().size() == (size_t) numErrors);
  assert("expression level 'coforall' loops are not supported" == guard.error(0)->message());
  assert("expression level loops with 'zip' must have an index" == guard.error(1)->message());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 4);
  assert(mod->stmt(0)->isCoforall());
  assert(mod->stmt(1)->isCoforall());
  assert(mod->stmt(2)->isVariable());
  assert(mod->stmt(3)->isVariable());

  auto coforall1 = mod->stmt(0)->toCoforall();
  assert(coforall1 != nullptr);
  assert(coforall1->index() == nullptr);
  assert(coforall1->iterand() != nullptr);
  assert(coforall1->iterand()->isIdentifier());
  assert(coforall1->numStmts() == 0);

  auto coforall2 = mod->stmt(1)->toCoforall();
  assert(coforall2 != nullptr);
  assert(coforall2->index() == nullptr);
  assert(coforall2->iterand() != nullptr);
  assert(coforall2->iterand()->isZip());
  assert(coforall2->numStmts() == 0);
  assert(guard.realizeErrors() == numErrors);

  auto var1 = mod->stmt(2)->toVariable();
  assert(var1 != nullptr);
  assert(var1->initExpression() != nullptr);
  assert(var1->initExpression()->isErroneousExpression());

  auto var2 = mod->stmt(3)->toVariable();
  assert(var2 != nullptr);
  assert(var2->initExpression() != nullptr);
  assert(var2->initExpression()->isErroneousExpression());
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

  return 0;
}
