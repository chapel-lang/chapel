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
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

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


// assumes the last statement is a variable declaration for x.
// returns the type of that.
static QualifiedType
parseTypeOfX(Context* context, const char* program) {
  auto m = parseModule(context, program);
  assert(m->numStmts() > 0);
  const Variable* x = m->stmt(m->numStmts()-1)->toVariable();
  assert(x);
  assert(x->name() == "x");

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  auto qt = rr.byAst(x).type();
  assert(qt.type());

  return qt;
}

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = parseTypeOfXInit(context,
                R""""(
                  var x = (1, 2);
                )"""");

  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();
  assert(tt->numElements() == 2);
  assert(tt->elementType(0) == tt->elementType(1));
  assert(tt->elementType(0).kind() == QualifiedType::VAR);
  assert(tt->elementType(0).type()->isIntType());

  auto rt = tt->toReferentialTuple(context);
  assert(rt == tt);

  auto vt = tt->toValueTuple(context);
  assert(vt == tt);
}

static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = parseTypeOfXInit(context,
                R""""(
                  type x = (int, int);
                )"""");

  assert(qt.kind() == QualifiedType::TYPE);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();
  assert(tt->numElements() == 2);
  assert(tt->elementType(0) == tt->elementType(1));
  assert(tt->elementType(0).kind() == QualifiedType::VAR);
  assert(tt->elementType(0).type()->isIntType());
}


static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = parseTypeOfXInit(context,
                R""""(
                  record R { }
                  var r: R;
                  var x = (r, r);
                )"""");

  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();
  assert(tt->numElements() == 2);
  assert(tt->elementType(0) == tt->elementType(1));
  assert(tt->elementType(0).kind() == QualifiedType::REF);
  assert(tt->elementType(0).type()->isRecordType());

  auto rt = tt->toReferentialTuple(context);
  assert(rt == tt);

  auto vt = tt->toValueTuple(context);
  assert(vt != tt);
  assert(vt->numElements() == 2);
  assert(vt->elementType(0) == vt->elementType(1));
  assert(vt->elementType(0).kind() == QualifiedType::VAR);
  assert(vt->elementType(0).type()->isRecordType());
}

static void test4() {
  printf("test4\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = parseTypeOfXInit(context,
                R""""(
                  record R { }
                  type x = (R, R);
                )"""");

  assert(qt.kind() == QualifiedType::TYPE);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();
  assert(tt->numElements() == 2);
  assert(tt->elementType(0) == tt->elementType(1));
  assert(tt->elementType(0).kind() == QualifiedType::VAR);
  assert(tt->elementType(0).type()->isRecordType());

  auto rt = tt->toReferentialTuple(context);
  assert(rt != tt);

  auto vt = tt->toValueTuple(context);
  assert(vt == tt);
}

static void test5() {
  printf("test5\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = parseTypeOfX(context,
                R""""(
                  record R { }
                  var r: R;
                  var x = (r, r);
                )"""");

  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();
  assert(tt->numElements() == 2);
  assert(tt->elementType(0) == tt->elementType(1));
  assert(tt->elementType(0).kind() == QualifiedType::VAR);
  assert(tt->elementType(0).type()->isRecordType());

  auto rt = tt->toReferentialTuple(context);
  assert(rt != tt);

  auto vt = tt->toValueTuple(context);
  assert(vt == tt);
}


int main() {
  test1();
  test2();
  test3();
  test4();
  test5();

  return 0;
}
