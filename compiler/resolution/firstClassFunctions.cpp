/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "astutil.h"
#include "buildDefaultFunctions.h"
#include "DecoratedClassType.h"
#include "driver.h"
#include "passes.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "resolveIntents.h"
#include "scopeResolve.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "typeSpecifier.h"
#include "version.h"
#include "visibleFunctions.h"
#include "wellknown.h"

#include "global-ast-vecs.h"

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>
#include <algorithm>
#include <iostream>
#include <sstream>

namespace {
  struct FcfFormalInfo {
    Type* type;
    IntentTag concreteIntent;
    const char* name;
  };

  struct FcfSuperInfo {
    std::vector<FcfFormalInfo> formals;
    RetTag retTag;
    Type* retType;
    bool isAnyFormalNamed;
    AggregateType* type;
    Type* sharedType;
    FnSymbol* thisMethod;
    const char* userTypeString;
  };
}

static const char* superTypePrefix = "chpl_fcf_";
static std::map<AggregateType*, Type*> superToSharedSuper;
static std::map<const char*, FcfSuperInfo> superNameToInfo;
static std::map<Type*, const char*> wrapperTypeToName;
static std::map<FnSymbol*, FnSymbol*> payloadToSharedChildFactory;
static VarSymbol* dummyFcfError = nullptr;
static std::map<FnSymbol*, bool> payloadToResolved;
static int uniqueFcfId = 0;

/************************************* | *************************************
*                                                                            *
*                                                                            *
*                                                                            *
************************************** | ************************************/

static bool isConcreteIntentBlank(IntentTag tag, Type* t);

static VarSymbol* getDummyFcfErrorInsertedAtProgram(CallExpr* call);

static Type* buildSharedWrapperType(AggregateType* parent);

static bool checkAndResolveFunctionSignature(FnSymbol* fn, Expr* use);

static bool checkAndResolveFunction(FnSymbol* fn, Expr* use);

FnSymbol* findFunctionFromNameMaybeError(UnresolvedSymExpr* usym,
                                         CallExpr* root);

static const FcfSuperInfo&
buildWrapperSuperTypeAtProgram(FnSymbol* fn);

static const char* intentToString(IntentTag tag);

static const char* typeToStringSpecializing(Type* t);

static const char*
buildUserFacingTypeString(const std::vector<FcfFormalInfo>& formals,
                          RetTag retTag,
                          Type* retType,
                          bool throws);

static const FcfSuperInfo&
buildWrapperSuperTypeAtProgram(const std::vector<FcfFormalInfo>& formals,
                               RetTag retTag,
                               Type* retType,
                               bool throws);

static const char* intentTagMnemonicMangled(IntentTag tag);

static const char* retTagMnemonicMangled(RetTag tag);

static const char*
buildSuperName(const std::vector<FcfFormalInfo>& formals,
               RetTag retTag,
               Type* retType,
               bool throws);

static AggregateType*
insertFcfWrapperSuperTypeAtProgram(const char* name);

static void
attachOldStyleRetTypeGetter(AggregateType* parent, Type* retType);

static void
attachOldStyleArgTypeGetter(AggregateType* parent,
                            const std::vector<FcfFormalInfo>& formals);

static FnSymbol*
attachSuperThis(AggregateType* super,
                const std::vector<FcfFormalInfo>& formals,
                RetTag retTag,
                Type* retType,
                bool throws);

/************************************* | *************************************
*                                                                            *
*                                                                            *
*                                                                            *
************************************** | ************************************/

static bool isConcreteIntentBlank(IntentTag tag, Type* t) {
  auto ret = concreteIntent(INTENT_BLANK, t) == concreteIntent(tag, t);
  return ret;
}

static VarSymbol* getDummyFcfErrorInsertedAtProgram(CallExpr* call) {
  if (dummyFcfError != nullptr) return dummyFcfError;
  auto parent = insertFcfWrapperSuperTypeAtProgram("_fcf_error");
  dummyFcfError = newTemp(parent);
  theProgram->block->body.insertAtTail(new DefExpr(dummyFcfError));
  return dummyFcfError;
}

