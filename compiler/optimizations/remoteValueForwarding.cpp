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

#include "optimizations.h"

#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"

#include "global-ast-vecs.h"

//#define DEBUG_SYNC_ACCESS_FUNCTION_SET

static void updateLoopBodyClasses(Map<Symbol*, Vec<SymExpr*>*>& defMap,
                                  Map<Symbol*, Vec<SymExpr*>*>& useMap);

static void updateTaskFunctions(Map<Symbol*, Vec<SymExpr*>*>& defMap,
                                Map<Symbol*, Vec<SymExpr*>*>& useMap);

static void buildSyncAccessFunctionSet(Vec<FnSymbol*>& syncAccessFunctionSet);

static bool isSafeToDeref(Map<Symbol*, Vec<SymExpr*>*>& defMap,
                          Map<Symbol*, Vec<SymExpr*>*>& useMap,
                          Symbol*                       field,
                          Symbol*                       ref);

class DotInfo {
  public:
    bool finalized;
    bool usesDotLocale;
    std::vector<SymExpr*> todo;
    DotInfo();
};
DotInfo::DotInfo() : finalized(false), usesDotLocale(false) { }

static std::map<Symbol*, DotInfo*> dotLocaleMap;
typedef std::map<Symbol*, DotInfo*>::iterator DotInfoIter;

// deserializers for types that don't have any ref fields. This is used as a
// cache of functions that don't need to be analyzed in
// `handleRefDeserializers`.
static std::set<FnSymbol*> regularDeserializers;

static void computeUsesDotLocale();

/************************************* | **************************************
*                                                                             *
* Convert reference args into values if they are only read and reading them   *
* early does not violate program semantics.                                   *
*                                                                             *
* pre-condition: the call graph is computed                                   *
*                                                                             *
************************************** | *************************************/

void remoteValueForwarding() {

  if (fNoInferConstRefs == false)
    inferConstRefs();

  if (fNoRemoteValueForwarding == false && requireOutlinedOn()) {
    computeUsesDotLocale();
    Map<Symbol*, Vec<SymExpr*>*> defMap;
    Map<Symbol*, Vec<SymExpr*>*> useMap;

    buildDefUseMaps(defMap, useMap);

    updateLoopBodyClasses(defMap, useMap);
    updateTaskFunctions(defMap, useMap);

    freeDefUseMaps(defMap, useMap);

    for (DotInfoIter it = dotLocaleMap.begin(); it != dotLocaleMap.end(); ++it) {
      delete it->second;
    }
    dotLocaleMap.clear();
  }
}


/************************************* | **************************************
*                                                                             *
* Change reference type fields in loop body argument classes (created when    *
* transforming recursive leader iterators into recursive functions) to value  *
* type fields when it is safe to do so.                                       *
*                                                                             *
************************************** | *************************************/

static bool isSafeToDerefField(Map<Symbol*, Vec<SymExpr*>*>& defMap,
                               Map<Symbol*, Vec<SymExpr*>*>& useMap,
                               Symbol*                       field);

static void updateLoopBodyClasses(Map<Symbol*, Vec<SymExpr*>*>& defMap,
                                  Map<Symbol*, Vec<SymExpr*>*>& useMap) {
  forv_Vec(AggregateType, ct, gAggregateTypes) {
    if (ct->symbol->hasFlag(FLAG_LOOP_BODY_ARGUMENT_CLASS)) {
      for_fields(field, ct) {
        if (field->hasFlag(FLAG_REF_TO_IMMUTABLE)) {
          INT_ASSERT(field->isRef());
          if (isSafeToDerefField(defMap, useMap, field) == true) {
            Type* vt = field->getValType();

            for_uses(use, useMap, field) {
              CallExpr* call = toCallExpr(use->parentExpr);

              INT_ASSERT(call);
              SET_LINENO(call);

              if (call->isPrimitive(PRIM_SET_MEMBER)) {
                Symbol*   tmp   = newTemp(vt);
                Expr*     value = call->get(3)->remove();
                CallExpr* deref = new CallExpr(PRIM_DEREF, value);

                call->insertBefore(new DefExpr(tmp));
                call->insertBefore(new CallExpr(PRIM_MOVE, tmp, deref));

                call->insertAtTail(tmp);

              } else if (call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
                CallExpr* move  = toCallExpr(call->parentExpr);
                Symbol*   tmp   = newTemp(vt);
                Expr*     value = call->remove();

                INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));

                move->insertBefore(new DefExpr(tmp));
                move->insertBefore(new CallExpr(PRIM_MOVE, tmp, value));

                move->insertAtTail(new CallExpr(PRIM_SET_REFERENCE, tmp));

              } else {
                INT_FATAL(field, "unexpected case");
              }
            }

            field->type = vt;
            field->qual = QUAL_VAL;
          }
        }
      }
    }
  }
}

