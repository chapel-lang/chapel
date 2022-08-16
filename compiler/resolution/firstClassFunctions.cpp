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
    const char* name;
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

using SharedFcfSuperInfo = std::shared_ptr<FcfSuperInfo>;

static std::map<const char*, SharedFcfSuperInfo> superNameToInfo;
static std::map<Type*, SharedFcfSuperInfo> typeToInfo;
static std::map<FnSymbol*, FnSymbol*> payloadToSharedChildFactory;
static std::map<FnSymbol*, bool> payloadToResolved;
static const char* superTypePrefix = "chpl_fcf_";
static VarSymbol* dummyFcfError = nullptr;
static int uniqueFcfId = 0;

/************************************* | *************************************
*                                                                            *
*                                                                            *
*                                                                            *
************************************** | ************************************/

static bool isConcreteIntentBlank(IntentTag tag, Type* t);

static VarSymbol* getDummyFcfErrorInsertedAtProgram(CallExpr* call);

static Type* buildSharedWrapperType(AggregateType* super);

static bool checkAndResolveFunctionSignature(FnSymbol* fn, Expr* use);

static bool checkAndResolveFunction(FnSymbol* fn, Expr* use);

static FnSymbol* findFunctionFromNameMaybeError(UnresolvedSymExpr* usym,
                                                CallExpr* root);

static const SharedFcfSuperInfo
buildWrapperSuperTypeAtProgram(FnSymbol* fn);

static const char* intentToString(IntentTag tag);

static const char* typeToStringSpecializing(Type* t);

static const char*
buildUserFacingTypeString(const std::vector<FcfFormalInfo>& formals,
                          RetTag retTag,
                          Type* retType,
                          bool throws);

static const SharedFcfSuperInfo
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

static FnSymbol*
attachSuperRetTypeGetter(AggregateType* super, Type* retType);

static FnSymbol*
attachSuperArgTypeGetter(AggregateType* super,
                         const std::vector<FcfFormalInfo>& formals);

static FnSymbol* attachSuperPayloadPtrGetter(AggregateType* super);

static FnSymbol*
attachSuperThis(AggregateType* super,
                const std::vector<FcfFormalInfo>& formals,
                RetTag retTag,
                Type* retType,
                bool throws);

static AggregateType*
insertChildWrapperAtPayload(const SharedFcfSuperInfo info,
                            FnSymbol* payload);

static FnSymbol*
attachChildThis(const SharedFcfSuperInfo info, AggregateType* child,
                FnSymbol* payload);

static FnSymbol*
attachChildWriteThis(const SharedFcfSuperInfo info, AggregateType* child,
                     FnSymbol* payload);

static FnSymbol*
attachChildPayloadPtrGetter(const SharedFcfSuperInfo info,
                            AggregateType* child,
                            FnSymbol* payload);

static FnSymbol*
attachChildSharedFactory(const SharedFcfSuperInfo info,
                         AggregateType* child,
                         FnSymbol* payload);

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
  auto super = insertFcfWrapperSuperTypeAtProgram("_fcf_error");
  dummyFcfError = newTemp(super);
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
    if (fn->retExprType) {
      resolveSpecifiedReturnType(fn);
    } else {
      INT_ASSERT(!fn->hasFlag(FLAG_NO_FN_BODY));
      resolveFunction(fn);
      INT_ASSERT(fn->isResolved());
    }
  }

  bool hasGenericFormal = false;
  for_formals(formal, fn) {
    hasGenericFormal |= formal->type->symbol->hasFlag(FLAG_GENERIC);
    if (hasGenericFormal) break;
  }

  if (hasGenericFormal) {
    USR_FATAL_CONT(use, "'%s' cannot be captured as a value because "
                        "it is a generic function",
                        fn->name);
    return false;
  }

  std::vector<CallExpr*> calls;
  collectCallExprs(fn, calls);

  // TODO: This check doesn't make any sense?
  bool containsYield = false;
  for_vector(CallExpr, cl, calls) {
    if (cl->isPrimitive(PRIM_YIELD)) {
      USR_FATAL_CONT(cl, "Iterators not allowed in first class functions");
      containsYield = true;
    }
  }

  if (containsYield) return false;

  return true;
}

