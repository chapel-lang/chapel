/*
 * Copyright 2004-2015 Cray Inc.
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

// wrappers.cpp
////////////////////////////////////////////////////////////////////////////////{
// Wrappers are used to lower the Chapel idea of a function call to something
// implementable in C.
//  default wrapper -- supplies a value for every argument in the called function
//      substituting default values for actual arguments that are omitted.
//      (C does not support default values for arguments.)
//  reorder named actual arguments to match the order expected by the inner
//      function, i.e. the order of the formals (used to be order wrapper)
//      (C does not support named argument passing.)
//  coercion wrapper -- add explicit casts to perform type coercions known only
//      to Chapel.
//      (C does not support base-class coercions, etc.)
//  promotion wrapper -- replaces implicit array traversals with explicit
//      array traversals.
//      (C has no notion of scalar operator promotion.)
////////////////////////////////////////////////////////////////////////////////}

#include "resolution.h"

#include "astutil.h"
#include "build.h"
#include "caches.h"
#include "callInfo.h"
#include "chpl.h"
#include "expr.h"
#include "ForLoop.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"


//########################################################################
//# Static Function Forward Declarations
//########################################################################
static FnSymbol*
buildEmptyWrapper(FnSymbol* fn, CallInfo* info);
static ArgSymbol* copyFormalForWrapper(ArgSymbol* formal);
static void
insertWrappedCall(FnSymbol* fn, FnSymbol* wrapper, CallExpr* call);
static FnSymbol*
buildDefaultWrapper(FnSymbol* fn,
                    Vec<Symbol*>* defaults,
                    SymbolMap* paramMap,
                    CallInfo* info);
static FnSymbol*
buildPromotionWrapper(FnSymbol* fn,
                      SymbolMap* promotion_subs,
                      CallInfo* info);

//########################################################################


static FnSymbol*
buildEmptyWrapper(FnSymbol* fn, CallInfo* info) {
  FnSymbol* wrapper = new FnSymbol(fn->name);
  // TODO: Make this less verbose by bulk-copying flags from the original
  // function and then negating flags we don't want.
  wrapper->addFlag(FLAG_WRAPPER);
  wrapper->addFlag(FLAG_INVISIBLE_FN);
  wrapper->addFlag(FLAG_INLINE);
  if (fn->hasFlag(FLAG_INIT_COPY_FN))
    wrapper->addFlag(FLAG_INIT_COPY_FN);
  if (fn->hasFlag(FLAG_AUTO_COPY_FN))
    wrapper->addFlag(FLAG_AUTO_COPY_FN);
  if (fn->hasFlag(FLAG_AUTO_DESTROY_FN))
    wrapper->addFlag(FLAG_AUTO_DESTROY_FN);
  if (fn->hasFlag(FLAG_DONOR_FN))
    wrapper->addFlag(FLAG_DONOR_FN);
  if (fn->hasFlag(FLAG_NO_PARENS))
    wrapper->addFlag(FLAG_NO_PARENS);
  if (fn->hasFlag(FLAG_CONSTRUCTOR))
    wrapper->addFlag(FLAG_CONSTRUCTOR);
  if (fn->hasFlag(FLAG_FIELD_ACCESSOR))
    wrapper->addFlag(FLAG_FIELD_ACCESSOR);
  if (fn->hasFlag(FLAG_REF_TO_CONST))
    wrapper->addFlag(FLAG_REF_TO_CONST);
  if (!fn->hasFlag(FLAG_ITERATOR_FN)) { // getValue is var, not iterator
    wrapper->retTag = fn->retTag;
    if (fn->setter)
      wrapper->setter = fn->setter->copy();
  }
  if (fn->hasFlag(FLAG_METHOD))
    wrapper->addFlag(FLAG_METHOD);
  if (fn->hasFlag(FLAG_METHOD_PRIMARY))
    wrapper->addFlag(FLAG_METHOD_PRIMARY);
  if (fn->hasFlag(FLAG_ASSIGNOP))
    wrapper->addFlag(FLAG_ASSIGNOP);
  wrapper->instantiationPoint = getVisibilityBlock(info->call);
  if (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR))
    wrapper->addFlag(FLAG_DEFAULT_CONSTRUCTOR);
  if (fn->hasFlag(FLAG_COMPILER_GENERATED))
    wrapper->addFlag(FLAG_WAS_COMPILER_GENERATED);
  wrapper->addFlag(FLAG_COMPILER_GENERATED);
  return wrapper;
}


//
// copy a formal and make the copy have blank intent. If the formal to copy has
// out intent or inout intent, flag the copy to make sure it is a reference
// If the formal is ref intent, leave it as ref on the wrapper formal.
//
static ArgSymbol* copyFormalForWrapper(ArgSymbol* formal) {
  ArgSymbol* wrapperFormal = formal->copy();
  if (formal->intent == INTENT_OUT || formal->intent == INTENT_INOUT ||
      formal->hasFlag(FLAG_WRAP_WRITTEN_FORMAL)) {
    wrapperFormal->addFlag(FLAG_WRAP_WRITTEN_FORMAL);
  }
  if (formal->intent != INTENT_REF && formal->intent != INTENT_CONST_REF) {
    wrapperFormal->intent = INTENT_BLANK;
  }
  return wrapperFormal;
}


static void
insertWrappedCall(FnSymbol* fn, FnSymbol* wrapper, CallExpr* call) {
  if (fn->getReturnSymbol() == gVoid || fn->retType == dtVoid) {
    wrapper->insertAtTail(call);
  } else {
    Symbol* tmp = newTemp("wrap_call_tmp");
    tmp->addFlag(FLAG_EXPR_TEMP);
    tmp->addFlag(FLAG_MAYBE_PARAM);
    tmp->addFlag(FLAG_MAYBE_TYPE);
    wrapper->insertAtTail(new DefExpr(tmp));
    wrapper->insertAtTail(new CallExpr(PRIM_MOVE, tmp, call));
    wrapper->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
  }
  fn->defPoint->insertAfter(new DefExpr(wrapper));
}


////
//// default wrapper code
////


static FnSymbol*
buildDefaultWrapper(FnSymbol* fn,
                    Vec<Symbol*>* defaults,
                    SymbolMap* paramMap,
                    CallInfo* info) {
  if (FnSymbol* cached = checkCache(defaultsCache, fn, defaults))
    return cached;
  SET_LINENO(fn);
  FnSymbol* wrapper = buildEmptyWrapper(fn, info);
  // Prevent name-clash in generated code.
  // Also, provide a hint where this fcn came from.
  wrapper->cname = astr("_default_wrap_", fn->cname);

  // Mimic return type.
  if (!fn->hasFlag(FLAG_ITERATOR_FN))
    wrapper->retType = fn->retType;

  SymbolMap copy_map;
  bool specializeDefaultConstructor =
    fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) &&
    !isSyncType(fn->_this->type) &&
    !fn->_this->type->symbol->hasFlag(FLAG_REF);
  if (specializeDefaultConstructor) {
    wrapper->removeFlag(FLAG_COMPILER_GENERATED);
    wrapper->_this = fn->_this->copy();
    copy_map.put(fn->_this, wrapper->_this);
    wrapper->insertAtTail(new DefExpr(wrapper->_this));
    if (defaults->v[defaults->n-1]->hasFlag(FLAG_IS_MEME)) {
      if (!isRecord(fn->_this->type) && !isUnion(fn->_this->type)) {
        wrapper->insertAtTail(new CallExpr(PRIM_MOVE, wrapper->_this,
                                           callChplHereAlloc((wrapper->_this->typeInfo())->symbol)));
        wrapper->insertAtTail(new CallExpr(PRIM_SETCID, wrapper->_this));
      }
    }
    // This call is required to establish the type of _this.
//    wrapper->insertAtTail(new CallExpr(PRIM_INIT_FIELDS, wrapper->_this));
  }
  CallExpr* call = new CallExpr(fn);
  call->square = info->call->square;    // Copy square brackets call flag.

  // Now walk the formals list of the called function, and expand formal
  // argument defaults as needed, so every formal in the called function
  // has a matching actual argument in the call.
  for_formals(formal, fn) {
    SET_LINENO(formal);

    if (!defaults->in(formal)) {
      // This formal does not require a default value.  It appears
      // in the list of actual argument supplied to the call.

      // Copy it into the formals for this wrapper function.
      ArgSymbol* wrapper_formal = copyFormalForWrapper(formal);
      if (fn->_this == formal)
        wrapper->_this = wrapper_formal;
      if (formal->hasFlag(FLAG_IS_MEME))
        wrapper->_this->defPoint->insertAfter(new CallExpr(PRIM_MOVE, wrapper->_this, wrapper_formal)); // unexecuted none/gasnet on 4/25/08
      wrapper->insertFormalAtTail(wrapper_formal);

      // By default, we simply pass the wrapper formal along to the wrapped function,
      // but there are some special cases where some fixup is required.
      Symbol* temp = wrapper_formal;

      // Check for the fixup cases:
      if (formal->type->symbol->hasFlag(FLAG_REF)) {
        // Formal is passed by reference.
        temp = newTemp("wrap_ref_arg");
        temp->addFlag(FLAG_MAYBE_PARAM);
        wrapper->insertAtTail(new DefExpr(temp));
        wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, new CallExpr(PRIM_ADDR_OF, wrapper_formal)));
      } else if (specializeDefaultConstructor && wrapper_formal->typeExpr &&
                 isRefCountedType(wrapper_formal->type)) {
        // Formal has a type expression attached and is reference counted (?).
        temp = newTemp("wrap_type_arg");
#ifndef HILDE_MM
        if (Symbol* field = fn->_this->type->getField(formal->name, false))
          if (field->defPoint->parentSymbol == fn->_this->type->symbol)
            temp->addFlag(FLAG_INSERT_AUTO_DESTROY);
#endif
        wrapper->insertAtTail(new DefExpr(temp));

        // Give the formal its own copy of the type expression.
        BlockStmt* typeExpr = wrapper_formal->typeExpr->copy();
        for_alist(expr, typeExpr->body) {
          wrapper->insertAtTail(expr->remove());
        }

        // The type of an array is computed at runtime, so we have to insert
        // some type computation code if the argument is an array alias field.
        bool isArrayAliasField = false;
        const char* aliasFieldArg = astr("chpl__aliasField_", formal->name);
        for_formals(formal, fn)
          if (formal->name == aliasFieldArg && !defaults->set_in(formal))
            isArrayAliasField = true;
        if (isArrayAliasField) {
          // The array type is the return type of this wrapper.
          Expr* arrayTypeExpr = wrapper->body->body.tail->remove();
          Symbol* arrayTypeTmp = newTemp("wrap_array_alias");
          arrayTypeTmp->addFlag(FLAG_MAYBE_TYPE);
          arrayTypeTmp->addFlag(FLAG_EXPR_TEMP);
          temp->addFlag(FLAG_EXPR_TEMP);
          // Add the type marker temporary, and use it to reindex this formal
          // before it is passed to the called function.
          wrapper->insertAtTail(new DefExpr(arrayTypeTmp));
          wrapper->insertAtTail(new CallExpr(PRIM_MOVE, arrayTypeTmp, arrayTypeExpr));
          wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, new CallExpr("reindex", gMethodToken, wrapper_formal, new CallExpr("chpl__getDomainFromArrayType", arrayTypeTmp))));
        } else {
          // Not an array alias field.  Just initialize this formal with
          // its default type expression.
          wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, new CallExpr(PRIM_INIT, wrapper->body->body.tail->remove())));
          wrapper->insertAtTail(new CallExpr("=", temp, wrapper_formal));
        }
      }

      // Add this formal to the map used to copy the function definition.
      // (Not sure why it is done this way, since we can also make up the 
      // wrapper from whole cloth.)
      copy_map.put(formal, temp);
      call->insertAtTail(temp);

      // If the wrapped formal is satisfied by a parameter, copy that parameter value 
      // into the wrapper formal as well.
      if (Symbol* value = paramMap->get(formal))
        paramMap->put(wrapper_formal, value);
      if (specializeDefaultConstructor && strcmp(fn->name, "_construct__tuple"))
        if (!formal->hasFlag(FLAG_TYPE_VARIABLE) && !paramMap->get(formal) && formal->type != dtMethodToken)
          if (Symbol* field = wrapper->_this->type->getField(formal->name, false))
            if (field->defPoint->parentSymbol == wrapper->_this->type->symbol)
            {
              Symbol* copyTemp = newTemp("wrap_arg");
              wrapper->insertAtTail(new DefExpr(copyTemp));
              // TODO AMM: Insertion of this autocopy can probably be left to
              // ownership flow analysis.  Since the LHS is a structure member,
              // the copy is required, but only if the RHS object is not owned.
              wrapper->insertAtTail(new CallExpr(PRIM_MOVE, copyTemp, new CallExpr("chpl__autoCopy", temp)));
              wrapper->insertAtTail(
                new CallExpr(PRIM_SET_MEMBER, wrapper->_this,
                             new_StringSymbol(field->name), copyTemp));
              copy_map.put(formal, copyTemp);
              call->argList.tail->replace(new SymExpr(copyTemp));
            }
    } else if (paramMap->get(formal)) {
      // handle instantiated param formals
      call->insertAtTail(paramMap->get(formal));
    } else if (formal->hasFlag(FLAG_IS_MEME)) {

      //
      // hack: why is the type of meme set to dtNil?
      //
      formal->type = wrapper->_this->type;

      call->insertAtTail(wrapper->_this);
    } else {
      const char* temp_name = astr("default_arg", formal->name);
      VarSymbol* temp = newTemp(temp_name);
      if (formal->intent != INTENT_INOUT && formal->intent != INTENT_OUT) {
        temp->addFlag(FLAG_MAYBE_PARAM);
        temp->addFlag(FLAG_EXPR_TEMP);
      }
      if (formal->hasFlag(FLAG_TYPE_VARIABLE))
        temp->addFlag(FLAG_TYPE_VARIABLE);
      copy_map.put(formal, temp);
      wrapper->insertAtTail(new DefExpr(temp));
      if (formal->intent == INTENT_OUT ||
          !formal->defaultExpr ||
          (formal->defaultExpr->body.length == 1 &&
           isSymExpr(formal->defaultExpr->body.tail) &&
           toSymExpr(formal->defaultExpr->body.tail)->var == gTypeDefaultToken)) {
        // use default value for type as default value for formal argument
        if (formal->typeExpr) {
          BlockStmt* typeExpr = formal->typeExpr->copy();
          for_alist(expr, typeExpr->body) {
            wrapper->insertAtTail(expr->remove());
          }
          if (formal->hasFlag(FLAG_TYPE_VARIABLE))
            wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, wrapper->body->body.tail->remove()));
          else
            wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, new CallExpr(PRIM_INIT, wrapper->body->body.tail->remove())));
        } else {
          if (formal->hasFlag(FLAG_TYPE_VARIABLE))
            wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, new SymExpr(formal->type->symbol)));
          else
            wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, new CallExpr(PRIM_INIT, new SymExpr(formal->type->symbol))));
        }
      } else {
        BlockStmt* defaultExpr = formal->defaultExpr->copy();
        for_alist(expr, defaultExpr->body) {
          wrapper->insertAtTail(expr->remove());
        }
        if (formal->intent != INTENT_INOUT) {
          wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, wrapper->body->body.tail->remove()));
        } else {
          // This calls the copy-construtor, to copy the return value into the calling context.
          wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, new CallExpr("chpl__initCopy", wrapper->body->body.tail->remove())));
          INT_ASSERT(!temp->hasFlag(FLAG_EXPR_TEMP));
          temp->removeFlag(FLAG_MAYBE_PARAM);
        }
      }
      call->insertAtTail(temp);
      if (specializeDefaultConstructor && strcmp(fn->name, "_construct__tuple"))
        if (!formal->hasFlag(FLAG_TYPE_VARIABLE))
          if (Symbol* field = wrapper->_this->type->getField(formal->name, false))
            if (field->defPoint->parentSymbol == wrapper->_this->type->symbol)
              wrapper->insertAtTail(
                new CallExpr(PRIM_SET_MEMBER, wrapper->_this,
                             new_StringSymbol(formal->name), temp));
    }
  }
  update_symbols(wrapper->body, &copy_map);

  insertWrappedCall(fn, wrapper, call);
  normalize(wrapper);
  return wrapper;
}


FnSymbol*
defaultWrap(FnSymbol* fn,
            Vec<ArgSymbol*>* actualFormals,
            CallInfo* info) {
  FnSymbol* wrapper = fn;
  int num_actuals = actualFormals->n;
  int num_formals = fn->numFormals();
  if (num_formals > num_actuals) {
    Vec<Symbol*> defaults;
    for_formals(formal, fn) {
      bool used = false;
      forv_Vec(ArgSymbol, arg, *actualFormals) {
        if (arg == formal)
          used = true;
      }
      if (!used)
        defaults.add(formal);
    }

    wrapper = checkCache(defaultsCache, fn, &defaults);
    if (wrapper == NULL) {
      wrapper = buildDefaultWrapper(fn, &defaults, &paramMap, info);
      addCache(defaultsCache, fn, wrapper, &defaults);
    }

    resolveFormals(wrapper);

    // update actualFormals for use in reorderActuals
    int j = 1;
    for_formals(formal, fn) {
      for (int i = 0; i < actualFormals->n; i++) {
        if (actualFormals->v[i] == formal) {
          ArgSymbol* newFormal = wrapper->getFormal(j);
          actualFormals->v[i] = newFormal;
          j++;
        }
      }
    }
  }
  return wrapper;
}


////
//// reorder the actuals to match the order of the formals
//// (this function is here because it used to create a wrapper)
////

void reorderActuals(FnSymbol* fn,
          Vec<ArgSymbol*>* actualFormals,
          CallInfo* info) {
  int numArgs = actualFormals->n;
  if (numArgs <= 1)
    return;  // no way we will need to reorder

  bool need_to_reorder = false;
  std::vector<int> formals_to_formals(numArgs);
  int i = 0;
  for_formals(formal, fn) {
    i++;

    int j = 0;
    forv_Vec(ArgSymbol, af, *actualFormals) {
      j++;
      if (af == formal) {
        if (i != j)
          need_to_reorder = true;
        formals_to_formals[i-1] = j-1;
      }
    }
  }
  if (need_to_reorder) {
    std::vector<Expr*> savedActuals(numArgs);
    int i = 0;
    // remove all actuals in an order
    for_actuals(actual, info->call)
      savedActuals[i++] = actual->remove();
    // reinsert them in the desired order
    for (i = 0; i < numArgs; i++)
      info->call->insertAtTail(savedActuals[formals_to_formals[i]]);
    // reorder CallInfo data as well
    // ideally this would be encapsulated in within the CallInfo class
    INT_ASSERT(info->actuals.n == numArgs);
    std::vector<Symbol*> ciActuals(numArgs);
    std::vector<const char*> ciActualNames(numArgs);
    for (i = 0; i < numArgs; i++)
      ciActuals[i] = info->actuals.v[i],
      ciActualNames[i] = info->actualNames.v[i];
    for (i = 0; i < numArgs; i++)
      info->actuals.v[i] = ciActuals[formals_to_formals[i]],
      info->actualNames.v[i] = ciActualNames[formals_to_formals[i]];
  }
  return;
}


// do we need to add some coercion from the actual to the formal?
static bool needToAddCoercion(Type* actualType, Symbol* actualSym,
                              Type* formalType, FnSymbol* fn) {
  if (actualType == formalType)
    return false;
  else
    return canCoerce(actualType, actualSym, formalType, fn) ||
           isDispatchParent(actualType, formalType);
}

// Add a coercion; replace prevActual and actualSym - the actual to 'call' -
// with the result of the coercion.
static void addArgCoercion(FnSymbol* fn, CallExpr* call, ArgSymbol* formal,
                           Expr*& actualExpr, Symbol*& actualSym,
                           bool& checkAgain)
{
  Expr* prevActual = actualExpr;
  SET_LINENO(prevActual);
  TypeSymbol* ats = actualSym->type->symbol;
  TypeSymbol* fts = formal->type->symbol;
  CallExpr* castCall;
  VarSymbol* castTemp = newTemp("coerce_tmp"); // ..., formal->type ?
  castTemp->addFlag(FLAG_COERCE_TEMP);
  // gotta preserve this-ness, so can write to this's fields in constructors
  if (actualSym->hasFlag(FLAG_ARG_THIS) &&
      isDispatchParent(actualSym->type, formal->type))
    castTemp->addFlag(FLAG_ARG_THIS);

  Expr* newActual = new SymExpr(castTemp);
  if (NamedExpr* namedActual = toNamedExpr(prevActual)) {
    // preserve the named portion
    Expr* newCurrActual = namedActual->actual;
    newCurrActual->replace(newActual);
    newActual = prevActual;
    prevActual = newCurrActual;
  } else {
    prevActual->replace(newActual);
  }
  // Now 'prevActual' has been removed+replaced and is ready to be passed
  // as an actual to a cast or some such.
  // We can update addArgCoercion's caller right away.
  actualExpr = newActual;
  actualSym  = castTemp;

  if (getSyncFlags(ats).any()) {

    // Tom notes: Ultimately, I hope to push all code related to sync
    // variable implementation into module code.  Moving the special
    // handling of sync demotion (i.e. extracting the underlying value
    // from a sync variable) into module code would render this
    // special-case code in the compiler moot.

    // Here we will often strip the type of its sync-ness.
    // After that we may need another coercion(s), e.g.
    //   _syncvar(int) --readFE()-> _ref(int) --(dereference)-> int --> real
    // or
    //   _syncvar(_syncvar(int))  -->...  _syncvar(int)  -->  [as above]
    //
    // We warn addArgCoercion's caller about that via checkAgain:
    checkAgain = true;

    //
    // apply readFF or readFE to single or sync actual unless this
    // is a member access of the sync or single actual
    //
    if (fn->numFormals() == 3 &&
        !strcmp(fn->name, "free"))
      // Don't insert a readFE or readFF when deleting a sync/single.
      castCall = NULL;

    else if (fn->numFormals() >= 2 &&
             fn->getFormal(1)->type == dtMethodToken &&
             formal == fn->_this)
      // NB if this case is removed, reduce the checksLeft number below.
      castCall = new CallExpr("value",  gMethodToken, prevActual);

    else if (ats->hasFlag(FLAG_SYNC))
      castCall = new CallExpr("readFE", gMethodToken, prevActual);

    else if (ats->hasFlag(FLAG_SINGLE))
      castCall = new CallExpr("readFF", gMethodToken, prevActual);

    else {
      INT_ASSERT(false);    // Unhandled case.
      castCall = NULL;      // make gcc happy
    }

  } else if (ats->hasFlag(FLAG_REF)) {
    //
    // dereference a reference actual
    //
    // after dereferencing we may need another coercion, e.g.
    //   _ref(int)  --coerce->  int  --coerce->  real
    // or
    //   _ref(_syncvar(int)) --> _syncvar(int) --> _ref(int) --> int --> real
    //
    checkAgain = true;
    castCall = new CallExpr(PRIM_DEREF, prevActual);

  } else {
    // There was code to handle the case when the flag *is* present.
    // I deleted that code. The assert ensures it wouldn't apply anyway.
    INT_ASSERT(!actualSym->hasFlag(FLAG_INSTANTIATED_PARAM));
    castCall = NULL;
  }

  if (castCall == NULL)
    // the common case
    castCall = new CallExpr("_cast", fts, prevActual);

  // move the result to the temp
  CallExpr* castMove = new CallExpr(PRIM_MOVE, castTemp, castCall);

  call->getStmtExpr()->insertBefore(new DefExpr(castTemp));
  call->getStmtExpr()->insertBefore(castMove);

  resolveCall(castCall);
  if (castCall->isResolved())
    resolveFns(castCall->isResolved());

  resolveCall(castMove);
}


////
//// add coercions on the actuals
//// (this function is here because it used to create a wrapper)
////

void coerceActuals(FnSymbol* fn, CallInfo* info) {
  if (info->actuals.n < 1)
    return; // nothing to do

  if (fn->retTag == RET_PARAM)
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

  int j = -1;
  Expr* currActual = info->call->get(1);
  for_formals(formal, fn) {
    j++;
    Symbol* actualSym = info->actuals.v[j];
    Type* formalType = formal->type;
    bool c2; // will we need to check again?

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
    int checksLeft = 6;

    do {
      c2 = false;
      Type* actualType = actualSym->type;
      if (needToAddCoercion(actualType, actualSym, formalType, fn)) {
        // addArgCoercion() updates currActual, actualSym, c2
        addArgCoercion(fn, info->call, formal, currActual, actualSym, c2);
      }
    } while (c2 && --checksLeft > 0);

    INT_ASSERT(!c2); // otherwise need to increase checksLeft
    currActual = currActual->next;
  }
}  // coerceActuals()


////
//// promotion wrapper code
////


static FnSymbol*
buildPromotionWrapper(FnSymbol* fn,
                      SymbolMap* promotion_subs,
                      CallInfo* info) {
  SET_LINENO(info->call);
  FnSymbol* wrapper = buildEmptyWrapper(fn, info);
  wrapper->addFlag(FLAG_PROMOTION_WRAPPER);
  // Special case: When promoting a default constructor, the promotion wrapper
  // itself is no longer a default constructor.
  wrapper->removeFlag(FLAG_DEFAULT_CONSTRUCTOR);
  wrapper->cname = astr("_promotion_wrap_", fn->cname);
  CallExpr* indicesCall = new CallExpr("_build_tuple"); // destructured in build
  CallExpr* iteratorCall = new CallExpr("_build_tuple");
  CallExpr* actualCall = new CallExpr(fn);
  bool zippered = true;
  int i = 1;
  for_formals(formal, fn) {
    SET_LINENO(formal);
    ArgSymbol* new_formal = copyFormalForWrapper(formal);
    if (Symbol* p = paramMap.get(formal))
      paramMap.put(new_formal, p);
    if (fn->_this == formal)
      wrapper->_this = new_formal;
    if (Symbol* sub = promotion_subs->get(formal)) {
      TypeSymbol* ts = toTypeSymbol(sub);
      if (!ts)
        INT_FATAL(fn, "error building promotion wrapper");
      new_formal->type = ts->type;
      wrapper->insertFormalAtTail(new_formal);
      iteratorCall->insertAtTail(new_formal);
      VarSymbol* index = newTemp(astr("p_i_", istr(i)));
      wrapper->insertAtTail(new DefExpr(index));
      indicesCall->insertAtTail(index);
      actualCall->insertAtTail(index);
    } else {
      wrapper->insertFormalAtTail(new_formal);
      actualCall->insertAtTail(new_formal);
    }
    i++;
  }

  // Convert 1-tuples to their contents for the second half of this function
  Expr* indices = indicesCall;
  if (indicesCall->numActuals() == 1)
    indices = indicesCall->get(1)->remove();

  Expr* iterator = iteratorCall;
  if (iteratorCall->numActuals() == 1) {
    iterator = iteratorCall->get(1)->remove();
    zippered = false;
  }

  if ((!fn->hasFlag(FLAG_EXTERN) && fn->getReturnSymbol() == gVoid) ||
      (fn->hasFlag(FLAG_EXTERN) && fn->retType == dtVoid)) {
      wrapper->insertAtTail(new BlockStmt(buildForallLoopStmt(indices, iterator, /*byref_vars=*/ NULL, new BlockStmt(actualCall), zippered)));
  } else {
    wrapper->addFlag(FLAG_ITERATOR_FN);
    wrapper->removeFlag(FLAG_INLINE);

    SymbolMap leaderMap;
    FnSymbol* lifn = wrapper->copy(&leaderMap);
    INT_ASSERT(! lifn->hasFlag(FLAG_RESOLVED));
    iteratorLeaderMap.put(wrapper,lifn);
    lifn->body = new BlockStmt(); // indices are not used in leader
    form_Map(SymbolMapElem, e, leaderMap) {
      if (Symbol* s = paramMap.get(e->key))
        paramMap.put(e->value, s);
    }
    ArgSymbol* lifnTag = new ArgSymbol(INTENT_PARAM, "tag", gLeaderTag->type);
    lifn->addFlag(FLAG_INLINE_ITERATOR); // Leader iterators are always inlined.
    lifn->insertFormalAtTail(lifnTag);
    lifn->where = new BlockStmt(new CallExpr("==", lifnTag, gLeaderTag));
    VarSymbol* leaderIndex = newTemp("p_leaderIndex");
    VarSymbol* leaderIterator = newTemp("p_leaderIterator");
    leaderIterator->addFlag(FLAG_EXPR_TEMP);
    lifn->insertAtTail(new DefExpr(leaderIterator));

    if( !zippered ) {
      lifn->insertAtTail(new CallExpr(PRIM_MOVE, leaderIterator, new CallExpr("_toLeader", iterator->copy(&leaderMap))));
    } else {
      lifn->insertAtTail(new CallExpr(PRIM_MOVE, leaderIterator, new CallExpr("_toLeaderZip", iterator->copy(&leaderMap))));
    }

    BlockStmt* body = new BlockStmt(new CallExpr(PRIM_YIELD, leaderIndex));
    BlockStmt* loop = ForLoop::buildForLoop(new SymExpr(leaderIndex), new SymExpr(leaderIterator), body, false, zippered);
    lifn->insertAtTail(loop);
    theProgram->block->insertAtTail(new DefExpr(lifn));
    toBlockStmt(body->parentExpr)->insertAtHead(new DefExpr(leaderIndex));
    normalize(lifn);
    lifn->instantiationPoint = getVisibilityBlock(info->call);

    SymbolMap followerMap;
    FnSymbol* fifn = wrapper->copy(&followerMap);
    INT_ASSERT(! fifn->hasFlag(FLAG_RESOLVED));
    iteratorFollowerMap.put(wrapper,fifn);
    form_Map(SymbolMapElem, e, followerMap) {
      if (Symbol* s = paramMap.get(e->key))
        paramMap.put(e->value, s);
    }
    ArgSymbol* fifnTag = new ArgSymbol(INTENT_PARAM, "tag", gFollowerTag->type);
    fifn->insertFormalAtTail(fifnTag);
    ArgSymbol* fifnFollower = new ArgSymbol(INTENT_BLANK, iterFollowthisArgname, dtAny);
    fifn->insertFormalAtTail(fifnFollower);
    fifn->where = new BlockStmt(new CallExpr("==", fifnTag, gFollowerTag));
    VarSymbol* followerIterator = newTemp("p_followerIterator");
    followerIterator->addFlag(FLAG_EXPR_TEMP);
    fifn->insertAtTail(new DefExpr(followerIterator));

    if( !zippered ) {
      fifn->insertAtTail(new CallExpr(PRIM_MOVE, followerIterator, new CallExpr("_toFollower", iterator->copy(&followerMap), fifnFollower)));
    } else {
      Expr* tMe = iterator->copy(&followerMap);
      fifn->insertAtTail(new CallExpr(PRIM_MOVE, followerIterator, new
                  CallExpr("_toFollowerZip", tMe, fifnFollower)));
    }

    BlockStmt* followerBlock = new BlockStmt();
    Symbol* yieldTmp = newTemp("p_yield");
    yieldTmp->addFlag(FLAG_EXPR_TEMP);
    followerBlock->insertAtTail(new DefExpr(yieldTmp));
    followerBlock->insertAtTail(new CallExpr(PRIM_MOVE, yieldTmp, actualCall->copy(&followerMap)));
    followerBlock->insertAtTail(new CallExpr(PRIM_YIELD, yieldTmp));
    fifn->insertAtTail(ForLoop::buildForLoop(indices->copy(&followerMap), new SymExpr(followerIterator), followerBlock, false, zippered));
    theProgram->block->insertAtTail(new DefExpr(fifn));
    normalize(fifn);
    fifn->addFlag(FLAG_GENERIC);
    fifn->instantiationPoint = getVisibilityBlock(info->call);

    BlockStmt* yieldBlock = new BlockStmt();
    yieldTmp = newTemp("p_yield");
    yieldTmp->addFlag(FLAG_EXPR_TEMP);
    yieldBlock->insertAtTail(new DefExpr(yieldTmp));
    yieldBlock->insertAtTail(new CallExpr(PRIM_MOVE, yieldTmp, actualCall));
    yieldBlock->insertAtTail(new CallExpr(PRIM_YIELD, yieldTmp));
    wrapper->insertAtTail(new BlockStmt(ForLoop::buildForLoop(indices, iterator, yieldBlock, false, zippered)));
  }
  fn->defPoint->insertBefore(new DefExpr(wrapper));
  normalize(wrapper);
  return wrapper;
}


