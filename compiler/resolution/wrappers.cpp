/*
 * Copyright 2004-2018 Cray Inc.
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

//////////////////////////////////////////////////////////////////////////////
//
// Wrappers are used to lower the Chapel idea of a function call to something
// implementable in C.
//  default wrapper -- supplies a value for every argument in the called
//      function substituting default values for actual arguments that are
//       omitted.
//      (C does not support default values for arguments.)
//
//  reorder named actual arguments to match the order expected by the inner
//      function, i.e. the order of the formals (used to be order wrapper)
//      (C does not support named argument passing.)
//
//  coercion wrapper -- add explicit casts to perform type coercions known only
//      to Chapel.
//      (C does not support base-class coercions, etc.)
//
//  promotion wrapper -- replaces implicit array traversals with explicit
//      array traversals.
//      (C has no notion of scalar operator promotion.)
//
//////////////////////////////////////////////////////////////////////////////

#include "wrappers.h"

#include "astutil.h"
#include "build.h"
#include "caches.h"
#include "callInfo.h"
#include "driver.h"
#include "expr.h"
#include "ForLoop.h"
#include "passes.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "resolveIntents.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "visibleFunctions.h"

#include <map>
#include <utility>

static FnSymbol*  wrapDefaultedFormals(
                               FnSymbol*                fn,
                               CallInfo&                info,
                               std::vector<ArgSymbol*>& actualToFormal);
static void addDefaultsAndReorder(FnSymbol *fn,
                                CallExpr* call,
                                CallInfo* info,
                                std::vector<ArgSymbol*>& actualIdxToFormal,
                                bool resolveNewCode);
static void addDefaultsAndReorder(FnSymbol *fn,
                                CallInfo& info,
                                std::vector<ArgSymbol*>& actualIdxToFormal);



static void       reorderActuals(FnSymbol*                fn,
                                 CallInfo&                info,
                                 std::vector<ArgSymbol*>& actualIdxToFormal);

static void       coerceActuals(FnSymbol* fn,
                                CallInfo& info);

static bool       isPromotionRequired(FnSymbol* fn, CallInfo& info,
                                std::vector<ArgSymbol*>& actualIdxToFormal);

static FnSymbol*  promotionWrap(FnSymbol* fn,
                                CallInfo& info,
                                std::vector<ArgSymbol*>& actualIdxToFormal,
                                bool      buildFastFollowerChecks);

static FnSymbol*  buildEmptyWrapper(FnSymbol* fn);

static ArgSymbol* copyFormalForWrapper(ArgSymbol* formal);

typedef struct DefaultExprFnEntry_s {
  FnSymbol* defaultExprFn;
  std::vector<std::pair<ArgSymbol*,ArgSymbol*> > usedFormals;
} DefaultExprFnEntry;

typedef std::map<ArgSymbol*, DefaultExprFnEntry> formalToDefaultExprEntryMap;
formalToDefaultExprEntryMap formalToDefaultExprEntry;

/************************************* | **************************************
*                                                                             *
* The argument actualIdxToFormals[i] stores, for actual i (counting from 0),  *
* the corresponding formal argument.                                          *
* (This mapping is nontrivial when named arguments are used)                  *
*                                                                             *
************************************** | *************************************/

static bool fnIsDefaultInit(FnSymbol* fn);

FnSymbol* wrapAndCleanUpActuals(FnSymbol*                fn,
                                CallInfo&                info,
                                std::vector<ArgSymbol*>  actualIdxToFormal,
                                bool                     fastFollowerChecks) {
  int       numActuals = static_cast<int>(actualIdxToFormal.size());
  FnSymbol* retval     = fn;

  if (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) ||
      fnIsDefaultInit(fn)) {
    // TODO:
    //  * remove fnIsDefaultInit component of the conditional
    //    once default init uses 'in' intent to move args to fields.
    //  * remove this branch of the conditional once
    //    initializers have replaced the default constructor

    if (numActuals < fn->numFormals()) {
      retval = wrapDefaultedFormals(retval, info, actualIdxToFormal);
    }

    // Map actuals to formals by position
    if (actualIdxToFormal.size() > 1) {
      reorderActuals(retval, info, actualIdxToFormal);
    }

    if (info.actuals.n > 0) {
      coerceActuals(retval, info);
    }

    // reset actualIdxToFormal
    // at this point in this branch, it's always matching the actuals
    // since reordering/defaults have been addressed.
    actualIdxToFormal.resize(retval->numFormals());
    int i = 0;
    for_formals(formal, retval) {
      actualIdxToFormal[i] = formal;
      i++;
    }

    if (isPromotionRequired(retval, info, actualIdxToFormal) == true) {
      retval = promotionWrap(retval, info, actualIdxToFormal, fastFollowerChecks);
    }
  } else {

    if (isPromotionRequired(retval, info, actualIdxToFormal) == true) {
      // Note: promotionWrap will handle default args in the inner call
      // to the original function, and it will create a different promotion
      // wrapper for each set of default arguments needed.
      retval = promotionWrap(retval, info, actualIdxToFormal, fastFollowerChecks);
    }

    // If we don't have the right number of arguments, adjust the
    // call site to handle default arguments.
    if (numActuals < retval->numFormals()) {
      // note: this handle default args and reordering
      addDefaultsAndReorder(retval, info, actualIdxToFormal);
    } else {
      // handle reordering only
      if (actualIdxToFormal.size() > 1) {
        reorderActuals(retval, info, actualIdxToFormal);
      }
    }

    // handle coercion
    // in the event of promotion, coercion might be necessary
    // for non-promoted arguments.
    if (info.actuals.n > 0) {
      coerceActuals(retval, info);
    }
  }

  return retval;
}

static bool fnIsDefaultInit(FnSymbol* fn) {
  return fn->hasFlag(FLAG_COMPILER_GENERATED) &&
         fn->hasFlag(FLAG_LAST_RESORT) &&
         (0 == strcmp(fn->name, "init") ||
          0 == strcmp(fn->name, "_new"));
}

/************************************* | **************************************
*                                                                             *
* wrapDefaultedFormals()                                                      *
*                                                                             *
* "Wrap" a call with fewer actuals than the number of formals.                *
* This implies that the Chapel procedure has formals with "default" values.   *
*                                                                             *
* This is handled by finding/generating a procedure with the same number of   *
* formals as the call's actuals and then providing the default value for the  *
* unspecified formals.                                                        *
*                                                                             *
************************************** | *************************************/

static FnSymbol* buildWrapperForDefaultedFormals(FnSymbol*     fn,
                                                 CallInfo&     info,
                                                 Vec<Symbol*>* defaults,
                                                 SymbolMap*    paramMap);

static void      formalIsNotDefaulted(FnSymbol*  fn,
                                      ArgSymbol* formal,
                                      CallExpr*  call,
                                      FnSymbol*  wrapFn,
                                      SymbolMap& copyMap,
                                      SymbolMap* paramMap);

static void      updateWrapCall(FnSymbol*  fn,
                                ArgSymbol* formal,
                                CallExpr*  call,
                                FnSymbol*  wrapFn,
                                Symbol*    temp,
                                SymbolMap& copyMap,
                                SymbolMap* paramMap);

static void      formalIsDefaulted(FnSymbol*  fn,
                                   ArgSymbol* formal,
                                   CallExpr*  call,
                                   FnSymbol*  wrapFn,
                                   SymbolMap& copyMap);

static bool      defaultedFormalUsesDefaultForType(ArgSymbol* formal);

static bool      formalDefaultIsCall(ArgSymbol* formal);

static void      defaultedFormalApplyDefaultForType(ArgSymbol* formal,
                                                    BlockStmt* wrapFn,
                                                    VarSymbol* temp);

static void      defaultedFormalApplyDefaultValue(FnSymbol*  fn,
                                                  ArgSymbol* formal,
                                                  bool       addAddrOf,
                                                  BlockStmt* wrapFn,
                                                  VarSymbol* temp);

static void      insertWrappedCall(FnSymbol* fn,
                                   FnSymbol* wrapper,
                                   CallExpr* call);

static Symbol* createDefaultedActual(FnSymbol*  fn,
                                     ArgSymbol* formal,
                                     CallExpr*  call,
                                     BlockStmt* body,
                                     SymbolMap& copyMap);

// info is used to handle out-of-order named arguments
// if there aren't any out-of-order arguments (as with promotion)
// it can be NULL.
static void addDefaultsAndReorder(FnSymbol *fn,
                                CallExpr* call,
                                CallInfo* info,
                                std::vector<ArgSymbol*>& actualFormals,
                                bool resolveNewCode) {

  int numFormals = fn->numFormals();
  std::vector<Symbol*> newActuals(numFormals);
  std::vector<int8_t> newActualDefaulted(numFormals);

  // Gather the actuals into newActuals with NULLs where
  // we need to fill in a default. This also happens
  // to address the need to reorder the actuals.
  int i = 0;
  for_formals(formal, fn) {
    bool actualProvidedForFormal = false;
    Symbol* actualSym = NULL;
    if (info) {
      int j = 0;
      for_vector(ArgSymbol, arg, actualFormals) {
        if (arg == formal) {
          actualSym = info->actuals.v[j];
        }
        j++;
      }
    } else {
      int j = 0;
      for_actuals(actual, call) {
        if (actualFormals[j] == formal) {
          actualProvidedForFormal = true;
          if (SymExpr* se = toSymExpr(actual))
            actualSym = se->symbol();
        }
        j++;
      }
    }

    newActuals[i] = actualSym;
    newActualDefaulted[i] = !actualProvidedForFormal;

    i++;
  }

  // Remove the actuals from the call
  // (we'll add them back again in a moment)
  for_actuals(actual, call) {
    actual->remove();
  }

  // Create a copyMap to handle cases like
  //   proc f(a, b=a)
  // where a formal argument's default value refers to a previous formal
  SymbolMap copyMap;

  // Create a Block to store the default values
  // We'll flatten this back out again in a minute.
  BlockStmt* body = new BlockStmt();
  call->getStmtExpr()->insertBefore(body);

  // Fill in the NULLs in newActuals with the appropriate default argument.
  i = 0;
  for_formals(formal, fn) {
    if (newActuals[i] == NULL) {
      // Fill it in with a default argument.
      newActuals[i] = createDefaultedActual(fn, formal, call, body, copyMap);
    }
    // TODO -- should this be adding a reference to the original actual?
    copyMap.put(formal, newActuals[i]);
    i++;
  }

  // Apply the map copyMap to the block storing defaults
  update_symbols(body, &copyMap);

  // Normalize and resolve the new code in the BlockStmt.
  normalize(body);
  if (resolveNewCode) {
    // Resolve the new code
    // This is important in the usual case where we are adding
    // some calls before a call that is being resolved. The rest
    // of resolution will assume that this new code is already
    // resolved, since it is before what is currently being handled.
    resolveBlockStmt(body);

    // resolveBlockStmt might remove DefExprs for params
    // in that event, we need to update them here.
    for (i = 0; i < numFormals; i++) {
      if (newActualDefaulted[i])
        if (Symbol* param = paramMap.get(newActuals[i]))
          newActuals[i] = param;
    }

    // Check that newActuals are coercible to the formals
    i = 0;
    for_formals(formal, fn) {
      if (newActualDefaulted[i]) {
        Symbol* actual = newActuals[i];
        bool actualIsTypeAlias = actual->hasFlag(FLAG_TYPE_VARIABLE);
        bool formalIsTypeAlias = formal->hasFlag(FLAG_TYPE_VARIABLE);
        bool formalIsParam     = formal->hasFlag(FLAG_INSTANTIATED_PARAM) ||
                                 formal->intent == INTENT_PARAM;

        // Use the value types to work around issues with string literals.
        // Value types should be sufficient for the check needed here, though.
        Type* actualValType = actual->getValType();
        Type* formalValType = formal->getValType();

        bool promotes = false;
        bool dispatches = canDispatch(actualValType, actual,
                                      formalValType, fn,
                                      &promotes, NULL, formalIsParam);

        if (actualIsTypeAlias != formalIsTypeAlias ||
            dispatches == false ||
            promotes == true) {
          USR_FATAL_CONT(formal, "Default expression not convertible to formal type");
        }
      }
      i++;
    }
  }

  // Add the actuals back to the call.
  for_vector(Symbol, actual, newActuals) {
    call->insertAtTail(actual);
  }

  // Adjust AST location to be call site
  reset_ast_loc(body, call);

  // Flatten body
  body->flattenAndRemove();
}

