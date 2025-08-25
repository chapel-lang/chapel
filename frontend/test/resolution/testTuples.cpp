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
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

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
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

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
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

  auto qt = resolveTypeOfXInit(context,
                R""""(
                  record R { }
                  var r: R;
                  var x = (r, r);
                )"""");

  assert(qt.kind() == QualifiedType::CONST_REF);
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
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

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
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

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
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

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
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

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
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

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
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

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

static void test9b() {
  printf("%s\n", __FUNCTION__);
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
      var (a, b) = (1, 2);

      var xa = a;
      var xb = b;
    )""";

    auto vars = resolveTypesOfVariables(context, program, {"xa", "xb"});
    assert(vars["xa"].type()->isIntType());
    assert(vars["xb"].type()->isIntType());
  }
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
    var ((a, b), (c, d)) = ((1, "hello"), (42.0, 5:uint));

    var xa = a;
    var xb = b;
    var xc = c;
    var xd = d;
    )""";

    auto vars = resolveTypesOfVariables(context, program,
                                        {"xa", "xb", "xc", "xd"});
    assert(vars["xa"].type()->isIntType());
    assert(vars["xb"].type()->isStringType());
    assert(vars["xc"].type()->isRealType());
    assert(vars["xd"].type()->isUintType());
  }
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string program = R"""(
    proc foo() {
      return ((1, ("two", 3.0)), ((4:uint, b"five"), 6.0i));
    }

    var ((a, (b, c)), ((d, e), f)) = foo();
    var xa = a;
    var xb = b;
    var xc = c;
    var xd = d;
    var xe = e;
    var xf = f;
    )""";

    auto vars = resolveTypesOfVariables(context, program,
                                        {"xa", "xb", "xc", "xd", "xe", "xf"});
    assert(vars["xa"].type()->isIntType());
    assert(vars["xb"].type()->isStringType());
    assert(vars["xc"].type()->isRealType());
    assert(vars["xd"].type()->isUintType());
    assert(vars["xe"].type()->isBytesType());
    assert(vars["xf"].type()->isImagType());
  }
}

static void test10() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

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
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

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

static void test11b() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();
  ErrorGuard guard(context);

  // Exercises a case where the frontend was attempting to resolve an '='
  // operator between "(a, b)" and "foo()", and naturally running into
  // constness errors. The Resolver handles this already, so we shouldn't ever
  // bother to resolve that assignment.
  auto t = resolveTypeOfX(context,
                R""""(
                record R { var i : int; }

                proc foo() {
                  return (new R(5), new R(42));
                }

                proc helper() {
                  var a : R;
                  var b : R;

                  (a, b) = foo();

                  return a;
                }

                var x = helper();
                )"""");

  assert(t->isRecordType());
}

static void test12() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

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
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

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

