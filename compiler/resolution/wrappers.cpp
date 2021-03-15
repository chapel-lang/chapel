/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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
#include "DecoratedClassType.h"
#include "driver.h"
#include "expr.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "iterator.h"
#include "optimizations.h"
#include "passes.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "resolveIntents.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "TransformLogicalShortCircuit.h"
#include "visibleFunctions.h"

#include <map>
#include <utility>

static void adjustForOperatorMethod(FnSymbol* fn, CallInfo& info,
                                    std::vector<ArgSymbol*>& actualFormals);

static void addDefaultTokensAndReorder(FnSymbol *fn,
                                       CallInfo& info,
                                       std::vector<ArgSymbol*>& actualIdxToFml);

static void handleDefaultArg(FnSymbol *fn, CallExpr* call,
                             ArgSymbol* formal, SymExpr* actual,
                             SymbolMap& copyMap,
                             bool resolveNewCode);



static void       reorderActuals(FnSymbol*                fn,
                                 CallInfo&                info,
                                 std::vector<ArgSymbol*>& actualIdxToFormal);

static void removeNamedExprs(FnSymbol* fn, CallExpr* call);

static void handleCoercion(FnSymbol* fn, CallExpr* call,
                           ArgSymbol* formal, SymExpr* actual,
                           SymbolMap& copyMap,
                           SymbolMap& inTmpToActualMap);

static void handleInIntent(FnSymbol* fn, CallExpr* call,
                           ArgSymbol* formal, SymExpr* actual,
                           SymbolMap& copyMap,
                           SymbolMap& inTmpToActualMap);

static void handleOutIntents(FnSymbol* fn, CallExpr* call,
                             SymbolMap& inTmpToActualMap);

bool       isPromotionRequired(FnSymbol* fn, CallInfo& info,
                               std::vector<ArgSymbol*>& actualIdxToFormal);

static FnSymbol*  promotionWrap(FnSymbol* fn,
                                CallInfo& info,
                                std::vector<ArgSymbol*>& actualIdxToFormal,
                                bool      buildFastFollowerChecks);

static FnSymbol*  buildEmptyWrapper(FnSymbol* fn);

static ArgSymbol* copyFormalForWrapper(ArgSymbol* formal);

static bool typeExprReturnsType(ArgSymbol* formal);

static IntentTag getIntent(ArgSymbol* formal);

typedef struct DefaultExprFnEntry_s {
  FnSymbol* defaultExprFn;
  std::vector<std::pair<ArgSymbol*,ArgSymbol*> > usedFormals;
} DefaultExprFnEntry;

typedef std::map<ArgSymbol*, DefaultExprFnEntry> formalToDefaultExprEntryMap;
formalToDefaultExprEntryMap formalToDefaultExprEntry;

//
// Return true if 'innerFn' is an initializer called within a _new wrapper or
// a default initializer.
//
static bool isNestedNewOrDefault(FnSymbol* innerFn, CallExpr* innerCall) {
  bool ret = false;

  if (FnSymbol* parentFn = toFnSymbol(innerCall->parentSymbol)) {
    if (innerFn->isInitializer()) {
      if (parentFn->hasFlag(FLAG_NEW_WRAPPER)) {
        ret = true;
      } else if (parentFn->isDefaultInit()) {
        // Likely a super.init()
        ret = true;
      }
    }
  }

  return ret;
}

/************************************* | **************************************
*                                                                             *
* actualIdxToFormal[i] is the formal argument that corresponds to i-th actual *
* (counting from 0). 'actualIdxToFormal' can be modified here.                *
* (This mapping is nontrivial when named arguments are used)                  *
*                                                                             *
************************************** | *************************************/