static bool isSafeToDerefField(Map<Symbol*, Vec<SymExpr*>*>& defMap,
                               Map<Symbol*, Vec<SymExpr*>*>& useMap,
                               Symbol*                       field) {
  bool retval = true;

  INT_ASSERT(!defMap.get(field));

  for_uses(use, useMap, field) {
    CallExpr* call = toCallExpr(use->parentExpr);

    INT_ASSERT(call);

    if (call->isPrimitive(PRIM_GET_MEMBER_VALUE) == true) {
      CallExpr* move = toCallExpr(call->parentExpr);
      SymExpr*  lhs  = toSymExpr(move->get(1));

      INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
      INT_ASSERT(lhs);

      if (isSafeToDeref(defMap, useMap, field, lhs->symbol()) == false) {
        retval = false;
        break;
      }

    } else if (call->isPrimitive(PRIM_SET_MEMBER) == false) {
      INT_FATAL(field, "unexpected case");
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool canForwardValue(Map<Symbol*, Vec<SymExpr*>*>& defMap,
                            Map<Symbol*, Vec<SymExpr*>*>& useMap,
                            Vec<FnSymbol*>&               syncFns,
                            FnSymbol*                     fn,
                            ArgSymbol*                    arg);

static bool isSufficientlyConst(ArgSymbol* arg);

static CallExpr* findDestroyCallForArg(ArgSymbol* arg);

static void defaultForwarding(Map<Symbol*, Vec<SymExpr*>*>& useMap,
                              FnSymbol*                     fn,
                              ArgSymbol*                    arg);
static void insertSerialization(FnSymbol*  fn,
                                ArgSymbol* arg);


static bool shouldSerialize(ArgSymbol* arg) {
  bool retval = false;
  Type* argType = arg->getValType();

  if (!argType->isSerializable()) {
    retval = false;
  } else if (isRecordWrappedType(argType)) {
    // OK to serialize if the record-wrapped type's underlying class is not
    // modified.
    //
    // A weird case is the record-wrapped type being passed across an on-stmt
    // in _do_destroy where it may eventually be deinit'd. We don't want to
    // serialize in that case because we want the original to be destroyed.
    //
    // BHARSH TODO: This seems a bit flimsy. If 'arg' is a reference to a
    // const domain (as written by the user), why doesn't it have the flag
    // FLAG_REF_TO_IMMUTABLE? That said, we don't seem to be leaking...
    retval = arg->intent == INTENT_CONST_REF && arg->hasFlag(FLAG_REF_TO_IMMUTABLE);
  } else {
    retval = true;
  }

  return retval;
}

static void updateTaskFunctions(Map<Symbol*, Vec<SymExpr*>*>& defMap,
                                Map<Symbol*, Vec<SymExpr*>*>& useMap) {
  Vec<FnSymbol*> syncSet;

  buildSyncAccessFunctionSet(syncSet);

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_ON) == true) {
      // Would need to flatten them if they are not already.
      INT_ASSERT(isGlobal(fn));

      // For each reference arg that is safe to dereference
      for_formals(arg, fn) {
        if (canForwardValue(defMap, useMap, syncSet, fn, arg)) {
          if (shouldSerialize(arg)) {
            insertSerialization(fn, arg);
          } else {
            defaultForwarding(useMap, fn, arg);
          }
        }
      }
    }
  }
}

static bool canForwardValue(Map<Symbol*, Vec<SymExpr*>*>& defMap,
                            Map<Symbol*, Vec<SymExpr*>*>& useMap,
                            Vec<FnSymbol*>&               syncFns,
                            FnSymbol*                     fn,
                            ArgSymbol*                    arg) {
  bool retval = false;

  if (arg->hasFlag(FLAG_NO_RVF)) {
    retval = false;
  } else if (arg->getValType()->symbol->hasFlag(FLAG_ALWAYS_RVF)) {
    retval = true;

  // Forward array values and references to array values.
  // This is OK because the array/domain/distribution wrapper
  // records have fields that do not vary.
  // It does not matter if the on-body synchronizes.
  //   It is the fields of the array class, e.g. DefaultRectangularArr,
  //   that may change. Also, the array class contains a pointer
  //   to the actual data, which might be replaced with another pointer.
  // An alternative strategy would be to migrate the contents of the
  // array header class into the wrapper record - but that would require
  // quite a lot of code changes, and some other features have entangled
  // designs (including privatization and the DSI interface).
  } else if (isRecordWrappedType(arg->getValType())) {
    // If it is passed by value already, forwarding would add nothing.
    retval = arg->isRef();

  // Similar arguments should hold for iterator records, as well.
  } else if (arg->getValType()->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
    retval = arg->isRef();

  // If this function accesses sync vars and the argument is not
  // const, then we cannot remote value forward the argument due
  // to the fence implied by the sync var accesses
  } else if (syncFns.set_in(fn) && isSufficientlyConst(arg) == false) {
    retval = false;

  // If this argument is a reference atomic type, we need to preserve
  // reference semantics, i.e. that the referenced atomic gets updated.
  // Therefore, dereferencing a ref atomic and forwarding its value is not
  // what we want.  That is, all atomics implicitly disable remote value
  // forwarding.
  // See resolveFormals() [functionResolution.cpp:839] for where we decide
  // to convert atomic formals to ref formals.
  } else if (isAtomicType(arg->type)) {
    retval = false;

  } else if (arg->isRef()) {
    // can forward if the actual is a QUAL_CONST_VAL or a ref-to-const
    DotInfo* info = dotLocaleMap[arg];
    if (info && info->usesDotLocale) {
      retval = false;
    } else if (arg->intent == INTENT_CONST_REF) {
      if (isClass(arg->getValType())) {
        // When passing a reference to a class across an on-statement, it will
        // normally be generated as a wide-reference to a wide-class. When the
        // reference is a const-ref, this introduces unnecessary communication
        // to simply get to the wide class pointer. Because the reference is
        // never written to, we can simply RVF the class pointer.
        retval = true;
      } else {
        retval = arg->hasFlag(FLAG_REF_TO_IMMUTABLE);
      }
    } else {
      retval = false;
    }
  } else {
    retval = false;
  }

  return retval;
}

