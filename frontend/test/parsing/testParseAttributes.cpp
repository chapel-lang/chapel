/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "test-parsing.h"

#include "chpl/parsing/Parser.h"
#include "chpl/framework/Context.h"
#include "chpl/uast/all-uast.h"

static bool areAttributesEqual(const Decl* lhs, const Decl* rhs) {
  auto lhsAttr = lhs->attributeGroup();
  auto rhsAttr = rhs->attributeGroup();

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

static void toggleCompilerFlag(Context* ctx, CompilerFlags::Name flag,
                               bool value) {
  chpl::CompilerFlags flags;
  ctx->advanceToNextRevision(false);
  flags.set(flag, value);
  setCompilerFlags(ctx, flags);
}

// Make sure MultiDecl attributeGroup equal component attributeGroup.
static void test0(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test0.chpl",
      "@chpldoc.nodoc\n"
      "var a, b, c = 0;\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto multiVar = mod->stmt(0)->toMultiDecl();
  assert(multiVar);

  auto parentAttr = multiVar->attributeGroup();
  assert(parentAttr);
  assert(!parentAttr->isDeprecated());
  assert(parentAttr->deprecationMessage().isEmpty());
  auto noDoc = parentAttr->getAttributeNamed(UniqueString::get(parser->context(),
                                                              "chpldoc.nodoc"));
  assert(noDoc);

  for (auto decl : multiVar->decls()) {
    assert(decl->isVariable());
    assert(decl->attributeGroupChildNum() == AstNode::NO_CHILD);
  }
}

// Make sure TupleDecl attributeGroup equal component attributeGroup.
static void test1(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test1.chpl",
      "@chpldoc.nodoc\n"
      "var (a, b, c) = foo;\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto tupleVar = mod->stmt(0)->toTupleDecl();
  assert(tupleVar);

  auto parentAttr = tupleVar->attributeGroup();
  assert(parentAttr);
  assert(!parentAttr->isDeprecated());
  assert(parentAttr->deprecationMessage().isEmpty());
  auto noDoc = parentAttr->getAttributeNamed(UniqueString::get(parser->context(),
                                                              "chpldoc.nodoc"));
  assert(noDoc);

  for (auto decl : tupleVar->decls()) {
    assert(decl->isVariable());
    assert(decl->attributeGroupChildNum() == AstNode::NO_CHILD);
  }
}

// Make sure nested TupleDecl with attributeGroup parses and places the
// attribute group on the outermost TupleDecl.
static void test1a(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test1a.chpl",
      "@chpldoc.nodoc\n"
      "var (a, (x, y), c);\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto tupleVar = mod->stmt(0)->toTupleDecl();
  assert(tupleVar);

  auto parentAttr = tupleVar->attributeGroup();
  assert(parentAttr);
  assert(!parentAttr->isDeprecated());
  assert(parentAttr->deprecationMessage().isEmpty());
  auto noDoc = parentAttr->getAttributeNamed(UniqueString::get(parser->context(),
                                                              "chpldoc.nodoc"));
  assert(noDoc);

  for (auto decl : tupleVar->decls()) {
    assert(decl->attributeGroupChildNum() == AstNode::NO_CHILD);
  }
  assert(tupleVar->decl(0)->isVariable());
  assert(tupleVar->decl(1)->isTupleDecl());
  assert(tupleVar->decl(2)->isVariable());
}

// Make sure multiDecl with nested TupleDecl parses and places the
// attributeGroup on the outer multiDecl
static void test1b(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test1b.chpl",
      "@chpldoc.nodoc\n"
      "var z, v, (a, (x, y), c);\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto multiDecl = mod->stmt(0)->toMultiDecl();
  assert(multiDecl);
  auto parentAttr = multiDecl->attributeGroup();
  assert(parentAttr);
  auto tupleVar = multiDecl->declOrComment(2)->toTupleDecl();
  assert(tupleVar);
  for (auto decl : tupleVar->decls()) {
    assert(decl->attributeGroupChildNum() == AstNode::NO_CHILD);
  }
  assert(tupleVar->decl(0)->isVariable());
  assert(tupleVar->decl(1)->isTupleDecl());
  assert(tupleVar->decl(2)->isVariable());
  auto tupleVarInner = tupleVar->decl(1)->toTupleDecl();
  assert(tupleVarInner);
  for (auto decl : tupleVarInner->decls()) {
    assert(decl->attributeGroupChildNum() == AstNode::NO_CHILD);
  }
  assert(tupleVarInner->decl(0)->isVariable());
  assert(tupleVarInner->decl(1)->isVariable());

  assert(!parentAttr->isDeprecated());
  assert(parentAttr->deprecationMessage().isEmpty());
  auto noDoc = parentAttr->getAttributeNamed(UniqueString::get(parser->context(),
                                                              "chpldoc.nodoc"));
  assert(noDoc);

  for (auto decl : multiDecl->decls()) {
    assert(decl->attributeGroupChildNum() == AstNode::NO_CHILD);
  }
  assert(multiDecl->declOrComment(0)->isVariable());
  assert(multiDecl->declOrComment(1)->isVariable());
  assert(multiDecl->declOrComment(2)->isTupleDecl());
}

// It's an error to have an attribute on a multi-decl where the first element
// is a tuple. This is because we can't detect that we're in a multi-decl in
// time to prevent consuming the attribute as a tuple attribute instead of
// placing it on the multi-decl.
static void test1c(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test1c.chpl",
      "@chpldoc.nodoc\n"
      "var (a, (x, y), c), z, v;\n");

  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto multiDecl = mod->stmt(0)->toMultiDecl();
  assert(multiDecl);
  auto parentAttr = multiDecl->attributeGroup();
  assert(parentAttr);
  assert(!parentAttr->isDeprecated());
  assert(parentAttr->deprecationMessage().isEmpty());
  auto noDoc = parentAttr->getAttributeNamed(UniqueString::get(parser->context(),
                                                              "chpldoc.nodoc"));
  assert(noDoc);
  auto tupleDecl = multiDecl->declOrComment(0)->toTupleDecl();
  assert(tupleDecl);
  assert(tupleDecl->attributeGroupChildNum() == AstNode::NO_CHILD);
  assert(tupleDecl->decl(0)->isVariable());
  assert(tupleDecl->decl(1)->isTupleDecl());
  assert(tupleDecl->decl(2)->isVariable());
  auto tupleDeclInner = tupleDecl->decl(1)->toTupleDecl();
  assert(tupleDeclInner);
  assert(tupleDeclInner->attributeGroupChildNum() == AstNode::NO_CHILD);
  assert(tupleDeclInner->decl(0)->isVariable());
  assert(tupleDeclInner->decl(1)->isVariable());
  assert(multiDecl->declOrComment(1)->isVariable());
  assert(multiDecl->declOrComment(2)->isVariable());
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

static std::string genAggregateAttributesTest(asttags::AstTag aggKind,
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
    ret += "@deprecated";

    if (hasDeprecationMsg) {
      ret += " ";
      ret += "(\"This thing is deprecated\")";
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
                                    asttags::AstTag aggKind,
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
  auto parseResult = parseStringAndReportErrors(parser, "test-aggregate.chpl",
                                         code.c_str());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto agg = mod->stmt(0)->toAggregateDecl();
  assert(agg);

  assert(agg->tag() == aggKind);
  auto attr = agg->attributeGroup();

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

    // Important to make sure the attributeGroup are not incorrectly carried
    // down to children here.
    assert(!areAttributesEqual(var, agg));

    auto varAttr = var->attributeGroup();

    if (doChildrenHavePragmas) {
      assert(varAttr);
      auto tag = genPragmaTag(i);
      assert(varAttr->hasPragma(tag));
    } else {
      assert(!varAttr);
    }
  }
}

// Make sure attributeGroup for aggregates get attached properly.
static void test2(Parser* parser) {
  ErrorGuard guard(parser->context());
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
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test3.chpl",
      "@chpldoc.nodoc\n"
      "@deprecated(\"Thingy is deprecated\")\n"
      "var x = 0;\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto var = mod->stmt(0)->toVariable();
  assert(var);

  auto varAttr = var->attributeGroup();
  assert(varAttr);

  auto noDoc = varAttr->getAttributeNamed(UniqueString::get(parser->context(),
                                                            "chpldoc.nodoc"));
  assert(noDoc);
  assert(varAttr->isDeprecated());
  assert(varAttr->deprecationMessage() == "Thingy is deprecated");
}

// Module, enum, and a var decl.
static void test4(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test4.chpl",
      "@chpldoc.nodoc\n"
      "@deprecated(notes=\"Module is deprecated\")\n"
      "module Foo {\n"
      "  var x = 0;\n"
      "  @deprecated(\"Enum is deprecated\")\n"
      "  enum Y { a, b, c }\n"
      "  pragma \"ref\"\n"
      "  var y = 0;\n"
      "}\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);

  auto modAttr = mod->attributeGroup();
  assert(modAttr);
  assert(modAttr->isDeprecated());
  assert(modAttr->deprecationMessage() == "Module is deprecated");
  auto noDoc = modAttr->getAttributeNamed(UniqueString::get(parser->context(),
                                                            "chpldoc.nodoc"));
  assert(noDoc);
}

// Procedures, formals, nested procedures.
static void test5(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test5.chpl",
      "@chpldoc.nodoc\n"
      "@deprecated(\"P1 is deprecated\")\n"
      "proc p1() {}\n"
      "proc p2() {}\n"
      "pragma \"ref\"\n"
      "proc p3() {\n"
      "  proc n1() {}\n"
      "  pragma \"no init\"\n"
      "  @deprecated(\"N2 is deprecated\")\n"
      "  proc n2() {}\n"
      "}\n"
      "@chpldoc.nodoc\n"
      "@deprecated(\"P4 is deprecated\")\n"
      "proc p4(pragma \"no init\" x, y) {}\n"
      "@deprecated(\"P5 is deprecated\")\n"
      "proc p5(x, pragma \"no init\" y) {}\n"
      "@chpldoc.nodoc\n"
      "proc p6() { var x = 0; }\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 6);

  auto p1 = mod->stmt(0)->toFunction();
  assert(p1);
  auto p1Attr = p1->attributeGroup();
  assert(p1Attr);
  assert(p1Attr->isDeprecated());
  assert(p1Attr->deprecationMessage() == "P1 is deprecated");
  auto noDoc = p1Attr->getAttributeNamed(UniqueString::get(parser->context(),
                                                           "chpldoc.nodoc"));
  assert(noDoc);

  auto p2 = mod->stmt(1)->toFunction();
  assert(!p2->attributeGroup());

  auto p3 = mod->stmt(2)->toFunction();
  assert(p3);
  auto p3Attr = p3->attributeGroup();
  assert(p3Attr);
  assert(!p3Attr->isDeprecated());
  assert(p3Attr->deprecationMessage().isEmpty());
  assert(p3Attr->hasPragma(PRAGMA_REF));

  assert(p3->numStmts() == 2);
  auto p3n1 = p3->stmt(0)->toFunction();
  assert(p3n1);
  assert(!p3n1->attributeGroup());
  auto p3n2 = p3->stmt(1)->toFunction();
  assert(p3n2);
  auto p3n2Attr = p3n2->attributeGroup();
  assert(p3n2Attr);
  assert(p3n2Attr->isDeprecated());
  assert(p3n2Attr->deprecationMessage() == "N2 is deprecated");
  assert(p3n2Attr->hasPragma(PRAGMA_NO_INIT));

  auto p4 = mod->stmt(3)->toFunction();
  assert(p4);
  auto p4Attr = p4->attributeGroup();
  assert(p4Attr);
  assert(p4Attr->isDeprecated());
  assert(p4Attr->deprecationMessage() == "P4 is deprecated");
  auto p4noDoc = p4Attr->getAttributeNamed(UniqueString::get(parser->context(),
                                                             "chpldoc.nodoc"));
  assert(p4noDoc);
  assert(p4->numFormals() == 2);
  auto p4f1 = p4->formal(0)->toFormal();
  assert(p4f1);
  auto p4f1Attr = p4f1->attributeGroup();
  assert(p4f1Attr);
  assert(!p4f1Attr->isDeprecated());
  assert(p4f1Attr->deprecationMessage().isEmpty());
  assert(p4f1Attr->hasPragma(PRAGMA_NO_INIT));
  auto p4f2 = p4->formal(1)->toFormal();
  assert(p4f2);
  assert(!p4f2->attributeGroup());

  auto p5 = mod->stmt(4)->toFunction();
  assert(p5);
  auto p5Attr = p5->attributeGroup();
  assert(p5Attr);
  assert(p5Attr->isDeprecated());
  assert(p5Attr->deprecationMessage() == "P5 is deprecated");
  assert(p5->numFormals() == 2);
  auto p5f1 = p5->formal(0)->toFormal();
  assert(p5f1);
  assert(!p5f1->attributeGroup());

  auto p5f2 = p5->formal(1)->toFormal();
  assert(p5f2);
  auto p5f2Attr = p5f2->attributeGroup();
  assert(p5f2Attr);
  assert(!p5f2Attr->isDeprecated());
  assert(p5f2Attr->deprecationMessage().isEmpty());
  assert(p5f2Attr->hasPragma(PRAGMA_NO_INIT));

  auto p6 = mod->stmt(5)->toFunction();
  assert(p6);
  auto p6Attr = p6->attributeGroup();
  assert(p6Attr);
  assert(!p6Attr->isDeprecated());
  auto p6noDoc = p6Attr->getAttributeNamed(UniqueString::get(parser->context(),
                                                             "chpldoc.nodoc"));
  assert(p6noDoc);
  assert(p6->numStmts() == 1);
  auto p6v1 = p6->stmt(0)->toVariable();
  assert(p6v1);
  assert(!p6v1->attributeGroup());
}

// Enum elements can be deprecated.
static void test6(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test6.chpl",
      "@chpldoc.nodoc\n"
      "@deprecated(\"Enum is deprecated\")\n"
      "enum Foo {\n"
      "  a,\n"
      "  @deprecated(\"Element b is deprecated\")\n"
      "  b = 0,\n"
      "  c,\n"
      "}\n");
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto en = mod->stmt(0)->toEnum();
  assert(en);
  auto enAttr = en->attributeGroup();
  assert(enAttr);
  assert(enAttr->isDeprecated());
  assert(enAttr->deprecationMessage() == "Enum is deprecated");
  auto enNoDoc = enAttr->getAttributeNamed(UniqueString::get(parser->context(),
                                                             "chpldoc.nodoc"));
  assert(enNoDoc);
  assert(en->numDeclOrComments() == 3);
  auto ee1 = en->declOrComment(0)->toEnumElement();
  assert(ee1);
  assert(!ee1->attributeGroup());
  auto ee2 = en->declOrComment(1)->toEnumElement();
  assert(ee2);
  assert(ee2->initExpression());
  assert(!ee2->initExpression()->isAttributeGroup());
  auto ee2Attr = ee2->attributeGroup();
  assert(ee2Attr);
  assert(ee2Attr->isDeprecated());
  assert(ee2Attr->deprecationMessage() == "Element b is deprecated");
  auto ee3 = en->declOrComment(2)->toEnumElement();
  assert(ee3);
  assert(!ee3->attributeGroup());
}

