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
#include "chpl/uast/all-uast.h"
#include "./ErrorGuard.h"

//
// TODO: Cases that need tests (that the current impl may not handle)...
//  1)  Deeply nested function referencing outer variables from multiple
//      levels at once.
//  2)  Mutually recursive nested functions (requires resolution at the
//      point-of-call).
//  3)  Utilizing outer variables in function signature.
//  4)  Utilization of param/type outer variables.
//  5)  Instantiation of a generic nested function at 1 level deep.
//  6)  Instantiation of a deeply nested generic function requiring
//      substitutions from multiple parent frames.
//

#define TEST_NAME(ctx__)\
  chpl::UniqueString::getConcat(ctx__, __FUNCTION__, ".chpl")

static std::string debugDeclName = "";

static void testDebuggingBreakpoint() {}

static Context*
turnOnWarnUnstable(Context* ctx) {
  testDebuggingBreakpoint();
  CompilerFlags flags;
  flags.set(CompilerFlags::WARN_UNSTABLE, true);
  setCompilerFlags(ctx, std::move(flags));
  assert(isCompilerFlagSet(ctx, CompilerFlags::WARN_UNSTABLE));
  return ctx;
}

// Compute outer variables and manually assert they are correct. This is an
// operation that is basically just collecting a subset of results from
// scope-resolution.
static void test0(void) {
  Context context;
  Context* ctx = turnOnWarnUnstable(&context);
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
    var global = 0;
    proc foo() {
      var x = 0; var y = 0; var z = 0;
      proc bar() {
        var a = 0; var b = 0;
        x; y; z;
        proc baz() {
          a; b; x; y; z; global;
          a; b; x; y; z; global;
          return 0;
        }
        return baz();
      }
      return bar();
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the top module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod && mod->numStmts() == 2);
  auto globalVar = mod->stmt(0)->toVariable();

  // Get the 'foo' function and its declared variables.
  auto fooFn = mod->stmt(1)->toFunction();
  assert(fooFn && fooFn->numStmts() == 5);
  auto xVar = fooFn->stmt(0)->toVariable();
  auto yVar = fooFn->stmt(1)->toVariable();
  auto zVar = fooFn->stmt(2)->toVariable();
  assert(xVar && yVar && zVar);

  // Get the 'bar' function and its declared variables.
  auto barFn = fooFn->stmt(3)->toFunction();
  assert(barFn && barFn->numStmts() == 7);
  auto aVar = barFn->stmt(0)->toVariable();
  auto bVar = barFn->stmt(1)->toVariable();

  // Get the 'baz' function.
  auto bazFn = barFn->stmt(5)->toFunction();
  assert(bazFn && bazFn->numStmts() == 13);

  // The 'foo' function should have no outer variables.
  assert(computeOuterVariables(ctx, fooFn->id()) == nullptr);

  auto ovsBarPtr = computeOuterVariables(ctx, barFn->id());
  assert(ovsBarPtr);
  auto& ovsBar = *ovsBarPtr;
  assert(ovsBar.numVariables() == 3);
  assert(ovsBar.numMentions() == 3);

  assert(ovsBar.numMentions(xVar->id()) == 1);
  assert(ovsBar.firstMention(xVar->id()) == barFn->stmt(2)->id());

  assert(ovsBar.numMentions(yVar->id()) == 1);
  assert(ovsBar.firstMention(yVar->id()) == barFn->stmt(3)->id());

  assert(ovsBar.numMentions(zVar->id()) == 1);
  assert(ovsBar.firstMention(zVar->id()) == barFn->stmt(4)->id());

  auto ovsBazPtr = computeOuterVariables(ctx, bazFn->id());
  assert(ovsBazPtr);
  auto& ovsBaz = *ovsBazPtr;
  assert(ovsBaz.numVariables() == 5);
  assert(ovsBaz.numMentions() == 10);

  assert(ovsBaz.variable(0) == aVar->id());
  assert(ovsBaz.numMentions(aVar->id()) == 2);
  assert(ovsBaz.mention(aVar->id(), 0) == bazFn->stmt(0)->id());
  assert(ovsBaz.mention(aVar->id(), 1) == bazFn->stmt(6)->id());

  assert(ovsBaz.variable(1) == bVar->id());
  assert(ovsBaz.numMentions(bVar->id()) == 2);
  assert(ovsBaz.mention(bVar->id(), 0) == bazFn->stmt(1)->id());
  assert(ovsBaz.mention(bVar->id(), 1) == bazFn->stmt(7)->id());

  assert(ovsBaz.variable(2) == xVar->id());
  assert(ovsBaz.numMentions(xVar->id()) == 2);
  assert(ovsBaz.mention(xVar->id(), 0) == bazFn->stmt(2)->id());
  assert(ovsBaz.mention(xVar->id(), 1) == bazFn->stmt(8)->id());

  assert(ovsBaz.variable(3) == yVar->id());
  assert(ovsBaz.numMentions(yVar->id()) == 2);
  assert(ovsBaz.mention(yVar->id(), 0) == bazFn->stmt(3)->id());
  assert(ovsBaz.mention(yVar->id(), 1) == bazFn->stmt(9)->id());

  assert(ovsBaz.variable(4) == zVar->id());
  assert(ovsBaz.numMentions(zVar->id()) == 2);
  assert(ovsBaz.mention(zVar->id(), 0) == bazFn->stmt(4)->id());
  assert(ovsBaz.mention(zVar->id(), 1) == bazFn->stmt(10)->id());

  assert(!ovsBaz.mentions(globalVar->id()));
  assert(ovsBaz.numMentions(globalVar->id()) == 0);
  assert(!guard.realizeErrors());
}

