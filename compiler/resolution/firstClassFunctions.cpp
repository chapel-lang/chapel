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

using FunctionTypeAndThis = std::pair<AggregateType*, FnSymbol*>;
using FunctionTypeMap = std::map<std::string, FunctionTypeAndThis>;

static FunctionTypeMap functionTypeMap;
static std::map<FnSymbol*, FnSymbol*> functionCaptureMap;
static VarSymbol* dummyFcfError = nullptr;
static int uniqueFcfId = 0;
static std::map<AggregateType*, Type*> sharedWrapperTypes;

/************************************* | *************************************
*                                                                            *
*                                                                            *
*                                                                            *
************************************** | ************************************/

static VarSymbol* getDummyFcfErrorInsertedAtProgram(CallExpr* call);

static CallExpr*
produceCallToCachedFcfWrapper(CallExpr* call, FnSymbol* capturedFn);

static std::string buildParentName(AList& argList, bool isForProcExpr,
                                   Type* retType,
                                   bool throws);

static AggregateType*
insertParentFcfWrapperTypeAtProgram(CallExpr* call, const char* name);

static void
attachFormalsForFuncFunction(AList& argList, FnSymbol* parentMethod);

static void
insertOldStyleRetTypeGetter(AggregateType* parent, Type* retType);

static void
insertOldStyleArgTypeTupleGetter(AggregateType* parent, AList& argList,
                                 bool isForProcExpr);

static FnSymbol* insertParentThisAtProgram(CallExpr* call,
                                              AggregateType* parent,
                                              AList& argList,
                                              bool isFormal,
                                              RetTag retTag,
                                              Type* retType,
                                              bool throws);

static Type* getSharedWrapperType(AggregateType* parent);

/************************************* | *************************************
*                                                                            *
*                                                                            *
*                                                                            *
************************************** | ************************************/

static VarSymbol* getDummyFcfErrorInsertedAtProgram(CallExpr* call) {
  if (dummyFcfError != nullptr) return dummyFcfError;
  auto parent = insertParentFcfWrapperTypeAtProgram(call, "_fcf_error");
  dummyFcfError = newTemp(parent);
  theProgram->block->body.insertAtTail(new DefExpr(dummyFcfError));
  return dummyFcfError;
}

static CallExpr*
produceCallToCachedFcfWrapper(CallExpr* call, FnSymbol* capturedFn) {
  if (call->isPrimitive(PRIM_CAPTURE_FN_FOR_CHPL)) {
    if (functionCaptureMap.find(capturedFn) != functionCaptureMap.end()) {
      return new CallExpr(functionCaptureMap[capturedFn]);
    }
  }

  return nullptr;
}

static Type* getSharedWrapperType(AggregateType* parent) {
  Type* result = NULL;

  if (sharedWrapperTypes.find(parent) != sharedWrapperTypes.end()) {
    result = sharedWrapperTypes[parent];
  } else {
    CallExpr* getParShared = new CallExpr(dtShared->symbol, parent->symbol);
    chpl_gen_main->insertAtHead(getParShared);
    tryResolveCall(getParShared);
    getParShared->remove();

    result = getParShared->typeInfo();
    result->symbol->addFlag(FLAG_FUNCTION_CLASS);

    sharedWrapperTypes[parent] = result;
  }

  INT_ASSERT(result != NULL);

  return result;
}

