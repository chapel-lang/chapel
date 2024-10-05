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

#define TEST_NAME(ctx__) TEST_NAME_FROM_FN_NAME(ctx__)

static void testEmptyRecordUserInit() {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents =
    " record r {\n"
    "   proc init() {}\n"
    " }\n"
    " var obj = new r();\n";

  setFileText(ctx, path, contents);

  // Parse and get the module.
  const ModuleVec& vec = parseToplevel(ctx, path);
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
  const ResolutionResultByPostorderID& rr = resolveModule(ctx, m->id());

  // Get the type of 'r'.
  auto& qtR = typeForModuleLevelSymbol(ctx, r->id());

  // Remember that 'new r' is the base expression of the call.
  auto& reNewExpr = rr.byAst(newExpr);
  auto& qtNewExpr = reNewExpr.type();
  assert(qtNewExpr.kind() == QualifiedType::INIT_RECEIVER);
  assert(qtNewExpr.type() == qtR.type());

  // The 'new' call should have the same type as the 'new' expr.
  // However, the 'new' expr has the special INIT_RECEIVER intent,
  // so don't compare that.
  auto& reNewCall = rr.byAst(newCall);
  auto& qtNewCall = reNewCall.type();
  assert(qtNewExpr.type() == qtNewCall.type());
  assert(qtNewExpr.param() == qtNewCall.param());

  // The 'new' call should have 'init' as an associated function.
  auto& associatedActions = reNewCall.associatedActions();
  assert(associatedActions.size() == 1);
  auto initTfs = associatedActions[0].fn();
  assert(initTfs->id() == fnInit->id());
  assert(initTfs->numFormals() == 1);
  assert(initTfs->formalName(0) == "this");
  auto receiverQualType = initTfs->formalType(0);
  assert(receiverQualType.type() == qtR.type());
  assert(receiverQualType.kind() == QualifiedType::REF);
}

static void testEmptyRecordCompilerGenInit() {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents =
    " record r {\n"
    " }\n"
    " var obj = new r();\n";

  setFileText(ctx, path, contents);

  // Get the module.
  const ModuleVec& vec = parseToplevel(ctx, path);
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
  const ResolutionResultByPostorderID& rr = resolveModule(ctx, m->id());

  // Get the type of 'r'.
  auto& qtR = typeForModuleLevelSymbol(ctx, r->id());

  // Remember that 'new r' is the base expression of the call.
  auto& reNewExpr = rr.byAst(newExpr);
  auto& qtNewExpr = reNewExpr.type();
  assert(qtNewExpr.kind() == QualifiedType::INIT_RECEIVER);
  assert(qtNewExpr.type() == qtR.type());

  // The 'new' call should have the same type as the 'new' expr.
  auto& reNewCall = rr.byAst(newCall);
  auto& qtNewCall = reNewCall.type();
  assert(qtNewExpr.type() == qtNewCall.type());
  assert(qtNewExpr.param() == qtNewCall.param());

  // The 'new' call should have 'init' as an associated function.
  // This 'init' is compiler generated.
  auto& associatedActions = reNewCall.associatedActions();
  assert(associatedActions.size() == 1);
  auto initTfs = associatedActions[0].fn();
  assert(initTfs->untyped()->compilerGeneratedOrigin() == r->id());
  assert(initTfs->numFormals() == 1);
  assert(initTfs->formalName(0) == "this");

  auto receiverQualType = initTfs->formalType(0);
  assert(receiverQualType.type() == qtR.type());
  assert(receiverQualType.kind() == QualifiedType::REF);
}

