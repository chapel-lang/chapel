/*
 * Copyright 2022 Hewlett Packard Enterprise Development LP
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
#include "chpl/uast/Catch.h"
#include "chpl/uast/Delete.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Throw.h"
#include "chpl/uast/Try.h"

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
      "/* c1 */\n"
      "var x = try /* c2 */ foo();\n"
      "var y = try! bar();\n"
      "try /* c3 */ foo();\n"
      "try! bar();\n"
      "throw baz();\n"
      "/* c4 */\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 7);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isVariable());
  assert(mod->stmt(2)->isVariable());
  assert(mod->stmt(3)->isTry());
  assert(mod->stmt(4)->isTry());
  assert(mod->stmt(5)->isThrow());
  assert(mod->stmt(6)->isComment());
  {
    auto var = mod->stmt(1)->toVariable();
    assert(var->initExpression());
    auto tryExpr = var->initExpression()->toTry();
    assert(tryExpr);
    assert(!tryExpr->isTryBang());
    assert(tryExpr->isExpressionLevel());
    assert(tryExpr->numHandlers() == 0);
    assert(tryExpr->numStmts() == 1);
    assert(tryExpr->stmt(0)->isFnCall());
  }
  {
    auto var = mod->stmt(2)->toVariable();
    assert(var->initExpression());
    auto tryExpr = var->initExpression()->toTry();
    assert(tryExpr);
    assert(tryExpr->isTryBang());
    assert(tryExpr->isExpressionLevel());
    assert(tryExpr->numHandlers() == 0);
    assert(tryExpr->numStmts() == 1);
    assert(tryExpr->stmt(0)->isFnCall());
  }
  {
    auto tryExpr = mod->stmt(3)->toTry();
    assert(tryExpr);
    assert(!tryExpr->isTryBang());
    assert(!tryExpr->isExpressionLevel());
    assert(tryExpr->numHandlers() == 0);
    assert(tryExpr->numStmts() == 1);
    assert(tryExpr->stmt(0)->isFnCall());
  }
  {
    auto tryExpr = mod->stmt(4)->toTry();
    assert(tryExpr);
    assert(tryExpr->isTryBang());
    assert(!tryExpr->isExpressionLevel());
    assert(tryExpr->numHandlers() == 0);
    assert(tryExpr->numStmts() == 1);
    assert(tryExpr->stmt(0)->isFnCall());
  }
  {
    auto throwExpr = mod->stmt(5)->toThrow();
    assert(throwExpr);
    assert(throwExpr->errorExpression());
    assert(throwExpr->errorExpression()->isFnCall());
  }
}

static std::string constructTryCatchTest(bool isTryBang, bool hasError,
                                         bool hasErrorType) {
  std::string ret;
  ret += "/* c1 */\n";
  ret += isTryBang ? "try!" : "try";
  ret += " { foo(); bar(); } catch ";

  if (hasError && hasErrorType) {
    ret += "e : ErrorType";
  } else if (hasError) {
    ret += "e ";
  } else {
    assert(!hasErrorType);
  }

  ret += "{ handleError(); }\n";
  ret += "/* c2 */\n";

  return ret;
}

static void tryCatchTest(Parser* parser, const char* testName,
                         bool isTryBang,
                         bool hasError,
                         bool hasErrorType) {
  auto test = constructTryCatchTest(isTryBang, hasError, hasErrorType);
  auto parseResult = parser->parseString(testName, test.c_str());
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isTry());
  // TODO: This last comment is getting cleared out...
  assert(mod->stmt(2)->isComment());
  const Try* tryStmt = mod->stmt(1)->toTry();
  assert(!tryStmt->isExpressionLevel());
  assert(tryStmt->isTryBang() == isTryBang);
  assert(tryStmt->numStmts() == 2);
  assert(tryStmt->stmt(0)->isFnCall());
  assert(tryStmt->stmt(1)->isFnCall());
  assert(tryStmt->numHandlers() == 1);
  const Catch* handler = tryStmt->handler(0);
  assert(handler);
  if (hasError) {
    assert(handler->error());
    if (hasErrorType) {
      auto te = handler->error()->typeExpression();
      assert(te && te->isIdentifier());
    }
  }

  // Check try body iterator.
  int count = 0;
  for (auto stmt : tryStmt->stmts()) {
    assert(stmt->isFnCall());
    count += 1;
  }
  assert(count == 2);

  // Check catch body iterator.
  count = 0;
  for (auto stmt : handler->stmts()) {
    assert(stmt->isFnCall());
    count += 1;
  }
  assert(count == 1);
}

static void test1(Parser* parser) {
  tryCatchTest(parser, "testTryCatch0.chpl", 0, 0, 0);
  tryCatchTest(parser, "testTryCatch1.chpl", 0, 1, 0);
  tryCatchTest(parser, "testTryCatch2.chpl", 0, 1, 1);
  tryCatchTest(parser, "testTryCatch3.chpl", 1, 0, 0);
  tryCatchTest(parser, "testTryCatch4.chpl", 1, 1, 0);
  tryCatchTest(parser, "testTryCatch5.chpl", 1, 1, 1);
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
      "/* c1 */\n"
      "try {\n"
      "  throw fooError();\n"
      "} catch e1: ErrorType1 {\n"
      "  writeln('E1');\n"
      "} catch e2: ErrorType2 {\n"
      "  writeln('E2');\n"
      "} catch {\n"
      "  halt('Unknown error');\n"
      "}\n"
      "/* c2 */\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isTry());
  assert(mod->stmt(2)->isComment());
  auto tryStmt = mod->stmt(1)->toTry();
  assert(tryStmt->numStmts() == 1 && tryStmt->stmt(0)->isThrow());
  assert(tryStmt->numHandlers() == 3);
  {
    auto catchBlock = tryStmt->handler(0);
    assert(catchBlock->error() && catchBlock->error()->typeExpression());
    assert(catchBlock->numStmts() == 1 && catchBlock->stmt(0)->isFnCall());
  }
  {
    auto catchBlock = tryStmt->handler(1);
    assert(catchBlock->error() && catchBlock->error()->typeExpression());
    assert(catchBlock->numStmts() == 1 && catchBlock->stmt(0)->isFnCall());
  }
  {
    auto catchBlock = tryStmt->handler(2);
    assert(!catchBlock->error());
    assert(catchBlock->numStmts() == 1 && catchBlock->stmt(0)->isFnCall());
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