FnSymbol* wrapAndCleanUpActuals(FnSymbol*                fn,
                                CallInfo&                info,
                                std::vector<ArgSymbol*>& actualIdxToFormal,
                                bool                     fastFollowerChecks) {
  int       numActuals = static_cast<int>(actualIdxToFormal.size());
  FnSymbol* retval     = fn;
  bool      anyDefault = false;

  if (isPromotionRequired(retval, info, actualIdxToFormal) == true) {
    // Note: promotionWrap will handle default args in the inner call
    // to the original function, and it will create a different promotion
    // wrapper for each set of default arguments needed.
    retval = promotionWrap(retval, info, actualIdxToFormal, fastFollowerChecks);
  }

  if (numActuals < retval->numFormals()) {
    // If we don't have the right number of arguments, add placeholders
    // for defaulted arguments and for skipped operator arguments.
    adjustForOperatorMethod(retval, info, actualIdxToFormal);
    addDefaultTokensAndReorder(retval, info, actualIdxToFormal);
    anyDefault = true;
  } else if (numActuals > retval->numFormals() && fn->hasFlag(FLAG_OPERATOR)) {
    // If we don't have the right number of arguments, remove unnecessary
    // actuals for operator calls
    adjustForOperatorMethod(retval, info, actualIdxToFormal);
  } else {
    // handle reordering only
    if (actualIdxToFormal.size() > 1) {
      reorderActuals(retval, info, actualIdxToFormal);
    }
  }

  // From this point forward, this function no longer uses the CallInfo
  CallExpr* call = info.call;

  // Remove any NamedExprs since they are no longer needed
  // now that the arguments are in the correct order
  removeNamedExprs(retval, call);

  // Now, consider each argument, and handle:
  //  coercion
  //  in intent
  //  default argument values
  //  out intent
  if (call->numActuals() > 0) {
    // Create a copyMap to handle cases like
    //   proc f(a, b=a, c:a.type)
    // where a formal arguments refer to previous formals
    SymbolMap copyMap;
    // For inout arguments, a map from the in-copy-temp to the real actual
    SymbolMap inTmpToActualMap;

    Expr* currActual = call->get(1);
    Expr* nextActual = nullptr;
    for_formals(formal, retval) {
      nextActual = currActual->next;

      SET_LINENO(currActual);

      SymExpr* actual = toSymExpr(currActual);
      INT_ASSERT(actual);
      // TODO: if named expr, can we remove it above? ordering
      // should be established by this point.

      if (anyDefault) {
        // Fix any gUnknown arguments added by the above defaults and
        // replace them with the real defaults.
        // Also handles filling in the `out` part for `inout`.
        handleDefaultArg(retval, call, formal, actual, copyMap, true);
      }

      // And then handle coercions (in case the default expression
      // needs a coercion to the actual type).
      handleCoercion(retval, call, formal, actual, copyMap, inTmpToActualMap);

      // adjust for in intent
      handleInIntent(retval, call, formal, actual, copyMap, inTmpToActualMap);

      copyMap.put(formal, actual->symbol());

      currActual = nextActual;
    }

    // Now handle all out intents
    // These are done in a separate step because their adjusted values
    // are not available on the way into the function (only on the way out).
    // Consider e.g. proc g(out x, y = x)
    // Here 'y = x' should refer to the value of 'x' on the way in to the fn.
    handleOutIntents(retval, call, inTmpToActualMap);
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* "Wrap" a call with fewer actuals than the number of formals.                *
* This implies that the Chapel procedure has formals with "default" values.   *
*                                                                             *
* This is handled by finding/generating a procedure with the same number of   *
* formals as the call's actuals and then providing the default value for the  *
* unspecified formals.                                                        *
*                                                                             *
************************************** | *************************************/

static bool      defaultedFormalUsesDefaultForType(ArgSymbol* formal);

static bool      formalDefaultIsVariable(ArgSymbol* formal);

static void      copyFormalTypeExpr(ArgSymbol* formal,
                                    BlockStmt* body,
                                    Symbol* temp);

static void      defaultedFormalApplyDefault(ArgSymbol* formal,
                                             BlockStmt* wrapFn,
                                             VarSymbol* temp,
                                             Expr*      fromExpr);

static void      defaultedFormalApplyDefaultValue(FnSymbol*  fn,
                                                  ArgSymbol* formal,
                                                  IntentTag formalIntent,
                                                  BlockStmt* wrapFn,
                                                  VarSymbol* temp);

static Symbol* createDefaultedActual(FnSymbol*  fn,
                                     ArgSymbol* formal,
                                     CallExpr*  call,
                                     BlockStmt* body,
                                     SymbolMap& copyMap);

static void adjustForOperatorMethod(FnSymbol* fn, CallInfo& info,
                                    std::vector<ArgSymbol*>& actualFormals) {
  if (fn->hasFlag(FLAG_OPERATOR)) {
    if (fn->hasFlag(FLAG_METHOD)) {
      int numFormals = fn->numFormals();
      if (numFormals >= info.call->numActuals() + 2) {
        // Insert an arg for "this" and for the method token, in reverse order
        // because inserting at the start of the call.
        info.call->insertAtHead(new SymExpr(fn->_this->typeInfo()->symbol));
        info.call->insertAtHead(new SymExpr(gMethodToken));

        // Update the CallInfo actuals and actualNames fields
        info.actuals.clear();
        info.actualNames.clear();
        for_actuals(actual, info.call) {
          SymExpr* se = toSymExpr(actual);
          INT_ASSERT(se);
          info.actuals.add(se->symbol());
          info.actualNames.add(nullptr);
        }

        actualFormals.resize(fn->numFormals());
        int i = 0;
        for_formals(formal, fn) {
          actualFormals[i] = formal;
          i++;
        }
      }
    } else {
      bool removeNext = false;
      for_actuals(actual, info.call) {
        if (actual->typeInfo() == dtMethodToken && !fn->hasFlag(FLAG_METHOD)) {
          // Remove the method token and tell ourselves to remove the "this"
          // actual as well if the function being called is just an operator
          // rather than an operator method
          actual->remove();
          removeNext = true;
        } else if (removeNext) {
          actual->remove();
          removeNext = false;
        }
      }
    }
  }
}

// info is used to handle out-of-order named arguments
// if there aren't any out-of-order arguments (as with promotion)
// it can be nullptr.
static
void addDefaultTokensAndReorder(FnSymbol *fn,
                                CallInfo& info,
                                std::vector<ArgSymbol*>& actualFormals) {

  int numFormals = fn->numFormals();
  std::vector<Symbol*> newActuals(numFormals);

  // Gather the actuals into newActuals with NULLs where
  // we need to fill in a default. This also happens
  // to address the need to reorder the actuals.
  int i = 0;
  for_formals(formal, fn) {
    Symbol* actualSym = nullptr;
    int j = 0;
    for_vector(ArgSymbol, arg, actualFormals) {
      if (arg == formal) {
        actualSym = info.actuals.v[j];
      }
      j++;
    }

    newActuals[i] = actualSym;

    i++;
  }

  // Remove the actuals from the call
  for_actuals(actual, info.call) {
    actual->remove();
  }

  // Add the actuals back in the call along with gUnknown for
  // defaulted arguments (we'll fix that in replaceDefaultTokensWithDefaults)
  for_vector_allowing_0s(Symbol, actual, newActuals) {
    if (actual != nullptr) {
      info.call->insertAtTail(actual);
    } else {
      info.call->insertAtTail(new SymExpr(gUnknown));
    }
    i++;
  }

  // Update the CallInfo actuals and actualNames fields
  info.actuals.clear();
  info.actualNames.clear();
  for_actuals(actual, info.call) {
    SymExpr* se = toSymExpr(actual);
    INT_ASSERT(se);
    info.actuals.add(se->symbol());
    info.actualNames.add(nullptr);
  }

  // update actualFormals[] in case it is used again
  // Since we addressed reordering above, this is always just
  // the formals in order.
  actualFormals.resize(fn->numFormals());
  i = 0;
  for_formals(formal, fn) {
    actualFormals[i] = formal;
    i++;
  }
}

static void handleDefaultArg(FnSymbol *fn, CallExpr* call,
                             ArgSymbol* formal, SymExpr* actual,
                             SymbolMap& copyMap,
                             bool resolveNewCode) {
  if (actual->symbol() == gUnknown) {
    // it's a defaulted argument, gUnknown is the placeholder
  } else {
    // it's not a defaulted argument
    return;
  }

  // Create a Block to store the default values
  // We'll flatten this back out again in a minute.
  BlockStmt* body = new BlockStmt(BLOCK_SCOPELESS);
  call->getStmtExpr()->insertBefore(body);

  // Fill it in with a default argument.
  Symbol* newActual = createDefaultedActual(fn, formal, call, body, copyMap);

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
    if (Symbol* param = paramMap.get(newActual))
      newActual = param;

    // Check that newActuals are coercible to the formals
    bool actualIsTypeAlias = newActual->hasFlag(FLAG_TYPE_VARIABLE);
    bool formalIsTypeAlias = formal->hasFlag(FLAG_TYPE_VARIABLE);
    bool formalIsParam     = formal->hasFlag(FLAG_INSTANTIATED_PARAM) ||
                             formal->intent == INTENT_PARAM;

    // Use the value types to work around issues with string literals.
    // Value types should be sufficient for the check needed here, though.
    Type* actualValType = newActual->getValType();
    Type* formalValType = formal->getValType();

    bool promotes = false;
    bool dispatches = canDispatch(actualValType, newActual,
                                  formalValType, formal, fn,
                                  &promotes, nullptr, formalIsParam);

    if (actualIsTypeAlias != formalIsTypeAlias ||
        dispatches == false ||
        promotes == true) {

      if (fn->hasFlag(FLAG_COMPILER_GENERATED) &&
          (fn->name == astrNew ||
           fn->name == astrInit || fn->name == astrInitEquals)) {
        USR_FATAL_CONT(call, "cannot initialize %s from %s",
                       toString(formalValType), toString(actualValType));
        USR_PRINT(formal, "when initializing %s with default value for %s",
                  toString(fn->getFormal(1)->getValType()), formal->name);
      } else {
        USR_FATAL_CONT(call, "cannot initialize %s from %s",
                       toString(formalValType), toString(actualValType));
        USR_PRINT(formal, "when calling %s with a default value for %s",
                  fn->name, formal->name);
      }
    }
  }

  // Replace the gUnknown in the call with the new actual
  actual->setSymbol(newActual);

  // Adjust AST location to be call site
  if (fn->getModule()->modTag != MOD_USER)
    reset_ast_loc(body, call);

  // Flatten body
  body->flattenAndRemove();
}

static DefaultExprFnEntry buildDefaultedActualFn(FnSymbol*  fn,
                                                 ArgSymbol* formal) {

  DefaultExprFnEntry ret;

  SET_LINENO(formal);


  // Set up the wrapper function
  FnSymbol* wrapper = new FnSymbol(astr(fn->name, "_default_", formal->name));
  ret.defaultExprFn = wrapper;

  wrapper->addFlag(FLAG_INVISIBLE_FN);
  wrapper->addFlag(FLAG_INLINE);
  wrapper->addFlag(FLAG_LINE_NUMBER_OK);
  wrapper->retTag = RET_VALUE;

  if (fn->hasFlag(FLAG_METHOD)) {
    wrapper->addFlag(FLAG_METHOD);
  }

  if (fn->hasFlag(FLAG_METHOD_PRIMARY)) {
    wrapper->addFlag(FLAG_METHOD_PRIMARY);
  }

  wrapper->setInstantiationPoint(fn->instantiationPoint());

  if (fn->hasFlag(FLAG_LAST_RESORT)) {
    wrapper->addFlag(FLAG_LAST_RESORT);
  }

  if (fn->hasFlag(FLAG_COMPILER_GENERATED)) {
    wrapper->addFlag(FLAG_WAS_COMPILER_GENERATED);
  }

  wrapper->addFlag(FLAG_DEFAULT_ACTUAL_FUNCTION);
  wrapper->addFlag(FLAG_COMPILER_GENERATED);
  wrapper->addFlag(FLAG_MAYBE_PARAM);
  wrapper->addFlag(FLAG_MAYBE_TYPE);
  wrapper->setGeneric(false); // We are here only when resolving a call.

  if (fn->throwsError())
    wrapper->throwsErrorInit();

  if (formal->hasFlag(FLAG_UNSAFE))
    wrapper->addFlag(FLAG_UNSAFE);

  if (formal->intent == INTENT_TYPE ||
      formal->hasFlag(FLAG_TYPE_VARIABLE))
    wrapper->retTag = RET_TYPE;

  if (formal->intent == INTENT_PARAM)
    wrapper->retTag = RET_PARAM;

  SymbolMap copyMap;

  // Set up the arguments
  if (fn->hasFlag(FLAG_METHOD) &&
      fn->_this != nullptr &&
      fn->isInitializer() == false &&
      fn->isCopyInit() == false) {
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

        // Important to use ref / const ref intent always for records.
        IntentTag intent = INTENT_BLANK;
        switch (fnFormal->intent) {
          case INTENT_BLANK:
          case INTENT_IN:
          case INTENT_CONST:
          case INTENT_CONST_IN:
            if (isRecord(fnFormal->typeInfo()) &&
                !fnFormal->hasFlag(FLAG_TYPE_VARIABLE)) {
              // always use 'const ref' intent in these cases for records
              // (even if the record has e.g. in default intent)
              intent = INTENT_CONST_REF;
            } else {
              intent = fnFormal->intent;
            }
            break;
          case INTENT_PARAM:
          case INTENT_TYPE:
          case INTENT_CONST_REF:
          case INTENT_REF_MAYBE_CONST:
          case INTENT_REF:
            // These are OK even for records
            intent = fnFormal->intent;
            break;
          case INTENT_OUT:
          case INTENT_INOUT:
            // Use ref
            intent = INTENT_REF;
            break;
          // intentionally no default
        }

        ArgSymbol* newFormal = nullptr;
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
  BlockStmt* block = new BlockStmt(BLOCK_SCOPELESS);
  wrapper->insertAtTail(block);

  wrapper->insertAtTail(new CallExpr(PRIM_RETURN, rvv));

  // We create a separate temp to store the result of the default
  // argument expression so that we can query its type while setting
  // up the return intent for the function. (The return value variable
  // gets special treatment and would be harder to use in this way).
  //
  // Use the name of the formal for better error messages.
  VarSymbol* temp   = newTemp(formal->name);

  temp->addFlag(FLAG_USER_VARIABLE_NAME);
  // Suppress lvalue errors, which are easily encountered with default
  // wrappers for initializers.
  temp->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);

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
      formal->type   != dtMethodToken) {
    formalIntent = getIntent(formal);
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
    defaultedFormalApplyDefault(formal, block, temp, nullptr);

  } else if (formalIntent == INTENT_OUT) {
    defaultedFormalApplyDefault(formal, block, temp, nullptr);

  } else {
    defaultedFormalApplyDefaultValue(fn, formal, formalIntent, block, temp);
  }

  // Update references to previous arguments to use the
  // default-expr-function formals
  update_symbols(block, &copyMap);

  // Add the function to the tree
  fn->defPoint->insertAfter(new DefExpr(wrapper));

  normalize(block);
  resolveBlockStmt(block);

  if (temp->isRef() && (formalIntent & INTENT_FLAG_REF) == 0) {
    // we may just pass false for definedConst into initCopy, but this feels
    // more principled
    Symbol *definedConst = rvv->hasFlag(FLAG_CONST) ?  gTrue : gFalse;
    CallExpr* copy = new CallExpr(astr_initCopy, temp, definedConst);
    block->insertAtTail(new CallExpr(PRIM_MOVE, rvv, copy));
    resolveCallAndCallee(copy);
  } else {
    block->insertAtTail(new CallExpr(PRIM_MOVE, rvv, temp));
  }
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
  DefaultExprFnEntry* entry = nullptr;
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

  SET_LINENO(formal);

  VarSymbol* temp   = newTemp(astr("default_arg_", formal->name));

  temp->addFlag(FLAG_DEFAULT_ACTUAL);
  // Note: FLAG_EXPR_TEMP might be removed elsewhere in resolution if 'temp' is
  // an array
  temp->addFlag(FLAG_EXPR_TEMP);
  temp->addFlag(FLAG_MAYBE_PARAM);
  temp->addFlag(FLAG_MAYBE_TYPE);
  // Allow e.g. proc f(ref x=returnSomeValue())
  temp->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);

  // TODO: do we need to add FLAG_INSERT_AUTO_DESTROY here?
  if (formal->intent == INTENT_IN || formal->intent == INTENT_CONST_IN) {
    temp->addFlag(FLAG_NO_AUTO_DESTROY);
  }

  body->insertAtTail(new DefExpr(temp));

  // Add a call to the defaulted formals fn passing in the
  // appropriate actual values.
  CallExpr* newCall = new CallExpr(entry->defaultExprFn);
  bool throws = newCall->resolvedFunction()->throwsError();

  // Add method token, this if needed
  if (fn->hasFlag(FLAG_METHOD) &&
      fn->_this != nullptr &&
      fn->isInitializer() == false &&
      fn->isCopyInit() == false) {
    // Set up mt and _this arguments
    newCall->insertAtTail(gMethodToken);
    Symbol* usedFormal = fn->_this;
    Symbol* mapTo = copyMap.get(usedFormal);
    INT_ASSERT(mapTo); // Should have another actual!
    newCall->insertAtTail(new SymExpr(mapTo));
  }

  // If the new call throws and it was in a 'try'/'try!' before
  // we moved it, put it into a new 'try'/'try!'
  if (throws && call->tryTag == TRY_TAG_IN_TRYBANG)
    newCall = new CallExpr(PRIM_TRYBANG_EXPR, newCall);

  if (throws && call->tryTag == TRY_TAG_IN_TRY)
    newCall = new CallExpr(PRIM_TRY_EXPR, newCall);

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