static void test7(Parser* parser) {
  ErrorGuard guard(parser->context());
  std::string program = R""""(
    @chpldoc.nodoc
    @deprecated("Enum is deprecated")
    enum Foo {
      a,
      @deprecated("Element b is deprecated")
      b = 0,
      c,
    }
  )"""";
  auto parseResult = parseStringAndReportErrors(parser, "test7.chpl",
                                                program.c_str());
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto en = mod->stmt(0)->toEnum();
  assert(en);
  auto enAttr = en->attributeGroup();
  assert(enAttr);
  assert(enAttr->isDeprecated());
  assert(enAttr->deprecationMessage() == "Enum is deprecated");
  auto enNoDoc = enAttr->getAttributeNamed(UniqueString::get(parser->context(),
                                                             "chpldoc.nodoc"));
  assert(enNoDoc);
  assert(en->numDeclOrComments() == 3);
  auto ee1 = en->declOrComment(0)->toEnumElement();
  assert(ee1);
  assert(!ee1->attributeGroup());
  auto ee2 = en->declOrComment(1)->toEnumElement();
  assert(ee2);
  assert(ee2->initExpression());
  assert(!ee2->initExpression()->isAttributeGroup());
  auto ee2Attr = ee2->attributeGroup();
  assert(ee2Attr);
  assert(ee2Attr->isDeprecated());
  assert(ee2Attr->deprecationMessage() == "Element b is deprecated");
  auto ee3 = en->declOrComment(2)->toEnumElement();
  assert(ee3);
  assert(!ee3->attributeGroup());
}