static Type* buildSharedWrapperType(AggregateType* super) {
  Type* ret = NULL;
  CallExpr* superShared = new CallExpr(dtShared->symbol, super->symbol);
  chpl_gen_main->insertAtHead(superShared);
  tryResolveCall(superShared);
  superShared->remove();
  ret = superShared->typeInfo();
  INT_ASSERT(ret);
  ret->symbol->addFlag(FLAG_FUNCTION_CLASS);
  return ret;
}

static bool checkAndResolveFunctionSignature(FnSymbol* fn, Expr* use) {
  resolveSignature(fn);

  if (fn->retType == dtUnknown) {
    INT_ASSERT(fn->retExprType);
    resolveSpecifiedReturnType(fn);
  }

  bool isInvalidSignature = false;
  for_formals(formal, fn) {
    isInvalidSignature |= formal->type->symbol->hasFlag(FLAG_GENERIC);
    if (isInvalidSignature) break;
  }

  if (isInvalidSignature) {
    USR_FATAL_CONT(use, "'%s' cannot be captured as a value because "
                        "it is a generic function",
                        fn->name);
    return false;
  }

  return true;
}

static bool checkAndResolveFunction(FnSymbol* fn, Expr* use) {
  if (payloadToResolved.find(fn) != payloadToResolved.end()) {
    return payloadToResolved[fn];
  }

  bool status = false;
  if (checkAndResolveFunctionSignature(fn, use)) {
    // TODO: How to see if this call emitted errors or not?
    if (!fn->isResolved())
      if (!fn->hasFlag(FLAG_NO_FN_BODY))
        resolveFunction(fn);
    status = true;
  }

  payloadToResolved.insert(std::make_pair(fn, status));

  return true;
}

static FnSymbol* findFunctionFromNameMaybeError(UnresolvedSymExpr* usym,
                                                CallExpr* root) {
  const char* name = usym->unresolved;
  Vec<FnSymbol*> visibleFns;

  // Recompute manually because this call may be in an instantiation.
  recomputeVisibleFunctions();
  getVisibleFunctions(name, root, visibleFns);

  if (visibleFns.n > 1) {
    USR_FATAL_CONT(usym, "cannot capture overloaded function '%s'", name);
    return nullptr;
  }

  // TODO: Lambdas should always be visible here.
  if (visibleFns.n == 0) {
    USR_FATAL_CONT(usym, "no function with name '%s' is visible", name);
    return nullptr;
  }

  FnSymbol* ret = visibleFns.head();

  return ret;
}

