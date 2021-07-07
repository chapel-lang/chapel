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
#include "chpl/uast/As.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Dot.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Import.h"
#include "chpl/uast/Module.h"
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
      "import /*c2*/ Foo as X /*c3*/;\n"
      "/*c4*/\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isImport());
  assert(mod->stmt(2)->isComment());
  const Import* imp = mod->stmt(1)->toImport();
  assert(imp->visibility() == Decl::DEFAULT_VISIBILITY);
  assert(imp->numVisibilityClauses() == 1);
  const VisibilityClause* visClause = imp->visibilityClause(0);
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
      "public import /*c2*/ A as X, /*c3*/ B.SM1 as Y, /*c4*/ C as Z;\n"
      "/*c5*/\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isImport());
  assert(mod->stmt(2)->isComment());
  const Import* imp = mod->stmt(1)->toImport();
  assert(imp->visibility() == Decl::PUBLIC);
  assert(imp->numVisibilityClauses() == 3);
  // Inspect the first visibility clause.
  {
    const VisibilityClause* visClause = imp->visibilityClause(0);
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
    const VisibilityClause* visClause = imp->visibilityClause(1);
    assert(visClause->symbol()->isAs());
    const As* as = visClause->symbol()->toAs();
    assert(as->symbol()->isDot());
    const Dot* dot = as->symbol()->toDot();
    assert(dot->numActuals() == 0);
    assert(dot->calledExpression()->isIdentifier());
    assert(dot->calledExpression()->toIdentifier()->name() == "B");
    assert(dot->field() == "SM1");
    assert(as->rename()->isIdentifier());
    assert(as->rename()->toIdentifier()->name() == "Y");
    assert(visClause->limitationKind() == VisibilityClause::NONE);
    assert(visClause->numLimitations() == 0);
  }
  // Inspect the third visibility clause.
  {
    const VisibilityClause* visClause = imp->visibilityClause(2);
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
    for (const auto visClause : imp->visibilityClauses()) {
      assert(visClause->tag() == asttags::VisibilityClause);
      assert(visClause->isVisibilityClause());
      assert(visClause == imp->visibilityClause(count));
      count++;
    }
    assert(count == imp->numVisibilityClauses());
  }
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
      "/*c1*/\n"
      "private import /*c2*/ A as X, B.{Y, Z};\n"
      "/*c7*/\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isImport());
  assert(mod->stmt(2)->isComment());
  const Import* imp = mod->stmt(1)->toImport();
  assert(imp->visibility() == Decl::PRIVATE);
  assert(imp->numVisibilityClauses() == 2);

  // Check the first visibility clause.
  {
    const VisibilityClause* visClause = imp->visibilityClause(0);
    assert(visClause->symbol()->isAs());
    const As* as = visClause->symbol()->toAs();
    assert(as->symbol()->isIdentifier());
    assert(as->symbol()->toIdentifier()->name() == "A");
    assert(as->rename()->isIdentifier());
    assert(as->rename()->toIdentifier()->name() == "X");
    assert(visClause->limitationKind() == VisibilityClause::NONE);
    assert(visClause->numLimitations() == 0);
  }

  // Check the second visibility clause.
  {
    const VisibilityClause* visClause = imp->visibilityClause(1);
    assert(visClause->symbol()->isIdentifier());
    assert(visClause->symbol()->toIdentifier()->name() == "B");
    assert(visClause->limitationKind() == VisibilityClause::BRACES);
    assert(visClause->numLimitations() == 2);

    assert(visClause->limitation(0)->isIdentifier());
    assert(visClause->limitation(0)->toIdentifier()->name() == "Y");
    assert(visClause->limitation(1)->isIdentifier());
    assert(visClause->limitation(1)->toIdentifier()->name() == "Z");
  
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
