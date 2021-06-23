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
#include "chpl/uast/Comment.h"
#include "chpl/uast/For.h"
#include "chpl/uast/DoWhile.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
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
      "do {\n"
      "  /* comment 2 */\n"
      "  bar();\n"
      "  /* comment 3 */\n"
      "} while /* comment 4 */ foo();\n"
      "/* comment 5 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isDoWhile());
  assert(mod->stmt(2)->isComment());
  const DoWhile* doWhile = mod->stmt(1)->toDoWhile();
  assert(doWhile);
  assert(doWhile->blockStyle() == BlockStyle::EXPLICIT);
  assert(doWhile->condition());
  assert(doWhile->condition()->isFnCall());
  assert(doWhile->numStmts() == 3);
  assert(doWhile->stmt(0)->isComment());
  assert(doWhile->stmt(1)->isFnCall());
  assert(doWhile->stmt(2)->isComment());
}

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
      "/* comment 1 */\n"
      "do /* comment 2 */\n"
      "  for x in thing do\n"
      "    /* comment 3 */\n"
      "    bar();\n"
      "  /* comment 4 */\n"
      "while /* comment 5 */ condition;\n"
      "/* comment 6 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isDoWhile());
  assert(mod->stmt(2)->isComment());
  const DoWhile* doWhile = mod->stmt(1)->toDoWhile();
  assert(doWhile);
  assert(doWhile->blockStyle() == BlockStyle::IMPLICIT);
  assert(doWhile->condition());
  assert(doWhile->condition()->isIdentifier());
  // Comment between 'while' and condition is discarded.
  assert(doWhile->numStmts() == 3);
  assert(doWhile->stmt(0)->isComment());
  assert(doWhile->stmt(0)->toComment()->str() == "/* comment 2 */");
  assert(doWhile->stmt(1)->isFor());
  assert(doWhile->stmt(2)->isComment());
  const For* forLoop = doWhile->stmt(1)->toFor();
  assert(forLoop);
  assert(forLoop->blockStyle() == BlockStyle::IMPLICIT);
  assert(forLoop->index());
  assert(forLoop->index()->isVariable());
  assert(forLoop->iterand() && forLoop->iterand()->isIdentifier());
  assert(forLoop->numStmts() == 2);
  assert(forLoop->stmt(0)->isComment());
  assert(forLoop->stmt(1)->isFnCall());
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
      "/* comment 1 */\n"
      "do\n"
      "  /* comment 2 */\n"
      "  do /* comment 3 */ {\n"
      "    /* comment 4 */\n"
      "    foo();\n"
      "    /* comment 5 */\n"
      "  } /* comment 6 */ while condition1;\n"
      "  /* comment 7 */\n"
      "while condition2;\n"
      "/* comment 8 */\n");

  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isDoWhile());
  assert(mod->stmt(2)->isComment());
  const DoWhile* doWhile = mod->stmt(1)->toDoWhile();
  assert(doWhile);
  assert(doWhile->blockStyle() == BlockStyle::IMPLICIT);
  assert(doWhile->condition());
  assert(doWhile->condition()->isIdentifier());
  assert(doWhile->numStmts() == 3);
  assert(doWhile->stmt(0)->isComment());
  assert(doWhile->stmt(1)->isDoWhile());
  assert(doWhile->stmt(2)->isComment());
  const DoWhile* doWhileInner = doWhile->stmt(1)->toDoWhile();
  assert(doWhileInner);
  assert(doWhileInner->blockStyle() == BlockStyle::EXPLICIT);
  assert(doWhileInner->condition()->isIdentifier());
  assert(doWhileInner->numStmts() == 3);
  assert(doWhileInner->stmt(0)->isComment());
  assert(doWhileInner->stmt(1)->isFnCall());
  assert(doWhileInner->stmt(2)->isComment());
}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  test0(p);
  test1(p);
  test2(p);

  return 0;
}
