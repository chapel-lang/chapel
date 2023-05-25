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
#include "chpl/uast/Function.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Return.h"

static void test0(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test0.chpl",
      "/* comment 1 */\n"
      "proc foo() {\n"
      "  /* comment 2 */\n"
      "  /* comment 3 */\n"
      "  return /* comment 4 */ bar() /* comment 5 */;\n"
      "  /* comment 6 */\n"
      "}\n"
      "/* comment 7 */\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isFunction());
  assert(mod->stmt(2)->isComment());
  const Function* func = mod->stmt(1)->toFunction();
  assert(func);
  assert(func->numStmts() == 4);
  assert(func->stmt(0)->isComment());
  assert(func->stmt(1)->isComment());
  assert(func->stmt(2)->isReturn());
  assert(func->stmt(3)->isComment());
  const Return* ret = func->stmt(2)->toReturn();
  assert(ret);
  assert(ret->value());
  assert(ret->value()->isFnCall());
}

static void test1(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test1.chpl",
      "/* comment 1 */\n"
      "proc foo() {\n"
      "  /* comment 2 */\n"
      "  /* comment 3 */\n"
      "  return /* comment 4 */;\n"
      "  /* comment 5 */\n"
      "}\n"
      "/* comment 6 */\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isFunction());
  assert(mod->stmt(2)->isComment());
  const Function* func = mod->stmt(1)->toFunction();
  assert(func);
  assert(func->numStmts() == 4);
  assert(func->stmt(0)->isComment());
  assert(func->stmt(1)->isComment());
  assert(func->stmt(2)->isReturn());
  assert(func->stmt(3)->isComment());
  const Return* ret = func->stmt(2)->toReturn();
  assert(ret);
  assert(ret->value() == nullptr);
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