Expr* fcfWrapperInstanceFromPrimCall(CallExpr *call) {
  INT_ASSERT(call && call->isPrimitive(PRIM_CAPTURE_FN_FOR_CHPL));

  auto use = toUnresolvedSymExpr(call->get(1));
  auto capturedFn = findFunctionFromNameMaybeError(use, call);

  if (!capturedFn || !checkAndResolveFunction(capturedFn, use)) {
    auto dummy = getDummyFcfErrorInsertedAtProgram(call);
    return new SymExpr(dummy);
  }

  // Reuse cached FCF wrapper if it already exists.
  if (payloadToSharedChildFactory.find(capturedFn) !=
      payloadToSharedChildFactory.end()) {
    auto ret = new CallExpr(payloadToSharedChildFactory[capturedFn]);
    return ret;
  }

  // TODO: Move me to dyno.
  if (fWarnUnstable) {
    USR_WARN(call, "First class functions are unstable");
  }

  auto& entry = buildWrapperSuperTypeAtProgram(capturedFn);
  AggregateType* super = entry.type;
  FnSymbol* superThis = entry.thisMethod;

  auto ct = new AggregateType(AGGREGATE_CLASS);
  std::ostringstream fcfNameBuilder;

  fcfNameBuilder << "chpl_fcf_" << uniqueFcfId++ << "_" << capturedFn->name;
  auto fcfName = fcfNameBuilder.str();

  auto ts = new TypeSymbol(astr(fcfName.c_str()), ct);

  // Wrapper class has the same visibility as the underlying function.
  capturedFn->defPoint->insertBefore(new DefExpr(ts));

  ct->dispatchParents.add(super);

  bool inserted = super->dispatchChildren.add_exclusive(ct);
  INT_ASSERT(inserted);

  VarSymbol* superField = new VarSymbol("super", super);
  superField->addFlag(FLAG_SUPER_CLASS);

  ct->fields.insertAtHead(new DefExpr(superField));
  ct->processGenericFields();
  ct->buildDefaultInitializer();
  buildDefaultDestructor(ct);

  auto thisMethod = new FnSymbol("this");

  thisMethod->addFlag(FLAG_FIRST_CLASS_FUNCTION_INVOCATION);
  thisMethod->addFlag(FLAG_COMPILER_GENERATED);
  thisMethod->addFlag(FLAG_OVERRIDE);
  thisMethod->setMethod(true);
  thisMethod->retTag = superThis->retTag;

  auto mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
  thisMethod->insertFormalAtTail(mt);
  auto receiver = new ArgSymbol(INTENT_BLANK, "this", ct);
  receiver->addFlag(FLAG_ARG_THIS);
  thisMethod->insertFormalAtTail(receiver);
  thisMethod->_this = receiver;

  CallExpr* innerCall = new CallExpr(capturedFn);
  int formalIdx = 0;

  for_alist(formalExpr, superThis->formals) {
    if (formalIdx++ < 2) continue;

    auto oldFormalDefExpr = toDefExpr(formalExpr);
    auto oldFormal = toArgSymbol(oldFormalDefExpr->sym);
    auto newFormal = oldFormal->copy();
    auto seNewFormal = new SymExpr(newFormal);

    innerCall->insertAtTail(seNewFormal);
    thisMethod->insertFormalAtTail(newFormal);
  }

  std::vector<CallExpr*> calls;

  collectCallExprs(capturedFn, calls);

  // TODO: This check doesn't make any sense?
  for_vector(CallExpr, cl, calls) {
    if (cl->isPrimitive(PRIM_YIELD)) {
      USR_FATAL_CONT(cl, "Iterators not allowed in first class functions");
    }
  }

  if (capturedFn->retType == dtVoid) {
    thisMethod->insertAtTail(innerCall);
  } else {
    VarSymbol* tmp = newTemp("_return_tmp_");
    thisMethod->insertAtTail(new DefExpr(tmp));
    thisMethod->insertAtTail(new CallExpr(PRIM_MOVE, tmp, innerCall));
    thisMethod->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
  }

  if (capturedFn->throwsError()) thisMethod->throwsErrorInit();

  if (isBlockStmt(call->parentExpr) == true) {
    call->insertBefore(new DefExpr(thisMethod));
  } else {
    call->parentExpr->insertBefore(new DefExpr(thisMethod));
  }

  normalize(thisMethod);

  ct->methods.add(thisMethod);

  FnSymbol* wrapper = new FnSymbol("wrapper");

  wrapper->addFlag(FLAG_INLINE);

  // Insert the wrapper into the AST now so we can resolve some things.
  call->getStmtExpr()->insertBefore(new DefExpr(wrapper));

  BlockStmt* block = new BlockStmt();
  wrapper->insertAtTail(block);

  Type* undecorated = getDecoratedClass(ct, ClassTypeDecorator::GENERIC_NONNIL);

  NamedExpr* usym = new NamedExpr(astr_chpl_manager,
                                  new SymExpr(dtUnmanaged->symbol));

  // Create a new "unmanaged child".
  auto seUnmanaged = new SymExpr(undecorated->symbol);
  auto init = new CallExpr(PRIM_NEW, usym, new CallExpr(seUnmanaged));

  // Cast to "unmanaged parent".
  const auto parUnmanagedDecor = ClassTypeDecorator::UNMANAGED_NONNIL;
  auto parUnmanaged = getDecoratedClass(super, parUnmanagedDecor);
  auto parCast = new CallExpr(PRIM_CAST, parUnmanaged->symbol, init);

  // Get a handle to the type "_shared(parent)".
  Type* parShared = entry.sharedType;

  // Create a new "shared parent" temporary.
  VarSymbol* temp = newTemp("retval", parShared);
  block->insertAtTail(new DefExpr(temp));

  // Initialize the temporary with the result of the cast.
  CallExpr* initTemp = new CallExpr("init", gMethodToken, temp, parCast);
  block->insertAtTail(initTemp);

  // Return the "shared parent" temporary.
  CallExpr* retTemp = new CallExpr(PRIM_RETURN, temp);
  block->insertAtTail(retTemp);

  normalize(wrapper);
  wrapper->setGeneric(false);

  CallExpr* ret = new CallExpr(wrapper);

  payloadToSharedChildFactory[capturedFn] = wrapper;

  /* make writeThis for FCFs */
  {
    ArgSymbol* fileArg = NULL;
    FnSymbol* fn = buildWriteThisFnSymbol(ct, &fileArg);

    // All compiler generated writeThis routines now throw.
    fn->throwsErrorInit();

    // when printing out a FCF, print out the function's name
    if (ioModule == NULL) {
      INT_FATAL("never parsed IO module, this shouldn't be possible");
    }
    fn->body->useListAdd(new UseStmt(ioModule, "", false));
    fn->getModule()->moduleUseAdd(ioModule);
    auto nameStrLit = new_StringSymbol(astr(capturedFn->name, "()"));
    fn->insertAtTail(new CallExpr(new CallExpr(".", fileArg,
                                               new_StringSymbol("writeIt")),
                                               nameStrLit));
    normalize(fn);
  }

  return ret;
}

