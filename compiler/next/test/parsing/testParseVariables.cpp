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
#include "chpl/uast/BoolLiteral.h"
#include "chpl/uast/Call.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Dot.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/OpCall.h"
#include "chpl/uast/PrimCall.h"
#include "chpl/uast/StringLiteral.h"
#include "chpl/uast/Variable.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace uast;
using namespace parsing;

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
                                         "var a: int;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto varDecl = mod->stmt(0)->toVariable();
  assert(varDecl);

  assert(varDecl->name() == "a");
  assert(!varDecl->isConfig());

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  assert(varDecl->initExpression() == nullptr);
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
                                         "var a = b;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto varDecl = mod->stmt(0)->toVariable();
  assert(varDecl);

  assert(varDecl->name() == "a");
  assert(!varDecl->isConfig());

  assert(varDecl->typeExpression() == nullptr);

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void test3(Parser* parser) {
  auto parseResult = parser->parseString("test3.chpl",
                                         "var a: int = b;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto varDecl = mod->stmt(0)->toVariable();
  assert(varDecl);

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void test3a(Parser* parser) {
  auto parseResult = parser->parseString("test3a.chpl",
                                         "var /* comment */ a: int = b;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto varDecl = mod->stmt(0)->toVariable();
  assert(varDecl);

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void test3b(Parser* parser) {
  auto parseResult = parser->parseString("test3b.chpl",
                                         "var a /* comment */ : int = b;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto varDecl = mod->stmt(0)->toVariable();
  assert(varDecl);

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void test3c(Parser* parser) {
  auto parseResult = parser->parseString("test3c.chpl",
                                         "var a : /* comment */ int = b;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto varDecl = mod->stmt(0)->toVariable();
  assert(varDecl);

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void test3d(Parser* parser) {
  auto parseResult = parser->parseString("test3d.chpl",
                                         "var a : int /* comment */ = b;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto varDecl = mod->stmt(0)->toVariable();
  assert(varDecl);

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void test3e(Parser* parser) {
  auto parseResult = parser->parseString("test3e.chpl",
                                         "var a : int = /* comment */ b;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto varDecl = mod->stmt(0)->toVariable();
  assert(varDecl);

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void test3f(Parser* parser) {
  auto parseResult = parser->parseString("test3f.chpl",
                                         "var a : int = b /* comment */;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto varDecl = mod->stmt(0)->toVariable();
  assert(varDecl);

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

static void test3g(Parser* parser) {
  auto parseResult = parser->parseString("test3g.chpl",
      "config var a : int = b /* comment */;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto varDecl = mod->stmt(0)->toVariable();
  assert(varDecl);

  assert(varDecl->isConfig());
  assert(varDecl->name() == "a");

  auto typeExpr = varDecl->typeExpression();
  assert(typeExpr);
  assert(typeExpr->isIdentifier());
  assert(0 == typeExpr->toIdentifier()->name().compare("int"));

  auto initExpr = varDecl->initExpression();
  assert(initExpr);
  assert(initExpr->isIdentifier());
  assert(0 == initExpr->toIdentifier()->name().compare("b"));
}

// Extern variables.
static void test4(Parser* parser) {
  auto parseResult = parser->parseString("test4.chpl",
    "extern var x: int = 0;\n"
    "public extern var y = 0.0;\n"
    "private extern \"foo\" ref z: int;\n");

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);

  auto var1 = mod->stmt(0)->toVariable();
  assert(var1);
  assert(var1->typeExpression() && var1->initExpression());
  assert(var1->kind() == Variable::VAR);
  assert(var1->linkage() == Decl::EXTERN);
  assert(!var1->linkageName());
  assert(var1->visibility() == Decl::DEFAULT_VISIBILITY);

  auto var2 = mod->stmt(1)->toVariable();
  assert(var2);
  assert(!var2->typeExpression() && var2->initExpression());
  assert(var2->kind() == Variable::VAR);
  assert(var2->linkage() == Decl::EXTERN);
  assert(!var2->linkageName());
  assert(var2->visibility() == Decl::PUBLIC);

  auto var3 = mod->stmt(2)->toVariable();
  assert(var3);
  assert(!var3->isConfig());
  assert(var3->kind() == Variable::REF);
  assert(var3->typeExpression() && !var3->initExpression());
  assert(var3->linkage() == Decl::EXTERN);
  assert(var3->linkageName() &&  var3->linkageName()->isStringLiteral());
  auto v3LinkageName = var3->linkageName()->toStringLiteral();
  assert(v3LinkageName->str() == "foo");
  assert(var3->visibility() == Decl::PRIVATE);
}

// TODO: Export variable (might be parse error).
static void test5(Parser* parser) {
  auto parseResult = parser->parseString("test5.chpl",
    "export var foo = 0;\n"
    "export \"bar\" var bar: real = 0.0;\n");

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  // TODO: More if not parse error.
}

// Type variables (also config and extern).
static void test6(Parser* parser) {
  auto parseResult = parser->parseString("test6.chpl",
    "public type foo = bar;\n"
    "type foo;\n"
    "public config type foo = bar;\n"
    "config type foo;\n"
    "extern type foo = bar;\n"
    "private extern type foo;\n");

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 6);

  for (auto stmt : mod->stmts()) {
    auto var = stmt->toVariable();
    assert(var && var->kind() == Variable::TYPE);
  }

  auto var1 = mod->stmt(0)->toVariable();
  assert(var1->visibility() == Decl::PUBLIC);
  assert(var1->linkage() == Decl::DEFAULT_LINKAGE);
  assert(!var1->linkageName());
  assert(var1->initExpression() && !var1->typeExpression());

  auto var2 = mod->stmt(1)->toVariable();
  assert(var2->visibility() == Decl::DEFAULT_VISIBILITY);
  assert(var2->linkage() == Decl::DEFAULT_LINKAGE);
  assert(!var2->linkageName());
  assert(!var2->initExpression() &&!var2->typeExpression());

  auto var3 = mod->stmt(2)->toVariable();
  assert(var3->visibility() == Decl::PUBLIC);
  assert(var3->linkage() == Decl::DEFAULT_LINKAGE);
  assert(!var3->linkageName());
  assert(var3->isConfig());
  assert(var3->initExpression() && !var3->typeExpression());

  auto var4 = mod->stmt(3)->toVariable();
  assert(var4->visibility() == Decl::DEFAULT_VISIBILITY);
  assert(var4->isConfig());
  assert(var4->linkage() == Decl::DEFAULT_LINKAGE);
  assert(!var4->linkageName());
  assert(!var4->initExpression() && !var4->typeExpression());

  auto var5 = mod->stmt(4)->toVariable();
  assert(var5->linkage() == Decl::EXTERN);
  assert(var5->initExpression());
  assert(!var5->linkageName());

  auto var6 = mod->stmt(5)->toVariable();
  assert(var6->visibility() == Decl::PRIVATE);
  assert(var6->linkage() == Decl::EXTERN);
  assert(!var6->initExpression());
  assert(!var6->linkageName());
}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  test1(p);
  test2(p);
  test3(p);
  test3a(p);
  test3b(p);
  test3c(p);
  test3d(p);
  test3e(p);
  test3f(p);
  test3g(p);
  test4(p);
  test5(p);
  test6(p);

  return 0;
}
