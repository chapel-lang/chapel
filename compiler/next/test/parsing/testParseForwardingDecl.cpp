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
#include "chpl/uast/Begin.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/queries/Context.h"
#include "chpl/uast/ForwardingDecl.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/VisibilityClause.h"


// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <array>
#include <cassert>

using namespace chpl;
using namespace uast;
using namespace parsing;

static void test0(Parser* parser) {

  const std::string myCircle =
        "record MyCircle {\n"
          "forwarding var impl: MyCircleImpl;\n"
        "}\n";

  auto parseResult = parser->parseString("test0.chpl", myCircle.c_str());

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isRecord());
  const Record* record = mod->stmt(0)->toRecord();
  assert(record);
  assert(record->numChildren() == 1);
  assert(record->child(0)->isForwardingDecl());
  const ForwardingDecl* fwd = record->child(0)->toForwardingDecl();
  assert(fwd);
  assert(fwd->expr()->isVariable());
  const Variable* var = fwd->expr()->toVariable();
  assert(var);
  assert(var->visibility() == Decl::DEFAULT_VISIBILITY);
}

static void test1(Parser* parser) {
  const std::string myCircle =
                    "record MyCircle {\n"
                      "var impl: MyCircleImpl;\n"

                      "proc getImplOrFail() {\n"
                        "if impl == nil then\n"
                          "halt('impl is nil');\n"
                        "else\n"
                          "return impl;\n"
                      "}\n"
                      "forwarding getImplOrFail();\n"
                     "}\n";

  auto parseResult = parser->parseString("test1.chpl", myCircle.c_str());
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);

  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isRecord());
  const Record* record = mod->stmt(0)->toRecord();
  assert(record);
  assert(record->numChildren() == 3);
  assert(record->child(0)->isVariable());
  assert(record->child(1)->isFunction());
  assert(record->child(2)->isForwardingDecl());
  const ForwardingDecl* fwd = record->child(2)->toForwardingDecl();
  assert(fwd);
  const Expression* expr = fwd->expr()->toExpression();
  assert(expr);
  assert(expr->isFnCall());
}


static void test2(Parser* parser) {
  const std::string myCircle =
                    "record MyCircle {\n"
                      "var impl: MyCircleImpl;\n"
                      "forwarding impl only area;\n"
                      "/* some comments after forwarding*/\n"
                    "}\n";

  auto parseResult = parser->parseString("test2.chpl", myCircle.c_str());

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isRecord());
  const Record* record = mod->stmt(0)->toRecord();
  assert(record);
  assert(record->numChildren() == 3);
  assert(record->child(0)->isVariable());
  assert(record->child(1)->isForwardingDecl());
  assert(record->child(2)->isComment());

  const ForwardingDecl* fwd = record->child(1)->toForwardingDecl();
  assert(fwd);
  const Expression* expr = fwd->expr()->toExpression();
  assert(expr);

  assert(expr->isVisibilityClause());
  const VisibilityClause* visClause = expr->toVisibilityClause();
  assert(visClause);
  assert(visClause->symbol()->isIdentifier());
  assert(visClause->limitationKind() == VisibilityClause::ONLY);
  assert(visClause->numLimitations() == 1);
}

static void test3(Parser* parser) {
  const std::string myCircle =
                    "record MyCircle {\n"
                      "var impl: MyCircleImpl;\n"
                      "/* some comments before forwarding*/\n"
                      "forwarding impl except circumference;\n"
                    "}\n";

  auto parseResult = parser->parseString("test3.chpl", myCircle.c_str());

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isRecord());
  const Record* record = mod->stmt(0)->toRecord();
  assert(record);
  assert(record->numChildren() == 3);
  assert(record->child(0)->isVariable());
  assert(record->child(1)->isComment());
  assert(record->child(2)->isForwardingDecl());
  const ForwardingDecl* fwd = record->child(2)->toForwardingDecl();
  assert(fwd);
  const Expression* expr = fwd->expr()->toExpression();
  assert(expr);
  assert(expr->isVisibilityClause());
  const VisibilityClause* visClause = expr->toVisibilityClause();
  assert(visClause);
  assert(visClause->symbol()->isIdentifier());
  assert(visClause->limitationKind() == VisibilityClause::EXCEPT);
  assert(visClause->numLimitations() == 1);
}

static void test4(Parser* parser) {
  const std::string myCircle =
                    "record MyCircle {\n"
                      "var impl: MyCircleImpl;\n"
                      "/* some comments before forwarding*/\n"
                      "forwarding var x = 10;\n"
                    "}\n";

  auto parseResult = parser->parseString("test4.chpl", myCircle.c_str());

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isRecord());
  const Record* record = mod->stmt(0)->toRecord();
  assert(record);
  assert(record->numChildren() == 3);
  assert(record->child(0)->isVariable());
  assert(record->child(1)->isComment());
  assert(record->child(2)->isForwardingDecl());
  const ForwardingDecl* fwd = record->child(2)->toForwardingDecl();
  assert(fwd);
  const Expression* expr = fwd->expr()->toExpression();
  assert(expr);
  assert(expr->isVariable());
  const Variable* var = expr->toVariable();
  assert(var);
  assert(var->visibility() == Decl::DEFAULT_VISIBILITY);
}


