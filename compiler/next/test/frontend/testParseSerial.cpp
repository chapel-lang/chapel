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
      "serial /* comment 2 */ do\n"
      "  /* comment 3 */\n"
      "  var a;\n"
      "/*comment 4 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSerial());
  assert(mod->stmt(2)->isComment());
  const Serial* serial = mod->stmt(1)->toSerial();
  assert(serial != nullptr);
  assert(serial->condition() == nullptr);
  assert(serial->numStmts() == 2);
  assert(serial->blockStyle() == BlockStyle::IMPLICIT);
  assert(serial->stmt(0)->isComment());
  assert(serial->stmt(1)->isVariable());
}

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
      "/* comment 1 */\n"
      "serial /* comment 2 */ foo /* comment 3 */ do\n"
      "  /* comment 4 */\n"
      "  var a;\n"
      "/* comment 5 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSerial());
  assert(mod->stmt(2)->isComment());
  const Serial* serial = mod->stmt(1)->toSerial();
  assert(serial != nullptr);
  assert(serial->condition() != nullptr);
  assert(serial->condition()->isIdentifier());
  assert(serial->numStmts() == 2);
  assert(serial->blockStyle() == BlockStyle::IMPLICIT);
  assert(serial->stmt(0)->isComment());
  assert(serial->stmt(1)->isVariable());
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
      "/* comment 1 */\n"
      "serial /* comment 2 */ {\n"
      "  /* comment 3 */\n"
      "  var a;\n"
      "  /* comment 4 */\n"
      "}\n"
      "/* comment 5 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSerial());
  const Serial* serial = mod->stmt(1)->toSerial();
  assert(serial != nullptr);
  assert(serial->condition() == nullptr);
  assert(serial->numStmts() == 3);
  assert(serial->blockStyle() == BlockStyle::EXPLICIT);
  assert(serial->stmt(0)->isComment());
  assert(serial->stmt(1)->isVariable());
  assert(serial->stmt(2)->isComment());
}

static void test3(Parser* parser) {
  auto parseResult = parser->parseString("test3.chpl",
      "/* comment 1 */\n"
      "serial /* comment 2 */ foo /* comment 3 */ {\n"
      "  /* comment 4 */\n"
      "  var a;\n"
      "  /* comment 5 */\n"
      "}\n"
      "/* comment 6 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSerial());
  assert(mod->stmt(2)->isComment());
  const Serial* serial = mod->stmt(1)->toSerial();
  assert(serial != nullptr);
  assert(serial->condition() != nullptr);
  assert(serial->condition()->isIdentifier());
  assert(serial->numStmts() == 3);
  assert(serial->blockStyle() == BlockStyle::EXPLICIT);
  assert(serial->stmt(0)->isComment());
  assert(serial->stmt(1)->isVariable());
  assert(serial->stmt(2)->isComment());
}

static void test4(Parser* parser) {
  auto parseResult = parser->parseString("test4.chpl",
      "/* comment 1 */\n"
      "serial do { var a; }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSerial());
  const Serial* serial = mod->stmt(1)->toSerial();
  assert(serial != nullptr);
  assert(serial->condition() == nullptr);
  assert(serial->numStmts() == 1);
  assert(serial->blockStyle() == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK);
  assert(serial->stmt(0)->isVariable());
}

static void test5(Parser* parser) {
  auto parseResult = parser->parseString("test5.chpl",
      "/* comment 1 */\n"
      "serial foo do { var a; }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 2);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isSerial());
  const Serial* serial = mod->stmt(1)->toSerial();
  assert(serial != nullptr);
  assert(serial->condition() != nullptr);
  assert(serial->condition()->isIdentifier());
  assert(serial->numStmts() == 1);
  assert(serial->blockStyle() == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK);
  assert(serial->stmt(0)->isVariable());
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