static void addDefaultsAndReorder(FnSymbol *fn,
                                CallInfo& info,
                                std::vector<ArgSymbol*>& actualFormals) {

  // Handle both reordering and default arguments
  addDefaultsAndReorder(fn, info.call, &info, actualFormals, true);

  // Update the CallInfo actuals and actualNames fields
  info.actuals.clear();
  info.actualNames.clear();
  for_actuals(actual, info.call) {
    SymExpr* se = toSymExpr(actual);
    INT_ASSERT(se);
    info.actuals.add(se->symbol());
    info.actualNames.add(NULL);
  }

  // update actualFormals[] for use in reorderActuals
  // Since we addressed reordering above, this is always just
  // the formals in order.
  actualFormals.resize(fn->numFormals());
  int i = 0;
  for_formals(formal, fn) {
    actualFormals[i] = formal;
    i++;
  }
}

static DefaultExprFnEntry buildDefaultedActualFn(FnSymbol*  fn,
                                                 ArgSymbol* formal) {

  DefaultExprFnEntry ret;

  SET_LINENO(formal);


  // Set up the wrapper function
  FnSymbol* wrapper = new FnSymbol(astr(fn->name, "_default_", formal->name));
  ret.defaultExprFn = wrapper;

  //wrapper->addFlag(FLAG_WRAPPER);

  wrapper->addFlag(FLAG_INVISIBLE_FN);

  wrapper->addFlag(FLAG_INLINE);

  wrapper->retTag = RET_VALUE;

  if (fn->hasFlag(FLAG_METHOD)) {
    wrapper->addFlag(FLAG_METHOD);
  }

  if (fn->hasFlag(FLAG_METHOD_PRIMARY)) {
    wrapper->addFlag(FLAG_METHOD_PRIMARY);
  }

  wrapper->instantiationPoint = fn->instantiationPoint;

  if (fn->hasFlag(FLAG_LAST_RESORT)) {
    wrapper->addFlag(FLAG_LAST_RESORT);
  }

  if (fn->hasFlag(FLAG_COMPILER_GENERATED)) {
    wrapper->addFlag(FLAG_WAS_COMPILER_GENERATED);
  }

  wrapper->addFlag(FLAG_COMPILER_GENERATED);
  wrapper->addFlag(FLAG_MAYBE_PARAM);
  wrapper->addFlag(FLAG_MAYBE_TYPE);

  if (fn->throwsError())
    wrapper->throwsErrorInit();

  if (formal->intent == INTENT_TYPE ||
      formal->hasFlag(FLAG_TYPE_VARIABLE))
    wrapper->retTag = RET_TYPE;

  if (formal->intent == INTENT_PARAM)
    wrapper->retTag = RET_PARAM;

  SymbolMap copyMap;

  // Set up the arguments
  if (fn->hasFlag(FLAG_METHOD) &&
      fn->_this != NULL &&
      !fn->hasFlag(FLAG_CONSTRUCTOR)) {
    // Set up mt and this arguments
    Symbol* thisArg = fn->_this;
    ArgSymbol* mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
    ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", thisArg->type);
    // Make sure to set FLAG_THIS and also set it as a type variable
    // for type methods.
    _this->copyFlags(thisArg);
    wrapper->insertFormalAtTail(mt);
    wrapper->insertFormalAtTail(_this);
    wrapper->_this = _this;
    copyMap.put(thisArg, _this);
  }

  // Add any arguments that the expression depended upon
  std::vector<SymExpr*> symExprs;
  // Including as type expressions (arrays might depend on domains)
  if (formal->typeExpr) collectSymExprs(formal->typeExpr, symExprs);
  collectSymExprs(formal->defaultExpr, symExprs);

  for_formals(fnFormal, fn) {
    bool usedFormal = false;
    for_vector(SymExpr, se, symExprs) {
      if (se->symbol() == fnFormal) {
        usedFormal = true;
        break;
      }
    }

    if (usedFormal) {
      if (fnFormal != fn->_this) {
        // _this handled separately
        // add formal if it's not _this

        // Always pass the formal by ref/const ref
        IntentTag intent = INTENT_BLANK;
        switch (fnFormal->intent) {
          case INTENT_IN:
          case INTENT_CONST:
          case INTENT_CONST_IN:
          case INTENT_CONST_REF:
            intent = INTENT_CONST_REF;
            break;
          case INTENT_REF_MAYBE_CONST:
          case INTENT_PARAM:
          case INTENT_TYPE:
          case INTENT_BLANK:
            intent = fnFormal->intent;
            break;
          case INTENT_OUT:
          case INTENT_INOUT:
          case INTENT_REF:
            intent = INTENT_REF;
            break;
          // intentionally no default
        }

        ArgSymbol* newFormal = NULL;
        newFormal = new ArgSymbol(intent, fnFormal->name, fnFormal->type);
        wrapper->insertFormalAtTail(newFormal);
        ret.usedFormals.push_back(std::make_pair(fnFormal, newFormal));
        copyMap.put(fnFormal, newFormal);

        // Propagate a param value
        if (Symbol* x = paramMap.get(fnFormal))
          paramMap.put(newFormal, x);
        // Propagate a type value
        if (fnFormal->hasFlag(FLAG_TYPE_VARIABLE))
          newFormal->addFlag(FLAG_TYPE_VARIABLE);
      }
    }
  }

  // Fill in the body of the function

  // Add a return value variable so the function is "normal"
  // while we resolve a block containing the default-expr. We
  // resolve the block before resolving the rest of the function
  // so that we can set the return intent appropriately.
  VarSymbol* rvv = newTemp("ret");
  rvv->addFlag(FLAG_RVV);
  rvv->addFlag(FLAG_MAYBE_PARAM);
  rvv->addFlag(FLAG_MAYBE_TYPE);

  wrapper->insertAtTail(new DefExpr(rvv));

  // This is the block we'll resolve to compute the return intent
  BlockStmt* block = new BlockStmt();
  wrapper->insertAtTail(block);

  wrapper->insertAtTail(new CallExpr(PRIM_RETURN, rvv));

  // We create a separate temp to store the result of the default
  // argument expression so that we can query its type while setting
  // up the return intent for the function. (The return value variable
  // gets special treatment and would be harder to use in this way).
  VarSymbol* temp   = newTemp("temp");

  /* The following code is tricky. If it's adjusted, the following
     tests are a good place to start:

     test/functions/bradc/defaultVals/defaultArrCausesCopy.chpl
     test/functions/default-arguments/iterator-default-arguments.chpl
     test/memory/figueroa/LeakedMemory6.chpl
   */

  /* If the default expression is a VarSymbol or an ArgSymbol,
     return a reference to that VarSymbol/ArgSymbol.

     If the default expression is a function call, return
     the same type as the called function. Otherwise, we might
     return a reference to stack memory.
   */

  IntentTag  formalIntent = formal->intent;
  if (formal->type   != dtTypeDefaultToken &&
      formal->type   != dtMethodToken      &&
      formal->intent == INTENT_BLANK) {
    formalIntent = blankIntentForType(formal->type);
  }

  if ((formalIntent & INTENT_FLAG_REF) != 0)
    temp->addFlag(FLAG_MAYBE_REF);

  if (formalIntent != INTENT_INOUT && formalIntent != INTENT_OUT) {
    temp->addFlag(FLAG_MAYBE_PARAM);
    temp->addFlag(FLAG_EXPR_TEMP); // ? is this needed?
  }

  if (formal->hasFlag(FLAG_TYPE_VARIABLE) == true) {
    temp->addFlag(FLAG_TYPE_VARIABLE);
  }

  block->insertAtTail(new DefExpr(temp));

  if (defaultedFormalUsesDefaultForType(formal) == true) {
    defaultedFormalApplyDefaultForType(formal, block, temp);

  } else if (formalIntent == INTENT_OUT) {
    defaultedFormalApplyDefaultForType(formal, block, temp);

  } else {
    // If the default expression is a call, don't use PRIM_ADDR_OF on it.
    // Instead, we'll set temp to a ref or not based on FLAG_MAYBE_REF.
    bool addAddrOf = false;
    if ((formalIntent & INTENT_FLAG_REF) != 0 && !formalDefaultIsCall(formal))
      addAddrOf = true;

    defaultedFormalApplyDefaultValue(fn, formal, addAddrOf, block, temp);
  }

  // Update references to previous arguments to use the
  // default-expr-function formals
  update_symbols(block, &copyMap);

  // Add the function to the tree
  fn->defPoint->insertAfter(new DefExpr(wrapper));

  normalize(block);
  resolveBlockStmt(block);

  block->insertAtTail(new CallExpr(PRIM_MOVE, rvv, temp));
  block->flattenAndRemove();

  // Now we know if 'temp' is a param or a type.
  if (temp->hasFlag(FLAG_TYPE_VARIABLE)) {
    wrapper->retTag = RET_TYPE;
  } else if (paramMap.get(temp)) {
    wrapper->retTag = RET_PARAM;
  } else if (temp->qualType().isRef()) {
    if (temp->qualType().isConst())
      wrapper->retTag = RET_CONST_REF;
    else
      wrapper->retTag = RET_REF;
  }

  // Finish resolving the function
  resolveSignatureAndFunction(wrapper);

  return ret;
}


