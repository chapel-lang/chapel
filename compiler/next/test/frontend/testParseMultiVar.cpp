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
#include "chpl/uast/Module.h"
#include "chpl/uast/MultiDecl.h"
#include "chpl/uast/Variable.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace uast;
using namespace frontend;

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl", "var x, y;");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto multi = module->stmt(0)->toMultiDecl();
  assert(multi->numDeclOrComments() == 2);
  auto x = multi->declOrComment(0)->toVariable();
  auto y = multi->declOrComment(1)->toVariable();
  assert(0 == x->name().compare("x"));
  assert(x->typeExpression() == nullptr);
  assert(x->initExpression() == nullptr);
  assert(x->kind() == Variable::VAR);
  assert(0 == y->name().compare("y"));
  assert(y->typeExpression() == nullptr);
  assert(y->initExpression() == nullptr);
  assert(y->kind() == Variable::VAR);

  // check the no-comments iterator
  int i = 0;
  for (auto decl : multi->decls()) {
    if (0 == i) assert(decl == x);
    if (1 == i) assert(decl == y);
    if (2 <= i) assert(false);
    i++;
  }
  assert(i == 2);
}
static void test1a(Parser* parser) {
  auto parseResult = parser->parseString("test1a.chpl", "const x, y;");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto multi = module->stmt(0)->toMultiDecl();
  assert(multi->numDeclOrComments() == 2);
  auto x = multi->declOrComment(0)->toVariable();
  auto y = multi->declOrComment(1)->toVariable();
  assert(0 == x->name().compare("x"));
  assert(x->typeExpression() == nullptr);
  assert(x->initExpression() == nullptr);
  assert(x->kind() == Variable::CONST);
  assert(0 == y->name().compare("y"));
  assert(y->typeExpression() == nullptr);
  assert(y->initExpression() == nullptr);
  assert(y->kind() == Variable::CONST);
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl", "var x, y = ii;");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto multi = module->stmt(0)->toMultiDecl();
  assert(multi->numDeclOrComments() == 2);
  auto x = multi->declOrComment(0)->toVariable();
  auto y = multi->declOrComment(1)->toVariable();
  assert(0 == x->name().compare("x"));
  assert(x->typeExpression() == nullptr);
  assert(x->initExpression() == nullptr);
  assert(0 == y->name().compare("y"));
  assert(y->typeExpression() == nullptr);
  assert(y->initExpression()->isIdentifier());

  // check the no-comments iterator
  int i = 0;
  for (auto decl : multi->decls()) {
    if (0 == i) assert(decl == x);
    if (1 == i) assert(decl == y);
    if (2 <= i) assert(false);
    i++;
  }
  assert(i == 2);
}

static void test3(Parser* parser) {
  auto parseResult = parser->parseString("test3.chpl", "var x, y: int;");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto multi = module->stmt(0)->toMultiDecl();
  assert(multi->numDeclOrComments() == 2);
  auto x = multi->declOrComment(0)->toVariable();
  auto y = multi->declOrComment(1)->toVariable();
  assert(0 == x->name().compare("x"));
  assert(x->typeExpression() == nullptr);
  assert(x->initExpression() == nullptr);
  assert(0 == y->name().compare("y"));
  assert(y->typeExpression()->isIdentifier());
  assert(y->initExpression() == nullptr);

  // check the no-comments iterator
  int i = 0;
  for (auto decl : multi->decls()) {
    if (0 == i) assert(decl == x);
    if (1 == i) assert(decl == y);
    if (2 <= i) assert(false);
    i++;
  }
  assert(i == 2);
}

static void checkTest4Decls(const MultiDecl* multi,
                            const Variable* a, const Variable* b,
                            const Variable* c, const Variable* d);

