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

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/uast/Call.h"
#include "chpl/uast/Comment.h"
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
using namespace uast;

// test showing point-of-instantiation behavior
// This test has a function 'helper' that is only available
// from the point-of-instantiaton of a generic function. This
// test checks that the 'helper' function is found through
// the point-of-instantiation.
static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::build(context, "input.chpl");
  std::string contents = "module M {\n"
                         "  proc generic(param p) {\n"
                         "    helper();\n"
                         "  }\n"
                         "}\n"
                         "module N {\n"
                         "  use M;\n"
                         "  proc helper() { }\n"
                         "  generic(1);\n"
                         "}\n";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 2);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 1);
  const Module* n = vec[1]->toModule();
  assert(n);
  assert(n->numStmts() == 3);
  const Function* mGeneric = m->stmt(0)->toFunction();
  assert(mGeneric);
  const Call* helperCall = mGeneric->stmt(0)->toCall();
  assert(helperCall);
  const Function* nHelper = n->stmt(1)->toFunction();
  assert(nHelper);
  const Call* genericCall = n->stmt(2)->toCall();
  assert(genericCall);

  // resolve module N
  const ResolutionResultByPostorderID& rr = resolveModule(context, n->id());

  // get the resolved function
  const ResolvedFunction* rfunc =
    resolveOnlyCandidate(context, rr.byAst(genericCall));
  assert(rfunc);
  assert(rfunc->id() == mGeneric->id());
  assert(rfunc->signature()->instantiatedFrom() != nullptr);

  const ResolvedFunction* rhelp =
    resolveOnlyCandidate(context, rfunc->resolutionById().byAst(helperCall));
  assert(rhelp);
  assert(rhelp->id() == nHelper->id());
}

// variant of the above, testing that a call to 'helper' is not
// resolved if it is present only in a scope that is neither a
// parent of the generic function nor available from the point
// of instantiation.
static void test1n() {
  printf("test1n\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::build(context, "input.chpl");
  std::string contents = "module M {\n"
                         "  proc generic(param p) {\n"
                         "    helper();\n"
                         "  }\n"
                         "}\n"
                         "module N {\n"
                         "  use M;\n"
                         "  import H;\n"
                         "  generic(1);\n"
                         "}\n"
                         "module H {\n"
                         "  proc helper() { }\n"
                         "}\n";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 3);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 1);
  const Module* n = vec[1]->toModule();
  assert(n);
  assert(n->numStmts() == 3);
  const Module* h = vec[2]->toModule();
  assert(h);
  assert(h->numStmts() == 1);

  const Function* mGeneric = m->stmt(0)->toFunction();
  assert(mGeneric);
  const Call* helperCall = mGeneric->stmt(0)->toCall();
  assert(helperCall);
  const Function* hHelper = h->stmt(0)->toFunction();
  assert(hHelper);
  const Call* genericCall = n->stmt(2)->toCall();
  assert(genericCall);

  // resolve module N
  const ResolutionResultByPostorderID& rr = resolveModule(context, n->id());

  // get the resolved function
  const ResolvedFunction* rfunc =
    resolveOnlyCandidate(context, rr.byAst(genericCall));
  assert(rfunc);
  assert(rfunc->id() == mGeneric->id());
  assert(rfunc->signature()->instantiatedFrom() != nullptr);

  const ResolvedExpression& rhelp = rfunc->resolutionById().byAst(helperCall);
  for (auto candidate : rhelp.mostSpecific()) {
    assert(candidate == nullptr);
  }
}

