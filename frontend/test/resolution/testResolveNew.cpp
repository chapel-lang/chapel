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

#include "test-resolution.h"

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

class CountingErrorHandler {
  Context::ReportErrorFnType oldHandler_ = nullptr;
  static std::vector<const ErrorBase*> errors_;
  Context* ctx_;

  // TODO: Can we just have the error handler be an instance of a class?
  static void globalReportError(Context* ctx, const ErrorBase* err) {
    errors_.push_back(err);
  }

 public:
  CountingErrorHandler(Context* ctx) : ctx_(ctx) {
    oldHandler_ = ctx_->errorHandler();
    ctx_->setErrorHandler(&globalReportError);
  }

  bool realizeErrors() {
    if (errors_.size() != 0) {
      for (auto err : errors_) oldHandler_(ctx_, err);
      errors_.clear();
      return true;
    }
    return false;
  }

  ~CountingErrorHandler() {
    assert(!this->realizeErrors());
    ctx_->setErrorHandler(oldHandler_);
  }
};

// C++ is so dumb...
std::vector<const ErrorBase*> CountingErrorHandler::errors_;

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
  const ModuleVec& vec = parseToplevel(context, path);
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
  const ModuleVec& vec = parseToplevel(context, path);
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

  assert(receiverQualType.kind() == QualifiedType::REF);

  context->collectGarbage();
}

// Resolve tertiary method in another module. Initialize object using 'new'.
static void test3() {
  Context ctx;
  Context* context = &ctx;

  context->advanceToNextRevision(true);
  auto errHandler = CountingErrorHandler(context);

  auto path = UniqueString::get(context, "test3.chpl");
  std::string contents =
    R""""(
    module A {
      class C {}
    }
    module B {
      use A;
      proc C.foo() {}
      var x = new C();
      x.foo();
    }
    )"""";
  setFileText(context, path, contents);

  // Get the modules.
  auto& br = parseFileToBuilderResult(context, path, UniqueString());
  assert(!br.numErrors());
  assert(br.numTopLevelExpressions() == 2);
  auto modA = br.topLevelExpression(0)->toModule();
  assert(modA);
  auto modB = br.topLevelExpression(1)->toModule();
  assert(modB);

  // Get the class from module 'A'.
  assert(modA->numStmts() == 1);
  auto cls = modA->stmt(0)->toClass();
  assert(cls);

  /*
      use A;
      proc C.foo() {}
      var x = new C();
      x.foo();
  */

  // Get the tertiary method, variable, and call from module 'B'.
  assert(modB->numStmts() == 4);
  auto tert = modB->stmt(1)->toFunction();
  assert(tert);
  auto x = modB->stmt(2)->toVariable();
  assert(x && x->initExpression());
  auto initExpr = x->initExpression()->toFnCall();
  assert(initExpr && initExpr->calledExpression());
  auto newExpr = initExpr->calledExpression()->toNew();
  assert(newExpr);
  auto call = modB->stmt(3)->toFnCall();
  assert(call);

  auto& rr = resolveModule(context, modB->id());
  assert(!errHandler.realizeErrors());

  // TODO: Get the TFS for the tertiary method...

  auto& reX = rr.byAst(x);
  assert(reX.type().kind() == QualifiedType::VAR);
  assert(!reX.type().isUnknown());
  assert(!reX.type().isErroneousType());
  // TODO: Way to forcibly set this to concrete? Note that we would have
  // to store a field in QT, because we cannot know for certain unless
  // we inspect the class fields (feels weird).
  assert(reX.type().genericity() == Type::Genericity::MAYBE_GENERIC);
  assert(reX.type().kind() == QualifiedType::VAR);
  auto typeX = reX.type().type();
  assert(typeX);
  auto clsX = typeX->toClassType();
  assert(clsX->basicClassType()->id() == cls->id());
  assert(clsX->decorator().isNonNilable());
  assert(clsX->decorator().isManaged());
  assert(clsX->manager());
  assert(clsX->manager()->isAnyOwnedType());

  // TODO: When is this not transitive?
  auto& reInitExpr = rr.byAst(initExpr);
  assert(reInitExpr.type() == reX.type());
  auto& reNewExpr = rr.byAst(newExpr);
  assert(reNewExpr.type() == reInitExpr.type());

  assert(reInitExpr.associatedFns().size() == 1);
  auto tfsInit = reInitExpr.associatedFns()[0];
  assert(tfsInit);
  auto ufsInit = tfsInit->untyped();
  assert(ufsInit);
  assert(!tfsInit->id().isEmpty());
  assert(tfsInit->id().symbolPath() == "A.C");
  assert(ufsInit->isCompilerGenerated());
  assert(tfsInit->numFormals() == 1);
  assert(tfsInit->formalName(0) == "this");
  auto recvDecor = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
  auto recvType = ClassType::get(context, clsX->basicClassType(),
                                 nullptr,
                                 recvDecor);
  auto qtReceiver = QualifiedType(QualifiedType::CONST_IN, recvType);
  assert(tfsInit->formalType(0) == qtReceiver);
  assert(!tfsInit->needsInstantiation());
  assert(!tfsInit->formalIsInstantiated(0)); // Is concrete!

  auto& reCall = rr.byAst(call);

  // Last part just confirms the call lines up.
  auto mscCall = reCall.mostSpecific().only();
  assert(mscCall);
  assert(mscCall->id() == tert->id());

  context->collectGarbage();
}