static Symbol* createDefaultedActual(FnSymbol*  fn,
                                     ArgSymbol* formal,
                                     CallExpr*  call,
                                     BlockStmt* body,
                                     SymbolMap& copyMap) {

  // Find the function we are to call, and call it
  DefaultExprFnEntry* entry = NULL;
  formalToDefaultExprEntryMap::iterator it;

  it = formalToDefaultExprEntry.find(formal);
  if (it != formalToDefaultExprEntry.end()) {
    // Use the existing entry
    entry = &it->second;
  } else {
    // Add a new entry
    entry = & formalToDefaultExprEntry.insert(
                 std::make_pair(formal, buildDefaultedActualFn(fn, formal))
               ).first->second;
  }

  INT_ASSERT(entry);

  // TODO - can't we get the param formals out of paramMap?
  // Or fn->substitutions?

  // TODO - can this simplify to something more like insertWrappedCall?
  SET_LINENO(formal);

  VarSymbol* temp   = newTemp(astr("default_arg_", formal->name));

  temp->addFlag(FLAG_EXPR_TEMP);
  temp->addFlag(FLAG_MAYBE_PARAM);
  temp->addFlag(FLAG_MAYBE_TYPE);
  // Allow e.g. proc f(ref x=returnSomeValue())
  temp->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);

  // TODO: do we need to add FLAG_INSERT_AUTO_DESTROY here?

  body->insertAtTail(new DefExpr(temp));

  // Add a call to the defaulted formals fn passing in the
  // appropriate actual values.
  CallExpr* newCall = new CallExpr(entry->defaultExprFn);

  // Add method token, this if needed
  if (fn->hasFlag(FLAG_METHOD) &&
      fn->_this != NULL &&
      !fn->hasFlag(FLAG_CONSTRUCTOR)) {
    // Set up mt and _this arguments
    newCall->insertAtTail(gMethodToken);
    Symbol* usedFormal = fn->_this;
    Symbol* mapTo = copyMap.get(usedFormal);
    INT_ASSERT(mapTo); // Should have another actual!
    newCall->insertAtTail(new SymExpr(mapTo));
  }

  size_t nUsedFormals = entry->usedFormals.size();
  for (size_t i = 0; i < nUsedFormals; i++) {
    Symbol* usedFormal = entry->usedFormals[i].first;
    Symbol* mapTo = copyMap.get(usedFormal);
    INT_ASSERT(mapTo); // Should have another actual!
    newCall->insertAtTail(new SymExpr(mapTo));
  }

  body->insertAtTail(new CallExpr(PRIM_MOVE, temp, newCall));

  // Now return the expression to use as the defaulted argument
  return temp;
}



static FnSymbol* wrapDefaultedFormals(FnSymbol*                fn,
                                      CallInfo&                info,
                                      std::vector<ArgSymbol*>& actualFormals) {
  Vec<Symbol*> defaults;
  int          j      = 1;
  FnSymbol*    retval = NULL;

  for_formals(formal, fn) {
    bool used = false;

    for_vector(ArgSymbol, arg, actualFormals) {
      if (arg == formal) {
        used = true;
      }
    }

    if (used == false) {
      defaults.add(formal);
    }
  }

  retval = checkCache(defaultsCache, fn, &defaults);

  if (retval == NULL) {
    retval = buildWrapperForDefaultedFormals(fn, info, &defaults, &paramMap);

    resolveSignature(retval);

    addCache(defaultsCache, fn, retval, &defaults);
  }

  // update actualFormals[] for use in reorderActuals
  for_formals(formal, fn) {
    for (size_t i = 0; i < actualFormals.size(); i++) {
      if (actualFormals[i] == formal) {
        actualFormals[i] = retval->getFormal(j++);
      }
    }
  }

  return retval;
}

static FnSymbol* buildWrapperForDefaultedFormals(FnSymbol*     fn,
                                                 CallInfo&     info,
                                                 Vec<Symbol*>* defaults,
                                                 SymbolMap*    paramMap) {
  SET_LINENO(fn);

  SymbolMap copyMap;
  CallExpr* call    = new CallExpr(fn);
  FnSymbol* retval  = buildEmptyWrapper(fn);
  retval->instantiationPoint = getVisibilityBlock(info.call);

  retval->cname = astr("_default_wrap_", fn->cname);

  if (fn->isIterator() == false) {
    retval->retType = fn->retType;
  }

  if (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR)      == true &&
      fn->_this->type->symbol->hasFlag(FLAG_REF) == false) {
    Symbol* _this = fn->_this->copy();

    copyMap.put(fn->_this, _this);

    retval->removeFlag(FLAG_COMPILER_GENERATED);

    retval->_this = _this;

    retval->insertAtTail(new DefExpr(_this));

    if (defaults->v[defaults->n - 1]->hasFlag(FLAG_IS_MEME) == true) {
      if (isRecord(fn->_this->type) == false &&
          isUnion(fn->_this->type)  == false) {
        CallExpr* hereAlloc = callChplHereAlloc(_this->typeInfo());

        retval->insertAtTail(new CallExpr(PRIM_MOVE,   _this, hereAlloc));
        retval->insertAtTail(new CallExpr(PRIM_SETCID, _this));
      }
    }

    retval->insertAtTail(new CallExpr(PRIM_INIT_FIELDS, _this));
  }

  for_formals(formal, fn) {
    SET_LINENO(formal);

    if (defaults->in(formal) == NULL) {
      formalIsNotDefaulted(fn, formal, call, retval, copyMap, paramMap);

    } else if (paramMap->get(formal) != NULL) {
      call->insertAtTail(paramMap->get(formal));

    } else if (formal->hasFlag(FLAG_IS_MEME) == true) {
      formal->type = retval->_this->type;

      call->insertAtTail(retval->_this);

    } else {
      formalIsDefaulted(fn, formal, call, retval, copyMap);
    }
  }

  update_symbols(retval->body, &copyMap);

  insertWrappedCall(fn, retval, call);

  normalize(retval);

  return retval;
}

// The call provides an actual for this formal.  The wrap function should
// accept this actual and pass it to the underlying function.
static void formalIsNotDefaulted(FnSymbol*  fn,
                                 ArgSymbol* formal,
                                 CallExpr*  call,
                                 FnSymbol*  wrapFn,
                                 SymbolMap& copyMap,
                                 SymbolMap* paramMap) {
  ArgSymbol* wrapFnFormal = copyFormalForWrapper(formal);

  wrapFn->insertFormalAtTail(wrapFnFormal);

  // If the formal has a param value, then wrapFormal should have same value
  if (Symbol* value = paramMap->get(formal)) {
    paramMap->put(wrapFnFormal, value);
  }

  if (fn->_this == formal) {
    wrapFn->_this = wrapFnFormal;
  }

  if (formal->hasFlag(FLAG_IS_MEME) == true &&
      wrapFn->_this                 != NULL) {
    Symbol* _this = wrapFn->_this;

    _this->defPoint->insertAfter(new CallExpr(PRIM_MOVE, _this, wrapFnFormal));
  }

  if (formal->type->symbol->hasFlag(FLAG_REF) == true) {
    Symbol*   temp         = newTemp("wrap_ref_arg");
    CallExpr* addrOfFormal = new CallExpr(PRIM_ADDR_OF, wrapFnFormal);

    temp->addFlag(FLAG_MAYBE_PARAM);

    wrapFn->insertAtTail(new DefExpr(temp));
    wrapFn->insertAtTail(new CallExpr(PRIM_MOVE, temp, addrOfFormal));

    updateWrapCall(fn, formal, call, wrapFn, temp, copyMap, paramMap);

  // Formal has a type expression attached and is array/dom/dist
  } else if (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR)      == true  &&
             fn->_this->type->symbol->hasFlag(FLAG_REF) == false &&
             wrapFnFormal->typeExpr                     != NULL  &&
             isRecordWrappedType(wrapFnFormal->type)    == true) {
    Symbol*        temp      = newTemp("wrap_type_arg");
    AggregateType* _thisType = toAggregateType(fn->_this->type);
    BlockStmt*      typeExpr = wrapFnFormal->typeExpr->copy();
    CallExpr*       initExpr = NULL;

    if (Symbol* field = _thisType->getField(formal->name, false)) {
      if (field->defPoint->parentSymbol == _thisType->symbol) {
        temp->addFlag(FLAG_INSERT_AUTO_DESTROY);
      }
    }

    wrapFn->insertAtTail(new DefExpr(temp));

    for_alist(expr, typeExpr->body) {
      wrapFn->insertAtTail(expr->remove());
    }

    initExpr = new CallExpr(PRIM_INIT, wrapFn->body->body.tail->remove());

    wrapFn->insertAtTail(new CallExpr(PRIM_MOVE, temp, initExpr));
    wrapFn->insertAtTail(new CallExpr("=",       temp, wrapFnFormal));

    updateWrapCall(fn, formal, call, wrapFn, temp,         copyMap, paramMap);

  } else {
    updateWrapCall(fn, formal, call, wrapFn, wrapFnFormal, copyMap, paramMap);
  }
}

