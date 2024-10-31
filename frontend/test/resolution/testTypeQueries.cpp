/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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
#include "chpl/uast/Call.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  auto t1 = resolveTypeOfX(context,
                R""""(
                  proc f(arg: ?) { return arg; }
                  var x = f(1);
                )"""");

  assert(t1 && t1->isIntType());

  auto t2 = resolveTypeOfX(context,
                R""""(
                  proc f(arg: ?) { var ret: arg.type; return ret; }
                  var x = f(1);
                )"""");

  assert(t2 && t2->isIntType());

  auto t3 = resolveTypeOfX(context,
                R""""(
                  proc f(arg: ?t) { var ret: arg.type; return ret; }
                  var x = f(1);
                )"""");

  assert(t3 && t3->isIntType());

  auto t4 = resolveTypeOfX(context,
                R""""(
                  proc f(arg: ?t) { var ret: t; return ret; }
                  var x = f(1);
                )"""");

  assert(t4 && t4->isIntType());
}

static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  auto t = resolveTypeOfX(context,
                R""""(
                  record R { type tt; }
                  proc R.init=(other: R) { this.tt = other.tt; }

                  proc f(arg: ?) { return arg; }
                  var a: R(int);
                  var x = f(a);
                )"""");

  assert(t);
  auto c = t->toCompositeType();
  assert(c);
  auto sorted = c->sortedSubstitutions();
  assert(sorted.size() == 1);
  assert(sorted[0].second.type()->isIntType());
}

static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  auto t = resolveTypeOfX(context,
                R""""(
                  record R { type tt; }
                  proc R.init=(other: R) { this.tt = other.tt; }

                  proc f(arg: R(?)) { return arg; }
                  var a: R(int);
                  var x = f(a);
                )"""");

  assert(t);
  auto c = t->toCompositeType();
  assert(c);
  auto sorted = c->sortedSubstitutions();
  assert(sorted.size() == 1);
  assert(sorted[0].second.type()->isIntType());
}

static void test4() {
  printf("test4\n");
  Context ctx;
  Context* context = &ctx;

  auto t = resolveTypeOfX(context,
                R""""(
                  record R { type tt; }
                  proc R.init=(other: R) { this.tt = other.tt; }

                  proc f(arg: R(?t)) { return arg; }
                  var a: R(int);
                  var x = f(a);
                )"""");

  assert(t);
  auto c = t->toCompositeType();
  assert(c);
  auto sorted = c->sortedSubstitutions();
  assert(sorted.size() == 1);
  assert(sorted[0].second.type()->isIntType());
}

static void test5() {
  printf("test5\n");
  Context ctx;
  Context* context = &ctx;

  auto t = resolveTypeOfX(context,
                R""""(
                  record R { type tt; }
                  proc R.init=(other: R) { this.tt = other.tt; }

                  proc f(arg: R(?t)) { var ret: t; return ret; }
                  var a: R(int);
                  var x = f(a);
                )"""");

  assert(t && t->isIntType());
}

static void test6() {
  printf("test6\n");
  Context ctx;
  Context* context = &ctx;

  auto t = resolveTypeOfX(context,
                R""""(
                  record RR { type ttt; }
                  record R { type tt; }
                  proc R.init=(other: R) { this.tt = other.tt; }

                  proc f(arg: R(RR(?t))) { var ret: t; return ret; }
                  var a: R(RR(int));
                  var x = f(a);
                )"""");

  assert(t && t->isIntType());
}

static void test7() {
  printf("test7\n");
  Context ctx;
  Context* context = &ctx;

  auto t = resolveTypeOfX(context,
                R""""(
                  proc f(arg: int(?)) { return arg; }
                  var a: int(8);
                  var x = f(a);
                )"""");

  assert(t && t->isIntType());
  auto it = t->toIntType();
  assert(it->bitwidth() == 8);
}

static void test8() {
  printf("test8\n");
  Context ctx;
  Context* context = &ctx;

  auto t = resolveTypeOfX(context,
                R""""(
                  proc f(arg: int(?w)) { return arg; }
                  var a: int(8);
                  var x = f(a);
                )"""");

  assert(t && t->isIntType());
  auto it = t->toIntType();
  assert(it->bitwidth() == 8);
}

static void test9() {
  printf("test9\n");
  Context ctx;
  Context* context = &ctx;

  auto t = resolveTypeOfX(context,
                R""""(
                  proc f(arg: int(?w)) {
                    var y: uint(w);
                    return y;
                  }
                  var a: int(8);
                  var x = f(a);
                )"""");

  assert(t && t->isUintType());
  auto ut = t->toUintType();
  assert(ut->bitwidth() == 8);
}

static void test10() {
  printf("test10\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
                           module M {
                             proc f(arg:?t = 3) { }
                             f();
                             f("hi");
                           }
                         )"""";

  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 3);
  const Call* callNoArgs = m->stmt(1)->toCall();
  assert(callNoArgs);
  const Call* callString = m->stmt(2)->toCall();
  assert(callString);

  {
    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    const ResolvedExpression& re = rr.byAst(callNoArgs);

    assert(re.type().type()->isVoidType());

    auto c = re.mostSpecific().only();
    assert(c);

    auto fn = c.fn();
    assert(fn->untyped()->name() == "f");

    assert(fn->numFormals() == 1);
    assert(fn->formalName(0) == "arg");
    assert(fn->formalType(0).kind() == QualifiedType::CONST_IN);
    assert(fn->formalType(0).type() == IntType::get(context, 64));
  }

  {
    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    const ResolvedExpression& re = rr.byAst(callString);

    assert(re.type().type()->isVoidType());

    auto c = re.mostSpecific().only();
    assert(c);

    auto fn = c.fn();
    assert(fn->untyped()->name() == "f");

    assert(fn->numFormals() == 1);
    assert(fn->formalName(0) == "arg");
    assert(fn->formalType(0).kind() == QualifiedType::CONST_REF);
    assert(fn->formalType(0).type() == CompositeType::getStringType(context));
  }
}

static void test11() {
  printf("test11\n");
  Context ctx;
  Context* context = &ctx;

  auto t = resolveTypeOfX(context,
                R""""(
                  proc g(a: int(?w), b: int(__primitive("*", 2, w))) {
                    return b;
                  }
                  var a: int(8);
                  var b: int(16);
                  var x = g(a, b);
                )"""");

  assert(t && t->isIntType());
  auto it = t->toIntType();
  assert(it->bitwidth() == 16);
}

static void test12a() {
  printf("test12\n");
  Context ctx;
  Context* context = &ctx;

  auto t = resolveTypeOfX(context,
                R""""(
                  record R { type t1; type t2; }
                  proc R.init=(other: R) {
                    this.t1 = other.t1;
                    this.t2 = other.t2;
                  }

                  proc f(a: R(?t, t)) {
                    return a;
                  }
                  var a: R(int, int);
                  var x = f(a);
                )"""");
  assert(t && t->isCompositeType());
  assert(t);
  auto c = t->toCompositeType();
  assert(c);
  auto sorted = c->sortedSubstitutions();
  assert(sorted.size() == 2);
  assert(sorted[0].second.type()->isIntType());
  assert(sorted[1].second.type()->isIntType());
}

static void test12b() {
  printf("test12\n");
  Context ctx;
  Context* context = &ctx;

  auto t = resolveTypeOfX(context,
                R""""(
                  record R { param p1: int; param p2: int; }
                  proc R.init=(other: R) {
                    this.p1 = other.p1;
                    this.p2 = other.p2;
                  }

                  proc f(a: R(?i, i)) {
                    return a;
                  }
                  var a: R(1, 1);
                  var x = f(a);
                )"""");
  assert(t && t->isCompositeType());
  assert(t);
  auto c = t->toCompositeType();
  assert(c);
  auto sorted = c->sortedSubstitutions();
  assert(sorted.size() == 2);
  assert(sorted[0].second.type()->isIntType());
  assert(sorted[0].second.param());
  assert(sorted[0].second.param()->toIntParam()->value() == 1);
  assert(sorted[1].second.type()->isIntType());
  assert(sorted[1].second.param());
  assert(sorted[1].second.param()->toIntParam()->value() == 1);
}

static void test13a() {
  printf("test13a\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = resolveTypeOfXInit(context,
                R""""(
                  record R { type t1; type t2; }
                  proc R.init=(other: R) {
                    this.t1 = other.t1;
                    this.t2 = other.t2;
                  }

                  proc f(a: R(?t, t)) {
                    return a;
                  }
                  var a: R(int, string);
                  var x = f(a);
                )"""", false);
  assert(qt.isErroneousType());
}

static void test13b() {
  printf("test13b\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = resolveTypeOfXInit(context,
                R""""(
                  record R { param p1: int; param p2: int; }
                  proc R.init=(other: R) {
                    this.p1 = other.p1;
                    this.p2 = other.p2;
                  }

                  proc f(a: R(?t, t)) {
                    return a;
                  }
                  var a: R(1, 2);
                  var x = f(a);
                )"""", false);
  assert(qt.isErroneousType());
}

static void test14() {
  printf("test14\n");
  Context ctx;
  Context* context = &ctx;

  auto t = resolveTypeOfX(context,
                R""""(
                  record R { type t1; type t2; }
                  proc R.init=(other: R) {
                    this.t1 = other.t1;
                    this.t2 = other.t2;
                  }

                  proc f(a: R(?t, if t == bool then string else int)) {
                    return a;
                  }
                  var a: R(bool, string);
                  var x = f(a);
                )"""");
  assert(t && t->isCompositeType());
  assert(t);
  auto c = t->toCompositeType();
  assert(c);
  auto sorted = c->sortedSubstitutions();
  assert(sorted.size() == 2);
  assert(sorted[0].second.type()->isBoolType());
  assert(sorted[1].second.type()->isStringType());
}

static void test15() {
  printf("test15\n");
  Context ctx;
  Context* context = &ctx;

  auto qt = resolveTypeOfXInit(context,
                R""""(
                  record R { type t1; type t2; }
                  proc R.init(type t1, type t2) {
                    this.t1 = t1;
                    this.t2 = t2;
                  }
                  proc R.init=(other: R) {
                    this.t1 = other.t1;
                    this.t2 = other.t2;
                  }

                  proc f(a: R(?t, if t == bool then string else int)) {
                    return a;
                  }
                  var a: R(bool, int);
                  var x = f(a);
                )"""", false);
  assert(qt.isErroneousType());
}

static void test16() {
  printf("test16\n");
  Context ctx;
  Context* context = &ctx;

  std::string setup =
                R""""(
                  record R { type t1; type t2; }
                  proc R.init(type t1, type t2) {
                    this.t1 = t1;
                    this.t2 = t2;
                  }
                  proc f(a: R(?t, if t == bool then string else int)...) type {
                    return t;
                  }
                )"""";

  auto qt = resolveQualifiedTypeOfX(context, setup +
                R""""(
                  var a: R(bool, string);
                  type x = f(a);
                )"""");
  assert(qt.type() && qt.type()->isBoolType());

  context->advanceToNextRevision(false);
  qt = resolveQualifiedTypeOfX(context, setup +
                R""""(
                  var a: R(bool, string);
                  var b: R(bool, string);
                  type x = f(a, b);
                )"""");
  assert(qt.type() && qt.type()->isBoolType());

  context->advanceToNextRevision(false);
  qt = resolveQualifiedTypeOfX(context, setup +
                R""""(
                  var a: R(bool, string);
                  var b: R(bool, string);
                  var c: R(bool, string);
                  type x = f(a, b, c);
                )"""");
  assert(qt.type() && qt.type()->isBoolType());
}

static void test17() {
  printf("test17\n");
  Context ctx;
  Context* context = &ctx;

  std::string setup =
                R""""(
                  record R { type t1; type t2; }
                  proc R.init(type t1, type t2) {
                    this.t1 = t1;
                    this.t2 = t2;
                  }
                  proc f(a: R(?t, if t == bool then string else int)...) type {
                    return t;
                  }
                )"""";

  auto qt = resolveQualifiedTypeOfX(context, setup +
                R""""(
                  var a: R(bool, string);
                  var b: R(bool, int);
                  type x = f(a, b);
                )"""");
  assert(qt.isErroneousType());

  context->advanceToNextRevision(false);
  qt = resolveQualifiedTypeOfX(context, setup+
                R""""(
                  var a: R(bool, int);
                  var b: R(bool, string);
                  type x = f(a, b);
                )"""");
  assert(qt.isErroneousType());

  context->advanceToNextRevision(false);
  qt = resolveQualifiedTypeOfX(context, setup +
                R""""(
                  var a: R(bool, int);
                  type x = f(a);
                )"""");
  assert(qt.isErroneousType());
}

static void test18() {
  printf("test17\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto qt = resolveQualifiedTypeOfX(context, R"""(
    proc foo(param val: bool) type {
      if val then return string;
      else return int;
    }

    record R {
      param val : bool;
      var data : foo(val);
    }

    proc helper(arg: R(?val)) {
      return arg.data;
    }

    var r : R(true);
    var x = helper(r);
    )""");

  assert(qt.type()->isStringType());
}

static void test19() {
  printf("%s\n", __FUNCTION__);
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto qt = resolveQualifiedTypeOfX(context,
                R""""(
                proc helper(param cond : bool) type {
                  if cond then return int;
                  else return string;
                }

                proc func(arg: helper(true)) {
                  return arg;
                }

                var x = func(2);
                )"""");
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type()->isIntType());
  assert(!guard.realizeErrors());
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
  test12a();
  test12b();
  test13a();
  test13b();
  test14();
  test15();
  test16();
  test17();
  test18();
  test19();

  return 0;
}