Expr* fcfRawFunctionPointerFromPrimCall(CallExpr* call) {
  INT_ASSERT(call && call->isPrimitive(PRIM_CAPTURE_FN_FOR_C));

  UnresolvedSymExpr* use = toUnresolvedSymExpr(call->get(1));
  FnSymbol* capturedFn = findFunctionFromNameMaybeError(use, call);

  if (!capturedFn) {
    auto dummy = getDummyFcfErrorInsertedAtProgram(call);
    return new SymExpr(dummy);
  }

  // TODO: Will this conflict with our new lowering?
  auto ret = new SymExpr(capturedFn);

  return ret;
}

// TODO: Do we need to ensure the function is resolved first?
static const FcfSuperInfo&
buildWrapperSuperTypeAtProgram(FnSymbol* fn) {
  std::vector<FcfFormalInfo> formals;
  RetTag retTag = fn->retTag;
  Type* retType = fn->retType;
  bool throws = fn->throwsError();

  for_formals(formal, fn) {
    const bool isAnonymous = formal->hasFlag(FLAG_ANONYMOUS_FORMAL);
    FcfFormalInfo info;
    info.type = formal->type;
    info.concreteIntent = formal->intent;
    info.name = isAnonymous ? nullptr : formal->name;
    formals.push_back(std::move(info));
  }

  auto& ret = buildWrapperSuperTypeAtProgram(formals, retTag, retType,
                                             throws);
  return ret;
}

static const char* intentToString(IntentTag tag) {
  switch (tag) {
    case INTENT_IN: return "in";
    case INTENT_OUT: return "out";
    case INTENT_INOUT: return "inout";
    case INTENT_CONST: return "const";
    case INTENT_CONST_IN: return "const in";
    case INTENT_REF: return "ref";
    case INTENT_CONST_REF: return "const ref";
    case INTENT_REF_MAYBE_CONST: return nullptr;
    case INTENT_PARAM: return "param";
    case INTENT_TYPE: return "type";
    case INTENT_BLANK: return nullptr;
  }
  return nullptr;
}