static void updateWrapCall(FnSymbol*  fn,
                           ArgSymbol* formal,
                           CallExpr*  call,
                           FnSymbol*  wrapFn,
                           Symbol*    temp,
                           SymbolMap& copyMap,
                           SymbolMap* paramMap) {
  copyMap.put(formal, temp);

  call->insertAtTail(temp);

  if (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR)      == true  &&
      fn->_this->type->symbol->hasFlag(FLAG_REF) == false &&
      strcmp(fn->name, "_construct__tuple")      != 0     &&
      formal->hasFlag(FLAG_TYPE_VARIABLE)        == false &&
      paramMap->get(formal)                      == NULL  &&
      formal->type                               != dtMethodToken) {
    Symbol*        _this     = wrapFn->_this;
    AggregateType* _thisType = toAggregateType(_this->type);

    if (Symbol* field = _thisType->getField(formal->name, false)) {
      Symbol* parent = field->defPoint->parentSymbol;

      if (parent == _thisType->symbol) {
        Symbol*   tmp      = newTemp("wrap_arg");
        Symbol*   name     = new_CStringSymbol(formal->name);
        CallExpr* autoCopy = new CallExpr("chpl__autoCopy", temp);

        wrapFn->insertAtTail(new DefExpr(tmp));

        wrapFn->insertAtTail(new CallExpr(PRIM_MOVE, tmp, autoCopy));

        wrapFn->insertAtTail(new CallExpr(PRIM_SET_MEMBER, _this, name, tmp));

        copyMap.put(formal, tmp);

        call->argList.tail->replace(new SymExpr(tmp));
      }
    }
  }
}

static void formalIsDefaulted(FnSymbol*  fn,
                              ArgSymbol* formal,
                              CallExpr*  call,
                              FnSymbol*  wrapFn,
                              SymbolMap& copyMap) {
  IntentTag  intent = formal->intent;
  VarSymbol* temp   = newTemp(astr("default_arg", formal->name));

  if (formal->type   != dtTypeDefaultToken &&
      formal->type   != dtMethodToken      &&
      formal->intent == INTENT_BLANK) {
    intent = blankIntentForType(formal->type);
  }

  if (intent != INTENT_INOUT && intent != INTENT_OUT) {
    temp->addFlag(FLAG_MAYBE_PARAM);
    temp->addFlag(FLAG_EXPR_TEMP);
  }

  if (formal->hasFlag(FLAG_TYPE_VARIABLE) == true) {
    temp->addFlag(FLAG_TYPE_VARIABLE);
  }

  copyMap.put(formal, temp);

  wrapFn->insertAtTail(new DefExpr(temp));

  if (defaultedFormalUsesDefaultForType(formal) == true) {
    defaultedFormalApplyDefaultForType(formal, wrapFn->body, temp);

  } else if (intent == INTENT_OUT) {
    defaultedFormalApplyDefaultForType(formal, wrapFn->body, temp);

  } else {
    bool addAddrOf = false;
    if ((intent & INTENT_FLAG_REF) != 0)
      addAddrOf = true;
    defaultedFormalApplyDefaultValue(fn, formal, addAddrOf, wrapFn->body, temp);
  }

  call->insertAtTail(temp);

  // MPF - this seems strange since it is assigning to fields that will be
  // set in the construct call at the end.  It is handling the current issue
  // that an iterator to initialize an array can refer to the fields.
  // See arrayDomInClassRecord2.chpl.
  //
  // In the future, it would probably be better to initialize the
  // fields in order in favor of calling the default constructor.
  if (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR)      == true  &&
      fn->_this->type->symbol->hasFlag(FLAG_REF) == false &&
      strcmp(fn->name, "_construct__tuple")      != 0) {
    if (formal->hasFlag(FLAG_TYPE_VARIABLE) == false) {
      AggregateType* type = toAggregateType(wrapFn->_this->type);

      if (Symbol* field = type->getField(formal->name, false)) {
        if (field->defPoint->parentSymbol == type->symbol) {
          VarSymbol* name = new_CStringSymbol(formal->name);

          wrapFn->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                            wrapFn->_this,
                                            name,
                                            temp));
        }
      }
    }
  }
}

static bool defaultedFormalUsesDefaultForType(ArgSymbol* formal) {
  bool retval = false;

  if (formal->defaultExpr->body.length == 1) {
    if (SymExpr* se = toSymExpr(formal->defaultExpr->body.tail)) {
      retval = se->symbol() == gTypeDefaultToken;
    }
  }

  return retval;
}

static bool formalDefaultIsCall(ArgSymbol* formal) {
  bool retval = true;

  if (formal->defaultExpr->body.length == 1) {
    if (isSymExpr(formal->defaultExpr->body.tail)) {
      retval = false;
    }
  }

  return retval;
}

static void defaultedFormalApplyDefaultForType(ArgSymbol* formal,
                                               BlockStmt* body,
                                               VarSymbol* temp) {
  if (formal->typeExpr != NULL) {
    BlockStmt* typeExpr = formal->typeExpr->copy();
    Expr*      lastExpr = NULL;

    for_alist(expr, typeExpr->body) {
      body->insertAtTail(expr->remove());
    }

    lastExpr = body->body.tail;

    if (formal->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      body->insertAtTail(new CallExpr(PRIM_MOVE, temp, lastExpr->remove()));

    } else {
      //
      // 2016-07-18: benharsh: I was encountering an issue where we were
      // attempting to wrap a function where we had inserted return temps
      // for chpl__buildArrayRuntimeType. This wrapping function then
      // created an invalid AST like this:
      //
      // (move call_tmp
      //       (move _return_tmp_ (call chpl__buildArrayRuntimeType ...)))
      //
      // With this change we assume that if the last Expr is a PRIM_MOVE
      // that we can use the LHS of that move in the PRIM_INIT call that
      // needs to be inserted.
      //
      // The test that exposed this issue is:
      //   test/arrays/diten/distArrInRecord.chpl
      //
      // Compiled with -suseBulkTransferStride
      //
      CallExpr* lastCall = toCallExpr(lastExpr);
      Expr*     initExpr = NULL;

      if (lastCall != NULL && lastCall->isPrimitive(PRIM_MOVE) == true) {
        initExpr = new CallExpr(PRIM_INIT, lastCall->get(1)->copy());

      } else {
        initExpr = new CallExpr(PRIM_INIT, lastExpr->remove());
      }

      body->insertAtTail(new CallExpr(PRIM_MOVE, temp, initExpr));
    }

  } else {
    Expr* expr = new SymExpr(formal->type->symbol);

    if (formal->hasFlag(FLAG_TYPE_VARIABLE) == false) {
      expr = new CallExpr(PRIM_INIT, expr);
    }

    body->insertAtTail(new CallExpr(PRIM_MOVE, temp, expr));
  }
}

static void defaultedFormalApplyDefaultValue(FnSymbol*  fn,
                                             ArgSymbol* formal,
                                             bool addAddrOf,
                                             BlockStmt* body,
                                             VarSymbol* temp) {
  BlockStmt* defaultExpr = formal->defaultExpr->copy();
  Expr*      fromExpr    = NULL;

  for_alist(expr, defaultExpr->body) {
    body->insertAtTail(expr->remove());
  }

  fromExpr = body->body.tail->remove();

  if (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR)      == true &&
      fn->_this->type->symbol->hasFlag(FLAG_REF) == false) {
    // Normalize may have added an initCopy for the defaultExpr.
    // If it didn't, add the copy here
    if (CallExpr* fromCall = toCallExpr(fromExpr)) {
      if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(fromCall->baseExpr)) {
        if (strcmp(urse->unresolved, "chpl__initCopy")      != 0 &&
            strcmp(urse->unresolved, "_createFieldDefault") != 0) {
          fromExpr = new CallExpr("chpl__initCopy", fromExpr);
        }

      } else {
        INT_ASSERT(false);
      }
    }

  } else {
    if (addAddrOf == true) {
      fromExpr = new CallExpr(PRIM_ADDR_OF, fromExpr);
    }
  }

  body->insertAtTail(new CallExpr(PRIM_MOVE, temp, fromExpr));

  if (formal->intent == INTENT_INOUT) {
    INT_ASSERT(!temp->hasFlag(FLAG_EXPR_TEMP));
    temp->removeFlag(FLAG_MAYBE_PARAM);
  }
}

static void insertWrappedCall(FnSymbol* fn,
                              FnSymbol* wrapper,
                              CallExpr* call) {
  if (fn->getReturnSymbol() == gVoid || fn->retType == dtVoid) {
    wrapper->insertAtTail(call);

  } else {
    Symbol* tmp = newTemp("wrap_call_tmp");

    tmp->addFlag(FLAG_EXPR_TEMP);
    tmp->addFlag(FLAG_MAYBE_PARAM);
    tmp->addFlag(FLAG_MAYBE_TYPE);

    wrapper->insertAtTail(new DefExpr(tmp));
    wrapper->insertAtTail(new CallExpr(PRIM_MOVE,   tmp, call));
    wrapper->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
  }

  fn->defPoint->insertAfter(new DefExpr(wrapper));
}

/************************************* | **************************************
*                                                                             *
* reorder the actuals to match the order of the formals                       *
*                                                                             *
************************************** | *************************************/

static void reorderActuals(FnSymbol*                fn,
                           CallInfo&                info,
                           std::vector<ArgSymbol*>& actualFormals) {
  int              numArgs       = actualFormals.size();
  std::vector<int> formalsToFormals(numArgs);
  bool             needToReorder = false;
  int              i             = 0;

  for_formals(formal, fn) {
    int j = 0;

    i++;

    for_vector(ArgSymbol, af, actualFormals) {
      j++;

      if (af == formal) {
        if (i != j) {
          needToReorder = true;
        }

        formalsToFormals[i - 1] = j - 1;
      }
    }
  }

  if (needToReorder == true) {
    std::vector<Expr*>       savedActuals(numArgs);
    std::vector<Symbol*>     ciActuals(numArgs);
    std::vector<const char*> ciActualNames(numArgs);
    int                      index = 0;

    // remove all actuals in an order
    for_actuals(actual, info.call) {
      savedActuals[index++] = actual->remove();
    }

    // reinsert them in the desired order
    for (int i = 0; i < numArgs; i++) {
      info.call->insertAtTail(savedActuals[formalsToFormals[i]]);
    }

    // reorder CallInfo data as well
    // ideally this would be encapsulated in within the CallInfo class
    INT_ASSERT(info.actuals.n == numArgs);

    for (int i = 0; i < numArgs; i++) {
      ciActuals[i]     = info.actuals.v[i];
      ciActualNames[i] = info.actualNames.v[i];
    }

    for (int i = 0; i < numArgs; i++) {
      info.actuals.v[i]     = ciActuals[formalsToFormals[i]];
      info.actualNames.v[i] = ciActualNames[formalsToFormals[i]];
    }
  }
}

/************************************* | **************************************
*                                                                             *
* add coercions on the actuals                                                *
*                                                                             *
************************************** | *************************************/