// TupleDecl formals
static void test14() {
  printf("%s\n", __FUNCTION__);

  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

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

  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    // Using a homogeneous tuple type expression
    auto vars = resolveTypesOfVariables(context,
                  R""""(
                    proc foo((x, y, z): 3*int) {
                      return x;
                    }

                    var three = (1, 2, 3);
                    var retOne = foo(three);

                    var a, b, c : int;

                    var retTwo = foo((a, b, c));
                  )"""",
                  {"retOne", "retTwo"});
    assert(vars["retOne"].type()->isIntType());
    assert(vars["retTwo"].type()->isIntType());
  }
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    auto qt = resolveTypeOfXInit(context,
                  R""""(
                  proc blah(type (a, b, c)) {
                    var ret : a;
                    return ret;
                  }

                  var x = blah((int, real, string));

                  )"""");

    assert(qt.isErroneousType());
    assert(guard.numErrors() == 2);

    // From post-parse-checks
    auto& err = guard.error(0);
    assert(err->message() == "intents on tuple-grouped arguments are not yet supported");

    assert(guard.error(1)->type() == ErrorType::NoMatchingCandidates);

    guard.realizeErrors();
  }
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    auto vars = resolveTypesOfVariables(context,
                  R""""(
                  proc baz((x, y, z), param which : int) {
                    if which == 0 then return x;
                    else if which == 1 then return y;
                    else if which == 2 then return z;
                    else return none;
                  }

                  var litA = baz((1, 2.0, "test"), 0);
                  var litB = baz((1, 2.0, "test"), 1);
                  var litC = baz((1, 2.0, "test"), 2);

                  var three = (1, 2.0, "test");
                  var varA = baz(three, 0);
                  var varB = baz(three, 1);
                  var varC = baz(three, 2);
                  )"""",
                  {"litA", "litB", "litC", "varA", "varB", "varC"});
    assert(vars["litA"].type()->isIntType());
    assert(vars["litB"].type()->isRealType());
    assert(vars["litC"].type()->isStringType());

    assert(vars["varA"].type()->isIntType());
    assert(vars["varB"].type()->isRealType());
    assert(vars["varC"].type()->isStringType());
  }
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);

    auto vars = resolveTypesOfVariables(context,
                  R""""(
                  proc baz(((a, b), c, d), param which : int) {
                    if which == 0 then return a;
                    else if which == 1 then return b;
                    else if which == 2 then return c;
                    else if which == 3 then return d;
                    else return none;
                  }

                  var litA = baz(((1, 2.0), "test", 4:uint), 0);
                  var litB = baz(((1, 2.0), "test", 4:uint), 1);
                  var litC = baz(((1, 2.0), "test", 4:uint), 2);
                  var litD = baz(((1, 2.0), "test", 4:uint), 3);

                  var arg = ((1, 2.0), "test", 4:uint);
                  var varA = baz(arg, 0);
                  var varB = baz(arg, 1);
                  var varC = baz(arg, 2);
                  var varD = baz(arg, 3);
                  )"""",
                  {"litA", "litB", "litC", "litD",
                   "varA", "varB", "varC", "varD"});
    assert(vars["litA"].type()->isIntType());
    assert(vars["litB"].type()->isRealType());
    assert(vars["litC"].type()->isStringType());
    assert(vars["litD"].type()->isUintType());

    assert(vars["varA"].type()->isIntType());
    assert(vars["varB"].type()->isRealType());
    assert(vars["varC"].type()->isStringType());
    assert(vars["varD"].type()->isUintType());
  }
}

static void test15() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

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
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

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
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

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

static void argHelper(std::string formal, std::string actual,
                           bool shouldResolve) {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string program = R"""(
    proc foo(arg: )""" + formal + R"""() {
      return arg;
    }

    var actual = )""" + actual + R"""(;
    var x = foo(actual);
    )""";

  auto m = parseModule(context, std::move(program));
  auto x = findVariable(m, "x");
  auto a = findVariable(m ,"actual");

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  if (shouldResolve) {
    // 'x' and 'actual' should have the same type
    auto xt = rr.byAst(x).type();
    auto at = rr.byAst(a).type();
    assert(xt.isUnknown() == false);
    assert(xt.isErroneousType() == false);
    assert(xt.hasTypePtr());
    assert(xt.type()->isTupleType());
    assert(xt.type() == at.type());

    auto t = xt.type()->toTupleType();
    std::stringstream str;
    t->stringify(str, chpl::StringifyKind::CHPL_SYNTAX);
    printf("  success: passing %s to %s\n", str.str().c_str(), formal.c_str());
  } else {
    assert(guard.numErrors() == 1);
    assert(guard.error(0)->type() == chpl::NoMatchingCandidates);
    guard.clearErrors();
    printf("  success: cannot pass %s to %s\n", actual.c_str(), formal.c_str());
  }
}

static void test18() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

  auto program = R""""(
                  var t = (1, "hello", 3.0);
                  var x = t(0);
                  var y = t(1);
                  var z = t(2);
                )"""";

  auto m = parseModule(context, std::move(program));

  auto x = findVariable(m, "x");
  auto y = findVariable(m ,"y");
  auto z = findVariable(m ,"z");

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  assert(rr.byAst(x).type().type()->isIntType());
  assert(rr.byAst(y).type().type()->isStringType());
  assert(rr.byAst(z).type().type()->isRealType());
}