// testing a simplified version of a pattern that comes up in test2
static void test2a() {
  printf("test2a\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::build(context, "input.chpl");
  std::string contents = R""""(
    module M {
      proc foo(param arg:int, val:int) { }
      proc foo(param arg:int, val:real) { }
      proc runM1() {
        var x: int;
        foo(1, x);
      }
    }
  )"""";


  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);
  const Module* M = vec[0]->toModule();
  assert(M);
  assert(M->numStmts() == 3);

  const Function* fooA = M->stmt(0)->toFunction();
  assert(fooA);
  const Function* fooB = M->stmt(1)->toFunction();
  assert(fooB);
  const Function* runM1 = M->stmt(2)->toFunction();
  assert(runM1);
  const Call* runM1FooCall = runM1->stmt(1)->toCall();
  assert(runM1FooCall);

  // resolve runM1
  const ResolvedFunction* rRunM1 =
    resolveConcreteFunction(context, runM1->id());
  assert(rRunM1);

  // find the resolved call to foo in runM1
  const ResolvedFunction* m1foo =
    resolveOnlyCandidate(context, rRunM1->byAst(runM1FooCall));
  assert(m1foo);
  assert(m1foo->id() == fooA->id());
}

// testing the challenging program from issue 18081
static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::build(context, "input.chpl");
  std::string contents = R""""(
    module G {
      proc foo(param arg:int, val:int) {
        bar(arg);
      }
      proc foo(param arg:int, val:real) {
        bar(arg);
      }
    }
    module M1 {
      use G;
      proc bar(arg: int) { }
      proc runM1() {
        var x: int;
        foo(1, x);
      }
    }

    module M2 {
      use G;
      proc bar(arg: int) { }
      proc runM2() {
        var y: real;
        foo(1, y);
      }
    }

    module User {
      use M1, M2;
      proc main() {
        runM1();
        runM2();
      }
    }
  )"""";


  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 4);
  const Module* g = vec[0]->toModule();
  assert(g);
  assert(g->numStmts() == 2);
  const Module* m1 = vec[1]->toModule();
  assert(m1);
  assert(m1->numStmts() == 3);
  const Module* m2 = vec[2]->toModule();
  assert(m2);
  assert(m2->numStmts() == 3);
  const Module* user = vec[3]->toModule();
  assert(user);
  assert(user->numStmts() == 2);

  const Function* fooA = g->stmt(0)->toFunction();
  assert(fooA);
  const Call* fooABarCall = fooA->stmt(0)->toCall();
  assert(fooABarCall);
  const Function* fooB = g->stmt(1)->toFunction();
  assert(fooB);
  const Call* fooBBarCall = fooB->stmt(0)->toCall();
  assert(fooBBarCall);

  const Function* m1Bar = m1->stmt(1)->toFunction();
  assert(m1Bar);
  const Function* runM1 = m1->stmt(2)->toFunction();
  assert(runM1);
  const Call* runM1FooCall = runM1->stmt(1)->toCall();
  assert(runM1FooCall);
  const Function* m2Bar = m2->stmt(1)->toFunction();
  assert(m2Bar);
  const Function* runM2 = m2->stmt(2)->toFunction();
  assert(runM2);
  const Call* runM2FooCall = runM2->stmt(1)->toCall();
  assert(runM2FooCall);

  // resolve runM1
  const ResolvedFunction* rRunM1 =
    resolveConcreteFunction(context, runM1->id());
  assert(rRunM1);
  // resolve runM2
  const ResolvedFunction* rRunM2 =
    resolveConcreteFunction(context, runM2->id());
  assert(rRunM2);

  // find the resolved calls to foo in runM1 and runM2
  const ResolvedFunction* m1foo =
    resolveOnlyCandidate(context, rRunM1->byAst(runM1FooCall));
  assert(m1foo);
  assert(m1foo->id() == fooA->id());
  const ResolvedFunction* m2foo =
    resolveOnlyCandidate(context, rRunM2->byAst(runM2FooCall));
  assert(m2foo);
  assert(m2foo->id() == fooB->id());

  const ResolvedFunction* m1foobar =
    resolveOnlyCandidate(context, m1foo->byAst(fooABarCall));
  assert(m1foobar);
  assert(m1foobar->id() == m1Bar->id());
  const ResolvedFunction* m2foobar =
    resolveOnlyCandidate(context, m2foo->byAst(fooBBarCall));
  assert(m2foobar);
  assert(m2foobar->id() == m2Bar->id());
}

