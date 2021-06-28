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
#include "chpl/uast/Break.h"
#include "chpl/uast/Conditional.h"
#include "chpl/uast/Continue.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/For.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Label.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/While.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <iostream>

using namespace chpl;
using namespace uast;
using namespace parsing;

static void test0(Parser* parser) {
  auto parseResult = parser->parseString("test0.chpl",
    "/*c1*/\n"
    "var thing = 0;\n"
    "label outer for i in myRange1 {\n"
    "  /*c2*/\n"
    "  for j in myRange2 do\n"
    "    if !(i%j) { found = j; break outer; }\n"
    "  /*c3*/\n"
    "}\n"
    "/*c12*/\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 4);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isVariable());
  assert(mod->stmt(2)->isLabel());
  assert(mod->stmt(3)->isComment());
  const Label* label = mod->stmt(2)->toLabel();
  assert(!label->name().compare("outer"));
  assert(label->loop() && label->loop()->isFor());
  const For* outerFor = label->loop()->toFor();
  assert(outerFor->numStmts() == 3);
  assert(outerFor->stmt(0)->isComment());
  assert(outerFor->stmt(1)->isFor());
  assert(outerFor->stmt(2)->isComment());
  const For* innerFor = outerFor->stmt(1)->toFor();
  assert(innerFor->blockStyle() == BlockStyle::IMPLICIT);
  assert(!innerFor->isExpressionLevel());
  assert(innerFor->numStmts() == 1);
  assert(innerFor->stmt(0)->isConditional());
  const Conditional* cond = innerFor->stmt(0)->toConditional();
  assert(!cond->hasElseBlock());
  assert(cond->thenBlockStyle() == BlockStyle::EXPLICIT);
  assert(cond->numThenStmts() == 2);
  assert(cond->thenStmt(0)->isOpCall());
  assert(cond->thenStmt(1)->isBreak());
  const Break* breakToOuter = cond->thenStmt(1)->toBreak();
  assert(!breakToOuter->target()->name().compare("outer"));
}

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
    "/*c1*/\n"
    "label outer for i in myRange {\n"
    "  if i%2 then /*c2*/ continue /*c3*/ outer /*c4*/;\n"
    "  writeln(i);\n"
    "  /*c5*/\n"
    "}\n"
    "/*c6*/\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isLabel());
  assert(mod->stmt(2)->isComment());
  const Label* label = mod->stmt(1)->toLabel();
  assert(!label->name().compare("outer"));
  assert(label->loop()->isFor());
  const For* forLoop = label->loop()->toFor();
  assert(forLoop->numStmts() == 3);
  assert(forLoop->stmt(0)->isConditional());
  assert(forLoop->stmt(1)->isFnCall());
  assert(forLoop->stmt(2)->isComment());
  const Conditional* cond = forLoop->stmt(0)->toConditional();
  assert(!cond->hasElseBlock());
  assert(cond->thenBlockStyle() == BlockStyle::IMPLICIT);
  assert(cond->numThenStmts() == 2);
  assert(cond->thenStmt(0)->isComment());
  assert(cond->thenStmt(1)->isContinue());
  const Continue* cont = cond->thenStmt(1)->toContinue();
  assert(cont->target());
  assert(!cont->target()->name().compare("outer"));
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
