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
#include "chpl/uast/Enum.h"
#include "chpl/uast/EnumElement.h"
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

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
                                         "enum myEnum { a }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto enumDecl = module->stmt(0)->toEnum();
  assert(enumDecl);
  assert(enumDecl->numDeclOrComments() == 1);
  auto a = enumDecl->declOrComment(0)->toEnumElement();
  assert(a);
  assert(0 == a->name().compare("a"));
  assert(a->initExpression() == nullptr);
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
                                         "enum myEnum { a=ii }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto enumDecl = module->stmt(0)->toEnum();
  assert(enumDecl);
  assert(enumDecl->numDeclOrComments() == 1);
  auto a = enumDecl->declOrComment(0)->toEnumElement();
  assert(a);
  assert(0 == a->name().compare("a"));
  auto initId = a->initExpression()->toIdentifier();
  assert(initId);
  assert(0 == initId->name().compare("ii"));
}

static void test3(Parser* parser) {
  auto parseResult = parser->parseString("test3.chpl",
                                         "enum myEnum { a=ii, b=jj }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto enumDecl = module->stmt(0)->toEnum();
  assert(enumDecl);
  assert(enumDecl->numDeclOrComments() == 2);
  auto a = enumDecl->declOrComment(0)->toEnumElement();
  auto b = enumDecl->declOrComment(1)->toEnumElement();
  assert(a);
  assert(b);
  assert(0 == a->name().compare("a"));
  auto aInit = a->initExpression()->toIdentifier();
  assert(aInit);
  assert(0 == aInit->name().compare("ii"));
  auto bInit = b->initExpression()->toIdentifier();
  assert(bInit);
  assert(0 == bInit->name().compare("jj"));
}

static void checkTest4(const Enum* enumDecl,
                       const EnumElement* a,
                       const EnumElement* b);

static void test4(Parser* parser) {
  auto parseResult = parser->parseString("test4.chpl",
                                         "enum myEnum { a=ii, b }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto enumDecl = module->stmt(0)->toEnum();
  assert(enumDecl);
  assert(enumDecl->numDeclOrComments() == 2);
  auto a = enumDecl->declOrComment(0)->toEnumElement();
  auto b = enumDecl->declOrComment(1)->toEnumElement();
  checkTest4(enumDecl, a, b);
}

static void checkTest4(const Enum* enumDecl,
                       const EnumElement* a,
                       const EnumElement* b) {
  assert(a);
  assert(b);
  assert(0 == a->name().compare("a"));
  auto aInit = a->initExpression()->toIdentifier();
  assert(aInit);
  assert(0 == aInit->name().compare("ii"));
  assert(b->initExpression() == nullptr);

  // also check the enumElementDecls iterator
  int i = 0;
  for (auto elt : enumDecl->enumElements()) {
    if (0 == i) assert(elt == a);
    if (1 == i) assert(elt == b);
    if (2 <= i) assert(false);
    i++;
  }
  assert(2 == i);
}

static void test4a(Parser* parser) {
  auto parseResult = parser->parseString("test4a.chpl",
                                         "/* c */ enum myEnum { a=ii, b }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 2);
  assert(module->stmt(0)->isComment());
  auto enumDecl = module->stmt(1)->toEnum();
  assert(enumDecl);
  assert(enumDecl->numDeclOrComments() == 2);
  auto a = enumDecl->declOrComment(0)->toEnumElement();
  auto b = enumDecl->declOrComment(1)->toEnumElement();
  checkTest4(enumDecl, a, b);
}

static void test4b(Parser* parser) {
  auto parseResult = parser->parseString("test4b.chpl",
                                         "enum /* c */ myEnum { a=ii, b }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto enumDecl = module->stmt(0)->toEnum();
  assert(enumDecl);
  assert(enumDecl->numDeclOrComments() == 2);
  auto a = enumDecl->declOrComment(0)->toEnumElement();
  auto b = enumDecl->declOrComment(1)->toEnumElement();
  checkTest4(enumDecl, a, b);
}

static void test4c(Parser* parser) {
  auto parseResult = parser->parseString("test4c.chpl",
                                         "enum myEnum /* c */ { a=ii, b }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto enumDecl = module->stmt(0)->toEnum();
  assert(enumDecl);
  assert(enumDecl->numDeclOrComments() == 2);
  auto a = enumDecl->declOrComment(0)->toEnumElement();
  auto b = enumDecl->declOrComment(1)->toEnumElement();
  checkTest4(enumDecl, a, b);
}

static void test4d(Parser* parser) {
  auto parseResult = parser->parseString("test4d.chpl",
                                         "enum myEnum { /* c */ a=ii, b }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto enumDecl = module->stmt(0)->toEnum();
  assert(enumDecl);
  assert(enumDecl->numDeclOrComments() == 3);
  assert(enumDecl->declOrComment(0)->isComment());
  auto a = enumDecl->declOrComment(1)->toEnumElement();
  auto b = enumDecl->declOrComment(2)->toEnumElement();
  checkTest4(enumDecl, a, b);
}

static void test4e(Parser* parser) {
  auto parseResult = parser->parseString("test4e.chpl",
                                         "enum myEnum { a /* c */ =ii, b }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto enumDecl = module->stmt(0)->toEnum();
  assert(enumDecl);
  assert(enumDecl->numDeclOrComments() == 2);
  auto a = enumDecl->declOrComment(0)->toEnumElement();
  auto b = enumDecl->declOrComment(1)->toEnumElement();
  checkTest4(enumDecl, a, b);
}

static void test4f(Parser* parser) {
  auto parseResult = parser->parseString("test4f.chpl",
                                         "enum myEnum { a = /* c */ ii, b }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto enumDecl = module->stmt(0)->toEnum();
  assert(enumDecl);
  assert(enumDecl->numDeclOrComments() == 2);
  auto a = enumDecl->declOrComment(0)->toEnumElement();
  auto b = enumDecl->declOrComment(1)->toEnumElement();
  checkTest4(enumDecl, a, b);
}

static void test4g(Parser* parser) {
  auto parseResult = parser->parseString("test4g.chpl",
                                         "enum myEnum { a = ii /* c */, b }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto enumDecl = module->stmt(0)->toEnum();
  assert(enumDecl);
  assert(enumDecl->numDeclOrComments() == 2);
  auto a = enumDecl->declOrComment(0)->toEnumElement();
  auto b = enumDecl->declOrComment(1)->toEnumElement();
  checkTest4(enumDecl, a, b);
}

static void test4h(Parser* parser) {
  auto parseResult = parser->parseString("test4h.chpl",
                                         "enum myEnum { a = ii, /* c */ b }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto enumDecl = module->stmt(0)->toEnum();
  assert(enumDecl);
  assert(enumDecl->numDeclOrComments() == 3);
  auto a = enumDecl->declOrComment(0)->toEnumElement();
  assert(enumDecl->declOrComment(1)->isComment());
  auto b = enumDecl->declOrComment(2)->toEnumElement();
  checkTest4(enumDecl, a, b);
}

static void test4i(Parser* parser) {
  auto parseResult = parser->parseString("test4i.chpl",
                                         "enum myEnum { a = ii, b /* c */ }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto enumDecl = module->stmt(0)->toEnum();
  assert(enumDecl);
  assert(enumDecl->numDeclOrComments() == 3);
  auto a = enumDecl->declOrComment(0)->toEnumElement();
  auto b = enumDecl->declOrComment(1)->toEnumElement();
  assert(enumDecl->declOrComment(2)->isComment());
  checkTest4(enumDecl, a, b);
}

static void test4j(Parser* parser) {
  auto parseResult = parser->parseString("test4i.chpl",
                                         "enum myEnum { a = ii, b, /*c*/ }\n");
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto enumDecl = module->stmt(0)->toEnum();
  assert(enumDecl);
  assert(enumDecl->numDeclOrComments() == 3);
  auto a = enumDecl->declOrComment(0)->toEnumElement();
  auto b = enumDecl->declOrComment(1)->toEnumElement();
  assert(enumDecl->declOrComment(2)->isComment());
  checkTest4(enumDecl, a, b);
}


int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  test1(p);
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
