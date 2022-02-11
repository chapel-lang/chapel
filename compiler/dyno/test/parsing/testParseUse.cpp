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
#include "chpl/uast/As.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Dot.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Use.h"
#include "chpl/uast/VisibilityClause.h"
#include "chpl/uast/Variable.h"

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
      "use /*c2*/ Foo as X /*c3*/;\n"
      "/*c4*/\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isUse());
  assert(mod->stmt(2)->isComment());
  const Use* use = mod->stmt(1)->toUse();
  assert(use->visibility() == Decl::DEFAULT_VISIBILITY);
  assert(use->numVisibilityClauses() == 1);
  const VisibilityClause* visClause = use->visibilityClause(0);
  assert(visClause->symbol()->isAs());
  const As* as = visClause->symbol()->toAs();
  assert(as->symbol()->isIdentifier());
  assert(as->symbol()->toIdentifier()->name() == "Foo");
  assert(as->rename()->isIdentifier());
  assert(as->rename()->toIdentifier()->name() == "X");
  assert(visClause->limitationKind() == VisibilityClause::NONE);
  assert(visClause->numLimitations() == 0);
}

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
      "/*c1*/\n"
      "public use /*c2*/ A as X, /*c3*/ B.SM1 as Y, /*c4*/ C as Z;\n"
      "/*c5*/\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isUse());
  assert(mod->stmt(2)->isComment());
  const Use* use = mod->stmt(1)->toUse();
  assert(use->visibility() == Decl::PUBLIC);
  assert(use->numVisibilityClauses() == 3);
  // Inspect the first visibility clause.
  {
    const VisibilityClause* visClause = use->visibilityClause(0);
    assert(visClause->symbol()->isAs());
    const As* as = visClause->symbol()->toAs();
    assert(as->symbol()->isIdentifier());
    assert(as->symbol()->toIdentifier()->name() == "A");
    assert(as->rename()->isIdentifier());
    assert(as->rename()->toIdentifier()->name() == "X");
    assert(visClause->limitationKind() == VisibilityClause::NONE);
    assert(visClause->numLimitations() == 0);
  }
  // Inspect the second visibility clause.
  {
    const VisibilityClause* visClause = use->visibilityClause(1);
    assert(visClause->symbol()->isAs());
    const As* as = visClause->symbol()->toAs();
    assert(as->symbol()->isDot());
    const Dot* dot = as->symbol()->toDot();
    assert(dot->receiver()->isIdentifier());
    assert(dot->receiver()->toIdentifier()->name() == "B");
    assert(dot->field() == "SM1");
    assert(as->rename()->isIdentifier());
    assert(as->rename()->toIdentifier()->name() == "Y");
    assert(visClause->limitationKind() == VisibilityClause::NONE);
    assert(visClause->numLimitations() == 0);
  }
  // Inspect the third visibility clause.
  {
    const VisibilityClause* visClause = use->visibilityClause(2);
    assert(visClause->symbol()->isAs());
    const As* as = visClause->symbol()->toAs();
    assert(as->symbol()->isIdentifier());
    assert(as->symbol()->toIdentifier()->name() == "C");
    assert(as->rename()->isIdentifier());
    assert(as->rename()->toIdentifier()->name() == "Z");
    assert(visClause->limitationKind() == VisibilityClause::NONE);
    assert(visClause->numLimitations() == 0);
  }
  // Make sure the 'visibilityClauses' iterator works as expected.
  {
    int count = 0;
    for (const auto visClause : use->visibilityClauses()) {
      assert(visClause->tag() == asttags::VisibilityClause);
      assert(visClause->isVisibilityClause());
      assert(visClause == use->visibilityClause(count));
      count++;
    }
    assert(count == use->numVisibilityClauses());
  }
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
      "/*c1*/\n"
      "private use /*c2*/ A as X /*c3*/ except Foo, /*c5*/ Bar, Baz /*c6*/;\n"
      "/*c7*/\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isUse());
  assert(mod->stmt(2)->isComment());
  const Use* use = mod->stmt(1)->toUse();
  assert(use->visibility() == Decl::PRIVATE);
  assert(use->numVisibilityClauses() == 1);
  const VisibilityClause* visClause = use->visibilityClause(0);
  assert(visClause->symbol()->isAs());
  const As* as = visClause->symbol()->toAs();
  assert(as->symbol()->isIdentifier());
  assert(as->symbol()->toIdentifier()->name() == "A");
  assert(as->rename()->isIdentifier());
  assert(as->rename()->toIdentifier()->name() == "X");
  assert(visClause->limitationKind() == VisibilityClause::EXCEPT);
  assert(visClause->numLimitations() == 3);
  assert(visClause->limitation(0)->isIdentifier());
  assert(visClause->limitation(1)->isIdentifier());
  assert(visClause->limitation(2)->isIdentifier());

  // Make sure the limitations iterator works as expected.
  {
    int count = 0;
    for (const auto limitation : visClause->limitations()) {
      assert(limitation->tag() == asttags::Identifier);
      assert(limitation->tag() == visClause->limitation(count)->tag());
      assert(limitation->isIdentifier());
      count++;
    }
    assert(count == visClause->numLimitations());
  }
}