static void test4(Parser* parser) {
  auto parseResult = parser->parseString("test4.chpl",
                                         "var a,\n"
                                         "    b: int,\n"
                                         "    c,\n"
                                         "    d = ii;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto multi = module->stmt(0)->toMultiDecl();
  assert(multi->numDeclOrComments() == 4);
  auto a = multi->declOrComment(0)->toVariable();
  auto b = multi->declOrComment(1)->toVariable();
  auto c = multi->declOrComment(2)->toVariable();
  auto d = multi->declOrComment(3)->toVariable();
  checkTest4Decls(multi, a, b, c, d);
}

static void checkTest4Decls(const MultiDecl* multi,
                            const Variable* a, const Variable* b,
                            const Variable* c, const Variable* d)
{
  assert(multi);
  assert(a);
  assert(b);
  assert(c);
  assert(d);
  assert(0 == a->name().compare("a"));
  assert(a->typeExpression() == nullptr);
  assert(a->initExpression() == nullptr);
  assert(0 == b->name().compare("b"));
  assert(b->typeExpression()->isIdentifier());
  assert(b->initExpression() == nullptr);
  assert(0 == c->name().compare("c"));
  assert(c->typeExpression() == nullptr);
  assert(c->initExpression() == nullptr);
  assert(0 == d->name().compare("d"));
  assert(d->typeExpression() == nullptr);
  assert(d->initExpression()->isIdentifier());

  // check the no-comments iterator
  int i = 0;
  for (auto decl : multi->decls()) {
    if (0 == i) assert(decl == a);
    if (1 == i) assert(decl == b);
    if (2 == i) assert(decl == c);
    if (3 == i) assert(decl == d);
    if (4 <= i) assert(false);
    i++;
  }
  assert(i == 4);
}

static void test4a(Parser* parser) {
  auto parseResult = parser->parseString("test4a.chpl",
                                         "/* comment */\n"
                                         "var a,\n"
                                         "    b: int,\n"
                                         "    c,\n"
                                         "    d = ii;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 2);
  assert(module->stmt(0)->isComment());
  auto multi = module->stmt(1)->toMultiDecl();
  assert(multi->numDeclOrComments() == 4);
  auto a = multi->declOrComment(0)->toVariable();
  auto b = multi->declOrComment(1)->toVariable();
  auto c = multi->declOrComment(2)->toVariable();
  auto d = multi->declOrComment(3)->toVariable();
  checkTest4Decls(multi, a, b, c, d);
}

static void test4b(Parser* parser) {
  auto parseResult = parser->parseString("test4b.chpl",
                                         "var /*comment*/ a,\n"
                                         "    b: int,\n"
                                         "    c,\n"
                                         "    d = ii;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto multi = module->stmt(0)->toMultiDecl();
  assert(multi->numDeclOrComments() == 5);
  assert(multi->declOrComment(0)->isComment());
  auto a = multi->declOrComment(1)->toVariable();
  auto b = multi->declOrComment(2)->toVariable();
  auto c = multi->declOrComment(3)->toVariable();
  auto d = multi->declOrComment(4)->toVariable();
  checkTest4Decls(multi, a, b, c, d);
}

static void test4c(Parser* parser) {
  auto parseResult = parser->parseString("test4c.chpl",
                                         "var a,\n"
                                         "    /*comment*/ b: int,\n"
                                         "    c,\n"
                                         "    d = ii;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto multi = module->stmt(0)->toMultiDecl();
  assert(multi->numDeclOrComments() == 5);
  auto a = multi->declOrComment(0)->toVariable();
  assert(multi->declOrComment(1)->isComment());
  auto b = multi->declOrComment(2)->toVariable();
  auto c = multi->declOrComment(3)->toVariable();
  auto d = multi->declOrComment(4)->toVariable();
  checkTest4Decls(multi, a, b, c, d);
}

static void test4d(Parser* parser) {
  auto parseResult = parser->parseString("test4d.chpl",
                                         "var a,\n"
                                         "    b /* comment */ : int,\n"
                                         "    c,\n"
                                         "    d = ii;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto multi = module->stmt(0)->toMultiDecl();
  assert(multi->numDeclOrComments() == 4);
  auto a = multi->declOrComment(0)->toVariable();
  auto b = multi->declOrComment(1)->toVariable();
  auto c = multi->declOrComment(2)->toVariable();
  auto d = multi->declOrComment(3)->toVariable();
  checkTest4Decls(multi, a, b, c, d);
}

static void test4e(Parser* parser) {
  auto parseResult = parser->parseString("test4e.chpl",
                                         "var a,\n"
                                         "    b : /* comment */ int,\n"
                                         "    c,\n"
                                         "    d = ii;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto multi = module->stmt(0)->toMultiDecl();
  assert(multi->numDeclOrComments() == 4);
  auto a = multi->declOrComment(0)->toVariable();
  auto b = multi->declOrComment(1)->toVariable();
  auto c = multi->declOrComment(2)->toVariable();
  auto d = multi->declOrComment(3)->toVariable();
  checkTest4Decls(multi, a, b, c, d);
}

static void test4f(Parser* parser) {
  auto parseResult = parser->parseString("test4f.chpl",
                                         "var a,\n"
                                         "    b : int,\n"
                                         "    /*comment */ c,\n"
                                         "    d = ii;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto multi = module->stmt(0)->toMultiDecl();
  assert(multi->numDeclOrComments() == 5);
  auto a = multi->declOrComment(0)->toVariable();
  auto b = multi->declOrComment(1)->toVariable();
  assert(multi->declOrComment(2)->isComment());
  auto c = multi->declOrComment(3)->toVariable();
  auto d = multi->declOrComment(4)->toVariable();
  checkTest4Decls(multi, a, b, c, d);
}

static void test4g(Parser* parser) {
  auto parseResult = parser->parseString("test4g.chpl",
                                         "var a,\n"
                                         "    b : int,\n"
                                         "    c,\n"
                                         "    /* comment */ d = ii;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto multi = module->stmt(0)->toMultiDecl();
  assert(multi->numDeclOrComments() == 5);
  auto a = multi->declOrComment(0)->toVariable();
  auto b = multi->declOrComment(1)->toVariable();
  auto c = multi->declOrComment(2)->toVariable();
  assert(multi->declOrComment(3)->isComment());
  auto d = multi->declOrComment(4)->toVariable();
  checkTest4Decls(multi, a, b, c, d);
}

static void test4h(Parser* parser) {
  auto parseResult = parser->parseString("test4h.chpl",
                                         "var a,\n"
                                         "    b : int,\n"
                                         "    c,\n"
                                         "    d /* comment */ = ii;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto multi = module->stmt(0)->toMultiDecl();
  assert(multi->numDeclOrComments() == 4);
  auto a = multi->declOrComment(0)->toVariable();
  auto b = multi->declOrComment(1)->toVariable();
  auto c = multi->declOrComment(2)->toVariable();
  auto d = multi->declOrComment(3)->toVariable();
  checkTest4Decls(multi, a, b, c, d);
}

static void test4i(Parser* parser) {
  auto parseResult = parser->parseString("test4i.chpl",
                                         "var a,\n"
                                         "    b : int,\n"
                                         "    c,\n"
                                         "    d = /* comment */ ii;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto multi = module->stmt(0)->toMultiDecl();
  assert(multi->numDeclOrComments() == 4);
  auto a = multi->declOrComment(0)->toVariable();
  auto b = multi->declOrComment(1)->toVariable();
  auto c = multi->declOrComment(2)->toVariable();
  auto d = multi->declOrComment(3)->toVariable();
  checkTest4Decls(multi, a, b, c, d);
}

static void test4j(Parser* parser) {
  auto parseResult = parser->parseString("test4j.chpl",
                                         "var a,\n"
                                         "    b : int,\n"
                                         "    c,\n"
                                         "    d = ii /* comment */ ;\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto multi = module->stmt(0)->toMultiDecl();
  assert(multi->numDeclOrComments() == 4);
  auto a = multi->declOrComment(0)->toVariable();
  auto b = multi->declOrComment(1)->toVariable();
  auto c = multi->declOrComment(2)->toVariable();
  auto d = multi->declOrComment(3)->toVariable();
  checkTest4Decls(multi, a, b, c, d);
}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  test1(p);
  test1a(p);
  test2(p);
  test3(p);
  test4(p);
  test4a(p);
  test4b(p);
  test4c(p);
  test4d(p);
  test4e(p);
  test4f(p);
  test4g(p);
  test4h(p);
  test4i(p);
  test4j(p);

  return 0;
}
