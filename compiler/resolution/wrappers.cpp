/*
 * Copyright 2004-2017 Cray Inc.
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
#include "passes.h"
#include "resolveIntents.h"
#include "stlUtil.h"
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
                      CallInfo* info,
                      bool buildFastFollowerChecks);

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
  if (!fn->isIterator()) { // getValue is var, not iterator
    wrapper->retTag = fn->retTag;
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
  if (!fn->isIterator())
    wrapper->retType = fn->retType;

  SymbolMap copy_map;

  bool specializeDefaultConstructor =
    fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) &&
    !fn->_this->type->symbol->hasFlag(FLAG_REF);
  if (specializeDefaultConstructor) {
    wrapper->removeFlag(FLAG_COMPILER_GENERATED);
    wrapper->_this = fn->_this->copy();

    copy_map.put(fn->_this, wrapper->_this);

    wrapper->insertAtTail(new DefExpr(wrapper->_this));

    if (defaults->v[defaults->n-1]->hasFlag(FLAG_IS_MEME)) {
      if (!isRecord(fn->_this->type) && !isUnion(fn->_this->type)) {
        wrapper->insertAtTail(new CallExpr(PRIM_MOVE,
                                           wrapper->_this,
                                           callChplHereAlloc(wrapper->_this->typeInfo())));

        wrapper->insertAtTail(new CallExpr(PRIM_SETCID, wrapper->_this));
      }
    }

    wrapper->insertAtTail(new CallExpr(PRIM_INIT_FIELDS, wrapper->_this));
  }

  CallExpr* call = new CallExpr(fn);

  call->square = info->call->square;    // Copy square brackets call flag.

  // Now walk the formals list of the called function, and expand formal
  // argument defaults as needed, so every formal in the called function
  // has a matching actual argument in the call.
  for_formals(formal, fn) {

    IntentTag intent = formal->intent;
    if (formal->type != dtTypeDefaultToken &&
        formal->type != dtMethodToken &&
        formal->intent == INTENT_BLANK)
      intent = blankIntentForType(formal->type);

    SET_LINENO(formal);

    if (!defaults->in(formal)) {
      // This formal does not require a default value.  It appears
      // in the list of actual argument supplied to the call.

      // Copy it into the formals for this wrapper function.
      ArgSymbol* wrapper_formal = copyFormalForWrapper(formal);
      if (fn->_this == formal)
        wrapper->_this = wrapper_formal;
      if (formal->hasFlag(FLAG_IS_MEME)) {
        if (wrapper->_this != NULL) {
          wrapper->_this->defPoint->insertAfter(new CallExpr(PRIM_MOVE, wrapper->_this, wrapper_formal)); // unexecuted none/gasnet on 4/25/08
        }
      }
      wrapper->insertFormalAtTail(wrapper_formal);

      // By default, we simply pass the wrapper formal along to the wrapped function,
      // but there are some special cases where some fixup is required.
      Symbol* temp = wrapper_formal;

      bool isArrayAliasField = false;

      // Check for the fixup cases:
      if (formal->type->symbol->hasFlag(FLAG_REF)) {
        // Formal is passed by reference.
        // MPF - shouldn't this code also check that wrapper_formal
        // doesn't also have FLAG_REF? Or maybe there is a better way
        // to write this? When does this code fire? Shouldn't it be
        // using the ref intent instead?
        temp = newTemp("wrap_ref_arg");
        temp->addFlag(FLAG_MAYBE_PARAM);
        wrapper->insertAtTail(new DefExpr(temp));
        wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, new CallExpr(PRIM_ADDR_OF, wrapper_formal)));
      } else if (specializeDefaultConstructor && wrapper_formal->typeExpr &&
                 isRecordWrappedType(wrapper_formal->type)) {
        // Formal has a type expression attached and is array/dom/dist

        temp = newTemp("wrap_type_arg");
        if (Symbol* field = fn->_this->type->getField(formal->name, false))
          if (field->defPoint->parentSymbol == fn->_this->type->symbol)
            temp->addFlag(FLAG_INSERT_AUTO_DESTROY);
        wrapper->insertAtTail(new DefExpr(temp));

        isArrayAliasField = false;
        const char* aliasFieldArg = astr("chpl__aliasField_", formal->name);
        for_formals(fml, fn)
          if (fml->name == aliasFieldArg && !defaults->set_in(fml))
            isArrayAliasField = true;

        // Array alias fields initialization is different because
        // no copy of the array elements occurs.
        if (isArrayAliasField) {

          temp->addFlag(FLAG_EXPR_TEMP);
          temp->addFlag(FLAG_NO_AUTO_DESTROY);

          wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, wrapper_formal));

        } else {
          // Not an array alias field.  Just initialize this formal with
          // its default type expression.

          // Give the formal its own copy of the type expression.
          BlockStmt* typeExpr = wrapper_formal->typeExpr->copy();
          for_alist(expr, typeExpr->body) {
            wrapper->insertAtTail(expr->remove());
          }

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
              if (isArrayAliasField) {
                wrapper->insertAtTail(new CallExpr(PRIM_MOVE, copyTemp, temp));
              } else {
                // MPF: I believe this autoCopy is problematic
                // adding FLAG_INSERT_AUTO_DESTROY doesn't cover it
                // because that flag is removed in cullForDefaultConstructor
                // The autoCopy started in commit
                //   3788ee34fa9f42bdce19e9e3cf46ccfbb1c60ac2
                wrapper->insertAtTail(new CallExpr(PRIM_MOVE, copyTemp, new CallExpr("chpl__autoCopy", temp)));
              }
              wrapper->insertAtTail(
                new CallExpr(PRIM_SET_MEMBER, wrapper->_this,
                             new_CStringSymbol(formal->name), copyTemp));
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

      // The formal was not supplied. We need to use a default value.

      const char* temp_name = astr("default_arg", formal->name);
      VarSymbol* temp = newTemp(temp_name);
      if (intent != INTENT_INOUT && intent != INTENT_OUT) {
        temp->addFlag(FLAG_MAYBE_PARAM);
        temp->addFlag(FLAG_EXPR_TEMP);
      }
      if (formal->hasFlag(FLAG_TYPE_VARIABLE))
        temp->addFlag(FLAG_TYPE_VARIABLE);
      copy_map.put(formal, temp);
      wrapper->insertAtTail(new DefExpr(temp));
      if (intent == INTENT_OUT ||
          !formal->defaultExpr ||
          (formal->defaultExpr->body.length == 1 &&
           isSymExpr(formal->defaultExpr->body.tail) &&
           toSymExpr(formal->defaultExpr->body.tail)->symbol() == gTypeDefaultToken)) {
        // use default value for type as default value for formal argument
        if (formal->typeExpr) {
          BlockStmt* typeExpr = formal->typeExpr->copy();
          for_alist(expr, typeExpr->body) {
            wrapper->insertAtTail(expr->remove());
          }
          Expr* lastExpr = wrapper->body->body.tail;
          if (formal->hasFlag(FLAG_TYPE_VARIABLE))
            wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, lastExpr->remove()));
          else {
            //
            // 2016-07-18: benharsh: I was encountering an issue where we were
            // attempting to wrap a function where we had inserted return temps
            // for chpl__buildArrayRuntimeType. This wrapping function then
            // created an invalid AST like this:
            //
            // (move call_tmp (move _return_tmp_ (call chpl__buildArrayRuntimeType ...)))
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
            if (lastCall != NULL && lastCall->isPrimitive(PRIM_MOVE)) {
              wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, new CallExpr(PRIM_INIT, lastCall->get(1)->copy())));
            } else {
              wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, new CallExpr(PRIM_INIT, lastExpr->remove())));
            }
          }
        } else {
          if (formal->hasFlag(FLAG_TYPE_VARIABLE))
            wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, new SymExpr(formal->type->symbol)));
          else
            wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, new CallExpr(PRIM_INIT, new SymExpr(formal->type->symbol))));
        }
      } else {
        // use argument default for the formal argument
        BlockStmt* defaultExpr = formal->defaultExpr->copy();
        for_alist(expr, defaultExpr->body) {
          wrapper->insertAtTail(expr->remove());
        }

        // Normally, addLocalCopiesAndWritebacks will handle
        // adding the copies. However, because of some issues with
        // default constructors, the copy is added here for them.
        // (In particular, the called constructor function does not
        //  include the necessary copies, because it would interfere
        //  with the array-domain link in
        //    record { var D={1..2}; var A:[D] int }
        //  )
        if (specializeDefaultConstructor) {
          // Copy construct from the default value.
          // Sometimes, normalize has already added an initCopy in the
          // defaultExpr. But if it didn't, we need to add a copy.
          Expr* fromExpr = wrapper->body->body.tail->remove();
          bool needsInitCopy = true;
          if (CallExpr* fromCall = toCallExpr(fromExpr)) {
            Expr* base = fromCall->baseExpr;
            if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(base)) {
              if (0 == strcmp(urse->unresolved, "chpl__initCopy") ||
                  0 == strcmp(urse->unresolved, "_createFieldDefault"))
                needsInitCopy = false;
            } else {
              INT_ASSERT(0); // if resolved, check for FLAG_INIT_COPY_FN
            }
          }
          if (needsInitCopy)
            fromExpr = new CallExpr("chpl__initCopy", fromExpr);

          wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, fromExpr));
        } else {
          // Otherwise, just pass it in
          if (intent & INTENT_FLAG_REF) {
            // For a ref intent argument, pass in address
            wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, new CallExpr(PRIM_ADDR_OF, wrapper->body->body.tail->remove())));
          } else {
            wrapper->insertAtTail(new CallExpr(PRIM_MOVE, temp, wrapper->body->body.tail->remove()));
          }
        }

        if (formal->intent == INTENT_INOUT) {
          INT_ASSERT(!temp->hasFlag(FLAG_EXPR_TEMP));
          temp->removeFlag(FLAG_MAYBE_PARAM);
        }
      }
      call->insertAtTail(temp);


      // MPF - this seems really strange since it is assigning to
      // fields that will be set in the construct call at the end.
      // It is handling the current issue that an iterator to
      // initialize an array can refer to the fields.
      // See arrayDomInClassRecord2.chpl.
      // In the future, it would probably be better to initialize the
      // fields in order in favor of calling the default constructor.
      if (specializeDefaultConstructor && strcmp(fn->name, "_construct__tuple"))
        if (!formal->hasFlag(FLAG_TYPE_VARIABLE))
          if (Symbol* field = wrapper->_this->type->getField(formal->name, false))
            if (field->defPoint->parentSymbol == wrapper->_this->type->symbol)
              wrapper->insertAtTail(
                new CallExpr(PRIM_SET_MEMBER, wrapper->_this,
                             new_CStringSymbol(formal->name), temp));

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

static IntentTag getIntent(ArgSymbol* formal)
{
  IntentTag intent = formal->intent;
  if ((intent == INTENT_BLANK || intent == INTENT_CONST) &&
      !formal->type->symbol->hasFlag(FLAG_ITERATOR_RECORD))
    intent = concreteIntentForArg(formal);
  return intent;
}

// do we need to add some coercion from the actual to the formal?
static bool needToAddCoercion(Type* actualType, Symbol* actualSym,
                              ArgSymbol* formal, FnSymbol* fn) {
  Type* formalType = formal->type;
  if (actualType == formalType)
    return false;
  // If we have an actual of ref(formalType) and
  // a REF or CONST REF argument intent, no coercion is necessary.
  else if(actualType == formalType->getRefType() &&
          (getIntent(formal) & INTENT_FLAG_REF) != 0)
    return false;
  else
    return canCoerce(actualType, actualSym, formalType, fn) ||
           isDispatchParent(actualType, formalType);
}

// Add a coercion; replace prevActual and actualSym - the actual to 'call' -
// with the result of the coercion.
static void addArgCoercion(FnSymbol*  fn,
                           CallExpr*  call,
                           ArgSymbol* formal,
                           Expr*&     actualExpr,
                           Symbol*&   actualSym,
                           bool&      checkAgain)
{
  SET_LINENO(actualExpr);

  Expr*       prevActual = actualExpr;
  TypeSymbol* ats        = actualSym->type->symbol;
  TypeSymbol* fts        = formal->type->symbol;
  CallExpr*   castCall   = NULL;
  VarSymbol*  castTemp   = newTemp("coerce_tmp"); // ..., formal->type ?

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

    if (SymExpr* prevSE = toSymExpr(prevActual))
      if (prevSE->symbol()->hasFlag(FLAG_REF_TO_CONST)) {
        castTemp->addFlag(FLAG_CONST);

        if (prevSE->symbol()->hasFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS))
          castTemp->addFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS);
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

    if (isString(fts))
      castTemp->addFlag(FLAG_INSERT_AUTO_DESTROY);
  }

  // move the result to the temp
  CallExpr* castMove = new CallExpr(PRIM_MOVE, castTemp, castCall);

  call->getStmtExpr()->insertBefore(new DefExpr(castTemp));
  call->getStmtExpr()->insertBefore(castMove);

  resolveCallAndCallee(castCall, true);
  if (FnSymbol* castTarget = castCall->isResolved()) {

    // Perhaps equivalently, we could check "if (tryToken)",
    // except tryToken is not visible in this file.
    if (!castTarget->hasFlag(FLAG_RESOLVED)) {
      // This happens e.g. when castTarget itself has an error.
      // Todo: in this case, we should report the error at the point
      // where it arises, supposedly within resolveFns(castTarget).
      // Why is it not reported there?
      USR_FATAL_CONT(call, "Error resolving a cast from %s to %s",
                     ats->name, fts->name);
      USR_PRINT(castTarget, "  the troublesome function is here");
      USR_STOP();
    }
  }

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
      if (needToAddCoercion(actualType, actualSym, formal, fn)) {
        if (formalType == dtStringC && actualType == dtString && actualSym->isImmediate()) {
          // We do this swap since we know the string is a valid literal
          // There also is no cast defined for string->c_string on purpose (you
          // need to use .c_str()) so the common case below does not work.
          VarSymbol *var = toVarSymbol(actualSym);
          SymExpr *newActual = new SymExpr(new_CStringSymbol(var->immediate->v_string));
          currActual->replace(newActual);
          currActual = newActual;
        } else {
          // addArgCoercion() updates currActual, actualSym, c2
          addArgCoercion(fn, info->call, formal, currActual, actualSym, c2);
        }
      }
    } while (c2 && --checksLeft > 0);

    INT_ASSERT(!c2); // otherwise need to increase checksLeft
    currActual = currActual->next;
  }
}  // coerceActuals()


////
//// promotion wrapper code
////

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
//    // _iteratorRecord has a field for each formal in the promoted function. e.g.
//    // `A + 2.0 * C` results in a record with fields for each array and the real
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
static void
buildPromotionFastFollowerCheck(bool isStatic,
                                bool addLead,
                                CallInfo* info,
                                FnSymbol* wrapper,
                                std::set<ArgSymbol*>& requiresPromotion) {
  const char* fnName = isStatic ? "chpl__staticFastFollowCheck" : "chpl__dynamicFastFollowCheck";
  const char* forwardFnName = astr(fnName, "Zip") ;

  FnSymbol* fastFollowCheckFn = new FnSymbol(fnName);
  if (isStatic) {
    fastFollowCheckFn->retTag = RET_PARAM;
  } else {
    fastFollowCheckFn->retTag = RET_VALUE;
  }

  ArgSymbol* x = new ArgSymbol(INTENT_BLANK, "x", dtIteratorRecord);
  fastFollowCheckFn->insertFormalAtTail(x);

  ArgSymbol* lead = new ArgSymbol(INTENT_BLANK, "lead", dtAny);
  if (addLead) {
    fastFollowCheckFn->insertFormalAtTail(lead);
  }


  CallExpr* buildTuple = new CallExpr("_build_tuple_always_allow_ref");
  for_formals(formal, wrapper) {
    if (requiresPromotion.count(formal) > 0) {
      Symbol* field = new VarSymbol(formal->name, formal->type);
      fastFollowCheckFn->insertAtTail(new DefExpr(field));
      fastFollowCheckFn->insertAtTail(new CallExpr(PRIM_MOVE, field, new CallExpr(PRIM_ITERATOR_RECORD_FIELD_VALUE_BY_FORMAL, x, formal)));
      buildTuple->insertAtTail(new SymExpr(field));
    }
  }
  fastFollowCheckFn->where = new BlockStmt(new CallExpr("==", new CallExpr(PRIM_TYPEOF, x), new CallExpr(PRIM_TYPEOF, info->call->copy())));

  Symbol* p_tup = newTemp("p_tup");
  fastFollowCheckFn->insertAtTail(new DefExpr(p_tup));
  fastFollowCheckFn->insertAtTail(new CallExpr(PRIM_MOVE, p_tup, buildTuple));

  Symbol* returnTmp = newTemp("p_ret");
  returnTmp->addFlag(FLAG_EXPR_TEMP);
  returnTmp->addFlag(FLAG_MAYBE_PARAM);
  fastFollowCheckFn->insertAtTail(new DefExpr(returnTmp));
  if (addLead) {
    fastFollowCheckFn->insertAtTail(new CallExpr(PRIM_MOVE, returnTmp, new CallExpr(forwardFnName, p_tup, lead)));
  } else {
    fastFollowCheckFn->insertAtTail(new CallExpr(PRIM_MOVE, returnTmp, new CallExpr(forwardFnName, p_tup)));
  }
  fastFollowCheckFn->insertAtTail(new CallExpr(PRIM_RETURN, returnTmp));

  theProgram->block->insertAtTail(new DefExpr(fastFollowCheckFn));
  normalize(fastFollowCheckFn);
  fastFollowCheckFn->addFlag(FLAG_GENERIC);
  fastFollowCheckFn->instantiationPoint = getVisibilityBlock(info->call);
}


static void fixUnresolvedSymExprsForPromotionWrapper(FnSymbol* wrapper, FnSymbol* fn) {
  // Fix the UnresolvedSymExprs we inserted to the actualCall. For each
  // call to `fn`, pick out any UnresolvedSymExprs and look in the loop
  // body for a corresponding DefExpr.

  std::vector<CallExpr*> calls;
  collectCallExprs(wrapper, calls);
  for_vector(CallExpr, call, calls) {
    if (call->isResolved() == fn) {
      for_actuals(actual, call) {
        if (UnresolvedSymExpr* unsym = toUnresolvedSymExpr(actual)) {
          // Get the StmtExpr in case 'call' returns something
          BlockStmt* callBlock = toBlockStmt(call->getStmtExpr()->parentExpr);
          INT_ASSERT(callBlock);
          BlockStmt* loop = toBlockStmt(callBlock->parentExpr);
          INT_ASSERT(loop && loop->isLoopStmt());
          std::vector<DefExpr*> defs;
          collectDefExprs(loop, defs);
          bool found = false;
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

static FnSymbol*
buildPromotionWrapper(FnSymbol* fn,
                      SymbolMap* promotion_subs,
                      CallInfo* info,
                      bool buildFastFollowerChecks) {
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
  std::set<ArgSymbol*> requiresPromotion;
  for_formals(formal, fn) {
    SET_LINENO(formal);
    ArgSymbol* new_formal = copyFormalForWrapper(formal);
    if (Symbol* p = paramMap.get(formal))
      paramMap.put(new_formal, p);
    if (fn->_this == formal)
      wrapper->_this = new_formal;
    if (Symbol* sub = promotion_subs->get(formal)) {
      requiresPromotion.insert(new_formal);
      TypeSymbol* ts = toTypeSymbol(sub);
      if (!ts)
        INT_FATAL(fn, "error building promotion wrapper");
      new_formal->type = ts->type;
      wrapper->insertFormalAtTail(new_formal);
      iteratorCall->insertAtTail(new_formal);

      // Rely on the 'destructureIndices' function in build.cpp to create a
      // VarSymbol and DefExpr for these indices. This solves a problem where
      // these 'p_i_' variables were declared outside of the loop body's scope.
      const char* name = astr("p_i_", istr(i));
      indicesCall->insertAtTail(new UnresolvedSymExpr(name));
      actualCall->insertAtTail(new UnresolvedSymExpr(name));
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


    // Build up the leader iterator
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
    lifn->addFlag(FLAG_GENERIC);
    lifn->instantiationPoint = getVisibilityBlock(info->call);


    // Build up the follower iterator
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
    ArgSymbol* fastFollower = new ArgSymbol(INTENT_PARAM, "fast", dtBool, NULL, new SymExpr(gFalse));
    fifn->insertFormalAtTail(fastFollower);
    fifn->where = new BlockStmt(new CallExpr("==", fifnTag, gFollowerTag));
    VarSymbol* followerIterator = newTemp("p_followerIterator");
    followerIterator->addFlag(FLAG_EXPR_TEMP);
    fifn->insertAtTail(new DefExpr(followerIterator));

    if( !zippered ) {
      fifn->insertAtTail(new CondStmt(new SymExpr(fastFollower),
                         new CallExpr(PRIM_MOVE, followerIterator, new CallExpr("_toFastFollower", iterator->copy(&followerMap), fifnFollower)),
                         new CallExpr(PRIM_MOVE, followerIterator, new CallExpr("_toFollower", iterator->copy(&followerMap), fifnFollower))));
    } else {
      fifn->insertAtTail(new CondStmt(new SymExpr(fastFollower),
                         new CallExpr(PRIM_MOVE, followerIterator, new CallExpr("_toFastFollowerZip", iterator->copy(&followerMap), fifnFollower)),
                         new CallExpr(PRIM_MOVE, followerIterator, new CallExpr("_toFollowerZip", iterator->copy(&followerMap), fifnFollower))));
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
    fixUnresolvedSymExprsForPromotionWrapper(fifn, fn);

    if (!fNoFastFollowers && buildFastFollowerChecks) {
      // Build up the static (param) fast follower check functions
      buildPromotionFastFollowerCheck(/*isStatic=*/true,  /*addLead=*/false, info, wrapper, requiresPromotion);
      buildPromotionFastFollowerCheck(/*isStatic=*/true,  /*addLead=*/true,  info, wrapper, requiresPromotion);

      // Build up the dynamic fast follower check functions
      buildPromotionFastFollowerCheck(/*isStatic=*/false, /*addLead=*/false, info, wrapper, requiresPromotion);
      buildPromotionFastFollowerCheck(/*isStatic=*/false, /*addLead=*/true,  info, wrapper, requiresPromotion);
    }


    // Finish building the serial iterator. We stopped mid-way so the common
    // code could be copied for the leader/follower
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
  fixUnresolvedSymExprsForPromotionWrapper(wrapper, fn);

  return wrapper;
}


FnSymbol*
promotionWrap(FnSymbol* fn, CallInfo* info, bool buildFastFollowerChecks) {

  Vec<Symbol*>* actuals = &info->actuals;
  if (!strcmp(fn->name, "="))
    return fn;
  // Don't try to promotion wrap _ref type constructor
  if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR))
    return fn;
  bool promotion_wrapper_required = false;
  SymbolMap promoted_subs;
  int j = -1;
  for_formals(formal, fn) {
    j++;
    Type* actualType = actuals->v[j]->type;
    Symbol* actualSym = actuals->v[j];

    if (isRecordWrappedType(actualType)) {
      makeRefType(actualType);
      actualType = actualType->refType;
      INT_ASSERT(actualType);
    }

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
      wrapper = buildPromotionWrapper(fn, &promoted_subs, info, buildFastFollowerChecks);
      addCache(promotionsCache, fn, wrapper, &promoted_subs);
    }
    resolveFormals(wrapper);
    return wrapper;
  }
  return fn;
}
