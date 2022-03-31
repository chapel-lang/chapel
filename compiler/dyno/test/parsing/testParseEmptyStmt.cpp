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
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/EmptyStmt.h"

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
        "hi();\n"
         ";\n"
         ";\n"
         "tester();\n"
         ";\n"
         ";\n"
         ";\n"
         "bye();\n");

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 8);

  auto e1 = mod->stmt(1)->toEmptyStmt();
  assert(e1);
  auto e2 = mod->stmt(2)->toEmptyStmt();
  assert(e2);
  auto e4 = mod->stmt(4)->toEmptyStmt();
  assert(e4);
  auto e5 = mod->stmt(5)->toEmptyStmt();
  assert(e5);
  auto e6 = mod->stmt(6)->toEmptyStmt();
  assert(e6);

  auto fnCall1 = mod->stmt(0)->toFnCall();
  assert(fnCall1->numActuals()==0);
  assert(fnCall1->callUsedSquareBrackets() == false);
  auto baseExpr = fnCall1->calledExpression();
  assert(baseExpr);
  auto baseExprIdent = baseExpr->toIdentifier();
  assert(0 == baseExprIdent->name().compare("hi"));

  auto fnCall2 = mod->stmt(3)->toFnCall();
  assert(fnCall2->numActuals()==0);
  assert(fnCall2->callUsedSquareBrackets() == false);
  auto baseExpr2 = fnCall2->calledExpression();
  assert(baseExpr2);
  auto baseExprIdent2 = baseExpr2->toIdentifier();
  assert(0 == baseExprIdent2->name().compare("tester"));

  auto fnCall3 = mod->stmt(7)->toFnCall();
  assert(fnCall3->numActuals()==0);
  assert(fnCall3->callUsedSquareBrackets() == false);
  auto baseExpr3 = fnCall3->calledExpression();
  assert(baseExpr3);
  auto baseExprIdent3 = baseExpr3->toIdentifier();
  assert(0 == baseExprIdent3->name().compare("bye"));

}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  test0(p);

  return 0;
}
