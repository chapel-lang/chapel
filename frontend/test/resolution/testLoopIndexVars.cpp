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

template <paramtags::ParamTag Tag, typename ParamT>
struct ParamCollector {
  using RV = ResolvedVisitor<ParamCollector>;

  using ValueT = decltype(std::declval<const ParamT*>()->value());

  std::map<std::string, int> resolvedIdents;
  std::multimap<std::string, ValueT> resolvedVals;

  ParamCollector() { }

  bool enter(const uast::VarLikeDecl* decl, RV& rv) {
    if (decl->storageKind() == Qualifier::PARAM ||
        decl->storageKind() == Qualifier::INDEX) {
      const ResolvedExpression& rr = rv.byAst(decl);
      auto val = rr.type().param();
      assert(val != nullptr && val->tag() == Tag);

      resolvedVals.emplace(decl->name().str(), ((const ParamT*) val)->value());
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
  auto context = buildStdContext();

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
  auto context = buildStdContext();
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
  assert(idx.type().isUnknown());
  assert(guard.realizeErrors() == 1);
}

static void testThese() {
  printf("testThese\n");
  auto context = buildStdContext();
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
  auto context = buildStdContext();
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
  auto context = buildStdContext();
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
  auto context = buildStdContext();
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
  auto context = buildStdContext();
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
  auto context = buildStdContext();
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
  auto context = buildStdContext();
  ErrorGuard guard(context);
  ResolutionContext rcval(context);
  auto rc = &rcval;

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
  auto fn = resolveFunction(rc, sig, nullptr);
  auto rf = fn->resolutionById();
  auto whileLoop = m->stmt(0)->toFunction()->stmt(1)->toWhile();
  auto cond = rf.byAst(whileLoop->condition());
  assert(cond.type().type() == BoolType::get(context));
}

// Note: values hardcoded to match 'helpTestParam' helper
struct IntParamLoopTester {
  ParamCollector<paramtags::ParamTag::IntParam, IntParam> pc;
  std::map<std::string, int> resolvedIdents;
  std::multimap<std::string, int64_t> resolvedVals;

  void noteExpectedIteration(int i) {
    resolvedIdents["i"] += 2;
    resolvedIdents["n"] += 1;
    resolvedIdents["x"] += 1;
    resolvedVals.emplace("i", i);
    resolvedVals.emplace("n", i+i);
  }

  void assertMatch() {
    assert(resolvedIdents == pc.resolvedIdents);
    assert(resolvedVals == pc.resolvedVals);
  }
};

static IntParamLoopTester helpTestIntParam(const char* rangeExpr, const char* extraCode = "") {
  printf("testParamFor with %s\n", rangeExpr);
  auto context = buildStdContext();
  ErrorGuard guard(context);
  ResolutionContext rcval(context);
  //
  // Test iteration over an iterator call
  //

  auto iterText = R"""(
                  var x = 0;
                  for param i in )""" + std::string(rangeExpr) + R"""( {
                    param n = __primitive("+", i, i);
                    __primitive("+=", x, n);
                    )""" + std::string(extraCode) + R"""(
                  }
                  )""";

  const Module* m = parseModule(context, iterText);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  ParamCollector<paramtags::ParamTag::IntParam, IntParam> pc;
  ResolvedVisitor<decltype(pc)> rv(&rcval, m, pc, rr);
  m->traverse(rv);

  const ResolvedExpression& re = rr.byAst(m->stmt(0));
  assert(re.type().type() == IntType::get(context, 0));
  return IntParamLoopTester{std::move(pc), {}, {}};
}

static void testParamFor() {
  {
    auto test = helpTestIntParam("1..10");
    for (int i = 1; i <= 10; i++) {
      test.noteExpectedIteration(i);
    }
    test.assertMatch();
  }

  {
    auto test = helpTestIntParam("1..10 by 2");
    for (int i = 1; i <= 10; i += 2) {
      test.noteExpectedIteration(i);
    }
    test.assertMatch();
  }

  {
    auto test = helpTestIntParam("1..10 by -2");
    for (int i = 10; i >= 1; i -= 2) {
      test.noteExpectedIteration(i);
    }
    test.assertMatch();
  }

  {
    auto test = helpTestIntParam("1..<10");
    for (int i = 1; i < 10; i++) {
      test.noteExpectedIteration(i);
    }
    test.assertMatch();
  }

  {
    auto test = helpTestIntParam("1..<10 by -2");
    for (int i = 9; i >= 1; i -= 2) {
      test.noteExpectedIteration(i);
    }
    test.assertMatch();
  }

  {
    auto test = helpTestIntParam("0..#10");
    for (int i = 0; i < 10; i ++) {
      test.noteExpectedIteration(i);
    }
    test.assertMatch();
  }

  {
    auto test = helpTestIntParam("0..#10 by -2");
    for (int i = 9; i >= 0; i -= 2) {
      test.noteExpectedIteration(i);
    }
    test.assertMatch();
  }

  {
    auto test = helpTestIntParam("..10#10");
    for (int i = 1; i <= 10; i++) {
      test.noteExpectedIteration(i);
    }
    test.assertMatch();
  }

  {
    auto test = helpTestIntParam("..10#10 by -2");
    for (int i = 10; i >= 1; i -= 2) {
      test.noteExpectedIteration(i);
    }
    test.assertMatch();
  }

  {
    auto test = helpTestIntParam("1..10", "break;");
    for (int i = 1; i <= 10; i++) {
      test.noteExpectedIteration(i);
      break;
    }
    test.assertMatch();
  }

  {
    auto test = helpTestIntParam("1..10", "continue;");
    for (int i = 1; i <= 10; i++) {
      test.noteExpectedIteration(i);
      continue;
    }
    test.assertMatch();
  }
}

//
// Test nested param loops
//
static void testNestedParamFor() {
  printf("testNestedParamFor\n");
  auto context = buildStdContext();
  ErrorGuard guard(context);
  ResolutionContext rcval(context);
  auto rc = &rcval;

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
  ParamCollector<paramtags::ParamTag::IntParam, IntParam> pc;
  ResolvedVisitor<decltype(pc)> rv(rc, m, pc, rr);
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

static void testNestedParamForLabeledBreakContinue(const std::string& control, int expectedWarnings) {
  auto program =
    R"""(
      label outer
      for param i in 0..3 {

        label inner
        for param j in 0..3 {
         compilerWarning("Start iteration i = " + (i : string) + ", j = " + (j : string));
         )""" + control + R"""(
         compilerWarning("End iteration i = " + (i : string) + ", j = " + (j : string));
        }
      }
  )""";
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);

  std::ignore = resolveTypesOfVariables(ctx, program, {});
  CHPL_ASSERT(guard.numErrors(/* countWarnings */ false) == 0);
  // expected warnings is multiplied by two since we track emitting AND showing
  // the warning
  CHPL_ASSERT(guard.numErrors() == (size_t) expectedWarnings * 2);
  guard.realizeErrors();
}

static void testNestedParamForLabeledBreakContinue() {
  // when not breaking or continuing, should print 2 * 4 * 4 = 32 warnings
  testNestedParamForLabeledBreakContinue("", 32);

  // when continuing without a label, we only print the start but not
  // the end warning. The same should be true if we explicitly list
  // the inner label.
  testNestedParamForLabeledBreakContinue("continue;", 16);
  testNestedParamForLabeledBreakContinue("continue inner;", 16);

  // when breaking without a label, we should print one "start iteration"
  // per outer loop invcation, and no "end iteration" warnings.
  testNestedParamForLabeledBreakContinue("break;", 4);
  testNestedParamForLabeledBreakContinue("break inner;", 4);

  // Since there's no code after the inner loop, breaking the inner loop
  // is the same as continuing the outer loop.
  testNestedParamForLabeledBreakContinue("continue outer;", 4);

  // breaking the outer loop should stop all iterations
  testNestedParamForLabeledBreakContinue("break outer;", 1);

  // test conditionally breaking the outer loop. This will break on the 8th
  // iteration of the inner body.
  testNestedParamForLabeledBreakContinue("if i * 4 + j == 7 { break outer; }", 15);

  // test conditionally continuing the outer loop. This will continue on the 8th
  // iteration of the inner body.
  testNestedParamForLabeledBreakContinue("if i * 4 + j == 7 { continue outer; }", 31);
}

struct TupleCollector {
  using RV = ResolvedVisitor<TupleCollector>;

  std::multimap<std::string, const Type*> resolvedVals;

  TupleCollector() { }

  bool enter(const uast::VarLikeDecl* decl, RV& rv) {
    if (decl->storageKind() == Qualifier::PARAM ||
        decl->storageKind() == Qualifier::INDEX) {
      const ResolvedExpression& rr = rv.byAst(decl);
      if (!rr.type().isUnknownOrErroneous()) {
        resolvedVals.emplace(decl->name().str(), rr.type().type());
      }
    }
    return true;
  }

  bool enter(const uast::AstNode* ast, RV& rv) {
    return true;
  }
  void exit(const uast::AstNode* ast, RV& rv) {}
};

static void testHeteroTuples() {
  printf("testHeteroTuples\n");
  auto context = buildStdContext();
  ErrorGuard guard(context);
  ResolutionContext rcval(context);
  auto rc = &rcval;

  auto loopText = R"""(
  var tmp = (1.0, 1, true);

  for x in tmp {
    for y in tmp {

    }
  }
  )""";
  const Module* m = parseModule(context, loopText);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  TupleCollector pc;
  ResolvedVisitor<TupleCollector> rv(rc, m, pc, rr);
  m->traverse(rv);

  std::multimap<std::string, const Type*> expected;
  expected.emplace("x", RealType::get(context, 0));
  expected.emplace("y", RealType::get(context, 0));
  expected.emplace("y", IntType::get(context, 0));
  expected.emplace("y", BoolType::get(context));
  expected.emplace("x", IntType::get(context, 0));
  expected.emplace("y", RealType::get(context, 0));
  expected.emplace("y", IntType::get(context, 0));
  expected.emplace("y", BoolType::get(context));
  expected.emplace("x", BoolType::get(context));
  expected.emplace("y", RealType::get(context, 0));
  expected.emplace("y", IntType::get(context, 0));
  expected.emplace("y", BoolType::get(context));

  assert(expected == pc.resolvedVals);
}

static void testIndexScope0() {
  printf("testIndexScope0\n");
  auto context = buildStdContext();
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
  auto context = buildStdContext();
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
    iter i1(param tag: iterKind) where tag == iterKind.standalone { yield 0.0; }
    iter i2(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
    iter i2(param tag: iterKind, followThis) where tag == iterKind.follower { yield ""; }
    iter i2() { yield ""; }

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
    assert(m["b"].type()->isRealType());
    assert(m["c"].kind() == QualifiedType::CONST_VAR);
    assert(m["c"].type()->isStringType());
    assert(m["d"].kind() == QualifiedType::CONST_VAR);
    assert(m["d"].type()->isStringType());
}

static void testExplicitTaggedIter(Context* context) {
  printf("%s\n", __FUNCTION__);
  ErrorGuard guard(context);

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  std::string program =
    R""""(

    iter i1() { yield 0.0; }
    iter i2(param tag: iterKind) where tag == iterKind.standalone { yield 0; }
    iter i3(param tag: iterKind) where tag == iterKind.leader { yield (0.0,0.0); }
    iter i4(param tag: iterKind, followThis) where tag == iterKind.follower { yield ""; }

    for a in i1() do;
    for b in i2(tag=iterKind.standalone) do;
    for c in i3(tag=iterKind.leader) do;
    for d in i4(tag=iterKind.follower, followThis="") do;
    )"""";

    auto mod = parseModule(context, program);
    auto& rr = resolveModule(context, mod->id());
    assert(!guard.realizeErrors());

    auto aLoop = parentAst(context, findVariable(mod, "a"))->toIndexableLoop();
    assert(rr.byAst(aLoop->iterand()).associatedActions().empty());
    auto aSig1 = rr.byAst(aLoop->iterand()).mostSpecific().only().fn();
    assert(aSig1->isSerialIterator(context));

    auto bLoop = parentAst(context, findVariable(mod, "b"))->toIndexableLoop();
    assert(rr.byAst(bLoop->iterand()).associatedActions().empty());
    auto bSig1 = rr.byAst(bLoop->iterand()).mostSpecific().only().fn();
    assert(bSig1->isParallelStandaloneIterator(context));

    auto cLoop = parentAst(context, findVariable(mod, "c"))->toIndexableLoop();
    assert(rr.byAst(cLoop->iterand()).associatedActions().empty());
    auto cSig1 = rr.byAst(cLoop->iterand()).mostSpecific().only().fn();
    assert(cSig1->isParallelLeaderIterator(context));

    auto dLoop = parentAst(context, findVariable(mod, "d"))->toIndexableLoop();
    assert(rr.byAst(dLoop->iterand()).associatedActions().empty());
    auto dSig1 = rr.byAst(dLoop->iterand()).mostSpecific().only().fn();
    assert(dSig1->isParallelFollowerIterator(context));

    auto m = resolveTypesOfVariables(context, program, { "a", "b", "c", "d"});
    assert(!guard.realizeErrors());
    assert(m["a"].kind() == QualifiedType::CONST_VAR);
    assert(m["a"].type()->isRealType());
    assert(m["b"].kind() == QualifiedType::CONST_VAR);
    assert(m["b"].type()->isIntType());
    assert(m["c"].kind() == QualifiedType::CONST_VAR);
    assert(m["c"].type()->isTupleType());
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

// Invoke an iterator in a loop expression, yielding a tuple of the iterator's
// yield values, then invoke the loop expression in a regular loop and ensure
// the index variable is as expected.
//
//
// Note: this test will need to be updated when we have support for 
// "precipitating" loop expressions into arrays. Right now, it assumes
// the ability to store loop expressions into variables.
template <typename Predicate>
static void pairIteratorInLoopExpression(
    Context* context, const char* iterators, const char* iterCall,
    std::array<const char*, 2> loopExprType, std::array<const char*, 2> loopType,
    Predicate&& pred, int expectErrors = 0) {
  ErrorGuard guard(context);
  ADVANCE_PRESERVING_STANDARD_MODULES_(context);

  std::string program = std::string(iterators) + "\n";
  program = program + "var r1: _iteratorRecord = " + loopExprType[0] + " i in " + iterCall + " " + loopExprType[1] + " (i, i);\n";
  program = program + loopType[0] + " j in r1 "  + loopType[1] + " j;\n";

  printf("===== Resolving program =====\n");
  printf("%s\n", program.c_str());
  printf("=============================\n\n");

  auto vars = resolveTypesOfVariables(context, program, { "r1", "j" });

  assert(guard.realizeErrors() == expectErrors);
  if (expectErrors) return;

  assert(vars.at("r1").kind() == QualifiedType::VAR);
  assert(vars.at("r1").type()->isLoopExprIteratorType());
  assert(vars.at("r1").type()->toLoopExprIteratorType()->yieldType().type());

  auto yieldedType = vars.at("r1").type()->toLoopExprIteratorType()->yieldType();
  assert(yieldedType.type()->isTupleType());
  assert(yieldedType.type()->toTupleType()->isStarTuple());
  assert(yieldedType.type()->toTupleType()->numElements() == 2);
  assert(pred(yieldedType.type()->toTupleType()->starType()));

  assert(vars.at("j") == yieldedType);
}

static void testForLoopExpression(Context* context) {

  printf("%s\n", __FUNCTION__);

  auto iters =
    R""""(
    iter i1() { yield 0.0; }
    iter i1(param tag: iterKind) where tag == iterKind.standalone { yield 0.0; }
    iter i1(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
    iter i1(param tag: iterKind, followThis) where tag == iterKind.follower { yield 0.0; }
    )"""";

  pairIteratorInLoopExpression(context, iters, "i1()", {"for", "do"}, {"for", "do"},
                               [](const QualifiedType& t) { return t.type()->isRealType(); });
}

static void testForallLoopExpressionStandalone(Context* context) {
  auto iters =
    R""""(
    iter i1() { yield 0.0; }
    iter i1(param tag: iterKind) where tag == iterKind.standalone { yield 0.0; }
    )"""";

  pairIteratorInLoopExpression(context, iters, "i1()", {"forall", "do"}, {"forall", "do"},
                               [](const QualifiedType& t) { return t.type()->isRealType(); });
}

static void testForallLoopExpressionLeaderFollower(Context* context) {
  // Note: compred to the previous test, no standalone iterator is available
  auto iters =
    R""""(
    iter i1() { yield 0.0; }
    iter i1(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
    iter i1(param tag: iterKind, followThis) where tag == iterKind.follower { yield 0.0; }
    )"""";

  pairIteratorInLoopExpression(context, iters, "i1()", {"forall", "do"}, {"forall", "do"},
                               [](const QualifiedType& t) { return t.type()->isRealType(); });
}

static void testBracketLoopExpressionStandalone(Context* context) {
  auto iters =
    R""""(
    iter i1(param tag: iterKind) where tag == iterKind.standalone { yield 0.0; }
    )"""";

  pairIteratorInLoopExpression(context, iters, "i1()", {"[", "]"}, {"[", "]"},
                               [](const QualifiedType& t) { return t.type()->isRealType(); });
}

static void testBracketLoopExpressionStandaloneZipperedSingleton(Context* context) {
  auto iters =
    R""""(
    iter i1(param tag: iterKind) where tag == iterKind.standalone { yield 0.0; }
    )"""";

  pairIteratorInLoopExpression(context, iters, "zip(i1())", {"[", "]"}, {"[", "]"},
                               [](const QualifiedType& t) { return t.type()->isRealType(); });
}

static void testBracketLoopExpressionStandaloneZippered(Context* context) {
  auto iters =
    R""""(
    iter i1(param tag: iterKind) where tag == iterKind.standalone { yield 0.0; }
    )"""";

  pairIteratorInLoopExpression(context, iters, "zip(i1(), i1())", {"[", "]"}, {"[", "]"},
                               [](const QualifiedType& t) { assert(false && "should not be called"); return true; },
                               /* expectErrors */ 1);
}

static void testBracketLoopExpressionZippered(Context* context) {
  auto iters =
    R""""(
    iter i1(param tag: iterKind) where tag == iterKind.standalone { yield 0.0; }
    iter i1(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
    iter i1(param tag: iterKind, followThis) where tag == iterKind.follower { yield 0.0; }
    )"""";

  pairIteratorInLoopExpression(context, iters, "zip(i1(), i1())", {"[", "]"}, {"[", "]"},
                               [](const QualifiedType& t) {
    return t.type()->isTupleType() &&
           t.type()->toTupleType()->starType().type()->isRealType();
  });
}

static void testBracketLoopExpressionLeaderFollower(Context* context) {
  // Note: compred to the previous test, no standalone iterator is available
  auto iters =
    R""""(
    iter i1() { yield 0.0; }
    iter i1(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
    iter i1(param tag: iterKind, followThis) where tag == iterKind.follower { yield 0.0; }
    )"""";

  // Follower iterator yields tuples of ints, so expect tuples of ints.
  pairIteratorInLoopExpression(context, iters, "i1()", {"[", "]"}, {"[", "]"},
                               [](const QualifiedType& t) { return t.type()->isRealType(); });
}

static void testBracketLoopExpressionSerial(Context* context) {
  // Note: compred to the previous test, no standalone iterator is available
  auto iters =
    R""""(
    iter i1() { yield 0.0; }
    )"""";

  // Follower iterator yields tuples of ints, so expect tuples of ints.
  pairIteratorInLoopExpression(context, iters, "i1()", {"[", "]"}, {"[", "]"},
                               [](const QualifiedType& t) { return t.type()->isRealType(); });
}

static void testForLoopExpressionInForall(Context* context) {
  auto iters =
    R""""(
    iter i1() { yield 0.0; }
    iter i1(param tag: iterKind) where tag == iterKind.standalone { yield 0.0; }
    iter i1(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
    iter i1(param tag: iterKind, followThis) where tag == iterKind.follower { yield 0.0; }
    )"""";

  // You can't iterate in paralell over a serial loop expression, even if
  // the overloads for the iterands are present.
  pairIteratorInLoopExpression(context, iters, "i1()", {"for", "do"}, {"forall", "do"},
                               [](const QualifiedType& t) { return true; },
                               /* expectErrors */ 1);
}

static void testForLoopExpressionInBracketLoop(Context* context) {
  auto iters =
    R""""(
    iter i1() { yield 0.0; }
    iter i1(param tag: iterKind) where tag == iterKind.standalone { yield 0.0; }
    iter i1(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
    iter i1(param tag: iterKind, followThis) where tag == iterKind.follower { yield 0.0; }
    )"""";

  // Bracket loop falls back to serial, so it's okay to give it a for expression.
  pairIteratorInLoopExpression(context, iters, "i1()", {"for", "do"}, {"[", "]"},
                               [](const QualifiedType& t) { return t.type()->isRealType(); });
}

// At this time, because parallel loops require a serial overload, you can
// fall back to the serial iterator even if you're using a forall expression.
// However, since we are not liking the idea of changing the return type based
// on context, the loop yield type is still the one from the parallel loop.
// This probably won't compile if we were to add type compatibility checks.
static void testForallExpressionInForLoop(Context* context) {
  auto iters =
    R""""(
    iter i1() { yield 0.0; }
    iter i1(param tag: iterKind) where tag == iterKind.standalone { yield 0.0; }
    iter i1(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
    iter i1(param tag: iterKind, followThis) where tag == iterKind.follower { yield 0.0; }
    )"""";

  // Bracket loop falls back to serial, so it's okay to give it a for expression.
  pairIteratorInLoopExpression(context, iters, "i1()", {"forall", "do"}, {"for", "do"},
                               [](const QualifiedType& t) { return t.type()->toRealType(); });
}

static void testBracketLoopSerialFallback(Context* context) {
  // Test that a bracket loop expression falls back to the serial iterator
  // if a leader is present, but one of the followers is not.
  auto iters =
    R""""(
    iter i1() { yield 0.0; }
    iter i1(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
    iter i1(param tag: iterKind, followThis) where tag == iterKind.follower { yield 0.0; }

    iter i2() { yield 0.0; }
    iter i2(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
    )"""";

  // Follower iterator yields tuples of ints, so expect tuples of ints.
  pairIteratorInLoopExpression(context, iters, "zip(i1(), i2())", {"[", "]"}, {"[", "]"},
                               [](const QualifiedType& t) {
    if (auto tt = t.type()->toTupleType()) {
      if (!tt->isStarTuple()) return false;
      return tt->starType().type()->isRealType();
    }
    return false;
  });
}

static void testUnpackingFromIterator(Context* context) {
  ErrorGuard guard(context);
  auto prog =
    R""""(
    iter i1() { yield (0.0, "hello"); }

    for (a, b) in i1() {}
    )"""";

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto types = resolveTypesOfVariables(context, prog, {"a", "b"});

  assert(types.at("a").type());
  assert(types.at("a").type()->isRealType());
  assert(types.at("b").type());
  assert(types.at("b").type()->isStringType());
}

static void testBasicUnpacking(Context* context) {
  ErrorGuard guard(context);
  auto prog =
    R""""(
    iter i1() { yield 0.0; }

    iter i2() { yield "hello"; }

    for (a, b) in zip(i1(), i2()) {}
    )"""";

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto types = resolveTypesOfVariables(context, prog, {"a", "b"});

  assert(types.at("a").type());
  assert(types.at("a").type()->isRealType());
  assert(types.at("b").type());
  assert(types.at("b").type()->isStringType());
}

static void testBasicUnpackingFailure(Context* context) {
  ErrorGuard guard(context);
  auto prog =
    R""""(
    iter i1() { yield 0.0; }

    for (a, b) in i1() {}
    )"""";

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto types = resolveTypesOfVariables(context, prog, {"a", "b"});

  assert(types.at("a").isUnknown());
  assert(types.at("b").isUnknown());

  assert(guard.numErrors() > 0);
  assert(guard.error(0)->type() == ErrorType::TupleDeclNotTuple);
  guard.realizeErrors();
}

static void testForallUnpacking(Context* context) {
  ErrorGuard guard(context);
  auto prog =
    R""""(
    iter i1(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
    iter i1(param tag: iterKind, followThis) where tag == iterKind.follower { yield 1.0; }
    iter i2(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
    iter i2(param tag: iterKind, followThis) where tag == iterKind.follower { yield "hello"; }

    forall (a, b) in zip(i1(), i2()) {}
    )"""";

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto types = resolveTypesOfVariables(context, prog, {"a", "b"});

  assert(!guard.realizeErrors());

  assert(types.at("a").type());
  assert(types.at("a").type()->isRealType());
  assert(types.at("b").type());
  assert(types.at("b").type()->isStringType());
}

static void testLoopExprZipUnpacking(Context* context) {
  ErrorGuard guard(context);
  auto prog =
    R""""(
    iter dummy() do yield 0;

    for ((a, b), (c, d)) in zip(foreach (_, _) in zip(dummy(), dummy()) do (1, "hello"),
                                foreach (_, _) in zip(dummy(), dummy()) do (1.0, false)) {}
    )"""";

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto types = resolveTypesOfVariables(context, prog, {"a", "b", "c", "d"});

  assert(!guard.realizeErrors());

  assert(types.at("a").type());
  assert(types.at("a").type()->isIntType());
  assert(types.at("b").type());
  assert(types.at("b").type()->isStringType());
  assert(types.at("c").type());
  assert(types.at("c").type()->isRealType());
  assert(types.at("d").type());
  assert(types.at("d").type()->isBoolType());
}

static void testLoopExprZipUnpackingTooFewInner(Context* context) {
  ErrorGuard guard(context);
  auto prog =
    R""""(
    iter dummy() do yield 0;

    for ((a, ), (c, d)) in zip(foreach (_, _) in zip(dummy(), dummy()) do (1, "hello"),
                               foreach (_, _) in zip(dummy(), dummy()) do (1.0, false)) {}
    )"""";

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto types = resolveTypesOfVariables(context, prog, {"a", "c", "d"});

  assert(guard.numErrors() > 0);
  assert(guard.error(0)->type() == ErrorType::TupleDeclMismatchedElems);
  assert(guard.realizeErrors());
}

static void testLoopExprZipUnpackingTooManyInner(Context* context) {
  ErrorGuard guard(context);
  auto prog =
    R""""(
    iter dummy() do yield 0;

    for ((a, b, _), (c, d)) in zip(foreach (_, _) in zip(dummy(), dummy()) do (1, "hello"),
                                foreach (_, _) in zip(dummy(), dummy()) do (1.0, false)) {}
    )"""";

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto types = resolveTypesOfVariables(context, prog, {"a", "b", "c", "d"});

  assert(guard.numErrors() > 0);
  assert(guard.error(0)->type() == ErrorType::TupleDeclMismatchedElems);
  assert(guard.realizeErrors());
}

static void testLoopExprZipUnpackingTooFewOuter(Context* context) {
  ErrorGuard guard(context);
  auto prog =
    R""""(
    iter dummy() do yield 0;

    for ((a, b), (c, d)) in zip(foreach (_, _) in zip(dummy(), dummy()) do (1, "hello"),
                                foreach (_, _) in zip(dummy(), dummy()) do (1.0, false),
                                dummy()) {}
    )"""";

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto types = resolveTypesOfVariables(context, prog, {"a", "b", "c", "d"});

  assert(guard.numErrors() > 0);
  assert(guard.error(0)->type() == ErrorType::TupleDeclMismatchedElems);
  assert(guard.realizeErrors());
}

static void testLoopExprZipUnpackingTooManyOuter(Context* context) {
  ErrorGuard guard(context);
  auto prog =
    R""""(
    iter dummy() do yield 0;

    for ((a, b), (c, d), _) in zip(foreach (_, _) in zip(dummy(), dummy()) do (1, "hello"),
                                foreach (_, _) in zip(dummy(), dummy()) do (1.0, false)) {}
    )"""";

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto types = resolveTypesOfVariables(context, prog, {"a", "b", "c", "d"});

  assert(guard.numErrors() > 0);
  assert(guard.error(0)->type() == ErrorType::TupleDeclMismatchedElems);
  assert(guard.realizeErrors());
}

static void testSingletonZip(Context* context){
  // In production, a single-iterator zip does not create a tuple.
  // Make sure we do the same.
  ErrorGuard guard(context);
  auto prog =
    R""""(
    iter dummy() do yield 0;

    for a in zip(dummy()) {}
    )"""";

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  auto types = resolveTypesOfVariables(context, prog, {"a"});

  assert(!guard.realizeErrors());

  assert(types.at("a").type());
  assert(types.at("a").type()->isIntType());
}

static void testBracketLoopExpressionStandaloneUnpackedZipperedSingleton(Context* context) {
  auto iters =
    R""""(
    iter i1(param tag: iterKind) where tag == iterKind.standalone { yield 0.0; }
    )"""";

  pairIteratorInLoopExpression(context, iters, "zip((...(i1(),)))", {"[", "]"}, {"[", "]"},
                               [](const QualifiedType& t) { return t.type()->isRealType(); });
}

static void testBracketLoopExpressionUnpackedZippered(Context* context) {
  auto iters =
    R""""(
    iter i1(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
    iter i1(param tag: iterKind, followThis) where tag == iterKind.follower { yield 0.0; }
    )"""";

  pairIteratorInLoopExpression(context, iters, "zip((...(i1(), i1())))", {"[", "]"}, {"[", "]"},
                               [](const QualifiedType& t) {
    return t.type()->isTupleType() &&
           t.type()->toTupleType()->starType().type()->isRealType();
  });
}

static void testForLoopExpressionTypeMethod(Context* context) {

  printf("%s\n", __FUNCTION__);

  auto iters =
    R""""(
    class C {
      iter type i1() { yield 0.0; }
      iter type i1(param tag: iterKind) where tag == iterKind.standalone { yield 0.0; }
      iter type i1(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
      iter type i1(param tag: iterKind, followThis) where tag == iterKind.follower { yield 0.0; }
    }
    )"""";

  pairIteratorInLoopExpression(context, iters, "C.i1()", {"for", "do"}, {"for", "do"},
                               [](const QualifiedType& t) { return t.type()->isRealType(); });
}

static void testForallLoopExpressionStandaloneTypeMethod(Context* context) {
  auto iters =
    R""""(
    class C {
      iter type i1() { yield 0.0; }
      iter type i1(param tag: iterKind) where tag == iterKind.standalone { yield 0.0; }
    }
    )"""";

  pairIteratorInLoopExpression(context, iters, "C.i1()", {"forall", "do"}, {"forall", "do"},
                               [](const QualifiedType& t) { return t.type()->isRealType(); });
}

static void testForallLoopExpressionLeaderFollowerTypeMethod(Context* context) {
  // Note: compred to the previous test, no standalone iterator is available
  auto iters =
    R""""(
    class C {
      iter type i1() { yield 0.0; }
      iter type i1(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
      iter type i1(param tag: iterKind, followThis) where tag == iterKind.follower { yield 0.0; }
    }
    )"""";

  pairIteratorInLoopExpression(context, iters, "C.i1()", {"forall", "do"}, {"forall", "do"},
                               [](const QualifiedType& t) { return t.type()->isRealType(); });
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
  testNestedParamForLabeledBreakContinue();
  testHeteroTuples();
  testIndexScope0();
  testIndexScope1();

  // Use a single context instance to avoid re-resolving internal modules.
  auto context = buildStdContext();
  testIterSigDetection(context);
  testExplicitTaggedIter(context);
  testSerialZip(context);
  testParallelZip(context);
  testForallStandaloneThese(context);
  testForallStandaloneRedirect(context);
  testForallLeaderFollowerThese(context);
  testForallLeaderFollowerRedirect(context);

  testForLoopExpression(context);
  testForallLoopExpressionStandalone(context);
  testForallLoopExpressionLeaderFollower(context);
  testBracketLoopExpressionStandalone(context);
  testBracketLoopExpressionStandaloneZippered(context);
  testBracketLoopExpressionStandaloneZipperedSingleton(context);
  testBracketLoopExpressionZippered(context);
  testBracketLoopExpressionLeaderFollower(context);
  testBracketLoopExpressionSerial(context);
  testForLoopExpressionInForall(context);
  testForLoopExpressionInBracketLoop(context);
  testForallExpressionInForLoop(context);

  testBracketLoopSerialFallback(context);

  testUnpackingFromIterator(context);
  testBasicUnpacking(context);
  testBasicUnpackingFailure(context);
  testForallUnpacking(context);
  testLoopExprZipUnpacking(context);
  testLoopExprZipUnpackingTooFewInner(context);
  testLoopExprZipUnpackingTooManyInner(context);
  testLoopExprZipUnpackingTooFewOuter(context);
  testLoopExprZipUnpackingTooManyOuter(context);
  testSingletonZip(context);

  testBracketLoopExpressionStandaloneUnpackedZipperedSingleton(context);
  testBracketLoopExpressionUnpackedZippered(context);

  testForLoopExpressionTypeMethod(context);
  testForallLoopExpressionStandaloneTypeMethod(context);
  testForallLoopExpressionLeaderFollowerTypeMethod(context);

  return 0;
}