static bool isSufficientlyConst(ArgSymbol* arg) {
  bool  retval     = false;

  //
  // See if this argument is 'const in'; if it is, it's a good candidate for
  // remote value forwarding.  My current thinking is that we should not
  // forward 'const ref' arguments because the const-ness only means that the
  // callee will not modify them, not that the caller won't.

  // If someone can successfully argue that I'm being too conservative, I'm
  // open to that.  My thinking is that I'd rather find a case that we think
  // we could be r.v.f.'ing later on than to have to chase down a race
  // condition due to optimizing too aggressively.
  //
  // Why the additional check against 'ref' types?  Because some
  // compiler-created arguments currently indicate ref-ness only via
  // the type and not the intent.  See the big comment I added in
  // addVarsToFormals() (flattenFunctions.cpp) in this same commit for
  // an example.  A case that currently fails without this test is:
  //
  //     test/multilocale/bradc/needMultiLocales/remoteReal.chpl
  //
  if (arg->intent == INTENT_CONST_IN  &&
      !arg->type->symbol->hasFlag(FLAG_REF)) {
    retval = true;

  } else if (arg->hasFlag(FLAG_REF_TO_IMMUTABLE)) {
    retval = true;

  // otherwise, conservatively assume it varies
  } else {
    retval = false;
  }

  return retval;
}

// Now that we changed the formal from ref to value,
// adjust its intent as well.
static void adjustArgIntentForDeref(ArgSymbol* arg) {
  INT_ASSERT(!arg->type->isRef());
  if (!(arg->intent & INTENT_FLAG_REF))
    return;

  arg->intent = (IntentTag)((arg->intent & ~INTENT_FLAG_REF) | INTENT_FLAG_IN);

  // We may get INTENT_REF_MAYBE_CONST
  // from flattenNestedFunction() during lowerIterators.
  // If combined with INTENT_FLAG_IN, it gives an invalid intent.
  // It is unclear whether the result should be const, so just remove it.
  arg->intent = (IntentTag)(arg->intent & ~INTENT_FLAG_MAYBE_CONST);

  // remove ref-specific flags
  arg->removeFlag(FLAG_REF_TO_IMMUTABLE);
  arg->removeFlag(FLAG_RETURN_SCOPE);
  arg->removeFlag(FLAG_SCOPE);
}

// Update each callsite to invoke the serializer.
static void serializeAtCallSites(FnSymbol* fn,  ArgSymbol* arg,
                       Type* dataType,          CallExpr* argDestroyCall,
                       FnSymbol* serializeFn,   bool newStyleInIntent)
{
  forv_Vec(CallExpr, call, *fn->calledBy) {
    SymExpr* actual = toSymExpr(formal_to_actual(call, arg));
    SET_LINENO(actual);

    Symbol* actualInput = actual->symbol();

    // If we're working with a copy added to support an 'in' intent,
    // we don't need that copy anymore since the serialize/deserialize
    // calls will have the same effect. So remove the copy call
    // in that event.
    if (newStyleInIntent) {
      Expr* initExpr = actual->symbol()->getInitialization();
      INT_ASSERT(initExpr);

      CallExpr* initCall = toCallExpr(initExpr);
      INT_ASSERT(initCall);

      if (initCall->isPrimitive(PRIM_MOVE) ||
          initCall->isPrimitive(PRIM_ASSIGN))
        initCall = toCallExpr(initCall->get(2));

      INT_ASSERT(initCall);

      FnSymbol* initFn = initCall->resolvedFunction();
      INT_ASSERT(initFn);
      INT_ASSERT(initFn->hasFlag(FLAG_INIT_COPY_FN) ||
                 initFn->hasFlag(FLAG_AUTO_COPY_FN));

      SymExpr* initArg = toSymExpr(initCall->get(1));
      INT_ASSERT(initArg);

      if (initArg->getValType() == actual->getValType()) {
        actualInput = initArg->symbol();
        initExpr->replace(new CallExpr(PRIM_MOVE, actual->symbol(), actualInput));
      }
    }

    VarSymbol* data = newTemp(astr(arg->cname, "_data"), dataType);
    if (arg->hasFlag(FLAG_COFORALL_INDEX_VAR)) {
      data->addFlag(FLAG_COFORALL_INDEX_VAR);
    }
    call->insertBefore(new DefExpr(data));

    if (serializeFn->hasFlag(FLAG_FN_RETARG)) {
      call->insertBefore(new CallExpr(serializeFn, actualInput, data));
    } else {
      call->insertBefore(new CallExpr(PRIM_MOVE, data, new CallExpr(serializeFn, actualInput)));
    }

    // Old argument not passed so we can't destroy the original
    // value in the task function anymore; destroy it just after
    // the serialize call.
    if (argDestroyCall) {
      FnSymbol* destroyFn = argDestroyCall->resolvedFunction();
      call->insertBefore(new CallExpr(destroyFn, actual->copy()));
    }

    actual->replace(new SymExpr(data));
  }
}

/*
 *  This function handles deserialization of AggregateTypes that have ref
 *  fields. Those fields need to point to data that is local for RVF to be
 *  meaningful. However, if the data is created inside the deserializer of that
 *  aggregate type, it'll be freed at the end of deserialization. Therefore, we
 *  hoist the deserialization of that data out of the main deserializer and into
 *  the `on` function. This is repeated recursively. The transformation is
 *  complicated and looks like the following for a single-ref-field record:
 *
 *  Before:
 *  -------
 *
 *  proc type chpl__deserialize1(data) {
 *    var ret: this;
 *    if <deserialization block marker> {
 *      var deserializedField = this.field.type.chpl__deserialize2(data[0]);
 *      this.field = deserializedField;
 *    }
 *    else {
 *      var deserializedField = this.field.type.chpl__deserialize2(data[0]);
 *      this.field = &deserializedField;
 *    }
 *    return ret;
 *  }
 *
 *  proc on_fn(... rvfData ...) {
 *    var deserialized = chpl__deserialize1(rvfData);
 *
 *    ...
 *
 *    autoDestroy(deserialized);
 *  }
 *
 *  After:
 *  ------
 *
 *  proc type chpl__deserialize1(data, newArgRef) {
 *    var ret: this;
 *    this.field = newArgRef;
 *    return ret;
 *  }
 *
 *  proc on_fn(... rvfData ...) {
 *    var deserializedField = this.field.type.chpl__deserialize2(rvfData[0]);
 *    ref newArgRef = &deserializedField;
 *    var deserialized = chpl__deserialize1(rvfData, newArgRef);
 *
 *    ...
 *
 *    autoDestroy(deserializedField);
 *    autoDestroy(deserialized);
 *  }
 *
 *  Roughly the changes are:
 *  - The thenStmt of the conditional is hoisted from inside chpl__deserialize1
 *    to the on_fn
 *  - The result of this sub-deserialization is passed by reference to the main
 *    deserializer.
 *  - This ref is directly assigned to the field inside the main deserializer.
 *
 */
