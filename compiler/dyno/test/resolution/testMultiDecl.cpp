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
#include "chpl/uast/Module.h"
#include "chpl/uast/MultiDecl.h"
#include "chpl/uast/TupleDecl.h"
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

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  auto M = parseModule(context,
                R""""(
                  var a, b, c: int;
                )"""");

  assert(M->numStmts() == 1);
  const MultiDecl* md = M->stmt(0)->toMultiDecl();
  assert(md);
  auto a = md->declOrComment(0)->toVariable();
  assert(a);
  auto b = md->declOrComment(1)->toVariable();
  assert(b);
  auto c = md->declOrComment(2)->toVariable();
  assert(c);

  const ResolutionResultByPostorderID& rr = resolveModule(context, M->id());
  auto aQt = rr.byAst(a).type();
  auto bQt = rr.byAst(b).type();
  auto cQt = rr.byAst(c).type();
  assert(aQt.kind() == QualifiedType::VAR);
  assert(aQt.type() == IntType::get(context, 0));
  assert(bQt.kind() == QualifiedType::VAR);
  assert(bQt.type() == IntType::get(context, 0));
  assert(cQt.kind() == QualifiedType::VAR);
  assert(cQt.type() == IntType::get(context, 0));
}

static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  auto M = parseModule(context,
                R""""(
                  const a, b, c = 1.0;
                )"""");

  assert(M->numStmts() == 1);
  const MultiDecl* md = M->stmt(0)->toMultiDecl();
  assert(md);
  auto a = md->declOrComment(0)->toVariable();
  assert(a);
  auto b = md->declOrComment(1)->toVariable();
  assert(b);
  auto c = md->declOrComment(2)->toVariable();
  assert(c);

  const ResolutionResultByPostorderID& rr = resolveModule(context, M->id());
  auto aQt = rr.byAst(a).type();
  auto bQt = rr.byAst(b).type();
  auto cQt = rr.byAst(c).type();
  assert(aQt.kind() == QualifiedType::CONST_VAR);
  assert(aQt.type() == RealType::get(context, 0));
  assert(bQt.kind() == QualifiedType::CONST_VAR);
  assert(bQt.type() == RealType::get(context, 0));
  assert(cQt.kind() == QualifiedType::CONST_VAR);
  assert(cQt.type() == RealType::get(context, 0));
}

static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  auto M = parseModule(context,
                R""""(
                  proc g() { return "a string"; }
                  var a, b = 1.0, c, d:int, e, f = g();
                )"""");

  assert(M->numStmts() == 2);
  const MultiDecl* md = M->stmt(1)->toMultiDecl();
  assert(md);
  auto a = md->declOrComment(0)->toVariable();
  assert(a);
  auto b = md->declOrComment(1)->toVariable();
  assert(b);
  auto c = md->declOrComment(2)->toVariable();
  assert(c);
  auto d = md->declOrComment(3)->toVariable();
  assert(d);
  auto e = md->declOrComment(4)->toVariable();
  assert(e);
  auto f = md->declOrComment(5)->toVariable();
  assert(f);

  const ResolutionResultByPostorderID& rr = resolveModule(context, M->id());
  auto aQt = rr.byAst(a).type();
  auto bQt = rr.byAst(b).type();
  auto cQt = rr.byAst(c).type();
  auto dQt = rr.byAst(d).type();
  auto eQt = rr.byAst(e).type();
  auto fQt = rr.byAst(f).type();
  assert(aQt.kind() == QualifiedType::VAR);
  assert(aQt.type() == RealType::get(context, 0));
  assert(bQt.kind() == QualifiedType::VAR);
  assert(bQt.type() == RealType::get(context, 0));
  assert(cQt.kind() == QualifiedType::VAR);
  assert(cQt.type() == IntType::get(context, 0));
  assert(dQt.kind() == QualifiedType::VAR);
  assert(dQt.type() == IntType::get(context, 0));
  assert(eQt.kind() == QualifiedType::VAR);
  assert(eQt.type() == RecordType::getStringType(context));
  assert(fQt.kind() == QualifiedType::VAR);
  assert(fQt.type() == RecordType::getStringType(context));
}

