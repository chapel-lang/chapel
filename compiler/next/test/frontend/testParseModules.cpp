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

static void test0(Parser* parser) {
  auto parseResult = parser->parseString("test0.chpl",
                                         "x;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModuleDecl());
  auto module = parseResult.topLevelExpressions[0]->toModuleDecl()->module();
  assert(module->kind() == Module::IMPLICIT);
  assert(module->name().compare("test0") == 0);
  assert(module->numStmts() == 1);
  auto ident = module->stmt(0)->toIdentifier();
  assert(ident);
  assert(0 == ident->name().compare("x"));
}


static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
                                         "module M { x; }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModuleDecl());
  auto module = parseResult.topLevelExpressions[0]->toModuleDecl()->module();
  assert(module->kind() == Module::DEFAULT_MODULE_KIND);
  assert(module->name().compare("M") == 0);
  assert(module->numStmts() == 1);
  auto ident = module->stmt(0)->toIdentifier();
  assert(ident);
  assert(0 == ident->name().compare("x"));
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
                                         "module M { x; }\n"
                                         "module N { y; }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 2);
  assert(parseResult.topLevelExpressions[0]->isModuleDecl());
  assert(parseResult.topLevelExpressions[1]->isModuleDecl());

  auto M = parseResult.topLevelExpressions[0]->toModuleDecl()->module();
  assert(M);
  assert(M->kind() == Module::DEFAULT_MODULE_KIND);
  assert(M->name().compare("M") == 0);
  assert(M->numStmts() == 1);
  auto MIdent = M->stmt(0)->toIdentifier();
  assert(MIdent);
  assert(0 == MIdent->name().compare("x"));

  auto N = parseResult.topLevelExpressions[1]->toModuleDecl()->module();
  assert(N);
  assert(N->kind() == Module::DEFAULT_MODULE_KIND);
  assert(N->name().compare("N") == 0);
  assert(N->numStmts() == 1);
  auto NIdent = N->stmt(0)->toIdentifier();
  assert(NIdent);
  assert(0 == NIdent->name().compare("y"));
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