static bool checkAndResolveFunction(FnSymbol* fn, Expr* use) {
  if (payloadToResolved.find(fn) != payloadToResolved.end()) {
    return payloadToResolved[fn];
  }

  bool status = false;
  if (checkAndResolveFunctionSignature(fn, use)) {
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
  auto payload = findFunctionFromNameMaybeError(use, call);

  if (!payload || !checkAndResolveFunction(payload, use)) {
    auto dummy = getDummyFcfErrorInsertedAtProgram(call);
    return new SymExpr(dummy);
  }

  // Reuse cached FCF wrapper if it already exists.
  if (payloadToSharedChildFactory.find(payload) !=
      payloadToSharedChildFactory.end()) {
    auto ret = new CallExpr(payloadToSharedChildFactory[payload]);
    return ret;
  }

  // TODO: Move me to dyno.
  if (fWarnUnstable) {
    USR_WARN(call, "First class functions are unstable");
  }

  auto info = buildWrapperSuperTypeAtProgram(payload);

  auto child = insertChildWrapperAtPayload(info, payload);
  std::ignore = attachChildThis(info, child, payload);
  std::ignore = attachChildWriteThis(info, child, payload);
  std::ignore = attachChildPayloadPtrGetter(info, child, payload);
  auto wrapper = attachChildSharedFactory(info, child, payload);

  CallExpr* ret = new CallExpr(wrapper);

  payloadToSharedChildFactory[payload] = wrapper;

  return ret;
}

Expr* fcfRawFunctionPointerFromPrimCall(CallExpr* call) {
  INT_ASSERT(call && call->isPrimitive(PRIM_CAPTURE_FN_FOR_C));

  UnresolvedSymExpr* use = toUnresolvedSymExpr(call->get(1));
  FnSymbol* payload = findFunctionFromNameMaybeError(use, call);

  if (!payload) {
    auto dummy = getDummyFcfErrorInsertedAtProgram(call);
    return new SymExpr(dummy);
  }

  // TODO: Will this conflict with our new lowering?
  auto ret = new SymExpr(payload);

  return ret;
}

// TODO: Do we need to ensure the function is resolved first?
static const SharedFcfSuperInfo
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
    info.name = isAnonymous ? nullptr : formal->cname;
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
  auto ret = vt->symbol->cname;
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
    if (!skip && info.name) oss << " ";
    if (info.name) oss << info.name;
    if ((!skip || info.name) && info.type != dtAny) oss << ": ";
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
static const SharedFcfSuperInfo
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

  // Build up the info.
  auto& v = superNameToInfo[superName];
  v = std::shared_ptr<FcfSuperInfo>(new FcfSuperInfo());
  v->name = superName;
  v->formals = formals;
  v->retTag = retTag;
  v->retType = retType;
  v->isAnyFormalNamed = isAnyFormalNamed;

  v->type = insertFcfWrapperSuperTypeAtProgram(superName);
  typeToInfo[v->type] = v;

  v->thisMethod = attachSuperThis(v->type, formals, retTag,
                                  retType,
                                  throws);

  v->sharedType = buildSharedWrapperType(v->type);
  typeToInfo[v->sharedType] = v;

  v->userTypeString = buildUserFacingTypeString(formals, retTag,
                                                retType,
                                                throws);

  std::ignore = attachSuperRetTypeGetter(v->type, retType);
  std::ignore = attachSuperArgTypeGetter(v->type, formals);
  std::ignore = attachSuperPayloadPtrGetter(v->type);

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

  auto info = buildWrapperSuperTypeAtProgram(formals, retTag, retType,
                                               throws);
  auto ret = info->sharedType;
  return ret;
}

Type* fcfWrapperSuperTypeFromFnType(FnSymbol* fn) {
  INT_ASSERT(fn->hasFlag(FLAG_ANONYMOUS_FN));
  INT_ASSERT(fn->hasFlag(FLAG_NO_FN_BODY));
  if (!checkAndResolveFunction(fn, fn->defPoint)) return dtUnknown;
  auto info = buildWrapperSuperTypeAtProgram(fn);
  auto ret = info->sharedType;
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
    oss << typeToStringSpecializing(info.type) << "_";
    if (info.name) oss << info.name;
    oss << "_";
  }

  oss << "_";
  if (retTag != RET_VALUE) oss << retTagMnemonicMangled(retTag) << "_";
  oss << typeToStringSpecializing(retType);
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

