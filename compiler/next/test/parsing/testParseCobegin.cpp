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
#include "chpl/queries/Context.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/Cobegin.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/TaskVar.h"

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
      "cobegin /* comment 2 */\n"
      "with /* comment 3 */ (ref a, var b = foo()) /* comment 4 */ {\n"
      "  /* comment 5 */\n"
      "  writeln(a);\n"
      "  /* comment 6 */\n"
      "}\n"
      "/* comment 7 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isCobegin());
  assert(mod->stmt(2)->isComment());
  const Cobegin* cobegin = mod->stmt(1)->toCobegin();
  assert(cobegin != nullptr);
  const WithClause* withClause = cobegin->withClause();
  assert(withClause);
  assert(withClause->numExprs() == 2);
  auto tv1 = withClause->expr(0)->toTaskVar();
  assert(tv1 && tv1->intent() == TaskVar::REF);
  assert(!tv1->typeExpression() && !tv1->initExpression());
  auto tv2 = withClause->expr(1)->toTaskVar();
  assert(tv2 && tv2->intent() == TaskVar::VAR);
  assert(!tv2->typeExpression() && tv2->initExpression());
  assert(cobegin->numTaskBodies() == 3);
  assert(cobegin->taskBody(0)->isComment());
  assert(cobegin->taskBody(0)->toComment()->str() == "/* comment 5 */");
  assert(cobegin->taskBody(1)->isFnCall());
  assert(cobegin->taskBody(2)->isComment());
  // Make sure the task bodies iterator works as expected.
  {
    // Include comments even if they have no meaning.
    std::array<ASTTag, 3> taskBodyList = {
      asttags::Comment,
      asttags::FnCall,
      asttags::Comment
    };
    auto i = 0;
    for (const auto taskBody : cobegin->taskBodies()) {
      assert(taskBody->tag() == taskBodyList[i]);
      assert(taskBody->tag() == cobegin->taskBody(i++)->tag());
    }
  }
}

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
      "/* comment 1 */\n"
      "cobegin /* comment 2 */ {\n"
      "  /* comment 5 */\n"
      "  writeln(a);\n"
      "  /* comment 6 */\n"
      "}\n"
      "/* comment 7 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isCobegin());
  assert(mod->stmt(2)->isComment());
  const Cobegin* cobegin = mod->stmt(1)->toCobegin();
  assert(cobegin != nullptr);
  assert(!cobegin->withClause());
  assert(cobegin->numTaskBodies() == 3);
  assert(cobegin->taskBody(0)->isComment());
  assert(cobegin->taskBody(0)->toComment()->str() == "/* comment 5 */");
  assert(cobegin->taskBody(1)->isFnCall());
  assert(cobegin->taskBody(2)->isComment());
}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  test0(p);
  test1(p);

  return 0;
}
