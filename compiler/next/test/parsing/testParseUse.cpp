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
#include "chpl/uast/Module.h"
#include "chpl/uast/Use.h"
#include "chpl/uast/UseClause.h"
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
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isUse());
  assert(mod->stmt(2)->isComment());
  const Use* use = mod->stmt(1)->toUse();
  assert(use->visibility() == Decl::DEFAULT_VISIBILITY);
  assert(use->numUseClauses() == 1);
  const UseClause* useClause = use->useClause(0);
  assert(useClause->symbol()->isAs());
  const As* as = useClause->symbol()->toAs();
  assert(as->symbol()->isIdentifier());
  assert(as->symbol()->toIdentifier()->name() == "Foo");
  assert(as->rename()->isIdentifier());
  assert(as->rename()->toIdentifier()->name() == "X");
  assert(useClause->limitationClauseKind() == UseClause::NONE);
  assert(useClause->numLimitations() == 0);
}

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
      "/*c1*/\n"
      "public use /*c2*/ A as X, /*c3*/ B.SM1 as Y, /*c4*/ C as Z;\n"
      "/*c5*/\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isUse());
  assert(mod->stmt(2)->isComment());
  const Use* use = mod->stmt(1)->toUse();
  assert(use->visibility() == Decl::PUBLIC);
  assert(use->numUseClauses() == 3);
  // Inspect the first use clause.
  {
    const UseClause* useClause = use->useClause(0);
    assert(useClause->symbol()->isAs());
    const As* as = useClause->symbol()->toAs();
    assert(as->symbol()->isIdentifier());
    assert(as->symbol()->toIdentifier()->name() == "A");
    assert(as->rename()->isIdentifier());
    assert(as->rename()->toIdentifier()->name() == "X");
    assert(useClause->limitationClauseKind() == UseClause::NONE);
    assert(useClause->numLimitations() == 0);
  }
  // Inspect the second use clause.
  {
    const UseClause* useClause = use->useClause(1);
    assert(useClause->symbol()->isAs());
    const As* as = useClause->symbol()->toAs();
    assert(as->symbol()->isDot());
    const Dot* dot = as->symbol()->toDot();
    assert(dot->numActuals() == 0);
    assert(dot->calledExpression()->isIdentifier());
    assert(dot->calledExpression()->toIdentifier()->name() == "B");
    assert(dot->field() == "SM1");
    assert(as->rename()->isIdentifier());
    assert(as->rename()->toIdentifier()->name() == "Y");
    assert(useClause->limitationClauseKind() == UseClause::NONE);
    assert(useClause->numLimitations() == 0);
  }
  // Inspect the third use clause.
  {
    const UseClause* useClause = use->useClause(2);
    assert(useClause->symbol()->isAs());
    const As* as = useClause->symbol()->toAs();
    assert(as->symbol()->isIdentifier());
    assert(as->symbol()->toIdentifier()->name() == "C");
    assert(as->rename()->isIdentifier());
    assert(as->rename()->toIdentifier()->name() == "Z");
    assert(useClause->limitationClauseKind() == UseClause::NONE);
    assert(useClause->numLimitations() == 0);
  }
  // Make sure the 'useClauses' iterator works as expected.
  {
    int count = 0;
    for (const auto useClause : use->useClauses()) {
      assert(useClause->tag() == asttags::UseClause);
      assert(useClause->isUseClause());
      assert(useClause == use->useClause(count));
      count++;
    }
    assert(count == use->numUseClauses());
  }
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
      "/*c1*/\n"
      "private use /*c2*/ A as X /*c3*/ except Foo, /*c5*/ Bar, Baz /*c6*/;\n"
      "/*c7*/\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isUse());
  assert(mod->stmt(2)->isComment());
  const Use* use = mod->stmt(1)->toUse();
  assert(use->visibility() == Decl::PRIVATE);
  assert(use->numUseClauses() == 1);
  const UseClause* useClause = use->useClause(0);
  assert(useClause->symbol()->isAs());
  const As* as = useClause->symbol()->toAs();
  assert(as->symbol()->isIdentifier());
  assert(as->symbol()->toIdentifier()->name() == "A");
  assert(as->rename()->isIdentifier());
  assert(as->rename()->toIdentifier()->name() == "X");
  assert(useClause->limitationClauseKind() == UseClause::EXCEPT);
  assert(useClause->numLimitations() == 3);
  assert(useClause->limitation(0)->isIdentifier()); 
  assert(useClause->limitation(1)->isIdentifier()); 
  assert(useClause->limitation(2)->isIdentifier()); 

  // Make sure the limitations iterator works as expected.
  {
    int count = 0;
    for (const auto limitation : useClause->limitations()) {
      assert(limitation->tag() == asttags::Identifier);
      assert(limitation->tag() == useClause->limitation(count)->tag());
      assert(limitation->isIdentifier());
      count++;
    }
    assert(count == useClause->numLimitations());
  }
}