static void determineManagerAndDecorator(Context* ctx,
                                         New::Management management,
                                         ClassTypeDecorator& outDecor,
                                         const Type*& outManager) {
  auto decorTag = ClassTypeDecorator::BORROWED;
  const Type* manager = nullptr;

  switch (management) {
    case New::DEFAULT_MANAGEMENT:
    case New::OWNED:
      decorTag = ClassTypeDecorator::MANAGED;
      manager = AnyOwnedType::get(ctx);
      break;
    case New::SHARED:
      decorTag = ClassTypeDecorator::MANAGED;
      manager = AnySharedType::get(ctx);
      break;
    case New::BORROWED:
      decorTag = ClassTypeDecorator::BORROWED;
      break;
    case New::UNMANAGED:
      decorTag = ClassTypeDecorator::UNMANAGED;
      break;
    default: break;
  }

  outDecor = ClassTypeDecorator(decorTag);
  outManager = manager;
  return;
}

static bool testNewExpression(Context* ctx, const char* expr) {
  static int testNumCounter = 0;
  std::string testName;
  testName += "testNewExpr";
  testName += std::to_string(testNumCounter++);
  testName += ".chpl";
  auto path = UniqueString::get(ctx, testName.c_str());
  std::string contents =
    R""""(
    module A {
      class C {}
      var x = )"""";
  contents += expr;
  contents += ";\n}\n";

  setFileText(ctx, path, contents);

  // Need to be known now to unpack the new expression.
  bool isNilableInNewExpr = strstr(expr, "?(");
  bool isPostfixNilable = strstr(expr, ")?");
  bool isNilable = (isNilableInNewExpr || isPostfixNilable);

  // Get the module.
  auto& br = parseFile(ctx, path);
  if (br.numErrors()) return false;

  assert(br.numTopLevelExpressions() == 1);
  auto modA = br.topLevelExpression(0)->toModule();
  assert(modA);

  // Get the class.
  assert(modA->numStmts() == 2);
  auto cls = modA->stmt(0)->toClass();
  assert(cls);

  // Get the call containing the new expression.
  auto x = modA->stmt(1)->toVariable();
  assert(x && x->initExpression());

  const Call* call = nullptr;
  if (isPostfixNilable) {
    auto opCall = x->initExpression()->toOpCall();
    assert(opCall && opCall->numActuals() == 1 && opCall->op() == "?");
    call = opCall->actual(0)->toCall();
  } else {
    call = x->initExpression()->toCall();
  }

  assert(call);
  assert(call->calledExpression() && call->calledExpression()->isNew());

  // Gather the pieces needed to construct the class type.
  auto management = call->calledExpression()->toNew()->management();
  ClassTypeDecorator decor(ClassTypeDecorator::BORROWED);
  const Type* manager = nullptr;
  determineManagerAndDecorator(ctx, management, decor, manager);
  auto initial = initialTypeForTypeDecl(ctx, cls->id());
  auto initialClass = initial->toClassType();
  assert(initialClass);
  auto basic = initialClass->basicClassType();
  assert(basic);

  // Set the nilability on the decorator.
  decor = isNilable ? decor.addNilable() : decor.addNonNil();

  // Construct the class type we are expecting.
  const Type* expectedCls = ClassType::get(ctx, basic, manager, decor);
  assert(expectedCls);

  auto& rr = resolveModule(ctx, modA->id());

  // Confirm that the expected type matches the variable type.
  auto& reX = rr.byAst(x);
  const Type* actualCls = reX.type().type();
  assert(actualCls);

  bool ret = (expectedCls == actualCls);
  return ret;
}

static void doTestNewExpression(Context* ctx, const char* expr) {
  auto errHandler = CountingErrorHandler(ctx);
  bool passed = testNewExpression(ctx, expr);
  assert(!errHandler.realizeErrors());
  assert(passed);
  ctx->advanceToNextRevision(true);
}

// Test different combinations of management/nilability flavors.
static void test4() {
  Context context;
  Context* ctx = &context;

  doTestNewExpression(ctx, "new C()");
  doTestNewExpression(ctx, "new owned C()");
  doTestNewExpression(ctx, "new shared C()");
  doTestNewExpression(ctx, "new borrowed C()");
  doTestNewExpression(ctx, "new unmanaged C()");
  doTestNewExpression(ctx, "new C?()");
  doTestNewExpression(ctx, "new owned C?()");
  doTestNewExpression(ctx, "new shared C?()");
  doTestNewExpression(ctx, "new borrowed C?()");
  doTestNewExpression(ctx, "new unmanaged C?()");
  doTestNewExpression(ctx, "new C()?");
  doTestNewExpression(ctx, "new owned C()?");
  doTestNewExpression(ctx, "new shared C()?");
  doTestNewExpression(ctx, "new borrowed C()?");
  doTestNewExpression(ctx, "new unmanaged C()?");
}

// TODO: Begin testing some generic records (without user initializers).
static void test5() {

}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();

  return 0;
}

