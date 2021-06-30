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

#include "chpl/uast/Array.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Defer.h"
#include "chpl/uast/Domain.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/IntLiteral.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Range.h"
#include "chpl/uast/Variable.h"
#include "chpl/parsing/Parser.h"
#include "chpl/queries/Context.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <array>
#include <cassert>

using namespace chpl;
using namespace uast;
using namespace parsing;

static void testRange(Parser* parser, const char* testName,
                      const char* intervalStr,
                      bool hasLowerBound,
                      bool hasUpperBound) {
  const char* lowerStr = hasLowerBound ? "0" : "";
  const char* upperStr = hasUpperBound ? "8" : "";

  std::string test = "var r = ";
  test += lowerStr;
  test += intervalStr;
  test += upperStr;
  test += ";\n";

  auto parseResult = parser->parseString(testName, test.c_str());

  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isVariable());
  const Variable* var = mod->stmt(0)->toVariable();
  assert(!var->typeExpression());
  assert(var->initExpression() && var->initExpression()->isRange());
  const Range* range = var->initExpression()->toRange();

  // Check the interval kind.
  if (!strcmp(intervalStr, "..<")) {
    assert(range->intervalKind() == Range::OPEN_HIGH);
  } else if (!strcmp(intervalStr, "..")) {
    assert(range->intervalKind() == Range::CLOSED);
  } else {
    assert(false);
  }

  if (hasLowerBound) {
    assert(range->lowerBound() && range->lowerBound()->isIntLiteral());
    assert(range->lowerBound()->toIntLiteral()->text() == lowerStr);
  }

  if (hasUpperBound) {
    assert(range->upperBound() && range->upperBound()->isIntLiteral());
    assert(range->upperBound()->toIntLiteral()->text() == upperStr);
  }
}

// TODO: Check for trailing comma?
static void testArrayDomain(Parser* parser, const char* testName,
                            bool isArray,
                            int numElements,
                            bool hasTrailingComma) {
  // These initializers must have at least one element.
  assert(numElements > 0);

  std::string test = isArray ? "var a = " : "var d = ";
  test += isArray ? "[" : "{";

  for (int i = 0; i < numElements-1; i++) {
    test += std::to_string(i);
    test += ", ";
  }

  test += std::to_string(numElements-1);
  if (hasTrailingComma) {
    test += ",";
  }

  test += isArray ? "]" : "}";
  test += ";\n";

  auto parseResult = parser->parseString(testName, test.c_str());

  assert(parseResult.errors.size() == 0);
  assert(parseResult.topLevelExpressions.size() == 1);
  assert(parseResult.topLevelExpressions[0]->isModule());
  auto mod = parseResult.topLevelExpressions[0]->toModule();
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isVariable());
  const Variable* var = mod->stmt(0)->toVariable();
  assert(!var->typeExpression());
  assert(var->initExpression());

  // If we used a common parent we could flatten this.
  if (const Array* a = var->initExpression()->toArray()) {
    assert(isArray);
    assert(a->numExprs() == numElements);
    for (int i = 0; i < a->numExprs(); i++) {
      assert(a->expr(i)->isIntLiteral());
      assert(a->expr(i)->toIntLiteral()->value() == i);
    }
  } else if (const Domain* d = var->initExpression()->toDomain()) {
    assert(!isArray);
    assert(d->numExprs() == numElements);
    for (int i = 0; i < d->numExprs(); i++) {
      assert(d->expr(i)->isIntLiteral());
      assert(d->expr(i)->toIntLiteral()->value() == i);
    }
  } else {
    assert(false && "Should not reach here");
  }

  auto exprs = isArray ? var->initExpression()->toArray()->exprs()
                       : var->initExpression()->toDomain()->exprs();

  // Check the array/domain iterator.
  int i = 0;
  for (auto expr : exprs) {
    assert(expr->isIntLiteral());
    assert(expr->toIntLiteral()->value() == i);
    i++;
  }
  assert(i == numElements);
}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  testRange(p, "testRange0.chpl", "..", true, true);
  testRange(p, "testRange1.chpl", "..", true, false);
  testRange(p, "testRange2.chpl", "..", false, false);
  testRange(p, "testRange3.chpl", "..", false, true);
  testRange(p, "testRange4.chpl", "..<", true, true);
  testRange(p, "testRange5.chpl", "..<", false, true);
 
  testArrayDomain(p, "testArray0.chpl", true, 1, false);
  testArrayDomain(p, "testArray1.chpl", true, 1, true);
  testArrayDomain(p, "testArray2.chpl", true, 8, false);
  testArrayDomain(p, "testArray3.chpl", true, 8, true);

  testArrayDomain(p, "testDomain0.chpl", false, 1, false);
  testArrayDomain(p, "testDomain1.chpl", false, 1, true);
  testArrayDomain(p, "testDomain2.chpl", false, 8, false);
  testArrayDomain(p, "testDomain3.chpl", false, 8, true);

  return 0;
}