static void testTertMethodCallCrossModule() {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
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

  setFileText(ctx, path, contents);

  // Get the modules.
  auto& br = parseAndReportErrors(ctx, path);
  assert(!guard.realizeErrors());
  assert(br.numTopLevelExpressions() == 2);
  auto modA = br.topLevelExpression(0)->toModule();
  assert(modA);
  auto modB = br.topLevelExpression(1)->toModule();
  assert(modB);

  // Get the class from module 'A'.
  assert(modA->numStmts() == 1);
  auto cls = modA->stmt(0)->toClass();
  assert(cls);

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

  auto& rr = resolveModule(ctx, modB->id());
  assert(!guard.realizeErrors());

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
  assert(reNewExpr.type().type() == reInitExpr.type().type());
  assert(reNewExpr.type().param() == reInitExpr.type().param());

  assert(reInitExpr.associatedActions().size() == 1);
  auto tfsInit = reInitExpr.associatedActions()[0].fn();
  assert(tfsInit);
  auto ufsInit = tfsInit->untyped();
  assert(ufsInit);
  assert(tfsInit->untyped()->compilerGeneratedOrigin().symbolPath() == "A.C");
  assert(ufsInit->isCompilerGenerated());
  assert(tfsInit->numFormals() == 1);
  assert(tfsInit->formalName(0) == "this");
  auto recvDecor = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
  auto recvType = ClassType::get(ctx, clsX->basicClassType(),
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
  assert(mscCall.fn()->id() == tert->id());
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

static void buildParseTestClassNewExpr(Context* ctx, const char* expr, int numErrors = 0) {
  ErrorGuard guard(ctx);

  static int testNumCounter = 0;
  std::string testName;
  testName += "buildParseTestClassNewExpr";
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
  auto& br = parseAndReportErrors(ctx, path);
  assert(guard.realizeErrors() == numErrors);

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

  assert(!guard.realizeErrors());
  bool ok = (expectedCls == actualCls);
  assert(ok);
  ctx->advanceToNextRevision(true);
}

// Test different combinations of management/nilability flavors.
static void testClassManagementNilabilityInNewExpr() {
  Context context;
  Context* ctx = &context;

  buildParseTestClassNewExpr(ctx, "new C()");
  buildParseTestClassNewExpr(ctx, "new owned C()");
  buildParseTestClassNewExpr(ctx, "new shared C()");
  buildParseTestClassNewExpr(ctx, "new borrowed C()", 1);
  buildParseTestClassNewExpr(ctx, "new unmanaged C()");
  buildParseTestClassNewExpr(ctx, "new C?()");
  buildParseTestClassNewExpr(ctx, "new owned C?()");
  buildParseTestClassNewExpr(ctx, "new shared C?()");
  buildParseTestClassNewExpr(ctx, "new borrowed C?()", 1);
  buildParseTestClassNewExpr(ctx, "new unmanaged C?()");
  buildParseTestClassNewExpr(ctx, "new C()?");
  buildParseTestClassNewExpr(ctx, "new owned C()?");
  buildParseTestClassNewExpr(ctx, "new shared C()?");
  buildParseTestClassNewExpr(ctx, "new borrowed C()?", 1);
  buildParseTestClassNewExpr(ctx, "new unmanaged C()?");
}

static void testGenericRecordUserInitDependentField() {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = R""""(
    operator =(ref lhs: int, rhs: int) {
      __primitive("=", lhs, rhs);
    }

    record r {
      type f1;
      param f2: int;
      var f3: f1;
      proc init(type f1, param P: int) {
        this.f1 = f1;
        this.f2 = P;
      }
    }
    var obj = new r(int, 8);
    )"""";

  setFileText(ctx, path, contents);

  // Get the module and the UAST we need.
  auto& br = parseAndReportErrors(ctx, path);
  assert(!guard.realizeErrors());

  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.singleModule();
  assert(mod);
  assert(mod->numStmts() == 3);
  auto rec = mod->stmt(1)->toRecord();
  assert(rec);
  auto obj = mod->stmt(2)->toVariable();
  assert(obj);

  // Resolve the module.
  auto& rr = resolveModule(ctx, mod->id());

  // Inspect the type of the variable 'obj'.
  auto& reObj = rr.byAst(obj);
  auto qtObj = reObj.type();
  assert(qtObj.kind() == QualifiedType::VAR);
  assert(qtObj.genericity() == Type::MAYBE_GENERIC); // TODO: CONCRETE
  auto typeObj = qtObj.type();
  auto rt = typeObj->toRecordType();
  assert(rt);
  assert(rt->id() == rec->id());
  assert(rt->instantiatedFrom() != nullptr);
  assert(rt->substitutions().size() == 2);

  // Check the first field of the instantiated record via substitutions.
  auto idf1 = parsing::fieldIdWithName(ctx, rt->id(),
                                       UniqueString::get(ctx, "f1"));
  assert(!idf1.isEmpty());
  auto qtf1 = rt->substitution(idf1);
  assert(qtf1.kind() == QualifiedType::TYPE);
  assert(qtf1.type() == IntType::get(ctx, 0));
  assert(qtf1.param() == nullptr);

  // Check the second field of the instantiated record via substitutions.
  auto idf2 = parsing::fieldIdWithName(ctx, rt->id(),
                                       UniqueString::get(ctx, "f2"));
  assert(!idf2.isEmpty());
  auto qtf2 = rt->substitution(idf2);
  assert(qtf2.kind() == QualifiedType::PARAM);
  assert(qtf2.type() == IntType::get(ctx, 0));
  assert(qtf2.param()->isIntParam());
  assert(qtf2.param()->toIntParam()->value() == 8);

  // Now check all fields via the resolved fields query.
  auto& rf = fieldsForTypeDecl(ctx, rt, DefaultsPolicy::USE_DEFAULTS);
  assert(rf.numFields() == 3);
  assert(!rf.isGeneric());
  assert(!rf.isGenericWithDefaults());

  // First field is 'type int(64)'
  auto ft1 = QualifiedType(QualifiedType::TYPE, IntType::get(ctx, 0));
  assert(rf.fieldType(0) == ft1);

  // Second is 'param int = 8'
  auto ft2 = QualifiedType(QualifiedType::PARAM, IntType::get(ctx, 0),
                           IntParam::get(ctx, 8));
  assert(rf.fieldType(1) == ft2);

  // Last is 'var int'
  auto ft3 = QualifiedType(QualifiedType::VAR, IntType::get(ctx, 0));
  assert(rf.fieldType(2) == ft3);

  // Confirm that the TFS for the initializer is correct.
  auto newCall = obj->initExpression();
  auto reNewCall = rr.byAst(newCall);
  assert(reNewCall.type() == reObj.type());
  assert(reNewCall.associatedActions().size() == 1);
  auto initTfs = reNewCall.associatedActions()[0].fn();
  assert(initTfs);
  assert(initTfs->untyped()->name() == "init");
  assert(initTfs->untyped()->numFormals() == 3);
  assert(initTfs->untyped()->id() == rec->declOrComment(3)->id());

  // Initializer type should be the same as the 'new' call type.
  auto qtRecv = initTfs->formalType(0);
  assert(qtRecv.type() == typeObj);

  // But the kind should be 'REF'.
  assert(qtRecv.kind() == QualifiedType::REF);
}

static void testRecordNewSegfault(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME(ctx);
  std::string contents = R""""(
    class C {}
    var r1 = new owned C();
    var r2 = new shared C();
    var r3 = new unmanaged C();
    )"""";

  setFileText(ctx, path, contents);

  // Get the module.
  auto& br = parseAndReportErrors(ctx, path);
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  assert(mod);

  // Resolve the module.
  std::ignore = resolveModule(ctx, mod->id());
  assert(!guard.realizeErrors());
}

static void testGenericRecordUserSecondaryInitDependentField() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto qt = resolveTypeOfXInit(context,
    R"""(
    proc defaultValueFor(type t: int) param do return 42;

    record r {
      type f1;
      type f2 = (f1, f1);
      param f3 = defaultValueFor(f1);
    }

    proc r.init(type f1) {
      this.f1 = f1;
    }

    var x = new r(int);
    )""");

  auto ct = qt.type()->toCompositeType();
  assert(ct);
  assert(ct->name() == "r");

  // It should already be instantiated, no need to use defaults.
  auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
  assert(fields.numFields() == 3);

  auto f1 = fields.fieldType(0);
  assert(f1.isType());
  assert(f1.type()->isIntType());
  assert(f1.type()->toIntType()->isDefaultWidth());

  auto f2 = fields.fieldType(1);
  assert(f2.isType());
  assert(f2.type()->isTupleType());
  assert(f2.type()->toTupleType()->numElements() == 2);
  assert(f2.type()->toTupleType()->elementType(0).type() == f1.type());
  assert(f2.type()->toTupleType()->elementType(1).type() == f1.type());

  auto f3 = fields.fieldType(2);
  assert(f3.isParam());
  assert(f3.type() == f1.type());
  assert(f3.param());
  assert(f3.param()->isIntParam());
  assert(f3.param()->toIntParam()->value() == 42);
}

static void testNewGenericWithDefaults() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
    R"""(
    record r {
      type f1 = int;
    }

    proc r.init(type f1) {
      this.f1 = f1;
    }

    var x1 = new r(int);
    var x2 = new r(bool);
    )""", { "x1", "x2" });


  {
    auto ct = vars.at("x1").type()->toCompositeType();
    assert(ct);
    assert(ct->name() == "r");

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 1);

    auto f1 = fields.fieldType(0);
    assert(f1.isType());
    assert(f1.type()->isIntType());
    assert(f1.type()->toIntType()->isDefaultWidth());
  }

  {
    auto ct = vars.at("x2").type()->toCompositeType();
    assert(ct);
    assert(ct->name() == "r");

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 1);

    auto f1 = fields.fieldType(0);
    assert(f1.isType());
    assert(f1.type()->isBoolType());
  }
}