// testing the challenging program from issue 18119
static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::build(context, "input.chpl");
  std::string contents = R""""(
    module G {
      proc genericfunc(param p) {
        helper1();
        helper2();
        helper3();
      }
    }

    module M {
      use G;
      proc mCallFunc() {
        genericfunc(1);
        proc helper2() { }
        // M.helper1, M.call.helper2, M.helper3
      }
      proc mGenericCallFunc(param p) {
        genericfunc(p); // this call has POI here, parent POI N
                        // uses M.helper1, N.helper2, M.helper3
                        // -- so used scopes is {M,N}
      }
      proc helper1() { }
      proc helper3() { }
    }

    module N {
      use G,M;
      proc nCallFunc() {
        genericfunc(1); // this call has POI here, no parent POI
                        // uses N.helper1, N.helper2, M.helper3
                        // -- so used scopes is {M,N}
      }
      proc helper1() { }
      proc helper2() { }

      proc main() {
        nCallFunc();
        mCallFunc();
        mGenericCallFunc(1); // no bug if this call is first
      }
    }
  )"""";


  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 3);
  const Module* G = vec[0]->toModule();
  assert(G);
  assert(G->numStmts() == 1);
  const Module* M = vec[1]->toModule();
  assert(M);
  assert(M->numStmts() == 5);
  const Module* N = vec[2]->toModule();
  assert(N);
  assert(N->numStmts() == 5);

  auto genericfunc = G->stmt(0)->toFunction();
  assert(genericfunc);
  auto helperCall1 = genericfunc->stmt(0)->toCall();
  assert(helperCall1);
  auto helperCall2 = genericfunc->stmt(1)->toCall();
  assert(helperCall2);
  auto helperCall3 = genericfunc->stmt(2)->toCall();
  assert(helperCall3);

  auto mCallFunc = M->stmt(1)->toFunction();
  assert(mCallFunc);
  assert(mCallFunc->numStmts() == 3);
  auto mCallFuncGCall = mCallFunc->stmt(0)->toCall();
  assert(mCallFuncGCall);
  auto mHelper2 = mCallFunc->stmt(1)->toFunction();
  assert(mHelper2);
  auto mGenericCallFunc = M->stmt(2)->toFunction();
  assert(mGenericCallFunc);
  auto mGenericCallFuncGCall = mGenericCallFunc->stmt(0)->toCall();
  assert(mCallFuncGCall);
  auto mHelper1 = M->stmt(3)->toFunction();
  assert(mHelper1);
  auto mHelper3 = M->stmt(4)->toFunction();
  assert(mHelper3);

  auto nCallFunc = N->stmt(1)->toFunction();
  assert(nCallFunc);
  auto nCallFuncGCall = nCallFunc->stmt(0)->toCall();
  assert(nCallFuncGCall);
  auto nHelper1 = N->stmt(2)->toFunction();
  assert(nHelper1);
  auto nHelper2 = N->stmt(3)->toFunction();
  assert(nHelper2);
  auto main = N->stmt(4)->toFunction();
  assert(main);
  assert(main->numStmts() >= 3);

  auto nCall = main->stmt(0)->toCall();
  assert(nCall);
  auto mCall = main->stmt(1)->toCall();
  assert(mCall);
  auto mGenericCall = main->stmt(2)->toCall();
  assert(mGenericCall);

  // resolve main
  const ResolvedFunction* rMain = resolveConcreteFunction(context, main->id());
  assert(rMain);

  // find the resolved calls in main to nCallFunc, mCallFunc, mGenericCallFunc
  const ResolvedFunction* rNCallFunc =
    resolveOnlyCandidate(context, rMain->byAst(nCall));
  assert(rNCallFunc);

  const ResolvedFunction* rMCallFunc =
    resolveOnlyCandidate(context, rMain->byAst(mCall));
  assert(rMCallFunc);

  const ResolvedFunction* rMGenericCallFunc =
    resolveOnlyCandidate(context, rMain->byAst(mGenericCall));
  assert(rMGenericCallFunc);

  // within each of those, find the call to genericfunc
  const ResolvedFunction* rNCallGF =
    resolveOnlyCandidate(context, rNCallFunc->byAst(nCallFuncGCall));
  assert(rNCallGF);
  const ResolvedFunction* rMCallGF =
    resolveOnlyCandidate(context, rMCallFunc->byAst(mCallFuncGCall));
  assert(rMCallGF);
  const ResolvedFunction* rMGenericCallGF =
    resolveOnlyCandidate(context, rMGenericCallFunc->byAst(mGenericCallFuncGCall));
  assert(rMGenericCallGF);

  // now within each one of those, check the
  // helper1 / helper2 / helper3 calls go to the right place.

  // first, check in nCall
  {
    const ResolvedFunction* h1 =
      resolveOnlyCandidate(context, rNCallGF->byAst(helperCall1));
    const ResolvedFunction* h2 =
      resolveOnlyCandidate(context, rNCallGF->byAst(helperCall2));
    const ResolvedFunction* h3 =
      resolveOnlyCandidate(context, rNCallGF->byAst(helperCall3));

    assert(h1);
    assert(h1->id() == nHelper1->id());

    assert(h2);
    assert(h2->id() == nHelper2->id());

    assert(h3);
    assert(h3->id() == mHelper3->id());
  }

  // next, check in mCall
  {
    const ResolvedFunction* h1 =
      resolveOnlyCandidate(context, rMCallGF->byAst(helperCall1));
    const ResolvedFunction* h2 =
      resolveOnlyCandidate(context, rMCallGF->byAst(helperCall2));
    const ResolvedFunction* h3 =
      resolveOnlyCandidate(context, rMCallGF->byAst(helperCall3));

    assert(h1);
    assert(h1->id() == mHelper1->id());

    assert(h2);
    assert(h2->id() == mHelper2->id());

    assert(h3);
    assert(h3->id() == mHelper3->id());
  }

  // then, check in mGenericCall
  {
    const ResolvedFunction* h1 =
      resolveOnlyCandidate(context, rMGenericCallGF->byAst(helperCall1));
    const ResolvedFunction* h2 =
      resolveOnlyCandidate(context, rMGenericCallGF->byAst(helperCall2));
    const ResolvedFunction* h3 =
      resolveOnlyCandidate(context, rMGenericCallGF->byAst(helperCall3));

    assert(h1);
    assert(h1->id() == mHelper1->id());

    assert(h2);
    assert(h2->id() == nHelper2->id());

    assert(h3);
    assert(h3->id() == mHelper3->id());
  }

  // check that the generic function signatures are the same
  assert(rNCallGF->signature() == rMCallGF->signature());
  assert(rMCallGF->signature() == rMGenericCallGF->signature());
}

