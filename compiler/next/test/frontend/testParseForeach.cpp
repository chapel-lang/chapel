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

#include "chpl/uast/Block.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Foreach.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/TaskVar.h"
#include "chpl/uast/WithClause.h"
#include "chpl/uast/Zip.h"
#include "chpl/frontend/Parser.h"
#include "chpl/queries/Context.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <iostream>

using namespace chpl;
using namespace uast;
using namespace frontend;

static void test0(Parser* parser) {
  auto parseResult = parser->parseString("test0.chpl",
      "/* comment 1 */\n"
      "foreach x in foo do\n"
      "  /* comment 2 */\n"
      "  foo();\n"
      "/* comment 3 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isForeach());
  assert(mod->stmt(2)->isComment());
  const Foreach* foreach = mod->stmt(1)->toForeach();
  assert(foreach != nullptr);
  assert(foreach->index() != nullptr);
  assert(foreach->index()->isVariable());
  assert(foreach->iterand() != nullptr);
  assert(foreach->iterand()->isIdentifier());
  assert(foreach->withClause() == nullptr);
  assert(foreach->numStmts() == 2);
  assert(foreach->blockStyle() == BlockStyle::IMPLICIT);
  assert(foreach->stmt(0)->isComment());
  assert(foreach->stmt(1)->isFnCall());
}

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
      "/* comment 1 */\n"
      "foreach x in foo with (ref thing) {\n"
      "  /* comment 2 */\n"
      "  foo();\n"
      "  /* comment 3 */\n"
      "}\n"
      "/* comment 4 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isForeach());
  assert(mod->stmt(2)->isComment());
  const Foreach* foreach = mod->stmt(1)->toForeach();
  assert(foreach != nullptr);
  assert(foreach->index() != nullptr);
  assert(foreach->index()->isVariable());
  assert(foreach->iterand() != nullptr);
  assert(foreach->iterand()->isIdentifier());
  const WithClause* withClause = foreach->withClause();
  assert(withClause);
  assert(withClause->numExprs() == 1);
  const TaskVar* taskVar = withClause->expr(0)->toTaskVar();
  assert(taskVar);
  assert(!taskVar->typeExpression());
  assert(!taskVar->initExpression());
  assert(taskVar->intent() == TaskVar::REF);
  assert(foreach->blockStyle() == BlockStyle::EXPLICIT);
  assert(foreach->numStmts() == 3);
  assert(foreach->stmt(0)->isComment());
  assert(foreach->stmt(1)->isFnCall());
  assert(foreach->stmt(2)->isComment());
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
      "/* comment 1 */\n"
      "foreach x in zip(a, b) {\n"
      "  foo();\n"
      "}\n"
      "/* comment 4 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isForeach());
  assert(mod->stmt(2)->isComment());
  const Foreach* foreach = mod->stmt(1)->toForeach();
  assert(foreach != nullptr);
  assert(foreach->index() != nullptr);
  assert(foreach->index()->isVariable());
  assert(foreach->iterand() != nullptr);
  const Zip* zip = foreach->iterand()->toZip();
  assert(zip);
  assert(zip->numActuals() == 2);
  assert(zip->actual(0)->isIdentifier());
  assert(zip->actual(1)->isIdentifier());
  assert(foreach->blockStyle() == BlockStyle::EXPLICIT);
  assert(foreach->numStmts() == 1);
  assert(foreach->stmt(0)->isFnCall());
}

static void test3(Parser* parser) {
  auto parseResult = parser->parseString("test3.chpl",
      "/* comment 1 */\n"
      "foreach x in zip(a, b, foo()) with (const ref thing1, in thing2=d) {\n"
      "  writeln(thing1);\n"
      "  thing2 = thing3;\n"
      "}\n"
      "/* comment 4 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isForeach());
  assert(mod->stmt(2)->isComment());
  const Foreach* foreach = mod->stmt(1)->toForeach();
  assert(foreach != nullptr);
  assert(foreach->index() != nullptr);
  assert(foreach->index()->isVariable());
  assert(foreach->iterand() != nullptr);
  const Zip* zip = foreach->iterand()->toZip();
  assert(zip);
  assert(zip->numActuals() == 3);
  assert(zip->actual(0)->isIdentifier());
  assert(zip->actual(1)->isIdentifier());
  assert(zip->actual(2)->isFnCall());
  const WithClause* withClause = foreach->withClause();
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
  assert(foreach->blockStyle() == BlockStyle::EXPLICIT);
  assert(foreach->numStmts() == 2);
  assert(foreach->stmt(0)->isFnCall());
  assert(foreach->stmt(1)->isOpCall());
}

static void test4(Parser* parser) {
  auto parseResult = parser->parseString("test4.chpl",
      "/* comment 1 */\n"
      "foreach foo() do\n"
      "  /* comment 2 */\n"
      "  bar();\n"
      "/* comment 3 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isForeach());
  assert(mod->stmt(2)->isComment());
  const Foreach* foreach = mod->stmt(1)->toForeach();
  assert(foreach != nullptr);
  assert(foreach->index() == nullptr);
  assert(foreach->iterand() != nullptr);
  assert(foreach->iterand()->isFnCall());
  assert(foreach->withClause() == nullptr);
  assert(foreach->numStmts() == 2);
  assert(foreach->blockStyle() == BlockStyle::IMPLICIT);
  assert(foreach->stmt(0)->isComment());
  assert(foreach->stmt(1)->isFnCall());
}

static void test5(Parser* parser) {
  auto parseResult = parser->parseString("test5.chpl",
      "/* comment 1 */\n"
      "foreach zip(a, b) with (var r=thing1) do {\n"
      "  writeln(r);\n"
      "}\n"
      "/* comment 4 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isForeach());
  assert(mod->stmt(2)->isComment());
  const Foreach* foreach = mod->stmt(1)->toForeach();
  assert(foreach != nullptr);
  assert(foreach->index() == nullptr);
  assert(foreach->iterand() != nullptr);
  const Zip* zip = foreach->iterand()->toZip();
  assert(zip);
  assert(zip->numActuals() == 2);
  assert(zip->actual(0)->isIdentifier());
  assert(zip->actual(1)->isIdentifier());
  const WithClause* withClause = foreach->withClause();
  assert(withClause);
  assert(withClause->numExprs() == 1);
  const TaskVar* taskVar0 = withClause->expr(0)->toTaskVar();
  assert(taskVar0);
  assert(!taskVar0->typeExpression());
  assert(taskVar0->initExpression());
  assert(taskVar0->intent() == TaskVar::VAR);
  assert(foreach->blockStyle() == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK);
  assert(foreach->numStmts() == 1);
  assert(foreach->stmt(0)->isFnCall());
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

  return 0;
}
