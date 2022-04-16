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
#include "chpl/uast/all-uast.h"

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

static void test1() {
  Context ctx;
  Context* context = &ctx;

  context->advanceToNextRevision(true);

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents =
    " record r {\n"
    "   proc init() {}\n"
    " }\n"
    " var obj = new r();\n";

  setFileText(context, path, contents);

  // Get the module.
  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);

  // Unpack all the uAST we need for the test.
  assert(m->numStmts() == 2);
  auto r = m->stmt(0)->toRecord();
  assert(r);
  assert(r->numDeclOrComments() == 1);
  auto fnInit = r->declOrComment(0)->toFunction();
  assert(fnInit);
  auto obj = m->stmt(1)->toVariable();
  assert(obj && !obj->typeExpression() && obj->initExpression());
  auto newCall = obj->initExpression()->toFnCall();
  assert(newCall);
  auto newExpr = newCall->calledExpression()->toNew();
  assert(newExpr);
  assert(newExpr->management() == New::DEFAULT_MANAGEMENT);

  // Resolve the module.
  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  (void) rr;

  // Get the type of 'r'.
  auto& qtR = typeForModuleLevelSymbol(context, r->id());
  (void) qtR;

  // Remember that 'new r' is the base expression of the call.
  auto& reNewExpr = rr.byAst(newExpr);
  auto& qtNewExpr = reNewExpr.type();
  assert(qtNewExpr.kind() == QualifiedType::VAR);
  assert(qtNewExpr.type() == qtR.type());

  // The 'new' call should have the same type as the 'new' expr.
  auto& reNewCall = rr.byAst(newCall);
  auto& qtNewCall = reNewCall.type();
  assert(qtNewExpr == qtNewCall);

  // The 'new' call should have 'init' as an associated function.
  auto& associatedFns = reNewCall.associatedFns();
  assert(associatedFns.size() == 1);
  auto initTfs = associatedFns[0];
  assert(initTfs->id() == fnInit->id());
  assert(initTfs->numFormals() == 1);
  assert(initTfs->formalName(0) == "this");
  auto receiverQualType = initTfs->formalType(0);
  assert(receiverQualType.type() == qtR.type());

  // TODO: should dyno mark this 'init' as 'REF'? Seems deceptive since
  // an initializer is always going to mutate state.
  assert(receiverQualType.kind() == QualifiedType::CONST_REF);

  context->collectGarbage();
}

static void test2() {
  Context ctx;
  Context* context = &ctx;

  context->advanceToNextRevision(true);

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents =
    " record r {\n"
    " }\n"
    " var obj = new r();\n";

  setFileText(context, path, contents);

  // Get the module.
  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);

  // Unpack all the uAST we need for the test.
  assert(m->numStmts() == 2);
  auto r = m->stmt(0)->toRecord();
  assert(r);
  auto obj = m->stmt(1)->toVariable();
  assert(obj && !obj->typeExpression() && obj->initExpression());
  auto newCall = obj->initExpression()->toFnCall();
  assert(newCall);
  auto newExpr = newCall->calledExpression()->toNew();
  assert(newExpr);
  assert(newExpr->management() == New::DEFAULT_MANAGEMENT);

  // Resolve the module.
  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  (void) rr;

  // Get the type of 'r'.
  auto& qtR = typeForModuleLevelSymbol(context, r->id());
  (void) qtR;

  // Remember that 'new r' is the base expression of the call.
  auto& reNewExpr = rr.byAst(newExpr);
  auto& qtNewExpr = reNewExpr.type();
  assert(qtNewExpr.kind() == QualifiedType::VAR);
  assert(qtNewExpr.type() == qtR.type());

  // The 'new' call should have the same type as the 'new' expr.
  auto& reNewCall = rr.byAst(newCall);
  auto& qtNewCall = reNewCall.type();
  assert(qtNewExpr == qtNewCall);

  // The 'new' call should have 'init' as an associated function.
  // This 'init' is compiler generated.
  auto& associatedFns = reNewCall.associatedFns();
  assert(associatedFns.size() == 1);
  auto initTfs = associatedFns[0];
  assert(initTfs->id() == r->id());
  assert(initTfs->numFormals() == 1);
  assert(initTfs->formalName(0) == "this");
  auto receiverQualType = initTfs->formalType(0);
  assert(receiverQualType.type() == qtR.type());

  // TODO: should dyno mark this 'init' as 'REF'? Seems deceptive since
  // an initializer is always going to mutate state.
  assert(receiverQualType.kind() == QualifiedType::REF);

  context->collectGarbage();
}

int main() {
  test1();
  test2();

  return 0;
}

