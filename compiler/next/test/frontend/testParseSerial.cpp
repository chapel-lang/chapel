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
#include "chpl/uast/ModuleDecl.h"
#include "chpl/uast/Serial.h"
#include "chpl/frontend/Parser.h"
#include "chpl/queries/Context.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace uast;
using namespace frontend;

static void test0(Parser* parser) {
  auto parseResult = parser->parseString("test0.chpl",
      "/* comment 1 */\n"
      "serial do var a;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModuleDecl());
  auto mod = parseResult.topLevelExpressions[0]->toModuleDecl()->module();
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSerial());
  const Serial* serial = mod->stmt(1)->toSerial();
  assert(serial != nullptr);
  assert(serial->condition() == nullptr);
  assert(serial->numStmts() == 1);
  assert(serial->usesDo());
  assert(serial->stmt(0)->isVariableDecl());
}

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
      "/* comment 1 */\n"
      "serial foo do var a;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModuleDecl());
  auto mod = parseResult.topLevelExpressions[0]->toModuleDecl()->module();
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSerial());
  const Serial* serial = mod->stmt(1)->toSerial();
  assert(serial != nullptr);
  assert(serial->condition() != nullptr);
  assert(serial->condition()->isIdentifier());
  assert(serial->numStmts() == 1);
  assert(serial->usesDo());
  assert(serial->stmt(0)->isVariableDecl());
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
      "/* comment 1 */\n"
      "serial { var a; }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModuleDecl());
  auto mod = parseResult.topLevelExpressions[0]->toModuleDecl()->module();
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSerial());
  const Serial* serial = mod->stmt(1)->toSerial();
  assert(serial != nullptr);
  assert(serial->condition() == nullptr);
  assert(serial->numStmts() == 1);
  assert(!serial->usesDo());
  assert(serial->stmt(0)->isVariableDecl());
}

static void test3(Parser* parser) {
  auto parseResult = parser->parseString("test3.chpl",
      "/* comment 1 */\n"
      "serial foo { var a; }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModuleDecl());
  auto mod = parseResult.topLevelExpressions[0]->toModuleDecl()->module();
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSerial());
  const Serial* serial = mod->stmt(1)->toSerial();
  assert(serial != nullptr);
  assert(serial->condition() != nullptr);
  assert(serial->condition()->isIdentifier());
  assert(serial->numStmts() == 1);
  assert(!serial->usesDo());
  assert(serial->stmt(0)->isVariableDecl());
}

static void test4(Parser* parser) {
  auto parseResult = parser->parseString("test4.chpl",
      "/* comment 1 */\n"
      "serial do { var a; }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModuleDecl());
  auto mod = parseResult.topLevelExpressions[0]->toModuleDecl()->module();
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSerial());
  const Serial* serial = mod->stmt(1)->toSerial();
  assert(serial != nullptr);
  assert(serial->condition() == nullptr);
  assert(serial->numStmts() == 1);
  assert(serial->usesDo());
  assert(serial->stmt(0)->isBlock());
  const Block* block = serial->stmt(0)->toBlock();
  assert(block != nullptr);
  assert(block->numStmts() == 1);
  assert(block->stmt(0)->isVariableDecl());
}

static void test5(Parser* parser) {
  auto parseResult = parser->parseString("test5.chpl",
      "/* comment 1 */\n"
      "serial foo do { var a; }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModuleDecl());
  auto mod = parseResult.topLevelExpressions[0]->toModuleDecl()->module();
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSerial());
  const Serial* serial = mod->stmt(1)->toSerial();
  assert(serial != nullptr);
  assert(serial->condition() != nullptr);
  assert(serial->condition()->isIdentifier());
  assert(serial->numStmts() == 1);
  assert(serial->usesDo());
  assert(serial->stmt(0)->isBlock());
  const Block* block = serial->stmt(0)->toBlock();
  assert(block != nullptr);
  assert(block->numStmts() == 1);
  assert(block->stmt(0)->isVariableDecl());
}

// Test nested comments.
static void test6(Parser* parser) {
  auto parseResult = parser->parseString("test6.chpl",
      "/* comment 1 */\n"
      "serial foo {\n"
      "  /* comment 2 */\n"
      "  var a;\n"
      "  /* comment 3 */\n"
      "}\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModuleDecl());
  auto mod = parseResult.topLevelExpressions[0]->toModuleDecl()->module();
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSerial());
  const Serial* serial = mod->stmt(1)->toSerial();
  assert(serial != nullptr);
  assert(!serial->usesDo());
  assert(serial->condition() != nullptr);
  assert(serial->condition()->isIdentifier());
  assert(serial->numStmts() == 3);
  assert(serial->stmt(0)->isComment());
  assert(serial->stmt(1)->isVariableDecl());
  assert(serial->stmt(2)->isComment());
}

// Test nested comments on do.
static void test7(Parser* parser) {
  auto parseResult = parser->parseString("test7.chpl",
      "/* comment 1 */\n"
      "serial do\n"
      "  /* comment 2 */\n"
      "  var a;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModuleDecl());
  auto mod = parseResult.topLevelExpressions[0]->toModuleDecl()->module();
  const Serial* serial = mod->stmt(1)->toSerial();
  assert(serial != nullptr);
  assert(serial->usesDo());
  assert(serial->condition() == nullptr);
  assert(serial->numStmts() == 2);
  assert(serial->stmt(0)->isComment());
  assert(serial->stmt(1)->isVariableDecl());
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
  test6(p);
  test7(p);

  return 0;
}
