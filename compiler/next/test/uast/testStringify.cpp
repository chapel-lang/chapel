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

#include "chpl/queries/Context.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/uast/all-uast.h"
#include "chpl/parsing/Parser.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

using namespace chpl;
using namespace uast;
using namespace parsing;

static void test0(Parser* parser) {
  // test Module stringify
  auto parseResult = parser->parseString("test0.chpl", "");
  auto mod = parseResult.singleModule();
  std::ostringstream ss;
  mod->stringify(ss, CHPL_SYNTAX);
  assert(ss.str() == "test0Module test0 \n");
}

static void test1(Parser* parser) {
  // test Identifier stringify
  auto parseResult = parser->parseString("test1.chpl", "x;");
  auto mod = parseResult.singleModule();
  auto identifier = mod->stmt(0)->toIdentifier();
  std::ostringstream ss;
  identifier->stringify(ss, CHPL_SYNTAX);
  assert(ss.str() == "test1@0Identifier x \n");
}

static void test2(Parser* parser) {
  // test Class stringify
  auto parseResult = parser->parseString("test2.chpl", "class C { }");
  auto mod = parseResult.singleModule();
  const AggregateDecl* agg = mod->stmt(0)->toAggregateDecl();
  auto cls = agg->toClass();
  std::ostringstream ss;
  cls->stringify(ss, CHPL_SYNTAX);
  assert(ss.str() == "test2.CClass C \n");
}

static void test3(Parser* parser) {
  // test record stringify
  auto parseResult = parser->parseString("test3.chpl",
                                         "record R { var x; proc method() { } }");
  auto mod = parseResult.singleModule();
  const AggregateDecl* agg = mod->stmt(0)->toAggregateDecl();
  auto rec = agg->toRecord();
  std::ostringstream ss;
  rec->stringify(ss, CHPL_SYNTAX);
  assert(ss.str() == "         test3.RRecord R \n"
                     "       test3.R@0  Variable x \n"
                     "  test3.R.method  Function method \n"
                     "test3.R.method@1    Formal this \n"
                     "test3.R.method@0      Identifier R \n"
                     "test3.R.method@2    Block \n");
}


int main(int argc, char** argv) {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();
  test0(p);
  test1(p);
  test2(p);
  test3(p);

  return 0;
}