static const char* typeToStringSpecializing(Type* t) {
  auto vt = t->getValType();
  if (vt == dtInt[INT_SIZE_DEFAULT]) return "int";
  if (vt == dtUInt[INT_SIZE_DEFAULT]) return "uint";
  if (vt == dtReal[COMPLEX_SIZE_DEFAULT]) return "real";
  if (vt == dtBools[BOOL_SIZE_DEFAULT]) return "bool";
  if (vt == dtComplex[COMPLEX_SIZE_DEFAULT]) return "complex";
  if (vt == dtImag[FLOAT_SIZE_DEFAULT]) return "imag";
  auto ret = vt->symbol->name;
  return ret;
}

// TODO: Original intent or concrete intent?
static const char*
buildUserFacingTypeString(const std::vector<FcfFormalInfo>& formals,
                          RetTag retTag,
                          Type* retType,
                          bool throws) {
  std::ostringstream oss;
  oss << "proc" << "("; // TODO: Is this always a 'proc'?

  for (int i = 0; i < formals.size(); i++) {
    auto& info = formals[i];
    bool skip = isConcreteIntentBlank(info.concreteIntent, info.type);
    if (!skip) oss << intentToString(info.concreteIntent);
    if (info.name) oss << " " << info.name;
    if (!skip && info.type != dtAny) oss << ": ";
    if (info.type != dtAny) oss << typeToStringSpecializing(info.type);
    if ((i+1) != formals.size()) oss << ", ";
  }

  oss << ")";
  if (retTag != RET_VALUE) oss << " " << retTagDescrString(retTag);
  if (retType != dtVoid) {
    oss << ": " << typeToStringSpecializing(retType);
  }

  auto ret = astr(oss.str().c_str());
  return ret;
}

// For the formal types, only the value types will be considered (if any
// desugared ref types even exist at this point at compilation).
static const FcfSuperInfo&
buildWrapperSuperTypeAtProgram(const std::vector<FcfFormalInfo>& formals,
                               RetTag retTag,
                               Type* retType,
                               bool throws) {
  auto superName = buildSuperName(formals, retTag, retType, throws);

  // Fast path if what we want already exists.
  if (superNameToInfo.find(superName) != superNameToInfo.end()) {
    auto& ret = superNameToInfo[superName];
    return ret;
  }

  bool isAnyFormalNamed = false;
  for (auto& info : formals) {
    isAnyFormalNamed |= (info.name != nullptr);
    if (isAnyFormalNamed) break;
  }

  // Build up the entry.
  auto& v = superNameToInfo[superName];
  v.formals = formals;
  v.retTag = retTag;
  v.retType = retType;
  v.isAnyFormalNamed = isAnyFormalNamed;
  v.type = insertFcfWrapperSuperTypeAtProgram(superName);
  wrapperTypeToName[v.type] = superName;
  v.thisMethod = attachSuperThis(v.type, formals, retTag, retType, throws);
  v.sharedType = buildSharedWrapperType(v.type);
  wrapperTypeToName[v.sharedType] = superName;
  v.userTypeString = buildUserFacingTypeString(formals, retTag,
                                               retType,
                                               throws);

  // Present for compat with old lambdas but not very useful.
  attachOldStyleRetTypeGetter(v.type, retType);
  attachOldStyleArgTypeGetter(v.type, formals);

  return v;
}

Expr* fcfWrapperInstanceFromFnExpr(FnSymbol* fn) {
  INT_FATAL("Not implemented yet!");
  return nullptr;
}

Type* fcfWrapperSuperTypeFromFuncFnCall(CallExpr* call) {
  INT_ASSERT(call && call->isPrimitive(PRIM_CREATE_FN_TYPE));

  AList& argList = call->argList;
  SymExpr* retTail = toSymExpr(argList.tail);

  // These are the pieces the are required to build the super type.
  std::vector<FcfFormalInfo> formals;
  RetTag retTag = RET_VALUE;
  Type* retType = retTail->symbol()->type;
  bool throws = false;

  // Build up the formal types and intents.
  for_alist(expr, argList) {
    auto se = toSymExpr(expr);
    INT_ASSERT(se);
    FcfFormalInfo info;
    info.type = se->symbol()->type;
    info.concreteIntent = concreteIntent(INTENT_BLANK, info.type);
    info.name = nullptr;
    formals.push_back(std::move(info));
  }

  auto& entry = buildWrapperSuperTypeAtProgram(formals, retTag, retType,
                                               throws);
  auto ret = entry.sharedType;
  return ret;
}

