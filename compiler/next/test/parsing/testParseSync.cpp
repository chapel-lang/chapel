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
#include "chpl/uast/Begin.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Sync.h"
#include "chpl/queries/Context.h"


// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <array>
#include <cassert>

using namespace chpl;
using namespace uast;
using namespace parsing;

static void test0(Parser* parser) {
  auto parseResult = parser->parseString("test0.chpl",
      "/* comment 1 */\n"
      "sync /* comment 2 */\n"
      "  begin foo();\n"
      "/* comment 3 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSync());
  assert(mod->stmt(2)->isComment());
  const Sync* sync = mod->stmt(1)->toSync();
  assert(sync);
  assert(sync->blockStyle() == BlockStyle::IMPLICIT);
  assert(sync->numStmts() == 2);
  assert(sync->stmt(0)->isComment());
  assert(sync->stmt(0)->toComment()->str() == "/* comment 2 */");
  const Begin* begin = sync->stmt(1)->toBegin();
  assert(begin);
  assert(begin->numStmts() == 1);
  assert(begin->stmt(0)->isFnCall());
  assert(begin->blockStyle() == BlockStyle::IMPLICIT);
  
  // Make sure the statements iterator works as expected.
  {
    std::array<ASTTag, 2> stmtList = {
      asttags::Comment,
      asttags::Begin
    };
    auto i = 0;
    for (const auto stmt : sync->stmts()) {
      assert(stmt->tag() == stmtList[i]);
      assert(stmt->tag() == sync->stmt(i++)->tag());
    }
  }
}


static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
      "/* comment 1 */\n"
      "sync /* comment 2 */ {\n"
      "  /* comment 3 */\n"
      "  begin foo();\n"
      "  /* comment 4 */\n"
      "}\n"
      "/* comment 5 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSync());
  assert(mod->stmt(2)->isComment());
  const Sync* sync = mod->stmt(1)->toSync();
  assert(sync);
  assert(sync->blockStyle() == BlockStyle::EXPLICIT);
  assert(sync->numStmts() == 3);
  assert(sync->stmt(0)->isComment());
  const Begin* begin = sync->stmt(1)->toBegin();
  assert(begin);
  assert(begin->numStmts() == 1);
  assert(begin->blockStyle() == BlockStyle::IMPLICIT);
  assert(begin->stmt(0)->isFnCall());
  assert(sync->stmt(2)->isComment());

  // Make sure the statements iterator works as expected.
  {
    std::array<ASTTag, 3> stmtList = {
      asttags::Comment,
      asttags::Begin,
      asttags::Comment
    };
    auto i = 0;
    for (const auto stmt : sync->stmts()) {
      assert(stmt->tag() == stmtList[i]);
      assert(stmt->tag() == sync->stmt(i++)->tag());
    }
  }
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
      "/* comment 1 */\n"
      "sync /* comment 2 */ {\n"
      "  /* comment 3 */\n"
      "  begin foo();\n"
      "  begin bar();\n"
      "  /* comment 4 */\n"
      "}\n"
      "/* comment 5 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSync());
  assert(mod->stmt(2)->isComment());
  const Sync* sync = mod->stmt(1)->toSync();
  assert(sync);
  assert(sync->blockStyle() == BlockStyle::EXPLICIT);
  assert(sync->numStmts() == 4);
  assert(sync->stmt(0)->isComment());
  const Begin* begin = sync->stmt(1)->toBegin();
  assert(begin);
  assert(begin->numStmts() == 1);
  assert(begin->blockStyle() == BlockStyle::IMPLICIT);
  assert(begin->stmt(0)->isFnCall());
  const Begin* beginBar = sync->stmt(2)->toBegin();
  assert(beginBar);
  assert(beginBar->numStmts() == 1);
  assert(beginBar->blockStyle() == BlockStyle::IMPLICIT);
  assert(beginBar->stmt(0)->isFnCall());
  assert(sync->stmt(3)->isComment());

  // Make sure the statements iterator works as expected.
  {
    std::array<ASTTag, 4> stmtList = {
      asttags::Comment,
      asttags::Begin,
      asttags::Begin,
      asttags::Comment
    };
    auto i = 0;
    for (const auto stmt : sync->stmts()) {
      assert(stmt->tag() == stmtList[i]);
      assert(stmt->tag() == sync->stmt(i++)->tag());
    }
  }
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