static CallExpr* handleRefDeserializers(Expr* anchor, FnSymbol* fn,
                                        FnSymbol* baseDeserializeFn,
                                        Symbol* arg) {

  // if no changes are necessary, we'll just return this
  CallExpr* baseCall = new CallExpr(baseDeserializeFn, arg);

  if (regularDeserializers.count(baseDeserializeFn) == 1) {
    return baseCall;
  }

  FnSymbol* deserializeFn = baseDeserializeFn->copy();
  CallExpr* modifiedCall = new CallExpr(deserializeFn, arg);

  bool modified = false;
  for_alist (stmt, deserializeFn->body->body) {
    if (CondStmt* cond = toCondStmt(stmt)) {
      SymExpr* flagExpr = toSymExpr(cond->condExpr);
      if (flagExpr->symbol()->hasFlag(FLAG_DESERIALIZATION_BLOCK_MARKER)) {
        if (!modified) {
          modified = true;
          baseDeserializeFn->defPoint->insertBefore(new DefExpr(deserializeFn));
        }

        // Phase 1: figure out which field we are deserializing for
        bool useThenBlock = true;
        CallExpr* setMemberCall = NULL;
        Type* fieldType = NULL;
        for_alist_backward (innerStmt, cond->elseStmt->body) {
          if (CallExpr* call = toCallExpr(innerStmt)) {
            if (call->isPrimitive(PRIM_SET_MEMBER)) {
              setMemberCall = call;

              SymExpr* field = toSymExpr(call->get(2));
              INT_ASSERT(field);

              if (field->symbol()->isRef()) {
                useThenBlock = false;
                fieldType = field->symbol()->type;
                break;
              }
              else {
                useThenBlock = true;
                break;
              }
            }
          }
        }

        // Phase 2: Choose val or ref deserialization
        if (useThenBlock) {
          // Phase 2a: We are not doing ref deserialization for this
          // field. Put all the statements in the `then` block in before the
          // conditional. The conditional will be discarded later.
          for_alist (stmt, cond->thenStmt->body) {
            cond->insertBefore(stmt->remove());
          }
        }
        else {
          // Phase 2b: We are doing ref deserialization for this field. Find the
          // symbol used for the serial data. (It is typically replaced with a
          // formal temp). We will have to replace that symbol with the actual
          // argument, when we hoist the val serialization block.

          SymbolMap map;
          Symbol* partialData = NULL;  // part of the serial buffer used
          for_alist (innerStmt, cond->thenStmt->body) {
            if (CallExpr* call = toCallExpr(innerStmt)) {
              if (call->isPrimitive(PRIM_MOVE)) {
                if (CallExpr* rhs = toCallExpr(call->get(2))) {
                  if (rhs->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
                    if (SymExpr* lhs = toSymExpr(call->get(1))) {
                      partialData = lhs->symbol();
                      map.put(toSymExpr(rhs->get(1))->symbol(), arg);
                    }
                  }
                  if (rhs->isNamed("chpl__deserialize")) {
                    INT_ASSERT(partialData == toSymExpr(rhs->get(1))->symbol());
                  }
                }
              }
            }
          }

          // Phase 3: Actually hoist the val deserialization outside of this
          // deserializer. While doing so, update the serial buffer argument.
          BlockStmt* hoistedDeser = cond->thenStmt;
          anchor->insertBefore(hoistedDeser->remove());
          update_symbols(hoistedDeser, &map);

          // Phase 4: Replace the call that sets the field with a PRIM_ADDROF.
          // Later on, we will pass that address (`hoistedRefField`) as an
          // argument to the main deserializer.
          VarSymbol* hoistedRefField = NULL;
          CallExpr* nestedDeser = NULL;
          for_alist_backward (hoistedDeserStmt, hoistedDeser->body) {
            if (CallExpr* call = toCallExpr(hoistedDeserStmt)) {
              if (call->isPrimitive(PRIM_SET_MEMBER)) {
                hoistedRefField = new VarSymbol("hoisted_ref",
                                                fieldType->getRefType());

                // probably not necessary, but just in case:
                hoistedRefField->addFlag(FLAG_INSERT_AUTO_DESTROY);

                SymExpr* hoistedField = toSymExpr(call->get(3)->remove());

                call->insertBefore(new DefExpr(hoistedRefField));
                call->insertBefore(new CallExpr(PRIM_MOVE, hoistedRefField,
                                                new CallExpr(PRIM_ADDR_OF,
                                                             hoistedField)));
                call->remove();

                // destroy the val at the end of the `on_fn`
                if (FnSymbol* destroy = getAutoDestroy(hoistedField->getValType())) {
                  CallExpr* lastExpr = toCallExpr(fn->body->body.tail);
                  INT_ASSERT(lastExpr && lastExpr->isPrimitive(PRIM_RETURN));

                  lastExpr->insertBefore(new CallExpr(destroy,
                                                      hoistedField->copy()));
                }
              }
              else if (call->isNamed("chpl__autoDestroy")) {
                call->remove(); // we'll destroy in the outer scope (above)
              }
              else if (call->isNamed("chpl__deserialize")) {
                // if there is any nested deserializer in the deserializer that
                // we just hoisted, we need to recursively check for that.
                // However, this is not a very clean recursion. See below.
                nestedDeser = call;
              }
            }
          }

          // Phase 5: If we have nested deserializers, recurse.
          if (nestedDeser) {
            Symbol* argToNestedCall = NULL;

            // Find the serial data argument to the nested call:
            for_actuals (actual, nestedDeser) {
              SymExpr* actualSE = toSymExpr(actual);
              INT_ASSERT(actualSE);
              Symbol *sym = actualSE->symbol();

              if (isTypeSymbol(sym)) {
                continue;
              }
              else if (sym->type->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) {
                continue;
              }

              argToNestedCall = actualSE->symbol();
              break;
            }

            // find the first move in the hoisted block. This move must be
            // the def of the argument to the deserialize call. This is where we
            // have a meaningful symbol to pass to the nested deserializer when
            // we hoist it out. So, basically the statement after this move is
            // going to be our anchor.
            CallExpr* moveToArg = NULL;
            for_alist (stmt, hoistedDeser->body) {
              if (CallExpr* call = toCallExpr(stmt)) {
                if (call->isPrimitive(PRIM_MOVE)) {
                  SymExpr* lhs = toSymExpr(call->get(1));
                  INT_ASSERT(lhs->symbol() == argToNestedCall);
                  moveToArg = call;
                  break;
                }
              }
            }


            // recurse
            FnSymbol* curDeserializer = nestedDeser->resolvedFunction();
            CallExpr* replCall = handleRefDeserializers(moveToArg->next,
                                                        fn,
                                                        curDeserializer,
                                                        argToNestedCall);

            // handle retargs and rtts in the call. This is similar to the logic
            // in `insertSerialization`. There might be a refactor where we
            // don't have the same code again. But I don't think what that is is
            // obvious.
            if (curDeserializer->hasFlag(FLAG_FN_RETARG)) {
              INT_ASSERT(replCall->resolvedFunction()->hasFlag(FLAG_FN_RETARG));
              replCall->insertAtTail(nestedDeser->argList.tail->remove());
            }

            if (DefExpr* firstFormal = toDefExpr(curDeserializer->formals.head)) {
              if (firstFormal->sym->type->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) {
                replCall->insertAtHead(nestedDeser->argList.head->remove());
              }
            }
            nestedDeser->replace(replCall);
          }

          // We have bunch of stuff that we hoisted up, but it is still all in a
          // block. Flatten it.
          hoistedDeser->flattenAndRemove();

          // Phase 6: Adjust the existing "base" deserializer. This means adding
          // an argument to the function, changing the call to have the ref
          // field as an argument, changing the setMemberCall to make use of
          // this argument to the function. Note that `modifiedCall` will
          // receive its retarg/rtt in `insertSerialization`.
          ArgSymbol *newArg = new ArgSymbol(INTENT_REF, "hoisted_field",
                                            hoistedRefField->type);
          if (deserializeFn->hasFlag(FLAG_FN_RETARG)) {
            Expr* retArg = deserializeFn->formals.tail->remove();
            deserializeFn->insertFormalAtTail(newArg);
            deserializeFn->insertFormalAtTail(retArg);
          }
          else {
            deserializeFn->insertFormalAtTail(newArg);
          }
          modifiedCall->insertAtTail(new SymExpr(hoistedRefField));

          setMemberCall->get(3)->replace(new SymExpr(newArg));
          cond->insertBefore(setMemberCall->remove());

        }

        // cleanup
        flagExpr->symbol()->defPoint->remove();
        cond->remove();
      }
    }
  }

  if (!modified) {
    regularDeserializers.insert(baseDeserializeFn);
    return baseCall;
  }
  else {
    modifiedCall->setResolvedFunction(deserializeFn);
    return modifiedCall;
  }
}