Expr* fcfWrapperInstanceFromPrimCall(CallExpr *call) {
  INT_ASSERT(call && call->isPrimitive());
  INT_ASSERT(call->isPrimitive(PRIM_CAPTURE_FN_FOR_CHPL) ||
             call->isPrimitive(PRIM_CAPTURE_FN_FOR_C));

  UnresolvedSymExpr* use = toUnresolvedSymExpr(call->get(1));
  const char* flname = use->unresolved;

  Vec<FnSymbol*> visibleFns;

  // Recompute manually because this call may be in an instantiation.
  recomputeVisibleFunctions();
  getVisibleFunctions(flname, call, visibleFns);

  if (visibleFns.n > 1) {
    USR_FATAL(call, "cannot capture overloaded function '%s'", flname);
  }

  // TODO: Lambdas should always be visible here.
  if (visibleFns.n == 0) {
    USR_FATAL(call, "no function with name '%s' is visible", flname);
  }

  FnSymbol* capturedFn = visibleFns.head();

  // Reuse cached FCF wrapper if it already exists.
  if (auto ret = produceCallToCachedFcfWrapper(call, capturedFn)) {
    return ret;
  }

  resolveSignature(capturedFn);

  bool isInvalidSignature = false;
  for_formals(formal, capturedFn) {
    isInvalidSignature |= formal->type->symbol->hasFlag(FLAG_GENERIC);
    if (isInvalidSignature) break;
  }

  if (isInvalidSignature) {
    USR_FATAL_CONT(call, "'%s' cannot be captured as a value because "
                         "it is a generic function",
                         capturedFn->name);
    auto dummy = getDummyFcfErrorInsertedAtProgram(call);
    return new SymExpr(dummy);
  }

  resolveFnForCall(capturedFn, call);

  // When all we need is a C pointer, we can cut out here, returning
  // a reference to the function symbol.
  // TODO: Support this behavior when passing to extern functions.
  if (call->isPrimitive(PRIM_CAPTURE_FN_FOR_C)) {
    return new SymExpr(capturedFn);
  }

  // TODO: Move me to dyno.
  if (fWarnUnstable) {
    USR_WARN(call, "First class functions are unstable.");
  }

  auto parentName = buildParentName(capturedFn->formals, true,
                                    capturedFn->retType,
                                    capturedFn->throwsError());
  AggregateType* parent = nullptr;
  FnSymbol* thisParentMethod = nullptr;

  if (functionTypeMap.find(parentName) != functionTypeMap.end()) {
    auto cached = functionTypeMap[parentName];
    parent = cached.first;
    thisParentMethod = cached.second;
  } else {
    parent = insertParentFcfWrapperTypeAtProgram(call, parentName.c_str());
    thisParentMethod = insertParentThisAtProgram(call, parent,
        capturedFn->formals, true, capturedFn->retTag, capturedFn->retType,
        capturedFn->throwsError());
    auto ins = std::make_pair(parent, thisParentMethod);
    functionTypeMap[parentName] = ins;
  }

  AggregateType *ct = new AggregateType(AGGREGATE_CLASS);
  std::ostringstream fcfName;

  fcfName << "_chpl_fcf_" << uniqueFcfId++ << "_" << flname;

  TypeSymbol *ts = new TypeSymbol(astr(fcfName.str().c_str()), ct);

  // Wrapper class has the same visibility as the underlying function.
  capturedFn->defPoint->insertBefore(new DefExpr(ts));

  ct->dispatchParents.add(parent);

  bool inserted = parent->dispatchChildren.add_exclusive(ct);
  INT_ASSERT(inserted);

  VarSymbol* super = new VarSymbol("super", parent);
  super->addFlag(FLAG_SUPER_CLASS);

  ct->fields.insertAtHead(new DefExpr(super));
  ct->processGenericFields();
  ct->buildDefaultInitializer();

  buildDefaultDestructor(ct);

  FnSymbol*  thisMethod = new FnSymbol("this");
  ArgSymbol* thisSymbol = new ArgSymbol(INTENT_BLANK, "this", ct);

  thisMethod->addFlag(FLAG_FIRST_CLASS_FUNCTION_INVOCATION);
  thisMethod->addFlag(FLAG_COMPILER_GENERATED);
  thisMethod->addFlag(FLAG_OVERRIDE);

  auto mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
  thisMethod->insertFormalAtTail(mt);

  thisMethod->setMethod(true);
  thisMethod->insertFormalAtTail(thisSymbol);
  thisMethod->_this = thisSymbol;
  thisSymbol->addFlag(FLAG_ARG_THIS);
  thisMethod->retTag = capturedFn->retTag;

  CallExpr* innerCall = new CallExpr(capturedFn);
  int formalIdx = 0;

  for_alist(formalExpr, thisParentMethod->formals) {
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

  if (capturedFn->throwsError()) {
    thisMethod->throwsErrorInit();
  }

  // (Seen note above)
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
  CallExpr* init = new CallExpr(PRIM_NEW, usym,
                                new CallExpr(new SymExpr(undecorated->symbol)));

  // Cast to "unmanaged parent".
  Type* parUnmanaged = getDecoratedClass(parent,
      ClassTypeDecorator::UNMANAGED_NONNIL);
  CallExpr* parCast = new CallExpr(PRIM_CAST, parUnmanaged->symbol,
                                   init);

  // Get a handle to the type "_shared(parent)".
  Type* parShared = getSharedWrapperType(parent);

  // Create a new "shared parent" temporary.
  VarSymbol* temp = newTemp("retval", parShared);
  block->insertAtTail(new DefExpr(temp));

  // Initialize the temporary with the result of the cast.
  CallExpr* initTemp = new CallExpr("init", gMethodToken, temp, parCast);
  block->insertAtTail(initTemp);

  // Return the "shared parent" temporary.
  CallExpr* ret = new CallExpr(PRIM_RETURN, temp);
  block->insertAtTail(ret);

  normalize(wrapper);
  wrapper->setGeneric(false);

  CallExpr* callWrapper = new CallExpr(wrapper);

  functionCaptureMap[capturedFn] = wrapper;

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
    fn->insertAtTail(new CallExpr(new CallExpr(".", fileArg,
                                               new_StringSymbol("writeIt")),
                                  new_StringSymbol(astr(flname, "()"))));
    normalize(fn);
  }

  return callWrapper;
}

