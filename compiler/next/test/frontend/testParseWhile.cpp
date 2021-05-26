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
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/While.h"
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
      "while foo() do\n"
      "  /* comment 2 */\n"
      "  bar();\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isWhile());
  const While* whileLoop = mod->stmt(1)->toWhile();
  assert(whileLoop != nullptr);
  assert(whileLoop->usesImplicitBlock());
  assert(whileLoop->condition() != nullptr);
  assert(whileLoop->condition()->isFnCall());
  assert(whileLoop->numStmts() == 2);
  assert(whileLoop->stmt(0)->isComment());
  assert(whileLoop->stmt(1)->isFnCall());
}

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
      "/* comment 1 */\n"
      "while foo() {\n"
      "  /* comment 2 */\n"
      "  bar();\n"
      "}\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isWhile());
  const While* whileLoop = mod->stmt(1)->toWhile();
  assert(whileLoop != nullptr);
  assert(!whileLoop->usesImplicitBlock());
  assert(whileLoop->condition() != nullptr);
  assert(whileLoop->condition()->isFnCall());
  assert(whileLoop->numStmts() == 2);
  assert(whileLoop->stmt(0)->isComment());
  assert(whileLoop->stmt(1)->isFnCall());
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
      "/* comment 1 */\n"
      "while foo() do {\n"
      "  /* comment 2 */\n"
      "  bar();\n"
      "}\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isWhile());
  const While* whileLoop = mod->stmt(1)->toWhile();
  assert(whileLoop != nullptr);
  assert(whileLoop->usesImplicitBlock());
  assert(whileLoop->condition() != nullptr);
  assert(whileLoop->condition()->isFnCall());
  assert(whileLoop->numStmts() == 1);
  assert(whileLoop->stmt(0)->isBlock());
}

static void test3(Parser* parser) {
  auto parseResult = parser->parseString("test3.chpl",
      "/* comment 1 */\n"
      "while condition1 {\n"
      "  /* comment 2 */\n"
      "  while condition2 do\n"
      "    /* comment 3 */\n"
      "   bar();\n"
      "  /* comment 4 */\n"
      "}\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isWhile());
  const While* whileLoop = mod->stmt(1)->toWhile();
  assert(whileLoop != nullptr);
  assert(!whileLoop->usesImplicitBlock());
  assert(whileLoop->condition() != nullptr);
  assert(whileLoop->condition()->isIdentifier());
  assert(whileLoop->numStmts() == 3);
  assert(whileLoop->stmt(0)->isComment());
  assert(whileLoop->stmt(1)->isWhile());
  assert(whileLoop->stmt(2)->isComment());
  const While* whileLoopInner = whileLoop->stmt(1)->toWhile();
  assert(whileLoopInner);
  assert(whileLoopInner->usesImplicitBlock());
  assert(whileLoopInner->numStmts() == 2);
  assert(whileLoopInner->stmt(0)->isComment());
  assert(whileLoopInner->stmt(1)->isFnCall());
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

  return 0;
}