static void testCompilerGeneratedGenericNewWithDefaultInit() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
    R"""(
    record r {
      param flag : bool;
      var x : if flag then int else real;
    }

    var x1 = new r(true);
    var x2 = new r(false);
    )""", { "x1", "x2" });


  {
    auto ct = vars.at("x1").type()->toCompositeType();
    assert(ct);
    assert(ct->name() == "r");

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 2);

    auto f1 = fields.fieldType(0);
    assert(f1.isParamTrue());
    auto f2 = fields.fieldType(1);
    assert(f2.kind() == QualifiedType::VAR);
    assert(f2.type());
    assert(f2.type()->isIntType());
    assert(f2.type()->toIntType()->isDefaultWidth());
  }

  {
    auto ct = vars.at("x2").type()->toCompositeType();
    assert(ct);
    assert(ct->name() == "r");

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 2);

    auto f1 = fields.fieldType(0);
    assert(f1.isParamFalse());
    auto f2 = fields.fieldType(1);
    assert(f2.kind() == QualifiedType::VAR);
    assert(f2.type());
    assert(f2.type()->isRealType());
    assert(f2.type()->toRealType()->isDefaultWidth());
  }
}

static void testCompilerGeneratedGenericNew() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
    R"""(
    record r {
      param flag : bool;
      var x : if flag then int else real;
    }

    var x1 = new r(true, 1);
    var x2 = new r(false, 1.0);
    var x3 = new r(true, 1.0);
    var x4 = new r(false, 1);
    )""", { "x1", "x2", "x3", "x4" });


  {
    auto ct = vars.at("x1").type()->toCompositeType();
    assert(ct);
    assert(ct->name() == "r");

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 2);

    auto f1 = fields.fieldType(0);
    assert(f1.isParamTrue());
    auto f2 = fields.fieldType(1);
    assert(f2.kind() == QualifiedType::VAR);
    assert(f2.type());
    assert(f2.type()->isIntType());
    assert(f2.type()->toIntType()->isDefaultWidth());
  }

  {
    auto ct = vars.at("x2").type()->toCompositeType();
    assert(ct);
    assert(ct->name() == "r");

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 2);

    auto f1 = fields.fieldType(0);
    assert(f1.isParamFalse());
    auto f2 = fields.fieldType(1);
    assert(f2.kind() == QualifiedType::VAR);
    assert(f2.type());
    assert(f2.type()->isRealType());
    assert(f2.type()->toRealType()->isDefaultWidth());
  }

  {
    auto qt = vars.at("x3");
    assert(qt.isUnknown());
  }

  {
    auto ct = vars.at("x4").type()->toCompositeType();
    assert(ct);
    assert(ct->name() == "r");

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 2);

    auto f1 = fields.fieldType(0);
    assert(f1.isParamFalse());
    auto f2 = fields.fieldType(1);
    assert(f2.kind() == QualifiedType::VAR);
    assert(f2.type());
    assert(f2.type()->isRealType());
    assert(f2.type()->toRealType()->isDefaultWidth());
  }

  assert(guard.realizeErrors() == 1);
}