Type* fcfWrapperTypeFromFuncFunction(AList& argList, CallExpr* call) {
  AggregateType* parent      = NULL;
  SymExpr*       retTail     = toSymExpr(argList.tail);
  Type*          retType     = retTail->symbol()->type;
  bool           throws      = false; // TODO: how to distinguish?
  std::string    parent_name = buildParentName(argList, false, retType, throws);

  if (functionTypeMap.find(parent_name) != functionTypeMap.end()) {
    parent = functionTypeMap[parent_name].first;

  } else {
    FnSymbol* parentMethod = NULL;

    parent       = insertParentFcfWrapperTypeAtProgram(call, parent_name.c_str());
    parentMethod = insertParentThisAtProgram(call,
                                                  parent,
                                                  argList,
                                                  false,
                                                  RET_VALUE,
                                                  retType,
                                                  throws);

    functionTypeMap[parent_name] = std::pair<AggregateType*,
                                             FnSymbol*>(parent, parentMethod);
  }

  Type* result = getSharedWrapperType(parent);

  return result;
}

Type* fcfWrapperTypeFromFnType(FnSymbol* fnType) {
  INT_FATAL("Not implemented yet!");
  return nullptr;  
}

/*
  Builds up the name of the parent for lookup by looking through the types
  of the arguments, either formal or actual
*/
static std::string buildParentName(AList& arg_list,
                                   bool   isFormal,
                                   Type*  retType,
                                   bool throws) {
  std::ostringstream oss;
  bool               isFirst = true;

  oss << "chpl__fcf_type_";

  if (isFormal) {
    if (arg_list.length == 0) {
      oss << "void";

    } else {
      for_alist(formalExpr, arg_list) {
        DefExpr* dExp = toDefExpr(formalExpr);
        ArgSymbol* fArg = toArgSymbol(dExp->sym);

        if (!isFirst)
          oss << "_";

        oss << fArg->type->symbol->cname;

        isFirst = false;
      }
    }
    oss << "_";
    oss << retType->symbol->cname;

  } else {
    int i = 0, alength = arg_list.length;

    if (alength == 1) {
      oss << "void_";
    }

    for_alist(actualExpr, arg_list) {
      if (!isFirst)
        oss << "_";

      SymExpr* sExpr = toSymExpr(actualExpr);

      ++i;

      oss << sExpr->symbol()->type->symbol->cname;

      isFirst = false;
    }
  }

  if (throws)
    oss << "_throws";

  return oss.str();
}

static AggregateType*
insertParentFcfWrapperTypeAtProgram(CallExpr* call, const char* name) {
  AggregateType* parent   = new AggregateType(AGGREGATE_CLASS);
  TypeSymbol* parentTs = new TypeSymbol(name, parent);

  parentTs->addFlag(FLAG_FUNCTION_CLASS);

  // Because the general function type is potentially usable by other modules,
  // insert it into ChapelBase.
  baseModule->block->insertAtHead(new DefExpr(parentTs));

  parent->dispatchParents.add(dtObject);

  dtObject->dispatchChildren.add(parent);

  VarSymbol* parentSuper = new VarSymbol("super", dtObject);

  parentSuper->addFlag(FLAG_SUPER_CLASS);

  parent->fields.insertAtHead(new DefExpr(parentSuper));

  parent->processGenericFields();

  parent->buildDefaultInitializer();

  buildDefaultDestructor(parent);

  return parent;
}