// same as test18, but with unsigned.
static void test18b() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

  auto program = R""""(
                  var t = (1, "hello", 3.0);
                  var x = t(0 : uint);
                  var y = t(1 : uint);
                  var z = t(2 : uint);
                )"""";

  auto m = parseModule(context, std::move(program));

  auto x = findVariable(m, "x");
  auto y = findVariable(m ,"y");
  auto z = findVariable(m ,"z");

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  assert(rr.byAst(x).type().type()->isIntType());
  assert(rr.byAst(y).type().type()->isStringType());
  assert(rr.byAst(z).type().type()->isRealType());
}

// Test "get svec member[ value]" primitives
static void test19() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();

  auto program = R""""(
                  var t = (1, 2, 3);
                  var x = t(0);
                  var y = t(1);
                  var z = t(2);

                  class Foo {
                    proc init() {}
                  }
                  var myFoo = new owned Foo();

                  var a = __primitive("get svec member", t, 0);
                  var b = __primitive("get svec member value", t, 0);
                  var c = __primitive("get svec member", (myFoo, myFoo), 1);
                  var d = __primitive("get svec member value", (myFoo, myFoo), 1);
                )"""";

  auto variables = resolveTypesOfVariablesInit(
      context, program, {"x", "y", "z", "a", "b", "c", "d"});

  assert(variables.at("x").type()->isIntType());
  assert(variables.at("y").type()->isIntType());
  assert(variables.at("z").type()->isIntType());

  assert(variables.at("a").kind() == QualifiedType::VAR);
  assert(variables.at("a").type()->isIntType());

  assert(variables.at("b").kind() == QualifiedType::VAR);
  assert(variables.at("b").type()->isIntType());

  assert(variables.at("c").kind() == QualifiedType::REF);
  assert(variables.at("c").type()->isClassType());

  assert(variables.at("d").kind() == QualifiedType::VAR);
  assert(variables.at("d").type()->isClassType());
}


static void testTupleGeneric() {
  printf("%s\n", __FUNCTION__);

  // Basic all-generic cases
  argHelper("(?,)", "(5,)", true);
  argHelper("(?,)", "('hello',)", true);
  argHelper("(?,?)", "(5, 1.0)", true);
  argHelper("(?,?)", "(5, 'hello')", true);
  argHelper("(?,?,?)", "(5, 1.0,'hello')", true);

  // Mixed concrete and generic, still expecting to pass
  argHelper("(int, ?)", "(5, 1.0)", true);
  argHelper("(int, ?)", "(5, 'hello')", true);
  argHelper("(int, ?)", "('hello', 5)", false);

  argHelper("(int, ?, string)", "(5, 42.0, 'hello')", true);
  argHelper("(int, ?, string)", "(5, 5, 'hello')", true);
  argHelper("(int, ?, string)", "(5, 'hi', 'hello')", true);
  argHelper("(int, ?, string)", "('hi', 42.0, 'hello')", false);
  argHelper("(int, ?, string)", "(5, 42.0, 5)", false);

  // Passing a tuples of incorrect size
  argHelper("(int, ?)", "(5,5,5)", false);
  argHelper("(int, ?, ?)", "(5,5)", false);

  // Some 'integral' and 'numeric' tests
  argHelper("(integral, integral)", "(5, 5)", true);
  argHelper("(integral, integral)", "(5, 'hello')", false);

  argHelper("(numeric, numeric)", "(5, 5)", true);
  argHelper("(numeric, numeric)", "(5, 42.0)", true);
  argHelper("(numeric, numeric)", "(5, 'hi')", false);
}

