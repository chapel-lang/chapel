#include "astutil.h"
#include "build.h"
#include "caches.h"
#include "callInfo.h"
#include "chpl.h"
#include "expr.h"
#include "resolution.h"
#include "stmt.h"
#include "symbol.h"


static FnSymbol*
buildEmptyWrapper(FnSymbol* fn) {
  FnSymbol* wrapper = new FnSymbol(fn->name);
  wrapper->isWrapper = true;
  wrapper->visible = false;
  wrapper->addPragma(PRAG_INLINE);
  if (fn->hasPragma(PRAG_NO_PARENS))
    wrapper->addPragma(PRAG_NO_PARENS);
  if (fn->fnTag != FN_ITERATOR) { // getValue is var, not iterator
    wrapper->retTag = fn->retTag;
    if (fn->setter)
      wrapper->setter = fn->setter->copy();
  }
  wrapper->isMethod = fn->isMethod;
  wrapper->instantiationPoint = fn->instantiationPoint;
  wrapper->isCompilerTemp = true;
  return wrapper;
}


//
// return true if formal matches name of a subsequent formal
//
static bool
isShadowedField(ArgSymbol* formal) {
  DefExpr* tmp = toDefExpr(formal->defPoint->next);
  while (tmp) {
    if (!strcmp(tmp->sym->name, formal->name))
      return true;
    tmp = toDefExpr(tmp->next);
  }
  return false;
}


