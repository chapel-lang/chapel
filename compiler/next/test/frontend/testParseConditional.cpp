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
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/Conditional.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Forall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/OpCall.h"
#include "chpl/uast/TaskVar.h"
#include "chpl/uast/WithClause.h"
#include "chpl/uast/Zip.h"
#include "chpl/frontend/Parser.h"
#include "chpl/queries/Context.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <array>
#include <cassert>
#include <iostream>

using namespace chpl;
using namespace uast;
using namespace frontend;

static void test0(Parser* parser) {
  auto parseResult = parser->parseString("test0.chpl",
      "/* c1 */\n"
      "if /* c2 */ foo /* c3 */ then\n"
      "  /* c4 */\n"
      "  bar();\n"
      "  /* c5 */\n"
      "else\n"
      "  /* c6 */\n"
      "  baz();\n"
      "/* c7 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isConditional());
  assert(mod->stmt(2)->isComment());
  const Conditional* cond = mod->stmt(1)->toConditional();
  assert(cond != nullptr);
  assert(cond->condition());
  assert(cond->condition()->isIdentifier());
  assert(!cond->condition()->toIdentifier()->name().compare("foo"));
  assert(cond->thenBlockStyle() == BlockStyle::IMPLICIT);
  assert(cond->numThenStmts() == 2);
  assert(cond->thenStmt(0)->isComment());
  assert(cond->thenStmt(0)->toComment()->str() == "/* c4 */");
  assert(cond->thenStmt(1)->isFnCall());
  assert(cond->hasElseBlock());
  assert(cond->elseBlockStyle() == BlockStyle::IMPLICIT);
  assert(cond->numElseStmts() == 2);
  assert(cond->elseStmt(0)->isComment());
  assert(cond->elseStmt(0)->toComment()->str() == "/* c6 */");
  assert(cond->elseStmt(1)->isFnCall());
  assert(!cond->isExpressionLevel());

  // Make sure the then iterator works as expected.
  {
    std::array<ASTTag, 3> thenStmtList = {
      asttags::Comment,
      asttags::FnCall,
      asttags::Comment
    };
    auto i = 0;
    for (const auto stmt : cond->thenStmts()) {
      assert(stmt->tag() == thenStmtList[i]);
      assert(stmt->tag() == cond->thenStmt(i++)->tag());
    }
  }

  // Make sure the else iterator works as expected.
  {
    std::array<ASTTag, 2> elseStmtList = {
      asttags::Comment,
      asttags::FnCall
    };
    auto i = 0;
    for (const auto stmt : cond->thenStmts()) {
      assert(stmt->tag() == elseStmtList[i]);
      assert(stmt->tag() == cond->elseStmt(i++)->tag());
    }
  }
}

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
      "/* c1 */\n"
      "if /* c2 */ foo() /* c3 */ {\n"
      "  /* c4 */\n"
      "  bar();\n"
      "  /* c5 */\n"
      "} /* c6 */ else /* c7 */ {\n"
      "  /* c8 */\n"
      "  baz();\n"
      "  /* c9 */\n"
      "}\n"
      "/* c10 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isConditional());
  assert(mod->stmt(2)->isComment());
  const Conditional* cond = mod->stmt(1)->toConditional();
  assert(cond != nullptr);
  assert(cond->condition());
  assert(cond->condition()->isFnCall());
  assert(cond->thenBlockStyle() == BlockStyle::EXPLICIT);
  assert(cond->numThenStmts() == 3);
  assert(cond->thenStmt(0)->isComment());
  assert(cond->thenStmt(0)->toComment()->str() == "/* c4 */");
  assert(cond->thenStmt(1)->isFnCall());
  assert(cond->thenStmt(2)->isComment());
  assert(cond->thenStmt(2)->toComment()->str() == "/* c5 */");
  assert(cond->hasElseBlock());
  assert(cond->elseBlockStyle() == BlockStyle::EXPLICIT);
  assert(cond->numElseStmts() == 3);
  assert(cond->elseStmt(0)->isComment());
  assert(cond->elseStmt(0)->toComment()->str() == "/* c8 */");
  assert(cond->elseStmt(1)->isFnCall());
  assert(cond->elseStmt(2)->isComment());
  assert(cond->elseStmt(2)->toComment()->str() == "/* c9 */");
  assert(!cond->isExpressionLevel());

  // Make sure the then iterator works as expected.
  {
    std::array<ASTTag, 3> thenStmtList = {
      asttags::Comment,
      asttags::FnCall,
      asttags::Comment,
    };
    auto i = 0;
    for (const auto stmt : cond->thenStmts()) {
      assert(stmt->tag() == thenStmtList[i]);
      assert(stmt->tag() == cond->thenStmt(i++)->tag());
    }
  }

  // Make sure the else iterator works as expected.
  {
    std::array<ASTTag, 3> elseStmtList = {
      asttags::Comment,
      asttags::FnCall,
      asttags::Comment,
    };
    auto i = 0;
    for (const auto stmt : cond->elseStmts()) {
      assert(stmt->tag() == elseStmtList[i]);
      assert(stmt->tag() == cond->elseStmt(i++)->tag());
    }
  }
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
      "/* c1 */\n"
      "if /* c2 */ foo /* c3 */ then {\n"
      "  /* c4 */\n"
      "  bar();\n"
      "  baz();\n"
      "  /* c5 */\n"
      "} /* c6 */ else /* c7 */ ding();\n"
      "/* c8 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isConditional());
  assert(mod->stmt(2)->isComment());
  const Conditional* cond = mod->stmt(1)->toConditional();
  assert(cond != nullptr);
  assert(cond->condition());
  assert(cond->condition()->isIdentifier());
  assert(cond->thenBlockStyle() == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK);
  assert(cond->numThenStmts() == 4);
  assert(cond->thenStmt(0)->isComment());
  assert(cond->thenStmt(1)->isFnCall());
  assert(cond->thenStmt(2)->isFnCall());
  assert(cond->thenStmt(3)->isComment());
  assert(cond->hasElseBlock());
  assert(cond->elseBlockStyle() == BlockStyle::IMPLICIT);
  assert(cond->numElseStmts() == 2);
  assert(cond->elseStmt(0)->isComment());
  assert(cond->elseStmt(1)->isFnCall());
  assert(!cond->isExpressionLevel());
}

