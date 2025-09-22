/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#include "initializerResolution.h"

#include "AggregateType.h"
#include "caches.h"
#include "callInfo.h"
#include "CatchStmt.h"
#include "DecoratedClassType.h"
#include "driver.h"
#include "expandVarArgs.h"
#include "expr.h"
#include "initializerRules.h"
#include "optimizations.h"
#include "passes.h"
#include "resolution.h"
#include "ResolutionCandidate.h"
#include "resolveFunction.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "TryStmt.h"
#include "view.h"
#include "visibleFunctions.h"
#include "wellknown.h"
#include "wrappers.h"
#include <llvm/ADT/SmallVector.h>

static void resolveInitCall(CallExpr* call, bool emitCallResolutionErrors,
                            AggregateType* newExprAlias = NULL,
                            bool forNewExpr = false);

static void gatherInitCandidates(CallInfo&                  info,
                                 Vec<FnSymbol*>&            visibleFns,
                                 Vec<ResolutionCandidate*>& candidates);

static void resolveInitializerMatch(FnSymbol* fn);

static void makeRecordInitWrappers(CallExpr* call);

static void makeActualsVector(const CallInfo&          info,
                              llvm::SmallVectorImpl<ArgSymbol*>& actualIdxToFormal);

static AggregateType* resolveNewFindType(CallExpr* newExpr);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

FnSymbol*
resolveInitializer(CallExpr* call, bool emitCallResolutionErrors) {
  FnSymbol* retval = NULL;

  callStack.add(call);

  resolveInitCall(call, emitCallResolutionErrors);

  // call->isResolved() is sometimes false on this.init() calls for generic
  // records, as it might be a partial call that needs to get adjusted in order
  // to resolve
  if (call->isResolved()) {

    resolveInitializerMatch(call->resolvedFunction());

    if (isGenericRecord(call->get(2)->typeInfo()->getValType())) {
      SymExpr* namedSe = NULL;

      // There are two cases for generic records
      if (NamedExpr* named = toNamedExpr(call->get(2))) {
        // Case 1) this is the outermost init call
        // This means that the second argument to the call will be named
        namedSe = toSymExpr(named->actual);

      } else if (isSymExpr(call->get(2))) {
        // Case 2) this is a this.init() call in the initializer.
        // This means that the second argument to the call will not be named.
        namedSe = toSymExpr(call->get(2));
      }

      Symbol* sym = namedSe->symbol();

      sym->type = call->resolvedFunction()->_this->type;

      if (sym->hasFlag(FLAG_DELAY_GENERIC_EXPANSION) == true) {
        sym->removeFlag(FLAG_DELAY_GENERIC_EXPANSION);
      }

      makeRecordInitWrappers(call);
    }

    retval = call->resolvedFunction();
  }

  callStack.pop();

  return retval;
}

/* This is a helper to buildNewWrapper() that recursively takes care
   of de-initializing fields in the event that an 'init()' or
   'postinit()' throws */
static void helpDeinitFields(AggregateType* type, VarSymbol* _this,
                             BlockStmt* body) {
  if (type == NULL) {
    INT_FATAL("helpDeinitFields() isn't designed to take 'NULL' types");
  }

  for_fields(field, type) {
    if (isRecord(field->type) && !field->hasFlag(FLAG_TYPE_VARIABLE)) {
      body->insertAtHead(new CallExpr("deinit", gMethodToken,
                           new CallExpr(PRIM_GET_MEMBER, _this, field)));
    } else if (field->hasFlag(FLAG_SUPER_CLASS) && field->type != dtObject) {
      // explicitly recurse over fields rather than calling super->deinit
      // to avoid re-deinit'ing 'this'
      helpDeinitFields(toAggregateType(field->type), _this, body);
    }
  }
}

// This is a map from the original initializer to the new wrapper
// The map is keyed by the FnSymbol of the original initializer and the expr of
//   the allocator (if any)
// The value is the '_new' wrapped initializer
static std::map<std::pair<FnSymbol*,Expr*>,FnSymbol*> newWrapperMap;

