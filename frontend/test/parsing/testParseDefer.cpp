/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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
#include "chpl/uast/Comment.h"
#include "chpl/uast/Defer.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"

static void test0(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test0.chpl",
      "/* comment 1 */\n"
      "defer /* comment 2 */\n"
      "  foo();\n"
      "/* comment 3 */\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isDefer());
  assert(mod->stmt(2)->isComment());
  const Defer* defer = mod->stmt(1)->toDefer();
  assert(defer);
  assert(defer->blockStyle() == BlockStyle::IMPLICIT);
  assert(defer->numStmts() == 2);
  assert(defer->stmt(0)->isComment());
  assert(defer->stmt(0)->toComment()->str() == "/* comment 2 */");
  assert(defer->stmt(1)->isFnCall());

  // Make sure the statements iterator works as expected.
  {
    AstTag stmtList[] = {
      asttags::Comment,
      asttags::FnCall
    };
    auto i = 0;
    for (const auto stmt : defer->stmts()) {
      assert(stmt->tag() == stmtList[i]);
      assert(stmt->tag() == defer->stmt(i++)->tag());
    }
  }
}

static void test1(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test1.chpl",
      "/* comment 1 */\n"
      "defer /* comment 2 */ {\n"
      "  /* comment 3 */\n"
      "  foo();\n"
      "  /* comment 4 */\n"
      "}\n"
      "/* comment 5 */\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isDefer());
  assert(mod->stmt(2)->isComment());
  const Defer* defer = mod->stmt(1)->toDefer();
  assert(defer);
  assert(defer->blockStyle() == BlockStyle::EXPLICIT);
  assert(defer->numStmts() == 3);
  assert(defer->stmt(0)->isComment());
  assert(defer->stmt(1)->isFnCall());
  assert(defer->stmt(2)->isComment());

  // Make sure the statements iterator works as expected.
  {
    AstTag stmtList[] = {
      asttags::Comment,
      asttags::FnCall,
      asttags::Comment
    };
    auto i = 0;
    for (const auto stmt : defer->stmts()) {
      assert(stmt->tag() == stmtList[i]);
      assert(stmt->tag() == defer->stmt(i++)->tag());
    }
  }
}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::createForTopLevelModule(ctx);
  Parser* p = &parser;

  test0(p);
  test1(p);

  return 0;
}