static FnSymbol*
attachSuperRetTypeGetter(AggregateType* super, Type* retType) {
  FnSymbol* ret = new FnSymbol("retType");

  ret->addFlag(FLAG_NO_IMPLICIT_COPY);
  ret->addFlag(FLAG_INLINE);
  ret->addFlag(FLAG_COMPILER_GENERATED);
  ret->retTag = RET_TYPE;
  ret->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                        "_mt",
                                        dtMethodToken));
  auto receiver = new ArgSymbol(INTENT_BLANK, "this", super);
  receiver->addFlag(FLAG_ARG_THIS);
  ret->insertFormalAtTail(receiver);
  ret->insertAtTail(new CallExpr(PRIM_RETURN, retType->symbol));
  super->methods.add(ret);
  super->symbol->defPoint->insertBefore(new DefExpr(ret));
  normalize(ret);
  ret->setMethod(true);
  ret->addFlag(FLAG_METHOD_PRIMARY);
  ret->name = astr("chpl_get_",
                   super->symbol->name,
                   "_",
                   ret->name);
  ret->addFlag(FLAG_NO_PARENS);
  ret->_this = receiver;

  return ret;
}

static FnSymbol*
attachSuperArgTypeGetter(AggregateType* super,
                            const std::vector<FcfFormalInfo>& formals) {
  FnSymbol* ret = new FnSymbol("argTypes");
  ret->addFlag(FLAG_NO_IMPLICIT_COPY);
  ret->addFlag(FLAG_INLINE);
  ret->addFlag(FLAG_COMPILER_GENERATED);
  ret->retTag = RET_TYPE;
  ret->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                        "_mt",
                                        dtMethodToken));

  CallExpr* expr = new CallExpr(PRIM_ACTUALS_LIST);
  for (auto& info : formals) expr->insertAtTail(info.type->symbol);

  auto receiver = new ArgSymbol(INTENT_BLANK, "this", super);
  receiver->addFlag(FLAG_ARG_THIS);

  ret->insertFormalAtTail(receiver);
  ret->insertAtTail(new CallExpr(PRIM_RETURN,
                                 new CallExpr("_build_tuple", expr)));
  DefExpr* def = new DefExpr(ret);
  super->symbol->defPoint->insertBefore(def);
  normalize(ret);
  super->methods.add(ret);
  ret->setMethod(true);
  ret->addFlag(FLAG_METHOD_PRIMARY);
  ret->name = astr("chpl_get_",
                   super->symbol->name, "_",
                   ret->name);
  ret->addFlag(FLAG_NO_PARENS);
  ret->_this = receiver;

  return ret;
}

static FnSymbol* attachSuperPayloadPtrGetter(AggregateType* super) {
  FnSymbol* ret = new FnSymbol("chpl_fcfPtr");

  ret->addFlag(FLAG_COMPILER_GENERATED);
  ret->retTag = RET_VALUE;
  ret->retType = dtCFnPtr;
  ret->setMethod(true);

  auto mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
  ret->insertFormalAtTail(mt);

  auto receiver = new ArgSymbol(INTENT_BLANK, "this", super);
  receiver->addFlag(FLAG_ARG_THIS);
  ret->insertFormalAtTail(receiver);
  ret->_this = receiver;

  auto msg = astr("Unexpected call to virtual method '", ret->name, "'");
  auto lit = new_StringSymbol(msg);
  ret->insertAtTail(new CallExpr("halt", lit));
  ret->insertAtTail(new CallExpr(PRIM_RETURN, new SymExpr(gNil)));

  baseModule->block->insertAtHead(new DefExpr(ret));
  normalize(ret);
  super->methods.add(ret);

  return ret;
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
  ret->setMethod(true);

  auto mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
  ret->insertFormalAtTail(mt);

  auto receiver = new ArgSymbol(INTENT_BLANK, "this", super);
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
  INT_ASSERT(t && t->symbol->hasFlag(FLAG_FUNCTION_CLASS));
  auto at = toAggregateType(t);

  if (typeToInfo.find(at) == typeToInfo.end()) {
    INT_ASSERT(at->dispatchParents.n > 0);
    auto super = at->dispatchParents.v[0];
    INT_ASSERT(super != dtObject);
    at = super;
  }

  INT_ASSERT(typeToInfo.find(at) != typeToInfo.end());
  auto info = typeToInfo[at];
  auto ret = astr(info->userTypeString);

  return ret;
}

