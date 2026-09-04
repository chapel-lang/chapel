/*
 * Copyright 2021-2026 Hewlett Packard Enterprise Development LP
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
#include "chpl/framework/ErrorBase.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/Variable.h"
#include "chpl/uast/Match.h"
#include "chpl/uast/MatchCase.h"
#include "chpl/uast/Module.h"

#include <iostream>

static void test0(Parser* parser) {
  printf("test0\n");
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test0.chpl",
      "/* c1 */\n"
      "union /* c2 */ select /* c3 */ foo /* c4 */ {\n"
      "  when /* c5 */ x do /* c6 */ f1();\n"
      "  when /* c7 */ y /* c8 */ { f2(); }\n"
      "  when /* c9 */ z /* c10 */ do { f3(); }\n"
      "  otherwise /* c11 */ { f4(); }\n"
      "}\n"
      "/* c12 */\n");
  guard.printErrors();
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isMatch());
  assert(mod->stmt(2)->isComment());
  auto match = mod->stmt(1)->toMatch();

  assert(match->expr()->isVariable());
  assert(match->numCaseStmts() == 3);
  assert(match->hasOtherwise());

  int idx = 0;
  for (auto caseStmt : match->caseStmts()) {
    assert(caseStmt == match->caseStmt(idx++));
    assert(caseStmt->isMatchCase());
    assert(!caseStmt->isOtherwise());
  }
  assert(idx == match->numCaseStmts());

  idx = 0;
  for (auto caseStmt : match->allCaseStmts()) {
    if (idx < match->numCaseStmts()) {
      assert(caseStmt == match->caseStmt(idx));
    } else {
      assert(caseStmt == match->otherwiseStmt());
    }
    idx++;
  }
  assert(idx == match->numCaseStmts() + 1);

  for (int i = 0; i < match->numCaseStmts(); i++) {
    auto caseStmt = match->caseStmt(i);
    assert(caseStmt->expr()->isVariable());

    auto bs = caseStmt->blockStyle();
    if (i == 0) assert(bs == BlockStyle::IMPLICIT);
    if (i == 1) assert(bs == BlockStyle::EXPLICIT);
    if (i == 2) assert(bs == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK);

    assert(caseStmt->body()->numStmts() == 1);
    assert(caseStmt->body()->stmt(0)->isFnCall());
  }

  auto otherwise = match->otherwiseStmt();
  assert(otherwise);
  assert(otherwise->isOtherwise());
  assert(otherwise->expr() == nullptr);
  assert(otherwise->blockStyle() == BlockStyle::EXPLICIT);
  assert(otherwise->body()->numStmts() == 1);
  assert(otherwise->body()->stmt(0)->isFnCall());
}

static void test1(Parser* parser) {
  printf("test1\n");
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test1.chpl",
      "union select foo {\n"
      "  when x do f1();\n"
      "  when y { f2(); }\n"
      "}\n");
  guard.printErrors();
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isMatch());
  auto match = mod->stmt(0)->toMatch();
  assert(match->numCaseStmts() == 2);
  assert(!match->hasOtherwise());
  assert(match->otherwiseStmt() == nullptr);

  int numAllCases = 0;
  for (auto caseStmt : match->allCaseStmts()) {
    assert(caseStmt == match->caseStmt(numAllCases++));
  }
  assert(numAllCases == match->numCaseStmts());
}

static void test2(Parser* parser) {
  printf("test2\n");
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test2.chpl",
      "union select foo {\n"
      "  otherwise do { f1(); }\n"
      "}\n");
  guard.printErrors();
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isMatch());
  auto match = mod->stmt(0)->toMatch();
  assert(match->numCaseStmts() == 0);
  assert(match->hasOtherwise());

  auto otherwise = match->otherwiseStmt();
  assert(otherwise);
  assert(otherwise->isOtherwise());
  assert(otherwise->expr() == nullptr);
  assert(otherwise->blockStyle() == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK);
  assert(otherwise->body()->numStmts() == 1);
  assert(otherwise->body()->stmt(0)->isFnCall());

  int numAllCases = 0;
  for (auto caseStmt : match->allCaseStmts()) {
    assert(caseStmt == otherwise);
    numAllCases++;
  }
  assert(numAllCases == 1);
}

static void test3(Parser* parser) {
  printf("test3\n");
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test3.chpl",
      "union select foo {\n"
      "  when 1 do f1();\n"
      "  when foo.bar do f2();\n"
      "  when f() do f3();\n"
      "}\n");
  guard.printErrors();
  assert(guard.numErrors() == 3);
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isMatch());
  for (int i = 0; i < guard.numErrors(); i++) {
    assert(guard.error(i)->type() == ErrorType::UnsupportedMatchExpr);
  }
  guard.clearErrors();
}

static void test4(Parser* parser) {
  printf("test4\n");
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test4.chpl",
      "union select foo {\n"
      "  otherwise do f1();\n"
      "  when x do f2();\n"
      "}\n");
  guard.printErrors();
  assert(guard.numErrors() > 0);
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->stmt(0)->isErroneousExpression());
  guard.clearErrors();
}

static void test5(Parser* parser) {
  printf("test5\n");
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test5.chpl",
      "union select foo {\n"
      "  when x, y do f1();\n"
      "}\n");
  guard.printErrors();
  assert(guard.numErrors() > 0);
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->stmt(0)->isErroneousExpression());
  guard.clearErrors();
}

static void test6(Parser* parser) {
  printf("test6\n");
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test6.chpl",
      "union select foo {\n"
      "  otherwise do f1();\n"
      "  otherwise do f2();\n"
      "}\n");
  guard.printErrors();
  assert(guard.numErrors() > 0);
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->stmt(0)->isErroneousExpression());
  guard.clearErrors();
}

static void test7(Parser* parser) {
  printf("test7\n");
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test7.chpl",
      "union select foo {\n"
      "  when x do f1();\n"
      "  when y do f2();\n"
      "  when x do f3();\n"
      "  when y do f4();\n"
      "}\n");
  guard.printErrors();
  assert(guard.numErrors() == 2);
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isMatch());
  for (int i = 0; i < guard.numErrors(); i++) {
    assert(guard.error(i)->type() == ErrorType::DuplicateMatchExpr);
  }
  guard.clearErrors();
}

static void test8(Parser* parser) {
  printf("test8\n");
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test8.chpl",
      "union select foo {\n"
      "  when x do f1();\n"
      "  when x do f2();\n"
      "  when x do f3();\n"
      "}\n");
  guard.printErrors();
  assert(guard.numErrors() == 2);
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isMatch());
  for (int i = 0; i < guard.numErrors(); i++) {
    assert(guard.error(i)->type() == ErrorType::DuplicateMatchExpr);
  }
  guard.clearErrors();
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
  return 0;
}
