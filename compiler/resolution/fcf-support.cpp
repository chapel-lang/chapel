/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#include "fcf-support.h"

#include "astutil.h"
#include "AstVisitorTraverse.h"
#include "buildDefaultFunctions.h"
#include "DecoratedClassType.h"
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
  using namespace fcfs;

  struct FcfFormalInfo {
    Type* type;
    IntentTag intent;
    const char* name;
  };

  struct FcfSuperInfo {
    const char* name;
    std::vector<FcfFormalInfo> formals;
    RetTag retTag;
    Type* retType;
    bool throws;
    bool isAnyFormalNamed;
    AggregateType* type;
    Type* sharedType;
    FnSymbol* thisMethod;
    const char* userTypeString;
  };

using SharedFcfSuperInfo = std::shared_ptr<FcfSuperInfo>;

static std::map<const char*, SharedFcfSuperInfo> superNameToInfo;
static std::map<Type*, SharedFcfSuperInfo> typeToInfo;
static std::map<FnSymbol*, FnSymbol*> payloadToSharedParentFactory;
static std::map<FnSymbol*, bool> payloadToResolved;
static const char* superTypePrefix = "chpl_fcf_";

static int uniqueFcfId = 0;

/************************************* | *************************************
*                                                                            *
*                                                                            *
*                                                                            *
************************************** | ************************************/

static bool isIntentSameAsDefault(IntentTag tag, Type* t);

static Type* buildSharedWrapperType(AggregateType* super);

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
insertFcfWrapperSuperTypeAtProgram(const char* name,
                                   AggregateType* superSuper=dtObject);

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

static FnSymbol*
attachSuperWriteMethod(AggregateType* super, const char* name);

static AggregateType*
insertChildWrapperAtPayload(const SharedFcfSuperInfo info,
                            FnSymbol* payload);

static FnSymbol*
attachChildThis(const SharedFcfSuperInfo info, AggregateType* child,
                FnSymbol* payload);

static FnSymbol*
attachChildWriteMethod(const SharedFcfSuperInfo info, AggregateType* child,
                       FnSymbol* payload,
                       const char* name);

static FnSymbol*
attachChildPayloadPtrGetter(const SharedFcfSuperInfo info,
                            AggregateType* child,
                            FnSymbol* payload);

static FnSymbol*
insertSharedParentFactory(const SharedFcfSuperInfo info,
                          AggregateType* child,
                          FnSymbol* payload);

static Expr* createLegacyClassInstance(FnSymbol* fn, Expr* use);

/************************************* | *************************************
*                                                                            *
*                                                                            *
*                                                                            *
************************************** | ************************************/

