/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Begin.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Sync.h"
#include "chpl/queries/Context.h"


// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace uast;
using namespace parsing;

static void test0(Parser* parser) {
  auto parseResult = parser->parseString("test0.chpl",
      " /* c1 */\n"
      " interface Foo(a, b, c) {\n"
      "   proc foo() {}\n"
      " }\n"
      " /* c2 */\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isInterface());
  assert(mod->stmt(2)->isComment());
}


int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  test0(p);
  return 0;
}