// check generic reuse from the same scope
static void test4() {
  printf("test4\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::build(context, "input.chpl");
  std::string contents = R""""(
    module M {
      proc genericfunc(param p) {
        helper1();
      }
      proc helper1() { }
      genericfunc(1);
      genericfunc(1);
    }
   )"""";

  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);
  auto M = vec[0]->toModule();
  assert(M);
  assert(M->numStmts() == 4);

  auto aCall = M->stmt(2)->toCall();
  assert(aCall);
  auto bCall = M->stmt(3)->toCall();
  assert(bCall);

  const auto& rM = resolveModule(context, M->id());

  auto rAGeneric = resolveOnlyCandidate(context, rM.byAst(aCall));
  assert(rAGeneric);
  auto rBGeneric = resolveOnlyCandidate(context, rM.byAst(bCall));
  assert(rBGeneric);

  // check that these two are the same instantiation.
  assert(rAGeneric == rBGeneric);
}

// check generic reuse from different scopes but with the same POI calls
static void test5() {
  printf("test5\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::build(context, "input.chpl");
  std::string contents = R""""(
    module N {
      proc genericfunc(param p) {
        helper1();
      }
      proc helper1() { }
    }
    module A {
      use N;
      proc otherfunc() { }
      genericfunc(1); // irrelevant poi 1
    }
    module B {
      use N;
      proc somefunc() { }
      genericfunc(1); // irrelevant poi 2
    }
    module Main {
      use A, B;
      proc main() {
      }
    }
   )"""";

  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 4);
  auto N = vec[0]->toModule();
  auto A = vec[1]->toModule();
  auto B = vec[2]->toModule();
  auto Main = vec[3]->toModule();
  assert(N);
  assert(N->numStmts() == 2);
  assert(A);
  assert(A->numStmts() >= 3);
  assert(B);
  assert(B->numStmts() >= 3);
  assert(Main);
  assert(Main->numStmts() == 2);

  auto aCall = A->stmt(2)->toCall();
  assert(aCall);
  auto bCall = B->stmt(2)->toCall();
  assert(bCall);

  resolveModule(context, Main->id());
  const auto& rA = resolveModule(context, A->id());
  const auto& rB = resolveModule(context, B->id());

  auto rAGeneric = resolveOnlyCandidate(context, rA.byAst(aCall));
  assert(rAGeneric);
  auto rBGeneric = resolveOnlyCandidate(context, rB.byAst(bCall));
  assert(rBGeneric);

  // check that these two are the same instantiation.
  assert(rAGeneric == rBGeneric);
}