static bool defaultedFormalUsesDefaultForType(ArgSymbol* formal) {
  bool retval = false;

  if (formal->defaultExpr->body.length == 1) {
    if (SymExpr* se = toSymExpr(formal->defaultExpr->body.tail)) {
      retval = se->symbol() == gTypeDefaultToken;
    }
  }

  return retval;
}

static bool formalDefaultIsVariable(ArgSymbol* formal) {
  Expr* e = formal->defaultExpr->body.tail;
  INT_ASSERT(e != nullptr);

  SymExpr* se = toSymExpr(e);
  if (se) {
    Symbol* s = se->symbol();
    // Is it nil or other special internal types?
    if (s->type == dtNil || s->type == dtNothing || s->type == dtVoid)
      return false; // treat these as non-variables (no ref, please)
    // Is it an immediate?
    if (s->isImmediate() || s->isParameter())
      return false; // treat these as non-variables (no ref, please)
    // Is it another argument?
    if (isArgSymbol(se->symbol()))
      return true; // other arguments are referenced
    // Is it an outer variable?
    if (s->defPoint->parentSymbol != formal)
      return true; // outer variables are referenced
  }

  // Otherwise, it is a call of some sort, possibly including
  // a VarSymbol storing the result of a call (as is the case with new).
  return false;
}

// sets temp to be the (runtime) type expression in formal's type
static void copyFormalTypeExpr(ArgSymbol* formal,
                               BlockStmt* body,
                               Symbol* temp) {
  if (formal->typeExpr != nullptr) {
    BlockStmt* typeExpr = formal->typeExpr->copy();
    Expr*      lastExpr = nullptr;

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
      Expr*     move     = nullptr;
      Expr*     typeExpr = nullptr;

      if (lastCall != nullptr && lastCall->isPrimitive(PRIM_MOVE) == true) {
        typeExpr = lastCall->get(1)->copy();

      } else {
        typeExpr = lastExpr->remove();
      }

      move = new CallExpr(PRIM_MOVE, temp, typeExpr);
      body->insertAtTail(move);
    }

  } else {
    Expr* typeExpr = new SymExpr(formal->type->symbol);
    Expr* move = new CallExpr(PRIM_MOVE, temp, typeExpr);
    body->insertAtTail(move);
  }
}

// Creates the default value for the type if fromExpr=nullptr.
// If fromExpr!=nullptr, coerces fromExpr into the type of the formal.
static void defaultedFormalApplyDefault(ArgSymbol* formal,
                                        BlockStmt* body,
                                        VarSymbol* temp,
                                        Expr* fromExpr) {
  Symbol* typeTmp = nullptr;
  if (formal->typeExpr != nullptr) {
    typeTmp = newTemp("_formal_type");
    typeTmp->addFlag(FLAG_TYPE_VARIABLE);
    body->insertAtTail(new DefExpr(typeTmp));
    copyFormalTypeExpr(formal, body, typeTmp);
  } else {
    typeTmp = formal->type->symbol;
  }

  if (formal->hasFlag(FLAG_TYPE_VARIABLE) == true) {
    body->insertAtTail(new CallExpr(PRIM_MOVE, temp, typeTmp));

  } else {
    Expr* initExpr = nullptr;
    if (fromExpr == nullptr) {
      initExpr = new CallExpr(PRIM_DEFAULT_INIT_VAR, temp, typeTmp);
    } else {
      SymExpr* fromExprSe = toSymExpr(fromExpr);
      if (fromExprSe && fromExprSe->symbol() == gNoInit) {
        initExpr = new CallExpr(PRIM_NOINIT_INIT_VAR, temp, typeTmp);
      } else {
        initExpr = new CallExpr(PRIM_MOVE,
                                temp,
                                new CallExpr(PRIM_COERCE, fromExpr, typeTmp));
      }
    }
    body->insertAtTail(initExpr);
  }
}