Type* fcfWrapperSuperTypeFromFnType(FnSymbol* fn) {
  INT_ASSERT(fn->hasFlag(FLAG_ANONYMOUS_FN));
  INT_ASSERT(fn->hasFlag(FLAG_NO_FN_BODY));
  if (!checkAndResolveFunction(fn, fn->defPoint)) return dtUnknown;
  auto& entry = buildWrapperSuperTypeAtProgram(fn);
  auto ret = entry.sharedType;
  return ret;
}

static const char* intentTagMnemonicMangled(IntentTag tag) {
  switch (tag) {
    case INTENT_IN: return "I";
    case INTENT_OUT: return "O";
    case INTENT_INOUT: return "U";
    case INTENT_CONST: return "C";
    case INTENT_CONST_IN: return "CI";
    case INTENT_REF: return "R";
    case INTENT_CONST_REF: return "CR";
    case INTENT_REF_MAYBE_CONST: return "RMC";
    case INTENT_PARAM: return "P";
    case INTENT_TYPE: return "T";
    case INTENT_BLANK: return "";
  }
}

static const char* retTagMnemonicMangled(RetTag tag) {
  switch (tag) {
    case RET_VALUE: return "";
    case RET_REF: return "R";
    case RET_CONST_REF: return "CR";
    case RET_PARAM: return "P";
    case RET_TYPE: return "T";
  }
}

// Either a list of formals or a list of types in a tuple.
static const char*
buildSuperName(const std::vector<FcfFormalInfo>& formals,
               RetTag retTag,
               Type* retType,
               bool throws) {
  std::ostringstream oss;

  oss << superTypePrefix;

  for (auto& info : formals) {
    bool skip = isConcreteIntentBlank(info.concreteIntent, info.type);
    if (!skip) oss << intentTagMnemonicMangled(info.concreteIntent);
    oss << info.type->symbol->name;
    if (info.name) oss << info.name;
    oss << "_";
  }

  oss << "_";
  if (retTag != RET_VALUE) oss << retTagMnemonicMangled(retTag);
  oss << retType->symbol->name;
  if (throws) oss << "_throws";

  auto ret = astr(oss.str());

  return ret;
}

static AggregateType*
insertFcfWrapperSuperTypeAtProgram(const char* name) {
  auto ret = new AggregateType(AGGREGATE_CLASS);
  auto tsym = new TypeSymbol(name, ret);

  tsym->addFlag(FLAG_FUNCTION_CLASS);

  // TODO: Should we be using the base module or "TheProgram" here?
  baseModule->block->insertAtHead(new DefExpr(tsym));
  ret->dispatchParents.add(dtObject);
  dtObject->dispatchChildren.add(ret);

  VarSymbol* superSuper = new VarSymbol("super", dtObject);
  superSuper->addFlag(FLAG_SUPER_CLASS);
  ret->fields.insertAtHead(new DefExpr(superSuper));

  ret->processGenericFields();
  ret->buildDefaultInitializer();
  buildDefaultDestructor(ret);

  return ret;
}

static void
attachOldStyleRetTypeGetter(AggregateType* parent, Type* retType) {
  FnSymbol* get = new FnSymbol("retType");

  get->addFlag(FLAG_NO_IMPLICIT_COPY);
  get->addFlag(FLAG_INLINE);
  get->addFlag(FLAG_COMPILER_GENERATED);
  get->retTag = RET_TYPE;
  get->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                        "_mt",
                                        dtMethodToken));
  ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", parent);
  _this->addFlag(FLAG_ARG_THIS);
  get->insertFormalAtTail(_this);
  get->insertAtTail(new CallExpr(PRIM_RETURN, retType->symbol));
  parent->methods.add(get);
  parent->symbol->defPoint->insertBefore(new DefExpr(get));
  normalize(get);
  get->setMethod(true);
  get->addFlag(FLAG_METHOD_PRIMARY);
  get->name = astr("chpl_get_",
                   parent->symbol->name,
                   "_",
                   get->name);
  get->addFlag(FLAG_NO_PARENS);
  get->_this = _this;
}