// test the stability attributes and all their named arguments
static void test8(Parser* parser) {
  ErrorGuard guard(parser->context());
  std::string program = R""""(
    @chpldoc.nodoc
    @unstable(category="to be deprecated", reason="Foo contains b and d", issue="1234")
    enum Foo {
      a,
      @deprecated(since="1.28", notes="Element b is deprecated", suggestion="use d")
      b = 0,
      c,
      @stable(since="1.28")
      d,
    }
  )"""";
  auto ctx = parser->context();
  auto parseResult = parseStringAndReportErrors(parser, "test8.chpl",
                                                program.c_str());
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto en = mod->stmt(0)->toEnum();
  assert(en);
  auto enAttr = en->attributeGroup();
  assert(enAttr);
  assert(enAttr->isUnstable());
  assert(enAttr->unstableMessage() == "Foo contains b and d");
  auto enNoDoc = enAttr->getAttributeNamed(UniqueString::get(ctx, "chpldoc.nodoc"));
  assert(enNoDoc);
  auto enumAttribute = enAttr->getAttributeNamed(USTR("unstable"));
  assert(enumAttribute);
  assert(enumAttribute->isNamedActual(0));
  assert(enumAttribute->isNamedActual(1));
  assert(enumAttribute->isNamedActual(2));
  assert(enumAttribute->actualName(0) == UniqueString::get(ctx, "category"));
  assert(enumAttribute->actualName(1) == UniqueString::get(ctx, "reason"));
  assert(enumAttribute->actualName(2) == UniqueString::get(ctx, "issue"));
  assert(enumAttribute->actual(0)->isStringLiteral());
  assert(enumAttribute->actual(1)->isStringLiteral());
  assert(enumAttribute->actual(2)->isStringLiteral());
  auto enNotes = enumAttribute->actual(0)->toStringLiteral();
  auto enSuggestion = enumAttribute->actual(1)->toStringLiteral();
  auto enIssue = enumAttribute->actual(2)->toStringLiteral();
  assert(enNotes->value() == "to be deprecated");
  assert(enSuggestion->value() == "Foo contains b and d");
  assert(enIssue->value() == "1234");
  assert(en->numDeclOrComments() == 4);
  auto ee1 = en->declOrComment(0)->toEnumElement();
  assert(ee1);
  assert(!ee1->attributeGroup());
  auto ee2 = en->declOrComment(1)->toEnumElement();
  assert(ee2);
  assert(ee2->initExpression());
  assert(!ee2->initExpression()->isAttributeGroup());
  auto ee2Attr = ee2->attributeGroup();
  assert(ee2Attr);
  assert(ee2Attr->isDeprecated());
  assert(ee2Attr->deprecationMessage() == "Element b is deprecated");
  assert(ee2Attr->numAttributes() == 1);
  auto attr = ee2Attr->getAttributeNamed(USTR("deprecated"));
  assert(attr);
  assert(attr->isNamedActual(0));
  assert(attr->isNamedActual(1));
  assert(attr->isNamedActual(2));
  auto name1 = attr->actualName(0);
  auto name2 = attr->actualName(1);
  auto name3 = attr->actualName(2);
  assert(name1 == UniqueString::get(ctx, "since"));
  assert(name2 == UniqueString::get(ctx, "notes"));
  assert(name3 == UniqueString::get(ctx, "suggestion"));
  assert(attr->actual(0)->isStringLiteral());
  assert(attr->actual(1)->isStringLiteral());
  assert(attr->actual(2)->isStringLiteral());
  auto str1 = attr->actual(0)->toStringLiteral();
  auto str2 = attr->actual(1)->toStringLiteral();
  auto str3 = attr->actual(2)->toStringLiteral();
  assert(str1->value() == "1.28");
  assert(str2->value() == "Element b is deprecated");
  assert(str3->value() == "use d");
  auto ee3 = en->declOrComment(2)->toEnumElement();
  assert(ee3);
  assert(!ee3->attributeGroup());
  auto ee4 = en->declOrComment(3)->toEnumElement();
  assert(ee4);
  auto ee4Attr = ee4->attributeGroup();
  assert(ee4Attr);
  auto ee4AttrStable = ee4Attr->getAttributeNamed(USTR("stable"));
  assert(ee4AttrStable);
  assert(ee4AttrStable->isNamedActual(0));
  assert(ee4AttrStable->actualName(0) == UniqueString::get(ctx, "since"));
  assert(ee4AttrStable->actual(0)->isStringLiteral());
  auto ee4AttrStableStr = ee4AttrStable->actual(0)->toStringLiteral();
  assert(ee4AttrStableStr->value() == "1.28");
}

