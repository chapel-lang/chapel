/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Interface.h"
#include "chpl/uast/Implements.h"
#include "chpl/uast/Module.h"
#include "chpl/framework/Context.h"

static void test0(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test0.chpl",
      " /* c1 */\n"
      " interface Foo(a, b, c) {\n"
      "   proc foo() {}\n"
      " }\n"
      " /* c2 */\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isInterface());
  assert(mod->stmt(2)->isComment());
  auto intf = mod->stmt(1)->toInterface();
  assert(intf);
  assert(intf->isFormalListExplicit());
  assert(intf->numFormals() == 3);
  assert(intf->numStmts() == 1);
  assert(intf->stmt(0)->isFunction());
}


int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::createForTopLevelModule(ctx);
  Parser* p = &parser;

  test0(p);
  return 0;
}