FnSymbol*
promotionWrap(FnSymbol* fn, CallInfo* info) {
  Vec<Symbol*>* actuals = &info->actuals;
  if (!strcmp(fn->name, "="))
    return fn;
  bool promotion_wrapper_required = false;
  SymbolMap promoted_subs;
  int j = -1;
  for_formals(formal, fn) {
    j++;
    Type* actualType = actuals->v[j]->type;
    Symbol* actualSym = actuals->v[j];
    bool promotes = false;
    if (canDispatch(actualType, actualSym, formal->type, fn, &promotes)){
      if (promotes) {
        promotion_wrapper_required = true;
        promoted_subs.put(formal, actualType->symbol);
      }
    }
  }
  if (promotion_wrapper_required) {
    if (fReportPromotion)
      USR_WARN(info->call, "promotion on %s", toString(info));

    promoted_subs.put(fn, (Symbol*)info->call->square); // add value of square to cache

    FnSymbol* wrapper = checkCache(promotionsCache, fn, &promoted_subs);
    if (wrapper == NULL) {
      wrapper = buildPromotionWrapper(fn, &promoted_subs, info);
      addCache(promotionsCache, fn, wrapper, &promoted_subs);
    }
    resolveFormals(wrapper);
    return wrapper;
  }
  return fn;
}