static void
insertWrappedCall(FnSymbol* fn, FnSymbol* wrapper, CallExpr* call) {
  if (fn->getReturnSymbol() == gVoid) {
    wrapper->insertAtTail(call);
  } else if (fn->fnTag != FN_ITERATOR) {
    wrapper->insertAtTail(new CallExpr(PRIMITIVE_RETURN, call));
  } else {
    VarSymbol* index = new VarSymbol("_i");
    index->isCompilerTemp = true;
    wrapper->insertAtTail(new DefExpr(index));
    wrapper->insertAtTail(buildForLoopStmt(new SymExpr(index), call, new BlockStmt(new CallExpr(PRIMITIVE_YIELD, index))));
    wrapper->fnTag = FN_ITERATOR;
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
                    bool isSquare) {
  if (FnSymbol* cached = checkCache(defaultsCache, fn, defaults))
    return cached;
  SET_LINENO(fn);
  FnSymbol* wrapper = buildEmptyWrapper(fn);
  if (fn->fnTag != FN_ITERATOR)
    wrapper->retType = fn->retType;
  wrapper->cname = astr("_default_wrap_", fn->cname);
  SymbolMap copy_map;
  bool specializeDefaultConstructor =
    fn->hasPragma(PRAG_DEFAULT_CONSTRUCTOR) &&
    !fn->_this->type->symbol->hasPragma(PRAG_SYNC) &&
    !fn->_this->type->symbol->hasPragma(PRAG_REF);
  if (specializeDefaultConstructor) {
    wrapper->isCompilerTemp = false;
    wrapper->_this = fn->_this->copy();
    copy_map.put(fn->_this, wrapper->_this);
    wrapper->insertAtTail(new DefExpr(wrapper->_this));
    if (defaults->v[defaults->n-1]->hasPragma(PRAG_IS_MEME)) {
      if (!isRecordType(fn->_this->type) && !isUnionType(fn->_this->type)) {
        wrapper->insertAtTail(new CallExpr(PRIMITIVE_MOVE, wrapper->_this,
                                new CallExpr(PRIMITIVE_CHPL_ALLOC, wrapper->_this,
                                new_StringSymbol(astr("instance of class ",
                                                      fn->_this->type->symbol->name)))));
        wrapper->insertAtTail(new CallExpr(PRIMITIVE_SETCID, wrapper->_this));
      }
    }
    wrapper->insertAtTail(new CallExpr(PRIMITIVE_INIT_FIELDS, wrapper->_this));
  }
  CallExpr* call = new CallExpr(fn);
  call->square = isSquare;
  for_formals(formal, fn) {
    SET_LINENO(formal);
    if (!defaults->in(formal)) {
      ArgSymbol* wrapper_formal = formal->copy();
      if (fn->_this == formal)
        wrapper->_this = wrapper_formal;
      if (formal->hasPragma(PRAG_IS_MEME))
        wrapper->_this->defPoint->insertAfter(new CallExpr(PRIMITIVE_MOVE, wrapper->_this, wrapper_formal)); // unexecuted none/gasnet on 4/25/08
      wrapper->insertFormalAtTail(wrapper_formal);
      Symbol* temp;
      if (formal->type->symbol->hasPragma(PRAG_REF)) {
        temp = new VarSymbol("_tmp");
        temp->isCompilerTemp = true;
        temp->canParam = true;
        if (formal->isTypeVariable)
          temp->isTypeVariable = true; // unexecuted none/gasnet on 4/25/08
        wrapper->insertAtTail(new DefExpr(temp));
        wrapper->insertAtTail(new CallExpr(PRIMITIVE_MOVE, temp, new CallExpr(PRIMITIVE_SET_REF, wrapper_formal)));
      } else if (specializeDefaultConstructor && wrapper_formal->typeExpr) {
        temp = new VarSymbol("_tmp");
        temp->isCompilerTemp = true;
        temp->canParam = true;
        if (formal->isTypeVariable)
          temp->isTypeVariable = true; // unexecuted none/gasnet on 4/25/08
        wrapper->insertAtTail(new DefExpr(temp));
        BlockStmt* typeExpr = wrapper_formal->typeExpr->copy();
        wrapper->insertAtTail(typeExpr);
        wrapper->insertAtTail(new CallExpr(PRIMITIVE_MOVE, temp, new CallExpr("_createFieldDefault", typeExpr->body.tail->remove(), wrapper_formal)));
      } else
        temp = wrapper_formal;
      copy_map.put(formal, temp);
      call->insertAtTail(temp);
      if (Symbol* value = paramMap->get(formal))
        paramMap->put(wrapper_formal, value);
      if (specializeDefaultConstructor && strcmp(fn->name, "_construct__tuple"))
        if (!formal->isTypeVariable && !paramMap->get(formal) && formal->type != dtMethodToken)
          if (Symbol* field = wrapper->_this->type->getField(formal->name))
            if (field->defPoint->parentSymbol == wrapper->_this->type->symbol)
              if (!isShadowedField(formal))
                wrapper->insertAtTail(
                  new CallExpr(PRIMITIVE_SET_MEMBER, wrapper->_this,
                               new_StringSymbol(formal->name), temp));
    } else if (paramMap->get(formal)) {
      // handle instantiated param formals
      call->insertAtTail(paramMap->get(formal));
    } else if (formal->hasPragma(PRAG_IS_MEME)) {

      //
      // hack: why is the type of meme set to dtNil?
      //
      formal->type = wrapper->_this->type;

      call->insertAtTail(wrapper->_this);
    } else {
      const char* temp_name = astr("_default_temp_", formal->name);
      VarSymbol* temp = new VarSymbol(temp_name);
      temp->isCompilerTemp = true;
      temp->canParam = true;
      if (formal->isTypeVariable)
        temp->isTypeVariable = true;
      copy_map.put(formal, temp);
      if (formal->intent == INTENT_OUT ||
          !formal->defaultExpr ||
          (formal->defaultExpr->body.length() == 1 &&
           isSymExpr(formal->defaultExpr->body.tail) &&
           toSymExpr(formal->defaultExpr->body.tail)->var == gNil)) {
        // use default value for type as default value for formal argument
        if (formal->typeExpr) {
          BlockStmt* typeExpr = formal->typeExpr->copy();
          wrapper->insertAtTail(typeExpr);
          wrapper->insertAtTail(new DefExpr(temp, NULL, typeExpr->body.tail->remove()));
        } else {
          wrapper->insertAtTail(new DefExpr(temp, NULL, new SymExpr(formal->type->symbol)));
        }
      } else {
        wrapper->insertAtTail(new DefExpr(temp));
        BlockStmt* defaultExpr = formal->defaultExpr->copy();
        wrapper->insertAtTail(defaultExpr);
        if (formal->intent != INTENT_INOUT) {
          wrapper->insertAtTail(new CallExpr(PRIMITIVE_MOVE, temp, defaultExpr->body.tail->remove()));
        } else {
          wrapper->insertAtTail(new CallExpr(PRIMITIVE_MOVE, temp, new CallExpr("_copy", defaultExpr->body.tail->remove())));
          temp->isExprTemp = false;
          temp->canParam = false;
        }
      }
      call->insertAtTail(temp);
      if (specializeDefaultConstructor && strcmp(fn->name, "_construct__tuple"))
        if (!formal->isTypeVariable)
          if (Symbol* field = wrapper->_this->type->getField(formal->name))
            if (field->defPoint->parentSymbol == wrapper->_this->type->symbol)
              if (!isShadowedField(formal))
                wrapper->insertAtTail(
                  new CallExpr(PRIMITIVE_SET_MEMBER, wrapper->_this,
                               new_StringSymbol(formal->name), temp));
    }
  }
  update_symbols(wrapper->body, &copy_map);

  insertWrappedCall(fn, wrapper, call);

  addCache(defaultsCache, fn, wrapper, defaults);
  normalize(wrapper);
  return wrapper;
}


FnSymbol*
defaultWrap(FnSymbol* fn,
            Vec<ArgSymbol*>* actualFormals,
            bool isSquare) {
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
    wrapper = buildDefaultWrapper(fn, &defaults, &paramMap, isSquare);

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
                  bool isSquare) {
  SET_LINENO(fn);
  FnSymbol* wrapper = buildEmptyWrapper(fn);
  wrapper->cname = astr("_order_wrap_", fn->cname);
  CallExpr* call = new CallExpr(fn);
  call->square = isSquare;
  SymbolMap copy_map;
  for_formals(formal, fn) {
    SET_LINENO(formal);
    Symbol* wrapper_formal = formal->copy();
    if (fn->_this == formal)
      wrapper->_this = wrapper_formal;
    copy_map.put(formal, wrapper_formal);
  }
  for_formals(formal, fn) {
    SET_LINENO(formal);
    wrapper->insertFormalAtTail(copy_map.get(order_map->get(formal)));
    call->insertAtTail(copy_map.get(formal));
  }
  insertWrappedCall(fn, wrapper, call);
  normalize(wrapper);
  return wrapper;
}


FnSymbol*
orderWrap(FnSymbol* fn,
          Vec<ArgSymbol*>* actualFormals,
          bool isSquare) {
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
    fn = buildOrderWrapper(fn, &formals_to_formals, isSquare);
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
                     bool isSquare) {
  // return cached if we already created this coercion wrapper
  if (FnSymbol* cached = checkCache(coercionsCache, fn, coercion_map))
    return cached;

  SET_LINENO(fn);
  FnSymbol* wrapper = buildEmptyWrapper(fn);

  //
  // stopgap: important for, for example, --no-local on
  // test/parallel/cobegin/bradc/varsEscape-workaround.chpl; when
  // function resolution is out-of-order, disabling this for
  // unspecified return types may not be necessary
  //
  if (fn->hasPragma(PRAG_SPECIFIED_RETURN_TYPE) && fn->fnTag != FN_ITERATOR)
    wrapper->retType = fn->retType;

  wrapper->cname = astr("_coerce_wrap_", fn->cname);
  CallExpr* call = new CallExpr(fn);
  call->square = isSquare;
  for_formals(formal, fn) {
    SET_LINENO(formal);
    Symbol* wrapperFormal = formal->copy();
    if (fn->_this == formal)
      wrapper->_this = wrapperFormal;
    wrapper->insertFormalAtTail(wrapperFormal);
    if (coercions->get(formal)) {
      TypeSymbol *ts = toTypeSymbol(coercion_map->get(formal));
      INT_ASSERT(ts);
      wrapperFormal->type = ts->type;
      if (ts->hasPragma(PRAG_SYNC)) {
        //
        // apply readFF or readFE to single or sync actual unless this
        // is a member access of the sync or single actual
        //
        if (fn->numFormals() >= 2 &&
            fn->getFormal(1)->type == dtMethodToken &&
            formal == fn->_this)
          call->insertAtTail(new CallExpr("value", gMethodToken, wrapperFormal));
        else if (ts->hasPragma(PRAG_SINGLE))
          call->insertAtTail(new CallExpr("readFF", gMethodToken, wrapperFormal));
        else
          call->insertAtTail(new CallExpr("readFE", gMethodToken, wrapperFormal));
      } else if (ts->hasPragma(PRAG_REF)) {
        //
        // dereference reference actual
        //
        call->insertAtTail(new CallExpr(PRIMITIVE_GET_REF, wrapperFormal));
      } else {
        call->insertAtTail(new CallExpr("_cast", formal->type->symbol, wrapperFormal));
      }
    } else {
      if (Symbol* actualTypeSymbol = coercion_map->get(formal))
        if (!(formal == fn->_this && fn->hasPragma(PRAG_REF_THIS)))
          wrapperFormal->type = actualTypeSymbol->type;
      call->insertAtTail(wrapperFormal);
    }
  }
  insertWrappedCall(fn, wrapper, call);
  normalize(wrapper);
  addCache(coercionsCache, fn, wrapper, coercion_map);
  return wrapper;
}


FnSymbol*
coercionWrap(FnSymbol* fn, CallInfo* info) {
  SymbolMap subs;
  Map<ArgSymbol*,bool> coercions;
  int j = -1;
  bool coerce = false;
  for_formals(formal, fn) {
    j++;
    Type* actualType = info->actuals.v[j]->type;
    Symbol* actualSym = info->actuals.v[j];
    if (actualType != formal->type) {
      if (canCoerce(actualType, actualSym, formal->type, fn) || isDispatchParent(actualType, formal->type)) {
        subs.put(formal, actualType->symbol);
        coercions.put(formal,true);
        coerce = true;
      } else {
        subs.put(formal, actualType->symbol);
      }
    }
  }
  if (coerce)
    fn = buildCoercionWrapper(fn, &subs, &coercions, info->call->square);
  return fn;  
}


////
//// promotion wrapper code
////


static FnSymbol*
buildPromotionWrapper(FnSymbol* fn,
                      SymbolMap* promotion_subs,
                      bool square) {
  // return cached if we already created this promotion wrapper
  SymbolMap map;
  Vec<Symbol*> keys;
  promotion_subs->get_keys(keys);
  forv_Vec(Symbol*, key, keys)
    map.put(key, promotion_subs->get(key));
  map.put(fn, (Symbol*)square); // add value of square to cache
  if (FnSymbol* cached = checkCache(promotionsCache, fn, &map))
    return cached;

  SET_LINENO(fn);
  FnSymbol* wrapper = buildEmptyWrapper(fn);
  wrapper->addPragma(PRAG_PROMOTION_WRAPPER);
  wrapper->cname = astr("_promotion_wrap_", fn->cname);
  CallExpr* indicesCall = new CallExpr("_build_tuple"); // destructured in build
  CallExpr* iterator = new CallExpr(square ? "_build_domain" : "_build_tuple");
  CallExpr* actualCall = new CallExpr(fn);
  int i = 1;
  for_formals(formal, fn) {
    SET_LINENO(formal);
    Symbol* new_formal = formal->copy();
    if (fn->_this == formal)
      wrapper->_this = new_formal;
    if (Symbol* sub = promotion_subs->get(formal)) {
      TypeSymbol* ts = toTypeSymbol(sub);
      if (!ts)
        INT_FATAL(fn, "error building promotion wrapper");
      new_formal->type = ts->type;
      wrapper->insertFormalAtTail(new_formal);
      iterator->insertAtTail(new_formal);
      VarSymbol* index = new VarSymbol(astr("_p_i_", istr(i)));
      index->isCompilerTemp = true;
      wrapper->insertAtTail(new DefExpr(index));
      indicesCall->insertAtTail(index);
      actualCall->insertAtTail(index);
    } else {
      wrapper->insertFormalAtTail(new_formal);
      actualCall->insertAtTail(new_formal);
    }
    i++;
  }

  Expr* indices = indicesCall;
  if (indicesCall->numActuals() == 1)
    indices = indicesCall->get(1)->remove();
  if (fn->getReturnSymbol() == gVoid) {
    wrapper->insertAtTail(new BlockStmt(buildForLoopStmt(indices, iterator, new BlockStmt(actualCall))));
  } else {
    wrapper->insertAtTail(new BlockStmt(buildForLoopStmt(indices, iterator, new BlockStmt(new CallExpr(PRIMITIVE_YIELD, actualCall)))));
    wrapper->fnTag = FN_ITERATOR;
    wrapper->removePragma(PRAG_INLINE);
  }
  fn->defPoint->insertBefore(new DefExpr(wrapper));
  normalize(wrapper);
  addCache(promotionsCache, fn, wrapper, &map);
  return wrapper;
}


FnSymbol*
promotionWrap(FnSymbol* fn, CallInfo* info) {
  Vec<Symbol*>* actuals = &info->actuals;
  bool isSquare = info->call->square;
  if (!strcmp(fn->name, "="))
    return fn;
  bool promotion_wrapper_required = false;
  SymbolMap promoted_subs;
  int j = -1;
  for_formals(formal, fn) {
    j++;
    Type* actualType = actuals->v[j]->type;
    Symbol* actualSym = actuals->v[j];
    bool require_scalar_promotion = false;
    if (canDispatch(actualType, actualSym, formal->type, fn, &require_scalar_promotion)){
      if (require_scalar_promotion) {
        promotion_wrapper_required = true;
        promoted_subs.put(formal, actualType->symbol);
      }
    }
  }
  if (promotion_wrapper_required) {
    if (fWarnPromotion)
      USR_WARN(info->call, "promotion on %s", toString(info));
    fn = buildPromotionWrapper(fn, &promoted_subs, isSquare);
    resolveFormals(fn);
  }
  return fn;
}