// check generic reuse from different scopes but with the same POI calls
static void test6() {
  printf("test6\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::build(context, "input.chpl");
  std::string contents = R""""(
    module M {
      proc genericfunc(param p) {
        helper1();
      }
    }
    module N {
      use M;
      proc callgenericfunc(param p) {
        genericfunc(p);
      }
      proc helper1() { }
    }
    module A {
      use N;
      proc otherfunc() { }
      callgenericfunc(1); // irrelevant poi 1
    }
    module B {
      use N;
      proc somefunc() { }
      callgenericfunc(1); // irrelevant poi 2
    }
    module Main {
      use A, B;
      proc main() {
      }
    }
   )"""";

  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 5);
  auto M = vec[0]->toModule();
  auto N = vec[1]->toModule();
  auto A = vec[2]->toModule();
  auto B = vec[3]->toModule();
  auto Main = vec[4]->toModule();
  assert(M);
  assert(M->numStmts() == 1);
  assert(N);
  assert(N->numStmts() == 3);
  assert(A);
  assert(A->numStmts() >= 3);
  assert(B);
  assert(B->numStmts() >= 3);
  assert(Main);
  assert(Main->numStmts() == 2);

  auto aCall = A->stmt(2)->toCall();
  assert(aCall);
  auto bCall = B->stmt(2)->toCall();
  assert(bCall);

  resolveModule(context, Main->id());
  const auto& rA = resolveModule(context, A->id());
  const auto& rB = resolveModule(context, B->id());

  auto rACallGeneric = resolveOnlyCandidate(context, rA.byAst(aCall));
  assert(rACallGeneric);
  assert(rACallGeneric->signature());
  assert(rACallGeneric->signature()->untyped());
  auto rBCallGeneric = resolveOnlyCandidate(context, rB.byAst(bCall));
  assert(rBCallGeneric);
  assert(rBCallGeneric->signature()->untyped());

  // check that these two have the same signature
  assert(rACallGeneric->signature() == rBCallGeneric->signature());

  // check that these two are the same instantiation.
  assert(rACallGeneric == rBCallGeneric);
}


int main() {
  test1();
  test1n();
  test2a();
  test2();
  test3();
  test4();
  test5();
  test6();

  return 0;
}