static void test5(Parser* parser) {
  const std::string myCircle =
                        "module test5 {\n"
                          "class MyCircleImpl {\n"
                            "var radius:real;\n"
                            "proc area() {\n"
                              "return pi*radius*radius;\n"
                            "}\n"
                            "proc circumference() {\n"
                              "return 2.0*pi*radius;\n"
                            "}\n"
                          "}\n"
                          "record MyCircle {\n"
                            "var impl: MyCircleImpl;\n"

                            "forwarding impl except circumference;\n"
                          "}\n"
                          "// var c = new MyCircle(new MyCircleImpl(10));\n"
                          "// writeln(c.area());\n"
                        "}\n";

  auto parseResult = parser->parseString("test5.chpl", myCircle.c_str());

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 4);

  assert(mod->stmt(0)->isClass());
  assert(mod->stmt(1)->isRecord());
  assert(mod->stmt(2)->isComment());
  assert(mod->stmt(3)->isComment());
  const Record* record = mod->stmt(1)->toRecord();
  assert(record);
  assert(record->numChildren() == 2);
  assert(record->child(0)->isVariable());
  assert(record->child(1)->isForwardingDecl());
  const ForwardingDecl* fwd = record->child(1)->toForwardingDecl();
  assert(fwd);
  const Expression* expr = fwd->expr()->toExpression();
  assert(expr);
  assert(expr->isVisibilityClause());
  const VisibilityClause* visClause = expr->toVisibilityClause();
  assert(visClause);
  assert(visClause->symbol()->isIdentifier());
  assert(visClause->limitationKind() == VisibilityClause::EXCEPT);
  assert(visClause->numLimitations() == 1);
}


/**
 * commented this test out until visibility is implemented
 *
*/

// static void test6(Parser* parser) {

//   const std::string myCircle =
//         "record MyCircle {\n"
//           "forwarding private var impl: MyCircleImpl;\n"
//         "}\n";

//   auto parseResult = parser->parseString("test6.chpl", myCircle.c_str());
//   for (int i=0;i<parseResult.numErrors();i++){
//     fprintf(stderr, "parse error: %s\n", parseResult.error(i).message().c_str());
//   }

//   assert(!parseResult.numErrors());
//   auto mod = parseResult.singleModule();
//   assert(mod);
//   assert(mod->numStmts() == 1);
//   assert(mod->stmt(0)->isRecord());
//   const Record* record = mod->stmt(0)->toRecord();
//   assert(record);
//   assert(record->numChildren() == 1);
//   assert(record->child(0)->isForwardingDecl());
//   const ForwardingDecl* fwd = record->child(0)->toForwardingDecl();
//   assert(fwd);
//   assert(fwd->expr()->isVariable());
//   const Variable* var = fwd->expr()->toVariable();
//   assert(var);
//   assert(var->visibility() == Decl::PRIVATE);
// }


/**
 * commented this test out until visibility is implemented
 *
*/
// static void test7(Parser* parser) {

//   const std::string myCircle =
//         "record MyCircle {\n"
//           "forwarding public var impl: MyCircleImpl;\n"
//         "}\n";

//   auto parseResult = parser->parseString("test7.chpl", myCircle.c_str());

//   assert(!parseResult.numErrors());
//   auto mod = parseResult.singleModule();
//   assert(mod);
//   assert(mod->numStmts() == 1);
//   assert(mod->stmt(0)->isRecord());
//   const Record* record = mod->stmt(0)->toRecord();
//   assert(record);
//   assert(record->numChildren() == 1);
//   assert(record->child(0)->isForwardingDecl());
//   const ForwardingDecl* fwd = record->child(0)->toForwardingDecl();
//   assert(fwd);
//   assert(fwd->expr()->isVariable());
//   const Variable* var = fwd->expr()->toVariable();
//   assert(var);
//   assert(var->visibility() == Decl::PUBLIC);
// }

static void test8(Parser* parser) {

  const std::string myCircle =
        "record MyCircle {\n"
          "pragma \"no doc\"\n"
          "forwarding var impl: MyCircleImpl;\n"
        "}\n";

  auto parseResult = parser->parseString("test8.chpl", myCircle.c_str());

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isRecord());
  const Record* record = mod->stmt(0)->toRecord();
  assert(record);
  assert(record->numChildren() == 1);
  assert(record->child(0)->isForwardingDecl());
  const ForwardingDecl* fwd = record->child(0)->toForwardingDecl();
  assert(fwd);
  assert(fwd->expr()->isVariable());
  assert(fwd->attributes());
  auto attr = fwd->attributes();
  assert(attr);
  assert(!attr->isDeprecated());
  assert(attr->hasPragma(PRAGMA_NO_DOC));
  const Variable* var = fwd->expr()->toVariable();
  assert(var);
  assert(var->visibility() == Decl::DEFAULT_VISIBILITY);
}

static void test9(Parser* parser) {

  const std::string myCircle =
        "record MyCircle {\n"
          "deprecated \"don't use this anymore\"\n"
          "forwarding var impl: MyCircleImpl;\n"
        "}\n";

  auto parseResult = parser->parseString("test9.chpl", myCircle.c_str());

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isRecord());
  const Record* record = mod->stmt(0)->toRecord();
  assert(record);
  assert(record->numChildren() == 1);
  assert(record->child(0)->isForwardingDecl());
  const ForwardingDecl* fwd = record->child(0)->toForwardingDecl();
  assert(fwd);
  assert(fwd->numChildren() == 2);
  assert(fwd->expr()->isVariable());
  assert(fwd->attributes());
  auto attr = fwd->attributes();
  assert(attr);
  assert(attr->isDeprecated());
  assert(!attr->hasPragma(PRAGMA_NO_DOC));
  const Variable* var = fwd->expr()->toVariable();
  assert(var);
  assert(var->visibility() == Decl::DEFAULT_VISIBILITY);

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
  // TODO: Add test6 and test7 when visibility implemented
  test8(p);
  test9(p);
  return 0;
}
