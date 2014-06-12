// wrappers.cpp
////////////////////////////////////////////////////////////////////////////////{
// Wrappers are used to lower the Chapel idea of a function call to something
// implementable in C.
//  default wrapper -- supplies a value for every argument in the called function
//      substituting default values for actual arguments that are omitted.
//      (C does not support default values for arguments.)
//  order wrapper -- reorders named actual arguments to match the order expected
//      by the inner function.
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
buildOrderWrapper(FnSymbol* fn,
                  SymbolMap* order_map,
                  CallInfo* info);
static FnSymbol*
buildCoercionWrapper(FnSymbol* fn,
                     SymbolMap* coercion_map,
                     Map<ArgSymbol*,bool>* coercions,
                     CallInfo* info);
static FnSymbol*
buildPromotionWrapper(FnSymbol* fn,
                      SymbolMap* promotion_subs,
                      CallInfo* info);

//########################################################################


static FnSymbol*
buildEmptyWrapper(FnSymbol* fn, CallInfo* info) {
  FnSymbol* wrapper = new FnSymbol(fn->name);
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
  if (!fn->hasFlag(FLAG_ITERATOR_FN)) { // getValue is var, not iterator
    wrapper->retTag = fn->retTag;
    if (fn->setter)
      wrapper->setter = fn->setter->copy();
  }
  if (fn->hasFlag(FLAG_METHOD))
    wrapper->addFlag(FLAG_METHOD);
  if (fn->hasFlag(FLAG_ASSIGNOP))
    wrapper->addFlag(FLAG_ASSIGNOP);
  wrapper->instantiationPoint = getVisibilityBlock(info->call);
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
    wrapper->insertAtTail(new CallExpr(PRIM_INIT_FIELDS, wrapper->_this));
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
        if (Symbol* field = fn->_this->type->getField(formal->name, false))
          if (field->defPoint->parentSymbol == fn->_this->type->symbol)
            temp->addFlag(FLAG_INSERT_AUTO_DESTROY);
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
              wrapper->insertAtTail(new CallExpr(PRIM_MOVE, copyTemp, new CallExpr("chpl__autoCopy", temp)));
              wrapper->insertAtTail(
                new CallExpr(PRIM_SET_MEMBER, wrapper->_this,
                             new_StringSymbol(formal->name), copyTemp));
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

    // update actualFormals for use in orderWrap
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
//// order wrapper code
////


static FnSymbol*
buildOrderWrapper(FnSymbol* fn,
                  SymbolMap* order_map,
                  CallInfo* info) {
  SET_LINENO(fn);
  FnSymbol* wrapper = buildEmptyWrapper(fn, info);
  wrapper->cname = astr("_order_wrap_", fn->cname);
  CallExpr* call = new CallExpr(fn);
  call->square = info->call->square;
  SymbolMap copy_map;
  for_formals(formal, fn) {
    SET_LINENO(formal);
    ArgSymbol* wrapper_formal = copyFormalForWrapper(formal);
    if (fn->_this == formal)
      wrapper->_this = wrapper_formal;
    copy_map.put(formal, wrapper_formal);
  }
  for_formals(formal, fn) {
    SET_LINENO(formal);
    wrapper->insertFormalAtTail(copy_map.get(order_map->get(formal)));
    if (formal->hasFlag(FLAG_INSTANTIATED_PARAM))
      call->insertAtTail(paramMap.get(formal));
    else
      call->insertAtTail(copy_map.get(formal));
  }
  insertWrappedCall(fn, wrapper, call);
  normalize(wrapper);
  return wrapper;
}


FnSymbol*
orderWrap(FnSymbol* fn,
          Vec<ArgSymbol*>* actualFormals,
          CallInfo* info) {
  bool order_wrapper_required = false;
  SymbolMap formals_to_formals;
  int i = 0;
  for_formals(formal, fn) {
    i++;

    int j = 0;
    forv_Vec(ArgSymbol, af, *actualFormals) {
      j++;
      if (af == formal) {
        if (i != j)
          order_wrapper_required = true;
        formals_to_formals.put(formal, actualFormals->v[i-1]);
      }
    }
  }
  if (order_wrapper_required) {
    FnSymbol* wrapper = checkCache(ordersCache, fn, &formals_to_formals);
    if (wrapper == NULL) {
      wrapper = buildOrderWrapper(fn, &formals_to_formals, info);
      addCache(ordersCache, fn, wrapper, &formals_to_formals);
    }
    resolveFormals(wrapper);
    return wrapper;
  }
  return fn;
}


////
//// coercion wrapper code
////


static FnSymbol*
buildCoercionWrapper(FnSymbol* fn,
                     SymbolMap* coercion_map,
                     Map<ArgSymbol*,bool>* coercions,
                     CallInfo* info) {
  SET_LINENO(fn);
  FnSymbol* wrapper = buildEmptyWrapper(fn, info);

  //
  // stopgap: important for, for example, --no-local on
  // test/parallel/cobegin/bradc/varsEscape-workaround.chpl; when
  // function resolution is out-of-order, disabling this for
  // unspecified return types may not be necessary
  //
  if (fn->hasFlag(FLAG_SPECIFIED_RETURN_TYPE) && !fn->hasFlag(FLAG_ITERATOR_FN))
    wrapper->retType = fn->retType;

  wrapper->cname = astr("_coerce_wrap_", fn->cname);
  CallExpr* call = new CallExpr(fn);
  call->square = info->call->square;
  for_formals(formal, fn) {
    SET_LINENO(formal);
    ArgSymbol* wrapperFormal = copyFormalForWrapper(formal);
    if (fn->_this == formal)
      wrapper->_this = wrapperFormal;
    wrapper->insertFormalAtTail(wrapperFormal);
    if (coercions->get(formal)) {
      TypeSymbol *ts = toTypeSymbol(coercion_map->get(formal));
      INT_ASSERT(ts);
      wrapperFormal->type = ts->type;
      if (getSyncFlags(ts).any()) {
        //
        // apply readFF or readFE to single or sync actual unless this
        // is a member access of the sync or single actual
        //
        if (fn->numFormals() == 3 &&
            !strcmp(fn->name, "free"))
          // Special case: Don't insert a readFE or readFF when deleting a sync/single.
          call->insertAtTail(new CallExpr("_cast", formal->type->symbol, wrapperFormal));
        else if (fn->numFormals() >= 2 &&
            fn->getFormal(1)->type == dtMethodToken &&
            formal == fn->_this)
          call->insertAtTail(new CallExpr("value", gMethodToken, wrapperFormal));
        else if (ts->hasFlag(FLAG_SINGLE))
          call->insertAtTail(new CallExpr("readFF", gMethodToken, wrapperFormal));
        else if (ts->hasFlag(FLAG_SYNC))
          call->insertAtTail(new CallExpr("readFE", gMethodToken, wrapperFormal));
        else
          INT_ASSERT(false);    // Unhandled case.
      } else if (ts->hasFlag(FLAG_REF)) {
        //
        // dereference reference actual
        //
        call->insertAtTail(new CallExpr(PRIM_DEREF, wrapperFormal));
      } else if (wrapperFormal->hasFlag(FLAG_INSTANTIATED_PARAM)) {
        call->insertAtTail(new CallExpr("_cast", formal->type->symbol, paramMap.get(formal)));
        /*** } else if ((ts->type == dtStringC) && (formal->typeInfo() == dtString)) {
             call->insertAtTail(new CallExpr("_cast", formal->type->symbol, wrapperFormal)); ***/
        } else {
        call->insertAtTail(new CallExpr("_cast", formal->type->symbol, wrapperFormal));
      }
    } else {
      if (Symbol* actualTypeSymbol = coercion_map->get(formal))
        wrapperFormal->type = actualTypeSymbol->type;
      call->insertAtTail(wrapperFormal);
    }
  }
  insertWrappedCall(fn, wrapper, call);
  normalize(wrapper);
  return wrapper;
}


FnSymbol*
coercionWrap(FnSymbol* fn, CallInfo* info) {
  SymbolMap subs;
  Map<ArgSymbol*,bool> coercions;
  int j = -1;
  for_formals(formal, fn) {
    j++;
    Type* actualType = info->actuals.v[j]->type;
    Symbol* actualSym = info->actuals.v[j];
    if (actualType != formal->type) {
      if (canCoerce(actualType, actualSym, formal->type, fn) || isDispatchParent(actualType, formal->type)) {
        subs.put(formal, actualType->symbol);
        coercions.put(formal,true);
      } else {
        subs.put(formal, actualType->symbol);
      }
    }
  }

  if (coercions.n > 0) {  // Any coercions?
    FnSymbol* wrapper = checkCache(coercionsCache, fn, &subs);
    if (wrapper == NULL) {
      wrapper = buildCoercionWrapper(fn, &subs, &coercions, info);
      addCache(coercionsCache, fn, wrapper, &subs);
    }
    resolveFormals(wrapper);
    return wrapper;
  }

  return fn;  
}


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
      wrapper->insertAtTail(new BlockStmt(buildForallLoopStmt(indices, iterator, new BlockStmt(actualCall), zippered)));
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
    BlockStmt* loop = buildForLoopStmt(new SymExpr(leaderIndex), new SymExpr(leaderIterator), body, false, zippered);
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
    fifn->insertAtTail(buildForLoopStmt(indices->copy(&followerMap), new SymExpr(followerIterator), followerBlock, false, zippered));
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
    wrapper->insertAtTail(new BlockStmt(buildForLoopStmt(indices, iterator, yieldBlock, false, zippered)));
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