static const TypedFnSignature* test20Helper(Context* context, std::string program) {
  auto M = parseModule(context, program);
  auto rr = resolveModule(context, M->id());
  auto x = M->stmt(M->numStmts()-1)->toVarLikeDecl();
  auto call = x->initExpression()->toFnCall();

  ResolutionContext rcval(context);
  auto r = rr.byAstOrNull(call);
  auto candidate = r->mostSpecific().only();
  auto sig = candidate.fn();
  sig = resolveFunction(&rcval, sig, r->poiScope())->signature();
  return sig;
}
static void test20() {
  printf("%s\n", __FUNCTION__);
  {
    auto context = buildStdContext();
    auto program = R"""(
      proc foo(v: ?t) do return v;
      var x = foo((1, "hello"));
    )""";
    auto sig = test20Helper(context, program);
    assert(sig->numFormals() == 1);
    assert(sig->formalType(0).type()->isTupleType());
    assert(sig->formalType(0).kind() == QualifiedType::CONST_REF);
    auto tt = sig->formalType(0).type()->toTupleType();
    assert(tt->numElements() == 2);
    assert(tt->elementType(0).type()->isIntType());
    assert(tt->elementType(1).type()->isStringType());
    assert(tt->elementType(0).kind() == QualifiedType::VAR);
    assert(tt->elementType(1).kind() == QualifiedType::REF);
  }
  {
    auto context = buildStdContext();
    auto program = R"""(
      proc foo(const v: ?t) do return v;
      var x = foo((1, "hello"));
    )""";
    auto sig = test20Helper(context, program);
    assert(sig->numFormals() == 1);
    assert(sig->formalType(0).type()->isTupleType());
    assert(sig->formalType(0).kind() == QualifiedType::CONST_REF);
    auto tt = sig->formalType(0).type()->toTupleType();
    assert(tt->numElements() == 2);
    assert(tt->elementType(0).type()->isIntType());
    assert(tt->elementType(1).type()->isStringType());
    assert(tt->elementType(0).kind() == QualifiedType::CONST_VAR);
    assert(tt->elementType(1).kind() == QualifiedType::CONST_REF);
  }
  {
    auto context = buildStdContext();
    auto program = R"""(
      proc foo(in v: ?t) do return v;
      var x = foo((1, "hello"));
    )""";
    auto sig = test20Helper(context, program);
    assert(sig->numFormals() == 1);
    assert(sig->formalType(0).type()->isTupleType());
    assert(sig->formalType(0).kind() == QualifiedType::IN);
    auto tt = sig->formalType(0).type()->toTupleType();
    assert(tt->numElements() == 2);
    assert(tt->elementType(0).type()->isIntType());
    assert(tt->elementType(1).type()->isStringType());
    assert(tt->elementType(0).kind() == QualifiedType::VAR);
    assert(tt->elementType(1).kind() == QualifiedType::VAR);
  }
  {
    auto context = buildStdContext();
    auto program = R"""(
      proc foo(const in v: ?t) do return v;
      var x = foo((1, "hello"));
    )""";
    auto sig = test20Helper(context, program);
    assert(sig->numFormals() == 1);
    assert(sig->formalType(0).type()->isTupleType());
    assert(sig->formalType(0).kind() == QualifiedType::CONST_IN);
    auto tt = sig->formalType(0).type()->toTupleType();
    assert(tt->numElements() == 2);
    assert(tt->elementType(0).type()->isIntType());
    assert(tt->elementType(1).type()->isStringType());
    assert(tt->elementType(0).kind() == QualifiedType::CONST_VAR);
    assert(tt->elementType(1).kind() == QualifiedType::CONST_VAR);
  }
  {
    auto context = buildStdContext();
    auto program = R"""(
      proc foo(ref v: ?t) do return v;
      var t = (1,"hello");
      var x = foo(t);
    )""";
    auto sig = test20Helper(context, program);
    assert(sig->numFormals() == 1);
    assert(sig->formalType(0).type()->isTupleType());
    assert(sig->formalType(0).kind() == QualifiedType::REF);
    auto tt = sig->formalType(0).type()->toTupleType();
    assert(tt->numElements() == 2);
    assert(tt->elementType(0).type()->isIntType());
    assert(tt->elementType(1).type()->isStringType());
    assert(tt->elementType(0).kind() == QualifiedType::VAR);
    assert(tt->elementType(1).kind() == QualifiedType::VAR);
  }
  {
    auto context = buildStdContext();
    auto program = R"""(
      proc foo(const ref v: ?t) do return v;
      var x = foo((1,"hello"));
    )""";
    auto sig = test20Helper(context, program);
    assert(sig->numFormals() == 1);
    assert(sig->formalType(0).type()->isTupleType());
    assert(sig->formalType(0).kind() == QualifiedType::CONST_REF);
    auto tt = sig->formalType(0).type()->toTupleType();
    assert(tt->numElements() == 2);
    assert(tt->elementType(0).type()->isIntType());
    assert(tt->elementType(1).type()->isStringType());
    assert(tt->elementType(0).kind() == QualifiedType::CONST_VAR);
    assert(tt->elementType(1).kind() == QualifiedType::CONST_VAR);
  }
}

