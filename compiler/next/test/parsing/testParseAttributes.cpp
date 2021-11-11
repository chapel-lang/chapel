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
#include "chpl/uast/all-uast.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace uast;
using namespace parsing;

static bool areAttributesEqual(const Decl* lhs, const Decl* rhs) {
  auto lhsAttr = lhs->attributes();
  auto rhsAttr = rhs->attributes();

  if (!lhsAttr && !rhsAttr) return true;
  if (!lhsAttr || !rhsAttr) return false;

  bool equalPragmas = true;
  for (PragmaTag pragma : lhsAttr->pragmas()) {
    if (!rhsAttr->hasPragma(pragma)) {
      equalPragmas = false;
      break;
    }
  }

  if (equalPragmas) {
    for (PragmaTag pragma : rhsAttr->pragmas()) {
      if (!lhsAttr->hasPragma(pragma)) {
        equalPragmas = false;
        break;
      }
    }
  }

  return lhsAttr->isDeprecated() == rhsAttr->isDeprecated() &&
    lhsAttr->deprecationMessage() == rhsAttr->deprecationMessage() &&
    equalPragmas;
}

// Make sure MultiDecl attributes equal component attributes.
static void test0(Parser* parser) {
  auto parseResult = parser->parseString("test0.chpl",
      "pragma \"no doc\"\n"
      "var a, b, c = 0;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto multiVar = mod->stmt(0)->toMultiDecl();
  assert(multiVar);

  auto parentAttr = multiVar->attributes();
  assert(parentAttr);
  assert(!parentAttr->isDeprecated());
  assert(parentAttr->deprecationMessage().isEmpty());
  assert(parentAttr->hasPragma(PRAGMA_NO_DOC));

  for (auto decl : multiVar->decls()) {
    assert(decl->isVariable());
    assert(areAttributesEqual(multiVar, decl));
  }
}

// Make sure TupleDecl attributes equal component attributes.
static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
      "pragma \"no doc\"\n"
      "var (a, b, c) = foo;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto tupleVar = mod->stmt(0)->toTupleDecl();
  assert(tupleVar);

  auto parentAttr = tupleVar->attributes();
  assert(parentAttr);
  assert(!parentAttr->isDeprecated());
  assert(parentAttr->deprecationMessage().isEmpty());
  assert(parentAttr->hasPragma(PRAGMA_NO_DOC));

  for (auto decl : tupleVar->decls()) {
    assert(decl->isVariable());
    assert(areAttributesEqual(tupleVar, decl));
  }
}

static PragmaTag genPragmaTag(int idx) {
  auto ret = (PragmaTag)((idx+1) % NUM_KNOWN_PRAGMAS);
  return ret;
}

static const char* genPragma(int idx) {
  auto tag = genPragmaTag(idx);
  auto ret = pragmaTagToName(tag);
  return ret;
}

static std::string genAggregateAttributesTest(asttags::ASTTag aggKind,
                                              bool isDeprecated,
                                              bool hasDeprecationMsg,
                                              int numPragmas,
                                              bool doChildrenHavePragmas,
                                              int numChildVars) {
  std::string ret;

  if (numPragmas > 0) {
    for (int i = 0; i < numPragmas; i++) {
      ret += "pragma \"";
      ret += genPragma(i);
      ret += "\"\n";
    }
  }

  if (isDeprecated) {
    ret += "deprecated";

    if (hasDeprecationMsg) {
      ret += " ";
      ret += "\"This thing is deprecated\"";
    }

    ret += "\n";
  }

  switch(aggKind) {
    case asttags::Class: ret += "class"; break;
    case asttags::Record: ret += "record"; break;
    case asttags::Union: ret += "union"; break;
    default: assert(false);
  }

  ret += " C {";

  assert(numChildVars >= 0);

  for (int i = 0; i < numChildVars; i++) {
    if (doChildrenHavePragmas) {
      ret += "pragma \"";
      ret += genPragma(i);
      ret += "\"\n";
    }

    ret += "var v";
    ret += std::to_string(i);
    ret += " = 0;\n";
  }

  ret += "}\n";

  return ret;
}

static void testAggregateAttributes(Parser* parser,
                                    asttags::ASTTag aggKind,
                                    bool isDeprecated,
                                    bool hasDeprecationMsg,
                                    int numPragmas,
                                    bool doChildrenHavePragmas,
                                    int numChildVars) {
  auto code = genAggregateAttributesTest(aggKind, isDeprecated,
                                         hasDeprecationMsg,
                                         numPragmas,
                                         doChildrenHavePragmas,
                                         numChildVars);
  auto parseResult = parser->parseString("test-aggregate.chpl",
                                         code.c_str());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto agg = mod->stmt(0)->toAggregateDecl();
  assert(agg);

  assert(agg->tag() == aggKind);
  auto attr = agg->attributes();

  if (isDeprecated) {
    assert(attr);
    assert(attr->isDeprecated());
    auto attrDepMsg = attr->deprecationMessage();
    assert(attrDepMsg.isEmpty() == !hasDeprecationMsg);
    if (hasDeprecationMsg) {
      assert(attrDepMsg == "This thing is deprecated");
    }
  }

  if (numPragmas > 0) {
    assert(attr);

    for (int i = 0; i < numPragmas; i++) {
      auto tag = genPragmaTag(i);
      assert(attr->hasPragma(tag));
    }
  }

  assert(agg->numDeclOrComments() == numChildVars);

  for (int i = 0; i < numChildVars; i++) {
    auto var = agg->declOrComment(i)->toVariable();
    assert(var);

    // Important to make sure the attributes are not incorrectly carried
    // down to children here.
    assert(!areAttributesEqual(var, agg));

    auto varAttr = var->attributes();

    if (doChildrenHavePragmas) {
      assert(varAttr);
      auto tag = genPragmaTag(i);
      assert(varAttr->hasPragma(tag));
    } else {
      assert(!varAttr);
    }
  }
}

