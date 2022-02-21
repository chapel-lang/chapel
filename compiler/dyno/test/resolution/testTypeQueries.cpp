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

// assumes the last statement is a variable declaration for x.
// returns the type of that.
static const Type*
parseTypeOfX(Context* context, const char* program) {
  auto m = parseModule(context, program);
  assert(m->numStmts() > 0);
  const Variable* x = m->stmt(m->numStmts()-1)->toVariable();
  assert(x);
  assert(x->name() == "x");

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  auto qt = rr.byAst(x).type();
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type());

  const Type* t = qt.type();
  return t;
}

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  auto t1 = parseTypeOfX(context,
                R""""(
                  proc f(arg: ?) { return arg; }
                  var x = f(1);
                )"""");

  assert(t1 && t1->isIntType());

  auto t2 = parseTypeOfX(context,
                R""""(
                  proc f(arg: ?) { var ret: arg.type; return ret; }
                  var x = f(1);
                )"""");

  assert(t2 && t2->isIntType());

  auto t3 = parseTypeOfX(context,
                R""""(
                  proc f(arg: ?t) { var ret: arg.type; return ret; }
                  var x = f(1);
                )"""");

  assert(t3 && t3->isIntType());

  auto t4 = parseTypeOfX(context,
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

  auto t = parseTypeOfX(context,
                R""""(
                  record R { type tt; }
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

  auto t = parseTypeOfX(context,
                R""""(
                  record R { type tt; }
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

  auto t = parseTypeOfX(context,
                R""""(
                  record R { type tt; }
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

  auto t = parseTypeOfX(context,
                R""""(
                  record R { type tt; }
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

  auto t = parseTypeOfX(context,
                R""""(
                  record RR { type ttt; }
                  record R { type tt; }
                  proc f(arg: R(RR(?t))) { var ret: t; return ret; }
                  var a: R(RR(int));
                  var x = f(a);
                )"""");

  assert(t && t->isIntType());
}




int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();

  return 0;
}
