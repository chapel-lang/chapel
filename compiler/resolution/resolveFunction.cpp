/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include "resolveFunction.h"

#include "astutil.h"
#include "AstVisitorTraverse.h"
#include "CatchStmt.h"
#include "CForLoop.h"
#include "DecoratedClassType.h"
#include "DeferStmt.h"
#include "driver.h"
#include "expr.h"
#include "ForLoop.h"
#include "ForallStmt.h"
#include "IfExpr.h"
#include "ImportStmt.h"
#include "iterator.h"
#include "LoopExpr.h"
#include "LoopStmt.h"
#include "ParamForLoop.h"
#include "passes.h"
#include "postFold.h"
#include "resolution.h"
#include "resolveIntents.h"
#include "splitInit.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "TryStmt.h"
#include "view.h"
#include "WhileStmt.h"

#include <set>

static void resolveFormals(FnSymbol* fn);

std::map<ArgSymbol*, std::string> exportedDefaultValues;

static void markIterator(FnSymbol* fn);

static void insertUnrefForArrayOrTupleReturn(FnSymbol* fn);

static bool doNotChangeTupleTypeRefLevel(FnSymbol* fn, bool forRet);

static void protoIteratorClass(FnSymbol* fn, Type* yieldedType);

static bool insertAndResolveCasts(FnSymbol* fn);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void resolveSignatureAndFunction(FnSymbol* fn) {
  resolveSignature(fn);
  resolveFunction(fn);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void resolveSignature(FnSymbol* fn) {
    // Don't resolve formals for concrete functions
    // more often than necessary.
    static std::set<FnSymbol*> done;

    if (done.find(fn) == done.end()) {
      done.insert(fn);

      resolveFormals(fn);
    }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void updateIfRefFormal(FnSymbol* fn, ArgSymbol* formal);

static bool needRefFormal(FnSymbol* fn, ArgSymbol* formal, bool* needRefIntent);

static bool shouldUpdateAtomicFormalToRef(FnSymbol* fn, ArgSymbol* formal);

static void handleParamCNameFormal(FnSymbol* fn, ArgSymbol* formal);

static void storeDefaultValuesForPython(FnSymbol* fn, ArgSymbol* formal);

static void resolveFormals(FnSymbol* fn) {
  for_formals(formal, fn) {
    if (formal->type == dtUnknown) {
      if (formal->typeExpr == NULL) {
        formal->type = dtObject;

      } else {
        resolveBlockStmt(formal->typeExpr);

        formal->type = formal->typeExpr->body.tail->getValType();
      }
    }

    if (formal->name == astr_chpl_cname) {
      handleParamCNameFormal(fn, formal);
      formal->defPoint->remove();
      continue;
    }

    if (formal->type->symbol->hasFlag(FLAG_REF) == false) {
      if ((formal->type                             != dtString &&
           formal->type                             != dtBytes) ||
          formal->hasFlag(FLAG_INSTANTIATED_PARAM) == false) {
        updateIfRefFormal(fn, formal);
      }
    }

    if ((formal->intent == INTENT_BLANK || formal->intent == INTENT_CONST) &&
        !formal->hasFlag(FLAG_TYPE_VARIABLE) &&
        formal->type->symbol->hasFlag(FLAG_MANAGED_POINTER) &&
        // This is a workaround for problems with owned/shared auto-destroy
        // (since blank intent for these types == in intent)
        !fn->hasFlag(FLAG_AUTO_DESTROY_FN) &&
        !fn->hasFlag(FLAG_INIT_COPY_FN) &&
        !fn->hasFlag(FLAG_AUTO_COPY_FN)) {
      IntentTag useIntent = concreteIntentForArg(formal);
      if ((useIntent & INTENT_FLAG_IN))
        formal->intent = useIntent;
    }

    if (formal->defaultExpr != NULL && fn->hasFlag(FLAG_EXPORT)) {
      storeDefaultValuesForPython(fn, formal);
    }
  }
}

// When compiling for Python interoperability, default values for arguments
// should get propagated to the generated Python files.
static void storeDefaultValuesForPython(FnSymbol* fn, ArgSymbol* formal) {
  if (fLibraryPython) {
    Expr* end = formal->defaultExpr->body.tail;

    if (SymExpr* sym = toSymExpr(end)) {
      VarSymbol* var = toVarSymbol(sym->symbol());

      // Might be an ArgSymbol instead of a VarSymbol
      if (var && var->isImmediate()) {
        Immediate* imm = var->immediate;
        switch (imm->const_kind) {
        case NUM_KIND_INT:
        case NUM_KIND_BOOL:
        case NUM_KIND_UINT:
        case NUM_KIND_REAL:
          {
          exportedDefaultValues[formal] = imm->to_string();
          break;
        }

        case CONST_KIND_STRING: {
          // Want to maintain the appearance of string-ness
          exportedDefaultValues[formal] = "\"" + imm->to_string() + "\"";
          break;
        }

        case NUM_KIND_COMPLEX: {
          // Complex immediates only come up for the type's default value, since
          // all other bits that could be literals also could be computations
          // involving variables named i (so we need to resolve it).
          INT_FATAL("Complex literals were not expected as a default value");
        }

        default: {
          INT_FATAL("Unexpected literal type for default value");
          break;
        }
        } // closes switch statement
      } else {
        USR_WARN(formal, "Non-literal default values are ignored in "
                 "exported functions, argument '%s' must always be "
                 "provided", formal->name);
      }
    } else {
      USR_WARN(formal, "Non-literal default values are ignored in exported"
               " functions, argument '%s' must always be provided",
               formal->name);
    }
  } else if (fLibraryCompile) {
      USR_WARN(formal, "Default values aren't applicable in C, argument '%s'"
               " for exported function '%s' must always be provided",
               formal->name, fn->name);
  }
}

// Fix up value types that need to be ref types.
static void updateIfRefFormal(FnSymbol* fn, ArgSymbol* formal) {
  // For begin functions, copy ranges in if passed by blank intent.
  // TODO: remove this code - it should no longer be necessary
  if (fn->hasFlag(FLAG_BEGIN)                   == true &&
      formal->type->symbol->hasFlag(FLAG_RANGE) == true) {
    if (formal->intent == INTENT_BLANK ||
        formal->intent == INTENT_IN) {
      formal->intent = INTENT_CONST_IN;
    }
  }

  bool needRefIntent = false;
  if (needRefFormal(fn, formal, &needRefIntent) == true) {
    makeRefType(formal->type);

    if (formal->type->refType) {
      formal->type = formal->type->refType;

    } else {
      formal->qual = QUAL_REF;
    }

    if (needRefIntent)
      formal->intent = INTENT_REF;

  // Adjust tuples for intent.
  } else if (formal->type->symbol->hasFlag(FLAG_TUPLE) == true      &&
             formal->hasFlag(FLAG_TYPE_VARIABLE)       == false     &&
             formal                                    != fn->_this &&
             doNotChangeTupleTypeRefLevel(fn, false)   == false) {

    AggregateType* tupleType = toAggregateType(formal->type);
    IntentTag      intent    = formal->intent;

    INT_ASSERT(tupleType);

    if (shouldAddInFormalTempAtCallSite(formal, fn)) {
      // In, const in, intents treat tuple as an value variable
      // so it should not contain any refs.
      formal->type = computeNonRefTuple(tupleType);
    } else {
      // (for !shouldAddInFormalTempAtCallSite),
      // let 'in' intent work similarly to the blank intent.
      if (intent == INTENT_IN) {
        intent = INTENT_BLANK;
      }

      formal->type = computeTupleWithIntentForArg(intent, tupleType, formal);
    }
  }
}

static bool needRefFormal(FnSymbol* fn, ArgSymbol* formal,
                          bool* needRefIntent) {
  bool retval = false;

  if (formal->intent == INTENT_INOUT     ||
      formal->intent == INTENT_OUT       ||
      formal->intent == INTENT_REF       ||
      formal->intent == INTENT_CONST_REF) {
    retval = true;

  } else if (shouldUpdateAtomicFormalToRef(fn, formal) == true) {
    retval = true;

  // Adjust compiler-generated record copy-init to take in RHS by ref
  // if it contains a record field marked with FLAG_COPY_MUTATES.
  } else if (fn->hasFlag(FLAG_COPY_INIT) &&
             fn->hasFlag(FLAG_COMPILER_GENERATED) &&
             recordContainingCopyMutatesField(formal->getValType())) {
    retval = true;
    *needRefIntent = true;

  } else if (fn->hasFlag(FLAG_INIT_COPY_FN) &&
             formal == fn->getFormal(1) &&
             recordContainingCopyMutatesField(formal->getValType())) {
    retval = true;
    *needRefIntent = true;

  } else if (fn->hasFlag(FLAG_ITERATOR_FN)     == true &&
             isRecordWrappedType(formal->type) == true) {
    retval = true;

  } else if (formal                              == fn->_this &&
             formal->hasFlag(FLAG_TYPE_VARIABLE) == false     &&
             (isUnion(formal->type)  == true ||
               (isRecord(formal->type) == true &&
                !formal->type->symbol->hasFlag(FLAG_RANGE)))) {
    retval = true;

  } else {
    retval = false;
  }

  return retval;
}

//
// Generally, atomics must also be passed by reference when
// passed by blank intent.  The following expression checks for
// these cases by looking for atomics passed by blank intent and
// changing their type to a ref type.  Interestingly, this
// conversion does not seem to be required for single-locale
// compilation, but it is for multi-locale.  Otherwise, updates
// to atomics are lost (as demonstrated by
// test/functions/bradc/intents/test_pass_atomic.chpl).
//
// I say "generally" because there are a few cases where passing
// atomics by reference breaks things -- primarily in
// constructors, assignment operators, and tuple construction.
// So we have some unfortunate special checks that dance around
// these cases.
//
// While I can't explain precisely why these special cases are
// required yet, here are the tests that tend to have problems
// without these special conditions:
//
//   test/release/examples/benchmarks/hpcc/ra-atomics.chpl
//   test/types/atomic/sungeun/no_atomic_assign.chpl
//   test/functions/bradc/intents/test_construct_atomic_intent.chpl
//   test/users/vass/barrierWF.test-1.chpl
//   test/studies/shootout/spectral-norm/sidelnik/spectralnorm.chpl
//   test/release/examples/benchmarks/ssca2/SSCA2_main.chpl
//   test/parallel/taskPar/sungeun/barrier/*.chpl
//
static bool shouldUpdateAtomicFormalToRef(FnSymbol* fn, ArgSymbol* formal) {
  return formal->intent                        == INTENT_BLANK &&
         formal->hasFlag(FLAG_TYPE_VARIABLE)   == false        &&
         isAtomicType(formal->type)            == true         &&

         fn->name                              != astrSassign  &&

         fn->hasFlag(FLAG_BUILD_TUPLE)         == false;
}

bool recordContainingCopyMutatesField(Type* t) {
  AggregateType* at = toAggregateType(t);
  if (at == NULL) return false;
  if (!isRecord(at)) return false;
  if (at->symbol->hasFlag(FLAG_COPY_MUTATES)) return true;

  bool ret = false;
  for_fields(field, at) {
    if (AggregateType* atf = toAggregateType(field->type)) {
      if (isRecord(atf))
        ret |= recordContainingCopyMutatesField(atf);
    }
  }

  // Set the flag so that:
  // 1. this is easier to compute in the future and
  // 2. other code working with this type will know
  //    (e.g. lvalue checking)
  if (ret && !at->symbol->hasFlag(FLAG_COPY_MUTATES))
    at->symbol->addFlag(FLAG_COPY_MUTATES);

  return ret;
}

static void handleParamCNameFormal(FnSymbol* fn, ArgSymbol* formal) {
  // Handle param cnames for functions
  resolveBlockStmt(formal->defaultExpr);
  SymExpr* se = toSymExpr(formal->defaultExpr->body.last());
  if (se == NULL) {
    USR_FATAL(fn, "extern name expression must be param");
  }
  VarSymbol* var = toVarSymbol(se->symbol());
  if (!var || !var->isParameter()) {
    USR_FATAL(fn, "extern name expression must be param");
  }
  if (var->type == dtString || var->type == dtStringC) {
    fn->cname = var->immediate->v_string;
  } else {
    USR_FATAL(fn, "extern name expression must be a string");
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void resolveSpecifiedReturnType(FnSymbol* fn) {
  Type* retType = NULL;

  resolveBlockStmt(fn->retExprType);

  retType = fn->retExprType->body.tail->typeInfo();

  if (SymExpr* se = toSymExpr(fn->retExprType->body.tail)) {
    // Try resolving global type aliases
    if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      retType = resolveTypeAlias(se);
    }

    if (retType->symbol->hasFlag(FLAG_GENERIC)    == true) {
      SET_LINENO(fn->retExprType->body.tail);

      retType = resolveDefaultGenericTypeSymExpr(se);
    }
  }

  if (retType != dtUnknown) {
    if (retType->symbol->hasFlag(FLAG_TUPLE)   == true   &&
        doNotChangeTupleTypeRefLevel(fn, true) == false) {
      AggregateType* tupleType = toAggregateType(retType);

      retType     = getReturnedTupleType(fn, tupleType);
      fn->retType = retType;

    } else if (fn->returnsRefOrConstRef() == true) {
      makeRefType(retType);

      retType     = retType->refType;
      fn->retType = retType;

    } else {
      fn->retType = retType;
    }

    if (fn->isIterator() == true && fn->iteratorInfo == NULL) {
      // Note: protoIteratorClass changes fn->retType to the iterator record.
      // The original return type is stored here in retType.
      protoIteratorClass(fn, retType);
    }

  } else {
    fn->retType = retType;
  }

  // Also update the return symbol type
  if (Symbol* ret = fn->getReturnSymbol()) {
    if (ret->type == dtUnknown) {
      ret->type = retType;
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void markTypesWithDefaultInitEqOrAssign(FnSymbol* fn);

void resolveFunction(FnSymbol* fn, CallExpr* forCall) {
  if (fn->isResolved() == false) {
    if (fn->id == breakOnResolveID) {
      printf("breaking on resolve fn %s[%d] (%d args)\n",
             fn->name, fn->id, fn->numFormals());
      gdbShouldBreakHere();
    }

    fn->addFlag(FLAG_RESOLVED);

    fn->tagIfGeneric();

    if (strcmp(fn->name, "init") == 0 && fn->isMethod()) {
      AggregateType* at = toAggregateType(fn->_this->getValType());
      if (at->symbol->hasFlag(FLAG_GENERIC) == false) {
        resolveTypeWithInitializer(at, fn);
      }
    }

    if (fn->hasFlag(FLAG_EXTERN) == true) {
      resolveBlockStmt(fn->body);

      resolveReturnType(fn);

    } else {
      if (fn->isIterator() == true) {
        markIterator(fn);
      }

      if (needsCapture(fn))
        convertFieldsOfRecordThis(fn);

      insertFormalTemps(fn);

      resolveBlockStmt(fn->body);

      insertUnrefForArrayOrTupleReturn(fn);

      Type* yieldedType = NULL;
      resolveReturnTypeAndYieldedType(fn, &yieldedType);

      fixPrimInitsAndAddCasts(fn);

      if (fn->isIterator() == true && fn->iteratorInfo == NULL) {
        protoIteratorClass(fn, yieldedType);
      }

      if (fn->isMethod() == true && fn->_this != NULL) {
        ensureInMethodList(fn);
      }

      if (forCall != NULL) {
        resolveAlsoParallelIterators(fn, forCall);
      }

      markTypesWithDefaultInitEqOrAssign(fn);
    }
    popInstantiationLimit(fn);
  }
}

static void markTypesWithDefaultInitEqOrAssign(FnSymbol* fn) {

  if (fn->name == astrSassign &&
      fn->numFormals() >= 1) {
    ArgSymbol* lhs = fn->getFormal(1);
    Type* t = lhs->getValType();
    if (fn->hasFlag(FLAG_COMPILER_GENERATED))
      t->symbol->addFlag(FLAG_TYPE_DEFAULT_ASSIGN);
    else
      t->symbol->addFlag(FLAG_TYPE_CUSTOM_ASSIGN);
  }

  if (fn->name == astrInitEquals &&
      fn->numFormals() >= 2) {
    ArgSymbol* lhs = fn->getFormal(2); // 1 is mt
    Type* t = lhs->getValType();
    if (fn->hasFlag(FLAG_COMPILER_GENERATED))
      t->symbol->addFlag(FLAG_TYPE_DEFAULT_INIT_EQUAL);
    else
      t->symbol->addFlag(FLAG_TYPE_CUSTOM_INIT_EQUAL);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool isFollowerIterator(FnSymbol* fn);
static bool isVecIterator(FnSymbol* fn);
static bool isIteratorOfType(FnSymbol* fn, Symbol* iterTag);

static void markIterator(FnSymbol* fn) {
  //
  // Mark serial loops that yield inside of follower, standalone, and
  // explicitly vectorized iterators as order independent. By using a
  // forall loop or a loop over a vectorized iterator, a user is asserting
  // that the loop can be executed in any iteration order.

  // Here we just mark the iterator's yielding loops as order independent
  // as they are ones that will actually execute the body of the loop that
  // invoked the iterator. Note that for nested loops with a single yield,
  // only the inner most loop is marked.
  //
  if (isFollowerIterator(fn)   == true ||
      isStandaloneIterator(fn) == true ||
      isVecIterator(fn)        == true) {
    std::vector<CallExpr*> callExprs;

    collectCallExprs(fn->body, callExprs);

    for_vector(CallExpr, call, callExprs) {
      if (call->isPrimitive(PRIM_YIELD) == true) {
        if (LoopStmt* loop = LoopStmt::findEnclosingLoop(call)) {
          if (loop->isCoforallLoop() == false) {
            loop->orderIndependentSet(true);
          }
        }
      }
    }
  }

  //
  // Mark parallel iterators for inlining.
  //
  if (isParallelIterator(fn)) {
    fn->addFlag(FLAG_INLINE_ITERATOR);
  }
}

bool isLeaderIterator(FnSymbol* fn) {
  return isIteratorOfType(fn, gLeaderTag);
}

bool isStandaloneIterator(FnSymbol* fn) {
  return isIteratorOfType(fn, gStandaloneTag);
}

static bool isFollowerIterator(FnSymbol* fn) {
  return isIteratorOfType(fn, gFollowerTag);
}

static bool isVecIterator(FnSymbol* fn) {
  bool retval = false;

  if (fn->isIterator() == true) {
    retval = fn->hasFlag(FLAG_VECTORIZE_YIELDING_LOOPS);
  }

  return retval;
}

// Simple wrappers to check if a function is a specific type of iterator
static bool isIteratorOfType(FnSymbol* fn, Symbol* iterTag) {
  bool retval = false;

  if (fn->isIterator() == true) {
    for_formals(formal, fn) {
      if (formal->type == iterTag->type && paramMap.get(formal) == iterTag) {
        retval = true;
        break;
      }
    }
  }

  return retval;
}

// leader or standalone
bool isParallelIterator(FnSymbol* fn) {
  if (!fn->isIterator())
    return false;

  for_formals(formal, fn) {
    if (formal->type == gLeaderTag->type          &&
        (paramMap.get(formal) == gLeaderTag    ||
         paramMap.get(formal) == gStandaloneTag )  )
      return true;
  }

  return false;
}

/************************************* | **************************************
*                                                                             *
* If the returnSymbol of 'fn' is assigned to from an _array record, insert    *
* an autoCopy for that _array. If the autoCopy has not yet been resolved,     *
* this function will call 'resolveAutoCopyEtc'.                               *
*                                                                             *
* This supports the 'copy-out' rule for returning arrays.                     *
*                                                                             *
* BHARSH: Should this also check if fn->retTag != RET_TYPE?                   *
*                                                                             *
************************************** | *************************************/

static bool doNotUnaliasArray(FnSymbol* fn);
static CallExpr* findSetShape(CallExpr* setRet, Symbol* ret);

static void insertUnrefForArrayOrTupleReturn(FnSymbol* fn) {
  bool skipArray = doNotUnaliasArray(fn);
  bool skipTuple = doNotChangeTupleTypeRefLevel(fn, true);

  if (skipArray && skipTuple)
    // neither tuple nor array unref is necessary, so return
    return;

  Symbol* ret = fn->getReturnSymbol();

  for_SymbolSymExprs(se, ret) {
    if (CallExpr* call = toCallExpr(se->parentExpr)) {
      if (call->isPrimitive(PRIM_MOVE) == true &&
          call->get(1)                 == se) {
        Type* rhsType = call->get(2)->typeInfo();

        bool arrayIsh = (rhsType->symbol->hasFlag(FLAG_ARRAY) ||
                         rhsType->symbol->hasFlag(FLAG_ITERATOR_RECORD));

        bool handleArray = skipArray == false && arrayIsh;
        bool handleTuple = skipTuple == false &&
                           isTupleContainingAnyReferences(rhsType);

        // TODO: Should we check if the RHS is a symbol with
        // 'no auto destroy' on it? If it is, then we'd be copying
        // the RHS and it would never be destroyed...
        if ((handleArray || handleTuple) && !isTypeExpr(call->get(2))) {

          SET_LINENO(call);
          Expr*      rhs       = call->get(2)->remove();
          VarSymbol* tmp       = newTemp("array_unref_ret_tmp", rhsType);
          CallExpr*  initTmp   = new CallExpr(PRIM_MOVE,     tmp, rhs);
          CallExpr*  unrefCall = new CallExpr("chpl__unref", tmp);
          CallExpr*  shapeSet  = findSetShape(call, ret);
          FnSymbol*  unrefFn   = NULL;

          // Used by callDestructors to catch assignment from
          // a ref to 'tmp' when we know we don't want to copy.
          tmp->addFlag(FLAG_NO_COPY);

          call->insertBefore(new DefExpr(tmp));
          call->insertBefore(initTmp);

          call->insertAtTail(unrefCall);

          unrefFn = resolveNormalCall(unrefCall);

          resolveFunction(unrefFn);

          // Relies on the ArrayView variant having
          // the 'unref fn' flag in ChapelArray.
          if (arrayIsh && unrefFn->hasFlag(FLAG_UNREF_FN) == false) {
            // If the function does not have this flag, this must
            // be a non-view array. Remove the unref call.
            unrefCall->replace(rhs->copy());

            tmp->defPoint->remove();

            initTmp->remove();

            INT_ASSERT(unrefCall->inTree() == false);

            if (shapeSet) setIteratorRecordShape(shapeSet);

          } else if (shapeSet) {
            // Set the shape on the array unref temp instead of 'ret'.
            shapeSet->get(1)->replace(new SymExpr(tmp));
            call->insertBefore(shapeSet->remove());
            setIteratorRecordShape(shapeSet);
          }
        }
      }
    }
  }
}

static bool doNotUnaliasArray(FnSymbol* fn) {
  return (fn->hasFlag(FLAG_NO_COPY_RETURN) ||
          fn->hasFlag(FLAG_UNALIAS_FN) ||
          fn->hasFlag(FLAG_RUNTIME_TYPE_INIT_FN) ||
          fn->hasFlag(FLAG_INIT_COPY_FN) ||
          fn->hasFlag(FLAG_AUTO_COPY_FN) ||
          fn->hasFlag(FLAG_UNREF_FN) ||
          fn->hasFlag(FLAG_RETURNS_ALIASING_ARRAY) ||
          fn->hasFlag(FLAG_FN_RETURNS_ITERATOR));
}

// Generally speaking, tuples containing refs should be converted
// to tuples without refs before returning.
// This function returns true for exceptional FnSymbols
// where tuples containing refs can be returned.
//
static
bool doNotChangeTupleTypeRefLevel(FnSymbol* fn, bool forRet) {
  if (fn->hasFlag(FLAG_INIT_TUPLE)               || // chpl__init_tuple
      fn->hasFlag(FLAG_BUILD_TUPLE)              || // _build_tuple(_allow_ref)
      fn->hasFlag(FLAG_BUILD_TUPLE_TYPE)         || // _build_tuple_type
      fn->hasFlag(FLAG_TUPLE_CAST_FN)            || // _cast for tuples
      fn->hasFlag(FLAG_EXPAND_TUPLES_WITH_VALUES)|| // iteratorIndex
      fn->hasFlag(FLAG_INIT_COPY_FN)             || // tuple chpl__initCopy
      fn->hasFlag(FLAG_AUTO_COPY_FN)             || // tuple chpl__autoCopy
      fn->hasFlag(FLAG_AUTO_DESTROY_FN)          || // tuple chpl__autoDestroy
      fn->hasFlag(FLAG_UNALIAS_FN)               || // tuple chpl__unalias
      fn->hasFlag(FLAG_ALLOW_REF)                || // iteratorIndex
      (forRet && fn->hasFlag(FLAG_ITERATOR_FN)) // not iterators b/c
                                    //  * they might return by ref
                                    //  * might need to return a ref even
                                    //    when not indicated return by ref.
     ) {
    return true;
  } else {
    return false;
  }
}

// Find the PRIM_ITERATOR_RECORD_SET_SHAPE call following 'setRet'.
// It should be setting the shape for 'ret'.
static CallExpr* findSetShape(CallExpr* setRet, Symbol* ret) {
  for (Expr* curr = setRet->next; curr; curr = curr->next)
    if (CallExpr* call = toCallExpr(curr))
      if (call->isPrimitive(PRIM_ITERATOR_RECORD_SET_SHAPE)) {
        INT_ASSERT(toSymExpr(call->get(1))->symbol() == ret);
        return call;
      }
  // not found
  return NULL;
}

class SplitInitVisitor : public AstVisitorTraverse {
 public:
  bool inFunction;
  bool changed;
  std::map<Symbol*, Expr*>& preventMap;
  SplitInitVisitor(std::map<Symbol*, Expr*>& preventMap)
    : inFunction(false), changed(false), preventMap(preventMap)
  { }
  virtual bool enterFnSym(FnSymbol* node);
  virtual bool enterDefExpr(DefExpr* def);
  virtual bool enterCallExpr(CallExpr* call);
};

bool SplitInitVisitor::enterFnSym(FnSymbol* node) {
  // Only visit the top level function requested
  if (inFunction) return false;
  inFunction = true;
  return true;
}

bool SplitInitVisitor::enterDefExpr(DefExpr* def) {
  return false;
}

bool SplitInitVisitor::enterCallExpr(CallExpr* call) {
  if (call->isPrimitive(PRIM_DEFAULT_INIT_VAR)) {

    // Can this be replaced by a split init?
    std::vector<CallExpr*> initAssigns;
    Expr* prevent = NULL;
    bool foundSplitInit = false;

    SymExpr* se = toSymExpr(call->get(1));
    Symbol* sym = se->symbol();

    bool isOutFormal = sym->hasFlag(FLAG_FORMAL_TEMP_OUT);

    // Don't allow an out-formal to be split-init after a return because
    // that would leave the out-formal uninitialized.
    bool allowReturns = !isOutFormal;
    foundSplitInit = findInitPoints(call, initAssigns, prevent, allowReturns);

    if (foundSplitInit) {
      // Check that all of the assignments have a same-type RHS
      for_vector(CallExpr, call, initAssigns) {
        Type* rhsType = call->get(2)->getValType();
        if (rhsType != sym->type) {
          prevent = call;
          foundSplitInit = false;
        }
      }
      // Check that it's not an array init we converted int =
      // (This is a workaround - the actual solution is to have something
      //  like init= for arrays)
      if (sym->hasFlag(FLAG_INITIALIZED_LATER))
        foundSplitInit = false;
    }

    if (foundSplitInit) {
      // Change the PRIM_DEFAULT_INIT_VAR to PRIM_INIT_VAR_SPLIT_DECL
      call->primitive = primitives[PRIM_INIT_VAR_SPLIT_DECL];
      SymExpr* typeSe = toSymExpr(call->get(2));
      Symbol* type = typeSe->symbol();

      // Change the '=' calls found into PRIM_INIT_VAR_SPLIT_INIT
      for_vector(CallExpr, assign, initAssigns) {
        SET_LINENO(assign);
        Expr* rhs = assign->get(2)->remove();
        CallExpr* init = new CallExpr(PRIM_INIT_VAR_SPLIT_INIT, sym, rhs, type);
        assign->replace(init);
        resolveInitVar(init);
      }
    } else if (prevent != NULL) {
      preventMap[sym] = prevent;
    }
  }

  return false;
}

class AddOutIntentTypeArgs : public AstVisitorTraverse {
 public:
  bool inFunction;
  bool changed;
  AddOutIntentTypeArgs()
    : inFunction(false), changed(false)
  { }
  virtual bool enterFnSym(FnSymbol* node);
  virtual bool enterCallExpr(CallExpr* call);
};

bool AddOutIntentTypeArgs::enterFnSym(FnSymbol* node) {
  // Only visit the top level function requested
  if (inFunction) return false;
  inFunction = true;
  return true;
}
bool AddOutIntentTypeArgs::enterCallExpr(CallExpr* call) {
  // Also fix runtime types for function calls with untyped out formals.
  // This needs to happen after considering split-init to avoid having
  // it interfere with deciding to do split init.
  if (call->resolvedOrVirtualFunction() != NULL) {
    ArgSymbol* prevFormal = NULL;
    Expr* prevActual = NULL;
    for_formals_actuals(formal, actual, call) {
      Type* formalType = formal->type->getValType();
      bool outIntent = formal->intent == INTENT_OUT ||
                       formal->originalIntent == INTENT_OUT;

      if (outIntent &&
          formal->typeExpr == NULL &&
          formalType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {

        INT_ASSERT(prevFormal && prevActual);
        INT_ASSERT(prevFormal->hasFlag(FLAG_TYPE_FORMAL_FOR_OUT));

        SymExpr* typeSe = toSymExpr(prevActual);
        VarSymbol* typeVar = toVarSymbol(typeSe->symbol());
        SymExpr* actualSe = toSymExpr(actual);
        Symbol* actualSym = actualSe->symbol();

        if (typeVar->hasFlag(FLAG_TYPE_FORMAL_FOR_OUT)) {
          // Handle two different patterns that need adjustment
          // in order to pass a runtime type for an untyped out formal.
          //
          // This happens here so that split-init can occur
          // without a formalTmp.type call prohibiting it.
          // Also, if split-init is applied to the out-intent argument,
          // the resulting variable shouldn't be default-initialized at
          // all, so we need to get the type for the argument another way.
          //
          // 1.
          // Convert this pattern of code:
          //   def dummyTypeTmp
          //   outFn(dummyTypeTmp, formalTmp)
          //   move coerce_tmp, (PRIM_COERCE formalTmp, coerceTypeTmp)
          // into this
          //   (def removed)
          //   outFn(coerceTypeTmp, formalTmp)
          //   move coerce_tmp, formalTmp
          //
          // 2.
          // Convert this pattern of code:
          //   def dummyTypeTmp
          //   outFn(dummyTypeTmp, formalTmp)
          //   call '=' otherVariable, formalTmp
          // into this
          //   def dummyTypeTmp
          //   dummyTypeTemp = otherVariable.type
          //   outFn(dummyTypeTmp, formalTmp)
          //   call '=' otherVariable, formalTmp

          SET_LINENO(call);

          SymExpr* singleUse = actualSym->getSingleUse();
          INT_ASSERT(singleUse);
          CallExpr* usingCall = toCallExpr(singleUse->parentExpr);
          INT_ASSERT(usingCall);

          if (usingCall->isPrimitive(PRIM_COERCE)) {
            // case 1
            INT_ASSERT(usingCall && usingCall->isPrimitive(PRIM_COERCE));
            CallExpr* moveCall = toCallExpr(usingCall->parentExpr);
            INT_ASSERT(moveCall && (moveCall->isPrimitive(PRIM_MOVE) ||
                                    moveCall->isPrimitive(PRIM_ASSIGN)));

            SymExpr* coerceType = toSymExpr(usingCall->get(2));
            INT_ASSERT(coerceType);
            typeSe->setSymbol(coerceType->symbol());
            typeVar->defPoint->remove();
            usingCall->replace(new SymExpr(actualSym));

            // Don't destroy the formal temp on the way into the coerce_tmp
            actualSym->addFlag(FLAG_NO_AUTO_DESTROY);
          } else {
            // case 2
            FnSymbol* fn = usingCall->resolvedFunction();
            INT_ASSERT(fn && fn->hasFlag(FLAG_ASSIGNOP));

            SymExpr* lhsSe = toSymExpr(usingCall->get(1));
            Symbol* lhs = lhsSe->symbol();

            BlockStmt* block = new BlockStmt(BLOCK_TYPE);
            CallExpr* m = new CallExpr(PRIM_MOVE, typeVar,
                                       new CallExpr(PRIM_TYPEOF, lhs));
            block->insertAtTail(m);
            call->insertBefore(block);
            resolveBlockStmt(block);
            block->flattenAndRemove();
          }
          changed = true;
        }
      }
      prevFormal = formal;
      prevActual = actual;
    }
  }
  return true;
}


class FixPrimInitsVisitor : public AstVisitorTraverse {
 public:
  bool inFunction;
  bool changed;
  std::map<Symbol*, Expr*>& preventMap;
  FixPrimInitsVisitor(std::map<Symbol*, Expr*>& preventMap)
    : inFunction(false), changed(false), preventMap(preventMap)
  { }
  virtual bool enterFnSym(FnSymbol* node);
  virtual bool enterCallExpr(CallExpr* call);
};

bool FixPrimInitsVisitor::enterFnSym(FnSymbol* node) {
  // Only visit the top level function requested
  if (inFunction) return false;
  inFunction = true;
  return true;
}

bool FixPrimInitsVisitor::enterCallExpr(CallExpr* call) {
  if (call->isPrimitive(PRIM_DEFAULT_INIT_VAR) ||
      call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL)) {
    Expr* prevent = NULL;
    SymExpr* se = toSymExpr(call->get(1));
    Symbol* sym = se->symbol();
    if (preventMap.count(sym))
      prevent = preventMap[sym];

    lowerPrimInit(call, prevent);
    // It was lowered if the call has been removed and replaced.
    if (call->isPrimitive(PRIM_NOOP))
      changed = true;
  }

  return true;
}

class MarkTempsVisitor : public AstVisitorTraverse {
 public:
  bool inFunction;
  MarkTempsVisitor() : inFunction(false) { }
  virtual bool enterFnSym(FnSymbol* node);
  virtual bool enterDefExpr(DefExpr* node);
};

bool MarkTempsVisitor::enterFnSym(FnSymbol* node) {
  // Only visit the top level function requested
  if (inFunction) return false;
  inFunction = true;
  return true;
}

static void gatherTempsDeadLastMention(VarSymbol* v,
                                       std::set<VarSymbol*>& temps) {

  // store the temporary we are working on in the set
  if (temps.insert(v).second == false)
    return; // stop here if it was already in the set.

  for_SymbolSymExprs(se, v) {
    if (CallExpr* call = toCallExpr(se->getStmtExpr())) {
      SymExpr* lhsSe = NULL;
      CallExpr* subCall = NULL;
      if (isInitOrReturn(call, lhsSe, subCall)) {
        // call above sets lhsSe and initOrCtor
      } else if (call->resolvedOrVirtualFunction()) {
        subCall = call;
      }

      // handle a returned variable being inited here
      if (lhsSe != NULL) {
        VarSymbol* lhs = toVarSymbol(lhsSe->symbol());
        if (lhs != NULL && lhs != v && lhs->hasFlag(FLAG_TEMP))
          gatherTempsDeadLastMention(lhs, temps);
      }

      // also handle out intent variables being inited here
      FnSymbol* fn = subCall ? subCall->resolvedOrVirtualFunction() : NULL;
      if (fn != NULL) {
        int i = 1;
        for_formals_actuals(formal, actual, subCall) {
          bool outIntent = (formal->intent == INTENT_OUT ||
                            formal->originalIntent == INTENT_OUT);
          bool maybeLaterAssign = (i == 1 && fn->name == astrSassign);

          if (outIntent || maybeLaterAssign) {
            SymExpr* se = toSymExpr(actual);
            if (NamedExpr* ne = toNamedExpr(actual)) {
              INT_ASSERT(ne->name == formal->name);
              se = toSymExpr(ne->actual);
            }
            INT_ASSERT(se != NULL);

            VarSymbol* tmpVar = toVarSymbol(se->symbol());
            if (tmpVar != NULL && tmpVar != v && tmpVar->hasFlag(FLAG_TEMP)) {
              if (outIntent) {
                // initializing a temp with out intent
                gatherTempsDeadLastMention(tmpVar, temps);
              } else if (maybeLaterAssign &&
                         tmpVar->hasFlag(FLAG_INITIALIZED_LATER)) {
                // See through default-init/assign pattern generated for arrays
                // In that pattern, a '=' call sets a init_coerce_tmp variable
                // marked with FLAG_INITIALIZED_LATER. If that variable is involved
                // in user variable initialization, we need to find it.
                gatherTempsDeadLastMention(tmpVar, temps);
              }
            }
          }
          i++;
        }
      }
    }
  }
}

static void markTempsDeadLastMention(std::set<VarSymbol*>& temps) {

  bool makeThemEndOfBlock = false;
  bool canMakeThemGlobal = true;

  // Look at how the temps are used
  for_set(VarSymbol, v, temps) {

    if (v->hasFlag(FLAG_DEAD_END_OF_BLOCK) ||
        v->hasFlag(FLAG_INDEX_VAR) ||
        v->hasFlag(FLAG_CHPL__ITER) ||
        v->hasFlag(FLAG_CHPL__ITER_NEWSTYLE) ||
        v->hasFlag(FLAG_FORMAL_TEMP) ||
        v->getValType()->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
      // index vars, iterator records are always end-of-block
      // but shouldn't be global variables.
      makeThemEndOfBlock = true;
      canMakeThemGlobal = false;
      break;
    }

    for_SymbolSymExprs(se, v) {
      if (CallExpr* call = toCallExpr(se->getStmtExpr())) {
        SymExpr* lhsSe = NULL;
        CallExpr* subCall = NULL;
        if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
          lhsSe = toSymExpr(call->get(1));
        } else if (isInitOrReturn(call, lhsSe, subCall)) {
          // call above sets lhsSe and initOrCtor
        } else if (call->resolvedOrVirtualFunction()) {
          subCall = call;
        }

        // returning into a user var?
        if (lhsSe != NULL) {
          VarSymbol* lhs = toVarSymbol(lhsSe->symbol());
          if (lhs != NULL && lhs != v && !lhs->hasFlag(FLAG_TEMP)) {
            // Used in initializing a user var, so mark end of block
            makeThemEndOfBlock = true;
            break;
          }
        }
        // out intent setting a user var?
        if (subCall != NULL && subCall->resolvedOrVirtualFunction() != NULL) {
          for_formals_actuals(formal, actual, subCall) {
            bool outIntent = (formal->intent == INTENT_OUT ||
                              formal->originalIntent == INTENT_OUT);
            if (outIntent) {
              SymExpr* se = toSymExpr(actual);
              if (NamedExpr* ne = toNamedExpr(actual)) {
                INT_ASSERT(ne->name == formal->name);
                se = toSymExpr(ne->actual);
              }
              INT_ASSERT(se != NULL);

              VarSymbol* outVar = toVarSymbol(se->symbol());

              if (outVar != NULL && outVar != v &&
                  !outVar->hasFlag(FLAG_TEMP)) {
                // Used in initializing a user var, so mark end of block
                makeThemEndOfBlock = true;
                break;
              }
            }
          }
        }

        if (makeThemEndOfBlock)
          break;
      }
    }
  }

  if (fNoEarlyDeinit)
    makeThemEndOfBlock = true;

  if (makeThemEndOfBlock) {
    for_set(VarSymbol, temp, temps) {
      temp->addFlag(FLAG_DEAD_END_OF_BLOCK);
      if (temp->defPoint != NULL) {
        FnSymbol* initFn = toFnSymbol(temp->defPoint->parentSymbol);
        if (initFn && initFn->hasFlag(FLAG_MODULE_INIT)) {
          ModuleSymbol* mod = temp->defPoint->getModule();
          if (mod && temp->defPoint->parentExpr == initFn->body &&
              canMakeThemGlobal) {
            // Move the temporary to global scope.
            mod->block->insertAtTail(temp->defPoint->remove());
          }
        }
      }
    }
  } else {
    for_set(VarSymbol, temp, temps) {
      temp->addFlag(FLAG_DEAD_LAST_MENTION);
    }
  }
}

void markTempDeadLastMention(VarSymbol* var) {
  INT_ASSERT(var && var->hasFlag(FLAG_TEMP));

  std::set<VarSymbol*> temps;
  gatherTempsDeadLastMention(var, temps);
  markTempsDeadLastMention(temps);
  INT_ASSERT(var->hasFlag(FLAG_DEAD_LAST_MENTION) ||
             var->hasFlag(FLAG_DEAD_END_OF_BLOCK));
}

bool MarkTempsVisitor::enterDefExpr(DefExpr* node) {
  // Mark temps as dead either "last mention" or "end of block"
  // and move temps marked "last mention" to global scope.
  //
  // This whole process depends on how split inits are handled
  // and needs to happen before addAutoDestroyCalls makes decisions
  // using the flags.
  //
  if (VarSymbol* var = toVarSymbol(node->sym)) {
    if (var->hasFlag(FLAG_TEMP) &&
        !(var->hasFlag(FLAG_DEAD_LAST_MENTION) ||
          var->hasFlag(FLAG_DEAD_END_OF_BLOCK))) {
      markTempDeadLastMention(var);
    }
  }
  return true;
}

void fixPrimInitsAndAddCasts(FnSymbol* fn) {
  // The function may contain default-init of a variable
  // followed by assignment or passing to out intent.
  //
  // In that event, use split init.
  //
  // To enable split-init decisions for out intent,
  // which needs to happen after types are established
  // and functions are called, earlier resolution steps leave
  // PRIM_DEFAULT_INIT_VAR in the tree and just use it to establish types.
  // This function needs to lower these.

  std::map<Symbol*, Expr*> splitInitPreventers;

  // Convert PRIM_DEFAULT_INIT_VAR to split init where possible
  if (fNoSplitInit == false) {
    SplitInitVisitor visitor(splitInitPreventers);
    fn->accept(&visitor);
  }

  // Fix out intent type formals
  // Note that this can remove PRIM_COERCE calls.
  {
    AddOutIntentTypeArgs visitor;
    fn->accept(&visitor);
  }

  // Handle PRIM_COERCE; insert and resolve casts
  // Note also that insertAndResolveCasts can add default-inits.
  insertAndResolveCasts(fn);

  // Lower any remaining PRIM_DEFAULT_INIT_VAR
  // Looping because the default init for a record might use
  // default arguments which in turn are set with default init.
  // This could be handled by adjusting the added AST instead of
  // revisiting the entire function.
  bool changed = true;
  while (changed) {
    FixPrimInitsVisitor visitor(splitInitPreventers);
    fn->accept(&visitor);
    changed = visitor.changed;
  }

  {
    MarkTempsVisitor visitor;
    fn->accept(&visitor);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static AggregateType* makeIteratorClass(FnSymbol* fn, Type* yieldedType);

static AggregateType* makeIteratorRecord(FnSymbol* fn, Type* yieldedType);

static const char*    iteratorClassName(FnSymbol* fn);

static FnSymbol*      makeGetIterator(AggregateType* iClass,
                                      AggregateType* iRecord);

static IteratorInfo*  makeIteratorInfo(AggregateType* iClass,
                                       AggregateType* iRecord,
                                       FnSymbol*      getIterator,
                                       FnSymbol*      fn,
                                       Type*          yieldedType);

static FnSymbol*      makeIteratorMethod(IteratorInfo* ii,
                                         const char*   name,
                                         Type*         retType);

static void protoIteratorClass(FnSymbol* fn, Type* yieldedType) {
  INT_ASSERT(yieldedType != NULL);
  if (yieldedType == dtUnknown) {
    USR_FATAL(fn, "unable to resolve yielded type");
  }

  SET_LINENO(fn);

  AggregateType* iClass  = makeIteratorClass(fn, yieldedType);
  AggregateType* iRecord = makeIteratorRecord(fn, yieldedType);
  FnSymbol*      getIter = makeGetIterator(iClass, iRecord);
  IteratorInfo*  ii      = makeIteratorInfo(iClass, iRecord, getIter,
                                            fn, yieldedType);

  iClass->iteratorInfo  = ii;
  iRecord->iteratorInfo = ii;
  fn->iteratorInfo      = ii;

  fn->retType           = iRecord;
  // Also adjust the type of the return symbol
  Symbol* retSym = fn->getReturnSymbol();
  INT_ASSERT(retSym);
  retSym->type = iRecord;

  fn->retTag            = RET_VALUE;

  fn->defPoint->insertBefore(new DefExpr(iClass->symbol));
  fn->defPoint->insertBefore(new DefExpr(iRecord->symbol));
  fn->defPoint->insertBefore(new DefExpr(getIter));

  makeRefType(iRecord);

  normalize(getIter);

  resolveFunction(getIter);
}

static AggregateType* makeIteratorClass(FnSymbol* fn, Type* yieldedType) {
  AggregateType* retval    = new AggregateType(AGGREGATE_CLASS);
  const char*    className = iteratorClassName(fn);
  TypeSymbol*    sym       = new TypeSymbol(astr("_ic_", className), retval);

  sym->addFlag(FLAG_ITERATOR_CLASS);
  sym->addFlag(FLAG_POD);

  retval->addRootType();

  VarSymbol* moreField = new VarSymbol("more", dtInt[INT_SIZE_DEFAULT]);
  retval->fields.insertAtTail(new DefExpr(moreField));
  // Creating "value" field here is trickier, see the PR message for #12963.

  return retval;
}

static AggregateType* makeIteratorRecord(FnSymbol* fn, Type* yieldedType) {
  AggregateType* retval    = new AggregateType(AGGREGATE_RECORD);
  const char*    className = iteratorClassName(fn);
  TypeSymbol*    sym       = new TypeSymbol(astr("_ir_", className), retval);

  sym->addFlag(FLAG_ITERATOR_RECORD);
  sym->addFlag(FLAG_NOT_POD);

  if (fn->retTag == RET_REF) {
    sym->addFlag(FLAG_REF_ITERATOR_CLASS);
  }

  retval->scalarPromotionType = yieldedType;

  return retval;
}

static const char* iteratorClassName(FnSymbol* fn) {
  const char* retval = NULL;

  if (fn->_this == NULL) {
    retval = astr(fn->name);

  } else {
    retval = astr(fn->name, "_", fn->_this->type->symbol->cname);
  }

  return retval;
}

static FnSymbol* makeGetIterator(AggregateType* iClass,
                                 AggregateType* iRecord) {
  VarSymbol* ret         = newTemp("_ic_", iClass);
  CallExpr*  icAllocCall = callChplHereAlloc(ret->typeInfo());
  FnSymbol*  retval      = new FnSymbol("_getIterator");

  retval->addFlag(FLAG_AUTO_II);
  retval->addFlag(FLAG_INLINE);
  retval->addFlag(FLAG_UNSAFE);

  retval->retType = iClass;

  retval->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "ir", iRecord));

  retval->insertAtTail(new DefExpr(ret));
  retval->insertAtTail(new CallExpr(PRIM_MOVE,   ret, icAllocCall));
  retval->insertAtTail(new CallExpr(PRIM_SETCID, ret));
  retval->insertAtTail(new CallExpr(PRIM_RETURN, ret));

  return retval;
}

static IteratorInfo*  makeIteratorInfo(AggregateType* iClass,
                                       AggregateType* iRecord,
                                       FnSymbol*      getIterator,
                                       FnSymbol*      fn,
                                       Type*          yieldedType) {
  Type*         defaultInt = dtInt[INT_SIZE_DEFAULT];
  IteratorInfo* ii         = new IteratorInfo();

  ii->iterator    = fn;
  ii->iclass      = iClass;
  ii->irecord     = iRecord;
  ii->getIterator = getIterator;

  ii->advance     = makeIteratorMethod(ii, "advance",  dtVoid);
  ii->zip1        = makeIteratorMethod(ii, "zip1",     dtVoid);
  ii->zip2        = makeIteratorMethod(ii, "zip2",     dtVoid);
  ii->zip3        = makeIteratorMethod(ii, "zip3",     dtVoid);
  ii->zip4        = makeIteratorMethod(ii, "zip4",     dtVoid);
  ii->hasMore     = makeIteratorMethod(ii, "hasMore",  defaultInt);
  ii->getValue    = makeIteratorMethod(ii, "getValue", yieldedType);
  ii->init        = makeIteratorMethod(ii, "init",     dtVoid);
  ii->incr        = makeIteratorMethod(ii, "incr",     dtVoid);

  ii->yieldedType = yieldedType;
  ii->iteratorRetTag = fn->retTag;

  return ii;
}

static FnSymbol* makeIteratorMethod(IteratorInfo* ii,
                                    const char*   name,
                                    Type*         retType) {
  FnSymbol* fn = new FnSymbol(name);

  fn->addFlag(FLAG_AUTO_II);

  if (strcmp(name, "advance") != 0) {
    fn->addFlag(FLAG_INLINE);
  }

  fn->setMethod(true);

  fn->_this   = new ArgSymbol(INTENT_BLANK, "this", ii->iclass);
  fn->_this->addFlag(FLAG_ARG_THIS);

  fn->retType = retType;

  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->insertFormalAtTail(fn->_this);

  ii->iterator->defPoint->insertBefore(new DefExpr(fn));

  normalize(fn);

  // Pretend that this function is already resolved.
  // Its body will be filled in during the lowerIterators pass.
  fn->addFlag(FLAG_RESOLVED);

  return fn;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void fixTypeNames(AggregateType* at) {
  const char* typeName = toString(at, false);

  if (at->symbol->name != typeName)
    at->symbol->name = typeName;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void computeReturnTypeParamVectors(BaseAST*      ast,
                                          Symbol*       retSymbol,
                                          Vec<Type*>&   retTypes,
                                          Vec<Symbol*>& retSymbols);

void resolveIfExprType(CondStmt* stmt) {
  Vec<Type*> retTypes;
  Vec<Symbol*> retSymbols;

  CallExpr* last = toCallExpr(stmt->thenStmt->body.tail);
  INT_ASSERT(last->isPrimitive(PRIM_MOVE));
  Symbol* ret = toSymExpr(last->get(1))->symbol();
  FnSymbol* fn = toFnSymbol(stmt->parentSymbol); // dummy to use for canDispatch

  computeReturnTypeParamVectors(stmt, ret, retTypes, retSymbols);

  if (retTypes.n == 1) {
    ret->type = retTypes.head();
  } else {
    INT_ASSERT(retTypes.n == 2 && retSymbols.n == 2);

    // NB: Assumes '0' and '1' correspond to 'then' and 'else', respectively.
    Type* thenType = retTypes.v[0];
    Type* elseType = retTypes.v[1];
    Type* retType  = NULL;

    Symbol* thenSym = retSymbols.v[0];
    Symbol* elseSym = retSymbols.v[1];

    bool thenTypeVar = thenSym->hasFlag(FLAG_TYPE_VARIABLE);
    bool elseTypeVar = elseSym->hasFlag(FLAG_TYPE_VARIABLE);

    // If only one branch returns a reference, that branch needs to copy the
    // result so that we can always free the result.
    if (isReferenceType(thenType) != isReferenceType(elseType)) {
      BlockStmt* refBranch = isReferenceType(thenType) ? stmt->thenStmt : stmt->elseStmt;
      CallExpr* call = toCallExpr(refBranch->body.tail);
      SymExpr* rhs = toSymExpr(call->get(2));
      if (typeNeedsCopyInitDeinit(rhs->getValType())) {
        CallExpr* copy = new CallExpr("chpl__autoCopy", rhs->remove());
        call->insertAtTail(copy);
        resolveCallAndCallee(copy);
        if (isReferenceType(thenType)) {
          thenType = copy->resolvedFunction()->retType;
        } else {
          elseType = copy->resolvedFunction()->retType;
        }
      }

      thenType = thenType->getValType();
      elseType = elseType->getValType();
    }

    if (thenType == elseType) {
      retType = thenType;
    } else {
      bool promote = false;

      if (canDispatch(elseType, elseSym, thenType, NULL, fn, &promote) &&
          promote == false) {
        retType = thenType;
      } else if (canDispatch(thenType, thenSym, elseType, NULL, fn, &promote) &&
                 promote == false) {
        retType = elseType;
      }
    }

    // For tuples, generally do not allow a tuple to contain a reference
    // when it is returned
    if (retType != NULL && retType->symbol->hasFlag(FLAG_TUPLE) == true) {
      // Compute the tuple type without any refs
      // Set the function return type to that type.
      AggregateType* tupleType = toAggregateType(retType);

      INT_ASSERT(tupleType);

      retType = getReturnedTupleType(fn, tupleType);
    }


    if (thenTypeVar != elseTypeVar) {
      USR_FATAL_CONT(stmt, "if-expression returns mixture of types and values");
      const char* thenKind = thenTypeVar ? "type" : "value";
      const char* elseKind = elseTypeVar ? "type" : "value";
      USR_PRINT("'then' branch returns a %s, but 'else' branch returns a %s", thenKind, elseKind);
      USR_STOP();
    } else if (retType == NULL || (thenTypeVar && thenType != elseType)) {
      USR_FATAL_CONT(stmt, "Unable to resolve type of if-expression");
      if (thenTypeVar || elseTypeVar) {
        USR_PRINT("if-expression returns type variables, did you mean to use a param conditional?");
      }
      USR_PRINT("'then' branch returns type \"%s\"", thenType->symbol->name);
      USR_PRINT("'else' branch returns type \"%s\"", elseType->symbol->name);
      USR_STOP();
    } else {
      ret->type = retType;
    }
  }

  // Now that we know the type, we can fully post-fold the final MOVEs. This is
  // important in the case in which an if-expr returns a slice. postFold will
  // remove FLAG_EXPR_TEMP and allow that slice to be modified.
  postFold(stmt->thenStmt->body.tail);
  postFold(stmt->elseStmt->body.tail);

  // resolveFunction() will insert casts later
}

// Resolves an inferred return type.
// resolveSpecifiedReturnType handles the case that the type is
// specified explicitly.
void resolveReturnTypeAndYieldedType(FnSymbol* fn, Type** yieldedType) {

  bool isIterator = fn->isIterator(); // TODO - do we need || fn->iteratorInfo != NULL;
  Symbol* ret     = fn->getReturnSymbol();
  Type*   retType = ret->type;

  if (isIterator) {
    // For iterators, the return symbol / return type is void
    // or the iterator record. Here we want to compute the yielded
    // type.
    ret = NULL;
    retType = dtUnknown;
  }

  Vec<Type*>   retTypes;
  Vec<Symbol*> retSymbols;

  if (isUnresolvedOrGenericReturnType(retType)) {

    computeReturnTypeParamVectors(fn, ret, retTypes, retSymbols);

    if (retTypes.n == 1) {
      retType = retTypes.head();

    } else if (retTypes.n > 1) {
      // adjust the reference level:
      //  if they are all references, leave it that way
      //  otherwise make them all values.
      bool allRef   = true;
      bool allValue = true;

      for (int i = 0; i < retTypes.n; i++) {
        if (isReferenceType(retTypes.v[i])) {
          allValue = false;
        } else {
          allRef = false;
        }
      }

      // If there is a mix, adjust the return types to be values.
      if (!allValue && !allRef) {
        for (int i = 0; i < retTypes.n; i++) {
          retTypes.v[i] = retTypes.v[i]->getValType();
        }
      }

      for (int i = 0; i < retTypes.n; i++) {
        bool best = true;

        for (int j = 0; j < retTypes.n; j++) {
          if (retTypes.v[i] != retTypes.v[j]) {
            bool requireScalarPromotion = false;

            if (canDispatch(retTypes.v[j],
                            retSymbols.v[j],
                            retTypes.v[i],
                            NULL,
                            fn,
                            &requireScalarPromotion) == false) {
              best = false;
            }

            if (requireScalarPromotion) {
              best = false;
            }
          }
        }

        if (best) {
          retType = retTypes.v[i];
          break;
        }
      }
    }

    if (!fn->iteratorInfo) {
      if (retTypes.n == 0) {
        if (isIterator) {
          // This feels like it should be:
          // retType = dtVoid;
          //
          // but that leads to compiler generated assignments of 'void' to
          // variables, which isn't allowed.  If we fib and claim that it
          // returns 'nothing', those assignments get removed and all is well.
          retType = dtNothing;
        } else {
          retType = dtVoid;
        }
      }
    }

  }

  if (isIterator == false) {
    ret->type = retType;

    if (retType == dtUnknown) {
      USR_FATAL(fn, "unable to resolve return type");
    }

    fn->retType = retType;

  } else {

    // Update the yielded type argument if it was requested
    if (yieldedType != NULL)
      *yieldedType = retType;

    // Update the types of the yielded symbols if they have FLAG_YVV
    if (isIterator) {
      forv_Vec(Symbol, yieldedSym, retSymbols) {
        if (yieldedSym->hasFlag(FLAG_YVV))
          yieldedSym->type = retType;
      }
    }
  }
}

void resolveReturnType(FnSymbol* fn) {
  return resolveReturnTypeAndYieldedType(fn, NULL);
}


static void computeReturnTypeParamVectors(BaseAST*      ast,
                                          Symbol*       retSymbol,
                                          Vec<Type*>&   retTypes,
                                          Vec<Symbol*>& retSymbols) {
  if (CallExpr* call = toCallExpr(ast)) {
    Expr* returnedExpr = NULL;
    if (retSymbol != NULL && call->isPrimitive(PRIM_MOVE)) {
      if (SymExpr* sym = toSymExpr(call->get(1))) {
        if (sym->symbol() == retSymbol)
          returnedExpr = call->get(2);
      }
    } else if(retSymbol == NULL && call->isPrimitive(PRIM_YIELD)) {
      returnedExpr = call->get(1);
    }

    if (returnedExpr != NULL) {
      if (SymExpr* se = toSymExpr(returnedExpr)) {
        retSymbols.add(se->symbol());
      } else {
        retSymbols.add(NULL);
      }

      retTypes.add(returnedExpr->typeInfo());
    }
  }

  // Only go in to nested functions if they are task functions.
  // Otherwise, we'll get confused by yields in inner functions.
  if (DefExpr* def = toDefExpr(ast)) {
    if (FnSymbol* innerFn = toFnSymbol(def->sym)) {
      if (!isTaskFun(innerFn))
        return;
    }
  }

  AST_CHILDREN_CALL(ast,
                    computeReturnTypeParamVectors,
                    retSymbol,
                    retTypes,
                    retSymbols);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

Type* getReturnedTupleType(FnSymbol* fn, AggregateType* retType) {
  Type* retval = NULL;

  INT_ASSERT(retType->symbol->hasFlag(FLAG_TUPLE));

  if (fn->returnsRefOrConstRef() == true) {
    retval = computeTupleWithIntent(INTENT_REF, retType);

  } else {
    // Compute the tuple type without any refs
    retval = computeNonRefTuple(retType);
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/


static bool backendRequiresCopyForIn(Type* t);

static void addLocalCopiesAndWritebacks(FnSymbol*  fn,
                                        SymbolMap& formals2vars);

static bool typeHasRefField(Type* type);

static bool hasRefField(Type* type);

void insertFormalTemps(FnSymbol* fn) {
  SymbolMap formals2vars;

  for_formals(formal, fn) {
    if (formalRequiresTemp(formal, fn)) {
      SET_LINENO(formal);

      VarSymbol* tmp = newTemp(astr("_formal_tmp_", formal->name));

      if (formal->hasFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT))
        tmp->addFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT);
      if (formal->hasFlag(FLAG_NO_AUTO_DESTROY))
        tmp->addFlag(FLAG_NO_AUTO_DESTROY);

      formals2vars.put(formal, tmp);
    }
  }

  if (formals2vars.n > 0) {
    // The names of formals in the body of this function are replaced
    // with the names of their corresponding local temporaries.
    update_symbols(fn->body, &formals2vars);

    // Add calls to chpl__initCopy to create local copies as necessary.
    // Add writeback code for out and inout intents.
    addLocalCopiesAndWritebacks(fn, formals2vars);
  }
}

// Returns true if the formal needs an internal temporary, false otherwise.
// See also ArgSymbol::requiresCPtr
bool formalRequiresTemp(ArgSymbol* formal, FnSymbol* fn) {
  return
    //
    // 'out' and 'inout' intents are passed by ref at the C level, so we
    // need to make an explicit copy in the codegen'd function */
    //
    (formal->intent == INTENT_OUT ||
     formal->intent == INTENT_INOUT ||
     //
     // 'in' and 'const in' also require a copy, but for simple types
     // (like ints or class references), we can rely on C's copy when
     // passing the argument, as long as the routine is not
     // inlined or an iterator.
     //
     ((formal->intent == INTENT_IN || formal->intent == INTENT_CONST_IN) &&
      (backendRequiresCopyForIn(formal->type) ||
       fn->hasFlag(FLAG_INLINE) ||
       fn->hasFlag(FLAG_ITERATOR_FN)))
    );
}

bool shouldAddInFormalTempAtCallSite(ArgSymbol* formal, FnSymbol* fn) {

  // Don't add copies at call site if function body will be removed anyway.
  // TODO: handle RET_TYPE but not for runtime types
  if (fn && fn->retTag == RET_PARAM)
    return false;

  // TODO: remove this filtering on records/unions
  if (isRecord(formal->getValType()) || isUnion(formal->getValType())) {
    if (formal->intent == INTENT_IN ||
        formal->intent == INTENT_CONST_IN ||
        formal->originalIntent == INTENT_IN ||
        formal->originalIntent == INTENT_CONST_IN)
      return true;
  }

  return false;
}

//
// Can Chapel rely on the compiler's back end (e.g.,
// C) to provide the copy for us for 'in' or 'const in' intents when
// passing an argument of type 't'.
//
static bool backendRequiresCopyForIn(Type* t) {
  return (isRecord(t) == true && !t->symbol->hasFlag(FLAG_RANGE)) ||
         isUnion(t)                      == true ||
         t->symbol->hasFlag(FLAG_ARRAY)  == true ||
         t->symbol->hasFlag(FLAG_DOMAIN) == true;
}


// Given the map from formals to local "_formal_tmp_" variables, this function
// adds code as necessary
//  - to copy the formal into the temporary at the start of the function
//  - and copy it back when done.
// The copy in is needed for "inout", "in" and "const in" intents.
// The copy out is needed for "inout" and "out" intents.
// Blank intent is treated like "const", and normally copies the formal through
// chpl__autoCopy.
// Note that autoCopy is called in this case, but not for "inout", "in" and
// "const in".
// Either record-wrapped types are always passed by ref, or some unexpected
// behavior will result by applying "in" intents to them.
static void addLocalCopiesAndWritebacks(FnSymbol*  fn,
                                        SymbolMap& formals2vars) {
  // Enumerate the formals that have local temps.
  form_Map(SymbolMapElem, e, formals2vars) {
    ArgSymbol* formal = toArgSymbol(e->key); // Get the formal.
    Symbol*    tmp    = e->value;            // Get the temp.

    SET_LINENO(formal);

    // TODO: Move this closer to the location (in code) where we determine
    // whether tmp owns its value or not.  That is, push setting these flags
    // (or not) into the cases below, as appropriate.
    Type* formalType = formal->type->getValType();

    // mark CONST as needed
    if (concreteIntent(formal->intent, formalType) & INTENT_FLAG_CONST) {
      tmp->addFlag(FLAG_CONST);

      if (isRefCountedType(formalType) == false) {
        tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
      }
    }

    // This switch adds the extra code inside the current function necessary
    // to implement the ref-to-value semantics, where needed.
    switch (formal->intent)
    {
      // These cases are weeded out by formalRequiresTemp() above.
     case INTENT_PARAM:
     case INTENT_TYPE:
     case INTENT_REF:
     case INTENT_CONST_REF:
     case INTENT_REF_MAYBE_CONST:
      INT_FATAL("Unexpected INTENT case.");
      break;

     case INTENT_OUT: {
      BlockStmt* defaultExpr = NULL;
      BlockStmt* typeExpr = NULL;

      if (formal->defaultExpr &&
          formal->defaultExpr->body.tail->typeInfo() != dtTypeDefaultToken) {
        defaultExpr = formal->defaultExpr->copy();
      }

      if (formalType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
        VarSymbol* typeTmp = NULL;

        if (formal->typeExpr != NULL) {
          typeTmp = newTemp("_formal_type_tmp_");
          typeTmp->addFlag(FLAG_MAYBE_TYPE);
          typeExpr = formal->typeExpr->copy();
        }

        if (defaultExpr != NULL) {
          CallExpr* init = new CallExpr(PRIM_INIT_VAR, tmp,
                                        defaultExpr->body.tail->remove());
          if (typeTmp != NULL)
            init->insertAtTail(new SymExpr(typeTmp));
          fn->insertAtHead(init);
          fn->insertAtHead(defaultExpr);
        } else {
          CallExpr* init = new CallExpr(PRIM_DEFAULT_INIT_VAR, tmp);
          if (typeTmp != NULL) {
            init->insertAtTail(new SymExpr(typeTmp));
          } else {
            // find the FLAG_TYPE_FORMAL_FOR_OUT formal just before this one
            // and set typeTmp to that.
            DefExpr* beforeDef = toDefExpr(formal->defPoint->prev);
            INT_ASSERT(beforeDef != NULL);
            ArgSymbol* typeFormal = toArgSymbol(beforeDef->sym);
            INT_ASSERT(typeFormal != NULL);
            init->insertAtTail(new SymExpr(typeFormal));
          }
          fn->insertAtHead(new CallExpr(PRIM_END_OF_STATEMENT));
          fn->insertAtHead(init);
        }

        // Copy the type expr if present
        if (typeExpr != NULL) {
          CallExpr* setType = new CallExpr(PRIM_MOVE,
                                           typeTmp,
                                           typeExpr->body.tail->remove());
          fn->insertAtHead(setType);
          fn->insertAtHead(typeExpr);
          typeExpr->flattenAndRemove();
          fn->insertAtHead(new DefExpr(typeTmp));
        }

      } else {

        if (defaultExpr != NULL) {
          CallExpr* init = new CallExpr(PRIM_INIT_VAR, tmp,
                                        defaultExpr->body.tail->remove(),
                                        formalType->symbol);
          fn->insertAtHead(init);
          fn->insertAtHead(defaultExpr);
        } else {
          CallExpr * init = new CallExpr(PRIM_DEFAULT_INIT_VAR, tmp,
                                         formalType->symbol);
          fn->insertAtHead(new CallExpr(PRIM_END_OF_STATEMENT));
          fn->insertAtHead(init);
        }
      }

      tmp->addFlag(FLAG_FORMAL_TEMP);
      tmp->addFlag(FLAG_FORMAL_TEMP_OUT);
      tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
      break;
     }
     case INTENT_INOUT:
      fn->insertAtHead(new CallExpr(PRIM_MOVE,
                                    tmp,
                                    new CallExpr("chpl__initCopy", formal)));

      tmp->addFlag(FLAG_FORMAL_TEMP);
      tmp->addFlag(FLAG_FORMAL_TEMP_INOUT);
      tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
      break;

     case INTENT_IN:
     case INTENT_CONST_IN:
      if (!shouldAddInFormalTempAtCallSite(formal, fn)) {
        fn->insertAtHead(new CallExpr(PRIM_MOVE,
                                      tmp,
                                      new CallExpr("chpl__initCopy", formal)));

        tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
      } else {
        // move from in intent argument to local variable to be destroyed
        // (The local variable is not strictly necessary but is a more
        //  typical pattern for follow-on passes)
        tmp->addFlag(FLAG_NO_COPY);
        fn->insertAtHead(new CallExpr(PRIM_MOVE, tmp, formal));

        // Default-initializers and '_new' wrappers take ownership
        // Note: FLAG_INSERT_AUTO_DESTROY is blindly applied to any formal
        // with const-in intent at the start of this function, so we need
        // to apply FLAG_NO_AUTO_DESTROY to avoid double-frees.
        if (fn->hasFlag(FLAG_NEW_WRAPPER) || fn->isDefaultInit()) {
          tmp->addFlag(FLAG_NO_AUTO_DESTROY);
        } else {
          tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
        }
      }
      break;

     case INTENT_BLANK:
     case INTENT_CONST:
     {
       TypeSymbol* ts = formal->type->symbol;

       if (!getRecordWrappedFlags(ts).any()   &&
           !ts->hasFlag(FLAG_ITERATOR_CLASS)  &&
           !ts->hasFlag(FLAG_ITERATOR_RECORD)) {
         if (fn->hasFlag(FLAG_BEGIN)) {
           // autoCopy/autoDestroy will be added later, in parallel pass
           // by insertAutoCopyDestroyForTaskArg()
           fn->insertAtHead(new CallExpr(PRIM_MOVE, tmp, formal));
           tmp->removeFlag(FLAG_INSERT_AUTO_DESTROY);

         } else {
           // Note that because we reject the case of record-wrapped types
           // above, the only way we can get a formal whose call to
           // chpl__autoCopy does anything different from calling
           // chpl__initCopy is if the formal is a tuple containing a
           // record-wrapped type.

           // This is probably not intentional: It gives tuple-wrapped
           // record-wrapped types different behavior from bare record-wrapped
           // types.
           fn->insertAtHead(new CallExpr(PRIM_MOVE,
                                         tmp,
                                         new CallExpr("chpl__autoCopy",
                                                      formal)));

           // WORKAROUND:
           // This is a temporary bug fix that results in leaked memory.
           //
           // Here we avoid calling the destructor for any formals that
           //  are records or have records because the call may result
           //  in repeatedly freeing memory if the user defined
           //  destructor calls delete on any fields.  I think we
           //  probably need a similar change in the INOUT/IN case
           //  above.  See test/types/records/sungeun/destructor3.chpl
           //  and test/users/ferguson/recordbug3.chpl.
           //
           // For records, this problem should go away if/when we
           //  implement 'const ref' intents and make them the default
           //  for records.
           //
           // Another solution (and the one that would fix records in
           //  classes) is to call the user record's default
           //  constructor if it takes no arguments.  This is not the
           //  currently described behavior in the spec.  This would
           //  require the user to implement a default constructor if
           //  explicit memory allocation is required.
           //
           if (!isAggregateType(formal->type) ||
               (isRecord(formal->type) &&
                ((formal->type->getModule()->modTag==MOD_INTERNAL) ||
                 (formal->type->getModule()->modTag==MOD_STANDARD))) ||
               !typeHasRefField(formal->type))
             tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
         }

       } else {
         fn->insertAtHead(new CallExpr(PRIM_MOVE, tmp, formal));
         // If this is a simple move, then we did not call chpl__autoCopy to
         // create tmp, so then it is a bad idea to insert a call to
         // chpl__autodestroy later.
         tmp->removeFlag(FLAG_INSERT_AUTO_DESTROY);
       }

       break;
     }
    }

    fn->insertAtHead(new DefExpr(tmp));

    // For inout or out intent, this assigns the modified value back to the
    // formal at the end of the function body.
    if (formal->intent == INTENT_INOUT) {
      fn->insertIntoEpilogue(new CallExpr("=", formal, tmp));
    } else if (formal->intent == INTENT_OUT) {
      fn->insertIntoEpilogue(new CallExpr(PRIM_ASSIGN, formal, tmp));
    }
  }
}

static bool typeHasRefField(Type* type) {
  bool retval = false;

  if (AggregateType* ct = toAggregateType(type)) {
    for_fields(field, ct) {
      if (hasRefField(field->typeInfo())) {
        retval = true;
        break;
      }
    }
  }

  return retval;
}

static bool hasRefField(Type* type) {
  bool retval = false;

  if (isPrimitiveType(type)) {
    retval = false;

  } else if (type->symbol->hasFlag(FLAG_OBJECT_CLASS)) {
    retval = false;

  } else if (isClass(type) == false) { // record or union
    if (AggregateType* ct = toAggregateType(type)) {
      for_fields(field, ct) {
        if (hasRefField(field->type)) {
          retval = true;
          break;
        }
      }
    }

  } else {
    retval = true;
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void insertCasts(BaseAST* ast, FnSymbol* fn, Vec<CallExpr*>& casts);

static bool insertAndResolveCasts(FnSymbol* fn) {
  bool changed = false;

  if (fn->retTag != RET_PARAM) {
    Vec<CallExpr*> casts;

    insertCasts(fn->body, fn, casts);
    changed = casts.size() > 0;

    forv_Vec(CallExpr, cast, casts) {
      resolveCallAndCallee(cast, true);
    }
  }

  return changed;
}

static void insertCasts(BaseAST* ast, FnSymbol* fn, Vec<CallExpr*>& casts) {
  if (isSymbol(ast) && ! isShadowVarSymbol(ast))
    return; // do not descend into nested symbols

  if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIM_MOVE)) {
        if (SymExpr* lhs = toSymExpr(call->get(1))) {
          Type* lhsType = lhs->symbol()->type;

          if (lhsType != dtUnknown) {
            Expr*     rhs     = call->get(2);
            Type*     rhsType = rhs->typeInfo();
            CallExpr* rhsCall = toCallExpr(rhs);

            if (call->id == breakOnResolveID)
              gdbShouldBreakHere();

            if (rhsCall && rhsCall->isPrimitive(PRIM_COERCE)) {
              rhsType = rhsCall->get(1)->typeInfo();
            }

            // would this be simpler with getValType?
            bool typesDiffer = (rhsType          != lhsType &&
                                rhsType->refType != lhsType &&
                                rhsType          != lhsType->refType);
            bool isTypeOf = rhsCall && rhsCall->isPrimitive(PRIM_TYPEOF);

            SET_LINENO(rhs);

            // Generally, we want to add casts for PRIM_MOVE that have two
            // different types. This function also handles PRIM_COERCE on the
            // right-hand side by either removing the PRIM_COERCE entirely if
            // the types are the same, or by using a = call if the types are
            // different. It could use a _cast call if the types are different,
            // but the = call works better in cases where an array is returned.

            if (rhsCall && rhsCall->isPrimitive(PRIM_COERCE)) {
              // handle move lhs, coerce rhs
              SymExpr* fromExpr     = toSymExpr(rhsCall->get(1));
              SymExpr* fromTypeExpr = toSymExpr(rhsCall->get(2));

              Symbol*  from         = fromExpr->symbol();
              Symbol*  fromType     = fromTypeExpr->symbol();

              Symbol*  to           = lhs->symbol();


              bool involvesRuntimeType = false;
              {
                Type* t1 = fromTypeExpr->getValType();
                Type* t2 = fromExpr->getValType();

                involvesRuntimeType =
                  t1->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE) ||
                  t2->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE);
              }

              bool useAssign = involvesRuntimeType;

              // Use assign (to get error) if coercion isn't normally
              // allowed between these types.
              // Note this also supports some things like syserr = int.
              if (!canDispatch(from->type, from, lhsType)) {
                useAssign = true;
              }

              // Use assign since no cast is available for
              // sync / single and their value type.
              if ((isSyncType(from->getValType()) ||
                   isSingleType(from->getValType()))) {
                useAssign = true;
              }

              // Check that lhsType == the result of coercion
              INT_ASSERT(lhsType == rhsCall->typeInfo());

              // If the types are the same but runtime types
              // are involved, we don't know that the runtime
              // types are the same until runtime (at least without
              // some better smarts in the compiler).

              if (!typesDiffer && !useAssign) {
                // types are the same. remove coerce and
                // handle reference level adjustments. No cast necessary.

                CallExpr* toResolve = NULL;

                if (rhsType == lhsType) {
                  rhs = new SymExpr(from);

                } else if (rhsType == lhsType->refType) {
                  toResolve = new CallExpr("chpl__autoCopy", new SymExpr(from));
                  rhs = toResolve;

                } else if (rhsType->refType == lhsType) {
                  rhs = new CallExpr(PRIM_ADDR_OF, new SymExpr(from));
                }

                CallExpr* move = new CallExpr(PRIM_MOVE, to, rhs);

                call->replace(move);

                if (toResolve) resolveExpr(toResolve);

                casts.add(move);

              } else if (useAssign) {

                // Here the types differ and we're expecting some
                // kind of promotion / iterator-array-conversion to apply.

                // In the future, it would be nice if this could no-init
                // a LHS array and then move records into it from the RHS.

                // Tell compiler it shouldn't raise errors connected
                // to default-initializing to since it is actually
                // set below.
                to->addFlag(FLAG_INITIALIZED_LATER);

                CallExpr* init = new CallExpr(PRIM_DEFAULT_INIT_VAR,
                                              to, fromType);
                call->insertBefore(init);

                // Since the initialization pattern normally does not
                // require adding an auto-destroy for a call-expr-temp,
                // add FLAG_INSERT_AUTO_DESTROY since we're assigning from
                // it.
                from->addFlag(FLAG_INSERT_AUTO_DESTROY);

                CallExpr* assign = new CallExpr("=", to, from);

                call->insertBefore(assign);

                // Resolve each of the new CallExprs They need to be resolved
                // separately since resolveExpr does not recurse.
                resolveExpr(init);
                resolveExpr(assign);

                // Enable error messages assignment between local
                // and distributed domains. It would be better if this
                // could be handled by some flavor of initializer.
                CallExpr* check = new CallExpr("chpl_checkCopyInit", to, from);
                call->insertBefore(check);
                resolveExpr(check);

                // We've replaced the move with no-init/assign, so remove it.
                call->remove();

              } else {
                // Add a cast if the types don't match

                // Remove the right-hand-side, which is call->get(2)
                // The code below assumes it is the final argument
                rhs->remove();

                Symbol* tmp = NULL;

                if (SymExpr* se = toSymExpr(fromExpr)) {
                  tmp = se->symbol();

                } else {
                  tmp = newTemp("_cast_tmp_", fromExpr->typeInfo());

                  call->insertBefore(new DefExpr(tmp));
                  call->insertBefore(new CallExpr(PRIM_MOVE, tmp, fromExpr));
                }

                // see comment about this above in assignment case
                from->addFlag(FLAG_INSERT_AUTO_DESTROY);

                CallExpr* cast = createCast(tmp, lhsType->symbol);

                call->insertAtTail(cast);
                casts.add(cast);
              }

            } else if (!isTypeOf) {
              // handle adding casts for a regular PRIM_MOVE

              if (typesDiffer) {

                // Remove the right-hand-side, which is call->get(2)
                // The code below assumes it is the final argument
                rhs->remove();

                // Add a cast if the types don't match
                Symbol* tmp = NULL;

                if (SymExpr* se = toSymExpr(rhs)) {
                  tmp = se->symbol();

                } else {
                  tmp = newTemp("_cast_tmp_", rhs->typeInfo());
                  call->insertBefore(new DefExpr(tmp));
                  call->insertBefore(new CallExpr(PRIM_MOVE,
                                                  tmp,
                                                  rhs->copy()));
                }

                if (lhsType->symbol->hasFlag(FLAG_TUPLE) &&
                    lhs->symbol()->hasFlag(FLAG_RVV)) {
                  // When returning tuples, we might return a
                  // tuple containing a ref, while the return type
                  // is the tuple with no refs. This code adjusts
                  // the AST to compensate.
                  CallExpr* unref = new CallExpr("chpl__unref", tmp);
                  call->insertAtTail(unref);
                  resolveExpr(unref);

                } else if (lhsType->getValType() == dtStringC &&
                           tmp->getValType() == dtString &&
                           tmp->isImmediate()) {
                  // Coercion from a param string to a c_string
                  // for the non-param case it would require .c_str()
                  // and as a result the regular cast function is not available.
                  VarSymbol*  var       = toVarSymbol(tmp);
                  const char* str       = var->immediate->v_string;
                  SymExpr*    newActual = new SymExpr(new_CStringSymbol(str));

                  // Remove the right-hand-side, which is call->get(2)
                  call->insertAtTail(newActual);

                } else {

                  CallExpr* cast = createCast(tmp, lhsType->symbol);
                  call->insertAtTail(cast);
                  casts.add(cast);
                }

              } else {
                // types are the same.
                // handle reference level adjustments. No cast necessary.

                if (rhsType == lhsType->refType) {
                  lhs->remove();
                  rhs->remove();

                  Expr* newRHS = NULL;
                  if (isCallExpr(rhs)) {
                    VarSymbol* tmp = newTemp(rhs->typeInfo());
                    DefExpr* def = new DefExpr(tmp);
                    call->insertBefore(def);
                    call->insertBefore(new CallExpr(PRIM_MOVE, tmp, rhs));
                    newRHS = new SymExpr(tmp);
                  } else {
                    newRHS = rhs;
                  }

                  CallExpr* move = new CallExpr(PRIM_MOVE,
                                                lhs,
                                                new CallExpr(PRIM_DEREF, newRHS));

                  call->replace(move);

                  casts.add(move);
                }
              }
            }
          }
        }
      }
  }

  AST_CHILDREN_CALL(ast, insertCasts, fn, casts);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void ensureInMethodList(FnSymbol* fn) {
  if (fn->isMethod() == true && fn->_this != NULL) {
    Type* thisType = fn->_this->type->getValType();
    bool  found    = false;

    INT_ASSERT(thisType);
    INT_ASSERT(thisType != dtUnknown);

    forv_Vec(FnSymbol, method, thisType->methods) {
      if (method == fn) {
        found = true;
        break;
      }
    }

    if (found == false) {
      thisType->methods.add(fn);
    }
  }
}