// Note: The wrapper for classes always returns unmanaged
// Note: A wrapper might be generated for records in the case of promotion
static FnSymbol* buildNewWrapper(FnSymbol* initFn, Expr* allocator = nullptr) {
  SET_LINENO(initFn);

  // TODO: allocator needs to be threaded through as a formal and an actual
  AggregateType* type = toAggregateType(initFn->_this->getValType());
  if (newWrapperMap.find({initFn, allocator}) != newWrapperMap.end()) {
    // TODO: this should use a ptr to a symbol, not a Expr
    // the Expr will either be NULL or always a new pointer
    return newWrapperMap[std::make_pair(initFn, allocator)];
  }

  FnSymbol* fn = new FnSymbol(astrNew);
  BlockStmt* body = fn->body;
  VarSymbol* initTemp = newTemp("initTemp", type);
  CallExpr* innerInit = new CallExpr(initFn, gMethodToken, initTemp);
  ArgSymbol* chpl_t = new ArgSymbol(INTENT_BLANK, "chpl_t", type);

  chpl_t->addFlag(FLAG_TYPE_VARIABLE);

  fn->addFlag(FLAG_NEW_WRAPPER);
  fn->addFlag(FLAG_COMPILER_GENERATED);
  fn->addFlag(FLAG_LAST_RESORT);
  fn->addFlag(FLAG_INSERT_LINE_FILE_INFO);
  fn->addFlag(FLAG_ALWAYS_PROPAGATE_LINE_FILE_INFO);
  fn->addFlag(FLAG_LLVM_RETURN_NOALIAS);

  if (initFn->hasFlag(FLAG_SUPPRESS_LVALUE_ERRORS)) {
    fn->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);
  }

  fn->insertFormalAtTail(chpl_t);

  SymbolMap initToNewMap;
  ArgSymbol* allocatorFormal = nullptr;
  if (isClass(type) && allocator != nullptr) {
    allocatorFormal = new ArgSymbol(INTENT_BLANK, "allocator_formal", allocator->typeInfo());
    fn->insertFormalAtTail(allocatorFormal);
  }
  for_formals(formal, initFn) {
    if (formal != initFn->_this && formal->type != dtMethodToken) {
      ArgSymbol* newArg = formal->copy();
      initToNewMap.put(formal, newArg);
      fn->insertFormalAtTail(newArg);

      if (newArg->variableExpr != NULL) {
        innerInit->insertAtTail(new CallExpr(PRIM_TUPLE_EXPAND, newArg));
      } else {
        innerInit->insertAtTail(new SymExpr(newArg));
      }

      if (newArg->hasFlag(FLAG_INSTANTIATED_PARAM)) {
        paramMap.put(newArg, paramMap.get(formal));
      }
    }
  }

  update_symbols(fn, &initToNewMap);

  body->insertAtTail(new DefExpr(initTemp));
  if (isClass(type)) {
    if (allocatorFormal != nullptr) {
      body->insertAtTail(new CallExpr(PRIM_MOVE, initTemp, callChplHereAllocWithAllocator(type, new SymExpr(allocatorFormal))));
    } else {
      body->insertAtTail(new CallExpr(PRIM_MOVE, initTemp, callChplHereAlloc(type)));
    }
  }

  // If either the initializer throws, or the postinit throws, make
  // this function representing the 'new' throw as well
  if (initFn->throwsError() ||
      (type->hasPostInitializer() && type->postinit->throwsError())) {
    fn->throwsErrorInit();
    BlockStmt* tryBody = new BlockStmt(innerInit);
    if (type->hasPostInitializer() == true) {
      tryBody->insertAtTail(new CallExpr("postinit", gMethodToken, initTemp));
    }

    const char* errorName = astr("e");
    BlockStmt* catchBody = new BlockStmt(callChplHereFree(initTemp));
    VarSymbol* error = new VarSymbol(errorName);
    DefExpr* errorDef = new DefExpr(error);

    // recreate body of CatchStmt::cleanup() so that it can be well formed (for
    // now)
    VarSymbol* casted = newTemp();
    Expr* castedCurrent = new CallExpr(PRIM_CURRENT_ERROR);
    DefExpr* castedDef = new DefExpr(casted, castedCurrent);
    Expr* nonNilC = new CallExpr(PRIM_TO_NON_NILABLE_CLASS, casted);
    Expr* toOwned = new CallExpr(PRIM_NEW,
                                 new CallExpr(new SymExpr(dtOwned->symbol),
                                              nonNilC));

    // deinitialize fields, including the 'super' field / parent class
    helpDeinitFields(type, initTemp, catchBody);

    errorDef->init = toOwned;
    catchBody->insertAtHead(errorDef);
    catchBody->insertAtHead(castedDef);

    CallExpr* rethrow = new CallExpr(PRIM_THROW,
                                     new SymExpr(error));
    catchBody->insertAtTail(rethrow);

    TryStmt* tryInit = new TryStmt(false, tryBody,
                                   new BlockStmt(CatchStmt::build(errorName,
                                                                  catchBody)));
    body->insertAtTail(tryInit);

  } else {
    body->insertAtTail(innerInit);
    if (type->hasPostInitializer() == true) {
      body->insertAtTail(new CallExpr("postinit", gMethodToken, initTemp));
    }
  }


  VarSymbol* result = newTemp();
  Expr* resultExpr = NULL;
  if (isClass(type)) {
    Type* uct = type->getDecoratedClass(ClassTypeDecorator::UNMANAGED_NONNIL);
    resultExpr = new CallExpr(PRIM_CAST, uct->symbol, initTemp);
  } else {
    resultExpr = new SymExpr(initTemp);
  }

  CallExpr* finalMove = new CallExpr(PRIM_MOVE, result, resultExpr);
  body->insertAtTail(new DefExpr(result));
  body->insertAtTail(finalMove);

  body->insertAtTail(new CallExpr(PRIM_RETURN, result));

  type->symbol->defPoint->insertBefore(new DefExpr(fn));

  fn->setInstantiationPoint(initFn->instantiationPoint());

  normalize(fn);

  newWrapperMap[std::make_pair(initFn, allocator)] = fn;

  return fn;
}

