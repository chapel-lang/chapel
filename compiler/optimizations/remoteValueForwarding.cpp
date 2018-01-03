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

#include "optimizations.h"

#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"

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
  if (fNoRemoteValueForwarding == false && requireOutlinedOn()) {
    inferConstRefs();
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
        if (field->isRef()) {
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

static void defaultForwarding(Map<Symbol*, Vec<SymExpr*>*>& useMap,
                              FnSymbol*                     fn,
                              ArgSymbol*                    arg);
static void insertSerialization(FnSymbol*  fn,
                                ArgSymbol* arg);


static bool shouldSerialize(ArgSymbol* arg) {
  bool retval = false;
  bool hasSerializer = serializeMap.find(arg->getValType()) != serializeMap.end();

  if (hasSerializer == false) {
    retval = false;
  } else if (isRecordWrappedType(arg->getValType())) {
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
    retval = hasSerializer;
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

  // Forward array values and references to array values.
  // This is OK because the array/domain/distribution wrapper
  // records have fields that do not vary.
  // It does not matter if the on-body synchronizes.
  // (The array class, e.g. DefaultRectangularArr, is what varies,
  //  and it contains a pointer to the actual data, which might
  //  be replaced with another pointer).
  // An alternative strategy would be to migrate the contents of the
  // array header class into the wrapper record - but that would require
  // quite a lot of code changes, and some other features have entangled
  // designs (including privatization and the DSI interface).
  } else if (isRecordWrappedType(arg->getValType())) {
    retval = true;

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
  } else if (arg->intent == INTENT_CONST_IN &&
      !arg->type->symbol->hasFlag(FLAG_REF)) {
    // BHARSH TODO: This can currently happen when the arg is the lhs of a +=,
    // but it obviously needs to have the 'ref' intent. One example can be seen
    // for += between strings.
    retval = true;

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
// BHARSH TODO: Split this function into more easily-digestible pieces
// BHARSH TODO: capture the assumptions made here in documentation
//
static void insertSerialization(FnSymbol*  fn,
                                ArgSymbol* arg) {
  Type* oldArgType    = arg->type;
  IntentTag oldIntent = arg->intent;
  ArgSymbol* oldArg   = NULL;

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

  // Update each callsite to invoke the serializer
  forv_Vec(CallExpr, call, *fn->calledBy) {
    SymExpr* actual = toSymExpr(formal_to_actual(call, arg));
    SET_LINENO(actual);

    VarSymbol* data = newTemp(astr(arg->cname, "_data"), dataType);
    if (arg->hasFlag(FLAG_COFORALL_INDEX_VAR)) {
      data->addFlag(FLAG_COFORALL_INDEX_VAR);
    }
    call->insertBefore(new DefExpr(data));

    if (serializeFn->hasFlag(FLAG_FN_RETARG)) {
      call->insertBefore(new CallExpr(serializeFn, actual->copy(), data));
    } else {
      call->insertBefore(new CallExpr(PRIM_MOVE, data, new CallExpr(serializeFn, actual->copy())));
    }

    actual->replace(new SymExpr(data));

    // Preserve the actual that will be used to create the runtime type within
    // the on-statement
    if (needsRuntimeType) {
      call->insertAtTail(actual);
    }
  }

  // We need to keep the RuntimeType around to use as the 'this' formal
  // for the deserialize call. Add FLAG_NO_RVF to avoid an infinite loop where
  // we keep trying to serialize a formal we just added.
  if (needsRuntimeType) {
    SET_LINENO(fn);
    oldArg = new ArgSymbol(oldIntent, astr(arg->cname, "_old"), oldArgType);
    oldArg->addFlag(FLAG_NO_RVF);
    fn->insertFormalAtTail(oldArg);
  }

  // Replace all uses of 'arg' with a local reference to a deserialized
  // instance of 'arg'
  SET_LINENO(fn);
  VarSymbol* deserialized = newTemp(arg->cname, oldArgType->getValType());
  VarSymbol* dsRef = newTemp(arg->cname, QualifiedType(QUAL_REF, oldArgType->getValType()));
  for_SymbolSymExprs(se, arg) {
    se->setSymbol(dsRef);
  }

  Expr* anchor = fn->body->body.head;
  anchor->insertBefore(new DefExpr(deserialized));
  anchor->insertBefore(new DefExpr(dsRef));

  CallExpr* deserializeCall = new CallExpr(deserializeFn, arg);
  CallExpr* callToAdd = NULL;

  if (needsRuntimeType) {
    FnSymbol* runtimeTypeFn = valueToRuntimeTypeMap.get(oldArgType->getValType());
    INT_ASSERT(runtimeTypeFn != NULL);
    VarSymbol* info = new VarSymbol("ds_info", runtimeTypeFn->retType);
    anchor->insertBefore(new DefExpr(info));
    anchor->insertBefore(new CallExpr(PRIM_MOVE, info, new CallExpr(runtimeTypeFn, oldArg)));

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

  CallExpr* lastExpr = toCallExpr(fn->body->body.tail);
  INT_ASSERT(lastExpr && lastExpr->isPrimitive(PRIM_RETURN));

  FnSymbol* dataDestroyFn = getAutoDestroy(arg->getValType());
  if (dataDestroyFn != NULL) {
    lastExpr->insertBefore(new CallExpr(dataDestroyFn, arg));
  }

  FnSymbol* deserializeDestroyFn = getAutoDestroy(deserialized->getValType());
  if (deserializeDestroyFn != NULL) {
    lastExpr->insertBefore(new CallExpr(deserializeDestroyFn, deserialized));
  }
}

static void defaultForwarding(Map<Symbol*, Vec<SymExpr*>*>& useMap,
                              FnSymbol*                     fn,
                              ArgSymbol*                    arg) {
  // Dereference the arg type.
  Type* prevArgType = arg->type;

  arg->type = arg->getValType();

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