// The nested function 'bar' is resolved because the parent function calls it.
static void test1(void) {
  Context context;
  Context* ctx = turnOnWarnUnstable(&context);
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
    proc foo() {
      var x = 0;
      proc bar() { return x; }
      var y = bar();
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the top module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod && mod->numStmts() == 1);

  // Get the 'foo' function and its declared variables.
  auto fooFn = mod->stmt(0)->toFunction();
  assert(fooFn && fooFn->numStmts() == 3);
  auto xVar = fooFn->stmt(0)->toVariable();
  auto yVar = fooFn->stmt(2)->toVariable();
  assert(xVar && yVar);

  // Get the 'bar' function and its body.
  auto barFn = fooFn->stmt(1)->toFunction();
  assert(barFn && barFn->numStmts() == 1);
  auto barRetX = barFn->stmt(0)->toReturn();
  assert(barRetX);
  auto barUseX = barRetX->value();
  assert(barUseX);

  // Directly resolve the concrete parent function 'foo'. Because the return
  // type of 'bar' is inferred, the compiler will be forced to resolve the
  // body of 'bar', while it is still resolving 'foo'.
  auto rfFoo = resolveConcreteFunction(ctx, fooFn->id());
  auto& yRe = rfFoo->byAst(yVar);
  auto& yQt = yRe.type();
  assert(yQt.type() == chpl::types::IntType::get(ctx, 0));
  assert(yQt.kind() == QualifiedType::VAR);
  auto sigBar = nestedTypedSignatureInitial(ctx, barFn->id(),
                                            rfFoo->signature());
  assert(sigBar);
  assert(!sigBar->needsInstantiation());
  assert(sigBar->parentFn() == rfFoo->signature());
  auto rfBar = resolveFunction(ctx, sigBar, nullptr);
  auto& reBarUseX = rfBar->byAst(barUseX);
  assert(reBarUseX.type().type() == chpl::types::IntType::get(ctx, 0));
  assert(reBarUseX.type().kind() == QualifiedType::REF);
  assert(reBarUseX.toId() == xVar->id());

  assert(!guard.realizeErrors());
}