static void test21() {
  printf("%s\n", __FUNCTION__);

  // Ensure that (type int, type int) tuples in type expression are properly
  // handled when they are specifying the type of var or const variable.

  auto context = buildStdContext();
  auto program = R"""(
    var x: 2*int = (7,3);
    const y: 2*int = (7,3);
    var x2: 2*int;
    const y2: 2*int;

    param firstMatch = x.type == x2.type;
    param secondMatch = y.type == y2.type;
  )""";

  auto vars = resolveTypesOfVariables(context, program,
                                      {"x", "y", "x2", "y2", "firstMatch", "secondMatch"});
  assert(vars["x"].type()->isTupleType());
  assert(vars["y"].type()->isTupleType());
  assert(vars["x2"].type()->isTupleType());
  assert(vars["y2"].type()->isTupleType());
  ensureParamBool(vars["firstMatch"], true);
  ensureParamBool(vars["secondMatch"], true);
}

static void test22() {
  printf("%s\n", __FUNCTION__);

  // Ensure that homogeneous tuple addition works

  auto context = buildStdContext();
  auto program = R"""(
    var t1 = (1,2,3);
    var t2 = t1 + 1;
    param match = t1.type == t2.type;
  )""";

  auto vars = resolveTypesOfVariables(context, program,
                                      {"t1", "t2", "match"});
  assert(vars["t1"].type()->isTupleType());
  assert(vars["t2"].type()->isTupleType());
  ensureParamBool(vars["match"], true);
}

static void test23() {
  printf("%s\n", __FUNCTION__);

  // Ensure that 'tuple.size' resolves.
  auto context = buildStdContext();
  auto program = R"""(
    var y: (int, int);
    param x = y.size;
  )""";

  auto qt = resolveQualifiedTypeOfX(context, program);
  ensureParamInt(qt, 2);
}

static void test24() {
  printf("%s\n", __FUNCTION__);
  Context ctx;
  auto context = &ctx;
  ErrorGuard guard(context);

  std::string program =
    R""""(
    operator=(ref lhs: real, rhs: real) {}
    operator=(ref lhs: int, rhs: int) {}
    proc foo() { return (1.0, 2); }
    proc test() {
      var x: real;
      (x, _) = foo();
    }
    test();
    )"""";

  auto mod = parseModule(context, program);
  auto& rr = resolveModule(context, mod->id());
  assert(!guard.realizeErrors());

  // Get the results for the 'test()' call
  //
  // TODO: workaround for current issue where we do not run call-init-deinit
  // for module statements.
  auto testCall = mod->stmt(4)->toFnCall();
  auto testSig = rr.byAst(testCall).mostSpecific().only().fn();
  ResolutionContext rc(context);
  auto fn = resolveFunction(&rc, testSig, nullptr);
  auto fnRR = fn->resolutionById();

  // Get the type of the '(x, _)' tuple itself.
  auto testFn = mod->stmt(3)->toFunction();
  auto astTup = testFn->stmt(1)->toCall()->actual(0)->toTuple();
  assert(astTup);
  auto& qtTup = fnRR.byAst(astTup).type();

  // The tuple itself is 'VAR' because its components will be assigned to.
  assert(qtTup.kind() == QualifiedType::VAR);
  assert(qtTup.type()->isTupleType());
  auto tpTup = qtTup.type()->toTupleType();

  // Its components are 'ref real(64)' and 'var nothing'.
  for (int i = 0; i < tpTup->numElements(); i++) {
    auto qt = tpTup->elementType(i);
    assert(i != 0 || (qt.type()->isRealType() && qt.kind() == QualifiedType::REF));
    assert(i != 1 || (qt.type()->isNothingType() && qt.kind() == QualifiedType::VAR));
  }

  // Finally confirm that there is an assignment for 'x' but not for '_'.
  for (int i = 0; i < astTup->numActuals(); i++) {
    auto actual = astTup->actual(i);
    auto& actions = fnRR.byAst(actual).associatedActions();
    assert(i != 0 || (actions.size() == 1 &&
                      actions[0].action() == AssociatedAction::ASSIGN));
    assert(i != 1 || actions.size() == 0);
  }
}