static void defaultedFormalApplyDefaultValue(FnSymbol*  fn,
                                             ArgSymbol* formal,
                                             IntentTag formalIntent,
                                             BlockStmt* body,
                                             VarSymbol* temp) {
  BlockStmt* defaultExpr = formal->defaultExpr->copy();
  Expr*      fromExpr    = nullptr;

  for_alist(expr, defaultExpr->body) {
    body->insertAtTail(expr->remove());
  }

  fromExpr = body->body.tail->remove();

  // If the default expression is a call or dtNil,
  // don't use PRIM_ADDR_OF on it.
  // Instead, we'll set temp to a ref or not based on FLAG_MAYBE_REF.
  if ((formalIntent & INTENT_FLAG_REF) != 0 &&
      formalDefaultIsVariable(formal))
  {
    fromExpr = new CallExpr(PRIM_ADDR_OF, fromExpr);
  }

  //
  // If an array formal with the in-intent has a type expr, like so:
  //     in A : [<something>] T = <fromExpr>
  // Then the runtime type needs to be preserved. We can accomplish this by
  // creating a default from the type and assigning the default expression
  // result into the new default:
  //     var A : [<something>] T;
  //     A = <fromExpr>;
  //
  // This case also handles coercions from the default to the formal type
  if (formalIntent & INTENT_FLAG_IN &&
      typeExprReturnsType(formal)) {
    VarSymbol* nt = newTemp(temp->type);
    body->insertAtTail(new DefExpr(nt));

    // When passing the 'fromExpr' below, the result is a PRIM_COERCE.
    defaultedFormalApplyDefault(formal, body, nt, fromExpr);
    fromExpr = new SymExpr(nt);
  }

  body->insertAtTail(new CallExpr(PRIM_MOVE, temp, fromExpr));

  if (formal->intent == INTENT_INOUT) {
    INT_ASSERT(!temp->hasFlag(FLAG_EXPR_TEMP));
    temp->removeFlag(FLAG_MAYBE_PARAM);
  }
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

    // remove all actuals
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

static void removeNamedExprs(FnSymbol* fn, CallExpr* call) {
  if (call->numActuals() > 0) {
    Expr* currActual = call->get(1);
    Expr* nextActual = nullptr;
    for_formals(formal, fn) {
      nextActual = currActual->next;

      if (NamedExpr* named = toNamedExpr(currActual)) {
        INT_ASSERT(named->name == formal->name);
        Expr* actual = named->actual->remove();
        currActual->replace(actual);
        INT_ASSERT(isSymExpr(actual));
      } else {
        INT_ASSERT(isSymExpr(currActual));
      }

      currActual = nextActual;
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


static void      addArgCoercion(FnSymbol*  fn,
                                CallExpr*  call,
                                ArgSymbol* formal,
                                SymExpr*   actual,
                                bool&      checkAgain);

static void handleCoercion(FnSymbol* fn, CallExpr* call,
                           ArgSymbol* formal, SymExpr* actual,
                           SymbolMap& copyMap,
                           SymbolMap& inTmpToActualMap) {

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

  {
    Symbol* origActual = actual->symbol();
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
      Symbol* actualSym = actual->symbol();
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
          actual->setSymbol(new_CStringSymbol(str));
        } else {
          addArgCoercion(fn, call, formal, actual, c2);
        }
      }
    } while (c2 && --checksLeft > 0);

    INT_ASSERT(c2 == false);

    // record mapping from innermost coercion tmp to original actual
    if (formal->originalIntent == INTENT_IN ||
        formal->originalIntent == INTENT_CONST_IN ||
        formal->originalIntent == INTENT_INOUT)
      inTmpToActualMap.put(actual->symbol(), origActual);
  }
}

// do we need to add some coercion from the actual to the formal?
static bool needToAddCoercion(Type*      actualType,
                              Symbol*    actualSym,
                              ArgSymbol* formal,
                              FnSymbol*  fn) {
  Type* formalType = formal->type;

  if (actualType == formalType)
    return false;

  // If we have an actual of ref(formalType) and
  // a REF or CONST REF argument intent, no coercion is necessary.
  // Likewise, a value actual passed to a ref formal
  // does not require coercion.
  if (actualType->getRefType() == formalType ||
      actualType == formalType->getRefType()) {
    IntentTag formalIntent = getIntent(formal);
    if ((formalIntent & INTENT_FLAG_REF) != 0 ||
        // or these that always turn into ref
        formalIntent == INTENT_OUT ||
        formalIntent == INTENT_INOUT) {
      return false;
    }
  }

  // New in-intents don't require coercion from ref to value
  // since it'll be handled by the initCopy call.
  if (actualType == formalType->getRefType() &&
      shouldAddInFormalTempAtCallSite(formal, fn))
    return false;

  // If actual and formal are type symbols, no coercion is necessary
  if (actualSym->hasFlag(FLAG_TYPE_VARIABLE) &&
      formal->hasFlag(FLAG_TYPE_VARIABLE))
    return false;

  // Avoid adding coercions from nil because these generate
  // errors for some C compilers.
  if (actualType == dtNil && isClassLikeOrPtr(formalType))
    return false;

  if (inOrOutFormalNeedingCopyType(formal)) {
    Type* toType = getCopyTypeDuringResolution(actualType);
    if (toType == formal->getValType())
      return false; // handled by other wrapper code, e.g. handleInIntent
  }

  // Handle inout argument given a different type (can convert on in,
  // = on the way back produces error if necessary).
  if (formal->originalIntent == INTENT_INOUT &&
      canCoerce(actualType->getValType(), actualSym,
                formalType->getValType(), formal, fn))
    return true;

  // One day, we shouldn't need coercion if canCoerceAsSubtype
  // returns true. That would cover the above case. However,
  // the emitted C code doesn't encode the class hierarchy in
  // the type system. (But we could do this for LLVM, say).

  if (canCoerce(actualType, actualSym, formalType, formal, fn))
    return true;

  return false;
}

static IntentTag getIntent(ArgSymbol* formal) {
  IntentTag retval = formal->intent;

  if (retval == INTENT_BLANK || retval == INTENT_CONST) {
    retval = concreteIntentForArg(formal);
  }

  return retval;
}

static bool argumentCanModifyActual(IntentTag intent) {
  switch (intent) {
    case INTENT_CONST:
    case INTENT_BLANK:
    case INTENT_PARAM:
    case INTENT_TYPE:
      INT_FATAL("Should never be reached");
      return false;
    case INTENT_IN:
    case INTENT_CONST_IN:
    case INTENT_CONST_REF:
      return false;
    case INTENT_OUT:
    case INTENT_INOUT:
    case INTENT_REF:
    case INTENT_REF_MAYBE_CONST: // this is conservative
      return true;
  }
  return false;
}

static void errorIfValueCoercionToRef(CallExpr* call, Symbol* actual,
                                      ArgSymbol* formal) {
  IntentTag intent = getIntent(formal);
  bool isRefFormal = formal->isRef() || (intent & INTENT_REF);
  FnSymbol* calledFn = formal->getFunction();
  Type* atype = actual->getValType();
  Type* ftype = formal->getValType();

  if (inGenerousResolutionForErrors()) {
    return;
  }

  // Ignore this class of error for tuples since the compiler is currently
  // producing this pattern for chpl__unref. This is a workaround and a
  // better solution would be preferred.
  if (formal->getValType()->symbol->hasFlag(FLAG_TUPLE)) {
    return;
  }

  // Not an error for inout our out intent
  // (the compiler should be managing the conversion on the way in
  //  to the function with implicit conversion and on the way out
  //  of the function with = )
  if (formal->originalIntent == INTENT_INOUT ||
      formal->originalIntent == INTENT_OUT)
    return;

  // Error for coerce->value passed to ref / out / etc
  if (argumentCanModifyActual(intent) || isRefFormal) {
    USR_FATAL_CONT(call, "in call to '%s', cannot pass result of coercion "
                         "by reference",
                         calledFn->name);

    USR_PRINT(call, "implicit coercion from '%s' to '%s'",
                    atype->symbol->name,
                    ftype->symbol->name);

    USR_PRINT(formal, "when passing to %s formal '%s'",
                      intentDescrString(intent),
                      formal->name);


  } else if (isRefFormal) {
    USR_FATAL_CONT(call, "in call to '%s', cannot pass result of coercion "
                         "by reference",
                         calledFn->name);

    USR_PRINT(call, "implicit coercion from '%s' to '%s'",
                    atype->symbol->name,
                    ftype->symbol->name);

    USR_PRINT(formal, "when passing to %s formal '%s'",
                      intentDescrString(intent),
                      formal->name);
  }
}

// Add a coercion; set actual's symbol to a new temp storing the result
// of a coercion.
static void addArgCoercion(FnSymbol*  fn,
                           CallExpr*  call,
                           ArgSymbol* formal,
                           SymExpr*   actual,
                           bool&      checkAgain) {
  SET_LINENO(actual);

  Symbol*     prevActual = actual->symbol();
  TypeSymbol* ats        = prevActual->type->symbol;
  TypeSymbol* fts        = formal->type->symbol;
  CallExpr*   castCall   = nullptr;
  bool        addedCast  = false;
  VarSymbol*  castTemp   = newTemp("coerce_tmp");

  castTemp->addFlag(FLAG_COERCE_TEMP);
  castTemp->addFlag(FLAG_INSERT_AUTO_DESTROY);

  if (prevActual->hasFlag(FLAG_ARG_THIS) &&
      isDispatchParent(prevActual->type, formal->type)) {
    castTemp->addFlag(FLAG_ARG_THIS);
  }

  actual->setSymbol(castTemp);

  // Add the Def for castTemp
  call->getStmtExpr()->insertBefore(new DefExpr(castTemp));

  // adjust fts for inout to use the value type
  if (formal->originalIntent == INTENT_INOUT)
    fts = fts->getValType()->symbol;

  // Here we will often strip the type of its sync-ness.
  // After that we may need another coercion(s), e.g.
  //   _syncvar(int) --readFE()-> int -> real
  //
  // We warn addArgCoercion's caller about that via checkAgain:
  if (isSyncType(ats->getValType()) == true) {
    checkAgain = true;
    castCall   = new CallExpr("readFE", gMethodToken, prevActual);
    USR_WARN(actual, "implicitly reading from a sync is deprecated; "
                     "apply a 'read\?\?()' method to the actual");

  } else if (isSingleType(ats->getValType()) == true) {
    checkAgain = true;

    castCall   = new CallExpr("readFF", gMethodToken, prevActual);
    USR_WARN(actual, "implicitly reading from a single is deprecated; "
                     "apply a 'read\?\?()' method to the actual");

  } else if (isManagedPtrType(ats->getValType()) == true &&
             !isManagedPtrType(formal->getValType())) {
    checkAgain = true;

    castCall   = new CallExpr("borrow", gMethodToken, prevActual);

  } else if (ats->getValType()->symbol->hasFlag(FLAG_TUPLE) &&
             formal->getValType()->symbol->hasFlag(FLAG_TUPLE) &&
             ats->getValType() != formal->getValType()) {

    // Handle tuple cast
    checkAgain = false;
    addedCast = true;

    castTemp->type = formal->getValType();

    castTemp->addFlag(FLAG_EXPR_TEMP); // for lvalue checking

    addTupleCoercion(toAggregateType(ats->getValType()),
                     toAggregateType(formal->getValType()),
                     prevActual,
                     castTemp,
                     call->getStmtExpr());

  } else if (ats->getValType()->symbol->hasFlag(FLAG_TUPLE) &&
             formal->getValType()->symbol->hasFlag(FLAG_TUPLE) &&
             fts->hasFlag(FLAG_REF) && !ats->hasFlag(FLAG_REF)) {

    // Add a PRIM_ADDR_OF to get the ref to the actual
    castCall = new CallExpr(PRIM_ADDR_OF, prevActual);

    if (prevActual->hasFlag(FLAG_EXPR_TEMP))
      castTemp->addFlag(FLAG_EXPR_TEMP); // for lvalue checking

    if (prevActual->hasFlag(FLAG_REF_TO_CONST) ||
        prevActual->isConstant() ||
        prevActual->isParameter()) {
      castTemp->addFlag(FLAG_REF_TO_CONST);
    }

  } else if (ats->hasFlag(FLAG_REF) &&
             !(ats->getValType()->symbol->hasFlag(FLAG_TUPLE) &&
               formal->getValType()->symbol->hasFlag(FLAG_TUPLE)) ) {

    AggregateType* at = toAggregateType(ats->getValType());

    //
    // dereference a reference actual
    //
    // after dereferencing we may need another coercion, e.g.
    //   _ref(int)  --coerce->  int  --coerce->  real
    // or
    //   _ref(_syncvar(int)) --> _syncvar(int) --> _ref(int) --> int --> real
    //
    checkAgain = true;

    if (typeNeedsCopyInitDeinit(at) && propagateNotPOD(at) &&
        !fn->hasFlag(FLAG_AUTO_COPY_FN) &&
        !fn->hasFlag(FLAG_INIT_COPY_FN)) {

      bool isConstCopy = formal->intent == INTENT_CONST_IN ||
                         formal->originalIntent == INTENT_CONST_IN ||
                         formal->hasFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT);
      Symbol *definedConst = isConstCopy ?  gTrue : gFalse;
      castCall = new CallExpr(astr_initCopy, prevActual, definedConst);
    } else {
      castCall   = new CallExpr(PRIM_DEREF, prevActual);
    }

    if (SymExpr* prevSE = toSymExpr(prevActual)) {
      if (prevSE->symbol()->hasFlag(FLAG_REF_TO_CONST)) {
        castTemp->addFlag(FLAG_CONST);

        if (prevSE->symbol()->hasFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS)) {
          castTemp->addFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS);
        }
      }
    }

  } else if (isUnmanagedClass(ats->typeInfo()) &&
             isBorrowedClass(formal->typeInfo())) {
    checkAgain = true;

    castCall   = new CallExpr(PRIM_CAST, formal->getValType()->symbol, prevActual);

  } else {
    // There was code to handle the case when the flag *is* present.
    // I deleted that code. The assert ensures it wouldn't apply anyway.
    INT_ASSERT(!actual->symbol()->hasFlag(FLAG_INSTANTIATED_PARAM));

    castCall = nullptr;
  }

  if (castCall == nullptr && !addedCast) {
    // the common case
    castCall = createCast(prevActual, fts);

    if (isString(fts)) {
      castTemp->addFlag(FLAG_INSERT_AUTO_DESTROY);
    }
  }

  if (castCall) {
    // move the result to the temp
    CallExpr* castMove = new CallExpr(PRIM_MOVE, castTemp, castCall);

    call->getStmtExpr()->insertBefore(castMove);

    resolveCallAndCallee(castCall, true);

    if (FnSymbol* castTarget = castCall->resolvedFunction()) {
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

  // Check for coercions resulting in a value that
  // they are not passed by ref or const ref.
  if (!castTemp->isRef()) {
    errorIfValueCoercionToRef(call, prevActual, formal);
  }
}

// A wrapper that mimics the state during default_arg creation, so that we can
// share code with that implementation.
static void copyFormalTypeExprWrapper(FnSymbol* fn,
                                      ArgSymbol* formal,
                                      VarSymbol* runtimeTypeTemp,
                                      CallExpr* call,
                                      Expr* insertBefore,
                                      SymbolMap& copyMap) {

  BlockStmt* body = new BlockStmt(BLOCK_SCOPELESS);
  insertBefore->insertBefore(body);

  copyFormalTypeExpr(formal, body, runtimeTypeTemp);

  update_symbols(body, &copyMap);
  normalize(body);
  resolveBlockStmt(body);
  reset_ast_loc(body, call);
  body->flattenAndRemove();
}

// BHARSH 2018-05-02: For a case like 'in D = {1..4}' normalization
// currently turns the AST into something like:
//   in D : {1..4} = {1..4}
// The typeExpr doesn't actually return a type, and is considered invalid in
// this particular context.
//
// Note: typeExpr is assumed to have been resolved during signature
// instantiation
static bool typeExprReturnsType(ArgSymbol* formal) {
  if (formal->typeExpr != nullptr) {
    Expr* last = formal->typeExpr->body.tail;
    if (CallExpr* call = toCallExpr(last)) {
      if (call->isPrimitive(PRIM_TYPEOF))
        return true;

      FnSymbol* fn = call->isResolved() ? call->resolvedFunction() : nullptr;
      if (fn && fn->retTag == RET_TYPE) {
        return true;
      }
    } else if (SymExpr* se = toSymExpr(last)) {
      if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
        return true;
      }
    }
  }

  return false;
}

/************************************* | **************************************
*                                                                             *
* handle intents at call site                                                 *
*                                                                             *
************************************** | *************************************/

// Do not create copies for the bogus actuals added for PRIM_TO_FOLLOWER.
static bool checkAnotherFunctionsFormal(FnSymbol* calleeFn, CallExpr* call,
                                        Symbol* actualSym) {
  bool result = isArgSymbol(actualSym) &&
                (call->parentSymbol != actualSym->defPoint->parentSymbol);

  if (result                                   &&
      propagateNotPOD(actualSym->getValType()) &&
      isFollowerIterator(calleeFn)             )
    USR_FATAL_CONT(calleeFn, "follower iterators accepting a non-POD argument by in-intent are not implemented");

  return result;
}

static bool isFormalTempConst(FnSymbol *fn, ArgSymbol *formal) {

  // Today, if we generate a default initializer for a type with const fields,
  // the formals that correspond to those fields have `in` intents. However, we
  // still need to set those temporaries that will be passed to those formals to
  // be constant before calling the initializer, in case the initializer has
  // another argument that will use that temporary.
  //
  // This comes up in:
  //
  // record R {
  //   const d;
  //   var a: [d] int;
  // }
  //
  // we ideally want to be able to make such formals have `const in` intent
  // instead of `in`. But there may be complications with that and we are close
  // to the release, so, we track the field symbol from the call in case it is a
  // default initializer

  if (fn->hasFlag(FLAG_COMPILER_GENERATED) && fn->name == astrInit) {
    Symbol *fieldSym = fn->getReceiverType()->getField(formal->name);
    return fieldSym->hasFlag(FLAG_CONST);
  }

  return formal->intent == INTENT_CONST_IN ||
         formal->originalIntent == INTENT_CONST_IN ||
         formal->hasFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT);
}