static bool isIntentSameAsDefault(IntentTag tag, Type* t) {
  auto ret = concreteIntent(INTENT_BLANK, t) == concreteIntent(tag, t);
  return ret;
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

static FcfFormalInfo extractFormalInfo(ArgSymbol* formal) {
  const bool isAnonymous = formal->hasFlag(FLAG_ANONYMOUS_FORMAL);
  FcfFormalInfo ret;

  ret.type = formal->type;
  ret.intent = formal->intent;
  ret.name = isAnonymous ? nullptr : formal->cname;

  return ret;
}

static const SharedFcfSuperInfo
buildWrapperSuperTypeAtProgram(FnSymbol* fn) {
  std::vector<FcfFormalInfo> formals;
  RetTag retTag = fn->retTag;
  Type* retType = fn->retType;
  bool throws = fn->throwsError();

  for_formals(formal, fn) {
    auto info = extractFormalInfo(formal);
    formals.push_back(std::move(info));
  }

  auto& ret = buildWrapperSuperTypeAtProgram(formals, retTag, retType,
                                             throws);
  return ret;
}

static const char* intentToString(IntentTag tag) {
  return FunctionType::intentToString(tag);
}

static const char* typeToStringSpecializing(Type* t) {
  return FunctionType::typeToString(t);
}

// TODO: Original intent or concrete intent?
static const char*
buildUserFacingTypeString(const std::vector<FcfFormalInfo>& formals,
                          RetTag retTag,
                          Type* retType,
                          bool throws) {
  std::ostringstream oss;
  oss << "proc" << "("; // TODO: Is this always a 'proc'?

  for (size_t i = 0; i < formals.size(); i++) {
    auto& info = formals[i];
    bool skip = isIntentSameAsDefault(info.intent, info.type);
    if (!skip) oss << intentToString(info.intent);
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

  // Fetch the wrapper class type for an FCF with no named formals.
  // It will be the parent class of this type.
  AggregateType* at = nullptr;
  if (isAnyFormalNamed) {
    std::vector<FcfFormalInfo> unnamedFormals;

    for (auto& f : formals) {
      FcfFormalInfo copy = { f.type, f.intent, nullptr };
      unnamedFormals.push_back(std::move(copy));
    }

    auto info = buildWrapperSuperTypeAtProgram(unnamedFormals, retTag,
                                               retType,
                                               throws);

    at = insertFcfWrapperSuperTypeAtProgram(superName, info->type);
    info->type->dispatchChildren.add(at);

  // Or no formals are named, so use 'object' as the parent class.
  } else {
    at = insertFcfWrapperSuperTypeAtProgram(superName);
  }

  // Build up the info.
  auto& v = superNameToInfo[superName];
  v = std::shared_ptr<FcfSuperInfo>(new FcfSuperInfo());
  v->name = superName;
  v->formals = formals;
  v->retTag = retTag;
  v->retType = retType;
  v->throws = throws;
  v->isAnyFormalNamed = isAnyFormalNamed;

  v->type = at;
  typeToInfo[v->type] = v;
  v->thisMethod = attachSuperThis(v->type, formals, retTag,
                                  retType,
                                  throws);
  std::ignore = attachSuperWriteMethod(v->type, "writeThis");
  if (fUseIOFormatters) {
    std::ignore = attachSuperWriteMethod(v->type, "encodeTo");
  }

  if (isAnyFormalNamed) v->thisMethod->addFlag(FLAG_OVERRIDE);

  // This ordering matters to prevent a circular dependency in 'toString'.
  v->sharedType = buildSharedWrapperType(v->type);
  typeToInfo[v->sharedType] = v;

  // Ordering matters relative to above.
  v->userTypeString = buildUserFacingTypeString(formals, retTag,
                                                retType,
                                                throws);

  // Only generate method skeletons for the root class.
  if (!isAnyFormalNamed) {
    std::ignore = attachSuperRetTypeGetter(v->type, retType);
    std::ignore = attachSuperArgTypeGetter(v->type, formals);
    std::ignore = attachSuperPayloadPtrGetter(v->type);
  }

  return v;
}

static const char* intentTagMnemonicMangled(IntentTag tag) {
  return FunctionType::intentTagMnemonicMangled(tag);
}

static const char* retTagMnemonicMangled(RetTag tag) {
  return FunctionType::retTagMnemonicMangled(tag);
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
    bool skip = isIntentSameAsDefault(info.intent, info.type);
    if (!skip) oss << intentTagMnemonicMangled(info.intent);
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
insertFcfWrapperSuperTypeAtProgram(const char* name,
                                   AggregateType* superSuper) {
  auto ret = new AggregateType(AGGREGATE_CLASS);
  auto tsym = new TypeSymbol(name, ret);

  tsym->addFlag(FLAG_FUNCTION_CLASS);

  // TODO: Should we be using the base module or "TheProgram" here?
  baseModule->block->insertAtHead(new DefExpr(tsym));

  ret->dispatchParents.add(superSuper);
  superSuper->dispatchChildren.add(ret);

  VarSymbol* superField = new VarSymbol("super", superSuper);
  superField->addFlag(FLAG_SUPER_CLASS);
  ret->fields.insertAtHead(new DefExpr(superField));

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
  ret->setMethod(true);
  ret->addFlag(FLAG_METHOD_PRIMARY);
  ret->addFlag(FLAG_NO_PARENS);
  ret->cname = astr("chpl_get_",
                    super->symbol->cname,
                    "_",
                    ret->cname);

  auto mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
  ret->insertFormalAtTail(mt);

  auto receiver = new ArgSymbol(INTENT_BLANK, "this", super);
  receiver->addFlag(FLAG_ARG_THIS);
  ret->insertFormalAtTail(receiver);
  ret->_this = receiver;

  ret->insertAtTail(new CallExpr(PRIM_RETURN, retType->symbol));

  super->methods.add(ret);
  super->symbol->defPoint->insertBefore(new DefExpr(ret));
  normalize(ret);

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
  ret->setMethod(true);
  ret->addFlag(FLAG_METHOD_PRIMARY);
  ret->addFlag(FLAG_NO_PARENS);
  ret->cname = astr("chpl_get_",
                    super->symbol->cname, "_",
                    ret->cname);

  auto mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
  ret->insertFormalAtTail(mt);

  CallExpr* expr = new CallExpr(PRIM_ACTUALS_LIST);
  for (auto& info : formals) expr->insertAtTail(info.type->symbol);

  auto receiver = new ArgSymbol(INTENT_BLANK, "this", super);
  receiver->addFlag(FLAG_ARG_THIS);
  ret->_this = receiver;
  ret->insertFormalAtTail(receiver);

  ret->insertAtTail(new CallExpr(PRIM_RETURN,
                                 new CallExpr("_build_tuple", expr)));

  DefExpr* def = new DefExpr(ret);
  super->symbol->defPoint->insertBefore(def);
  normalize(ret);
  super->methods.add(ret);

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
  if (throws) ret->throwsErrorInit();

  auto mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
  ret->insertFormalAtTail(mt);

  auto receiver = new ArgSymbol(INTENT_BLANK, "this", super);
  receiver->addFlag(FLAG_ARG_THIS);
  ret->insertFormalAtTail(receiver);
  ret->_this = receiver;

  for (size_t i = 0; i < formals.size(); i++) {
    auto& info = formals[i];
    std::ignore = info.name; // Assume all formals anonymous.
    std::string name;
    name += "f";
    name += std::to_string(i);
    auto f = new ArgSymbol(info.intent,
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

static FnSymbol*
attachSuperWriteMethod(AggregateType* super, const char* name) {
  ArgSymbol* fileArg = nullptr;
  auto ret = buildWriteThisFnSymbol(super, &fileArg, name);
  ret->throwsErrorInit();
  normalize(ret);
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
  if (info->throws) ret->throwsErrorInit();

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
attachChildWriteMethod(const SharedFcfSuperInfo info,
                     AggregateType* child,
                     FnSymbol* payload,
                     const char* name) {
  ArgSymbol* fileArg = NULL;
  FnSymbol* ret = buildWriteThisFnSymbol(child, &fileArg, name);

  // All compiler generated writeThis routines now throw.
  ret->throwsErrorInit();

  if (ioModule == NULL) {
    INT_FATAL("never parsed IO module, this shouldn't be possible");
  }

  ret->body->useListAdd(new UseStmt(ioModule, "", false));
  ret->getModule()->moduleUseAdd(ioModule);
  auto str = new_StringSymbol(astr(payload->name, "()"));
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
insertSharedParentFactory(const SharedFcfSuperInfo info,
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

static Expr* createLegacyClassInstance(FnSymbol* fn, Expr* use) {
  INT_ASSERT(fn && fn->defPoint && fn->type);

  auto ft = toFunctionType(fn->type);
  INT_ASSERT(ft);

  INT_ASSERT(!ft->isGeneric() && ft->returnType() != dtUnknown);

  checkAndResolveSignatureAndBody(fn, use);

  // Reuse cached FCF wrapper if it already exists.
  if (payloadToSharedParentFactory.find(fn) !=
      payloadToSharedParentFactory.end()) {
    auto ret = new CallExpr(payloadToSharedParentFactory[fn]);
    return ret;
  }

  auto info = buildWrapperSuperTypeAtProgram(fn);

  auto child = insertChildWrapperAtPayload(info, fn);
  std::ignore = attachChildThis(info, child, fn);

  std::ignore = attachChildWriteMethod(info, child, fn, "writeThis");
  if (fUseIOFormatters) {
    std::ignore = attachChildWriteMethod(info, child, fn, "encodeTo");
  }

  std::ignore = attachChildPayloadPtrGetter(info, child, fn);
  auto factory = insertSharedParentFactory(info, child, fn);

  payloadToSharedParentFactory[fn] = factory;

  // Return an inserted call to the wrapper that creates the instance.
  auto ret = new CallExpr(factory);
  return ret;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

//
// TODO: The modern implementation stuff will go here...
//

} // end anonymous namespace

namespace fcfs {

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// This visitor will collect all the uses of outer variables in a function.
struct OuterVariableCollector : public AstVisitorTraverse {
  using OuterVarToMentionMap = std::map<Symbol*, std::vector<SymExpr*>>;

  FnSymbol* owner_;
  std::vector<Symbol*>& outerVariables_;
  OuterVarToMentionMap& outerVariableToMentions_;
  std::vector<FnSymbol*>& childFunctions_;

  OuterVariableCollector(FnSymbol* owner,
                         std::vector<Symbol*>& outerVariables,
                         OuterVarToMentionMap& outerVariableToMentions,
                         std::vector<FnSymbol*>& childFunctions)
      : owner_(owner),
        outerVariables_(outerVariables),
        outerVariableToMentions_(outerVariableToMentions),
        childFunctions_(childFunctions) {
    return;
  }

 ~OuterVariableCollector() = default;

  bool isOuterSymbol(Symbol* node) const {
    auto p = node->defPoint->parentSymbol;
    auto ret = p != owner_;
    return ret;
  }

  bool isSymbolOfInterest(Symbol* node) const {
    auto ret = isLcnSymbol(node) && !isGlobal(node);
    return ret;
  }

  void visitSymExpr(SymExpr* node) override {
    auto sym = node->symbol();
    if (!isOuterSymbol(sym) || !isSymbolOfInterest(sym)) return;
    auto& mentions = outerVariableToMentions_[sym];
    if (mentions.empty()) outerVariables_.push_back(sym);
    mentions.push_back(node);
  }

  bool enterFnSym(FnSymbol* node) override {
    childFunctions_.push_back(node);
    return false;
  }
};

ClosureEnv::ClosureEnv(FnSymbol* owner) : owner_(owner) {
  INT_ASSERT(owner);
  auto p = owner->defPoint->parentSymbol;

  INT_ASSERT(p->getModule() == owner->getModule());

  // Visitor takes our fields by reference and populates them.
  if (isFnSymbol(p) && p != owner->getModule()->initFn) {
    auto v = OuterVariableCollector(owner_, outerVariables_,
                                    outerVariableToMentions_,
                                    childFunctions_);
    owner->body->accept(&v);
  }
}

FnSymbol* ClosureEnv::owner() const {
  return owner_;
}

bool ClosureEnv::isEmpty() const {
  bool ret = numOuterVariables() == 0;
  return ret;
}

int ClosureEnv::numOuterVariables() const {
  auto ret = ((int) outerVariables_.size());
  return ret;
}

Symbol* ClosureEnv::outerVariable(int idx) const {
  INT_ASSERT(0 <= idx && idx < numOuterVariables());
  auto ret = outerVariables_[idx];
  return ret;
}

int ClosureEnv::numMentions(Symbol* sym) const {
  auto it = outerVariableToMentions_.find(sym);
  if (it == outerVariableToMentions_.end()) return 0;
  auto ret = ((int) it->second.size());
  return ret;
}

SymExpr* ClosureEnv::firstMention(Symbol* sym) const {
  INT_ASSERT(1 <= numMentions(sym));
  auto ret = mention(sym, 0);
  return ret;
}

SymExpr* ClosureEnv::mention(Symbol* sym, int idx) const {
  auto it = outerVariableToMentions_.find(sym);
  if (it == outerVariableToMentions_.end()) return nullptr;
  int hi = ((int) it->second.size());
  INT_ASSERT(0 <= idx && idx < hi);
  auto ret = it->second[idx];
  return ret;
}

const std::vector<FnSymbol*>& ClosureEnv::childFunctions() const {
  return childFunctions_;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool
readConfigParamBool(ModuleSymbol* modSym, const char* configParamName,
                    VarSymbol*& cachedValue) {

  // TODO: This is O(n) number of params, we need a better way to look
  // things up after resolve. I think that 'dyno' can always do the
  // elegant thing here. Just preserve the ID for 'ChapelBase', and then
  // use it in conjunction with a lookup for the config name. You fetch
  // the 'ResolvedExpression' and you're done.
  if (!cachedValue) {
    if (!modSym->initFn || !modSym->initFn->isResolved()) {
      INT_FATAL(modSym, "Called before '%s' is resolved",
                        modSym->name);
    }

    form_Map(SymbolMapElem, e, paramMap) {
      auto sym = e->key;
      if (sym->defPoint && sym->defPoint->getModule() == modSym) {
        if (!strcmp(sym->name, configParamName)) {
          auto vs = toVarSymbol(e->value);
          if (!vs || (vs != gTrue && vs != gFalse)) {
            INT_FATAL("Unexpected config param type or bad AST");
            return false;
          }
          cachedValue = vs;
          break;
        }
      }
    }

    // Provide a hint, just in case.
    if (!cachedValue) {
      INT_FATAL("Could not find '%s', is it declared in '%s'?",
                configParamName,
                modSym->name);
    }
  }

  bool ret = (cachedValue == gTrue);
  return ret;
}

bool useLegacyBehavior(void) {
  static VarSymbol* cachedValue = nullptr;
  return readConfigParamBool(baseModule, "fcfsUseLegacyBehavior",
                             cachedValue);
}

bool usePointerImplementation(void) {
  static VarSymbol* cachedValue = nullptr;
  return readConfigParamBool(baseModule, "fcfsUsePointerImplementation",
                             cachedValue);
}

Expr* createFunctionClassInstance(FnSymbol* fn, Expr* use) {
  if (usePointerImplementation()) INT_FATAL(use, "Should not be called!");
  auto ret = createLegacyClassInstance(fn, use);
  use->replace(ret);
  normalize(ret);
  return ret;
}

Type* functionClassSuperTypeFromFunctionType(FunctionType* ft) {
  std::vector<FcfFormalInfo> formals;
  RetTag retTag = ft->returnIntent();
  Type* retType = ft->returnType();
  bool throws = ft->throws();

  // Build up the formal types and intents.
  for (int i = 0; i < ft->numFormals(); i++) {
    auto formal = ft->formal(i);
    FcfFormalInfo info;
    info.type = formal->type;
    info.intent = formal->intent;
    info.name = formal->name;
    formals.push_back(std::move(info));
  }

  auto info = buildWrapperSuperTypeAtProgram(formals, retTag, retType,
                                             throws);
  auto ret = info->sharedType;
  return ret;

}

Type* functionClassSuperTypeForFuncConstructor(CallExpr* call) {
  INT_ASSERT(call && call->isPrimitive(PRIM_CREATE_FN_TYPE));

  AList& argList = call->argList;
  SymExpr* retTail = toSymExpr(argList.tail);

  // These are the pieces the are required to build the super type.
  std::vector<FcfFormalInfo> formals;
  RetTag retTag = RET_VALUE;
  Type* retType = retTail->symbol()->type;
  bool throws = false;

  // Build up the formal types and intents. This cannot call the helper
  // 'extractFormalInfo' because the expressions in the list are not
  // formals - they are type symbols. The formals of the function type that
  // is constructed by a call to 'func()' all have the blank intent, and
  // the function type always returns by value.
  for_alist(expr, argList) {
    if (expr == argList.tail) break;

    auto se = toSymExpr(expr);
    INT_ASSERT(se);
    FcfFormalInfo info;
    info.type = se->symbol()->type;
    info.intent = INTENT_BLANK;
    info.name = nullptr;
    formals.push_back(std::move(info));
  }

  auto info = buildWrapperSuperTypeAtProgram(formals, retTag, retType,
                                             throws);
  auto ret = info->sharedType;
  return ret;
}

static bool isAnyErrorSinkType(Type* t) {
  return (t == errorSink(FunctionType::PROC)->type  ||
          t == errorSink(FunctionType::ITER)->type  ||
          t == errorSink(FunctionType::OPERATOR)->type);
}

const char* functionClassTypeToString(Type* t) {
  INT_ASSERT(t && t->symbol->hasFlag(FLAG_FUNCTION_CLASS));

  auto at = toAggregateType(t);

  if (isAnyErrorSinkType(at)) {
    if (t == errorSink(FunctionType::PROC)->type) return "proc";
    if (t == errorSink(FunctionType::ITER)->type) return "iter";
    if (t == errorSink(FunctionType::OPERATOR)->type) return "operator";
    INT_FATAL("Not possible!");
  }

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

bool checkAndResolveSignature(FnSymbol* fn, Expr* use) {
  std::ignore = fn->tagIfGeneric();

  if (fn->isResolved()) return false;

  resolveSignature(fn);

  // This just checks that the current tagging is valid.
  INT_ASSERT(fn->isGenericIsValid());

  // Now resolve the return type (and possibly the body).
  if (fn->retType == dtUnknown) {
    if (fn->retExprType) {
      resolveSpecifiedReturnType(fn);
      return false;

    // We are a function type with no return type, so we return void.
    } else if (fn->hasFlag(FLAG_NO_FN_BODY)) {
      fn->retType = dtVoid;

    // Only try to resolve the body if we can succeed.
    } else if (!fn->isResolved() && !fn->isGeneric()) {
      resolveFunction(fn);
      INT_ASSERT(fn->isResolved());
      return true;
    }
  }

  return false;
}

void checkAndResolveSignatureAndBody(FnSymbol* fn, Expr* use) {
  if (payloadToResolved.find(fn) != payloadToResolved.end()) return;

  // TODO: What is the best way to detect if resolution failed?
  bool done = fn->isResolved() || checkAndResolveSignature(fn, use);

  if (!done) {
    INT_ASSERT(!fn->isResolved());
    if (!fn->isGeneric() && !fn->hasFlag(FLAG_NO_FN_BODY)) {
      resolveFunction(fn);
      done = fn->isResolved();
    }
  }

  payloadToResolved.insert(std::make_pair(fn, done));

  return;
}

std::vector<FnSymbol*> lookupFunctions(const char* name, Expr* use) {
  std::vector<FnSymbol*> ret;
  Vec<FnSymbol*> visibleFns;

  // Recompute manually because this call may be in an instantiation.
  recomputeVisibleFunctions();

  // Look up visible functions with a temporary call.
  auto call = new CallExpr(name);
  use->replace(call);
  getVisibleFunctions(name, call, visibleFns);
  call->replace(use);

  for (int i = 0; i < visibleFns.n; i++) {
    ret.push_back(visibleFns.v[i]);
  }

  return ret;
}

static std::map<FnSymbol*, ClosureEnv> fnToEnv;

const ClosureEnv& computeOuterVariables(FnSymbol* fn) {
  auto it = fnToEnv.find(fn);
  if (it != fnToEnv.end()) return it->second;
  auto inserted = fnToEnv.emplace_hint(it, fn, ClosureEnv(fn));
  return inserted->second;
}

VarSymbol* errorSink(FunctionType::Kind kind) {
  static VarSymbol* procError = nullptr;
  static VarSymbol* iterError = nullptr;
  static VarSymbol* operatorError = nullptr;

  VarSymbol** create = nullptr;

  switch (kind) {
    case FunctionType::PROC: create = &procError; break;
    case FunctionType::ITER: create = &iterError; break;
    case FunctionType::OPERATOR: create = &operatorError; break;
  }

  if (*create) return *create;

  auto name = astr("chpl_", FunctionType::kindToString(kind), "_error");
  auto super = insertFcfWrapperSuperTypeAtProgram(name);
  VarSymbol* ret = newTemp(super);
  theProgram->block->body.insertAtTail(new DefExpr(ret));
  *create = ret;

  return ret;
}

} // end namespace 'fcfs'