// Insert and return the temp that will hold the deserialized
// instance of 'arg'.
// Replace all references to 'arg' within the task function
// with local references to that temp.
static VarSymbol* replaceArgWithDeserialized(FnSymbol* fn, ArgSymbol* arg,
                                Type* oldArgType, FnSymbol* deserializeFn,
                                bool needsRuntimeType)
{
  VarSymbol* deserialized = newTemp(arg->cname, oldArgType->getValType());
  VarSymbol* dsRef = newTemp(arg->cname, QualifiedType(QUAL_REF, oldArgType->getValType()));
  for_SymbolSymExprs(se, arg) {
    se->setSymbol(dsRef);
  }

  Expr* anchor = fn->body->body.head;
  anchor->insertBefore(new DefExpr(deserialized));
  anchor->insertBefore(new DefExpr(dsRef));

  // TODO we should probably create the call as normal, then replace it. It'll
  // help with easier recursion w.r.t retarg functions
  CallExpr* deserializeCall = handleRefDeserializers(anchor, fn, deserializeFn,
                                                     arg);
  CallExpr* callToAdd = NULL;

  if (needsRuntimeType) {
    FnSymbol* runtimeTypeFn = valueToRuntimeTypeMap.get(oldArgType->getValType());
    INT_ASSERT(runtimeTypeFn != NULL);
    VarSymbol* info = new VarSymbol("ds_info", runtimeTypeFn->retType);
    anchor->insertBefore(new DefExpr(info));

    // Add 'this' actual
    deserializeCall->insertAtHead(new SymExpr(info));
  }

  if (deserializeFn->hasFlag(FLAG_FN_RETARG)) {
    VarSymbol* refTemp = newTemp(deserialized->qualType().toRef());
    anchor->insertBefore(new DefExpr(refTemp));;
    anchor->insertBefore(new CallExpr(PRIM_MOVE, refTemp, new CallExpr(PRIM_SET_REFERENCE, deserialized)));
    deserializeCall->insertAtTail(new SymExpr(refTemp));
    callToAdd = deserializeCall;
  } else {
    callToAdd = new CallExpr(PRIM_MOVE, deserialized, deserializeCall);
  }
  anchor->insertBefore(callToAdd);
  anchor->insertBefore(new CallExpr(PRIM_MOVE, dsRef, new CallExpr(PRIM_SET_REFERENCE, deserialized)));

  return deserialized;
}

