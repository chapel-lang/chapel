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
#include "chpl/types/IntType.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
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

// test resolving a very simple module
static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  {
    context->advanceToNextRevision(true);
    auto path = UniqueString::get(context, "input.chpl");
    std::string contents = "var x: int;\n"
                           "x;";
    setFileText(context, path, contents);

    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    assert(m->numStmts() == 2);
    const Variable* x = m->stmt(0)->toVariable();
    assert(x);
    const Identifier* xIdent = m->stmt(1)->toIdentifier();
    assert(xIdent);

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

    assert(rr.byAst(x).type().type()->isIntType());
    assert(rr.byAst(xIdent).type().type()->isIntType());
    assert(rr.byAst(xIdent).toId() == x->id());

    context->collectGarbage();
  }
}

// test resolving a module in an incremental manner
static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  {
    printf("part 1\n");
    context->advanceToNextRevision(true);
    auto path = UniqueString::get(context, "input.chpl");
    std::string contents = "";
    setFileText(context, path, contents);

    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    resolveModule(context, m->id());

    context->collectGarbage();
  }

  {
    printf("part 2\n");
    context->advanceToNextRevision(true);
    auto path = UniqueString::get(context, "input.chpl");
    std::string contents = "var x;";
    setFileText(context, path, contents);

    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    resolveModule(context, m->id());

    context->collectGarbage();
  }

  {
    printf("part 3\n");
    context->advanceToNextRevision(true);
    auto path = UniqueString::get(context, "input.chpl");
    std::string contents = "var x: int;";
    setFileText(context, path, contents);

    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);

    const Variable* x = m->stmt(0)->toVariable();
    assert(x);

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    assert(rr.byAst(x).type().type()->isIntType());

    context->collectGarbage();
  }


  // Run it a few times to make sure there aren't errors related to
  // collectGarbage being run across multiple revisions.
  for (int i = 0; i < 3; i++) {
    printf("part %i\n", 3+i);
    context->advanceToNextRevision(true);
    auto path = UniqueString::get(context, "input.chpl");
    std::string contents = "var x: int;\n"
                           "x;";
    setFileText(context, path, contents);

    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    assert(m->numStmts() == 2);
    const Variable* x = m->stmt(0)->toVariable();
    assert(x);
    const Identifier* xIdent = m->stmt(1)->toIdentifier();
    assert(xIdent);

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

    assert(rr.byAst(x).type().type()->isIntType());
    assert(rr.byAst(xIdent).type().type()->isIntType());
    assert(rr.byAst(xIdent).toId() == x->id());

    context->collectGarbage();
  }
}

static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");

  {
    printf("part 1\n");
    context->advanceToNextRevision(true);
    std::string contents = "proc foo(arg: int) {\n"
                           "  return arg;\n"
                           "}\n"
                           "var y = foo(1);";
    setFileText(context, path, contents);
    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    const Function* procfoo = m->stmt(0)->toFunction();
    assert(procfoo && procfoo->name() == "foo");
    const Variable* y = m->stmt(1)->toVariable();
    assert(y);
    const AstNode* rhs = y->initExpression();
    assert(rhs);
    const FnCall* fnc = rhs->toFnCall();
    assert(fnc);
    const Identifier* foo = fnc->calledExpression()->toIdentifier();
    assert(foo && foo->name() == "foo");

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    auto rrfoo = rr.byAst(foo);
    assert(rrfoo.toId() == procfoo->id());

    auto yt = rr.byAst(y).type().type();
    assert(yt->isIntType());

    context->collectGarbage();
  }

  {
    printf("part 2\n");
    context->advanceToNextRevision(true);
    std::string contents = "var y = foo(1);";
    setFileText(context, path, contents);
    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    const Variable* y = m->stmt(0)->toVariable();
    assert(y);
    const AstNode* rhs = y->initExpression();
    assert(rhs);
    const FnCall* fnc = rhs->toFnCall();
    assert(fnc);
    const Identifier* foo = fnc->calledExpression()->toIdentifier();
    assert(foo && foo->name() == "foo");

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    auto rrfoo = rr.byAst(foo);
    assert(rrfoo.toId().isEmpty());

    auto rry = rr.byAst(y);
    auto yt = rry.type().type();
    assert(yt);
    assert(yt->isErroneousType());

    context->collectGarbage();
  }
}

// this test combines several ideas and is a more challenging
// case for instantiation, conversions, and type construction
static void test4() {
  printf("test4\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
                           module M {
                             class Parent { }
                             class C : Parent { type t; var x: t; }

                             proc f(arg: Parent) { }
                             var x: owned C(int);
                             f(x);
                          }
                        )"""";

  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 5);
  const Call* call = m->stmt(4)->toCall();
  assert(call);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  const ResolvedExpression& re = rr.byAst(call);

  assert(re.type().type()->isVoidType());

  const TypedFnSignature* fn = re.mostSpecific().only();
  assert(fn != nullptr);
  assert(fn->untyped()->name() == "f");
}

// this test checks a simple instantiation situation
static void test5() {
  printf("test5\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
                           module M {
                             var x:int(64);
                             var y = x;
                             proc f(arg) { }
                             f(y);
                           }
                        )"""";

  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 4);
  const Call* call = m->stmt(3)->toCall();
  assert(call);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  const ResolvedExpression& re = rr.byAst(call);

  assert(re.type().type()->isVoidType());

  const TypedFnSignature* fn = re.mostSpecific().only();
  assert(fn != nullptr);
  assert(fn->untyped()->name() == "f");

  assert(fn->numFormals() == 1);
  assert(fn->formalName(0) == "arg");
  assert(fn->formalType(0).kind() == QualifiedType::CONST_IN);
  assert(fn->formalType(0).type() == IntType::get(context, 64));
}

// this test checks a particular incremental pattern
// that crashed earlier versions
static void test6() {
  printf("test6\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");

  {
    printf("part 1\n");
    context->advanceToNextRevision(true);
    std::string contents = R""""(
                              module M {
                                var x = 1;
                                proc f() { return x; }
                              }
                           )"""";

    setFileText(context, path, contents);
    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    resolveModule(context, m->id());

    context->collectGarbage();
  }

  {
    printf("part 2\n");
    context->advanceToNextRevision(true);
    std::string contents = R""""(
                              module M {
                                var x = 1;
                                proc f() { return x; }
                                f();
                              }
                           )"""";

    setFileText(context, path, contents);
    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);

    resolveModule(context, m->id());

    context->collectGarbage();
  }
}

// check a parenless function call
static void test7() {
  printf("test7\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
                           module M {
                             proc parenless { return 1; }
                             parenless;
                           }
                        )"""";

  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 2);
  const Identifier* ident = m->stmt(1)->toIdentifier();
  assert(ident);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  const ResolvedExpression& re = rr.byAst(ident);

  assert(re.type().type());
  assert(re.type().type()->isIntType());

  const TypedFnSignature* fn = re.mostSpecific().only();
  assert(fn != nullptr);
  assert(fn->untyped()->name() == "parenless");
  assert(fn->numFormals() == 0);
}


int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();

  return 0;
}