// Make sure attributes for aggregates get attached properly.
static void test2(Parser* parser) {
  testAggregateAttributes(parser, /*aggKind*/ asttags::Class,
                          /*isDeprecated*/ false,
                          /*hasDeprecationMsg*/ false,
                          /*numPragmas*/ 0,
                          /*doChildrenHavePragmas*/ false,
                          /*numChildVars*/ 0);
  testAggregateAttributes(parser, /*aggKind*/ asttags::Record,
                          /*isDeprecated*/ true,
                          /*hasDeprecationMsg*/ false,
                          /*numPragmas*/ 8,
                          /*doChildrenHavePragmas*/ false,
                          /*numChildVars*/ 1);
  testAggregateAttributes(parser, /*aggKind*/ asttags::Union,
                          /*isDeprecated*/ true,
                          /*hasDeprecationMsg*/ true,
                          /*numPragmas*/ 0,
                          /*doChildrenHavePragmas*/ true,
                          /*numChildVars*/ 3);
  testAggregateAttributes(parser, /*aggKind*/ asttags::Class,
                          /*isDeprecated*/ true,
                          /*hasDeprecationMsg*/ true,
                          /*numPragmas*/ 0,
                          /*doChildrenHavePragmas*/ true,
                          /*numChildVars*/ 4);
  testAggregateAttributes(parser, /*aggKind*/ asttags::Record,
                          /*isDeprecated*/ true,
                          /*hasDeprecationMsg*/ true,
                          /*numPragmas*/ 16,
                          /*doChildrenHavePragmas*/ true,
                          /*numChildVars*/ 1);
  testAggregateAttributes(parser, /*aggKind*/ asttags::Union,
                          /*isDeprecated*/ false,
                          /*hasDeprecationMsg*/ false,
                          /*numPragmas*/ 8,
                          /*doChildrenHavePragmas*/ false,
                          /*numChildVars*/ 0);
  testAggregateAttributes(parser, /*aggKind*/ asttags::Class,
                          /*isDeprecated*/ true,
                          /*hasDeprecationMsg*/ false,
                          /*numPragmas*/ 128,
                          /*doChildrenHavePragmas*/ true,
                          /*numChildVars*/ 64);
}

