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
#include "chpl/uast/Break.h"
#include "chpl/uast/Conditional.h"
#include "chpl/uast/Continue.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/For.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Label.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/While.h"
#include "chpl/frontend/Parser.h"
#include "chpl/queries/Context.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <iostream>

using namespace chpl;
using namespace uast;
using namespace frontend;

static void test0(Parser* parser) {
  auto parseResult = parser->parseString("test0.chpl",
    "/*c1*/\n"
    "for i in myRange {\n"
    "  while alwaysTrue do\n"
    "    if !(i%2) then /*c2*/ break /*c2*/ print /*c4*/;\n"
    "    else /*c5*/ break /*c6*/ next;\n"
    "  /*c7*/\n"
    "  label /*c8*/ print /*c9*/ writeln(i);\n"
    "  label /*c10*/ next /*c11*/;\n"
    "}\n"
    "/*c12*/\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(1)->isFor());
  const For* forLoop = mod->stmt(1)->toFor();
  assert(forLoop->numStmts() == 4);
  assert(forLoop->stmt(0)->isWhile());
  assert(forLoop->stmt(1)->isComment());
  // First walk through and check the break statements.
  const While* whileLoop = forLoop->stmt(0)->toWhile();
  assert(whileLoop->numStmts() == 1);
  assert(whileLoop->stmt(0)->isConditional());
  const Conditional* cond = whileLoop->stmt(0)->toConditional();
  assert(cond->numThenStmts() == 2);
  assert(cond->thenStmt(0)->isComment());
  assert(cond->thenStmt(1)->isBreak());
  const Break* thenBreak = cond->thenStmt(1)->toBreak();
  assert(thenBreak->target() && thenBreak->target()->isIdentifier());
  assert(!thenBreak->target()->toIdentifier()->name().compare("print"));
  assert(cond->hasElseBlock());
  assert(cond->numElseStmts() == 2);
  assert(cond->elseStmt(0)->isComment());
  assert(cond->elseStmt(1)->isBreak());
  const Break* elseBreak = cond->elseStmt(1)->toBreak();
  assert(elseBreak->target() && elseBreak->target()->isIdentifier());
  assert(!elseBreak->target()->toIdentifier()->name().compare("next"));
  // Now check the label statements.
  assert(forLoop->stmt(2)->isLabel());
  const Label* labelPrint = forLoop->stmt(2)->toLabel();
  assert(!labelPrint->name().compare("print"));
  assert(labelPrint->blockStyle() == BlockStyle::IMPLICIT);
  assert(labelPrint->numStmts() == 2);
  assert(labelPrint->stmt(0)->isComment());
  assert(labelPrint->stmt(1)->isFnCall());
  assert(forLoop->stmt(3)->isLabel());
  const Label* labelNext = forLoop->stmt(3)->toLabel();
  assert(!labelNext->name().compare("next"));
  assert(labelNext->blockStyle() == BlockStyle::IMPLICIT);
  assert(labelNext->numStmts() == 0);
}

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
    "/*c1*/\n"
    "var i = 0;\n"
    "while alwaysTrue {\n"
    "  if i%2 then /*c2*/ continue /*c3*/ next /*c4*/;\n"
    "  writeln(i);\n"
    "  label next if i >= 100 then break; else i += 1;\n"
    "}\n"
    "/*c5*/\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 4);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isVariable());
  assert(mod->stmt(2)->isWhile());
  assert(mod->stmt(3)->isComment());
  const While* whileLoop = mod->stmt(2)->toWhile();
  assert(whileLoop->blockStyle() == BlockStyle::EXPLICIT);
  assert(whileLoop->numStmts() == 3);
  assert(whileLoop->stmt(0)->isConditional());
  const Conditional* cond = whileLoop->stmt(0)->toConditional();
  assert(!cond->hasElseBlock());
  assert(cond->numThenStmts() == 2);
  assert(cond->thenStmt(0)->isComment());
  assert(cond->thenStmt(1)->isContinue());
  const Continue* cont = cond->thenStmt(1)->toContinue();
  assert(cont->target() && !cont->target()->name().compare("next"));
  assert(whileLoop->stmt(1)->isFnCall());
  assert(whileLoop->stmt(2)->isLabel());
  const Label* label = whileLoop->stmt(2)->toLabel();
  assert(!label->name().compare("next"));
  assert(label->numStmts() == 1);
  assert(label->stmt(0)->isConditional());
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