static bool      needToAddCoercion(Type*      actualType,
                                   Symbol*    actualSym,
                                   ArgSymbol* formal,
                                   FnSymbol*  fn);

static IntentTag getIntent(ArgSymbol* formal);

static void      addArgCoercion(FnSymbol*  fn,
                                CallExpr*  call,
                                ArgSymbol* formal,
                                Expr*&     actualExpr,
                                Symbol*&   actualSym,
                                bool&      checkAgain);

static void coerceActuals(FnSymbol* fn, CallInfo& info) {
  if (fn->retTag == RET_PARAM) {
    //
    // This call will be tossed in postFold(), so why bother with coercions?
    //
    // Most importantly, we don't want a readFE-like coercion in this case,
    // because the coercion will stick around even if the call is removed.
    //
    // Todo: postFold() will remove some other calls, too. However we don't
    // know which - until 'fn' is resolved, which here it may not be, yet.
    // So for now we act only if fn has the param retTag.
    //
    // The runner-up todo would be 'type' functions, which actually
    // may need to be invoked at run time if they return a runtime type.
    // Therefore "coercions" might also be needed, e.g., to readFE from
    // a sync var actual to determine the size of the array type's domain.
    // So we will keep the coercions uniformly for now, as if they are
    // a part of type functions' semantics.
    //
    return;
  }

  int   j          = -1;
  Expr* currActual = info.call->get(1);

  for_formals(formal, fn) {
    j++;
    Symbol* actualSym  = info.actuals.v[j];
    Type*   formalType = formal->type;
    bool    c2         = false;
    int     checksLeft = 6;

    // There does not seem to be a limit of how many coercions will be
    // needed for a given actual. For example, in myExpr.someFun(...),
    // each level of _syncvar(T) in myExpr's type adds two coercions,
    // PRIM_DEREF and CallExpr("value",...), to the coercions needed by T.
    //
    // Note: if we take away the special handling of a sync/single actual
    // when it is the receiver to 'fn' (the "value" case above), fewer
    // coercions will suffice for the same number of _syncvar layers.
    //
    // We could have the do-loop below terminate only upon !c2. For now,
    // I am putting a limit on the number of iterations just in case.
    // I am capping it at 6 arbitrarily. This allows for the 5 coercions
    // plus 1 last check in the case of a receiver actual of the type
    // _ref(_syncvar(_syncvar(int))), e.g. an array element "sync sync int".
    // -vass 8'2014
    //

    do {
      Type* actualType = actualSym->type;

      c2 = false;

      if (needToAddCoercion(actualType, actualSym, formal, fn)) {
        if (formalType               == dtStringC &&
            actualType               == dtString  &&
            actualSym->isImmediate() == true) {
          // We do this swap since we know the string is a valid literal
          // There also is no cast defined for string->c_string on purpose (you
          // need to use .c_str()) so the common case below does not work.
          VarSymbol*  var       = toVarSymbol(actualSym);
          const char* str       = var->immediate->v_string;
          SymExpr*    newActual = new SymExpr(new_CStringSymbol(str));

          currActual->replace(newActual);

          currActual = newActual;

        } else {
          addArgCoercion(fn, info.call, formal, currActual, actualSym, c2);
        }
      }
    } while (c2 && --checksLeft > 0);

    INT_ASSERT(c2 == false);

    currActual = currActual->next;
  }
}

// do we need to add some coercion from the actual to the formal?
static bool needToAddCoercion(Type*      actualType,
                              Symbol*    actualSym,
                              ArgSymbol* formal,
                              FnSymbol*  fn) {
  Type* formalType = formal->type;
  bool  retval     = false;

  if (actualType == formalType) {
    retval = false;

  // If we have an actual of ref(formalType) and
  // a REF or CONST REF argument intent, no coercion is necessary.
  } else if (actualType == formalType->getRefType() &&
             (getIntent(formal) & INTENT_FLAG_REF) != 0) {
    retval = false;

  // If actual and formal are type symbols, no coercion is necessary
  } else if (actualSym->hasFlag(FLAG_TYPE_VARIABLE) &&
             formal->hasFlag(FLAG_TYPE_VARIABLE)) {
    retval = false;

  } else if (canCoerce(actualType, actualSym, formalType, fn) == true) {
    retval =  true;

  } else if (isDispatchParent(actualType, formalType)         == true) {
    retval =  true;

  } else {
    retval = false;
  }

  return retval;
}

static IntentTag getIntent(ArgSymbol* formal) {
  IntentTag retval = formal->intent;

  if (retval == INTENT_BLANK || retval == INTENT_CONST) {
    if (formal->type->symbol->hasFlag(FLAG_ITERATOR_RECORD) == false) {
      retval = concreteIntentForArg(formal);
    }
  }

  return retval;
}

// Add a coercion; replace prevActual and actualSym - the actual to 'call' -
// with the result of the coercion.
static void addArgCoercion(FnSymbol*  fn,
                           CallExpr*  call,
                           ArgSymbol* formal,
                           Expr*&     actualExpr,
                           Symbol*&   actualSym,
                           bool&      checkAgain) {
  SET_LINENO(actualExpr);

  Expr*       prevActual = actualExpr;
  TypeSymbol* ats        = actualSym->type->symbol;
  TypeSymbol* fts        = formal->type->symbol;
  CallExpr*   castCall   = NULL;
  VarSymbol*  castTemp   = newTemp("coerce_tmp"); // ..., formal->type ?
  Expr*       newActual  = new SymExpr(castTemp);

  castTemp->addFlag(FLAG_COERCE_TEMP);

  // gotta preserve this-ness, so can write to this's fields in constructors
  if (actualSym->hasFlag(FLAG_ARG_THIS) &&
      isDispatchParent(actualSym->type, formal->type)) {
    castTemp->addFlag(FLAG_ARG_THIS);
  }

  if (NamedExpr* namedActual = toNamedExpr(prevActual)) {
    // preserve the named portion
    Expr* newCurrActual = namedActual->actual;

    newCurrActual->replace(newActual);

    newActual  = prevActual;
    prevActual = newCurrActual;

  } else {
    prevActual->replace(newActual);
  }

  // Now 'prevActual' has been removed+replaced and is ready to be passed
  // as an actual to a cast or some such.
  // We can update addArgCoercion's caller right away.
  actualExpr = newActual;
  actualSym  = castTemp;

  // Here we will often strip the type of its sync-ness.
  // After that we may need another coercion(s), e.g.
  //   _syncvar(int) --readFE()-> _ref(int) --(dereference)-> int --> real
  // or
  //   _syncvar(_syncvar(int))  -->...  _syncvar(int)  -->  [as above]
  //
  // We warn addArgCoercion's caller about that via checkAgain:
  if (isSyncType(ats->type) == true) {
    checkAgain = true;
    castCall   = new CallExpr("readFE", gMethodToken, prevActual);

  } else if (isSingleType(ats->type) == true) {
    checkAgain = true;

    castCall   = new CallExpr("readFF", gMethodToken, prevActual);

  } else if (ats->hasFlag(FLAG_REF) &&
             !(ats->getValType()->symbol->hasFlag(FLAG_TUPLE) &&
               formal->getValType()->symbol->hasFlag(FLAG_TUPLE)) ) {
    //
    // dereference a reference actual
    //
    // after dereferencing we may need another coercion, e.g.
    //   _ref(int)  --coerce->  int  --coerce->  real
    // or
    //   _ref(_syncvar(int)) --> _syncvar(int) --> _ref(int) --> int --> real
    //
    checkAgain = true;

    // MPF - this call here is suspect because dereferencing should
    // call a record's copy-constructor (e.g. autoCopy).
    castCall   = new CallExpr(PRIM_DEREF, prevActual);

    if (SymExpr* prevSE = toSymExpr(prevActual)) {
      if (prevSE->symbol()->hasFlag(FLAG_REF_TO_CONST)) {
        castTemp->addFlag(FLAG_CONST);

        if (prevSE->symbol()->hasFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS)) {
          castTemp->addFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS);
        }
      }
    }

  } else {
    // There was code to handle the case when the flag *is* present.
    // I deleted that code. The assert ensures it wouldn't apply anyway.
    INT_ASSERT(!actualSym->hasFlag(FLAG_INSTANTIATED_PARAM));

    castCall = NULL;
  }

  if (castCall == NULL) {
    // the common case
    castCall = createCast(prevActual, fts);

    if (isString(fts)) {
      castTemp->addFlag(FLAG_INSERT_AUTO_DESTROY);
    }
  }

  // move the result to the temp
  CallExpr* castMove = new CallExpr(PRIM_MOVE, castTemp, castCall);

  call->getStmtExpr()->insertBefore(new DefExpr(castTemp));
  call->getStmtExpr()->insertBefore(castMove);

  resolveCallAndCallee(castCall, true);

  if (FnSymbol* castTarget = castCall->resolvedFunction()) {
    // Perhaps equivalently, we could check "if (tryToken)",
    // except tryToken is not visible in this file.
    if (!castTarget->hasFlag(FLAG_RESOLVED)) {
      // This happens e.g. when castTarget itself has an error.
      // Todo: in this case, we should report the error at the point
      // where it arises, supposedly within resolveFns(castTarget).
      // Why is it not reported there?
      USR_FATAL_CONT(call,
                     "Error resolving a cast from %s to %s",
                     ats->name,
                     fts->name);

      USR_PRINT(castTarget, "  the troublesome function is here");
      USR_STOP();
    }
  }

  resolveCall(castMove);
}

/************************************* | **************************************
*                                                                             *
* handle promoting e.g. foo(x:int) is called with an array of int             *
*                                                                             *
************************************** | *************************************/

namespace {
  struct PromotionInfo {
    FnSymbol* fn;
    FnSymbol* wrapperFn;
    // The following vectors are indexed by the i'th formal to fn (0-based).

    // The TypeSymbol representing the type that is promoted (e.g. array)
    // or NULL if that argument isn't promoted.
    std::vector<TypeSymbol*> promotedType;
    std::vector<uint8_t> defaulted;

    // for the i'th formal to fn, fnFormals is NULL
    // if it's defaulted or the ArgSymbol in wrapperFn if not.
    //std::vector<ArgSymbol*> fnFormals;
    std::vector<ArgSymbol*> wrapperFormals;

    // Subs contains information used to cache the promotion wrapper.
    SymbolMap subs;

    // The call to the promoted function inside of the wrapper.
    std::vector<CallExpr*> wrapCalls;