static void handleInIntent(FnSymbol* fn, CallExpr* call,
                           ArgSymbol* formal, SymExpr* actual,
                           SymbolMap& copyMap,
                           SymbolMap& inTmpToActualMap) {

  bool inout = (formal->intent == INTENT_INOUT ||
                formal->originalIntent == INTENT_INOUT) &&
               fn->hasFlag(FLAG_PROMOTION_WRAPPER) == false;

  bool in = (formal->intent == INTENT_IN ||
             formal->originalIntent == INTENT_IN ||
             formal->intent == INTENT_CONST_IN ||
             formal->originalIntent == INTENT_CONST_IN);

  if (inout == false && in == false)
    return;

  // don't consider inout in promotion wrapper for this purpose,
  // because it's handled within the body of the function.

  // In intents for initializers called within _new or default init functions
  // are handled by the _new or default init functions.
  if (isNestedNewOrDefault(fn, call) && !inout) {
    return;
  }
  // In intents for chpl__coerceCopy / chpl__coerceMove are fundamental
  // and shouldn't be adjusted here.
  if (fn->hasFlag(FLAG_COERCE_FN) && !inout)
    return;

  Expr* anchor = call->getStmtExpr();

  Symbol* origActualSym = actual->symbol();
  Symbol* actualSym = origActualSym;
  {
    // update origActualSym to take into account any conversions added
    Symbol* mapSym = inTmpToActualMap.get(origActualSym);
    if (mapSym != nullptr)
      origActualSym = mapSym;
  }


  // The result of a default argument for 'in' intent is already owned and
  // does not need to be copied.
  if ((formalRequiresTemp(formal, fn) || inout) &&
      (shouldAddInFormalTempAtCallSite(formal, fn) || inout) &&
      !checkAnotherFunctionsFormal(fn, call, actualSym) &&
      actualSym->hasFlag(FLAG_DEFAULT_ACTUAL) == false) {

    // Arrays and domains need special handling in order to preserve their
    // runtime types.
    bool rtt = actualSym->getValType()->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE);
    bool coerceRuntimeTypes = rtt && typeExprReturnsType(formal);

    VarSymbol* runtimeTypeTemp = nullptr;
    if (coerceRuntimeTypes) {
      runtimeTypeTemp = newTemp("_formal_type_tmp");
      runtimeTypeTemp->addFlag(FLAG_TYPE_VARIABLE);
      anchor->insertBefore(new DefExpr(runtimeTypeTemp));

      copyFormalTypeExprWrapper(fn, formal, runtimeTypeTemp,
                                call, anchor, copyMap);
    }

    // A copy might be necessary here but might not.
    if (doesCopyInitializationRequireCopy(actual) || inout) {
      // Add a new formal temp at the call site that mimics variable
      // initialization from the actual.
      VarSymbol* tmp = newTemp(astr("_formal_tmp_in_", formal->name));
      tmp->addFlag(FLAG_EXPR_TEMP);

      // for in intent,
      // "move" from call site to called function, so don't destroy
      // here. The called function will destroy.
      if (!inout)
        tmp->addFlag(FLAG_NO_AUTO_DESTROY);

      // for inout intent, need to destroy the temp at the call site
      // to allow passing in a single argument (otherwise, when we try
      // to do the write-back after the call, the value would be deinited
      // already).
      if (inout) {
        tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
        tmp->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);
        tmp->addFlag(FLAG_FORMAL_TEMP_OUT_CALLSITE);
      }

      // Does this need to be here?
      if (formal->hasFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT)) {
        tmp->addFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT);
      }

      CallExpr* copy = nullptr;

      Symbol *definedConst = isFormalTempConst(fn, formal) ?  gTrue : gFalse;
      if (coerceRuntimeTypes)
        copy = new CallExpr(astr_coerceCopy, runtimeTypeTemp, actualSym,
                            definedConst);
      else
        copy = new CallExpr(astr_initCopy, actualSym, definedConst);

      CallExpr* move = new CallExpr(PRIM_MOVE, tmp, copy);
      anchor->insertBefore(new DefExpr(tmp));
      anchor->insertBefore(move);

      resolveCallAndCallee(copy, false); // false - allow unresolved
      resolveCall(move);

      actual->setSymbol(tmp);
    } else {
      // Is actualSym something that owns its value?
      // Is it a call-temp storing the result of a call?
      // Then "move" ownership to the called function
      // (don't destroy it here, it will be destroyed there).
      actualSym->addFlag(FLAG_NO_AUTO_DESTROY);

      if (coerceRuntimeTypes) {
        VarSymbol* tmp = newTemp(astr("_formal_tmp_in_", formal->name));
        tmp->addFlag(FLAG_NO_AUTO_DESTROY);
        tmp->addFlag(FLAG_EXPR_TEMP);

        // Does this need to be here?
        if (formal->hasFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT)) {
          tmp->addFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT);
        }

        Symbol *definedConst = isFormalTempConst(fn, formal) ?  gTrue : gFalse;
        CallExpr* copy = new CallExpr(astr_coerceMove,
                                      runtimeTypeTemp, actualSym, definedConst);

        CallExpr* move = new CallExpr(PRIM_MOVE, tmp, copy);
        anchor->insertBefore(new DefExpr(tmp));
        anchor->insertBefore(move);

        resolveCallAndCallee(copy, false); // false - allow unresolved
        resolveCall(move);

        actual->setSymbol(tmp);
      }
      else {
        if (actualSym->getValType()->symbol->hasFlag(FLAG_DOMAIN)) {
          if (!actualSym->hasFlag(FLAG_TYPE_VARIABLE)) {
            // we are moving a domain to an `in` formal without coercion. We
            // should adjust its constness
            Symbol *definedConst = isFormalTempConst(fn, formal) ? gTrue : gFalse;


            Expr *nextExpr = new CallExpr(PRIM_NOOP);
            anchor->insertBefore(nextExpr);
            setDefinedConstForDomainSymbol(actualSym, nextExpr, definedConst);
            nextExpr->remove();
          }
        }
      }
    }
  }

  if (inout) {
    // temporary is required for inout
    // the default actual is enough, though.
    if (actual->symbol()->hasFlag(FLAG_DEFAULT_ACTUAL) == false &&
        fn->hasFlag(FLAG_PROMOTION_WRAPPER) == false) {
      INT_ASSERT(actual->symbol() != origActualSym);
      inTmpToActualMap.put(actual->symbol(), origActualSym);
    }
  }
}

static void handleOutIntents(FnSymbol* fn, CallExpr* call,
                             SymbolMap& inTmpToActualMap) {

  int j = 0;

  // Function with no actuals can't use out intent
  // Returning early in that event simplifies the following code.
  if (call->numActuals() == 0)
    return;

  Expr* anchor = call->getStmtExpr();
  Expr* anchorAfter = call->getStmtExpr();

  Expr* currActual = call->get(1);
  Expr* nextActual = nullptr;

  bool anyGenericOut = false;
  for_formals(formal, fn) {
    if (formal->intent == INTENT_OUT ||
        formal->originalIntent == INTENT_OUT)
      if (formal->getValType()->symbol->hasFlag(FLAG_GENERIC))
        anyGenericOut = true;
  }
  if (anyGenericOut) {
    // resolve out functions now, to infer any types of out arguments,
    // so that the call-site temporaries have proper types.
    resolveFunction(fn, call);
  }


  for_formals(formal, fn) {
    SET_LINENO(currActual);
    nextActual = currActual->next;

    bool out = formal->intent == INTENT_OUT ||
               formal->originalIntent == INTENT_OUT;
    bool inout = formal->intent == INTENT_INOUT ||
                 formal->originalIntent == INTENT_INOUT;

    if (out || inout) {
      Expr* useExpr = currActual;
      if (NamedExpr* named = toNamedExpr(currActual))
        useExpr = named->actual;

      SymExpr* actualSe = toSymExpr(useExpr);
      Symbol* assignTo = nullptr;
      Symbol* assignFrom = nullptr;

      if (out) {
        Type* formalType = formal->type->getValType();

        // If the actual argument has generic or dtSplitInitType type,
        // update it to infer the type from the called function.
        if (actualSe->symbol()->type == dtSplitInitType ||
            actualSe->symbol()->type->symbol->hasFlag(FLAG_GENERIC))
          actualSe->symbol()->type = formalType;

        VarSymbol* tmp = newTemp(astr("_formal_tmp_out_", formal->name),
                                 formal->getValType());
        tmp->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);
        tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
        tmp->addFlag(FLAG_EXPR_TEMP);
        tmp->addFlag(FLAG_FORMAL_TEMP_OUT_CALLSITE);

        // Transform  f(x) where x is passed with out intent into
        //   DefExpr tmp
        //   f(tmp)
        //   x = tmp     -> this might turn into split-init of x
        anchor->insertBefore(new DefExpr(tmp));
        currActual->replace(new SymExpr(tmp));
        assignTo = actualSe->symbol();
        assignFrom = tmp;

      } else {
        Symbol* mapSym = inTmpToActualMap.get(actualSe->symbol());
        if (mapSym == nullptr) {
          // e.g. inout with a defaulted actual
          assignTo = actualSe->symbol();
          assignFrom = actualSe->symbol();
        } else {
          // we have
          //  in_tmp = copy(x)
          //  f(in_tmp)
          //
          // add assign like
          //  x = in_tmp
          assignTo = mapSym;
          assignFrom = actualSe->symbol();
        }
      }

      if (assignTo != assignFrom) {
        CallExpr* assign = new CallExpr("=", assignTo, assignFrom);
        anchorAfter->insertAfter(assign);
        anchorAfter = assign;
      }
    }

    currActual = nextActual;
    j++;
  }
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
    bool      zippered;
    bool      hasLeaderFollowers;
    bool      resultIsUsed;

    // The following vectors are indexed by the i'th formal to fn (0-based).

    // The TypeSymbol representing the type that is promoted (e.g. array)
    // or nullptr if that argument isn't promoted.
    std::vector<TypeSymbol*> promotedType;

    int formalToActualOpMod;

    std::vector<uint8_t> defaulted;

    // for the i'th formal to fn, fnFormals is nullptr
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
                                        BlockStmt* instantiationPt,
                                        CallInfo&  info,
                                        bool       fastFollowerChecks);

static BlockStmt* buildPromotionLoop(PromotionInfo& promotion,
                                     BlockStmt* instantiationPt,
                                     CallInfo&  info,
                                     bool       fastFollowerChecks);

