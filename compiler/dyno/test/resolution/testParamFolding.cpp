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

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace parsing;
using namespace resolution;
using namespace types;
using namespace uast;

// error handler that causes the test to fail if it runs
static void reportError(const ErrorMessage& err) {
  printf("error encountered - %s\n", err.message().c_str());
  assert(false && "fatal error");
}

static const Module* parseModule(Context* context, const char* src) {
  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = src;
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);

  return vec[0];
}

// assumes the last statement is a variable declaration for x
// with an initialization expression.
// Returns the type of the initializer expression.
static QualifiedType
parseTypeOfXInit(Context* context, const char* program) {
  auto m = parseModule(context, program);
  assert(m->numStmts() > 0);
  const Variable* x = m->stmt(m->numStmts()-1)->toVariable();
  assert(x);
  assert(x->name() == "x");
  auto initExpr = x->initExpression();
  assert(initExpr);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  auto qt = rr.byAst(initExpr).type();
  assert(qt.type());

  return qt;
}

static void test1() {
  Context ctx;
  Context* context = &ctx;

  // configure context to fail test if there are any errors
  context->setErrorHandler(reportError);

  QualifiedType qt = parseTypeOfXInit(context, "var x = true || f();");
  assert(qt.isParamTrue());
}

static void test2() {
  Context ctx;
  Context* context = &ctx;

  // configure context to fail test if there are any errors
  context->setErrorHandler(reportError);

  QualifiedType qt = parseTypeOfXInit(context, "var x = false && f();");
  assert(qt.isParamFalse());
}

static void test3() {
  Context ctx;
  Context* context = &ctx;

  // 2nd argument param should not be folded

  // configure context to fail test if there are any errors
  context->setErrorHandler(reportError);

  QualifiedType qt = parseTypeOfXInit(context,
                                      "var a: bool; var x = a || true;");
  assert(!qt.isParam() && !qt.hasParamPtr());
  assert(qt.type() == BoolType::get(context, 0));
}

static void test4() {
  Context ctx;
  Context* context = &ctx;

  // configure context to fail test if there are any errors
  context->setErrorHandler(reportError);

  // 2nd argument param should not be folded
  QualifiedType qt = parseTypeOfXInit(context,
                                      "var a: bool; var x = a && false;");
  assert(!qt.isParam() && !qt.hasParamPtr());
  assert(qt.type() == BoolType::get(context, 0));
}

int main() {
  test1();
  test2();
  test3();
  test4();

  return 0;
}

