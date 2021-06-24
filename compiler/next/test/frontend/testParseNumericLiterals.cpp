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
#include "chpl/uast/ImagLiteral.h"
#include "chpl/uast/IntLiteral.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/RealLiteral.h"
#include "chpl/uast/UintLiteral.h"
#include "chpl/uast/Variable.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace uast;
using namespace frontend;

static void testIntLiteral(Parser* parser,
                           const char* testname,
                           const char* lit,
                           int64_t expectValue) {
  std::string toparse = "var x = ";
  toparse += lit;
  toparse += ";\n";
  auto parseResult = parser->parseString(testname, toparse.c_str());
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto variable = module->stmt(0)->toVariable();
  assert(variable);
  auto intLit = variable->initExpression()->toIntLiteral();
  assert(intLit);
  assert(intLit->value() == expectValue);
  assert(0 == intLit->text().compare(lit));
}
static void testUintLiteral(Parser* parser,
                            const char* testname,
                            const char* lit,
                            uint64_t expectValue) {
  std::string toparse = "var x = ";
  toparse += lit;
  toparse += ";\n";
  auto parseResult = parser->parseString(testname, toparse.c_str());
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto variable = module->stmt(0)->toVariable();
  assert(variable);
  auto uintLit = variable->initExpression()->toUintLiteral();
  assert(uintLit);
  assert(uintLit->value() == expectValue);
  assert(0 == uintLit->text().compare(lit));
}
static void testRealLiteral(Parser* parser,
                            const char* testname,
                            const char* lit,
                            double expectValue) {
  std::string toparse = "var x = ";
  toparse += lit;
  toparse += ";\n";
  auto parseResult = parser->parseString(testname, toparse.c_str());
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto variable = module->stmt(0)->toVariable();
  assert(variable);
  auto realLit = variable->initExpression()->toRealLiteral();
  assert(realLit);
  assert(realLit->value() == expectValue);
  assert(0 == realLit->text().compare(lit));
}
static void testImagLiteral(Parser* parser,
                            const char* testname,
                            const char* lit,
                            double expectValue) {
  std::string toparse = "var x = ";
  toparse += lit;
  toparse += ";\n";
  auto parseResult = parser->parseString(testname, toparse.c_str());
  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto variable = module->stmt(0)->toVariable();
  assert(variable);
  auto imagLit = variable->initExpression()->toImagLiteral();
  assert(imagLit);
  assert(imagLit->value() == expectValue);
  assert(0 == imagLit->text().compare(lit));
}


