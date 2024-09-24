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

#include <map>

#define ADVANCE_PRESERVING_STANDARD_MODULES_(ctx__) \
  do { \
    ctx__->advanceToNextRevision(false); \
    setupModuleSearchPaths(ctx__, false, false, {}, {}); \
  } while (0)

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
    if (decl->storageKind() == Qualifier::PARAM ||
        decl->storageKind() == Qualifier::INDEX) {
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
  assert(idxType.type() == xType.type());
  assert(idxType.kind() == Qualifier::CONST_VAR);
}

//
// Testing resolution of loop index variables
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

  const TypedFnSignature* sig = rr.byAst(loop->iterand()).mostSpecific().only().fn();
  auto fn = resolveFunction(context, sig, nullptr);
  auto rf = fn->resolutionById();
  auto whileLoop = m->stmt(0)->toFunction()->stmt(1)->toWhile();
  auto cond = rf.byAst(whileLoop->condition());
  assert(cond.type().type() == BoolType::get(context));
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

static void testIndexScope0() {
  printf("testIndexScope0\n");
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

static void testIndexScope1() {
  printf("testIndexScope1\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // Ensure that each mention of 'i' refers to the correct index variable.
  auto iterText = R""""(
                  iter foo() { yield 0; }
                  for i in zip(foo(), for i in zip(foo(), foo()) do i) do i;
                  )"""";

  const Module* m = parseModule(context, iterText);
  auto loop = m->stmt(1)->toFor();

  const ResolutionResultByPostorderID& rr = scopeResolveModule(context, m->id());
  assert(!guard.realizeErrors());
  auto idx1 = loop->index();
  auto use1 = loop->body()->stmt(0);
  auto idx2 = loop->iterand()->toZip()->actual(1)->toFor()->index();
  auto use2 = loop->iterand()->toZip()->actual(1)->toFor()->body()->stmt(0);
  assert(rr.byAst(use1).toId() == idx1->id());
  assert(rr.byAst(use2).toId() == idx2->id());
}

static void testIterSigDetection(Context* context) {
  printf("%s\n", __FUNCTION__);
  ErrorGuard guard(context);

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  std::string program =
    R""""(

    iter i1() { yield 0.0; }
    iter i1(param tag: iterKind) where tag == iterKind.standalone { yield 0; }
    iter i2(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
    iter i2(param tag: iterKind, followThis) where tag == iterKind.follower { yield ""; }
    iter i2() { yield false; }

    for a in i1() do;
    forall b in i1() do;
    for c in i2() do;
    forall d in i2() do;

    )"""";

    auto mod = parseModule(context, program);
    auto& rr = resolveModule(context, mod->id());
    assert(!guard.realizeErrors());

    auto aLoop = parentAst(context, findVariable(mod, "a"))->toIndexableLoop();
    auto aSig1 = rr.byAst(aLoop->iterand()).mostSpecific().only().fn();
    assert(aSig1->isSerialIterator(context));

    auto bLoop = parentAst(context, findVariable(mod, "b"))->toIndexableLoop();
    auto bSig1 = rr.byAst(bLoop->iterand()).associatedActions()[0].fn();
    assert(bSig1->isParallelStandaloneIterator(context));

    auto cLoop = parentAst(context, findVariable(mod, "c"))->toIndexableLoop();
    auto cSig1 = rr.byAst(cLoop->iterand()).mostSpecific().only().fn();
    assert(cSig1->isSerialIterator(context));

    auto dLoop = parentAst(context, findVariable(mod, "d"))->toIndexableLoop();
    auto dSig1 = rr.byAst(dLoop->iterand()).associatedActions()[0].fn();
    assert(dSig1->isParallelLeaderIterator(context));
    auto dSig2 = rr.byAst(dLoop->iterand()).associatedActions()[1].fn();
    assert(dSig2->isParallelFollowerIterator(context));

    auto m = resolveTypesOfVariables(context, program, { "a", "b", "c", "d" });
    assert(!guard.realizeErrors());
    assert(m["a"].kind() == QualifiedType::CONST_VAR);
    assert(m["a"].type()->isRealType());
    assert(m["b"].kind() == QualifiedType::CONST_VAR);
    assert(m["b"].type()->isIntType());
    assert(m["c"].kind() == QualifiedType::CONST_VAR);
    assert(m["c"].type()->isBoolType());
    assert(m["d"].kind() == QualifiedType::CONST_VAR);
    assert(m["d"].type()->isStringType());
}

static void
unpackIterKindStrToBool(const std::string& str,
                        bool* needSerial=nullptr,
                        bool* needStandalone=nullptr,
                        bool* needLeader=nullptr,
                        bool* needFollower=nullptr) {
  bool* p = nullptr;
  if (str.empty()) {
    p = needSerial;
  } else if (str == "standalone") {
    p = needStandalone;
  } else if (str == "leader") {
    p = needLeader;
  } else if (str == "follower") {
    p = needFollower;
  } else {
    assert(false && "Invalid 'iterKind' string!");
  }
  if (p) *p = true;
}

static void
assertIterIsCorrect(Context* context, const AssociatedAction& aa,
                    const std::string& iterKindStr) {
  bool needSerial = false;
  bool needStandalone = false;
  bool needLeader = false;
  bool needFollower = false;
  unpackIterKindStrToBool(iterKindStr, &needSerial, &needStandalone,
                          &needLeader,
                          &needFollower);

  assert(aa.action() == AssociatedAction::ITERATE);
  assert(aa.fn());

  auto fn = aa.fn();
  if (needSerial) {
    assert(fn->isSerialIterator(context));
  } else if (needStandalone) {
    assert(fn->isParallelStandaloneIterator(context));
  } else if (needLeader) {
    assert(fn->isParallelLeaderIterator(context));
  } else if (needFollower) {
    assert(fn->isParallelFollowerIterator(context));
  } else {
    assert(false && "Should not reach here!");
  }
}

static void
assertLoopMatches(Context* context, const std::string& program,
                  const char* iterKindStr,
                  int idxLoopAst,
                  int idxIterAst,
                  int idxFollowerIterAst=-1) {
  bool needSerial = false;
  bool needStandalone = false;
  bool needLeader = false;
  bool needFollower = false;
  unpackIterKindStrToBool(iterKindStr, &needSerial, &needStandalone,
                          &needLeader,
                          &needFollower);
  needFollower = needFollower || needLeader;

  // Unpack needed ASTs and properties about them.
  const Module* m = parseModule(context, program);
  auto loop = m->stmt(idxLoopAst)->toIndexableLoop();
  auto iter = m->stmt(idxIterAst)->toFunction();
  auto iterFollower = idxFollowerIterAst > 0
      ? m->stmt(idxFollowerIterAst)->toFunction()
      : nullptr;
  assert(loop && iter && loop->iterand() && loop->index());
  auto iterand = loop->iterand();
  auto index = loop->index();
  bool isIterMethod = parsing::idIsMethod(context, iter->id());
  // bool isBracketLoop = loop->isBracketLoop();
  // bool isForall = loop->isForall();


  // Resolve the module.
  auto& rr = resolveModule(context, m->id());
  auto& reIterand = rr.byAst(iterand);

  if (iterand->toZip()) {
    assert(false && "Zip iterands not handled in this test yet!");
    return;
  } else {
    int numExpectedActions = (needLeader || needFollower) ? 2 : 1;
    assert(reIterand.associatedActions().size() == (size_t) numExpectedActions);

    auto& aa1 = reIterand.associatedActions()[0];
    assertIterIsCorrect(context, aa1, iterKindStr);

    if (needFollower) {
      assert(iterFollower);
      auto& aa2 = reIterand.associatedActions()[1];
      assertIterIsCorrect(context, aa2, "follower");
      bool isFollowerMethod = parsing::idIsMethod(context, iterFollower->id());
      assert(isFollowerMethod == isIterMethod);
    }

    auto& reIndex = rr.byAst(index);
    assert(reIndex.type().type() == IntType::get(context, 0));
  }
}

static void testSerialZip(Context* context) {
  printf("%s\n", __FUNCTION__);
  ErrorGuard guard(context);

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto program = R""""(
                  record r {}
                  iter r.these() do yield 0;
                  var r1 = new r();
                  for tup in zip(r1, r1) do tup;
                  )"""";

  const Module* m = parseModule(context, program);
  auto iter = m->stmt(1)->toFunction();
  auto var = m->stmt(2)->toVariable();
  auto loop = m->stmt(3)->toFor();
  assert(iter && var && loop && loop->iterand() && loop->index());
  auto index = loop->index();
  auto zip = loop->iterand()->toZip();
  assert(zip);

  auto& rr = resolveModule(context, m->id());
  auto& reZip = rr.byAst(zip);

  assert(reZip.associatedActions().empty());

  assert(zip->numActuals() == 2);
  for (auto actual : zip->actuals()) {
    auto& re = rr.byAst(actual);
    assert(re.toId() == var->id());
    assert(re.associatedActions().size() == 1);
    auto& aa = re.associatedActions().back();
    assert(aa.action() == AssociatedAction::ITERATE);
    auto fn = aa.fn();
    assert(fn->untyped()->kind() == Function::ITER);
  }

  auto t = reZip.type().type()->toTupleType();
  assert(t && t->numElements() == 2);
  assert(t->elementType(0).type() == IntType::get(context, 0));
  assert(t->elementType(1).type() == IntType::get(context, 0));

  auto& reIndex = rr.byAst(index);
  assert(reIndex.type() == reZip.type());
  assert(!guard.realizeErrors());
}

static void testParallelZip(Context* context) {
  printf("%s\n", __FUNCTION__);
  ErrorGuard guard(context);

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto program = R""""(
                  record r {}
                  iter r.these(param tag: iterKind) where tag == iterKind.leader do yield (0, 0);
                  iter r.these(param tag: iterKind, followThis) where tag == iterKind.follower do yield 0;
                  var r1 = new r();
                  forall tup in zip(r1, r1) do tup;
                  )"""";

  const Module* m = parseModule(context, program);
  auto iterLeader = m->stmt(1)->toFunction();
  auto iterFollower = m->stmt(2)->toFunction();
  auto var = m->stmt(3)->toVariable();
  auto loop = m->stmt(4)->toForall();
  assert(iterLeader && iterFollower && var && loop &&
         loop->iterand() &&
         loop->iterand()->isZip() &&
         loop->index());
  auto index = loop->index();
  auto zip = loop->iterand()->toZip();

  auto& rr = resolveModule(context, m->id());
  auto& reZip = rr.byAst(zip);

  for (auto& e : guard.errors()) {
    std::cout << e->message() << std::endl;
  }

  assert(reZip.associatedActions().empty());

  assert(zip->numActuals() == 2);
  for (int i = 0; i < zip->numActuals(); i++) {
    auto actual = zip->actual(i);
    auto& re = rr.byAst(actual);
    assert(re.toId() == var->id());
    bool isLeaderActual = (i == 0);

    // Only the first actual should have a leader iterator attached.
    if (isLeaderActual) {
      assert(re.associatedActions().size() == 2);
      auto& aa = re.associatedActions()[0];
      assertIterIsCorrect(context, aa, "leader");
    } else {
      assert(re.associatedActions().size() == 1);
    }

    // Check all actuals for the follower iterator.
    auto& aa = re.associatedActions()[(isLeaderActual ? 1 : 0)];
    assertIterIsCorrect(context, aa, "follower");
  }

  auto t = reZip.type().type()->toTupleType();
  assert(t && t->numElements() == 2);
  assert(t->elementType(0).type() == IntType::get(context, 0));
  assert(t->elementType(1).type() == IntType::get(context, 0));

  auto& reIndex = rr.byAst(index);
  assert(reIndex.type() == reZip.type());
  assert(!guard.realizeErrors());
}

static void testForallStandaloneThese(Context* context) {
  printf("%s\n", __FUNCTION__);
  ErrorGuard guard(context);

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto program = R""""(
                  record r {}
                  iter r.these(param tag: iterKind) where tag == iterKind.standalone do yield 0;
                  var r1 = new r();
                  forall i in r1 do i;
                  )"""";
  assertLoopMatches(context, program, "standalone", 3, 1);
  assert(!guard.realizeErrors());
}

static void testForallStandaloneRedirect(Context* context) {
  printf("%s\n", __FUNCTION__);
  ErrorGuard guard(context);

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto program = R""""(
                  iter foo(param tag: iterKind) where tag == iterKind.standalone do yield 0;
                  forall i in foo() do i;
                  )"""";
  assertLoopMatches(context, program, "standalone", 1, 0);
  assert(!guard.realizeErrors());
}

static void testForallLeaderFollowerThese(Context* context) {
  printf("%s\n", __FUNCTION__);
  ErrorGuard guard(context);

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto program = R""""(
                  record r {}
                  iter r.these(param tag: iterKind) where tag == iterKind.leader do yield (0, 0);
                  iter r.these(param tag: iterKind, followThis) where tag == iterKind.follower do yield 0;
                  var r1 = new r();
                  forall i in r1 do i;
                  )"""";

  assertLoopMatches(context, program, "leader", 4, 1, 2);
  assert(!guard.realizeErrors());
}

static void testForallLeaderFollowerRedirect(Context* context) {
  printf("%s\n", __FUNCTION__);
  ErrorGuard guard(context);

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto program = R""""(
                  iter foo(param tag: iterKind) where tag == iterKind.leader do yield (0, 0);
                  iter foo(param tag: iterKind, followThis) where tag == iterKind.follower do yield 0;
                  forall i in foo() do i;
                  )"""";
  assertLoopMatches(context, program, "leader", 2, 0, 1);
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
  testIndexScope0();
  testIndexScope1();

  // Use a single context instance to avoid re-resolving internal modules.
  auto ctx = buildStdContext();
  Context* context = ctx.get();
  testIterSigDetection(context);
  testSerialZip(context);
  testParallelZip(context);
  testForallStandaloneThese(context);
  testForallStandaloneRedirect(context);
  testForallLeaderFollowerThese(context);
  testForallLeaderFollowerRedirect(context);

  return 0;
}