// Here is a more complicated example that incorporates deep nesting and
// param/type variables in the signature. The nested function 'bar' should
// be fully resolved by the call to 'bar', because its return type is
// inferred. The nested function 'baz' should not be fully resolved, though
// it is scope resolved in order to determine that 'n' is a dependency.
/**
static void test2(void) {
  Context context;
  Context* ctx = turnOnWarnUnstable(&context);
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::cout << path.c_str() << std::endl;

  std::string contents =
    R""""(
    proc foo() {
      param x = 8;
      proc bar(param a = x) {
        type t = x.type;
        var n = a;
        proc baz(b: t): int { return n; }
        return baz();
      }
      var y = bar();
    }
    )"""";

  setFileText(ctx, path, contents);

  // Get the top module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod && mod->numStmts() == 1);

  // Get the 'foo' function and its declared variables.
  auto fooFn = mod->stmt(0)->toFunction();
  assert(fooFn && fooFn->numStmts() == 3);
  auto xVar = fooFn->stmt(0)->toVariable();
  auto yVar = fooFn->stmt(2)->toVariable();
  assert(xVar && xVar->storageKind() == Qualifier::PARAM);
  assert(yVar);

  // Get the 'bar' function, its formal, and its body.
  auto barFn = fooFn->stmt(1)->toFunction();
  assert(barFn && barFn->numStmts() == 4 && barFn->numFormals() == 1);
  auto aVar = barFn->formal(0)->toFormal();
  assert(aVar && aVar->storageKind() == Qualifier::PARAM);
  auto tVar = barFn->stmt(0)->toVariable();
  assert(tVar && tVar->storageKind() == Qualifier::TYPE);
  auto nVar = barFn->stmt(1)->toVariable();
  assert(nVar);
  auto barRetBaz = barFn->stmt(3)->toReturn();
  assert(barRetBaz);
  auto aUseX = aVar->initExpression()->toIdentifier();
  assert(aUseX);
  auto tUseX = tVar->initExpression()->toDot()->receiver()->toIdentifier();
  assert(tUseX);

  auto bazFn = barFn->stmt(2)->toFunction();
  assert(bazFn && bazFn->numFormals() == 1 && bazFn->numStmts() == 1);
  auto bVar = bazFn->formal(0)->toFormal();
  assert(bVar);
  auto bazRetN = bazFn->stmt(0)->toReturn();
  assert(bazRetN);
  auto bazUseN = bazRetN->value();
  assert(bazUseN);
  auto bUseT = bVar->typeExpression()->toIdentifier();
  assert(bUseT);

  // Get the resolution results for 'foo'.
  auto rfFoo = resolveConcreteFunction(ctx, fooFn->id());
  auto& yRe = rfFoo->byAst(yVar);
  auto& yQt = yRe.type();
  assert(yQt.type() == chpl::types::IntType::get(ctx, 0));
  assert(yQt.kind() == QualifiedType::VAR);

  // Get the resolution results for 'bar'.
  auto sigBar = nestedTypedSignatureInitial(ctx, barFn->id(),
                                            rfFoo->signature());
  assert(sigBar);
  assert(!sigBar->needsInstantiation());
  assert(sigBar->parentFn() == rfFoo->signature());
  auto rfBar = resolveFunction(ctx, sigBar, nullptr);

  auto& reAUseX = rfBar->byAst(aUseX);
  assert(reAUseX.type().type() == chpl::types::IntType::get(ctx, 0));
  assert(reAUseX.type().kind() == QualifiedType::PARAM);
  assert(reAUseX.toId() == xVar->id());

  auto& reTUseX = rfBar->byAst(tUseX);
  assert(reTUseX.type().type() == chpl::types::IntType::get(ctx, 0));
  assert(reTUseX.type().kind() == QualifiedType::TYPE);
  assert(reTUseX.toId() == xVar->id());

  // Get the resolution results for 'baz'.
  auto sigBaz = nestedTypedSignatureInitial(ctx, barFn->id(),
                                            rfBar->signature());
  assert(sigBaz);
  assert(!sigBaz->needsInstantiation());
  assert(sigBaz->parentFn() == rfBar->signature());
  auto rfBaz = resolveFunction(ctx, sigBaz, nullptr);
  auto& reBUseT = rfBaz->byAst(bUseT);
  assert(reBUseT.type().type() == chpl::types::IntType::get(ctx, 0));
  assert(reBUseT.type().kind() == QualifiedType::TYPE);
  assert(reBUseT.toId() == tVar->id());
  auto& reBazUseN = rfBaz->byAst(bazUseN);
  assert(reBazUseN.type().type() == chpl::types::IntType::get(ctx, 0));
  assert(reBazUseN.type().kind() == QualifiedType::REF);
  assert(reBazUseN.toId() == nVar->id());

  assert(!guard.realizeErrors());
}
**/

int main() {
  test0();
  test1();
  // test2();
  return 0;
}