static void test4() {
  // test case mixing MultiDecl and TupleDecl
  printf("test4\n");
  Context ctx;
  Context* context = &ctx;

  auto M = parseModule(context,
                R""""(
                  var a, b: real, (c, d): (int, real), (e, f) = (1.0, 2);
                )"""");

  assert(M->numStmts() == 1);
  const MultiDecl* md = M->stmt(0)->toMultiDecl();
  assert(md);
  auto a = md->declOrComment(0)->toVariable();
  assert(a);
  auto b = md->declOrComment(1)->toVariable();
  assert(b);
  auto cd = md->declOrComment(2)->toTupleDecl();
  assert(cd);
  auto c = cd->decl(0)->toVariable();
  assert(c);
  auto d = cd->decl(1)->toVariable();
  assert(d);
  auto ef = md->declOrComment(3)->toTupleDecl();
  assert(ef);
  auto e = ef->decl(0)->toVariable();
  assert(e);
  auto f = ef->decl(1)->toVariable();
  assert(f);

  const ResolutionResultByPostorderID& rr = resolveModule(context, M->id());
  auto aQt = rr.byAst(a).type();
  auto bQt = rr.byAst(b).type();
  auto cQt = rr.byAst(c).type();
  auto dQt = rr.byAst(d).type();
  auto eQt = rr.byAst(e).type();
  auto fQt = rr.byAst(f).type();
  assert(aQt.kind() == QualifiedType::VAR);
  assert(aQt.type() == RealType::get(context, 0));
  assert(bQt.kind() == QualifiedType::VAR);
  assert(bQt.type() == RealType::get(context, 0));
  assert(cQt.kind() == QualifiedType::VAR);
  assert(cQt.type() == IntType::get(context, 0));
  assert(dQt.kind() == QualifiedType::VAR);
  assert(dQt.type() == RealType::get(context, 0));
  assert(eQt.kind() == QualifiedType::VAR);
  assert(eQt.type() == RealType::get(context, 0));
  assert(fQt.kind() == QualifiedType::VAR);
  assert(fQt.type() == IntType::get(context, 0));
}

static void test5() {
  // this is the test case from issue #19340
  printf("test5\n");
  Context ctx;
  Context* context = &ctx;

  auto M = parseModule(context,
                R""""(
                  var (a, b), (c, d): (int, real);
                )"""");

  assert(M->numStmts() == 1);
  const MultiDecl* md = M->stmt(0)->toMultiDecl();
  assert(md);
  auto ab = md->declOrComment(0)->toTupleDecl();
  assert(ab);
  auto a = ab->decl(0)->toVariable();
  assert(a);
  auto b = ab->decl(1)->toVariable();
  assert(b);
  auto cd = md->declOrComment(1)->toTupleDecl();
  assert(cd);
  auto c = cd->decl(0)->toVariable();
  assert(c);
  auto d = cd->decl(1)->toVariable();
  assert(d);

  const ResolutionResultByPostorderID& rr = resolveModule(context, M->id());
  auto aQt = rr.byAst(a).type();
  auto bQt = rr.byAst(b).type();
  auto cQt = rr.byAst(c).type();
  auto dQt = rr.byAst(d).type();
  assert(aQt.kind() == QualifiedType::VAR);
  assert(aQt.type() == IntType::get(context, 0));
  assert(bQt.kind() == QualifiedType::VAR);
  assert(bQt.type() == RealType::get(context, 0));
  assert(cQt.kind() == QualifiedType::VAR);
  assert(cQt.type() == IntType::get(context, 0));
  assert(dQt.kind() == QualifiedType::VAR);
  assert(dQt.type() == RealType::get(context, 0));
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();

  return 0;
}