static void
attachFormalsForFuncFunction(AList& argList, FnSymbol* parentMethod) {
  char nameBuffer[100];
  int  nameIndex = 0;
  int i = 0;
  int alength = argList.length;

  for_alist(actualExpr, argList) {
    sprintf(nameBuffer, "name%i", nameIndex++);

    if (i != (alength-1)) {
      auto seActual = toSymExpr(actualExpr);
      INT_ASSERT(seActual);
      if (seActual->symbol()->type == dtNothing) continue;
      auto newFormal = new ArgSymbol(INTENT_BLANK,
                                     nameBuffer,
                                     seActual->symbol()->type);
      parentMethod->insertFormalAtTail(newFormal);
    }
    ++i;
  }
}

static void
insertOldStyleRetTypeGetter(AggregateType* parent, Type* retType) {
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
  get->cname = astr("chpl_get_",
                    parent->symbol->cname,
                    "_",
                    get->cname);
  get->addFlag(FLAG_NO_PARENS);
  get->_this = _this;
}

static void
insertOldStyleArgTypeTupleGetter(AggregateType* parent, AList& argList,
                                    bool isForProcExpr) {
  FnSymbol* get = new FnSymbol("argTypes");
  get->addFlag(FLAG_NO_IMPLICIT_COPY);
  get->addFlag(FLAG_INLINE);
  get->addFlag(FLAG_COMPILER_GENERATED);
  get->retTag = RET_TYPE;
  get->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                        "_mt",
                                        dtMethodToken));

  CallExpr* expr = new CallExpr(PRIM_ACTUALS_LIST);
  if (isForProcExpr) {
    for_alist(formalExpr, argList) {
      DefExpr* dExp = toDefExpr(formalExpr);
      ArgSymbol* fArg = toArgSymbol(dExp->sym);
      expr->insertAtTail(fArg->type->symbol);
    }
  } else {
    for_alist(actualExpr, argList) {
      if (actualExpr != argList.tail) {
        SymExpr* sExpr = toSymExpr(actualExpr);
        expr->insertAtTail(sExpr->symbol()->type->symbol);
      }
    }
  }

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
  get->cname = astr("chpl_get_",
                    parent->symbol->cname, "_",
                    get->cname);
  get->addFlag(FLAG_NO_PARENS);
  get->_this = _this;
}

static FnSymbol*
insertParentThisAtProgram(CallExpr* call, AggregateType* parent,
                          AList& argList,
                          bool isForProcExpr,
                          RetTag retTag,
                          Type* retType,
                          bool throws) {
  insertOldStyleRetTypeGetter(parent, retType);
  insertOldStyleArgTypeTupleGetter(parent, argList, isForProcExpr);

  FnSymbol* ret = new FnSymbol("this");
  ret->retTag = retTag;
  ret->addFlag(FLAG_FIRST_CLASS_FUNCTION_INVOCATION);
  ret->addFlag(FLAG_COMPILER_GENERATED);
  ret->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                                  "_mt",
                                                  dtMethodToken));
  ret->setMethod(true);
  ArgSymbol* thisParentSymbol = new ArgSymbol(INTENT_BLANK, "this", parent);
  thisParentSymbol->addFlag(FLAG_ARG_THIS);
  ret->insertFormalAtTail(thisParentSymbol);
  ret->_this = thisParentSymbol;

  // We handle the arg list differently depending on if it's a list of
  // formal args or actual args
  if (isForProcExpr) {
    for_alist(formalExpr, argList) {
      auto oldFormalDefExpr = toDefExpr(formalExpr);
      auto oldFormal = toArgSymbol(oldFormalDefExpr->sym);
      if (oldFormal->type == dtNothing) continue;
      if (oldFormal->type != dtNothing) {
        auto newFormal = oldFormal->copy();
        ret->insertFormalAtTail(newFormal);
      }
    }
  } else {
    attachFormalsForFuncFunction(argList, ret);
  }

  if (retType != dtVoid) {
    VarSymbol *tmp = newTemp("_return_tmp_", retType);
    ret->insertAtTail(new DefExpr(tmp));
    ret->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
  }

  if (throws) {
    ret->throwsErrorInit();
  }

  baseModule->block->insertAtHead(new DefExpr(ret));
  normalize(ret);
  parent->methods.add(ret);

  return ret;
}