static void test25() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
    proc foo() { return (1.0, 2); }
    var (v, _) = foo();
    var x = v;
    )"""";

  auto qt = resolveQualifiedTypeOfX(context, program);
  assert(!guard.realizeErrors());
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type()->isRealType());
}

static void test26() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
    record r {}
    iter foo(): (int, r) { yield (0, new r()); }
    for (x, y) in foo() do;
    )"""";

  auto m = resolveTypesOfVariables(context, program, { "x", "y" });
  assert(!guard.realizeErrors());
  assert(m["x"].kind() == QualifiedType::CONST_VAR);
  assert(m["x"].type()->isIntType());
  assert(m["y"].kind() == QualifiedType::CONST_VAR);
  assert(m["y"].type()->isRecordType());

  auto mod = parseModule(context, program);
  auto& rr = resolveModule(context, mod->id());
  auto astTup = mod->stmt(2)->toIndexableLoop()->index();
  assert(astTup->isTupleDecl());
  auto& qtTup = rr.byAst(astTup).type();
  assert(qtTup.kind() == QualifiedType::CONST_VAR);
  assert(qtTup.type()->isTupleType());
}

static void test27() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
    record r {}
    iter foo(): (int, r) { yield (0, new r()); }
    for (x, _) in foo() do;
    )"""";

  auto qt = resolveTypeOfVariable(context, program, "x");
  assert(!guard.realizeErrors());
  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type()->isIntType());
}

// This is private issue #6382.
static void test28() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
    iter myIter() {
      yield (1, 2);
      yield (3, 4);
    }

    for (i, j) in myIter() {
      var z = i;
    }
    operator=(ref lhs: int, rhs: int) {}
    )"""";

  auto m = resolveTypesOfVariables(context, program, { "i", "j", "z" });
  assert(!guard.realizeErrors());
  auto& i = m["i"];
  assert(i.kind() == QualifiedType::CONST_VAR && i.type()->isIntType());
  auto& j = m["j"];
  assert(j.kind() == QualifiedType::CONST_VAR && j.type()->isIntType());
  auto& z = m["z"];
  assert(z.kind() == QualifiedType::VAR && z.type()->isIntType());
}

static void test29() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
    var g: (int, int);
    iter myIter() ref: (int, int) { yield g; }

    for (i, j) in myIter() {
      var z = i;
    }
    operator=(ref lhs: int, rhs: int) {}
    )"""";

  // The entire tuple (i, j) is itself a 'REF' tuple.
  auto mod = parseModule(context, program);
  auto& rr = resolveModule(context, mod->id());
  auto astTup = mod->stmt(2)->toIndexableLoop()->index();
  assert(astTup->isTupleDecl());
  auto& qtTup = rr.byAst(astTup).type();
  assert(qtTup.kind() == QualifiedType::REF);
  assert(qtTup.type()->isTupleType());
  auto tpTup = qtTup.type()->toTupleType();
  for (int i = 0; i < tpTup->numElements(); i++) {
    auto qt = tpTup->elementType(i);
    // But its elements are 'VAR'.
    assert(qt.kind() == QualifiedType::VAR);
    assert(qt.type()->isIntType());
  }

  // The de-tupled components still maintain their 'REF'ness.
  auto m = resolveTypesOfVariables(context, program, { "i", "j", "z" });
  assert(!guard.realizeErrors());
  auto& i = m["i"];
  assert(i.kind() == QualifiedType::REF && i.type()->isIntType());
  auto& j = m["j"];
  assert(j.kind() == QualifiedType::REF && j.type()->isIntType());
  auto& z = m["z"];
  assert(z.kind() == QualifiedType::VAR && z.type()->isIntType());
}

static void test30() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
    var g: (int, int);
    iter myIter() const ref: (int, int) { yield g; }

    for (i, j) in myIter() {
      var z = i;
    }
    operator=(ref lhs: int, rhs: int) {}
    )"""";

  // The entire tuple (i, j) is itself a 'REF' tuple.
  auto mod = parseModule(context, program);
  auto& rr = resolveModule(context, mod->id());
  auto astTup = mod->stmt(2)->toIndexableLoop()->index();
  assert(astTup->isTupleDecl());
  auto& qtTup = rr.byAst(astTup).type();
  assert(qtTup.kind() == QualifiedType::CONST_REF);
  assert(qtTup.type()->isTupleType());

  // The de-tupled components still maintain their 'REF'ness.
  auto m = resolveTypesOfVariables(context, program, { "i", "j", "z" });
  assert(!guard.realizeErrors());
  auto& i = m["i"];
  assert(i.kind() == QualifiedType::CONST_REF && i.type()->isIntType());
  auto& j = m["j"];
  assert(j.kind() == QualifiedType::CONST_REF && j.type()->isIntType());
  auto& z = m["z"];
  assert(z.kind() == QualifiedType::VAR && z.type()->isIntType());
}