static void test3(Parser* parser) {
  auto parseResult = parser->parseString("test3.chpl",
      "/*c1*/\n"
      "use A.SM1 only Foo as X, Bar, Baz as Y;\n"
      "/*c7*/\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isUse());
  assert(mod->stmt(2)->isComment());
  const Use* use = mod->stmt(1)->toUse();
  assert(use->numUseClauses() == 1);
  const UseClause* useClause = use->useClause(0);
  assert(useClause->symbol()->isDot());
  const Dot* dot = useClause->symbol()->toDot();
  assert(dot->calledExpression()->isIdentifier());
  assert(dot->calledExpression()->toIdentifier()->name() == "A");
  assert(dot->field() == "SM1");
  assert(useClause->limitationClauseKind() == UseClause::ONLY);
  assert(useClause->numLimitations() == 3);
  // Check the first limitation.
  {
    assert(useClause->limitation(0)->isAs());
    const As* as = useClause->limitation(0)->toAs();
    assert(as->symbol()->isIdentifier());
    assert(as->symbol()->toIdentifier()->name() == "Foo");
    assert(as->rename()->isIdentifier());
    assert(as->rename()->toIdentifier()->name() == "X");
  }
  // Check the second limitation.
  {
    assert(useClause->limitation(1)->isIdentifier());
    assert(useClause->limitation(1)->toIdentifier()->name() == "Bar");
  }
  // Check the third limitation.
  {
    assert(useClause->limitation(2)->isAs());
    const As* as = useClause->limitation(2)->toAs();
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
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isUse());
  assert(mod->stmt(2)->isComment());
  const Use* use = mod->stmt(1)->toUse();
  assert(use->visibility() == Decl::DEFAULT_VISIBILITY);
  assert(use->numUseClauses() == 1);
  const UseClause* useClause = use->useClause(0);
  assert(useClause->symbol()->isIdentifier());
  assert(useClause->symbol()->toIdentifier()->name() == "Foo");
  assert(useClause->numLimitations() == 0);
  assert(useClause->limitationClauseKind() == UseClause::ONLY);
}

static void test5(Parser* parser) {
  auto parseResult = parser->parseString("test5.chpl",
      "/*c1*/\n"
      "use Foo except *;\n"
      "/*c7*/\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isUse());
  assert(mod->stmt(2)->isComment());
  const Use* use = mod->stmt(1)->toUse();
  assert(use->visibility() == Decl::DEFAULT_VISIBILITY);
  assert(use->numUseClauses() == 1);
  const UseClause* useClause = use->useClause(0);
  assert(useClause->symbol()->isIdentifier());
  assert(useClause->symbol()->toIdentifier()->name() == "Foo");
  assert(useClause->limitationClauseKind() == UseClause::EXCEPT);
  assert(useClause->numLimitations() == 1);
  assert(useClause->limitation(0)->isIdentifier());
  assert(useClause->limitation(0)->toIdentifier()->name() == "*");
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