// TODO: test attributes on all the other places they can go


// test for @unstable, @stable attributes
// test for @chpldoc.nodoc
static void test9(Parser* parser) {
  ErrorGuard guard(parser->context());
  std::string program = R""""(
    @chpldoc.nodoc
    @unstable(category="experimental", reason="Enum is unstable", issue="82566")
    enum Foo {
      a,
      @stable(since="1.28")
      b = 0,
      c,
    }
  )"""";
  auto ctx = parser->context();
  auto parseResult = parseStringAndReportErrors(parser, "test9.chpl",
                                                program.c_str());
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto en = mod->stmt(0)->toEnum();
  assert(en);
  auto enAttr = en->attributeGroup();
  assert(enAttr);
  auto chpldocName = UniqueString::get(ctx, "chpldoc.nodoc");
  auto enumNodoc = enAttr->getAttributeNamed(chpldocName);
  assert(enumNodoc);
  auto enumUnstable = enAttr->getAttributeNamed(USTR("unstable"));
  assert(enumUnstable->isNamedActual(0));
  assert(enumUnstable->isNamedActual(1));
  assert(enumUnstable->isNamedActual(2));
  assert(enumUnstable->actualName(0) == UniqueString::get(ctx, "category"));
  assert(enumUnstable->actualName(1) == UniqueString::get(ctx, "reason"));
  assert(enumUnstable->actualName(2) == UniqueString::get(ctx, "issue"));
  assert(enumUnstable->actual(0)->isStringLiteral());
  assert(enumUnstable->actual(1)->isStringLiteral());
  assert(enumUnstable->actual(2)->isStringLiteral());

  auto enCat = enumUnstable->actual(0)->toStringLiteral();
  auto enReason = enumUnstable->actual(1)->toStringLiteral();
  auto enIssue = enumUnstable->actual(2)->toStringLiteral();

  assert(enCat->value() == "experimental");
  assert(enReason->value() == "Enum is unstable");
  assert(enIssue->value() == "82566");
  assert(en->numDeclOrComments() == 3);
  auto ee1 = en->declOrComment(0)->toEnumElement();
  assert(ee1);
  assert(!ee1->attributeGroup());
  auto ee2 = en->declOrComment(1)->toEnumElement();
  assert(ee2);
  assert(ee2->initExpression());
  assert(!ee2->initExpression()->isAttributeGroup());
  auto ee2Attr = ee2->attributeGroup();
  assert(ee2Attr);
  assert(ee2Attr->numAttributes() == 1);
  auto attr = ee2Attr->getAttributeNamed(USTR("stable"));
  assert(attr);
  assert(attr->isNamedActual(0));
  auto name1 = attr->actualName(0);
  assert(name1 == UniqueString::get(ctx, "since"));
  assert(attr->actual(0)->isStringLiteral());
  auto str1 = attr->actual(0)->toStringLiteral();
  assert(str1->value() == "1.28");
  auto ee3 = en->declOrComment(2)->toEnumElement();
  assert(ee3);
  assert(!ee3->attributeGroup());
}


