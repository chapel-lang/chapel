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
#include "chpl/uast/For.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"

#include <iostream>

static void test0(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test0.chpl",
      "/* comment 1 */\n"
      "for /*comment 2*/ foo /*comment 3*/ do\n"
      "  /* comment 4 */\n"
      "  var a;\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isFor());
  const For* forLoop = mod->stmt(1)->toFor();
  assert(forLoop != nullptr);
  assert(forLoop->index() == nullptr);
  assert(forLoop->iterand() != nullptr);
  assert(forLoop->iterand()->isIdentifier());
  assert(forLoop->numStmts() == 2);
  assert(forLoop->blockStyle() == BlockStyle::IMPLICIT);
  assert(!forLoop->isExpressionLevel());
  assert(!forLoop->isParam());
  assert(forLoop->stmt(0)->isComment());
  assert(forLoop->stmt(1)->isVariable());
}

static void test1(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test1.chpl",
      "/* comment 1 */\n"
      "for foo in /* comment 2 */ bar do\n"
      "  /* comment 3 */\n"
      "  var a;\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isFor());
  const For* forLoop = mod->stmt(1)->toFor();
  assert(forLoop != nullptr);
  assert(forLoop->index() != nullptr);
  assert(forLoop->index()->isVariable());
  assert(forLoop->iterand() != nullptr);
  assert(forLoop->iterand()->isIdentifier());
  assert(forLoop->numStmts() == 2);
  assert(forLoop->blockStyle() == BlockStyle::IMPLICIT);
  assert(!forLoop->isExpressionLevel());
  assert(!forLoop->isParam());
  assert(forLoop->stmt(0)->isComment());
  assert(forLoop->stmt(1)->isVariable());
}

static void test2(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test2.chpl",
      "/* comment 1 */\n"
      "for /* comment 2 */ param foo in bar do\n"
      "  /* comment 3 */\n"
      "  var a;\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isFor());
  const For* forLoop = mod->stmt(1)->toFor();
  assert(forLoop != nullptr);
  assert(forLoop->index() != nullptr);
  assert(forLoop->index()->isVariable());
  assert(forLoop->iterand() != nullptr);
  assert(forLoop->iterand()->isIdentifier());
  assert(forLoop->numStmts() == 2);
  assert(forLoop->blockStyle() == BlockStyle::IMPLICIT);
  assert(!forLoop->isExpressionLevel());
  assert(forLoop->isParam());
  assert(forLoop->stmt(0)->isComment());
  assert(forLoop->stmt(1)->isVariable());
}

static void test3(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test3.chpl",
      "/* comment 1 */\n"
      "for foo in bar /* comment 2 */ {\n"
      "  /* comment 3 */\n"
      "  var a;\n"
      "  /* comment 4 */\n"
      "}\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isFor());
  const For* forLoop = mod->stmt(1)->toFor();
  assert(forLoop != nullptr);
  assert(forLoop->index() != nullptr);
  assert(forLoop->index()->isVariable());
  assert(forLoop->iterand() != nullptr);
  assert(forLoop->iterand()->isIdentifier());
  assert(forLoop->numStmts() == 3);
  assert(forLoop->blockStyle() == BlockStyle::EXPLICIT);
  assert(!forLoop->isExpressionLevel());
  assert(!forLoop->isParam());
  assert(forLoop->stmt(0)->isComment());
  assert(forLoop->stmt(1)->isVariable());
  assert(forLoop->stmt(2)->isComment());
}

static void test4(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test4.chpl",
      "/* comment 1 */\n"
      "for foo in bar do\n"
      "/* comment 2 */\n"
      "{\n"
      "  /* comment 2 */\n"
      "  var a;\n"
      "  /* comment 3 */\n"
      "}\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isFor());
  const For* forLoop = mod->stmt(1)->toFor();
  assert(forLoop != nullptr);
  assert(forLoop->index() != nullptr);
  assert(forLoop->index()->isVariable());
  assert(forLoop->iterand() != nullptr);
  assert(forLoop->iterand()->isIdentifier());
  assert(forLoop->blockStyle() == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK);
  assert(!forLoop->isExpressionLevel());
  assert(!forLoop->isParam());
  assert(forLoop->numStmts() == 3);
  assert(forLoop->stmt(0)->isComment());
  assert(forLoop->stmt(1)->isVariable());
  assert(forLoop->stmt(2)->isComment());
}

static void test5(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test5.chpl",
      "for i in 1..10 with (ref A) { }");
  auto numErrors = 1;
  assert(guard.errors().size() == (size_t) numErrors);
  assert("'with' clauses are not supported on 'for' loops" == guard.error(0)->message());
  assert(guard.realizeErrors() == numErrors);
}

static void test6(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test6.chpl",
      "for i in 1..10 with (re A) { }\n"
      "for i in 1..10 with () { }\n"
      "for i in 1..10 with ref A { }\n");
  auto numErrors = 7;
  assert(guard.errors().size() == (size_t)numErrors);
  assert("invalid intent expression in 'with' clause" == guard.error(1)->message());
  assert("'with' clauses are not supported on 'for' loops" == guard.error(2)->message());
  assert("'with' clause cannot be empty" == guard.error(3)->message());
  assert("'with' clauses are not supported on 'for' loops" == guard.error(4)->message());
  assert("missing parentheses around 'with' clause intents" == guard.error(6)->message());
  // The other errors are from the parser as "near ...".
  // It would be really nice to not have those be emitted at all.
  assert(guard.realizeErrors() == numErrors);
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