static void
attachOldStyleArgTypeGetter(AggregateType* parent,
                            const std::vector<FcfFormalInfo>& formals) {
  FnSymbol* get = new FnSymbol("argTypes");
  get->addFlag(FLAG_NO_IMPLICIT_COPY);
  get->addFlag(FLAG_INLINE);
  get->addFlag(FLAG_COMPILER_GENERATED);
  get->retTag = RET_TYPE;
  get->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                        "_mt",
                                        dtMethodToken));

  CallExpr* expr = new CallExpr(PRIM_ACTUALS_LIST);
  for (auto& info : formals) expr->insertAtTail(info.type->symbol);

  ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", parent);
  _this->addFlag(FLAG_ARG_THIS);

  get->insertFormalAtTail(_this);
  get->insertAtTail(new CallExpr(PRIM_RETURN,
                                 new CallExpr("_build_tuple", expr)));
  DefExpr* def = new DefExpr(get);
  parent->symbol->defPoint->insertBefore(def);
  normalize(get);
  parent->methods.add(get);
  get->setMethod(true);
  get->addFlag(FLAG_METHOD_PRIMARY);
  get->name = astr("chpl_get_",
                   parent->symbol->name, "_",
                   get->name);
  get->addFlag(FLAG_NO_PARENS);
  get->_this = _this;
}

static FnSymbol*
attachSuperThis(AggregateType* super,
                const std::vector<FcfFormalInfo>& formals,
                RetTag retTag,
                Type* retType,
                bool throws) {
  FnSymbol* ret = new FnSymbol("this");
  ret->retTag = retTag;
  ret->addFlag(FLAG_FIRST_CLASS_FUNCTION_INVOCATION);
  ret->addFlag(FLAG_COMPILER_GENERATED);
  ret->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                        "_mt",
                                        dtMethodToken));
  ret->setMethod(true);
  ArgSymbol* receiver = new ArgSymbol(INTENT_BLANK, "this", super);
  receiver->addFlag(FLAG_ARG_THIS);
  ret->insertFormalAtTail(receiver);
  ret->_this = receiver;

  for (int i = 0; i < formals.size(); i++) {
    auto& info = formals[i];
    std::ignore = info.name; // Assume all formals anonymous.
    std::string name;
    name += "f";
    name += std::to_string(i);
    auto f = new ArgSymbol(info.concreteIntent,
                           astr(name.c_str()),
                           info.type);
    ret->insertFormalAtTail(f);
  }

  if (retType != dtVoid) {
    VarSymbol *tmp = newTemp("_return_tmp_", retType);
    ret->insertAtTail(new DefExpr(tmp));
    ret->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
  }

  if (throws) ret->throwsErrorInit();

  baseModule->block->insertAtHead(new DefExpr(ret));
  normalize(ret);
  super->methods.add(ret);

  return ret;
}

const char* fcfWrapperTypeToString(Type* t) {
  INT_ASSERT(t->symbol->hasFlag(FLAG_FUNCTION_CLASS));
  auto at = toAggregateType(t);

  if (wrapperTypeToName.find(at) == wrapperTypeToName.end()) {
    INT_ASSERT(at->dispatchParents.n > 0);
    auto super = at->dispatchParents.v[0];
    INT_ASSERT(super != dtObject);
    at = super;
  }

  INT_ASSERT(wrapperTypeToName.find(at) != wrapperTypeToName.end());
  auto name = wrapperTypeToName[at];

  INT_ASSERT(superNameToInfo.find(name) != superNameToInfo.end());
  auto& entry = superNameToInfo[name];

  auto ret = astr(entry.userTypeString);
  return ret;
}

