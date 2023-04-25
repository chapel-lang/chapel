/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "test-resolution.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/TupleDecl.h"
#include "chpl/uast/Variable.h"

// assumes the last statement is a variable declaration for x.
// returns the type of that.
static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = resolveTypeOfXInit(context,
                R""""(
                  var x = (1, 2);
                )"""");

  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();
  assert(tt->isStarTuple() == true);
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

  auto qt = resolveTypeOfXInit(context,
                R""""(
                  type x = (int, real);
                )"""");

  assert(qt.kind() == QualifiedType::TYPE);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();
  assert(tt->isStarTuple() == false);
  assert(tt->numElements() == 2);
  assert(tt->elementType(0).kind() == QualifiedType::VAR);
  assert(tt->elementType(0).type()->isIntType());
  assert(tt->elementType(1).kind() == QualifiedType::VAR);
  assert(tt->elementType(1).type()->isRealType());
}


static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = resolveTypeOfXInit(context,
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

  auto qt = resolveTypeOfXInit(context,
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

  auto qt = resolveQualifiedTypeOfX(context,
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

static void test6() {
  printf("test6\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = resolveTypeOfXInit(context,
                R""""(
                  record R { }
                  proc R.init() { }
                  proc R.deinit() { }
                  proc f() {
                    var r: R;
                    return (r, r);
                  }
                  var x = f();
                )"""");

  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();
  auto rt = tt->toReferentialTuple(context);
  assert(rt != tt);
  auto vt = tt->toValueTuple(context);
  assert(vt == tt);
}

static void test7() {
  printf("test7\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = resolveTypeOfXInit(context,
                R""""(
                  record R { }
                  var r: R;
                  var glob = (r, r);
                  proc f() ref {
                    return glob;
                  }
                  var x = f();
                )"""");

  assert(qt.kind() == QualifiedType::REF);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();
  auto rt = tt->toReferentialTuple(context);
  assert(rt != tt);
  auto vt = tt->toValueTuple(context);
  assert(vt == tt);
}

static void test8() {
  printf("test8\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = resolveTypeOfXInit(context,
                R""""(
                  record R { }
                  proc f() : (R, R) {
                    var r: R;
                    return (r, r);
                  }
                  var x = f();
                )"""");

  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();
  auto rt = tt->toReferentialTuple(context);
  assert(rt != tt);
  auto vt = tt->toValueTuple(context);
  assert(vt == tt);
}

static void test9() {
  printf("test9\n");
  Context ctx;
  Context* context = &ctx;

  auto M = parseModule(context,
                R""""(
                  var (a, (b, c)) = (1, (2.0, 3));
                )"""");

  assert(M->numStmts() == 1);
  const TupleDecl* td = M->stmt(0)->toTupleDecl();
  assert(td);
  auto a = td->decl(0)->toVariable();
  assert(a);
  auto innerTd = td->decl(1)->toTupleDecl();
  assert(innerTd);
  auto b = innerTd->decl(0)->toVariable();
  assert(b);
  auto c = innerTd->decl(1)->toVariable();
  assert(c);

  const ResolutionResultByPostorderID& rr = resolveModule(context, M->id());
  auto aQt = rr.byAst(a).type();
  auto bQt = rr.byAst(b).type();
  auto cQt = rr.byAst(c).type();
  assert(aQt.kind() == QualifiedType::VAR);
  assert(aQt.type() == IntType::get(context, 0));
  assert(bQt.kind() == QualifiedType::VAR);
  assert(bQt.type() == RealType::get(context, 0));
  assert(cQt.kind() == QualifiedType::VAR);
  assert(cQt.type() == IntType::get(context, 0));
}

static void test10() {
  printf("test10\n");
  Context ctx;
  Context* context = &ctx;

  auto M = parseModule(context,
                R""""(
                  const (a, (b, c)) = (1, (2.0, 3));
                )"""");

  assert(M->numStmts() == 1);
  const TupleDecl* td = M->stmt(0)->toTupleDecl();
  assert(td);
  auto a = td->decl(0)->toVariable();
  assert(a);
  auto innerTd = td->decl(1)->toTupleDecl();
  assert(innerTd);
  auto b = innerTd->decl(0)->toVariable();
  assert(b);
  auto c = innerTd->decl(1)->toVariable();
  assert(c);

  const ResolutionResultByPostorderID& rr = resolveModule(context, M->id());
  auto aQt = rr.byAst(a).type();
  auto bQt = rr.byAst(b).type();
  auto cQt = rr.byAst(c).type();
  assert(aQt.kind() == QualifiedType::CONST_VAR);
  assert(aQt.type() == IntType::get(context, 0));
  assert(bQt.kind() == QualifiedType::CONST_VAR);
  assert(bQt.type() == RealType::get(context, 0));
  assert(cQt.kind() == QualifiedType::CONST_VAR);
  assert(cQt.type() == IntType::get(context, 0));
}

static void test11() {
  printf("test11\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = resolveTypeOfXInit(context,
                R""""(
                  proc f(in arg: (real, real)) { return arg; }
                  var x = f( (1,2) );
                )"""");

  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();

  assert(tt->numElements() == 2);
  assert(tt->isStarTuple());
  assert(tt->elementType(0) == tt->elementType(1));
  assert(tt->elementType(0).kind() == QualifiedType::VAR);
  assert(tt->elementType(0).type()->isRealType());
}

static void test12() {
  printf("test12\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = resolveTypeOfXInit(context,
                R""""(
                  record R { }
                  proc f(in arg: (R, R)) { return arg; }
                  var r: R;
                  var x = f( (r, r) );
                )"""");


  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();

  assert(tt->numElements() == 2);
  assert(tt->isStarTuple());
  assert(tt->elementType(0) == tt->elementType(1));
  assert(tt->elementType(0).kind() == QualifiedType::VAR);
  assert(tt->elementType(0).type()->isRecordType());

  auto rt = tt->toReferentialTuple(context);
  assert(rt != tt);
  auto vt = tt->toValueTuple(context);
  assert(vt == tt);
}

static void test13() {
  printf("test13\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = resolveTypeOfXInit(context,
                R""""(
                  record R { param p; }
                  proc f(in arg: (R, R)) { return arg; }
                  var r: R(1);
                  var x = f( (r, r) );
                )"""");


  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();

  assert(tt->numElements() == 2);
  assert(tt->isStarTuple());
  assert(tt->elementType(0) == tt->elementType(1));
  assert(tt->elementType(0).kind() == QualifiedType::VAR);
  assert(tt->elementType(0).type()->isRecordType());
  auto rec = tt->elementType(0).type()->toRecordType();
  assert(rec->instantiatedFrom() != nullptr);

  auto rt = tt->toReferentialTuple(context);
  assert(rt != tt);
  auto vt = tt->toValueTuple(context);
  assert(vt == tt);
}

static void test14() {
  printf("test14\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = resolveTypeOfXInit(context,
                R""""(
                  proc f(x: int, (y, z): (real, int)) { return (x, y, z); }
                  var x = f( 1, (2.0, 3) );
                )"""");

  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();

  assert(tt->numElements() == 3);
  assert(!tt->isStarTuple());
  assert(tt->elementType(0).type()->isIntType());
  assert(tt->elementType(1).type()->isRealType());
  assert(tt->elementType(2).type()->isIntType());
}

static void test15() {
  printf("test15\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = resolveTypeOfXInit(context,
                R""""(
                  var tup = (1, 2);
                  var x = ( (... tup), 3.0);
                )"""");

  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();

  assert(tt->numElements() == 3);
  assert(!tt->isStarTuple());
  assert(tt->elementType(0).type()->isIntType());
  assert(tt->elementType(1).type()->isIntType());
  assert(tt->elementType(2).type()->isRealType());
}

static void test16() {
  printf("test16\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = resolveQualifiedTypeOfX(context,
                R""""(
                  proc helper(a: int, b: real) { return b; }
                  var tup = (1, 2.0);
                  var x = helper( (... tup) );
                )"""");

  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type()->isRealType());
}

static void test17() {
  printf("test17\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = resolveQualifiedTypeOfX(context,
                R""""(
                  var x : 3*int;
                )"""");

  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type()->isTupleType());
  auto tt = qt.type()->toTupleType();

  assert(tt->numElements() == 3);
  assert(tt->isStarTuple());
  assert(tt->elementType(0).type()->isIntType());
  assert(tt->elementType(1).type()->isIntType());
  assert(tt->elementType(2).type()->isIntType());
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  test10();
  test11();
  test12();
  test13();
  test14();
  test15();
  test16();
  test17();

  return 0;
}