// test that we don't warn or error on @chpldoc.unknown, etc
static void test10(Parser* parser) {
  ErrorGuard guard(parser->context());
  std::string program = R""""(
    @chpldoc.unknown
    @chpldoc.nodoc
    @chpldoc.subname.attribute("bar")
    proc Foo() {  }
  )"""";
  auto ctx = parser->context();
  toggleCompilerFlag(ctx, CompilerFlags::WARN_UNKNOWN_TOOL_SPACED_ATTRS, true);
  assert(isCompilerFlagSet(ctx, CompilerFlags::WARN_UNKNOWN_TOOL_SPACED_ATTRS));
  auto parseResult = parseStringAndReportErrors(parser, "test10.chpl",
                                                program.c_str());
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto en = mod->stmt(0)->toFunction();
  assert(en);
  auto enAttr = en->attributeGroup();
  assert(enAttr);
  auto nodocName = UniqueString::get(ctx, "chpldoc.nodoc");
  auto unknownName = UniqueString::get(ctx, "chpldoc.unknown");
  auto subnameName = UniqueString::get(ctx, "chpldoc.subname.attribute");
  auto nodoc = enAttr->getAttributeNamed(nodocName);
  auto unknown = enAttr->getAttributeNamed(unknownName);
  auto subname = enAttr->getAttributeNamed(subnameName);
  assert(nodoc);
  assert(unknown);
  assert(subname);

  assert(nodoc->numActuals() == 0);
  assert(unknown->numActuals() == 0);
  assert(subname->numActuals() == 1);

  toggleCompilerFlag(ctx, CompilerFlags::WARN_UNKNOWN_TOOL_SPACED_ATTRS, false);
  assert(!isCompilerFlagSet(ctx, CompilerFlags::WARN_UNKNOWN_TOOL_SPACED_ATTRS));
}