static void testCompilerGeneratedGenericNewWithDefaultInitClass() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
    R"""(
    class C {
      param flag : bool;
      var x : if flag then int else real;
    }

    var x1 = new C(true);
    var x2 = new C(false);
    )""", { "x1", "x2" });


  {
    auto clt = vars.at("x1").type()->toClassType();
    assert(clt);
    auto ct = clt->basicClassType();
    assert(ct);
    assert(ct->name() == "C");

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 2);

    auto f1 = fields.fieldType(0);
    assert(f1.isParamTrue());
    auto f2 = fields.fieldType(1);
    assert(f2.kind() == QualifiedType::VAR);
    assert(f2.type());
    assert(f2.type()->isIntType());
    assert(f2.type()->toIntType()->isDefaultWidth());
  }

  {
    auto clt = vars.at("x2").type()->toClassType();
    assert(clt);
    auto ct = clt->basicClassType();
    assert(ct);
    assert(ct->name() == "C");

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 2);

    auto f1 = fields.fieldType(0);
    assert(f1.isParamFalse());
    auto f2 = fields.fieldType(1);
    assert(f2.kind() == QualifiedType::VAR);
    assert(f2.type());
    assert(f2.type()->isRealType());
    assert(f2.type()->toRealType()->isDefaultWidth());
  }
}