    PromotionInfo(FnSymbol* fn,
                  CallInfo& info,
                  std::vector<ArgSymbol*>& actualFormals);
  };
}

static FnSymbol*  buildPromotionWrapper(PromotionInfo& promotion,
                                        BlockStmt* visibilityBlock,
                                        CallInfo&  info,
                                        bool       fastFollowerChecks);

static BlockStmt* buildPromotionLoop(PromotionInfo& promotion,
                                     BlockStmt* visibilityBlock,
                                     CallInfo&  info,
                                     bool       fastFollowerChecks);

static void       buildLeaderIterator(FnSymbol* wrapFn,
                                      BlockStmt* visibilityBlock,
                                      Expr*     iterator);

static void       buildFollowerIterator(PromotionInfo& promotion,
                                        BlockStmt* visibilityBlock,
                                        Expr*     indices,
                                        Expr*     iterator,
                                        CallExpr* wrapCall);

static CondStmt*  selectFollower(ArgSymbol* fastFollower,
                                 Expr*      iterator,
                                 VarSymbol* followerIterator,
                                 SymbolMap& followerMap,
                                 ArgSymbol* fiFnFollower);

static BlockStmt* followerForLoop(PromotionInfo& promotion,
                                  Expr*      indices,
                                  Expr*      iterator,
                                  VarSymbol* followerIterator,
                                  SymbolMap& followerMap,
                                  CallExpr* wrapCall);

static void initPromotionWrapper(PromotionInfo& promotion,
                                 BlockStmt* visibilityBlock);

static Expr*      getIndices(PromotionInfo& promotion);

static Expr*      getIterator(PromotionInfo& promotion);

static CallExpr* createPromotedCallForWrapper(PromotionInfo& promotion);

static void       collectPromotionFormals(PromotionInfo& promotion,
                                          std::set<ArgSymbol*>& formals);

static void       fixUnresolvedSymExprsForPromotionWrapper(FnSymbol* wrapper,
                                                           FnSymbol* fn);

static void fixDefaultArgumentsInWrapCall(PromotionInfo& promotion);

static void       buildFastFollowerCheck(bool                  isStatic,
                                         bool                  addLead,
                                         CallInfo&             info,
                                         FnSymbol*             wrapper,
                                         std::set<ArgSymbol*>& formals);

static bool isPromotionRequired(FnSymbol* fn, CallInfo& info,
                                std::vector<ArgSymbol*>& actualFormals) {
  bool retval = false;

  if (fn->name != astrSequals && fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) == false) {
    int numActuals = actualFormals.size();
    for (int j = 0; j < numActuals; j++) {
      Symbol* actual     = info.actuals.v[j];
      Symbol* formal     = actualFormals[j];
      Type*   actualType = actual->type;
      bool    promotes   = false;

      if (isRecordWrappedType(actualType) == true) {
        makeRefType(actualType);

        actualType = actualType->refType;

        INT_ASSERT(actualType);
      }

      if (canDispatch(actualType, actual, formal->type, fn, &promotes)) {
        if (promotes == true) {
          retval = true;
          break;
        }
      }
    }
  }

  return retval;
}

static FnSymbol* promotionWrap(FnSymbol* fn,
                               CallInfo& info,
                               std::vector<ArgSymbol*>& actualIdxToFormal,
                               bool      fastFollowerChecks) {
  SymbolMap subs;
  FnSymbol* retval = NULL;

  if (fReportPromotion) {
    USR_WARN(info.call, "promotion on %s", info.toString());
  }

  PromotionInfo promotion(fn, info, actualIdxToFormal);

  retval = checkCache(promotionsCache, promotion.fn, &promotion.subs);

  if (retval == NULL) {
    SET_LINENO(info.call);
    BlockStmt* visibilityBlock = getVisibilityBlock(info.call);
    retval = buildPromotionWrapper(promotion,
                                   visibilityBlock,
                                   info,
                                   fastFollowerChecks);

    resolveSignature(retval);

    addCache(promotionsCache, promotion.fn, promotion.wrapperFn, &promotion.subs);
  }

  return retval;
}

/* Collect the substitutions that the function implementing the promotion
   will use. The substitutions map from the formal to
   the relevant actual type - or, in the event of a defaulted argument,
   to itself.
 */
PromotionInfo::PromotionInfo(FnSymbol* fn,
                             CallInfo& info,
                             std::vector<ArgSymbol*>& actualFormals) {

  this->fn = fn;
  this->wrapperFn = NULL; // established later along with wrapperFormals

  int numActuals = actualFormals.size();

  for (int j = 0; j < numActuals; j++) {
    Symbol* actual     = info.actuals.v[j];
    Symbol* formal     = actualFormals[j];
    Type*   actualType = actual->type;
    bool    promotes   = false;

    if (isRecordWrappedType(actualType) == true) {
      makeRefType(actualType);

      actualType = actualType->refType;
    }

    if (canDispatch(actualType, actual, formal->type, fn, &promotes)) {
      if (promotes == true) {
        this->subs.put(formal, actualType->symbol);
      }
    }
  }


  for_formals(formal, fn) {
    TypeSymbol* promotedType = NULL;
    bool defaulted = false;
    if (TypeSymbol* ts = toTypeSymbol(this->subs.get(formal))) {
      // this formal is promoted (we just added it to subs in the above loop)
      promotedType = ts;
    } else {
      bool actualProvidedForFormal = false;
      for_vector(ArgSymbol, arg, actualFormals) {
        if (arg == formal) {
          actualProvidedForFormal = true;
          break;
        }
      }
      if (!actualProvidedForFormal)
        defaulted = true;
    }

    this->promotedType.push_back(promotedType);
    this->defaulted.push_back(defaulted);

    if (defaulted) {
      // For defaulted arguments, add something to subs to indicate that.
      // This is just so the cached promotion wrapper is different when
      // there is a different set of defaulted args.
      this->subs.put(formal, formal);
    }
  }
}

static FnSymbol* buildPromotionWrapper(PromotionInfo& promotion,
                                       BlockStmt* visibilityBlock,
                                       CallInfo&  info,
                                       bool       fastFollowerChecks) {

  BlockStmt* loop       = NULL;
  initPromotionWrapper(promotion, visibilityBlock);
  FnSymbol*  retval     = promotion.wrapperFn;
  FnSymbol*  fn         = promotion.fn;

  if (fn->retType == dtVoid || fn->getReturnSymbol() == gVoid) {
    Expr*      indices  = getIndices (promotion);
    Expr*      iterator = getIterator(promotion);
    CallExpr*  wrapCall = createPromotedCallForWrapper(promotion);
    BlockStmt* block    = new BlockStmt(wrapCall);
    bool       zippered = isCallExpr(iterator) ? true : false;

    loop = buildForallLoopStmt(indices, iterator, NULL, block, zippered);

    // Save the wrapCall to adjust it later
    promotion.wrapCalls.push_back(wrapCall);
  } else {
    loop = buildPromotionLoop(promotion, visibilityBlock, info, fastFollowerChecks);
  }

  retval->insertAtTail(new BlockStmt(loop));

  fn->defPoint->insertBefore(new DefExpr(retval));

  normalize(retval);

  fixUnresolvedSymExprsForPromotionWrapper(retval, fn);

  fixDefaultArgumentsInWrapCall(promotion);

  return retval;
}

static BlockStmt* buildPromotionLoop(PromotionInfo& promotion,
                                     BlockStmt* visibilityBlock,
                                     CallInfo&  info,
                                     bool       fastFollowerChecks) {
  FnSymbol*  wrapFn     = promotion.wrapperFn;

  Expr*      indices    = getIndices(promotion);
  Expr*      iterator   = getIterator(promotion);
  CallExpr*  wrapCall   = createPromotedCallForWrapper(promotion);
  bool       zippered   = isCallExpr(iterator) ? true : false;
  BlockStmt* yieldBlock = new BlockStmt();
  VarSymbol* yieldTmp   = newTemp("p_yield");

  wrapFn->addFlag(FLAG_ITERATOR_FN);
  wrapFn->removeFlag(FLAG_INLINE);

  yieldTmp->addFlag(FLAG_EXPR_TEMP);

  buildLeaderIterator(wrapFn, visibilityBlock, iterator);

  buildFollowerIterator(promotion, visibilityBlock, indices, iterator, wrapCall);

  if (fNoFastFollowers == false && fastFollowerChecks == true) {
    std::set<ArgSymbol*> requiresPromotion;

    collectPromotionFormals(promotion, requiresPromotion);

    // Build static (param) fast follower check functions
    buildFastFollowerCheck(true,  false, info, wrapFn, requiresPromotion);
    buildFastFollowerCheck(true,  true,  info, wrapFn, requiresPromotion);

    // Build dynamic fast follower check functions
    buildFastFollowerCheck(false, false, info, wrapFn, requiresPromotion);
    buildFastFollowerCheck(false, true,  info, wrapFn, requiresPromotion);
  }

  yieldBlock->insertAtTail(new DefExpr(yieldTmp));

  yieldBlock->insertAtTail(new CallExpr(PRIM_MOVE, yieldTmp, wrapCall));
  yieldBlock->insertAtTail(new CallExpr(PRIM_YIELD, yieldTmp));
  // Save the wrapCall to adjust it later
  promotion.wrapCalls.push_back(wrapCall);

  return ForLoop::buildForLoop(indices, iterator, yieldBlock, false, zippered);
}

