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
#include "chpl/uast/BytesLiteral.h"
#include "chpl/uast/CStringLiteral.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/StringLiteral.h"
#include "chpl/uast/Variable.h"

static uast::BuilderResult parseExprAsVarInit(Parser* parser,
                                              const std::string& testname,
                                              const std::string& init,
                                              const AstNode*& exprOut) {
  ErrorGuard guard(parser->context());
  std::string toparse = "var x = ";
  toparse += init;
  toparse += ";\n";
  auto parseResult = parseStringAndReportErrors(parser, testname.c_str(), toparse.c_str());
  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto variable = mod->stmt(0)->toVariable();
  assert(variable);
  exprOut = variable->initExpression();
  return parseResult;
}

static void testStringLiteral(Parser* parser,
                              const std::string& testname,
                              const std::string& str,
                              StringLikeLiteral::QuoteStyle expectQuoteStyle,
                              const std::string& expectValue) {
  const AstNode* initExpr = nullptr;
  auto parseResult = parseExprAsVarInit(parser, testname, str, initExpr);
  auto strLit = initExpr->toStringLiteral();
  assert(strLit);
  assert(strLit->quoteStyle() == expectQuoteStyle);
  assert(strLit->value().str() == expectValue);
}
static void testBytesLiteral(Parser* parser,
                             const std::string& testname,
                             std::string str,
                             StringLikeLiteral::QuoteStyle expectQuoteStyle,
                             const std::string& expectValue) {
  const AstNode* initExpr = nullptr;
  auto parseResult = parseExprAsVarInit(parser, testname, str, initExpr);
  auto bytesLit = initExpr->toBytesLiteral();
  assert(bytesLit);
  assert(bytesLit->quoteStyle() == expectQuoteStyle);
  assert(bytesLit->value().str() == expectValue);
}
static void testCStringLiteral(Parser* parser,
                               const std::string& testname,
                               std::string str,
                               StringLiteral::QuoteStyle expectQuoteStyle,
                               const std::string& expectValue) {
  const AstNode* initExpr = nullptr;
  ErrorGuard guard(parser->context());
  std::string toparse = "var x = ";
  toparse += str.c_str();
  toparse += ";\n";
  auto parseResult = parseStringAndReportErrors(parser, testname.c_str(), toparse.c_str());
  assert(guard.numErrors() == 1);
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto variable = mod->stmt(0)->toVariable();
  assert(variable);
  initExpr = variable->initExpression();
  auto strLit = initExpr->toCStringLiteral();
  assert(strLit);
  assert(strLit->quoteStyle() == expectQuoteStyle);
  assert(strLit->value().str() == expectValue);
  assert(guard.error(0)->message() == "the type 'c_string' is deprecated and with it, C string literals; use 'c_ptrToConst(\"string\")' or 'string.c_str()' from the 'CTypes' module instead");
  assert(guard.error(0)->kind() == ErrorBase::Kind::WARNING);
  assert(guard.realizeErrors()==1);
}

static void testTripleLiteral(Parser* parser,
                              const char* testname,
                              const char* s,
                              const std::string& expectValue) {

  std::string threeSingle = "'''";
  std::string threeDouble = "\"\"\"";

  std::string tSingleS = threeSingle + s + threeSingle;
  std::string tDoubleS = threeDouble + s + threeDouble;

  std::string tn = testname;

  testStringLiteral(parser, "triple-single-s-" + tn,
                    tSingleS, StringLikeLiteral::TRIPLE_SINGLE, expectValue);
  testStringLiteral(parser, "triple-double-s-" + tn,
                    tDoubleS, StringLikeLiteral::TRIPLE_DOUBLE, expectValue);

  testBytesLiteral(parser, "triple-single-b-" + tn,
                   "b" + tSingleS,
                   StringLikeLiteral::TRIPLE_SINGLE, expectValue);
  testBytesLiteral(parser, "triple-double-b-" + tn,
                   "b" + tDoubleS,
                   StringLikeLiteral::TRIPLE_DOUBLE, expectValue);
}

static void testSingleLiteral(Parser* parser,
                              const char* testname,
                              const char* s,
                              const std::string& expectValue) {

  std::string oneSingle = "'";
  std::string oneDouble = "\"";

  std::string oSingleS = oneSingle + s + oneSingle;
  std::string oDoubleS = oneDouble + s + oneDouble;

  std::string tn = testname;

  testStringLiteral(parser, "single-s-" + tn,
                    oSingleS, StringLikeLiteral::SINGLE, expectValue);
  testStringLiteral(parser, "double-s-" + tn,
                    oDoubleS, StringLikeLiteral::DOUBLE, expectValue);

  testBytesLiteral(parser, "single-b-" + tn,
                   "b" + oSingleS, StringLikeLiteral::SINGLE, expectValue);
  testBytesLiteral(parser, "double-b-" + tn,
                   "b" + oDoubleS, StringLikeLiteral::DOUBLE, expectValue);

  testCStringLiteral(parser, "single-c-" + tn,
                     "c" + oSingleS, StringLikeLiteral::SINGLE, expectValue);
  testCStringLiteral(parser, "double-c-" + tn,
                     "c" + oDoubleS, StringLikeLiteral::DOUBLE, expectValue);
}


static void testBadLiteral(Parser* parser,
                           const char* testname,
                           const char* str) {
  ErrorGuard guard(parser->context());
  std::string toparse = "var x = ";
  toparse += str;
  toparse += ";\n";
  auto parseResult = parseStringAndReportErrors(parser, testname, toparse.c_str());
  assert(guard.realizeErrors() > 0);
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  auto variable = mod->stmt(0)->toVariable();
  assert(variable);
  assert(variable->initExpression()->isErroneousExpression());
}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::createForTopLevelModule(ctx);
  Parser* p = &parser;

  testSingleLiteral(p, "test0.chpl", "hi", std::string("hi"));
  testTripleLiteral(p, "test2.chpl", "hi", std::string("hi"));
  testStringLiteral(p, "test4.chpl", std::string("\"'hi'\""),
                    StringLiteral::DOUBLE, std::string("'hi'"));
  testStringLiteral(p, "test5.chpl", std::string("'\"hi\"'"),
                    StringLiteral::SINGLE, std::string("\"hi\""));
  testSingleLiteral(p, "test6.chpl", "\\'\\\"\\?\\\\\\a\\b\\f\\n\\r\\t\\v",
                    std::string("'\"\?\\\a\b\f\n\r\t\v"));
  testTripleLiteral(p, "test7.chpl", "\\'\\\"\\?\\\\\\a\\b\\f\\n\\r\\t\\v",
                    std::string("\\'\\\"\\\?\\\\\\a\\b\\f\\n\\r\\t\\v"));

  std::string zeroOne;
  zeroOne.push_back('\x00');
  zeroOne.push_back('\x01');
  zeroOne.push_back('\x11');
  testSingleLiteral(p, "test10.chpl", "\\x00\\x01\\x11", zeroOne);

  testTripleLiteral(p, "test12.chpl", "\\x00\\x01", std::string("\\x00\\x01"));

  std::string t20;
  t20.push_back('\x12');
  t20.push_back('3');
  t20.push_back('4');
  testSingleLiteral(p, "test20.chpl", "\\x1234", t20);

  std::string t21;
  t21.push_back('\x0a');
  t21.push_back('g');
  testSingleLiteral(p, "test21.chpl", "\\xag", t21);

  testBadLiteral(p, "test30.chpl", "\"\\q\"");
  testBadLiteral(p, "test31.chpl", "\"\\u1234\"");
  testBadLiteral(p, "test32.chpl", "\"\\xG\"");

  return 0;
}