// test that we reject invalid attribute names
static void test11(Parser* parser) {
  ErrorGuard guard(parser->context());
  std::string program = R""""(
    @unknown
    proc Foo() {  }
  )"""";
  auto parseResult = parseStringAndReportErrors(parser, "test11.chpl",
                                                program.c_str());
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->message() == "Unknown top-level attribute 'unknown'");
  assert(guard.error(0)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.realizeErrors() == 1);
}


// test that we reject invalid attribute argument names
// test that we reject invalid attribute argument types
static void test12(Parser* parser) {
  ErrorGuard guard(parser->context());
  std::string program = R""""(
    @unstable(issues=82566)
    proc Foo() {  }
    @deprecated(sincer=1.27)
    proc Bar() {  }
    @stable(sienc=1.27)
    var x: string;
  )"""";
  auto parseResult = parseStringAndReportErrors(parser, "test12.chpl",
                                                program.c_str());
  assert(guard.numErrors() == 6);
  assert(guard.error(0)->message() == "unrecognized argument name 'issues'. "
         "'@unstable' attribute only accepts 'category', 'issue', and 'reason' "
         "arguments");
  assert(guard.error(0)->kind() == ErrorBase::Kind::ERROR);

  assert(guard.error(1)->message() == "unstable attribute arguments must be "
         "string literals for now");
  assert(guard.error(1)->kind() == ErrorBase::Kind::ERROR);

  assert(guard.error(2)->message() == "unrecognized argument name 'sincer'. "
         "'@deprecated' attribute only accepts 'since', 'notes', 'parenful', and "
         "'suggestion' arguments");
  assert(guard.error(2)->kind() == ErrorBase::Kind::ERROR);

  assert(guard.error(3)->message() == "deprecated attribute arguments must be "
         "string literals for now");
  assert(guard.error(3)->kind() == ErrorBase::Kind::ERROR);

  assert(guard.error(4)->message() == "unrecognized argument name 'sienc'. "
         "'@stable' attribute only accepts 'since' or unnamed argument");
  assert(guard.error(4)->kind() == ErrorBase::Kind::ERROR);

  assert(guard.error(5)->message() == "stable attribute arguments must be "
         "string literals for now");
  assert(guard.error(5)->kind() == ErrorBase::Kind::ERROR);

  assert(guard.realizeErrors() == 6);
}