static void       buildLeaderIterator(PromotionInfo& promotion,
                                      BlockStmt* instantiationPt,
                                      Expr*     iterator,
                                      bool      zippered);

static void       buildFollowerIterator(PromotionInfo& promotion,
                                        BlockStmt* instantiationPt,
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
                                 BlockStmt* instantiationPt);

static Expr*      getIndices(PromotionInfo& promotion);

static Expr*      getIterator(PromotionInfo& promotion);

static bool       haveLeaderAndFollowers(PromotionInfo& promotion,
                                         CallExpr* call);

static CallExpr* createPromotedCallForWrapper(PromotionInfo& promotion);

static void       collectPromotionFormals(PromotionInfo& promotion,
                                          std::set<ArgSymbol*>& formals);

static void       fixUnresolvedSymExprsForPromotionWrapper(FnSymbol* wrapper,
                                                           FnSymbol* fn);

static Symbol* leadingArg(PromotionInfo& promotion, CallExpr* call) {
  int i = promotion.formalToActualOpMod;
  // Operators might have a different number of formals than provided actuals.
  // In this case, the promotedType array access needs to be adjusted to account
  // for the offset.
  for_actuals(actual, call) {
    if (i < 0) {
      i++;
      continue;
    }
    if (promotion.promotedType[i++] != nullptr)
      return symbolForActual(actual);
  }

  INT_ASSERT(false); // did not find any promoted things
  return nullptr;
}

// getMoveToIRtemp(): returns the PRIM_MOVE of 'call' into a temp,
// which we expect to hold the iterator record (IR).
// Returns nullptr if we do not know how to handle this pattern.
//
// Handles a corner case of a "nested call", for example "A.foo()"
// where A.foo is a promoted call to a paren-less function foo. Ex.
//   test/functions/promotion/issue-12736-ok.chpl
//
// If so, 'irTemp' in addSetIteratorShape() is the IR (iterator record)
// for the outer call and we do not yet have the IR for the inner call.
// So we cannot "set shape" on this inner IR. This is most likely OK
// because the inner IR feeds directly into the outer call. The outer call
// should also be promoted and inherit the shape from the inner call.
//
// We will go ahead and set the shape for 'irTemp' right away from the shape
// of the inner call. We will also remember 'irTemp' to avoid setting its
// shape the second time, which would not work, in isDuplicateSetIteratorShape.
//
static CallExpr* getMoveToIRtemp(CallExpr* call, bool &isNestedCall) {
  CallExpr* move = toCallExpr(call->parentExpr);

  if (move == nullptr)
    // If call's result is not used, do not set the shape.
    // This happens, for example, during resolveSerializeDeserialize().
    return nullptr;

  if (move->isPrimitive(PRIM_MOVE))
    return move; // this is the normal case

  CallExpr* parent2 = toCallExpr(move->parentExpr);
  if (call != move->baseExpr ||
      parent2 == nullptr     ||
      ! parent2->isPrimitive(PRIM_MOVE))
    return nullptr; // dunno how to handle this case

  // Otherwise, this is a nested call, see the comments above.
  isNestedCall = true;
  return parent2;
}

// Returns true if we should not be setting this IR's shape.
// See the comment on getMoveToIRtemp().
static bool isDuplicateSetIteratorShape(Symbol* irTemp, bool isNestedCall) {
  static std::set<Symbol*> outIRtemps;
  auto it = outIRtemps.find(irTemp);

  if (it != outIRtemps.end()) {
    outIRtemps.erase(it);
    return true; // otherwise we would be setting its shape twice, see above
  }

  if (isNestedCall)
    outIRtemps.insert(irTemp);  // remember it for the future

  return false; // this is the normal case
}

// insert PRIM_ITERATOR_RECORD_SET_SHAPE(iterRecord,shapeSource)
static void addSetIteratorShape(PromotionInfo& promotion, CallExpr* call) {
  bool isNestedCall = false;
  CallExpr* move = getMoveToIRtemp(call, isNestedCall); // sets isNestedCall
  if (move == nullptr)
    return;

  Symbol* irTemp = toSymExpr(move->get(1))->symbol();
  if (isDuplicateSetIteratorShape(irTemp, isNestedCall))
    return;

  // The first promoted argument argument determines the shape.
  Symbol* shapeSource = leadingArg(promotion, call);

  Symbol* fromForExpr = (! promotion.hasLeaderFollowers             ||
                         checkIteratorFromForExpr(move, shapeSource) )
                        ? gTrue : gFalse;

  move->insertAfter(new CallExpr(PRIM_ITERATOR_RECORD_SET_SHAPE,
                                 irTemp, shapeSource, fromForExpr));
}


