/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/For.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"

#include <iostream>

static void test0(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test0.chpl",
      "{{\n"
      "  var x = A[\n"
      "}}\n"
      "var y = 1;\n");
  assert(guard.realizeErrors() == 1);
}

static void test1(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test1.chpl",
      "class CC\n"
      "  proc foo() CC {\n"
      "    return new unmanaged CC();\n"
      "  }\n"
      "}\n");
  assert(guard.realizeErrors() == 1);
}

static void test2(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test2.chpl",
      "pragma \"lineno ok\"\n"
      "proc Matrix(const Arrays: ...?n) {}\n");
  assert(guard.realizeErrors() == 1);
}

static void test3(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test3.chpl",
      "class C {\n"
      "  var a: pwned C;\n"
      "}\n");
  assert(guard.realizeErrors() == 1);
}


static void test4(Parser* parser) {
  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, "test4.chpl",
      "pragma \"lineno ok\"\n"
      "use foo;\n");
  assert(guard.realizeErrors() == 1);
}

int main() {
  Context::Configuration config_devel;
  config_devel.chplEnvOverrides.insert({"CHPL_DEVELOPER", "1"});
  Context context_devel(config_devel);
  Context context;

  auto parser_devel = Parser::createForTopLevelModule(&context_devel);
  Parser* pd = &parser_devel;

  auto parser = Parser::createForTopLevelModule(&context);
  Parser* p = &parser;

  test0(pd);
  test0(p);
  test1(pd);
  test1(p);
  test2(pd);
  test2(p);
  test3(pd);
  test3(p);
  test4(pd);
  test4(p);

  return 0;
}