//
// This function inserts NamedExprs into 'initCall' for each instantiated
// field in 'at'.
//
static void insertNamedInstantiationInfo(CallExpr* newExpr,
                                         CallExpr* initCall,
                                         AggregateType* at) {
  AggregateType* rootType = at->getRootInstantiation();
  if (at != rootType) {
    // Insert super class instantiations first
    if (at->isClass() && at != dtObject && at->dispatchParents.v[0] != dtObject) {
      insertNamedInstantiationInfo(newExpr, initCall, at->dispatchParents.v[0]);
    }

    for_fields(field, at) {
      if (at->symbol->hasFlag(FLAG_GENERIC)) {
        if (field->type == dtUnknown || field->type->symbol->hasFlag(FLAG_GENERIC)) {
          continue;
        }
      }
      if (field->hasFlag(FLAG_TYPE_VARIABLE)) {
        initCall->insertAtTail(new NamedExpr(field->name, new SymExpr(field->type->symbol)));
      } else if (field->hasFlag(FLAG_PARAM)) {
        initCall->insertAtTail(new NamedExpr(field->name, new SymExpr(at->getSubstitution(field->name))));
      } else if (at->getSubstitution(field->name) != NULL) {
        USR_FATAL(newExpr, "A type alias of '%s' may not be used in a new-expression because it contains a typeless field ('%s')", rootType->symbol->name, field->name);
      }
    }
  }
}