static void testCompilerGeneratedGenericNewClass() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
    R"""(
    class C {
      param flag : bool;
      var x : if flag then int else real;
    }

    var x1 = new C(true, 1);
    var x2 = new C(false, 1.0);
    var x3 = new C(true, 1.0);
    var x4 = new C(false, 1);
    )""", { "x1", "x2", "x3", "x4" });


  {
    auto clt = vars.at("x1").type()->toClassType();
    assert(clt);
    auto ct = clt->basicClassType();
    assert(ct);

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 2);

    auto f1 = fields.fieldType(0);
    assert(f1.isParamTrue());
    auto f2 = fields.fieldType(1);
    assert(f2.kind() == QualifiedType::VAR);
    assert(f2.type());
    assert(f2.type()->isIntType());
    assert(f2.type()->toIntType()->isDefaultWidth());
  }

  {
    auto clt = vars.at("x2").type()->toClassType();
    assert(clt);
    auto ct = clt->basicClassType();
    assert(ct);

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 2);

    auto f1 = fields.fieldType(0);
    assert(f1.isParamFalse());
    auto f2 = fields.fieldType(1);
    assert(f2.kind() == QualifiedType::VAR);
    assert(f2.type());
    assert(f2.type()->isRealType());
    assert(f2.type()->toRealType()->isDefaultWidth());
  }

  {
    auto qt = vars.at("x3");
    assert(qt.isUnknown());
  }

  {
    auto clt = vars.at("x4").type()->toClassType();
    assert(clt);
    auto ct = clt->basicClassType();
    assert(ct);

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 2);

    auto f1 = fields.fieldType(0);
    assert(f1.isParamFalse());
    auto f2 = fields.fieldType(1);
    assert(f2.kind() == QualifiedType::VAR);
    assert(f2.type());
    assert(f2.type()->isRealType());
    assert(f2.type()->toRealType()->isDefaultWidth());
  }

  assert(guard.realizeErrors() == 1);
}

