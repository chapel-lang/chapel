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
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"

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
                                         "x;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->kind() == Module::IMPLICIT);
  assert(module->name().compare("test0") == 0);
  assert(module->numStmts() == 1);
  auto ident = module->stmt(0)->toIdentifier();
  assert(ident);
  assert(0 == ident->name().compare("x"));
}

static void test0a(Parser* parser) {
  auto parseResult = parser->parseString("test0.chpl",
                                         "/* comment1 */\n"
                                         "x;\n"
                                         "/* comment2 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->kind() == Module::IMPLICIT);
  assert(module->name().compare("test0") == 0);
  assert(module->numStmts() == 3);
  assert(module->stmt(0)->isComment());
  assert(module->stmt(1)->isIdentifier());
  assert(module->stmt(2)->isComment());
}

static void test0b(Parser* parser) {
  auto parseResult = parser->parseString("test0.chpl",
                                         "/* comment1 */\n"
                                         "/* comment2 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->kind() == Module::IMPLICIT);
  assert(module->name().compare("test0") == 0);
  assert(module->numStmts() == 2);
  assert(module->stmt(0)->isComment());
  assert(module->stmt(1)->isComment());
}



static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
                                         "module M { x; }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->kind() == Module::DEFAULT_MODULE_KIND);
  assert(module->name().compare("M") == 0);
  assert(module->numStmts() == 1);
  auto ident = module->stmt(0)->toIdentifier();
  assert(ident);
  assert(0 == ident->name().compare("x"));
}

static void test1a(Parser* parser) {
  auto parseResult = parser->parseString("test1a.chpl",
                                         "/* comment 1 */\n"
                                         "module M {\n"
                                         "  /* comment 2 */\n"
                                         "  x;\n"
                                         "  /* comment 3 */\n"
                                         "}\n"
                                         "/* comment 4 */");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 3);
  assert(parseResult.topLevelExpressions[0]->isComment());
  assert(parseResult.topLevelExpressions[1]->isModule());
  assert(parseResult.topLevelExpressions[2]->isComment());
  auto module = parseResult.topLevelExpressions[1]->toModule();
  assert(module->kind() == Module::DEFAULT_MODULE_KIND);
  assert(module->name().compare("M") == 0);
  assert(module->numStmts() == 3);
  assert(module->stmt(0)->isComment());
  assert(module->stmt(1)->isIdentifier());
  assert(module->stmt(2)->isComment());
}

static void test1b(Parser* parser) {
  auto parseResult = parser->parseString("test1b.chpl",
                                         "module M {\n"
                                         "}\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->kind() == Module::DEFAULT_MODULE_KIND);
  assert(module->name().compare("M") == 0);
  assert(module->numStmts() == 0);
}

static void test1c(Parser* parser) {
  auto parseResult = parser->parseString("test1c.chpl",
                                         "/* comment 1 */\n"
                                         "module M {\n"
                                         "  /* comment 2 */\n"
                                         "  /* comment 3 */\n"
                                         "}\n"
                                         "/* comment 4 */");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 3);
  assert(parseResult.topLevelExpressions[0]->isComment());
  assert(parseResult.topLevelExpressions[1]->isModule());
  assert(parseResult.topLevelExpressions[2]->isComment());
  auto module = parseResult.topLevelExpressions[1]->toModule();
  assert(module->kind() == Module::DEFAULT_MODULE_KIND);
  assert(module->name().compare("M") == 0);
  assert(module->numStmts() == 2);
  assert(module->stmt(0)->isComment());
  assert(module->stmt(1)->isComment());
}