static AggregateType*
insertChildWrapperAtPayload(const SharedFcfSuperInfo info,
                            FnSymbol* payload) {
  AggregateType* super = info->type;
  auto ret = new AggregateType(AGGREGATE_CLASS);
  std::ostringstream oss;

  oss << "chpl_fcf_" << uniqueFcfId++ << "_" << payload->name;
  auto name = oss.str();

  auto ts = new TypeSymbol(astr(name.c_str()), ret);

  // Wrapper class has the same visibility as the underlying function.
  payload->defPoint->insertBefore(new DefExpr(ts));

  ret->dispatchParents.add(super);

  bool inserted = super->dispatchChildren.add_exclusive(ret);
  INT_ASSERT(inserted);

  VarSymbol* superField = new VarSymbol("super", super);
  superField->addFlag(FLAG_SUPER_CLASS);

  ret->fields.insertAtHead(new DefExpr(superField));
  ret->processGenericFields();
  ret->buildDefaultInitializer();
  buildDefaultDestructor(ret);

  typeToInfo[ret] = info;

  return ret;
}

static FnSymbol*
attachChildThis(const SharedFcfSuperInfo info, AggregateType* child,
                FnSymbol* payload) {
  FnSymbol* superThis = info->thisMethod;

  auto ret = new FnSymbol("this");

  ret->addFlag(FLAG_FIRST_CLASS_FUNCTION_INVOCATION);
  ret->addFlag(FLAG_COMPILER_GENERATED);
  ret->addFlag(FLAG_OVERRIDE);
  ret->setMethod(true);
  ret->retTag = superThis->retTag;

  auto mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
  ret->insertFormalAtTail(mt);

  auto receiver = new ArgSymbol(INTENT_BLANK, "this", child);
  receiver->addFlag(FLAG_ARG_THIS);

  ret->insertFormalAtTail(receiver);
  ret->_this = receiver;

  CallExpr* innerCall = new CallExpr(payload);
  int formalIdx = 0;

  for_alist(formalExpr, superThis->formals) {
    if (formalIdx++ < 2) continue;

    auto oldFormalDefExpr = toDefExpr(formalExpr);
    auto oldFormal = toArgSymbol(oldFormalDefExpr->sym);
    auto newFormal = oldFormal->copy();
    auto seNewFormal = new SymExpr(newFormal);

    innerCall->insertAtTail(seNewFormal);
    ret->insertFormalAtTail(newFormal);
  }

  if (payload->retType == dtVoid) {
    ret->insertAtTail(innerCall);
  } else {
    VarSymbol* tmp = newTemp("_return_tmp_");
    ret->insertAtTail(new DefExpr(tmp));
    ret->insertAtTail(new CallExpr(PRIM_MOVE, tmp, innerCall));
    ret->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
  }

  if (payload->throwsError()) ret->throwsErrorInit();

  if (isBlockStmt(payload->defPoint->parentExpr) == true) {
    payload->defPoint->insertBefore(new DefExpr(ret));
  } else {
    payload->defPoint->insertBefore(new DefExpr(ret));
  }

  normalize(ret);

  child->methods.add(ret);

  return ret;
}

static FnSymbol*
attachChildWriteThis(const SharedFcfSuperInfo info,
                     AggregateType* child,
                     FnSymbol* payload) {
  ArgSymbol* fileArg = NULL;
  FnSymbol* ret = buildWriteThisFnSymbol(child, &fileArg);

  // All compiler generated writeThis routines now throw.
  ret->throwsErrorInit();

  if (ioModule == NULL) {
    INT_FATAL("never parsed IO module, this shouldn't be possible");
  }

  ret->body->useListAdd(new UseStmt(ioModule, "", false));
  ret->getModule()->moduleUseAdd(ioModule);
  auto str = new_StringSymbol(payload->name);
  auto writeCall = new CallExpr(".", fileArg, new_StringSymbol("writeIt"),
                                str);
  ret->insertAtTail(new CallExpr(writeCall));
  normalize(ret);

  return ret;
}