static void testSimpleUserGenericNew() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
    R"""(
    // Need an operator= for non-compile-time values to be assigned.
    operator =(ref lhs: numeric, rhs: numeric) {
      __primitive("=", lhs, rhs);
    }

    class C {
      var x;

      proc init(value) {
        this.x = value;
      }
    }

    var x1 = new C(42);
    var x2 = new C(1.0);
    )""", { "x1", "x2" });


  {
    auto ct = vars.at("x1").type()->getCompositeType();
    assert(ct);
    assert(ct->name() == "C");

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 1);

    auto f1 = fields.fieldType(0);
    assert(f1.kind() == QualifiedType::VAR);
    assert(f1.type());
    assert(f1.type()->isIntType());
    assert(f1.type()->toIntType()->isDefaultWidth());
  }

  {
    auto ct = vars.at("x2").type()->getCompositeType();
    assert(ct);
    assert(ct->name() == "C");

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 1);

    auto f1 = fields.fieldType(0);
    assert(f1.kind() == QualifiedType::VAR);
    assert(f1.type());
    assert(f1.type()->isRealType());
    assert(f1.type()->toRealType()->isDefaultWidth());
  }
}

static void testUserGenericNew() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
    R"""(
    // Need an operator= for non-compile-time values to be assigned.
    operator =(ref lhs: numeric, rhs: numeric) {
      __primitive("=", lhs, rhs);
    }

    record r {
      param flag : bool;
      var x : if flag then int else real;

      proc init(param flag: bool, x: if flag then int else real) {
        this.flag = flag;
        this.x = x;
      }
    }

    var x1 = new r(true, 1);
    var x2 = new r(false, 1.0);
    var x3 = new r(true, 1.0);
    var x4 = new r(false, 1);
    )""", { "x1", "x2", "x3", "x4" });


  {
    auto ct = vars.at("x1").type()->toCompositeType();
    assert(ct);
    assert(ct->name() == "r");

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 2);

    auto f1 = fields.fieldType(0);
    assert(f1.isParamTrue());
    auto f2 = fields.fieldType(1);
    assert(f2.kind() == QualifiedType::VAR);
    assert(f2.type());
    assert(f2.type()->isIntType());
    assert(f2.type()->toIntType()->isDefaultWidth());
  }

  {
    auto ct = vars.at("x2").type()->toCompositeType();
    assert(ct);
    assert(ct->name() == "r");

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 2);

    auto f1 = fields.fieldType(0);
    assert(f1.isParamFalse());
    auto f2 = fields.fieldType(1);
    assert(f2.kind() == QualifiedType::VAR);
    assert(f2.type());
    assert(f2.type()->isRealType());
    assert(f2.type()->toRealType()->isDefaultWidth());
  }

  {
    auto qt = vars.at("x3");
    assert(qt.isUnknown());
  }

  {
    auto ct = vars.at("x4").type()->toCompositeType();
    assert(ct);
    assert(ct->name() == "r");

    // It should already be instantiated, no need to use defaults.
    auto fields = fieldsForTypeDecl(context, ct, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 2);

    auto f1 = fields.fieldType(0);
    assert(f1.isParamFalse());
    auto f2 = fields.fieldType(1);
    assert(f2.kind() == QualifiedType::VAR);
    assert(f2.type());
    assert(f2.type()->isRealType());
    assert(f2.type()->toRealType()->isDefaultWidth());
  }

  assert(guard.realizeErrors() == 1);
}


int main() {
  testEmptyRecordUserInit();
  testEmptyRecordCompilerGenInit();
  testTertMethodCallCrossModule();
  testClassManagementNilabilityInNewExpr();
  testGenericRecordUserInitDependentField();
  testRecordNewSegfault();
  testGenericRecordUserSecondaryInitDependentField();
  testNewGenericWithDefaults();
  testCompilerGeneratedGenericNewWithDefaultInit();
  testCompilerGeneratedGenericNew();
  testCompilerGeneratedGenericNewWithDefaultInitClass();
  testCompilerGeneratedGenericNewClass();
  testSimpleUserGenericNew();
  testUserGenericNew();

  return 0;
}