static void test3(Parser* parser) {
  auto parseResult = parser->parseString("test3.chpl",
      "/* c1 */\n"
      "if /* c2 */ foo /* c3 */ then\n"
      "  /* c4 */\n"
      "  bar();\n"
      "/* c5 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isConditional());
  assert(mod->stmt(2)->isComment());
  const Conditional* cond = mod->stmt(1)->toConditional();
  assert(cond != nullptr);
  assert(cond->condition());
  assert(cond->condition()->isIdentifier());
  assert(cond->thenBlockStyle() == BlockStyle::IMPLICIT);
  assert(cond->numThenStmts() == 2);
  assert(cond->thenStmt(0)->isComment());
  assert(cond->thenStmt(0)->toComment()->str() == "/* c4 */");
  assert(cond->thenStmt(1)->isFnCall());
  assert(!cond->hasElseBlock());
  assert(cond->numElseStmts() == 0);
  assert(!cond->isExpressionLevel());

  // Make sure the then iterator works as expected.
  {
    std::array<ASTTag, 2> thenStmtList = {
      asttags::Comment,
      asttags::FnCall
    };
    auto i = 0;
    for (const auto stmt : cond->thenStmts()) {
      assert(stmt->tag() == thenStmtList[i]);
      assert(stmt->tag() == cond->thenStmt(i++)->tag());
    }
  }
}

static void test4(Parser* parser) {
  auto parseResult = parser->parseString("test4.chpl",
      "/* c1 */\n"
      "if foo += bar() then\n"
      "  baz();\n"
      "/* c2 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isConditional());
  assert(mod->stmt(2)->isComment());
  const Conditional* cond = mod->stmt(1)->toConditional();
  assert(cond != nullptr);
  assert(cond->condition());
  assert(cond->condition()->isOpCall());
  assert(!cond->condition()->toOpCall()->op().compare("+="));
  assert(cond->thenBlockStyle() == BlockStyle::IMPLICIT);
  assert(cond->numThenStmts() == 1);
  assert(cond->thenStmt(0)->isFnCall());
  assert(!cond->hasElseBlock());
  assert(cond->numElseStmts() == 0);
  assert(!cond->isExpressionLevel());
}

static void test5(Parser* parser) {
  auto parseResult = parser->parseString("test5.chpl",
      "/* c1 */\n"
      "if var foo = bar() then\n"
      "  baz();\n"
      "/* c2 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isConditional());
  assert(mod->stmt(2)->isComment());
  const Conditional* cond = mod->stmt(1)->toConditional();
  assert(cond != nullptr);
  assert(cond->condition());
  const Variable* condVar = cond->condition()->toVariable();
  assert(condVar);
  assert(!condVar->name().compare("foo"));
  assert(!condVar->typeExpression());
  assert(condVar->initExpression());
  assert(condVar->initExpression()->isFnCall());
  assert(cond->thenBlockStyle() == BlockStyle::IMPLICIT);
  assert(cond->numThenStmts() == 1);
  assert(cond->thenStmt(0)->isFnCall());
  assert(!cond->hasElseBlock());
  assert(cond->numElseStmts() == 0);
  assert(!cond->isExpressionLevel());
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