bool isPromotionRequired(FnSymbol* fn, CallInfo& info,
                         std::vector<ArgSymbol*>& actualFormals) {
  bool retval = false;

  if (fn->name != astrSassign) {
    int numActuals = actualFormals.size();
    for (int j = 0; j < numActuals; j++) {
      Symbol* actual     = info.actuals.v[j];
      ArgSymbol* formal  = actualFormals[j];
      Type*   actualType = actual->type;
      bool    promotes   = false;

      if (isRecordWrappedType(actualType) == true) {
        makeRefType(actualType);

        actualType = actualType->refType;

        INT_ASSERT(actualType);
      }

      // Operator calls are allowed to have actuals for a method token and
      // "this" argument, even if the best operator match is not a method.
      // We'll clean it up later and we shouldn't promote based on it.
      if (formal == nullptr && fn->hasFlag(FLAG_OPERATOR))
        continue;

      if (canDispatch(actualType, actual,
                      formal->type, formal, fn, &promotes)) {
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
  FnSymbol* retval = nullptr;

  if (fReportPromotion) {
    USR_WARN(info.call, "promotion on %s", info.toString());
  }

  PromotionInfo promotion(fn, info, actualIdxToFormal);

  retval = checkCache(promotionsCache, promotion.fn, &promotion.subs);

  if (retval == nullptr) {
    SET_LINENO(info.call);
    BlockStmt* instantiationPt = getInstantiationPoint(info.call);
    retval = buildPromotionWrapper(promotion,
                                   instantiationPt,
                                   info,
                                   fastFollowerChecks);

    resolveSignature(retval);

    addCache(promotionsCache, promotion.fn, promotion.wrapperFn, &promotion.subs);
  }

  addSetIteratorShape(promotion, info.call);

  return retval;
}

/* Collect the substitutions that the function implementing the promotion
   will use. The substitutions map from the formal to
   the relevant actual type - or, in the event of a defaulted argument,
   to itself.
 */
PromotionInfo::PromotionInfo(FnSymbol* fn,
                             CallInfo& info,
                             std::vector<ArgSymbol*>& actualFormals) :
  fn(fn),
  // these are established later along with wrapperFormals
  wrapperFn(nullptr),
  zippered(false),
  hasLeaderFollowers(false),
  resultIsUsed(info.call != info.call->getStmtExpr())
{
  int numActuals = actualFormals.size();

  // Operators may mismatch the number of formals and the number of actuals.
  // Store any difference for later.
  if (numActuals != fn->numFormals() && fn->hasFlag(FLAG_OPERATOR)) {
    formalToActualOpMod = fn->numFormals() - numActuals;
  } else {
    formalToActualOpMod = 0;
  }

  for (int j = 0; j < numActuals; j++) {
    Symbol* actual     = info.actuals.v[j];
    ArgSymbol* formal  = actualFormals[j];
    Type*   actualType = actual->type;
    bool    promotes   = false;

    if (formal == nullptr) {
      // Operators may mismatch the number of formals and the number of actuals
      // If there is an actual that is not represented by a formal, skip it, it
      // will be handled later.
      INT_ASSERT(fn->hasFlag(FLAG_OPERATOR));
      continue;
    }

    if (isRecordWrappedType(actualType) == true) {
      makeRefType(actualType);

      actualType = actualType->refType;
    }

    if (canDispatch(actualType, actual, formal->type, formal, fn, &promotes)) {
      if (promotes == true) {
        this->subs.put(formal, actualType->symbol);
      }
    }
  }


  for_formals(formal, fn) {
    TypeSymbol* promotedType = nullptr;
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
                                       BlockStmt* instantiationPt,
                                       CallInfo&  info,
                                       bool       fastFollowerChecks) {

  initPromotionWrapper(promotion, instantiationPt);
  FnSymbol*  retval     = promotion.wrapperFn;
  FnSymbol*  fn         = promotion.fn;

  BlockStmt* loop = buildPromotionLoop(promotion, instantiationPt, info,
                                       fastFollowerChecks);
  retval->insertAtTail(loop);
  loop->flattenAndRemove();

  fn->defPoint->insertBefore(new DefExpr(retval));

  normalize(retval);

  fixUnresolvedSymExprsForPromotionWrapper(retval, fn);

  return retval;
}

static void insertAndSaveWrapCall(PromotionInfo& promotion, BlockStmt* block,
                                  VarSymbol* temp, CallExpr* wrapCall)
{
  if (promotion.resultIsUsed) {
    block->insertAtTail(new DefExpr(temp));
    block->insertAtTail(new CallExpr(PRIM_MOVE, temp, wrapCall));
    block->insertAtTail(new CallExpr(PRIM_YIELD, temp));

  } else {
    // No need to yield. NB wrapCall's type may be 'void'.
    block->insertAtTail(wrapCall);
  }

  // Save the wrapCall to adjust it later
  promotion.wrapCalls.push_back(wrapCall);
}

// The info needed to call buildFastFollowerChecksIfNeeded() later.
static std::map<FnSymbol*, std::set<ArgSymbol*> > promotionFormalsMap;

static BlockStmt* buildPromotionLoop(PromotionInfo& promotion,
                                     BlockStmt* instantiationPt,
                                     CallInfo&  info,
                                     bool       fastFollowerChecks) {
  FnSymbol*  wrapFn     = promotion.wrapperFn;

  Expr*      indices    = getIndices(promotion);
  Expr*      iterator   = getIterator(promotion);
  CallExpr*  wrapCall   = createPromotedCallForWrapper(promotion);
  bool       zippered   = promotion.zippered;
  BlockStmt* yieldBlock = new BlockStmt();
  VarSymbol* yieldTmp   = newTemp("p_yield");

  wrapFn->addFlag(FLAG_ITERATOR_FN);
  wrapFn->removeFlag(FLAG_INLINE);

  yieldTmp->addFlag(FLAG_EXPR_TEMP);

 if (haveLeaderAndFollowers(promotion, info.call))
 {
  promotion.hasLeaderFollowers = true;

  buildLeaderIterator(promotion, instantiationPt, iterator, zippered);

  buildFollowerIterator(promotion, instantiationPt, indices, iterator, wrapCall);

  if (fNoFastFollowers == false && fastFollowerChecks == true) {
    std::set<ArgSymbol*> requiresPromotion;

    collectPromotionFormals(promotion, requiresPromotion);

    INT_ASSERT(!promotionFormalsMap.count(wrapFn));

    // We will buildFastFollowerCheck() later, when (a) they are called for,
    // and (b) when we have the _iteratorRecord type for the promoted expr.
    // Test: studies/kmeans/kmeans-blc.chpl
    promotionFormalsMap[wrapFn] = requiresPromotion;
  }
 }

  insertAndSaveWrapCall(promotion, yieldBlock, yieldTmp, wrapCall);

  return ForLoop::buildForLoop(indices, iterator, yieldBlock,
                               zippered,
                               /* isForExpr */ true);
}

static void buildLeaderIterator(PromotionInfo& promotion,
                                BlockStmt* instantiationPt,
                                Expr*     iterator,
                                bool      zippered) {
  SymbolMap   leaderMap;
  FnSymbol*   liFn       = promotion.wrapperFn->copy(&leaderMap);

  Type*       tagType    = gLeaderTag->type;
  ArgSymbol*  liFnTag    = new ArgSymbol(INTENT_PARAM, "tag", tagType);

  VarSymbol*  liIndex    = newTemp("p_leaderIndex");
  VarSymbol*  liIterator = newTemp("p_leaderIterator");

  // Leader iterators always return by value
  liFn->retTag = RET_VALUE;

  INT_ASSERT(liFn->hasFlag(FLAG_RESOLVED) == false);

  form_Map(SymbolMapElem, e, leaderMap) {
    if (Symbol* s = paramMap.get(e->key)) {
      paramMap.put(e->value, s);
    }
  }

  liIterator->addFlag(FLAG_EXPR_TEMP);

  CallExpr *toLeader = nullptr;
  if (zippered) {
    CallExpr *iterCall = toCallExpr(iterator);
    INT_ASSERT(iterCall);
    INT_ASSERT(iterCall->isPrimitive(PRIM_ZIP));

    toLeader = new CallExpr("_toLeader", iterCall->get(1)->copy(&leaderMap));
  }
  else {
    toLeader = new CallExpr("_toLeader", iterator->copy(&leaderMap));
  }

  BlockStmt* loopBody = new BlockStmt(new CallExpr(PRIM_YIELD, liIndex));

  ForallStmt* fs = ForallStmt::buildHelper(new SymExpr(liIndex),
                                           new SymExpr(liIterator),
                                           nullptr, // intents
                                           loopBody,
                                           false, //only leader - not zippered
                                           true); // do not mess with iterator
                                                  // and no shadow vars please

  BlockStmt* loop = buildChapelStmt(fs);

  liFn->addFlag(FLAG_INLINE_ITERATOR);
  liFn->setGeneric(true);
  liFn->removeFlag(FLAG_INVISIBLE_FN);

  liFn->insertFormalAtTail(liFnTag);

  liFn->where = new BlockStmt(new CallExpr("==", liFnTag, gLeaderTag));

  liFn->body  = new BlockStmt();

  liFn->insertAtTail(new DefExpr(liIterator));

  liFn->insertAtTail(new CallExpr(PRIM_MOVE, liIterator, toLeader));

  liFn->insertAtTail(loop);

  promotion.fn->defPoint->getModule()->block->insertAtHead(new DefExpr(liFn));

  normalize(liFn);

  liFn->setInstantiationPoint(instantiationPt);
}

static void buildFollowerIterator(PromotionInfo& promotion,
                                  BlockStmt* instantiationPt,
                                  Expr*     indices,
                                  Expr*     iterator,
                                  CallExpr* wrapCall) {
  SymbolMap  followerMap;
  FnSymbol*  fn         = promotion.fn;
  FnSymbol*  wrapFn     = promotion.wrapperFn;

  SymExpr*   symFalse         = new SymExpr(gFalse);

  ArgSymbol* fiFnTag          = nullptr;
  ArgSymbol* fiFnFollower     = nullptr;
  ArgSymbol* fastFollower     = nullptr;

  VarSymbol* followerIterator = newTemp("p_followerIterator");

  FnSymbol*  fiFn             = wrapFn->copy(&followerMap);

  form_Map(SymbolMapElem, e, followerMap) {
    if (Symbol* s = paramMap.get(e->key)) {
      paramMap.put(e->value, s);
    }
  }

  INT_ASSERT(fiFn->hasFlag(FLAG_RESOLVED) == false);

  followerIterator->addFlag(FLAG_EXPR_TEMP);

  fiFnTag      = new ArgSymbol(INTENT_PARAM, "tag", gFollowerTag->type);
  fiFnFollower = new ArgSymbol(INTENT_BLANK, iterFollowthisArgname, dtAny);
  fastFollower = new ArgSymbol(INTENT_PARAM, "fast", dtBool, nullptr, symFalse);

  fiFn->setGeneric(true);
  fiFn->removeFlag(FLAG_INVISIBLE_FN);

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

  fn->defPoint->getModule()->block->insertAtHead(new DefExpr(fiFn));

  normalize(fiFn);

  fiFn->setInstantiationPoint(instantiationPt);

  fixUnresolvedSymExprsForPromotionWrapper(fiFn, fn);
}

static CondStmt* selectFollower(ArgSymbol* fastFollower,
                                Expr*      iterator,
                                VarSymbol* followerIterator,
                                SymbolMap& followerMap,
                                ArgSymbol* fiFnFollower) {
  CallExpr*   callFast = nullptr;
  CallExpr*   moveFast = nullptr;

  CallExpr*   callSlow = nullptr;
  CallExpr*   moveSlow = nullptr;

  if (CallExpr *iterCall = toCallExpr(iterator)) {
    INT_ASSERT(iterCall->isPrimitive(PRIM_ZIP));

    callFast = generateFastFollowersForZip(iterCall, fiFnFollower, &followerMap,
                                           /*getIterator=*/false);
    callSlow = generateRegularFollowersForZip(iterCall, fiFnFollower, &followerMap,
                                           /*getIterator=*/false);
  } else {
    callFast = new CallExpr("_toFastFollower", iterator->copy(&followerMap), fiFnFollower);
    callSlow = new CallExpr("_toFollower", iterator->copy(&followerMap), fiFnFollower);
  }


  moveFast = new CallExpr(PRIM_MOVE, followerIterator, callFast);
  moveSlow = new CallExpr(PRIM_MOVE, followerIterator, callSlow);

  return new CondStmt(new SymExpr(fastFollower), moveFast, moveSlow);
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

  CallExpr* wrapCallCopy = wrapCall->copy(&followerMap);

  insertAndSaveWrapCall(promotion, block, yieldTmp, wrapCallCopy);

  return ForLoop::buildForLoop(indices->copy(&followerMap),
                               new SymExpr(followerIterator),
                               block,
                               promotion.zippered,
                               /* isForExpr */ true);
}

// The returned string is canonical ie from astr().
const char* unwrapFnName(FnSymbol* fn) {
  INT_ASSERT(! strncmp(fn->name, "chpl_promo", 10));
  const char* uscore = strchr(fn->name+11, '_');
  return astr(uscore+1);
}

static void initPromotionWrapper(PromotionInfo& promotion,
                                 BlockStmt* instantiationPoint) {

  FnSymbol* fn = promotion.fn;
  FnSymbol* retval = buildEmptyWrapper(fn);
  retval->setInstantiationPoint(instantiationPoint);

  static int wrapId = 0;
  retval->name = astr("chpl_promo", istr(++wrapId), "_", fn->name);
  retval->cname = retval->name;

  retval->addFlag(FLAG_PROMOTION_WRAPPER);
  retval->addFlag(FLAG_FN_RETURNS_ITERATOR);

  if (fn->hasFlag(FLAG_OPERATOR)) {
    retval->addFlag(FLAG_OPERATOR);
  }

  int i = 0;
  for_formals(formal, fn) {

    ArgSymbol* newFormal = nullptr;

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
  Expr*     retval      = nullptr;

  int i = 0;
  for_formals(formal, fn) {
    SET_LINENO(formal);

    if (promotion.promotedType[i] != nullptr) {
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
  CallExpr* iteratorCall = new CallExpr(PRIM_ZIP);
  Expr*     retval       = nullptr;

  int i = 0;
  for_formals(formal, fn) {

    SET_LINENO(formal);

    if (promotion.promotedType[i] != nullptr) {
      ArgSymbol* newFormal = promotion.wrapperFormals[i];
      INT_ASSERT(newFormal);
      iteratorCall->insertAtTail(newFormal);
    }
    i++;
  }

  // If there was only one promoted argument, don't call _build_tuple after all
  if (iteratorCall->numActuals() == 1) {
    retval = iteratorCall->get(1)->remove();
    promotion.zippered = false;
  } else {
    retval = iteratorCall;
    promotion.zippered = true;
  }

  return retval;
}


static std::set<Symbol*> haveLeaderSymbolStack;

static FnSymbol* leaderForSymbol(Expr* anchor, Symbol* leadingSym) {
  Type* leadingType = leadingSym->getValType();

  if (leadingType->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
    // This is an iterator or a forwarder. Check its IteratorGroup.
    FnSymbol* serialIter = getTheIteratorFn(leadingType);
    return serialIter->iteratorGroup->leader;

  } else {
    if (haveLeaderSymbolStack.count(leadingSym) > 0)
      return nullptr; // We are recursing. Report no leader, then.

    haveLeaderSymbolStack.insert(leadingSym);
    // Not an iterator. To iterate over it, need to invoke these().
    CallExpr* callLeader = new CallExpr("these", gMethodToken,
      leadingSym, new NamedExpr(astrTag, new SymExpr(gLeaderTag)));
    BlockStmt* container = new BlockStmt(callLeader);
    anchor->insertAfter(container);
    FnSymbol* leaderFn = tryResolveCall(callLeader);
    container->remove();
    haveLeaderSymbolStack.erase(leadingSym);
    return leaderFn;
  }
}

//
// We should create the leader and follower flavors of the serial iterator
// for a promotion - only when those flavors are available from the arguments
// of the promoted call, as follows:
//
// * The leading argument, i.e. the first of the promoted arguments,
//   must provide a leader iterator. If it provides a leader iterator
//   and does NOT provide a follower iterator, it is a user error.
//
// * All promoted arguments must provide follower iterators.
//
// If these conditions are not satisfied, we cannot execute the promoted
// expression in parallel. If we generate the L/F iterators in this case,
// the compiler will find and invoke them. However, their bodies will fail
// to resolve, causing compilation error inappropriately. If we do not
// generate L/F, the compiler will be forced to use the serial iterator,
// which is the correct semantics.
//
// One example is:  test/studies/sudoku/deitz/sudoku3.chpl
// whose simple version is the promotion of '&' in: (A!=k) & linearize(A!=k)
// Here, the first arg of & has parallel iterators, the second doesn't.
//
// Taking the standalone iterator into account - when there is only a single
// promoted argument - is future work - GitHub Issue #12323.
//
static bool haveLeaderAndFollowers(PromotionInfo& promotion, CallExpr* call) {
  // This is analogous to optionalFollowersAreMissing() in foralls.cpp,
  // with an additional check for the first leader and the first follower.
  Expr* anchor = call->getStmtExpr();
  FnSymbol* leader = nullptr;  // non-null for promoted args after the first
  VarSymbol* followme = nullptr;
  int i = 0;
  if (promotion.formalToActualOpMod < 0) {
    // Operators might have more actuals than there are formals.  In this case,
    // we need to skip the extra actuals (the "this" actual and method token)
    i = promotion.formalToActualOpMod;
  }

  for_actuals(actualExpr, call) {
    if (i < 0) {
      i++;
      continue;
    }
    if (promotion.promotedType[i++] != nullptr) {
      Symbol* actual = symbolForActual(actualExpr);

      if (leader == nullptr) {
        // The first argument. Needs both L+F.
        leader = leaderForSymbol(anchor, actual);
        if (leader == nullptr) return false; // no leader

        // We are going to resolve follower calls to determine whether the
        // followers are available. So we need to know the type of followThis,
        // which is the leader's yield type. For that, resolve the leader.
        resolveFunction(leader);

        QualifiedType yType = fsIterYieldType(call, leader);
        followme = new VarSymbol("followme", yType);

        if (! fsGotFollower(anchor, followme, actual)) {
          USR_FATAL_CONT(call, "a follower iterator is required for %d-th argument of the promoted expression", i);
          USR_PRINT(call, "because it is the first promoted argument and has a leader iterator");
          USR_PRINT(leader, "the leader iterator is here");
        }
      } else {
        // A subsequent argument. Needs a follower.
        if (! fsGotFollower(anchor, followme, actual))
          return false;
      }
    }
  }
  return true; // all needed iterators are present
}

// Returns a CallExpr which contains the call to the original function
// as the last statement. Assumes that addDefaultTokensAndReorder etc will
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

    Expr* actual = nullptr;

    if (promotion.promotedType[i] != nullptr) {
      // a promoted argument
      const char* name = astr("p_i_", istr(i+1));
      actual = new UnresolvedSymExpr(name);
    } else if (promotion.defaulted[i] == false) {
      ArgSymbol* newFormal = promotion.wrapperFormals[i];
      actual = new SymExpr(newFormal);
    }

    if (actual != nullptr) {
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
    if (promotion.promotedType[i] != nullptr) {
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
          BlockStmt*            callBlock = nullptr;
          BlockStmt*            loop      = nullptr;
          bool                  found = false;

          callBlock = toBlockStmt(call->getStmtExpr()->parentExpr);
          loop      = toBlockStmt(callBlock->parentExpr);

          INT_ASSERT(loop && loop->isLoopStmt());

          collectDefExprs(loop, defs);

          for_vector(DefExpr, def, defs) {
            if (def->sym->name == unsym->unresolved) {
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

enum FastFollowerCheckType {
  CAN_HAVE_FF,
  STATIC_FF_CHECK,
  DYNAMIC_FF_CHECK
};

static CallExpr *buildFastFollowerCheckForward(std::vector<SymExpr *> exprs,
                                               const char *fnName,
                                               const char *boolOp,
                                               ArgSymbol *lead) {
  if (exprs.size() == 1) {
    CallExpr *ret = new CallExpr(fnName, exprs[0]->copy());
    if (lead != nullptr) {
      ret->insertAtTail(new SymExpr(lead));
    }
    return ret;
  }
  else {
    return buildFastFollowerCheckCallForZipOrProm(exprs, fnName, boolOp, lead);
  }
  return nullptr;
}

static void buildFastFollowerCheck(FastFollowerCheckType checkType,
                                   bool                  addLead,
                                   FnSymbol*             wrapper,
                                   Type*                 IRtype,
                                   std::set<ArgSymbol*>& requiresPromotion) {
  const char* fnName     = nullptr;
  FnSymbol*   checkFn    = nullptr;

  ArgSymbol*  x          = new ArgSymbol(INTENT_BLANK, "x", IRtype);

  std::vector<SymExpr *> fieldSymExprs;

  Symbol*     returnTmp  = newTemp("p_ret");
  CallExpr*   forward    = nullptr;

  returnTmp->addFlag(FLAG_EXPR_TEMP);
  returnTmp->addFlag(FLAG_MAYBE_PARAM);

  const char *boolOp = nullptr;
  switch (checkType) {
    case CAN_HAVE_FF:
      fnName          = "chpl__canHaveFastFollowers";
      checkFn         = new FnSymbol(fnName);
      checkFn->retTag = RET_PARAM;
      boolOp           = "||";
      break;
    case STATIC_FF_CHECK:
      fnName          = "chpl__staticFastFollowCheck";
      checkFn         = new FnSymbol(fnName);
      checkFn->retTag = RET_PARAM;
      boolOp           = "&&";
      break;
    case DYNAMIC_FF_CHECK:
      fnName          = "chpl__dynamicFastFollowCheck";
      checkFn         = new FnSymbol(fnName);
      checkFn->retTag = RET_VALUE;
      boolOp           = "&&";
      break;
    default:
      INT_FATAL("Unknown FastFollowerCheckType");
      break;
  }

  checkFn->addFlag(FLAG_COMPILER_GENERATED);

  checkFn->insertFormalAtTail(x);

  for_formals(formal, wrapper) {
    if (requiresPromotion.count(formal) > 0) {
      Symbol*      field       = new VarSymbol(formal->name, formal->getRefType());

      PrimitiveTag primTag     = PRIM_ITERATOR_RECORD_FIELD_VALUE_BY_FORMAL;
      CallExpr*    byFormal    = new CallExpr(primTag,   x,     formal);
      CallExpr*    moveToField = new CallExpr(PRIM_MOVE, field, byFormal);

      checkFn->insertAtTail(new DefExpr(field));

      checkFn->insertAtTail(moveToField);

      fieldSymExprs.push_back(new SymExpr(field));
    }
  }

  ArgSymbol *lead = nullptr;
  if (addLead == true) {
    lead = new ArgSymbol(INTENT_BLANK, "lead", dtAny);

    checkFn->insertFormalAtTail(lead);
  } else {
    INT_ASSERT(! x->type->symbol->hasFlag(FLAG_GENERIC));
  }

  forward = buildFastFollowerCheckForward(fieldSymExprs, fnName, boolOp, lead);

  checkFn->insertAtTail(new DefExpr(returnTmp));
  checkFn->insertAtTail(new CallExpr(PRIM_MOVE,   returnTmp, forward));
  checkFn->insertAtTail(new CallExpr(PRIM_RETURN, returnTmp));

  wrapper->defPoint->getModule()->block->insertAtHead(new DefExpr(checkFn));

  TransformLogicalShortCircuit handleAndsOrs;
  checkFn->accept(&handleAndsOrs);
  normalize(checkFn);
  checkFn->setGeneric(addLead);
  INT_ASSERT(! wrapper->isGeneric()); //fyi
}

void buildFastFollowerChecksIfNeeded(CallExpr* checkCall) {
  if (checkCall->numActuals() == 0) return; // weird, don't deal handle it

  Type* ir = checkCall->get(1)->getValType();
  if (! ir->symbol->hasFlag(FLAG_ITERATOR_RECORD))
    // We build check fns only for promotion wrappers.
    return;

  FnSymbol* wrapFn = getTheIteratorFn(ir);
  if (promotionFormalsMap.count(wrapFn) == 0)
    // Either this wrapFn has been handled, or we are not supposed to
    // create fast follower checks, ex. for a chpl__loopexpr_iter.
    return;

  std::set<ArgSymbol*>& requiresPromotion = promotionFormalsMap[wrapFn];
  SET_LINENO(wrapFn);

  // Build "canHaveFastFollowers" check functions -- these don't call DSI
  // functions. They are called before calling DSI functions and return true for
  // arrays, false otherwise.
  buildFastFollowerCheck(CAN_HAVE_FF,  false, wrapFn, ir, requiresPromotion);
  buildFastFollowerCheck(CAN_HAVE_FF,  true,  wrapFn, ir, requiresPromotion);

  // Build static (param) fast follower check functions
  buildFastFollowerCheck(STATIC_FF_CHECK,  false, wrapFn, ir, requiresPromotion);
  buildFastFollowerCheck(STATIC_FF_CHECK,  true,  wrapFn, ir, requiresPromotion);

  // Build dynamic fast follower check functions
  buildFastFollowerCheck(DYNAMIC_FF_CHECK, false, wrapFn, ir, requiresPromotion);
  buildFastFollowerCheck(DYNAMIC_FF_CHECK, true,  wrapFn, ir, requiresPromotion);

  // Done with this wrapFn.
  promotionFormalsMap.erase(wrapFn);
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
  wrapper->addFlag(FLAG_COMPILER_GENERATED);

  if (fn->hasFlag(FLAG_INIT_COPY_FN))   wrapper->addFlag(FLAG_INIT_COPY_FN);
  if (fn->hasFlag(FLAG_AUTO_COPY_FN))   wrapper->addFlag(FLAG_AUTO_COPY_FN);
  if (fn->hasFlag(FLAG_AUTO_DESTROY_FN))wrapper->addFlag(FLAG_AUTO_DESTROY_FN);
  if (fn->hasFlag(FLAG_NO_PARENS))      wrapper->addFlag(FLAG_NO_PARENS);
  if (fn->hasFlag(FLAG_FIELD_ACCESSOR)) wrapper->addFlag(FLAG_FIELD_ACCESSOR);
  if (fn->hasFlag(FLAG_REF_TO_CONST))   wrapper->addFlag(FLAG_REF_TO_CONST);
  if (fn->hasFlag(FLAG_METHOD_PRIMARY)) wrapper->addFlag(FLAG_METHOD_PRIMARY);
  if (fn->hasFlag(FLAG_ASSIGNOP))       wrapper->addFlag(FLAG_ASSIGNOP);
  if (fn->hasFlag(FLAG_LAST_RESORT))    wrapper->addFlag(FLAG_LAST_RESORT);

  if (   fn->hasFlag(FLAG_VOID_NO_RETURN_VALUE))
    wrapper->addFlag(FLAG_VOID_NO_RETURN_VALUE);
  if (   fn->hasFlag(FLAG_FN_RETURNS_ITERATOR))
    wrapper->addFlag(FLAG_FN_RETURNS_ITERATOR);
  if (   fn->hasFlag(FLAG_SUPPRESS_LVALUE_ERRORS))
    wrapper->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);
  if (   fn->hasFlag(FLAG_COMPILER_GENERATED))
    wrapper->addFlag(FLAG_WAS_COMPILER_GENERATED); // note "was"

  // getValue is var, not iterator
  if (!fn->isIterator()) wrapper->retTag = fn->retTag;
  if (fn->isMethod())    wrapper->setMethod(true);
  if (fn->throwsError()) wrapper->throwsErrorInit();

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