static void testBadLiteral(Parser* parser,
                           const char* testname,
                           const char* lit) {
  std::string toparse = "var x = ";
  toparse += lit;
  toparse += ";\n";
  auto parseResult = parser->parseString(testname, toparse.c_str());
  assert(parseResult.errors.size() > 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto module = parseResult.topLevelExpressions[0]->toModule();
  assert(module->numStmts() == 1);
  auto variable = module->stmt(0)->toVariable();
  assert(variable);
  assert(variable->initExpression()->isErroneousExpression());
}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  testIntLiteral(p, "testAb.chpl", "0b0", 0);
  testIntLiteral(p, "testAB.chpl", "0b0", 0);
  testIntLiteral(p, "testAd.chpl", "0", 0);
  testIntLiteral(p, "testAo.chpl", "0o0", 0);
  testIntLiteral(p, "testAO.chpl", "0O0", 0);
  testIntLiteral(p, "testAh.chpl", "0x0", 0);
  testIntLiteral(p, "testAH.chpl", "0X0", 0);

  testIntLiteral(p, "testBd.chpl", "1", 1);

  testIntLiteral(p, "testCb.chpl", "0b10", 2);
  testIntLiteral(p, "testCB.chpl", "0B10", 2);
  testIntLiteral(p, "testCd.chpl", "10", 10);
  testIntLiteral(p, "testCo.chpl", "0o10", 8);
  testIntLiteral(p, "testCO.chpl", "0O10", 8);
  testIntLiteral(p, "testCh.chpl", "0x10", 16);
  testIntLiteral(p, "testCH.chpl", "0X10", 16);

  testIntLiteral(p, "testDd.chpl", "4294967296", 4294967296);
  testIntLiteral(p, "testEe.chpl",
                 "4611686018427387904", 4611686018427387904);

  testUintLiteral(p, "testFb.chpl",
      "0b1111111111111111111111111111111111111111111111111111111111111111",
      18446744073709551615ull);
  testUintLiteral(p, "testFB.chpl",
      "0B1111111111111111111111111111111111111111111111111111111111111111",
      18446744073709551615ull);
  testUintLiteral(p, "testFd.chpl",
                 "18446744073709551615", 18446744073709551615ull);
  testUintLiteral(p, "testFo.chpl",
                 "0o1777777777777777777777", 18446744073709551615ull);
  testUintLiteral(p, "testFO.chpl",
                 "0O1777777777777777777777", 18446744073709551615ull);
  testUintLiteral(p, "testFh.chpl",
                 "0xFFFFFFFFFFFFFFFF", 18446744073709551615ull);
  testUintLiteral(p, "testFh2.chpl",
                 "0xffffffffffffffff", 18446744073709551615ull);
  testUintLiteral(p, "testFH.chpl",
                 "0XFFFFFFFFFFFFFFFF", 18446744073709551615ull);
  testUintLiteral(p, "testFH2.chpl",
                 "0Xffffffffffffffff", 18446744073709551615ull);

  testIntLiteral(p, "testGb.chpl",
      "0b111111111111111111111111111111111111111111111111111111111111111",
      9223372036854775807ull);
  testIntLiteral(p, "testGB.chpl",
      "0B111111111111111111111111111111111111111111111111111111111111111",
      9223372036854775807ull);
  testIntLiteral(p, "testGd.chpl",
                 "9223372036854775807", 9223372036854775807ull);
  testIntLiteral(p, "testGo.chpl",
                 "0o777777777777777777777", 9223372036854775807ull);
  testIntLiteral(p, "testGO.chpl",
                 "0O777777777777777777777", 9223372036854775807ull);
  testIntLiteral(p, "testGh.chpl",
                 "0x7FFFFFFFFFFFFFFF", 9223372036854775807ull);
  testIntLiteral(p, "testGh2.chpl",
                 "0x7fffffffffffffff", 9223372036854775807ull);
  testIntLiteral(p, "testGH.chpl",
                 "0X7FFFFFFFFFFFFFFF", 9223372036854775807ull);
  testIntLiteral(p, "testGH2.chpl",
                 "0X7fffffffffffffff", 9223372036854775807ull);

  testRealLiteral(p, "testR0.chpl", "0.0", 0.0);
  testRealLiteral(p, "testR1.chpl", "1.0", 1.0);
  testRealLiteral(p, "testR15.chpl", "1.5", 1.5);
  testRealLiteral(p, "testR15e2.chpl", "1.5e2", 150);
  testRealLiteral(p, "testR15em2.chpl", "1.5e-2", 0.015);
  testRealLiteral(p, "testR2e2.chpl", "2e2", 200.0);
  testRealLiteral(p, "testRx2e2.chpl", "0x2p2", 8.0);
  testRealLiteral(p, "testRx11.chpl", "0x011.1", 17.0625);
  testRealLiteral(p, "testRx11p2.chpl", "0x011.1p2", 68.25);
  testRealLiteral(p, "testRx11pm2.chpl", "0x01.1p-2", 0.265625);

  testImagLiteral(p, "testI0.chpl", "0.0i", 0.0);
  testImagLiteral(p, "testI1.chpl", "1.0i", 1.0);
  testImagLiteral(p, "testI15.chpl", "1.5i", 1.5);
  testImagLiteral(p, "testI15e2.chpl", "1.5e2i", 150);
  testImagLiteral(p, "testI15em2.chpl", "1.5e-2i", 0.015);
  testImagLiteral(p, "testI2e2.chpl", "2e2i", 200.0);
  testImagLiteral(p, "testIx2e2.chpl", "0x2p2i", 8.0);
  testImagLiteral(p, "testIx11.chpl", "0x011.1i", 17.0625);
  testImagLiteral(p, "testIx11p2.chpl", "0x011.1p2i", 68.25);
  testImagLiteral(p, "testIx11pm2.chpl", "0x01.1p-2i", 0.265625);


  testBadLiteral(p, "testHb.chpl",
      "0b10000000000000000000000000000000000000000000000000000000000000000");
  testBadLiteral(p, "testHd.chpl", "18446744073709551616");
  testBadLiteral(p, "testHo.chpl", "0o2000000000000000000000");
  testBadLiteral(p, "testHh.chpl", "0x10000000000000000");

  return 0;
}