//
// Builds and returns a call to an initializer based on the arguments in
// 'newExpr'. The call will be inserted at the end of 'block'. The call and its
// resolved function will both be resolved.
//
// Note: Modifies 'newExpr'
//
static CallExpr* buildInitCall(CallExpr* newExpr,
                               AggregateType* at,
                               BlockStmt* block) {
  AggregateType* rootType = at->getRootInstantiation();

  Expr* modToken = NULL;
  Expr* modValue = NULL;
  if (SymExpr* se = toSymExpr(newExpr->get(1))) {
    if (se->symbol() == gModuleToken) {
      modValue = newExpr->get(2)->remove();
      modToken = newExpr->get(1)->remove();
    }
  }

  newExpr->get(1)->remove();

  VarSymbol* tmp = newTemp("initTemp", rootType);
  CallExpr* call = new CallExpr("init", gMethodToken, new NamedExpr("this", new SymExpr(tmp)));
  call->tryTag = newExpr->tryTag;

  insertNamedInstantiationInfo(newExpr, call, at);

  for (int i = 1; i <= newExpr->numActuals(); i++) {
    call->insertAtTail(newExpr->get(i)->copy());
  }

  if (modToken != NULL) {
    call->insertAtHead(modValue);
    call->insertAtHead(modToken);
  }

  block->insertAtTail(new DefExpr(tmp));
  block->insertAtTail(call);

  if (rootType->isGeneric()) {
    tmp->addFlag(FLAG_DELAY_GENERIC_EXPANSION);
    resolveGenericActuals(call);
  }

  // Find the correct 'init' function without wrapping/promoting
  AggregateType* alias = at == rootType ? NULL : at;
  const bool emitCallResolutionErrors = true;
  resolveInitCall(call, emitCallResolutionErrors, alias, true);
  resolveInitializerMatch(call->resolvedFunction());
  tmp->type = call->resolvedFunction()->_this->getValType();
  resolveTypeWithInitializer(toAggregateType(tmp->type), call->resolvedFunction());

  // Check for arguments where the type is not known.
  // These arguments indicate that something needed to be provided
  // at the initializer call site.
  for_formals(arg, call->resolvedFunction()) {
    if (arg->type == dtUnknown || arg->type == dtTypeDefaultToken)
      USR_FATAL(call, "initialization requires an argument for %s", arg->name);
  }

  // check that the type created by init is compatible with the requested
  // type (e.g. for `type t = C(int); var x = new t(real);`).
  if (isSubtypeOrInstantiation(tmp->type, at, call) == false) {
    USR_FATAL_CONT(call, "initializer produces a different type");
    USR_PRINT(call, "new was provided type '%s'", toString(at));
    USR_PRINT(call, "init resulted in type '%s'", toString(tmp->type));
  }

  // Avoid a potential access of uninitialized memory in the case where the
  // record has a bool field.  C will pad around boolean fields, but that memory
  // cannot normally be updated - without explicitly zero initializing it,
  // valgrind will complain when we use it to create the hash to ensure inferred
  // const ref arguments are not implicitly modified.
  if (isRecord(tmp->type)) {
    if (fWarnUnstable && !fNoConstArgChecks) {
      call->insertBefore(new CallExpr(PRIM_ZERO_VARIABLE, new SymExpr(tmp)));
    }
  }

  return call;
}

// Creates a new temp and stores the DefExpr for it at the end
// of block, or, if in a module init fn, in global scope.
static
VarSymbol* resolveNewInitializerMakeTemp(const char* name, BlockStmt* block) {
  VarSymbol* tmp = newTemp(name);
  tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);

  BlockStmt* inBlock = toBlockStmt(block->parentExpr);
  FnSymbol* inFn = toFnSymbol(inBlock->parentSymbol);
  if (inFn && inFn->hasFlag(FLAG_MODULE_INIT) && inFn->body == inBlock) {
    // make it a global variable
    inFn->defPoint->insertAfter(new DefExpr(tmp));
  } else {
    block->insertAtTail(new DefExpr(tmp));
  }

  return tmp;
}