// Simple test for deprecation message on a variable.
static void test3(Parser* parser) {
  auto parseResult = parser->parseString("test3.chpl",
      "pragma \"no doc\"\n"
      "deprecated \"Thingy is deprecated\"\n"
      "var x = 0;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto var = mod->stmt(0)->toVariable();
  assert(var);

  auto varAttr = var->attributes();
  assert(varAttr);

  assert(varAttr->hasPragma(PRAGMA_NO_DOC));
  assert(varAttr->isDeprecated());
  assert(varAttr->deprecationMessage() == "Thingy is deprecated");
}

// Module, enum, and a var decl.
static void test4(Parser* parser) {
  auto parseResult = parser->parseString("test4.chpl",
      "pragma \"no doc\"\n"
      "deprecated \"Module is deprecated\"\n"
      "module Foo {\n"
      "  var x = 0;\n"
      "  deprecated \"Enum is deprecated\"\n"
      "  enum Y { a, b, c }\n"
      "  pragma \"ref\"\n"
      "  var y = 0;\n"
      "}\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);

  auto modAttr = mod->attributes();
  assert(modAttr);
  assert(modAttr->isDeprecated());
  assert(modAttr->deprecationMessage() == "Module is deprecated");
  assert(modAttr->hasPragma(PRAGMA_NO_DOC));
}

// Procedures, formals, nested procedures.
static void test5(Parser* parser) {
  auto parseResult = parser->parseString("test5.chpl",
      "pragma \"no doc\"\n"
      "deprecated \"P1 is deprecated\"\n"
      "proc p1() {}\n"
      "proc p2() {}\n"
      "pragma \"ref\"\n"
      "proc p3() {\n"
      "  proc n1() {}\n"
      "  pragma \"no init\"\n"
      "  deprecated \"N2 is deprecated\"\n"
      "  proc n2() {}\n"
      "}\n"
      "pragma \"no doc\"\n"
      "deprecated \"P4 is deprecated\"\n"
      "proc p4(pragma \"no init\" x, y) {}\n"
      "deprecated \"P5 is deprecated\"\n"
      "proc p5(x, pragma \"no init\" y) {}\n"
      "pragma \"no doc\"\n"
      "proc p6() { var x = 0; }\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 6);

  auto p1 = mod->stmt(0)->toFunction();
  assert(p1);
  auto p1Attr = p1->attributes();
  assert(p1Attr);
  assert(p1Attr->isDeprecated());
  assert(p1Attr->deprecationMessage() == "P1 is deprecated");
  assert(p1Attr->hasPragma(PRAGMA_NO_DOC));

  auto p2 = mod->stmt(1)->toFunction();
  assert(!p2->attributes());

  auto p3 = mod->stmt(2)->toFunction();
  assert(p3);
  auto p3Attr = p3->attributes();
  assert(p3Attr);
  assert(!p3Attr->isDeprecated());
  assert(p3Attr->deprecationMessage().isEmpty());
  assert(p3Attr->hasPragma(PRAGMA_REF));

  assert(p3->numStmts() == 2);
  auto p3n1 = p3->stmt(0)->toFunction();
  assert(p3n1);
  assert(!p3n1->attributes());
  auto p3n2 = p3->stmt(1)->toFunction();
  assert(p3n2);
  auto p3n2Attr = p3n2->attributes();
  assert(p3n2Attr);
  assert(p3n2Attr->isDeprecated());
  assert(p3n2Attr->deprecationMessage() == "N2 is deprecated");
  assert(p3n2Attr->hasPragma(PRAGMA_NO_INIT));

  auto p4 = mod->stmt(3)->toFunction();
  assert(p4);
  auto p4Attr = p4->attributes();
  assert(p4Attr);
  assert(p4Attr->isDeprecated());
  assert(p4Attr->deprecationMessage() == "P4 is deprecated");
  assert(p4Attr->hasPragma(PRAGMA_NO_DOC));
  assert(p4->numFormals() == 2);
  auto p4f1 = p4->formal(0)->toFormal();
  assert(p4f1);
  auto p4f1Attr = p4f1->attributes();
  assert(p4f1Attr);
  assert(!p4f1Attr->isDeprecated());
  assert(p4f1Attr->deprecationMessage().isEmpty());
  assert(p4f1Attr->hasPragma(PRAGMA_NO_INIT));
  auto p4f2 = p4->formal(1)->toFormal();
  assert(p4f2);
  assert(!p4f2->attributes());

  auto p5 = mod->stmt(4)->toFunction();
  assert(p5);
  auto p5Attr = p5->attributes();
  assert(p5Attr);
  assert(p5Attr->isDeprecated());
  assert(p5Attr->deprecationMessage() == "P5 is deprecated");
  assert(p5->numFormals() == 2);
  auto p5f1 = p5->formal(0)->toFormal();
  assert(p5f1);
  assert(!p5f1->attributes());

  auto p5f2 = p5->formal(1)->toFormal();
  assert(p5f2);
  auto p5f2Attr = p5f2->attributes();
  assert(p5f2Attr);
  assert(!p5f2Attr->isDeprecated());
  assert(p5f2Attr->deprecationMessage().isEmpty());
  assert(p5f2Attr->hasPragma(PRAGMA_NO_INIT));

  auto p6 = mod->stmt(5)->toFunction();
  assert(p6);
  auto p6Attr = p6->attributes();
  assert(p6Attr);
  assert(!p6Attr->isDeprecated());
  assert(p6Attr->hasPragma(PRAGMA_NO_DOC));
  assert(p6->numStmts() == 1);
  auto p6v1 = p6->stmt(0)->toVariable();
  assert(p6v1);
  assert(!p6v1->attributes());
}

// Enum elements can be deprecated.
static void test6(Parser* parser) {
  auto parseResult = parser->parseString("test7.chpl",
      "pragma \"no doc\"\n"
      "deprecated \"Enum is deprecated\"\n"
      "enum Foo {\n"
      "  a,\n"
      "  deprecated \"Element b is deprecated\"\n"
      "  b = 0,\n"
      "  c,\n"
      "}\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto en = mod->stmt(0)->toEnum();
  assert(en);
  auto enAttr = en->attributes();
  assert(enAttr);
  assert(enAttr->isDeprecated());
  assert(enAttr->deprecationMessage() == "Enum is deprecated");
  assert(enAttr->hasPragma(PRAGMA_NO_DOC));
  assert(en->numDeclOrComments() == 3);
  auto ee1 = en->declOrComment(0)->toEnumElement();
  assert(ee1);
  assert(!ee1->attributes());
  auto ee2 = en->declOrComment(1)->toEnumElement();
  assert(ee2);
  assert(ee2->initExpression());
  auto ee2Attr = ee2->attributes();
  assert(ee2Attr);
  assert(ee2Attr->isDeprecated());
  assert(ee2Attr->deprecationMessage() == "Element b is deprecated");
  auto ee3 = en->declOrComment(2)->toEnumElement();
  assert(ee3);
  assert(!ee3->attributes());
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
  test6(p);

  return 0;
}
