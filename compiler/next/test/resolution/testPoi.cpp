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

// very simple test showing point-of-instantiation behavior
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

  const TypedFnSignature* sig = rr.byAst(genericCall).mostSpecific.only();
  const PoiScope* poiScope = rr.byAst(genericCall).poiScope;
  assert(sig);
  assert(sig->untypedSignature->functionId == mGeneric->id());
  assert(poiScope);

  // get the resolved function
  const ResolvedFunction* rfunc = resolvedFunction(context, sig, poiScope);
  assert(rfunc);
  assert(rfunc->signature == sig);

  const ResolvedExpression& re = rfunc->resolutionById.byAst(helperCall);
  const TypedFnSignature* helpSig = re.mostSpecific.only();
  assert(helpSig);
  assert(helpSig->untypedSignature->functionId == nHelper->id());
}

// showing the challenging program from issue 18081
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
    resolvedConcreteFunction(context, runM1->id());
  assert(rRunM1);
  // resolve runM2
  const ResolvedFunction* rRunM2 =
    resolvedConcreteFunction(context, runM2->id());
  assert(rRunM2);

  // find the resolved calls to foo in runM1 and runM2
  const ResolvedExpression& runM1FooCallR = rRunM1->byAst(runM1FooCall);
  const ResolvedExpression& runM2FooCallR = rRunM2->byAst(runM2FooCall);
  assert(runM1FooCallR.mostSpecific.only());
  assert(runM2FooCallR.mostSpecific.only());

  const ResolvedFunction* m1foo =
    resolvedFunction(context, runM1FooCallR.mostSpecific.only(),
                              runM1FooCallR.poiScope);

  const ResolvedFunction* m2foo =
    resolvedFunction(context, runM2FooCallR.mostSpecific.only(),
                              runM2FooCallR.poiScope);


  assert(m1foo->functionId() == fooA->id());
  assert(m2foo->functionId() == fooB->id());

  const ResolvedExpression& fooABarCallR = m1foo->byAst(fooABarCall);
  const ResolvedExpression& fooBBarCallR = m2foo->byAst(fooBBarCall);
  assert(fooABarCallR.mostSpecific.only());
  assert(fooBBarCallR.mostSpecific.only());

  assert(fooABarCallR.mostSpecific.only()->functionId() == m1Bar->id());
  assert(fooBBarCallR.mostSpecific.only()->functionId() == m2Bar->id());
}


int main() {
  test1();
  test2();

  return 0;
}
