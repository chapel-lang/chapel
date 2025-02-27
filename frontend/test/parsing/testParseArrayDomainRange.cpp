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

#include "chpl/uast/Array.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Defer.h"
#include "chpl/uast/Domain.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/IntLiteral.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Range.h"
#include "chpl/uast/Variable.h"
#include "chpl/parsing/Parser.h"
#include "chpl/framework/Context.h"

#include <array>

static void testRange(Parser* parser, const char* testName,
                      const char* intervalStr,
                      bool hasLowerBound,
                      bool hasUpperBound) {
  ErrorGuard guard(parser->context());
  const char* lowerStr = hasLowerBound ? "0" : "";
  const char* upperStr = hasUpperBound ? "8" : "";

  std::string test = "var r = ";
  test += lowerStr;
  test += intervalStr;
  test += upperStr;
  test += ";\n";

  auto parseResult = parseStringAndReportErrors(parser, testName, test.c_str());

  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isVariable());
  const Variable* var = mod->stmt(0)->toVariable();
  assert(!var->typeExpression());
  assert(var->initExpression() && var->initExpression()->isRange());
  const Range* range = var->initExpression()->toRange();

  // Check the interval kind.
  if (!strcmp(intervalStr, "..<")) {
    assert(range->opKind() == Range::OPEN_HIGH);
  } else if (!strcmp(intervalStr, "..")) {
    assert(range->opKind() == Range::DEFAULT);
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

static void testArrayDomain(Parser* parser, const char* testName,
                            bool isArray,
                            int numElements,
                            bool hasTrailingComma) {
  // These initializers must have at least one element.
  assert(numElements > 0);

  ErrorGuard guard(parser->context());
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

  auto parseResult = parseStringAndReportErrors(parser, testName, test.c_str());

  assert(!guard.realizeErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
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
    assert(a->hasTrailingComma() == hasTrailingComma);
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

template<size_t N>
static std::string getNDArrayRows(std::array<int, N> shape,
                                  int idx, bool trailingCommas) {
  if (idx == N-1) {
    std::string test = "";
    std::string sep;
    for (int i = 0; i < shape[idx]; i++) {
      test += sep + std::to_string(i);
      sep = ", ";
    }
    if (trailingCommas) {
      test += ",";
    }
    return test;
  } else {
    std::string test = "";
    std::string sep;
    for (int i = 0; i < shape[idx]; i++) {

      test += sep + getNDArrayRows(shape, idx+1, trailingCommas);
      sep = std::string(N-idx-1, ';');
    }
    return test;
  }
}

template<size_t N>
static void testNDArrayShape(Parser* parser,
                             const char* testName,
                             std::array<int, N> shape,
                             bool trailingCommas) {

  ErrorGuard guard(parser->context());
  std::string test = "var a = ";
  test += "[" + getNDArrayRows(shape, 0, trailingCommas);
  if (shape[0] == 1) test += ";";
  test += "]";
  test += ";\n";

  auto parseResult = parseStringAndReportErrors(parser, testName, test.c_str());

  assert(!guard.realizeErrors());

  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isVariable());
  auto init = mod->stmt(0)->toVariable()->initExpression();
  assert(init->isArray());
  auto arr = init->toArray();

  std::function<void(const AstNode*, decltype(shape), int)> checkerFunc;
  checkerFunc = [&checkerFunc](const AstNode* expr, auto shape, int idx) {
    if (idx == shape.size()-1) {
      auto row = expr->toArrayRow();
      assert(row);
      assert(row->numExprs() == shape[idx]);
      for (auto expr : row->exprs()) {
        assert(expr->isIntLiteral());
      }
    } else {
      if (idx == 0) {
        assert(expr->isArray());
      } else {
        assert(expr->isArrayRow());
      }
      assert(expr->numChildren() == shape[idx]);
      for (auto expr : expr->children()) {
        checkerFunc(expr, shape, idx+1);
      }
    }
  };

  checkerFunc(arr, shape, 0);

}

static void testNDArrayLiteral(Parser* parser,
                             const char* testName,
                             const char* literal,
                             int nArrayRows) {

  ErrorGuard guard(parser->context());
  std::string test = "var a = ";
  test += literal;
  test += ";\n";

  auto parseResult = parseStringAndReportErrors(parser, testName, test.c_str());

  assert(!guard.realizeErrors());

  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
  assert(mod->stmt(0)->isVariable());
  auto init = mod->stmt(0)->toVariable()->initExpression();
  assert(init->isArray());
  auto arr = init->toArray();

  std::function<int(const AstNode*)> counterFunc;
  counterFunc = [&counterFunc](const AstNode* expr) {
    int counter = expr->isArrayRow() ? 1 : 0;
    for (auto expr : expr->children()) {
      counter += counterFunc(expr);
    }
    return counter;
  };
  assert(nArrayRows == counterFunc(arr));
}

static void testNDArrayError(Parser* parser,
                             const char* testName,
                             const char* literal,
                            int numErrors) {

  ErrorGuard guard(parser->context());
  std::string test = "var a = ";
  test += literal;
  test += ";\n";

  auto parseResult = parseStringAndReportErrors(parser, testName, test.c_str());

  assert(guard.realizeErrors() == numErrors);
}


int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::createForTopLevelModule(ctx);
  Parser* p = &parser;

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

  testNDArrayShape(p, "testNDArray0.chpl", std::array{2, 2}, false);
  testNDArrayShape(p, "testNDArray1.chpl", std::array{1, 8}, false);
  testNDArrayShape(p, "testNDArray2.chpl", std::array{5, 1}, false);
  testNDArrayShape(p, "testNDArray3.chpl", std::array{3, 5, 7}, false);
  testNDArrayShape(p, "testNDArray3.chpl", std::array{3, 5, 7, 9, 11}, false);
  testNDArrayShape(p, "testNDArray4.chpl", std::array{2, 2}, true);
  // this case is and should be a syntax error, checked later
  // testNDArrayShape(p, "testNDArray5.chpl", std::array{1, 8}, true);
  testNDArrayShape(p, "testNDArray6.chpl", std::array{5, 1}, true);
  testNDArrayShape(p, "testNDArray7.chpl", std::array{3, 5, 7}, true);
  testNDArrayShape(p, "testNDArray3.chpl", std::array{3, 5, 7, 9, 11}, true);

  testNDArrayLiteral(p, "testNDArray8.chpl", "[0;]", 1);
  testNDArrayLiteral(p, "testNDArray9.chpl", "[0,;]", 1);
  testNDArrayLiteral(p, "testNDArray10.chpl", "[0, 1, 2;]", 1);
  testNDArrayLiteral(p, "testNDArray11.chpl", "[0, 1, 2,;]", 1);
  testNDArrayLiteral(p, "testNDArray12.chpl", "[0, 1; 2, 3]", 2);
  testNDArrayLiteral(p, "testNDArray13.chpl", "[0, 1; 2, 3;]", 2);
  testNDArrayLiteral(p, "testNDArray14.chpl", "[0; 1; 2; 3]", 4);
  testNDArrayLiteral(p, "testNDArray15.chpl", "[0,; 1,; 2,; 3,]", 4);
  testNDArrayLiteral(p, "testNDArray16.chpl", "[0; 1; 2; 3;]", 4);
  testNDArrayLiteral(p, "testNDArray17.chpl", "[0,; 1,; 2,; 3,;]", 4);

  testNDArrayError(p, "testNDArray18.chpl", "[0, 1 ; 2, 3; ;]", 1);
  testNDArrayError(p, "testNDArray19.chpl", "[0, 1; 2]", 1);
  testNDArrayError(p, "testNDArray20.chpl", "[0; 2, 2]", 1);
  testNDArrayError(p, "testNDArray21.chpl", "[0, 1; 2, 3;; 4, ; 6, 7]", 1);

  return 0;
}