// test that we skip attribute names with toolspaces by default
static void test13(Parser* parser) {
  auto ctx = parser->context();
  ErrorGuard guard(ctx);
  std::string program = R""""(
    @linter.ignoreArgument("bar")
    proc Foo(bar) {  }
  )"""";
  assert(!isCompilerFlagSet(ctx, CompilerFlags::WARN_UNKNOWN_TOOL_SPACED_ATTRS));
  auto parseResult = parseStringAndReportErrors(parser, "test13.chpl",
                                                program.c_str());
  assert(!guard.realizeErrors());
}


// test warning on invalid attribute toolspaces with flag
// test excepting some toolspace names from warning with flag
static void test14(Parser* parser) {
  auto ctx = parser->context();
  ErrorGuard guard(ctx);
  std::string program = R""""(
    @linter.ignoreArgument("bar")
    @othertool.attribute(1234)
    proc Foo(bar) {  }
  )"""";
  toggleCompilerFlag(ctx, CompilerFlags::WARN_UNKNOWN_TOOL_SPACED_ATTRS, true);
  std::vector<UniqueString> toolNames = {UniqueString::get(ctx,"linter")};
  assert(isCompilerFlagSet(ctx, CompilerFlags::WARN_UNKNOWN_TOOL_SPACED_ATTRS));
  parsing::setAttributeToolNames(ctx, toolNames);
  auto parseResult = parseStringAndReportErrors(parser, "test14.chpl",
                                                program.c_str());

  assert(guard.numErrors() == 1);
  assert(guard.error(0)->message() == "Unknown attribute tool name 'othertool'");
  assert(guard.error(0)->kind() == ErrorBase::Kind::WARNING);
  assert(guard.realizeErrors() == 1);
  toggleCompilerFlag(ctx, CompilerFlags::WARN_UNKNOWN_TOOL_SPACED_ATTRS, false);
  assert(!isCompilerFlagSet(ctx, CompilerFlags::WARN_UNKNOWN_TOOL_SPACED_ATTRS));
}



// test for setting an attribute more than once on the same decl
static void test15(Parser* parser) {
  auto ctx = parser->context();
  ErrorGuard guard(ctx);
  std::string program = R""""(
    @chpldoc.nodoc
    @unstable
    @chpldoc.nodoc
    proc Foo(bar) {  }
  )"""";
  auto parseResult = parseStringAndReportErrors(parser, "test15.chpl",
                                                program.c_str());
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->message() == "repeated attribute 'chpldoc.nodoc'");
  assert(guard.error(0)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.realizeErrors() == 1);
}


// test for setting a named attribute argument more than once
static void test16(Parser* parser) {
  auto ctx = parser->context();
  ErrorGuard guard(ctx);
  std::string program = R""""(
    @unstable(reason="foo", reason='bar')
    proc Foo(bar) {  }
  )"""";
  auto parseResult = parseStringAndReportErrors(parser, "test16.chpl",
                                                program.c_str());
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->message() == "repeated argument name 'reason'");
  assert(guard.error(0)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.realizeErrors() == 1);
}

// test for setting attribute arguments without names
static void test17(Parser* parser) {
  auto ctx = parser->context();
  ErrorGuard guard(ctx);
  std::string program = R""""(
    @unstable("this thing is unstable")
    proc Foo(bar) {  }
    @deprecated("this thing is deprecated")
    var x: int;
    @stable("1.28")
    var y: int;
  )"""";
  auto parseResult = parseStringAndReportErrors(parser, "test17.chpl",
                                                program.c_str());

  assert(!guard.realizeErrors());
}

// test for setting multiple attribute arguments without names
static void test18(Parser* parser) {
  auto ctx = parser->context();
  ErrorGuard guard(ctx);
  std::string program = R""""(
    @unstable("this thing is unstable","experimental", "15634")
    proc Foo(bar) {  }
    @deprecated("this thing is deprecated", "use Baz instead")
    var x: int;
    @stable("1.28", "1.29")
    var y: int;
  )"""";
  auto parseResult = parseStringAndReportErrors(parser, "test18.chpl",
                                                program.c_str());

  assert(guard.numErrors() == 3);
  assert(guard.error(0)->message() == "unstable attribute only accepts one "
         "unnamed argument");
  assert(guard.error(0)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.error(1)->message() == "deprecated attribute only accepts one "
         "unnamed argument");
  assert(guard.error(1)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.error(2)->message() == "stable attribute only accepts one "
         "argument");
  assert(guard.error(2)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.realizeErrors() == 3);
}