static void test31() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
    var g: (int, int);
    ref (a, b) = g;
    )"""";

  // The entire tuple (i, j) is itself a 'REF' tuple.
  auto mod = parseModule(context, program);
  auto& rr = resolveModule(context, mod->id());
  auto astTup = mod->stmt(1);
  assert(astTup->isTupleDecl());
  auto& qtTup = rr.byAst(astTup).type();
  assert(qtTup.kind() == QualifiedType::REF);
  assert(qtTup.type()->isTupleType());

  // The de-tupled components still maintain their 'REF'ness.
  auto m = resolveTypesOfVariables(context, program, { "a", "b" });
  assert(!guard.realizeErrors());
  assert(m["a"].kind() == QualifiedType::REF);
  assert(m["a"].type()->isIntType());
  assert(m["b"].kind() == QualifiedType::REF);
  assert(m["b"].type()->isIntType());
}

static void test32() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
    var g: (int, int);
    const ref (a, b) = g;
    )"""";

  auto mod = parseModule(context, program);
  auto& rr = resolveModule(context, mod->id());
  auto astTup = mod->stmt(1);
  assert(astTup->isTupleDecl());
  auto& qtTup = rr.byAst(astTup).type();
  assert(qtTup.kind() == QualifiedType::CONST_REF);
  assert(qtTup.type()->isTupleType());

  auto m = resolveTypesOfVariables(context, program, { "a", "b" });
  assert(!guard.realizeErrors());
  assert(m["a"].kind() == QualifiedType::CONST_REF);
  assert(m["a"].type()->isIntType());
  assert(m["b"].kind() == QualifiedType::CONST_REF);
  assert(m["b"].type()->isIntType());
}

static void test33() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
    proc foo() { return (1.0, 2); }
    proc test() {
      var x : real;
      var y : int;
      (x, y, _) = foo();
    }
    test();
    )"""";

  auto mod = parseModule(context, program);
  resolveModule(context, mod->id());

  assert(guard.numErrors() == 1);
  auto& err = guard.error(0);
  assert(err->message() == "tuple size mismatch in split tuple assign");
  guard.realizeErrors();
}

static void test34() {
  printf("%s\n", __FUNCTION__);
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::string program =
    R""""(
    record R {}
    proc foo(x) {}

    var data: ((R,),);
    foo(data);
    )"""";

  // Should resolve without error
  auto mod = parseModule(context, program);
  resolveModule(context, mod->id());
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
  test9b();
  test10();
  test11();
  test11b();
  test12();
  test13();
  test14();
  test15();
  test16();
  test17();
  test18();
  test18b();
  test19();
  testTupleGeneric();

  test20();
  test21();
  test22();
  test23();
  test24();
  test25();
  test26();
  test27();
  test28();
  test29();
  test30();
  test31();
  test32();
  test33();
  test34();

  return 0;
}
