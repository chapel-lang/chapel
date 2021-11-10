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

const std::string circleImpl =
        "/* comment 1 */\n"
        "class MyCircleImpl {\n"
          "var radius:real;\n"
          "proc area() {\n"
            "return pi*radius*radius;\n"
          "}\n"
          "proc circumference() {\n"
            "return 2.0*pi*radius;\n"
          "}\n"
        "}\n";

static void test0(Parser* parser) {

  const std::string myCircle =
        "record MyCircle {\n"
          "forwarding var impl: MyCircleImpl;\n"
        "}\n";

  const std::string circle = circleImpl + myCircle;

  auto parseResult = parser->parseString("test0.chpl", circle.c_str());

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isClass());
  assert(mod->stmt(2)->isRecord());
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

  const std::string circle = circleImpl + myCircle;

  auto parseResult = parser->parseString("test1.chpl", circle.c_str());
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);

  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isClass());
  assert(mod->stmt(2)->isRecord());
  const Record* record = mod->stmt(2)->toRecord();
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

  const std::string circle = circleImpl + myCircle;

  auto parseResult = parser->parseString("test2.chpl", circle.c_str());

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isClass());
  assert(mod->stmt(2)->isRecord());
  const Record* record = mod->stmt(2)->toRecord();
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

  const std::string circle = circleImpl + myCircle;

  auto parseResult = parser->parseString("test3.chpl", circle.c_str());

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isClass());
  assert(mod->stmt(2)->isRecord());
  const Record* record = mod->stmt(2)->toRecord();
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

  const std::string circle = circleImpl + myCircle;

  auto parseResult = parser->parseString("test4.chpl", circle.c_str());

  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  assert(mod->stmt(0)->isComment());
  assert(mod->stmt(1)->isClass());
  assert(mod->stmt(2)->isRecord());
  const Record* record = mod->stmt(2)->toRecord();
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
  return 0;
}
