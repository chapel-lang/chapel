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
#include "chpl/uast/While.h"
#include "chpl/uast/BoolLiteral.h"
#include "chpl/uast/Cobegin.h"

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

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
        "while true do\n"
         ";\n");

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto stmt = mod->stmt(0)->toWhile();
  assert(stmt);
  auto cond = stmt->condition()->toBoolLiteral();
  assert(cond->value()==true);
  assert(stmt->blockStyle() == BlockStyle::IMPLICIT);
  assert(stmt->numStmts()==1);
  auto body = stmt->body();
  assert(body);
  assert(body->numChildren()==1);
  // TODO: Why is the body blockstyle EXPLICIT here? Shouldn't it follow
  // from the parent loop blockstyle?
  // Yes, but it requires changes to parser and some nodes (like Loop) to get
  // the blockStyle from the child instead of storing it in the parent
  // (or at least make sure the styles match when the node is built)
  assert(body->blockStyle()==BlockStyle::EXPLICIT);
  auto e1 = body->stmt(0)->toEmptyStmt();
  assert(e1);
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
        "cobegin { ; writeln['']; }\n");

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto stmt = mod->stmt(0)->toCobegin();
  assert(stmt);
  assert(stmt->numTaskBodies() == 2);
  auto e1 = stmt->taskBody(0)->toEmptyStmt();
  assert(e1);
  auto call = stmt->taskBody(1)->toFnCall();
  assert(call);
  assert(call->numActuals()==1);
  assert(call->callUsedSquareBrackets() == true);
  auto baseExpr = call->calledExpression();
  assert(baseExpr);
  auto baseExprIdent = baseExpr->toIdentifier();
  assert(0 == baseExprIdent->name().compare("writeln"));
}

static void test3(Parser* parser) {
  // TODO: There is discussion of whether this should be an error or not
  // Brad says "I’d be most comfortable if it went back to being a syntax error in both the production and dyno compilers"
  // It originated from test/users/thom/topLevelCode.chpl and causes some
  // discrepancy in the test result between dyno and production
  auto parseResult = parser->parseString("test3.chpl",
        "proc myProc();\n"
        "{\n //comment;\n"
        "}");

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 2);
  auto proc = mod->stmt(0)->toFunction();
  assert(proc);
  auto blk = mod->stmt(1)->toBlock();
  assert(blk);
  assert(blk->numStmts() == 1);
  auto comm = blk->stmt(0)->toComment();
  assert(comm);

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

  return 0;
}