// Destroy 'arg' and 'deserialized' before returning from the task function.
static void destroyArgAndDeserialized(FnSymbol* fn, ArgSymbol* arg,
                             bool newStyleInIntent, VarSymbol* deserialized)
{
  CallExpr* lastExpr = toCallExpr(fn->body->body.tail);
  INT_ASSERT(lastExpr && lastExpr->isPrimitive(PRIM_RETURN));

  FnSymbol* dataDestroyFn = getAutoDestroy(arg->getValType());
  if (dataDestroyFn != NULL) {
    lastExpr->insertBefore(new CallExpr(dataDestroyFn, arg));
  }

  if (!newStyleInIntent) {
    FnSymbol* deserializeDestroyFn = getAutoDestroy(deserialized->getValType());
    if (deserializeDestroyFn != NULL) {
      lastExpr->insertBefore(new CallExpr(deserializeDestroyFn, deserialized));
    }
  }
}

//
// BHARSH 2017-08-21:
//
// This function inserts calls to chpl__serialize and chpl__deserialize.
// For example, AST like this:
//
//   call on_fn tmp myRecord;
//
//   function on_fn(const in dummy_locale_arg, const ref myRecord : Foo) {
//     call writeln myRecord;
//   }
//
// Will be transformed to AST like this:
//
//   var myRecord_data : 3*int;
//   call chpl__serialize myRecord myRecord_data;
//   call on_fn tmp myRecord_data
//
//   function on_fn(const in dummy_locale_arg, const myRecord_data : 3*int) {
//     var myRecord : Foo;
//     call chpl__deserialize myRecord_data myRecord;
//     call writeln myRecord;
//     call chpl__autoDestroy myRecord_data;
//     call chpl__autoDestroy myRecord;
//   }
//
// If we're RVF-ing something with a runtime type, this function copies the
// original formal and uses it to construct a runtime type within the
// on-statement. This will change the number of formals for this on-statement
// unlike traditional RVF.
//
// BHARSH TODO: Split this function into better easily-digestible pieces
// BHARSH TODO: capture the assumptions made here in documentation
//
static void insertSerialization(FnSymbol*  fn,
                                ArgSymbol* arg) {
  Type* oldArgType    = arg->type;
  bool newStyleInIntent = shouldAddInFormalTempAtCallSite(arg, fn);

  Serializers ser = serializeMap[oldArgType->getValType()];

  FnSymbol* serializeFn   = ser.serializer;
  FnSymbol* deserializeFn = ser.deserializer;
  INT_ASSERT(serializeFn != NULL && deserializeFn != NULL);

  bool needsRuntimeType = oldArgType->getValType()->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE);

  Type* dataType = NULL;
  if (serializeFn->hasFlag(FLAG_FN_RETARG)) {
    ArgSymbol* retArg = toArgSymbol(toDefExpr(serializeFn->formals.tail)->sym);
    INT_ASSERT(retArg && retArg->hasFlag(FLAG_RETARG));

    dataType = retArg->getValType();
  } else {
    dataType = serializeFn->retType;
  }
  arg->type = dataType;
  adjustArgIntentForDeref(arg);

  // If argDestroyCall is set, we'll move the destroy
  // call from the task function to the call sites.
  CallExpr* argDestroyCall = findDestroyCallForArg(arg);

  // Current compiler always moves the auto-destroy
  // for a coforall index variable into the task function
  // These asserts just remind us to check this code if that changes.
  if (arg->hasFlag(FLAG_COFORALL_INDEX_VAR) &&
      getAutoDestroy(arg->getValType()))
    INT_ASSERT(argDestroyCall);
  else
    INT_ASSERT(!argDestroyCall);

  serializeAtCallSites(fn, arg, dataType, argDestroyCall, serializeFn,
                       newStyleInIntent);

  // Remove the old auto-destroy call from the task fn.
  if (argDestroyCall) {
    argDestroyCall->remove();
  }

  SET_LINENO(fn);

  // The deserialized instance.
  VarSymbol* deserialized = replaceArgWithDeserialized(fn, arg, oldArgType,
                                             deserializeFn, needsRuntimeType);

  destroyArgAndDeserialized(fn, arg, newStyleInIntent, deserialized);
}

static CallExpr* findDestroyCallForArg(ArgSymbol* arg) {
  // Scroll backwards looking for an auto-destroy call for arg
  // in the function defining arg.

  FnSymbol* fn = arg->getFunction();
  for (Expr* stmt = fn->body->body.tail;
       stmt != NULL;
       stmt = stmt->prev) {

    // Look for a CallExpr to auto destroy fn with argument arg
    if (CallExpr* call = toCallExpr(stmt))
      if (FnSymbol* calledFn = call->resolvedFunction())
        if (calledFn->hasFlag(FLAG_AUTO_DESTROY_FN))
          if (SymExpr* se = toSymExpr(call->get(1)))
            if (se->symbol() == arg)
              return call;
  }

  return NULL;
}