static void buildLeaderIterator(FnSymbol* wrapFn,
                                BlockStmt* visibilityBlock,
                                Expr*     iterator) {
  SymbolMap   leaderMap;
  FnSymbol*   liFn       = wrapFn->copy(&leaderMap);

  Type*       tagType    = gLeaderTag->type;
  ArgSymbol*  liFnTag    = new ArgSymbol(INTENT_PARAM, "tag", tagType);

  VarSymbol*  liIndex    = newTemp("p_leaderIndex");
  VarSymbol*  liIterator = newTemp("p_leaderIterator");

  bool        zippered   = isCallExpr(iterator) ? true : false;
  const char* leaderName = zippered ? "_toLeaderZip" : "_toLeader";

  BlockStmt*  loop       = NULL;
  BlockStmt*  loopBody   = new BlockStmt(new CallExpr(PRIM_YIELD, liIndex));
  CallExpr*   toLeader   = NULL;

  INT_ASSERT(liFn->hasFlag(FLAG_RESOLVED) == false);

  iteratorLeaderMap.put(wrapFn, liFn);

  form_Map(SymbolMapElem, e, leaderMap) {
    if (Symbol* s = paramMap.get(e->key)) {
      paramMap.put(e->value, s);
    }
  }

  liIterator->addFlag(FLAG_EXPR_TEMP);

  toLeader = new CallExpr(leaderName, iterator->copy(&leaderMap));

  loop     = ForLoop::buildForLoop(new SymExpr(liIndex),
                                   new SymExpr(liIterator),
                                   loopBody,
                                   false,
                                   zippered);

  liFn->addFlag(FLAG_INLINE_ITERATOR);
  liFn->addFlag(FLAG_GENERIC);

  liFn->insertFormalAtTail(liFnTag);

  liFn->where = new BlockStmt(new CallExpr("==", liFnTag, gLeaderTag));

  liFn->body  = new BlockStmt();

  liFn->insertAtTail(new DefExpr(liIterator));

  liFn->insertAtTail(new CallExpr(PRIM_MOVE, liIterator, toLeader));

  liFn->insertAtTail(loop);

  theProgram->block->insertAtTail(new DefExpr(liFn));

  toBlockStmt(loopBody->parentExpr)->insertAtHead(new DefExpr(liIndex));

  normalize(liFn);

  liFn->instantiationPoint = visibilityBlock;
}

static void buildFollowerIterator(PromotionInfo& promotion,
                                  BlockStmt* visibilityBlock,
                                  Expr*     indices,
                                  Expr*     iterator,
                                  CallExpr* wrapCall) {
  SymbolMap  followerMap;
  FnSymbol*  fn         = promotion.fn;
  FnSymbol*  wrapFn     = promotion.wrapperFn;

  SymExpr*   symFalse         = new SymExpr(gFalse);

  ArgSymbol* fiFnTag          = NULL;
  ArgSymbol* fiFnFollower     = NULL;
  ArgSymbol* fastFollower     = NULL;

  VarSymbol* followerIterator = newTemp("p_followerIterator");

  FnSymbol*  fiFn             = wrapFn->copy(&followerMap);

  iteratorFollowerMap.put(wrapFn, fiFn);

  form_Map(SymbolMapElem, e, followerMap) {
    if (Symbol* s = paramMap.get(e->key)) {
      paramMap.put(e->value, s);
    }
  }

  INT_ASSERT(fiFn->hasFlag(FLAG_RESOLVED) == false);

  followerIterator->addFlag(FLAG_EXPR_TEMP);

  fiFnTag      = new ArgSymbol(INTENT_PARAM, "tag", gFollowerTag->type);
  fiFnFollower = new ArgSymbol(INTENT_BLANK, iterFollowthisArgname, dtAny);
  fastFollower = new ArgSymbol(INTENT_PARAM, "fast", dtBool, NULL, symFalse);

  fiFn->addFlag(FLAG_GENERIC);

  fiFn->insertFormalAtTail(fiFnTag);
  fiFn->insertFormalAtTail(fiFnFollower);
  fiFn->insertFormalAtTail(fastFollower);

  fiFn->where = new BlockStmt(new CallExpr("==", fiFnTag, gFollowerTag));

  fiFn->insertAtTail(new DefExpr(followerIterator));

  fiFn->insertAtTail(selectFollower(fastFollower,
                                    iterator,
                                    followerIterator,
                                    followerMap,
                                    fiFnFollower));

  fiFn->insertAtTail(followerForLoop(promotion,
                                     indices,
                                     iterator,
                                     followerIterator,
                                     followerMap,
                                     wrapCall));

  theProgram->block->insertAtTail(new DefExpr(fiFn));

  normalize(fiFn);

  fiFn->instantiationPoint = visibilityBlock;

  fixUnresolvedSymExprsForPromotionWrapper(fiFn, fn);
}

static CondStmt* selectFollower(ArgSymbol* fastFollower,
                                Expr*      iterator,
                                VarSymbol* followerIterator,
                                SymbolMap& followerMap,
                                ArgSymbol* fiFnFollower) {
  const char* name1 = NULL;
  CallExpr*   call1 = NULL;
  CallExpr*   move1 = NULL;

  const char* name2 = NULL;
  CallExpr*   call2 = NULL;
  CallExpr*   move2 = NULL;

  if (isCallExpr(iterator) == true) {
    name1 = "_toFastFollowerZip";
    name2 = "_toFollowerZip";

  } else {
    name1 = "_toFastFollower";
    name2 = "_toFollower";
  }

  call1 = new CallExpr(name1, iterator->copy(&followerMap), fiFnFollower);
  call2 = new CallExpr(name2, iterator->copy(&followerMap), fiFnFollower);

  move1 = new CallExpr(PRIM_MOVE, followerIterator, call1);
  move2 = new CallExpr(PRIM_MOVE, followerIterator, call2);

  return new CondStmt(new SymExpr(fastFollower), move1, move2);
}

static BlockStmt* followerForLoop(PromotionInfo& promotion,
                                  Expr*      indices,
                                  Expr*      iterator,
                                  VarSymbol* followerIterator,
                                  SymbolMap& followerMap,
                                  CallExpr*  wrapCall) {
  VarSymbol* yieldTmp = newTemp("p_yield");
  BlockStmt* block    = new BlockStmt();

  yieldTmp->addFlag(FLAG_EXPR_TEMP);

  block->insertAtTail(new DefExpr(yieldTmp));

  CallExpr* wrapCallCopy = wrapCall->copy(&followerMap);
  promotion.wrapCalls.push_back(wrapCallCopy);

  // Save the wrapCall to adjust it later
  block->insertAtTail(new CallExpr(PRIM_MOVE, yieldTmp, wrapCallCopy));

  block->insertAtTail(new CallExpr(PRIM_YIELD, yieldTmp));

  return ForLoop::buildForLoop(indices->copy(&followerMap),
                               new SymExpr(followerIterator),
                               block,
                               false,
                               isCallExpr(iterator) ? true : false);
}

static void initPromotionWrapper(PromotionInfo& promotion,
                                 BlockStmt* visibilityBlock) {

  FnSymbol* fn = promotion.fn;
  FnSymbol* retval = buildEmptyWrapper(fn);
  retval->instantiationPoint = visibilityBlock;

  retval->cname = astr("_promotion_wrap_", fn->cname);

  retval->addFlag(FLAG_PROMOTION_WRAPPER);

  if (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) == true) {
    retval->removeFlag(FLAG_DEFAULT_CONSTRUCTOR);
  }

  int i = 0;
  for_formals(formal, fn) {

    ArgSymbol* newFormal = NULL;

    if (promotion.defaulted[i] == false) {
      // Only create promotion wrapper formals for those arguments that
      // aren't receiving the default value.

      SET_LINENO(formal);
      newFormal = copyFormalForWrapper(formal);

      if (Symbol* p = paramMap.get(formal)) {
        paramMap.put(newFormal, p);
      }

      if (fn->_this == formal) {
        retval->_this = newFormal;
      }

      if (TypeSymbol* ts = promotion.promotedType[i]) {
        newFormal->type = ts->type;
      }

      retval->insertFormalAtTail(newFormal);
    }

    promotion.wrapperFormals.push_back(newFormal);

    i++;
  }

  promotion.wrapperFn = retval;
}

static Expr* getIndices(PromotionInfo& promotion) {
  FnSymbol* fn          = promotion.fn;
  CallExpr* indicesCall = new CallExpr("_build_tuple");
  Expr*     retval      = NULL;

  int i = 0;
  for_formals(formal, fn) {
    SET_LINENO(formal);

    if (promotion.promotedType[i] != NULL) {
      const char* name = astr("p_i_", istr(i+1));

      indicesCall->insertAtTail(new UnresolvedSymExpr(name));
    }
    i++;
  }

  // If there was only one promoted argument, don't call _build_tuple after all
  if (indicesCall->numActuals() == 1) {
    retval = indicesCall->get(1)->remove();
  } else {
    retval = indicesCall;
  }

  return retval;
}

static Expr* getIterator(PromotionInfo& promotion) {
  FnSymbol* fn           = promotion.fn;
  CallExpr* iteratorCall = new CallExpr("_build_tuple");
  Expr*     retval       = NULL;

  int i = 0;
  for_formals(formal, fn) {

    SET_LINENO(formal);

    if (promotion.promotedType[i] != NULL) {
      ArgSymbol* newFormal = promotion.wrapperFormals[i];
      INT_ASSERT(newFormal);
      iteratorCall->insertAtTail(newFormal);
    }
    i++;
  }

  // If there was only one promoted argument, don't call _build_tuple after all
  if (iteratorCall->numActuals() == 1) {
    retval = iteratorCall->get(1)->remove();
  } else {
    retval = iteratorCall;
  }

  return retval;
}

// Returns a CallExpr which contains the call to the original function
// as the last statement. Assumes that addDefaultsAndReorder will
// eventually be called for this call. It will have the wrong
// formal-actual alignment until that happens.
static CallExpr* createPromotedCallForWrapper(PromotionInfo& promotion) {
  FnSymbol* fn = promotion.fn;
  CallExpr* retval     = new CallExpr(fn);

  Vec<Symbol*> actuals;
  std::vector<ArgSymbol*> actualIdxToFormal;

  // When adding the call to the original function within a
  // promotion wrapper, we need to pass non-promoted arguments
  // to the original function through. But some of the arguments
  // might not need to be passed if we're using their default value.
  int i = 0;
  for_formals(formal, fn) {

    SET_LINENO(formal);

    Expr* actual = NULL;

    if (promotion.promotedType[i] != NULL) {
      // a promoted argument
      const char* name = astr("p_i_", istr(i+1));
      actual = new UnresolvedSymExpr(name);
    } else if (promotion.defaulted[i] == false) {
      ArgSymbol* newFormal = promotion.wrapperFormals[i];
      actual = new SymExpr(newFormal);
    }

    if (actual != NULL) {
      retval->insertAtTail(actual);
      actualIdxToFormal.push_back(formal);
    }

    i++;
  }

  return retval;
}

static void collectPromotionFormals(PromotionInfo& promotion,
                                    std::set<ArgSymbol*>& formals) {

  int numFormals = promotion.fn->numFormals();
  for (int i = 0; i < numFormals; i++) {
    if (promotion.promotedType[i] != NULL) {
      ArgSymbol* newFormal = promotion.wrapperFormals[i];
      formals.insert(newFormal);
    }
  }
}

