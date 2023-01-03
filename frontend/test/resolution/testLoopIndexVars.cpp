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
#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/For.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"
#include "chpl/uast/While.h"
#include "./ErrorGuard.h"

#include <map>

static auto myiter = std::string(R""""(
iter myiter() {
  yield 1;
}

)"""");

static auto recIter = std::string(R""""(
                      record R {
                        iter these() {
                          yield 1;
                        }
                        proc init() { }
                        proc deinit() { }
                      }

                      )"""");

std::vector<const ErrorBase*> errors;

struct ParamCollector {
  using RV = ResolvedVisitor<ParamCollector>;

  std::map<std::string, int> resolvedIdents;
  std::multimap<std::string, int64_t> resolvedVals;

  ParamCollector() { }

  bool enter(const uast::VarLikeDecl* decl, RV& rv) {
    if (decl->storageKind() == IntentList::PARAM ||
        decl->storageKind() == IntentList::INDEX) {
      const ResolvedExpression& rr = rv.byAst(decl);
      auto val = rr.type().param()->toIntParam();
      assert(val != nullptr);

      resolvedVals.emplace(decl->name().str(), val->value());
    }
    return true;
  }

  bool enter(const uast::Identifier* ident, RV& rv) {
    resolvedIdents[ident->name().str()] += 1;
    return true;
  }

  bool enter(const uast::AstNode* ast, RV& rv) {
    return true;
  }
  void exit(const uast::AstNode* ast, RV& rv) {
  }
};

static void testSimpleLoop(std::string loopName) {
  printf("testing simple loop '%s'\n", loopName.c_str());
  Context ctx;
  Context* context = &ctx;

  auto loopText = myiter + loopName +
R"""( i in myiter() {
  var x = i;
}
)""";

  printf("===== Resolving program =====\n");
  printf("%s\n", loopText.c_str());
  printf("=============================\n\n");

  const Module* m = parseModule(context, loopText);
  auto loop = m->stmt(1)->toIndexableLoop();
  auto xVar = loop->body()->stmt(0);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto idxType = rr.byAst(loop->index()).type();
  assert(idxType.type() == IntType::get(context, 0));

  auto xType = rr.byAst(xVar).type();
  assert(idxType == xType);
}

//
// Testing resolution of loop index variables
// TODO:
// - zippered iteration
// - forall loops
// - error messages
//

static void testAmbiguous() {
  printf("testAmbiguous\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  //
  // Make sure nothing explodes when encountering an ambiguity
  //

  auto iterText = myiter +
                  R""""(
                  iter myiter() {
                    yield "str";
                  }
                  for i in myiter() {
                  }
                  )"""";

  const Module* m = parseModule(context, iterText);
  auto loop = m->stmt(2)->toFor();

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto idx = rr.byAst(loop->index());
  assert(idx.type().isErroneousType());
  assert(guard.realizeErrors() == 1);
}

static void testThese() {
  printf("testThese\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  //
  // Test iteration over expression requiring ".these()"
  //

  auto theseText = recIter +
                   R""""(
                   var r : R;
                   for i in r {
                   }
                   )"""";

  const Module* m = parseModule(context, theseText);
  auto loop = m->stmt(2)->toFor();

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto idx = rr.byAst(loop->index());
  assert(idx.type().type() == IntType::get(context, 0));
}

static void testTheseReturn() {
  printf("testTheseReturn\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  //
  // Should call .these on the returned value
  //
  auto theseText = recIter +
                   R""""(
                   proc retR() {
                     var r : R;
                     return r;
                   }
                   for i in retR() {
                   }
                   )"""";

  const Module* m = parseModule(context, theseText);
  auto loop = m->stmt(2)->toFor();

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto idx = rr.byAst(loop->index());
  assert(idx.type().type() == IntType::get(context, 0));
}

static void testNoThese() {
  printf("testNoThese\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  //
  // Should issue some kind of error
  //
  auto theseText = R""""(
                   record R {
                   }
                   var r : R;
                   for i in r {
                   }
                   )"""";

  const Module* m = parseModule(context, theseText);
  auto loop = m->stmt(2)->toFor();

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto idx = rr.byAst(loop->index());
  assert(idx.type().isErroneousType());
  assert(guard.realizeErrors() == 1);
}

static void testAmbiguousThese() {
  printf("testAmbiguousThese\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  //
  // Should issue some kind of ambiguity error
  //
  auto theseText = R""""(
                   record R {
                     iter these() {
                       yield 1;
                     }
                     iter these() {
                       yield "str";
                     }
                   }
                   var r : R;
                   for i in r {
                   }
                   )"""";

  const Module* m = parseModule(context, theseText);
  auto loop = m->stmt(2)->toFor();

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto idx = rr.byAst(loop->index());
  assert(idx.type().isErroneousType());
  assert(guard.realizeErrors() == 1);
}

static void testNoIndex() {
  printf("testNoIndex\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto iterText = myiter +
                  R""""(
                  for myiter() {
                  }
                  )"""";
  const Module* m = parseModule(context, iterText);

  resolveModule(context, m->id());
}

static void testTheseNoIndex() {
  printf("testTheseNoIndex\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto theseText = recIter +
                   R""""(
                   var r : R;
                   for r {
                   }
                   )"""";
  const Module* m = parseModule(context, theseText);
  auto loop = m->stmt(2)->toFor();

  //
  // Confirm that a '.these' iterator was still resolved and stored
  //
  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto& iterandRE = rr.byAst(loop->iterand());
  auto& associatedActions = iterandRE.associatedActions();
  assert(associatedActions.size() == 1);

  auto theseFn = associatedActions[0].fn();
  auto recR = m->stmt(0)->toRecord();
  auto recThese = recR->declOrComment(0)->toFunction();
  assert(theseFn->id() == recThese->id());
}

static void testCForLoop() {
  printf("testCForLoop\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto iterText = R""""(
                   iter myIter() {
                     var i : int;
                     while __primitive("C for loop",
                             __primitive("=", i, 0),
                             __primitive("<=", i, 9),
                             __primitive("+=", i, 1)) {
                       yield i;
                     }
                   }

                   var x = 0;
                   for i in myIter() {
                     __primitive("+=", x, i);
                   }
                   )"""";
  const Module* m = parseModule(context, iterText);
  auto loop = m->stmt(2)->toFor();

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto idx = rr.byAst(loop->index());
  assert(idx.type().type() == IntType::get(context, 0));

  const TypedFnSignature* sig = rr.byAst(loop->iterand()).mostSpecific().only();
  auto fn = resolveFunction(context, sig, nullptr);
  auto rf = fn->resolutionById();
  auto whileLoop = m->stmt(0)->toFunction()->stmt(1)->toWhile();
  auto cond = rf.byAst(whileLoop->condition());
  assert(cond.type().type() == BoolType::get(context, 0));
}

static void testParamFor() {
  printf("testParamFor\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  //
  // Test iteration over an iterator call
  //

  auto iterText = R""""(
                  var x = 0;
                  for param i in 1..10 {
                    param n = __primitive("+", i, i);
                    __primitive("+=", x, n);
                  }
                  )"""";

  const Module* m = parseModule(context, iterText);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  ParamCollector pc;
  ResolvedVisitor<ParamCollector> rv(context, m, pc, rr);
  m->traverse(rv);

  const ResolvedExpression& re = rr.byAst(m->stmt(0));
  assert(re.type().type() == IntType::get(context, 0));

  std::map<std::string, int> resolvedIdents;
  std::multimap<std::string, int64_t> resolvedVals;
  for (int i = 1; i <= 10; i++) {
    resolvedIdents["i"] += 2;
    resolvedIdents["n"] += 1;
    resolvedIdents["x"] += 1;
    resolvedVals.emplace("i", i);
    resolvedVals.emplace("n", i+i);
  }
  assert(resolvedIdents == pc.resolvedIdents);
  assert(resolvedVals == pc.resolvedVals);
}


//
// Test nested param loops
//
static void testNestedParamFor() {
  printf("testNestedParamFor\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto loopText = R"""(
  var sum = 0;

  for param i in 1..3 {
    param n = __primitive("+", i, i);
    __primitive("+=", sum, n);

    for param j in 1..5 {
      param m = __primitive("+", n, j);
      __primitive("+=", sum, m);
    }
  }
  )""";
  const Module* m = parseModule(context, loopText);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  ParamCollector pc;
  ResolvedVisitor<ParamCollector> rv(context, m, pc, rr);
  m->traverse(rv);

  const ResolvedExpression& re = rr.byAst(m->stmt(0));
  assert(re.type().type() == IntType::get(context, 0));

  std::map<std::string, int> resolvedIdents;
  std::multimap<std::string, int64_t> resolvedVals;
  for (int i = 1; i <= 3; i++) {
    resolvedIdents["i"] += 2;
    resolvedIdents["n"] += 1;
    resolvedIdents["sum"] += 1;

    resolvedVals.emplace("i", i);
    resolvedVals.emplace("n", i+i);

    for (int j = 1; j <= 5; j++) {
      resolvedIdents["j"] += 1;
      resolvedIdents["n"] += 1;
      resolvedIdents["m"] += 1;
      resolvedIdents["sum"] += 1;

      resolvedVals.emplace("j", j);
      resolvedVals.emplace("m", i + i + j);
    }
  }

  assert(resolvedIdents == pc.resolvedIdents);
  assert(resolvedVals == pc.resolvedVals);
}

static void testIndexScope() {
  printf("testIndexScope\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  //
  // Ensure that the loop iterand is scope-resolved 'outside' of the loop,
  // such that the argument 'i' doesn't refer to the loop index variable 'i'.
  //

  auto iterText = R""""(
                  iter myiter(arg : int) {
                    yield "str";
                  }

                  var i = 5;
                  for i in myiter(i) {
                  }
                  )"""";

  const Module* m = parseModule(context, iterText);
  auto loop = m->stmt(2)->toFor();

  const ResolutionResultByPostorderID& rr = scopeResolveModule(context, m->id());
  auto arg = loop->iterand()->toCall()->actual(0);
  auto argRes = rr.byAst(arg);
  assert(argRes.toId() == m->stmt(1)->id());
  assert(!guard.realizeErrors());
}


int main() {
  testSimpleLoop("for");
  testSimpleLoop("coforall");
  testSimpleLoop("foreach");

  testAmbiguous();
  testThese();
  testTheseReturn();
  testNoThese();
  testAmbiguousThese();
  testNoIndex();
  testTheseNoIndex();
  testCForLoop();
  testParamFor();
  testNestedParamFor();
  testIndexScope();

  return 0;
}