static void defaultForwarding(Map<Symbol*, Vec<SymExpr*>*>& useMap,
                              FnSymbol*                     fn,
                              ArgSymbol*                    arg) {
  // Dereference the arg type.
  Type* prevArgType = arg->type;

  arg->type = arg->getValType();
  adjustArgIntentForDeref(arg);

  forv_Vec(CallExpr, call, *fn->calledBy) {
    // Find actual for arg.
    SymExpr* actual = toSymExpr(formal_to_actual(call, arg));

    INT_ASSERT(actual && actual->getValType() == prevArgType->getValType());
    SET_LINENO(actual);

    // Insert de-reference temp of value.
    VarSymbol* deref = newTemp("rvfDerefTmp", arg->type);
    if (arg->hasFlag(FLAG_COFORALL_INDEX_VAR)) {
      deref->addFlag(FLAG_COFORALL_INDEX_VAR);
    }

    Expr* rhs = NULL;
    if (actual->isRef()) {
      rhs = new CallExpr(PRIM_DEREF, new SymExpr(actual->symbol()));
    } else {
      rhs = new SymExpr(actual->symbol());
    }

    call->insertBefore(new DefExpr(deref));
    call->insertBefore(new CallExpr(PRIM_MOVE,
                                    deref,
                                    rhs));

    actual->replace(new SymExpr(deref));
  }

  // Insert re-reference temps at use points.
  for_uses(use, useMap, arg) {
    SET_LINENO(use);

    CallExpr* call = toCallExpr(use->parentExpr);
    if (!call) continue;

    if (call && call->isPrimitive(PRIM_DEREF)) {
      call->replace(new SymExpr(arg));

    } else if (call && isDerefMove(call)) {
      use->replace(new SymExpr(arg));
    } else if (call && call->isPrimitive(PRIM_MOVE) && call->get(1)->isRef()) {
      use->replace(new CallExpr(PRIM_SET_REFERENCE, arg));

    } else {
      Expr*      stmt   = use->getStmtExpr();
      VarSymbol* reref = newTemp("rvfRerefTmp", prevArgType);

      Expr* rhs = NULL;
      if (reref->isRef()) {
        rhs = new CallExpr(PRIM_SET_REFERENCE, arg);
      } else {
        rhs = new SymExpr(arg);
      }

      stmt->insertBefore(new DefExpr(reref));
      stmt->insertBefore(new CallExpr(PRIM_MOVE,
                                      reref,
                                      rhs));

      use->replace(new SymExpr(reref));
    }
  }
}