static void test3(Parser* parser) {
  auto parseResult = parser->parseString("test3.chpl",
      "/*c1*/\n"
      "use A.SM1 only Foo as X, Bar, Baz as Y;\n"
      "/*c7*/\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isUse());
  assert(mod->stmt(2)->isComment());
  const Use* use = mod->stmt(1)->toUse();
  assert(use->numVisibilityClauses() == 1);
  const VisibilityClause* visClause = use->visibilityClause(0);
  assert(visClause->symbol()->isDot());
  const Dot* dot = visClause->symbol()->toDot();
  assert(dot->receiver()->isIdentifier());
  assert(dot->receiver()->toIdentifier()->name() == "A");
  assert(dot->field() == "SM1");
  assert(visClause->limitationKind() == VisibilityClause::ONLY);
  assert(visClause->numLimitations() == 3);
  // Check the first limitation.
  {
    assert(visClause->limitation(0)->isAs());
    const As* as = visClause->limitation(0)->toAs();
    assert(as->symbol()->isIdentifier());
    assert(as->symbol()->toIdentifier()->name() == "Foo");
    assert(as->rename()->isIdentifier());
    assert(as->rename()->toIdentifier()->name() == "X");
  }
  // Check the second limitation.
  {
    assert(visClause->limitation(1)->isIdentifier());
    assert(visClause->limitation(1)->toIdentifier()->name() == "Bar");
  }
  // Check the third limitation.
  {
    assert(visClause->limitation(2)->isAs());
    const As* as = visClause->limitation(2)->toAs();
    assert(as->symbol()->isIdentifier());
    assert(as->symbol()->toIdentifier()->name() == "Baz");
    assert(as->rename()->isIdentifier());
    assert(as->rename()->toIdentifier()->name() == "Y");
  }
}

static void test4(Parser* parser) {
  auto parseResult = parser->parseString("test4.chpl",
      "/*c1*/\n"
      "use Foo only;\n"
      "/*c7*/\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isUse());
  assert(mod->stmt(2)->isComment());
  const Use* use = mod->stmt(1)->toUse();
  assert(use->visibility() == Decl::DEFAULT_VISIBILITY);
  assert(use->numVisibilityClauses() == 1);
  const VisibilityClause* visClause = use->visibilityClause(0);
  assert(visClause->symbol()->isIdentifier());
  assert(visClause->symbol()->toIdentifier()->name() == "Foo");
  assert(visClause->numLimitations() == 0);
  assert(visClause->limitationKind() == VisibilityClause::ONLY);
}

static void test5(Parser* parser) {
  auto parseResult = parser->parseString("test5.chpl",
      "/*c1*/\n"
      "use Foo except %;\n"
      "/*c7*/\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isUse());
  assert(mod->stmt(2)->isComment());
  const Use* use = mod->stmt(1)->toUse();
  assert(use->visibility() == Decl::DEFAULT_VISIBILITY);
  assert(use->numVisibilityClauses() == 1);
  const VisibilityClause* visClause = use->visibilityClause(0);
  assert(visClause->symbol()->isIdentifier());
  assert(visClause->symbol()->toIdentifier()->name() == "Foo");
  assert(visClause->limitationKind() == VisibilityClause::EXCEPT);
  assert(visClause->numLimitations() == 1);
  assert(visClause->limitation(0)->isIdentifier());
  assert(visClause->limitation(0)->toIdentifier()->name() == "%");
}

static void test6(Parser* parser) {
  auto parseResult = parser->parseString("test6.chpl",
      "/*c1*/\n"
      "use 1+1;\n"
      "/*c7*/\n");
  assert(parseResult.numErrors() >= 1);
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isErroneousExpression());
  assert(mod->stmt(2)->isComment());
}

static void test7(Parser* parser) {
  auto parseResult = parser->parseString("test7.chpl",
      "/*c1*/\n"
      "use Foo only +;\n"
      "/*c7*/\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isUse());
  assert(mod->stmt(2)->isComment());
  const Use* use = mod->stmt(1)->toUse();
  assert(use->visibility() == Decl::DEFAULT_VISIBILITY);
  assert(use->numVisibilityClauses() == 1);
  const VisibilityClause* visClause = use->visibilityClause(0);
  assert(visClause->symbol()->isIdentifier());
  assert(visClause->symbol()->toIdentifier()->name() == "Foo");
  assert(visClause->limitationKind() == VisibilityClause::ONLY);
  assert(visClause->numLimitations() == 1);
  assert(visClause->limitation(0)->isIdentifier());
  assert(visClause->limitation(0)->toIdentifier()->name() == "+");
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