// test that we reject attributes with list of named arguments not inside parens
static void test19(Parser* parser) {
  auto ctx = parser->context();
  ErrorGuard guard(ctx);
  std::string program = R""""(
    @unstable category="experimental", reason="Enum is unstable", issue="82566"
    proc Foo(bar) {  }
  )"""";
  auto parseResult = parseStringAndReportErrors(parser, "test19.chpl",
                                                program.c_str());

  assert(guard.error(0)->message() == "near 'category'");
  assert(guard.error(0)->kind() == ErrorBase::Kind::SYNTAX);
  assert(guard.realizeErrors()==1);
}

// test that we reject attributes with a single unnamed argument not inside parens
static void test20(Parser* parser) {
  auto ctx = parser->context();
  ErrorGuard guard(ctx);
  std::string program = R""""(
    @stable "1.28"
    proc Foo(bar) {  }
  )"""";
  auto parseResult = parseStringAndReportErrors(parser, "test20.chpl",
                                                program.c_str());
  assert(guard.error(0)->message() == "near '\"'");
  assert(guard.error(0)->kind() == ErrorBase::Kind::SYNTAX);
  assert(guard.realizeErrors()==1);
}

// test that we reject attributes with a single named argument not inside parens
static void test21(Parser* parser) {
  auto ctx = parser->context();
  ErrorGuard guard(ctx);
  std::string program = R""""(
    @deprecated suggestion="use Baz instead"
    proc Foo(bar) {  }
  )"""";
  auto parseResult = parseStringAndReportErrors(parser, "test21.chpl",
                                                program.c_str());
  assert(guard.error(0)->message() == "near 'suggestion'");
  assert(guard.error(0)->kind() == ErrorBase::Kind::SYNTAX);
  assert(guard.realizeErrors()==1);
}

// test that we reject attributes with a list of unnamed arguments not inside parens
static void test22(Parser* parser) {
  auto ctx = parser->context();
  ErrorGuard guard(ctx);
  std::string program = R""""(
    @unstable "this thing is unstable", "experimental", "15634"
    proc Foo(bar) {  }
  )"""";
  auto parseResult = parseStringAndReportErrors(parser, "test22.chpl",
                                                program.c_str());

  assert(guard.error(0)->message() == "near '\"'");
  assert(guard.error(0)->kind() == ErrorBase::Kind::SYNTAX);
  assert(guard.realizeErrors()==1);
}

// check interfaces
static void test23(Parser* parser) {
  auto ctx = parser->context();
  ErrorGuard guard(ctx);
  std::string program = R""""(
    /*I am the docstring for the interface*/
    @unstable("I am unstable")
    interface foo {
      /*I am the docstring for the method*/
      @deprecated("I am deprecated")
      proc Self.bar();
    }
  )"""";
  auto parseResult = parseStringAndReportErrors(parser, "test23.chpl",
                                                program.c_str());
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 2);

  auto comment = mod->stmt(0)->toComment();
  assert(comment);
  assert(comment->str() == "/*I am the docstring for the interface*/");

  auto intf = mod->stmt(1)->toInterface();
  assert(intf);

  auto intfAttr = intf->attributeGroup();
  assert(intfAttr);
  assert(intfAttr->isUnstable());
  assert(intf->numFormals() == 0);
  assert(intf->numStmts() == 2);

  auto methodComment = intf->stmt(0)->toComment();
  assert(methodComment);
  assert(methodComment->str() == "/*I am the docstring for the method*/");

  auto intfMethod = intf->stmt(1)->toFunction();
  assert(intfMethod);
  assert(intfMethod->name() == "bar");
  auto intfMethodAttr = intfMethod->attributeGroup();
  assert(intfMethodAttr);
  assert(intfMethodAttr->isDeprecated());


}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::createForTopLevelModule(ctx);
  Parser* p = &parser;

  test0(p);
  test1(p);
  test1a(p);
  test1b(p);
  test1c(p);
  test2(p);
  test3(p);
  test4(p);
  test5(p);
  test6(p);
  test7(p);
  test8(p);
  test9(p);
  test10(p);
  test11(p);
  test12(p);
  test13(p);
  test14(p);
  test15(p);
  test16(p);
  test17(p);
  test18(p);
  test19(p);
  test20(p);
  test21(p);
  test22(p);
  test23(p);

  return 0;
}