static bool isSyncSingleMethod(FnSymbol* fn) {

  bool retval = false;

  if (fn->_this != NULL) {
    Type* valType = fn->_this->getValType();

    if  (isSyncType(valType)   == true ||
         isSingleType(valType) == true) {
      retval = true;
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* Compute set of functions that access sync variables.                        *
*                                                                             *
************************************** | *************************************/

static void buildSyncAccessFunctionSet(Vec<FnSymbol*>& syncAccessFunctionSet) {
  Vec<FnSymbol*> syncAccessFunctionVec;

  //
  // Find all methods on sync/single vars
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (isSyncSingleMethod(fn)) {
      if (!fn->hasFlag(FLAG_DONT_DISABLE_REMOTE_VALUE_FORWARDING) &&
          !syncAccessFunctionSet.set_in(fn)) {
        syncAccessFunctionSet.set_add(fn);
        syncAccessFunctionVec.add(fn);
#ifdef DEBUG_SYNC_ACCESS_FUNCTION_SET
        printf("%s:%d %s\n",
               fn->getModule()->name,
               fn->linenum(),
               fn->name);
#endif
      }
    }
  }

  //
  // Find all functions that indirectly call methods on sync/single vars. Note
  // that syncAccessFunctionSet is just used for fast membership check, while
  // syncAccessFunctionVec is trickily appended to while iterating over it so
  // that we look at callsites of newly discovered functions.
  //
  forv_Vec(FnSymbol, fn, syncAccessFunctionVec) {
    forv_Vec(CallExpr, caller, *fn->calledBy) {
      FnSymbol* parent = toFnSymbol(caller->parentSymbol);
      INT_ASSERT(parent);

      if (!parent->hasFlag(FLAG_DONT_DISABLE_REMOTE_VALUE_FORWARDING) &&
          !syncAccessFunctionSet.set_in(parent)) {

        syncAccessFunctionSet.set_add(parent);
        syncAccessFunctionVec.add(parent);

#ifdef DEBUG_SYNC_ACCESS_FUNCTION_SET
        printf("%s:%d %s\n",
               parent->getModule()->name,
               parent->linenum(),
               parent->name);

        printf("  %s:%d %s\n",
               fn->getModule()->name,
               fn->linenum(),
               fn->name);
#endif
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Return true IFF it is safe to dereference a reference arg.                  *
*                                                                             *
* It is safe to dereference IFF the reference is not modified and any use of  *
* the reference is a simple dereference or is passed or moved to another      *
* reference that is safe to dereference.                                      *
*                                                                             *
* The argument <field> is used to ignore SET_MEMBER when testing whether a    *
* a reference field can be replaced with a value; this handles the case where *
* the reference field is reassigned to itself (probably of another instance)  *
*                                                                             *
************************************** | *************************************/

static bool isSafeToDeref(Map<Symbol*, Vec<SymExpr*>*>& defMap,
                          Map<Symbol*, Vec<SymExpr*>*>& useMap,
                          Symbol*                       field,
                          Symbol*                       ref,
                          Vec<Symbol*>&                 visited);

static bool isSafeToDeref(Map<Symbol*, Vec<SymExpr*>*>& defMap,
                          Map<Symbol*, Vec<SymExpr*>*>& useMap,
                          Symbol*                       field,
                          Symbol*                       ref,
                          Vec<Symbol*>&                 visited,
                          SymExpr*                      use);




//
// Implement the start of the recursion
//
static bool isSafeToDeref(Map<Symbol*, Vec<SymExpr*>*>& defMap,
                          Map<Symbol*, Vec<SymExpr*>*>& useMap,
                          Symbol*                       field,
                          Symbol*                       ref) {
  Vec<Symbol*> visited;

  return isSafeToDeref(defMap, useMap, field, ref, visited);
}


//
// The recursive loop
//
static bool isSafeToDeref(Map<Symbol*, Vec<SymExpr*>*>& defMap,
                          Map<Symbol*, Vec<SymExpr*>*>& useMap,
                          Symbol*                       field,
                          Symbol*                       ref,
                          Vec<Symbol*>&                 visited) {
  bool retval = true;

  if (visited.set_in(ref) == NULL) {
    int numDefs = (defMap.get(ref)) ? defMap.get(ref)->n : 0;

    visited.set_add(ref);

    if (isArgSymbol(ref) && numDefs > 0) {
      retval = false;

    } else if (numDefs > 1) {
      retval = false;

    } else {
      for_uses(use, useMap, ref) {
        if (isSafeToDeref(defMap,
                          useMap,
                          field,
                          ref,
                          visited,
                          use) == false) {
          retval = false;
          break;
        }
      }
    }
  }

  return retval;
}

//
// Helper when looping over the uses of a ref-var
//
static bool isSafeToDeref(Map<Symbol*, Vec<SymExpr*>*>& defMap,
                          Map<Symbol*, Vec<SymExpr*>*>& useMap,
                          Symbol*                       field,
                          Symbol*                       ref,
                          Vec<Symbol*>&                 visited,
                          SymExpr*                      use) {
  bool retval = true;

  if (CallExpr* call = toCallExpr(use->parentExpr)) {
    if (call->isResolved()) {
      ArgSymbol* arg = actual_to_formal(use);

      if (arg->intent == INTENT_CONST_REF) {
        retval = true;
      } else {
        retval = isSafeToDeref(defMap, useMap, field, arg, visited);
      }

    } else if (call->isPrimitive(PRIM_MOVE)) {
      SymExpr* newRef = toSymExpr(call->get(1));

      INT_ASSERT(newRef);

      retval = isSafeToDeref(defMap,
                             useMap,
                             field,
                             newRef->symbol(),
                             visited);

    } else if (call->isPrimitive(PRIM_SET_MEMBER) == true &&
               field                              != NULL) {
      SymExpr* se = toSymExpr(call->get(2));

      INT_ASSERT(se);

      if (se->symbol() != field) {
        retval = false;
      }

    } else if (!call->isPrimitive(PRIM_DEREF)) {
      retval = false; // what does this preclude? can this be an assert?
    }

  } else {
    retval = false; // what does this preclude? can this be an assert?
  }

  return retval;
}

static bool computeDotLocale(Symbol* sym) {
  INT_ASSERT(sym->isRef());

  DotInfo* info = dotLocaleMap[sym];

  if (info == NULL) {
    // No uses of this symbol, so definitely no uses of dot-locale
    return false;
  } else if (info->finalized) {
    return info->usesDotLocale;
  }

  bool retval = false;

  while (!info->todo.empty() && !retval) {
    SymExpr* use = info->todo.back();
    info->todo.pop_back();

    CallExpr* call = toCallExpr(use->parentExpr);
    INT_ASSERT(call);
    CallExpr* parent = toCallExpr(call->parentExpr);

    if (call->isResolved()) {
      ArgSymbol* form = actual_to_formal(use);
      if (form->isRef() && computeDotLocale(form)) retval = true;
    } else if (parent && isMoveOrAssign(parent)) {
      // If something copies this reference, we need to see if the new
      // reference also uses .locale
      Symbol* LHS = toSymExpr(parent->get(1))->symbol();
      if (call->isPrimitive(PRIM_ADDR_OF) ||
          call->isPrimitive(PRIM_SET_REFERENCE)) {
        if (computeDotLocale(LHS)) retval = true;
      } else if (call->isPrimitive(PRIM_GET_MEMBER) ||
                 call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                 call->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                 call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
        // Check if the field is used in a .locale
        if (LHS->isRef() && use == call->get(2) && computeDotLocale(LHS)) {
          retval = true;
        }
      } else if (call->isPrimitive(PRIM_WIDE_GET_LOCALE) ||
                 call->isPrimitive(PRIM_WIDE_GET_NODE)) {
        retval = true;
      }
    } else if (call->isPrimitive(PRIM_MOVE)) {
      SymExpr* LHS = toSymExpr(call->get(1));
      if (LHS->isRef() && use != LHS && computeDotLocale(LHS->symbol())) retval = true;
    } else if (call->isPrimitive(PRIM_SET_MEMBER) && use == call->get(3)) {
      // See if the field we're setting may use .locale
      // This could be improved by only looking at the base object's instance,
      // but that's much more complicated.
      SymExpr* member = toSymExpr(call->get(2));
      if (member->isRef() && computeDotLocale(member->symbol())) {
        retval = true;
      }
    } else if (call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
      // BHARSH 2016-11-02: We could try to handle the homogeneous tuple case
      // by iterating over all of the fields, but to keep this initial
      // implementation simple I'm tempted to just return true and leave this
      // case for later.
      retval = true;
    }
  }

  info->finalized = true;
  info->usesDotLocale = retval;

  return retval;
}

static void computeUsesDotLocale() {
  std::vector<Symbol*> todo;

  forv_Vec(SymExpr, se, gSymExprs) {
    if (!(isVarSymbol(se->symbol()) || isArgSymbol(se->symbol()))) continue;
    if (!se->isRef()) continue;

    DotInfo* info = NULL;
    DotInfoIter it = dotLocaleMap.find(se->symbol());
    if (it == dotLocaleMap.end()) {
      info = new DotInfo();
      todo.push_back(se->symbol());
      dotLocaleMap[se->symbol()] = info;
    } else {
      info = it->second;
    }

    info->todo.push_back(se);
  }

  for_vector(Symbol, sym, todo) {
    computeDotLocale(sym);
  }
}