static void test1d(Parser* parser) {
  auto parseResult = parser->parseString("test1d.chpl",
                                         "module /* comment */ M {\n"
                                         "}\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->kind() == Module::DEFAULT_MODULE_KIND);
  assert(module->name().compare("M") == 0);
  assert(module->numStmts() == 0);
}

static void test1e(Parser* parser) {
  auto parseResult = parser->parseString("test1d.chpl",
                                         "prototype /* comment */ module M {\n"
                                         "}\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->kind() == Module::PROTOTYPE);
  assert(module->name().compare("M") == 0);
  assert(module->numStmts() == 0);
}

static void test1f(Parser* parser) {
  auto parseResult = parser->parseString("test1f.chpl",
                                         "public /* comment */ module M {\n"
                                         "}\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->kind() == Module::DEFAULT_MODULE_KIND);
  assert(module->name().compare("M") == 0);
  assert(module->numStmts() == 0);
}


static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
                                         "module M { x; }\n"
                                         "module N { y; }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 2);
  assert(parseResult.topLevelExpressions[0]->isModule());
  assert(parseResult.topLevelExpressions[1]->isModule());

  auto M = parseResult.topLevelExpressions[0]->toModule();
  assert(M);
  assert(M->kind() == Module::DEFAULT_MODULE_KIND);
  assert(M->name().compare("M") == 0);
  assert(M->numStmts() == 1);
  auto MIdent = M->stmt(0)->toIdentifier();
  assert(MIdent);
  assert(0 == MIdent->name().compare("x"));

  auto N = parseResult.topLevelExpressions[1]->toModule();
  assert(N);
  assert(N->kind() == Module::DEFAULT_MODULE_KIND);
  assert(N->name().compare("N") == 0);
  assert(N->numStmts() == 1);
  auto NIdent = N->stmt(0)->toIdentifier();
  assert(NIdent);
  assert(0 == NIdent->name().compare("y"));
}

static void test2a(Parser* parser) {
  auto parseResult = parser->parseString("test2a.chpl",
                                         "/* comment 1 */\n"
                                         "module M {\n"
                                         "  /* comment 2 */\n"
                                         "  x;\n"
                                         "  /* comment 3 */\n"
                                         "}\n"
                                         "/* comment 4 */\n"
                                         "module N {\n"
                                         "  /* comment 5 */\n"
                                         "  /* comment 6 */\n"
                                         "  y;\n"
                                         "  /* comment 7 */\n"
                                         "  /* comment 8 */\n"
                                         "  z;\n"
                                         "  /* comment 9 */\n"
                                         "  /* comment 10 */\n"
                                         "}\n"
                                         "/* comment 11 */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 5);
  assert(parseResult.topLevelExpressions[0]->isComment());
  assert(parseResult.topLevelExpressions[1]->isModule());
  assert(parseResult.topLevelExpressions[2]->isComment());
  assert(parseResult.topLevelExpressions[3]->isModule());
  assert(parseResult.topLevelExpressions[4]->isComment());

  auto M = parseResult.topLevelExpressions[1]->toModule();
  assert(M);
  assert(M->kind() == Module::DEFAULT_MODULE_KIND);
  assert(M->name().compare("M") == 0);
  assert(M->numStmts() == 3);
  assert(M->stmt(0)->isComment());
  assert(M->stmt(1)->isIdentifier());
  assert(M->stmt(2)->isComment());

  auto N = parseResult.topLevelExpressions[3]->toModule();
  assert(N);
  assert(N->kind() == Module::DEFAULT_MODULE_KIND);
  assert(N->name().compare("N") == 0);
  assert(N->numStmts() == 8);
  assert(N->stmt(0)->isComment());
  assert(N->stmt(1)->isComment());
  assert(N->stmt(2)->isIdentifier());
  assert(N->stmt(3)->isComment());
  assert(N->stmt(4)->isComment());
  assert(N->stmt(5)->isIdentifier());
  assert(N->stmt(6)->isComment());
  assert(N->stmt(7)->isComment());
}

static void test2b(Parser* parser) {
  auto parseResult = parser->parseString("test2b.chpl",
                                         "/* comment */\n"
                                         "module M { x; }\n"
                                         "module N { y; }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 3);
  assert(parseResult.topLevelExpressions[0]->isComment());
  assert(parseResult.topLevelExpressions[1]->isModule());
  assert(parseResult.topLevelExpressions[2]->isModule());
}

static void test2c(Parser* parser) {
  auto parseResult = parser->parseString("test2c.chpl",
                                         "module M { x; }\n"
                                         "/* comment */\n"
                                         "module N { y; }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 3);
  assert(parseResult.topLevelExpressions[0]->isModule());
  assert(parseResult.topLevelExpressions[1]->isComment());
  assert(parseResult.topLevelExpressions[2]->isModule());
}

static void test2d(Parser* parser) {
  auto parseResult = parser->parseString("test2d.chpl",
                                         "module M { x; }\n"
                                         "module N { y; }\n"
                                         "/* comment */\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 3);
  assert(parseResult.topLevelExpressions[0]->isModule());
  assert(parseResult.topLevelExpressions[1]->isModule());
  assert(parseResult.topLevelExpressions[2]->isComment());
}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  test0(p);
  test0a(p);
  test0b(p);
  test1(p);
  test1a(p);
  test1b(p);
  test1c(p);
  test1d(p);
  test1e(p);
  test1f(p);
  test2(p);
  test2a(p);
  test2b(p);
  test2c(p);
  test2d(p);

  return 0;
}