static void fixUnresolvedSymExprsForPromotionWrapper(FnSymbol* wrapper,
                                                     FnSymbol* fn) {
  // Fix the UnresolvedSymExprs we inserted to the actualCall. For each
  // call to `fn`, pick out any UnresolvedSymExprs and look in the loop
  // body for a corresponding DefExpr.

  std::vector<CallExpr*> calls;

  collectCallExprs(wrapper, calls);

  for_vector(CallExpr, call, calls) {
    if (call->resolvedFunction() == fn) {
      for_actuals(actual, call) {
        if (UnresolvedSymExpr* unsym = toUnresolvedSymExpr(actual)) {
          std::vector<DefExpr*> defs;
          BlockStmt*            callBlock = NULL;
          BlockStmt*            loop      = NULL;
          bool                  found = false;

          callBlock = toBlockStmt(call->getStmtExpr()->parentExpr);
          loop      = toBlockStmt(callBlock->parentExpr);

          INT_ASSERT(loop && loop->isLoopStmt());

          collectDefExprs(loop, defs);

          for_vector(DefExpr, def, defs) {
            if (strcmp(def->sym->name, unsym->unresolved) == 0) {
              unsym->replace(new SymExpr(def->sym));

              found = true;
              break;
            }
          }

          INT_ASSERT(found);
        }
      }
    }
  }
}

static void fixDefaultArgumentsInWrapCall(PromotionInfo& promotion) {
  bool anyDefaulted = false;
  for (size_t i = 0; i < promotion.defaulted.size(); i++) {
    if (promotion.defaulted[i])
      anyDefaulted = true;
  }
  // Handle default arguments, if necessary.
  if (anyDefaulted) {

    // Set up actualIdxToFormal to indicate which defaults we need
    std::vector<ArgSymbol*> actualIdxToFormal;

    int i = 0;
    for_formals(formal, promotion.fn) {
      if (promotion.defaulted[i] == false) {
        actualIdxToFormal.push_back(formal);
      }
      i++;
    }

    // Update the calls
    for_vector(CallExpr, wrapCall, promotion.wrapCalls) {
      addDefaultsAndReorder(promotion.fn, wrapCall, NULL,
                            actualIdxToFormal,
                            false /* don't resolve it yet
                                     since other parts of the promotion
                                     wrapper aren't resolved */);
    }
  }
}

//
// In order for fast followers to trigger, the invoking loop requires a static
// and dynamic check. They determine at compile time if the iterands implement
// a fast follower, and at runtime if all the iterands can fast follow the
// leader. Here we build up the checks for an iterator record. We basically
// convert the iterator record into a tuple and call the "zip" check. Note
// that we only stuff the components of the iterator record that actually
// require promotion into the tuple.
//
// i.e. we build up something like:
//
//    _iteratorRecord has a field for each formal in the promoted function.
//    `A + 2.0 * C` results in a record with fields for each array and the real
//
//    proc chpl__dynamicFastFollowCheck(x: _iteratorRecord, lead) {
//      // tuple that only has elements for fields that require promotion
//      var promotion_tup: recordToPromotionTuple(x);
//      var cur_tup_idx = 1;
//      for param fieldNum in 1..numFields(x.type) {
//        var field = getField(x.type, fieldNum);
//        if requiresPromotion(field.type, x) {
//          promotion_tup(cur_tup_idx) = field;
//          cur_tup_idx += 1;
//        }
//      }
//      return chpl__dynamicFastFollowCheckZip(promotion_tup, lead);
//    }
//
// However, since the iterator record isn't fully built (none of the fields
// exist yet), we use a primitive as a placeholder. When the record is filled
// in during iterator lowering, we replace the primitive with the actual field.
//
static void buildFastFollowerCheck(bool                  isStatic,
                                   bool                  addLead,
                                   CallInfo&             info,
                                   FnSymbol*             wrapper,
                                   std::set<ArgSymbol*>& requiresPromotion) {
  const char* fnName     = NULL;
  FnSymbol*   checkFn    = NULL;

  ArgSymbol*  x          = new ArgSymbol(INTENT_BLANK, "x", dtIteratorRecord);

  CallExpr*   buildTuple = new CallExpr("_build_tuple_always_allow_ref");

  Symbol*     pTup       = newTemp("p_tup");
  Symbol*     returnTmp  = newTemp("p_ret");
  CallExpr*   forward    = NULL;

  returnTmp->addFlag(FLAG_EXPR_TEMP);
  returnTmp->addFlag(FLAG_MAYBE_PARAM);

  if (isStatic == true) {
    fnName          = "chpl__staticFastFollowCheck";

    checkFn         = new FnSymbol(fnName);
    checkFn->retTag = RET_PARAM;

  } else {
    fnName          = "chpl__dynamicFastFollowCheck";

    checkFn         = new FnSymbol(fnName);
    checkFn->retTag = RET_VALUE;
  }

  checkFn->addFlag(FLAG_GENERIC);

  checkFn->insertFormalAtTail(x);

  if (addLead == true) {
    ArgSymbol* lead = new ArgSymbol(INTENT_BLANK, "lead", dtAny);

    checkFn->insertFormalAtTail(lead);

    forward = new CallExpr(astr(fnName, "Zip"), pTup, lead);

  } else {
    forward = new CallExpr(astr(fnName, "Zip"), pTup);
  }

  for_formals(formal, wrapper) {
    if (requiresPromotion.count(formal) > 0) {
      Symbol*      field       = new VarSymbol(formal->name, formal->type);

      PrimitiveTag primTag     = PRIM_ITERATOR_RECORD_FIELD_VALUE_BY_FORMAL;
      CallExpr*    byFormal    = new CallExpr(primTag,   x,     formal);
      CallExpr*    moveToField = new CallExpr(PRIM_MOVE, field, byFormal);

      checkFn->insertAtTail(new DefExpr(field));

      checkFn->insertAtTail(moveToField);

      buildTuple->insertAtTail(new SymExpr(field));
    }
  }

  CallExpr* typeOfLhs = new CallExpr(PRIM_TYPEOF, x);
  CallExpr* typeOfRhs = new CallExpr(PRIM_TYPEOF, info.call->copy());

  checkFn->where = new BlockStmt(new CallExpr("==", typeOfLhs, typeOfRhs));

  checkFn->insertAtTail(new DefExpr(pTup));
  checkFn->insertAtTail(new CallExpr(PRIM_MOVE, pTup, buildTuple));

  checkFn->insertAtTail(new DefExpr(returnTmp));
  checkFn->insertAtTail(new CallExpr(PRIM_MOVE,   returnTmp, forward));
  checkFn->insertAtTail(new CallExpr(PRIM_RETURN, returnTmp));

  theProgram->block->insertAtTail(new DefExpr(checkFn));

  normalize(checkFn);

  checkFn->instantiationPoint = getVisibilityBlock(info.call);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static FnSymbol* buildEmptyWrapper(FnSymbol* fn) {
  FnSymbol* wrapper = new FnSymbol(fn->name);

  wrapper->addFlag(FLAG_WRAPPER);

  wrapper->addFlag(FLAG_INVISIBLE_FN);

  wrapper->addFlag(FLAG_INLINE);

  if (fn->hasFlag(FLAG_INIT_COPY_FN)) {
    wrapper->addFlag(FLAG_INIT_COPY_FN);
  }

  if (fn->hasFlag(FLAG_AUTO_COPY_FN)) {
    wrapper->addFlag(FLAG_AUTO_COPY_FN);
  }

  if (fn->hasFlag(FLAG_AUTO_DESTROY_FN)) {
    wrapper->addFlag(FLAG_AUTO_DESTROY_FN);
  }

  if (fn->hasFlag(FLAG_DONOR_FN)) {
    wrapper->addFlag(FLAG_DONOR_FN);
  }

  if (fn->hasFlag(FLAG_NO_PARENS)) {
    wrapper->addFlag(FLAG_NO_PARENS);
  }

  if (fn->hasFlag(FLAG_CONSTRUCTOR)) {
    wrapper->addFlag(FLAG_CONSTRUCTOR);
  }

  if (fn->hasFlag(FLAG_FIELD_ACCESSOR)) {
    wrapper->addFlag(FLAG_FIELD_ACCESSOR);
  }

  if (fn->hasFlag(FLAG_REF_TO_CONST)) {
    wrapper->addFlag(FLAG_REF_TO_CONST);
  }

  if (!fn->isIterator()) { // getValue is var, not iterator
    wrapper->retTag = fn->retTag;
  }

  if (fn->isMethod() == true) {
    wrapper->setMethod(true);
  }

  if (fn->hasFlag(FLAG_METHOD_PRIMARY)) {
    wrapper->addFlag(FLAG_METHOD_PRIMARY);
  }

  if (fn->hasFlag(FLAG_ASSIGNOP)) {
    wrapper->addFlag(FLAG_ASSIGNOP);
  }

  if (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR)) {
    wrapper->addFlag(FLAG_DEFAULT_CONSTRUCTOR);
  }

  if (fn->hasFlag(FLAG_LAST_RESORT)) {
    wrapper->addFlag(FLAG_LAST_RESORT);
  }

  if (fn->hasFlag(FLAG_COMPILER_GENERATED)) {
    wrapper->addFlag(FLAG_WAS_COMPILER_GENERATED);
  }

  if (fn->hasFlag(FLAG_VOID_NO_RETURN_VALUE)) {
    wrapper->addFlag(FLAG_VOID_NO_RETURN_VALUE);
  }

  wrapper->addFlag(FLAG_COMPILER_GENERATED);

  if (fn->throwsError())
    wrapper->throwsErrorInit();

  return wrapper;
}

/************************************* | **************************************
*                                                                             *
* copy a formal and make the copy have blank intent. If the formal to copy    *
* has out intent or inout intent, flag the copy to make sure it is a          *
* reference.                                                                  *
*                                                                             *
* If the formal is ref intent, leave it as ref on the wrapper formal.         *
*                                                                             *
************************************** | *************************************/

static ArgSymbol* copyFormalForWrapper(ArgSymbol* formal) {
  ArgSymbol* wrapperFormal = formal->copy();

  if (formal->intent == INTENT_OUT ||
      formal->intent == INTENT_INOUT ||
      formal->hasFlag(FLAG_WRAP_WRITTEN_FORMAL)) {
    wrapperFormal->addFlag(FLAG_WRAP_WRITTEN_FORMAL);
  }

  if (formal->intent != INTENT_REF && formal->intent != INTENT_CONST_REF) {
    wrapperFormal->intent = INTENT_BLANK;
  }

  return wrapperFormal;
}
