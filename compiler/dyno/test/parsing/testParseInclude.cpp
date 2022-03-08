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
#include "chpl/queries/Context.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Local.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Include.h"

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
      "/* comment 1 */\n"
      "include module Foo;\n"
      "/* comment 2 */\n"
      "include private module Bar;\n"
      "include prototype module Baz;\n"
      "/* comment 3*/\n"
      "include private prototype module Zing;\n"
      "include public prototype module A;\n"
      "include public module B;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 9);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isInclude());
  assert(mod->stmt(2)->isComment());
  assert(mod->stmt(3)->isInclude());
  assert(mod->stmt(4)->isInclude());
  assert(mod->stmt(5)->isComment());
  assert(mod->stmt(6)->isInclude());
  assert(mod->stmt(7)->isInclude());
  assert(mod->stmt(8)->isInclude());

  auto inc1 = mod->stmt(1)->toInclude();
  assert(inc1->visibility() == Decl::DEFAULT_VISIBILITY);
  assert(!inc1->isPrototype());
  assert(inc1->name() == "Foo");

  auto inc2 = mod->stmt(3)->toInclude();
  assert(inc2->visibility() == Decl::PRIVATE);
  assert(!inc2->isPrototype());
  assert(inc2->name() == "Bar");

  auto inc3 = mod->stmt(4)->toInclude();
  assert(inc3->visibility() == Decl::DEFAULT_VISIBILITY);
  assert(inc3->isPrototype());
  assert(inc3->name() == "Baz");

  auto inc4 = mod->stmt(6)->toInclude();
  assert(inc4->visibility() == Decl::PRIVATE);
  assert(inc4->isPrototype());
  assert(inc4->name() == "Zing");

  auto inc5 = mod->stmt(7)->toInclude();
  assert(inc5->visibility() == Decl::PUBLIC);
  assert(inc5->isPrototype());
  assert(inc5->name() == "A");

  auto inc6 = mod->stmt(8)->toInclude();
  assert(inc6->visibility() == Decl::PUBLIC);
  assert(!inc6->isPrototype());
  assert(inc6->name() == "B");
}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  test0(p);

  return 0;
}