static FnSymbol*
attachChildPayloadPtrGetter(const SharedFcfSuperInfo info,
                            AggregateType* child,
                            FnSymbol* payload) {
  auto ret = new FnSymbol("chpl_fcfPtr");

  ret->addFlag(FLAG_COMPILER_GENERATED);
  ret->addFlag(FLAG_OVERRIDE);
  ret->setMethod(true);
  ret->retTag = RET_VALUE;
  ret->retType = dtCFnPtr;

  auto mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
  ret->insertFormalAtTail(mt);

  auto receiver = new ArgSymbol(INTENT_BLANK, "this", child);
  receiver->addFlag(FLAG_ARG_THIS);
  ret->insertFormalAtTail(receiver);
  ret->_this = receiver;

  // Have to use temporary/move to get the type colored properly.
  auto tmp = newTemp("tmp_ret");
  ret->insertAtTail(new DefExpr(tmp));
  auto move = new CallExpr(PRIM_MOVE, new SymExpr(tmp),
                           new SymExpr(payload));
  ret->insertAtTail(move);
  ret->insertAtTail(new CallExpr(PRIM_RETURN, new SymExpr(tmp)));

  DefExpr* def = new DefExpr(ret);
  child->symbol->defPoint->insertAfter(def);
  normalize(ret);
  child->methods.add(ret);

  return ret;
}

static FnSymbol*
attachChildSharedFactory(const SharedFcfSuperInfo info,
                         AggregateType* child,
                         FnSymbol* payload) {
  AggregateType* super = info->type;
  FnSymbol* ret = new FnSymbol("wrapper");

  ret->addFlag(FLAG_COMPILER_GENERATED);
  ret->addFlag(FLAG_INLINE);

  // Insert the wrapper into the AST now so we can resolve some things.
  payload->defPoint->getStmtExpr()->insertBefore(new DefExpr(ret));

  BlockStmt* block = new BlockStmt();
  ret->insertAtTail(block);

  auto dec = getDecoratedClass(child, ClassTypeDecorator::GENERIC_NONNIL);
  auto usym = new NamedExpr(astr_chpl_manager,
                            new SymExpr(dtUnmanaged->symbol));

  // Create a new "unmanaged child".
  auto seUnmanaged = new SymExpr(dec->symbol);
  auto init = new CallExpr(PRIM_NEW, usym, new CallExpr(seUnmanaged));

  // Cast to "unmanaged parent".
  const auto parUnmanagedDecor = ClassTypeDecorator::UNMANAGED_NONNIL;
  auto parUnmanaged = getDecoratedClass(super, parUnmanagedDecor);
  auto parCast = new CallExpr(PRIM_CAST, parUnmanaged->symbol, init);

  // Get a handle to the type "_shared(parent)".
  Type* parShared = info->sharedType;

  // Create a new "shared parent" temporary.
  VarSymbol* temp = newTemp("retval", parShared);
  block->insertAtTail(new DefExpr(temp));

  // Initialize the temporary with the result of the cast.
  CallExpr* initTemp = new CallExpr("init", gMethodToken, temp, parCast);
  block->insertAtTail(initTemp);

  // Return the "shared parent" temporary.
  CallExpr* retTemp = new CallExpr(PRIM_RETURN, temp);
  block->insertAtTail(retTemp);

  normalize(ret);
  ret->setGeneric(false);

  return ret;
}

bool fcfIsValidExternType(Type* t) {
  if (!t || !t->symbol->hasFlag(FLAG_FUNCTION_CLASS)) return false;
  auto info = typeToInfo[t];
  INT_ASSERT(info.get() != nullptr);

  // TODO: What do we do about intents here? E.g., out intent...
  for (auto& formal : info->formals) {
    if (!isExternType(formal.type)) return false;
  }

  if (!isExternType(info->retType)) return false;

  return true;
}