void resolveNewInitializer(CallExpr* newExpr, Type* manager) {
  // Get root instantiation so we can easily check against e.g. dtOwned
  bool nilable = isNilableClassType(manager);
  if (isManagedPtrType(manager))
    manager = getManagedPtrManagerType(manager);
  else if (manager == dtUnmanagedNilable)
    manager = dtUnmanaged;

  // there is also a post-parse check for this, but it can only catch `new borrowed C()`
  // this also catches `type T = borrowed C; new T()`
  if (manager == dtBorrowed || manager == dtBorrowedNilable) {
    USR_FATAL_CONT(newExpr, "cannot create a 'borrowed' object using 'new'");
  }

  INT_ASSERT(isNewLike(newExpr));
  AggregateType* at = resolveNewFindType(newExpr);

  BlockStmt* block = new BlockStmt(BLOCK_SCOPELESS);
  Expr* stmt = newExpr->getStmtExpr();
  stmt->insertBefore(block);


  // pull out the allocator before `buildInitCall`
  Expr* allocator = nullptr;
  if (newExpr->isPrimitive(PRIM_NEW_WITH_ALLOCATOR)) {
    if (!isClass(at)) {
      USR_FATAL_CONT(newExpr, "cannot use an allocator with non-class types");
    }

    allocator = newExpr->get(1)->copy();
    newExpr->get(1)->remove();
  }
  CallExpr* initCall = buildInitCall(newExpr, at, block);
  FnSymbol* initFn = initCall->resolvedFunction();
  Symbol* initTemp = toSymExpr(toNamedExpr(initCall->get(2))->actual)->symbol();
  AggregateType* initType = toAggregateType(initFn->_this->getValType());

  CallInfo info;
  if (info.isWellFormed(initCall) == false) {
    info.haltNotWellFormed();
  }

  llvm::SmallVector<ArgSymbol*, 8> actualIdxToFormal;

  makeActualsVector(info, actualIdxToFormal);

  if (isClass(at) || isPromotionRequired(initFn, info, actualIdxToFormal)) {
    FnSymbol* newWrapper = buildNewWrapper(initFn, allocator);

    initCall->setResolvedFunction(newWrapper);
    initCall->get(2)->remove(); // 'this'
    initCall->get(1)->remove(); // '_mt'
    if (allocator) {
      initCall->insertAtHead(allocator);
    }
    initCall->insertAtHead(new SymExpr(initType->symbol));
    CallExpr* newCall = toCallExpr(initCall->remove());

    initTemp->defPoint->remove();

    bool getBorrow = false;
    if (manager == dtBorrowed) {
      manager = dtOwned;
      getBorrow = true;
    }

    // If the default value for a formal is a new-expression, the final
    // statement in the BlockStmt will be a PRIM_NEW.
    ArgSymbol *argSym = toArgSymbol(stmt->parentSymbol);
    bool inArgSymbol = stmt == newExpr && argSym != NULL;

    VarSymbol* new_temp = newTemp("new_temp");
    block->insertAtTail(new DefExpr(new_temp));

    if (isRecord(at)) {
      CallExpr* newMove = new CallExpr(PRIM_MOVE, new_temp, newCall);
      block->insertAtTail(newMove);
      newExpr->replace(new SymExpr(new_temp));

    } else if (isManagedPtrType(manager) == false) {
      Expr* new_temp_rhs = newCall;

      if (nilable) {
        // new unmanaged T(...)?
        VarSymbol* tmpM = resolveNewInitializerMakeTemp("new_temp_n", block);

        block->insertAtTail(new CallExpr(PRIM_MOVE, tmpM, new_temp_rhs));
        new_temp_rhs = createCast(tmpM, dtAnyManagementNilable->symbol);
      }

      CallExpr* newMove = new CallExpr(PRIM_MOVE, new_temp, new_temp_rhs);
      block->insertAtTail(newMove);
      newExpr->replace(new SymExpr(new_temp));

    } else {
      CallExpr* newMove = new CallExpr(PRIM_MOVE, new_temp, newCall);
      block->insertAtTail(newMove);

      Expr* new_temp_rhs = new CallExpr(PRIM_NEW, manager->symbol, new_temp);

      if (getBorrow) {
        // (new owned T(...)).borrow()
        VarSymbol* tmpM = resolveNewInitializerMakeTemp("new_temp_m", block);
        VarSymbol* tmpR = resolveNewInitializerMakeTemp("new_temp_r", block);

        block->insertAtTail(new CallExpr(PRIM_INIT_VAR, tmpM, new_temp_rhs));
        block->insertAtTail(new CallExpr(PRIM_MOVE,
                                         tmpR,
                                         new CallExpr("borrow",
                                                      gMethodToken,
                                                      tmpM)));
        new_temp_rhs = new SymExpr(tmpR);
      }

      if (nilable) {
        // new owned T(...)? or new borrowed T()?
        VarSymbol* tmpM = resolveNewInitializerMakeTemp("new_temp_n", block);

        block->insertAtTail(new CallExpr(PRIM_INIT_VAR, tmpM, new_temp_rhs));
        new_temp_rhs = createCast(tmpM, dtAnyManagementNilable->symbol);
      }

      newExpr->replace(new_temp_rhs);
    }

    block->insertAfter(newExpr);
    resolveBlockStmt(block);
    newExpr->convertToNoop();

    if (inArgSymbol) {
      // Need to insert an initCopy for promoted new-expressions in order to
      // turn an iterator record into an array.
      BlockStmt* block = toBlockStmt(newExpr->parentExpr);
      Expr* tail = block->body.tail;
      if (tail->typeInfo()->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
        VarSymbol* ir_temp = newTemp("ir_temp");
        Symbol *definedConst = argSym->hasFlag(FLAG_CONST) ?  gTrue : gFalse;
        CallExpr* tempMove = new CallExpr(PRIM_MOVE, ir_temp,
                                          new CallExpr(astr_initCopy,
                                                       tail->copy(),
                                                       definedConst));
        tail->insertBefore(tempMove);
        normalize(tempMove);
        tail->replace(new SymExpr(ir_temp));
      }
    }
  } else {
    block->insertAtTail(initTemp->defPoint->remove());
    block->insertAtTail(initCall->remove());
    newExpr->replace(new SymExpr(initTemp));

    if (initType->hasPostInitializer()) {
      CallExpr* postinit = new CallExpr("postinit", gMethodToken, initTemp);
      block->insertAtTail(postinit);
    }

    block->insertAfter(newExpr);
    resolveBlockStmt(block);
    newExpr->convertToNoop();
    block->flattenAndRemove();
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void resolveInitCall(CallExpr* call, bool emitCallResolutionErrors,
                            AggregateType* newExprAlias,
                            bool forNewExpr) {
  CallInfo info;

  if (call->id == breakOnResolveID) {
    printf("breaking on resolve call %d:\n", call->id);
    print_view(call);
    debuggerBreakHere();
  }

  if (info.isWellFormed(call) == true) {
    Vec<FnSymbol*>            visibleFns, mostApplicable;
    Vec<ResolutionCandidate*> candidates;
    ResolutionCandidate*      best        = NULL;

    findVisibleFunctionsAllPOIs(info, visibleFns);

    trimVisibleCandidates(info, mostApplicable, visibleFns);

    gatherInitCandidates(info, mostApplicable, candidates);

    explainGatherCandidate(info, candidates);

    best = disambiguateForInit(info, candidates);

    if (best == NULL) {
      if (call->partialTag == false) {
        if (forNewExpr == true) {
          // This exists to enable multiple fatal error messages when an
          // initializer fails to resolve due to nilability errors. If the
          // compiler is able to resolve the initializer call while being
          // more flexible with nilability rules, compilation can continue.
          //
          // TODO: We do not issue an error here because the compiler will
          // later attempt to resolve the initializer call once again, in
          // which case it would issue the same error. Instead, issue no errors
          // in this conditional and let another part of resolution handle that.
          // In the future, the compiler should not be attempting to resolve
          // an already-resolved call.
          bool existingErrors = fatalErrorsEncountered();
          if (newExprAlias != NULL && emitCallResolutionErrors) {
            USR_FATAL_CONT(call, "Unable to resolve new-expression with type alias '%s'", newExprAlias->symbol->name);
          }
          if (!inGenerousResolutionForErrors()) {
            startGenerousResolutionForErrors();
            const bool forNewExpr = false;
            resolveInitCall(call, emitCallResolutionErrors, newExprAlias,
                            forNewExpr);
            FnSymbol* retry = call->resolvedFunction();
            stopGenerousResolutionForErrors();

            if (fIgnoreNilabilityErrors && existingErrors == false && retry)
              clearFatalErrors();
          }
        } else {
          if (emitCallResolutionErrors) {
            if (candidates.n == 0) {
              printResolutionErrorUnresolved(info, mostApplicable);

              USR_STOP();
            } else {
              printResolutionErrorAmbiguous (info, candidates);
            }
          }
        }
      }

    } else {
      instantiateBody(best->fn);

      if (explainCallLine != 0 && explainCallMatch(call) == true &&
          emitCallResolutionErrors) {
        USR_PRINT(best->fn, "best candidate is: %s", toString(best->fn));
      }

      if (call->partialTag == false) {
        SET_LINENO(call);

        call->baseExpr->replace(new SymExpr(best->fn));

        if (emitCallResolutionErrors) {
          checkForStoringIntoTuple(call, best->fn);
          resolveNormalCallCompilerWarningStuff(call, best->fn);
        }
      }
    }

    forv_Vec(ResolutionCandidate*, candidate, candidates) {
      delete candidate;
    }

  } else if (emitCallResolutionErrors) {
    info.haltNotWellFormed();
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void doGatherInitCandidates(CallInfo&                  info,
                                   Vec<FnSymbol*>&            visibleFns,
                                   bool                       generated,
                                   Vec<ResolutionCandidate*>& candidates);

static void filterInitCandidate(CallInfo&                  info,
                                FnSymbol*                  fn,
                                Vec<ResolutionCandidate*>& candidates);

static void gatherInitCandidates(CallInfo&                  info,
                                 Vec<FnSymbol*>&            visibleFns,
                                 Vec<ResolutionCandidate*>& candidates) {
  // Search user-defined (i.e. non-compiler-generated) functions first.
  doGatherInitCandidates(info, visibleFns, false, candidates);

  // If no results, try again with any compiler-generated candidates.
  if (candidates.n == 0) {
    doGatherInitCandidates(info, visibleFns, true, candidates);
  }
}

static void doGatherInitCandidates(CallInfo&                  info,
                                   Vec<FnSymbol*>&            visibleFns,
                                   bool                       lastResort,
                                   Vec<ResolutionCandidate*>& candidates) {
  forv_Vec(FnSymbol, visibleFn, visibleFns) {
    // Only consider functions marked with/without FLAG_LAST_RESORT
    if (visibleFn->hasFlag(FLAG_LAST_RESORT) == lastResort) {

      // Some expressions might resolve to methods without parenthesis.
      // If the call is marked with methodTag, it indicates the called
      // function should be a no-parens function or a type constructor.
      // (a type constructor call without parens uses default arguments)
      if (info.call->methodTag) {
        if (visibleFn->hasFlag(FLAG_NO_PARENS)) {
          // OK

        } else {
          // Skip this candidate
          continue;
        }
      }

      if (fExplainVerbose &&
          ((explainCallLine && explainCallMatch(info.call)) ||
           info.call->id == explainCallID)) {
        USR_PRINT(visibleFn, "Considering function: %s", toString(visibleFn));

        if (info.call->id == breakOnResolveID) {
          debuggerBreakHere();
        }
      }

      filterInitCandidate(info, visibleFn, candidates);
    }
  }
}

/** Tests to see if a function is a candidate for resolving a specific call.
 *  If it is a candidate, we add it to the candidate lists.
 */
static void filterInitCandidate(CallInfo&                  info,
                                FnSymbol*                  fn,
                                Vec<ResolutionCandidate*>& candidates) {
  ResolutionCandidate* candidate = new ResolutionCandidate(fn);
  VisibilityInfo visInfo(info);

  if (candidate->isApplicable(info, &visInfo) == true) {
    candidates.add(candidate);

  } else {
    delete candidate;
  }
}

/************************************* | **************************************
*                                                                             *
* Copied from resolveFunction(FnSymbol* fn) in functionResolution.            *
*                                                                             *
* Removed code for extern functions (since I don't think it will apply),      *
* iterators, type constructors.                                               *
*                                                                             *
************************************** | *************************************/

static void resolveInitializerBody(FnSymbol* fn);

static void resolveInitializerMatch(FnSymbol* fn) {
  if (fn->isResolved() == false) {
    AggregateType* at = toAggregateType(fn->_this->getValType());

    if (fn->id == breakOnResolveID) {
      printf("breaking on resolve fn %s[%d] (%d args)\n",
             fn->name, fn->id, fn->numFormals());
      debuggerBreakHere();
    }

    insertFormalTemps(fn);
    at->setFirstGenericField();
    resolveInitializerBody(fn);

    popInstantiationLimit(fn);
  }
}

static void resolveInitializerBody(FnSymbol* fn) {
  fn->addFlag(FLAG_RESOLVED);

  resolveBlockStmt(fn->body);

  resolveReturnType(fn);

  toAggregateType(fn->_this->type)->initializerResolved = true;

  fixPrimInitsAndAddCasts(fn);

  ensureInMethodList(fn);

  setDefinedConstForFieldsInInitializer(fn);
}

/************************************* | **************************************
*                                                                             *
* This creates wrapper functions for calls to record initializers with        *
* default values, out of order named arguments, etc.  That effort was skipped *
* during the call match stage because the "this" argument to the initializer  *
* was still generic until the body had been resolved.  After we have          *
* determined the concrete type for the "this" argument, then we are capable   *
* of creating valid wrappers.                                                 *
*                                                                             *
* Note that this action is not necessary for class initializers, because      *
* such calls are wrapped by the "_new" function, and appropriate wrappers     *
* will be created for it, so we don't need to wrap the initializer itself.    *
*                                                                             *
************************************** | *************************************/

static void makeRecordInitWrappers(CallExpr* call) {
  CallInfo info;

  if (info.isWellFormed(call) == true) {
    llvm::SmallVector<ArgSymbol*, 8> actualIdxToFormal;
    FnSymbol*               wrap = NULL;

    makeActualsVector(info, actualIdxToFormal);

    wrap = wrapAndCleanUpActuals(call->resolvedFunction(),
                                 info,
                                 actualIdxToFormal,
                                 true);

    call->baseExpr->replace(new SymExpr(wrap));

    resolveFunction(wrap);

  } else {
    info.haltNotWellFormed();
  }
}

// Modified version of computeActualFormalAlignment to only populate the
// actualIdxToFormal Vec.  Substitutes the formalIdxToActual Vec with one
// that stores booleans, because I do still need that information in order to
// correctly populate the actuals
//
// This work was already performed when we found the right resolution candidate
// so the "failure" modes should never get triggered.  The information we need
// was cleaned up, though, so we are just going to recreate the parts we need.
static void makeActualsVector(const CallInfo&          info,
                              llvm::SmallVectorImpl<ArgSymbol*>& actualIdxToFormal) {
  const CallExpr*   call = info.call;
  FnSymbol*         fn   = call->resolvedFunction();
  std::vector<bool> formalIdxToActual;

  for (int i = 0; i < fn->numFormals(); i++) {
    formalIdxToActual.push_back(false);
  }
  for (int i = 0; i < info.actuals.n; i++) {
    actualIdxToFormal.push_back(NULL);
  }

  for (int i = 0; i < info.actuals.n; i++) {
    if (info.actualNames.v[i]) {
      bool match = false;
      int  j     = 0;

      for_formals(formal, fn) {
        if (strcmp(info.actualNames.v[i], formal->name) == 0) {
          match                = true;
          actualIdxToFormal[i] = formal;
          formalIdxToActual[j] = true;
          break;
        }

        j++;
      }

      // Fail if no matching formal is found.
      if (!match) {
        INT_FATAL(call,
                  "Compilation should have already ensured this action "
                  "would be valid");
      }
    }
  }

  // Fill in unmatched formals in sequence with the remaining actuals.
  // Record successful substitutions.
  int        j      = 0;
  ArgSymbol* formal = (fn->numFormals()) ? fn->getFormal(1) : NULL;

  for (int i = 0; i < info.actuals.n; i++) {
    if (info.actualNames.v[i] == NULL) {
      bool match = false;

      while (formal) {
        if (formal->variableExpr) {
          return;
        }

        if (formalIdxToActual[j] == false) {
          match                = true;
          actualIdxToFormal[i] = formal;
          formalIdxToActual[j] = true;
          formal               = next_formal(formal);
          j++;
          break;
        }

        formal = next_formal(formal);
        j++;
      }

      // Fail if there are too many unnamed actuals.
      if (!match && !(fn->isGeneric() && fn->hasFlag(FLAG_INIT_TUPLE))) {
        INT_FATAL(call,
                  "Compilation should have verified this action was valid");
      }
    }
  }

  // Make sure that any remaining formals are matched by name
  // or have a default value.
  while (formal) {
    if (formalIdxToActual[j] == false && !formal->defaultExpr) {
      // Fail if not.
      INT_FATAL(call,
                "Compilation should have verified this action was valid");
    }

    formal = next_formal(formal);

    j++;
  }
}

static AggregateType* resolveNewFindType(CallExpr* newExpr) {
  SymExpr* typeExpr = NULL;

  int type_index = newExpr->isPrimitive(PRIM_NEW) ? 1 : 2;

  // Find the SymExpr for the type.
  //   1) Common case  :- primNew(Type, arg1, ...);
  //   2) Module scope :- primNew(module=, moduleName, Type, arg1, ...);
  //   3) Nested call  :- primNew(Inner(_mt, this), arg1, ...);
  if (SymExpr* se = toSymExpr(newExpr->get(type_index))) {
    if (se->symbol() != gModuleToken) {
      typeExpr = se;

    } else {
      typeExpr = toSymExpr(newExpr->get(type_index+2));
    }

  } else if (CallExpr* partial = toCallExpr(newExpr->get(type_index))) {
    if (SymExpr* se = toSymExpr(partial->baseExpr)) {
      typeExpr = partial->partialTag ? se : NULL;
    }
  }

  Type*    type     = resolveTypeAlias(typeExpr);

  return toAggregateType(type);
}
