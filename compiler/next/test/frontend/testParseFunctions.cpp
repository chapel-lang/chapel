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

#include "chpl/frontend/Parser.h"
#include "chpl/queries/Context.h"
#include "chpl/uast/Call.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/FormalDecl.h"
#include "chpl/uast/Function.h"
#include "chpl/uast/FunctionDecl.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/ModuleDecl.h"
#include "chpl/uast/OpCall.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace uast;
using namespace frontend;

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
                                         "proc f() { }");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModuleDecl());
  auto module = parseResult.topLevelExpressions[0]->toModuleDecl()->module();
  assert(module->name().compare("test1") == 0);
  assert(module->numStmts() == 1);
  auto functionDecl = module->stmt(0)->toFunctionDecl();
  assert(functionDecl);
  assert(functionDecl->name().compare("f") == 0);
  assert(functionDecl->linkage() == Function::DEFAULT_LINKAGE);
  assert(functionDecl->kind() == Function::PROC);
  assert(functionDecl->returnIntent() == Function::DEFAULT_RETURN_INTENT);
  assert(functionDecl->isInline() == false);
  assert(functionDecl->isOverride() == false);
  assert(functionDecl->throws() == false);
  assert(functionDecl->linkageNameExpression() == nullptr);
  assert(functionDecl->numFormals() == 0);
  assert(functionDecl->thisFormal() == nullptr);
  assert(functionDecl->returnType() == nullptr);
  assert(functionDecl->numLifetimeClauses() == 0);
  assert(functionDecl->numStmts() == 0);
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
                                         "inline proc f(a: int) ref { x; }");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModuleDecl());
  auto module = parseResult.topLevelExpressions[0]->toModuleDecl()->module();
  assert(module->name().compare("test2") == 0);
  assert(module->numStmts() == 1);
  auto functionDecl = module->stmt(0)->toFunctionDecl();
  assert(functionDecl);
  assert(functionDecl->name().compare("f") == 0);
  assert(functionDecl->linkage() == Function::DEFAULT_LINKAGE);
  assert(functionDecl->kind() == Function::PROC);
  assert(functionDecl->returnIntent() == Function::REF);
  assert(functionDecl->isInline() == true);
  assert(functionDecl->isOverride() == false);
  assert(functionDecl->throws() == false);
  assert(functionDecl->linkageNameExpression() == nullptr);
  assert(functionDecl->numFormals() == 1);
  auto formal = functionDecl->formal(0);
  assert(formal);
  assert(formal->intent() == Formal::DEFAULT_INTENT);
  assert(formal->name().compare("a") == 0);
  auto typeExpr = formal->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(typeExpr->toIdentifier()->name().compare("int") == 0);
  assert(formal->initExpression() == nullptr);
  assert(functionDecl->thisFormal() == nullptr);
  assert(functionDecl->returnType() == nullptr);
  assert(functionDecl->numLifetimeClauses() == 0);
  assert(functionDecl->numStmts() == 1);
  auto stmt = functionDecl->stmt(0);
  assert(stmt);
  assert(stmt->isIdentifier());
  assert(stmt->toIdentifier()->name().compare("x") == 0);
}

static void test3(Parser* parser) {
  auto parseResult = parser->parseString( "test3.chpl",
               "override proc const R.f(ref a: int = b) const ref { }");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModuleDecl());
  auto module = parseResult.topLevelExpressions[0]->toModuleDecl()->module();
  assert(module->name().compare("test3") == 0);
  assert(module->numStmts() == 1);
  auto functionDecl = module->stmt(0)->toFunctionDecl();
  assert(functionDecl);
  assert(functionDecl->name().compare("f") == 0);
  assert(functionDecl->linkage() == Function::DEFAULT_LINKAGE);
  assert(functionDecl->kind() == Function::PROC);
  assert(functionDecl->returnIntent() == Function::CONST_REF);
  assert(functionDecl->isInline() == false);
  assert(functionDecl->isOverride() == true);
  assert(functionDecl->throws() == false);
  assert(functionDecl->linkageNameExpression() == nullptr);
  assert(functionDecl->numFormals() == 2); // 'this' and 'a'

  auto thisFormal = functionDecl->formal(0);
  assert(thisFormal);
  assert(thisFormal->intent() == Formal::CONST);
  assert(thisFormal->name().compare("this") == 0);
  auto thisTypeExpr = thisFormal->typeExpression();
  assert(thisTypeExpr);
  assert(thisTypeExpr->isIdentifier());
  assert(thisTypeExpr->toIdentifier()->name().compare("R") == 0);
  assert(thisFormal->initExpression() == nullptr);

  auto formal = functionDecl->formal(1);
  assert(formal);
  assert(formal->intent() == Formal::REF);
  assert(formal->name().compare("a") == 0);
  auto typeExpr = formal->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(typeExpr->toIdentifier()->name().compare("int") == 0);
  auto initExpr = formal->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(initExpr->toIdentifier()->name().compare("b") == 0);

  assert(functionDecl->thisFormal() == thisFormal);
  assert(functionDecl->returnType() == nullptr);
  assert(functionDecl->numLifetimeClauses() == 0);
  assert(functionDecl->numStmts() == 0);
}



int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  test1(p);
  test2(p);
  test3(p);

  return 0;
}
