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
#include "chpl/uast/BytesLiteral.h"
#include "chpl/uast/CStringLiteral.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/StringLiteral.h"
#include "chpl/uast/Variable.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace uast;
using namespace frontend;

static void testStringLiteral(Parser* parser,
                              const char* testname,
                              const char* str,
                              StringLiteral::QuoteStyle expectQuoteStyle,
                              const std::string& expectValue) {
  std::string toparse = "var x = ";
  toparse += str;
  toparse += ";\n";
  auto parseResult = parser->parseString(testname, toparse.c_str());
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto variable = module->stmt(0)->toVariable();
  assert(variable);
  auto strLit = variable->initExpression()->toStringLiteral();
  assert(strLit);
  assert(strLit->quoteStyle() == expectQuoteStyle);
  assert(strLit->str() == expectValue);
}
static void testBytesLiteral(Parser* parser,
                             const char* testname,
                             const char* str,
                             StringLiteral::QuoteStyle expectQuoteStyle,
                             const std::string& expectValue) {
  std::string toparse = "var x = b";
  toparse += str;
  toparse += ";\n";
  auto parseResult = parser->parseString(testname, toparse.c_str());
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto variable = module->stmt(0)->toVariable();
  assert(variable);
  auto bytesLit = variable->initExpression()->toBytesLiteral();
  assert(bytesLit);
  assert(bytesLit->quoteStyle() == expectQuoteStyle);
  assert(bytesLit->str() == expectValue);
}
static void testCStringLiteral(Parser* parser,
                               const char* testname,
                               const char* str,
                               StringLiteral::QuoteStyle expectQuoteStyle,
                               const std::string& expectValue) {
  std::string toparse = "var x = c";
  toparse += str;
  toparse += ";\n";
  auto parseResult = parser->parseString(testname, toparse.c_str());
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto variable = module->stmt(0)->toVariable();
  assert(variable);
  auto strLit = variable->initExpression()->toCStringLiteral();
  assert(strLit);
  assert(strLit->quoteStyle() == expectQuoteStyle);
  assert(strLit->str() == expectValue);
}

static void testLiteral(Parser* parser,
                        const char* testname,
                        const char* str,
                        StringLiteral::QuoteStyle expectQuoteStyle,
                        const std::string& expectValue) {
  testStringLiteral(parser, testname, str, expectQuoteStyle, expectValue);
  testBytesLiteral(parser, testname, str, expectQuoteStyle, expectValue);
}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  testLiteral(p, "test0.chpl", "\"hi\"",
              StringLiteral::DOUBLE, std::string("hi"));
  testLiteral(p, "test1.chpl", "'hi'",
              StringLiteral::SINGLE, std::string("hi"));
  testLiteral(p, "test2.chpl", "\"\"\"hi\"\"\"",
              StringLiteral::TRIPLE_DOUBLE, std::string("hi"));
  testLiteral(p, "test3.chpl", "'''hi'''",
              StringLiteral::TRIPLE_SINGLE, std::string("hi"));
  testLiteral(p, "test4.chpl", "\"'hi'\"",
              StringLiteral::DOUBLE, std::string("'hi'"));
  testLiteral(p, "test5.chpl", "'\"hi\"'",
              StringLiteral::SINGLE, std::string("\"hi\""));
  testLiteral(p, "test6.chpl", "'\\'\\\"\\?\\\\\\a\\b\\f\\n\\r\\t\\v'",
              StringLiteral::SINGLE, std::string("'\"\?\\\a\b\f\n\r\t\v"));
  testLiteral(p, "test7.chpl", "'''\\'\\\"\\?\\\\\\a\\b\\f\\n\\r\\t\\v'''",
              StringLiteral::TRIPLE_SINGLE,
              std::string("\\'\\\"\\\?\\\\\\a\\b\\f\\n\\r\\t\\v"));
  testLiteral(p, "test8.chpl", "\"\\'\\\"\\?\\\\\\a\\b\\f\\n\\r\\t\\v\"",
              StringLiteral::DOUBLE, std::string("'\"\?\\\a\b\f\n\r\t\v"));
  testLiteral(p, "test9.chpl",
              "\"\"\"\\'\\\"\\?\\\\\\a\\b\\f\\n\\r\\t\\v\"\"\"",
              StringLiteral::TRIPLE_DOUBLE,
              std::string("\\'\\\"\\\?\\\\\\a\\b\\f\\n\\r\\t\\v"));

  std::string zeroOne;
  zeroOne.push_back('\x00');
  zeroOne.push_back('\x01');
  testLiteral(p, "test10.chpl", "'\\x00\\x01'",
              StringLiteral::SINGLE, zeroOne);
  testLiteral(p, "test11.chpl", "\"\\x00\\x01\"",
              StringLiteral::DOUBLE, zeroOne);
  testLiteral(p, "test12.chpl", "'''\\x00\\x01'''",
              StringLiteral::TRIPLE_SINGLE, std::string("\\x00\\x01"));
  testLiteral(p, "test13.chpl", "\"\"\"\\x00\\x01\"\"\"",
              StringLiteral::TRIPLE_DOUBLE, std::string("\\x00\\x01"));

  testCStringLiteral(p, "test20.chpl", "'hi'",
                     StringLiteral::SINGLE, std::string("hi"));
  testCStringLiteral(p, "test21.chpl", "\"hi\"",
                     StringLiteral::DOUBLE, std::string("hi"));

  return 0;
}
