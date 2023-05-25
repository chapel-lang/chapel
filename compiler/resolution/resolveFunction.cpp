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

#include "resolveFunction.h"

#include "astutil.h"
#include "AstVisitorTraverse.h"
#include "caches.h"
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
#include "TemporaryConversionThunk.h"
#include "TryStmt.h"
#include "view.h"
#include "WhileStmt.h"
#include "llvm/ADT/DenseSet.h"

#include <set>

std::map<ArgSymbol*, std::string> exportedDefaultValues;

struct ConversionsTableValue {
  FnSymbol* assign;
  FnSymbol* initEq;
  FnSymbol* cast;
  ConversionsTableValue() : assign(NULL), initEq(NULL), cast(NULL) { }
};

typedef std::pair<Type*,Type*> ConversionsTableKey;
typedef std::map<ConversionsTableKey, ConversionsTableValue> ConversionsTable;

static ConversionsTable conversionsTable;

static void resolveFormals(FnSymbol* fn);

static void markIteratorAndLoops(FnSymbol* fn);

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
  static llvm::DenseSet<FnSymbol*> done;

  // This gets called about 10M times in arkouda and
  // resolve goes from 30s to 28s
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

    // If the formal has void type, and its parent symbol is not compiler-
    // generated, then it is an invalid 'void' source-code formal.
    //
    // If the formal has void type, and its parent symbol is compiler-
    // generated, and the parent symbol is astrInit, then it is an aggregate
    // field initialization, such as new c(); and the field cannot be void.
    if (formal->type == dtVoid && !formal->hasFlag(FLAG_TYPE_VARIABLE)) {
      Symbol* ps = formal->defPoint->parentSymbol;
      if (ps) {
        if (!ps->hasFlag(FLAG_COMPILER_GENERATED))
          USR_FATAL(formal,
                    "Formal '%s' cannot be declared 'void'."
                    " Consider using 'nothing' instead.",
                    formal->name);
        else if (ps->name == astrInit)
          USR_FATAL(formal,
                    "Field '%s' cannot be declared 'void'."
                    " Consider using 'nothing' instead.",
                    formal->name);
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
//   test/studies/ssca2/main/SSCA2_main.chpl
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
    fn->cname = astr(var->immediate->v_string.c_str());
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

  // resolve specified return types for any 'out' intent formals
  for_formals(formal, fn) {
    if (formal->originalIntent == INTENT_OUT) {
      if (formal->type == dtUnknown && formal->typeExpr != NULL) {
        resolveBlockStmt(formal->typeExpr);
        formal->type = formal->typeExpr->body.tail->getValType();
      }
    }
  }

  // Stop if there is nothing else to do.
  if (!fn->retExprType || fn->retType != dtUnknown) return;

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

    } else if (fn->returnsRefOrConstRef() &&
               ! retType->symbol->hasFlag(FLAG_GENERIC)) {
      // makeRefType is no-op if retType is generic?
      makeRefType(retType);

      retType     = retType->refType;
      fn->retType = retType;

    } else {
      fn->retType = retType;
    }

    if (fn->isIterator() && fn->iteratorInfo == NULL &&
        ! retType->symbol->hasFlag(FLAG_GENERIC)) {
      // Note: protoIteratorClass changes fn->retType to the iterator record.
      // The original return type is stored here in retType.
      protoIteratorClass(fn, retType);
    }
    // Note: if not here, we will protoIteratorClass()
    // in resolveFunction() after resolveReturnTypeAndYieldedType().

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
static void resolveAlsoConversions(FnSymbol* fn, CallExpr* forCall);

void resolveFunction(FnSymbol* fn, CallExpr* forCall) {
  if (! fn->isResolved() && ! fn->hasFlag(FLAG_CG_INTERIM_INST)) {
    if (fn->id == breakOnResolveID) {
      printf("breaking on resolve fn %s[%d] (%d args)\n",
             fn->name, fn->id, fn->numFormals());
      gdbShouldBreakHere();
    }

    fn->addFlag(FLAG_RESOLVED);

    fn->tagIfGeneric();

    createCacheInfoIfNeeded(fn);

    if (strcmp(fn->name, "init") == 0 && fn->isMethod()) {
      AggregateType* at = toAggregateType(fn->_this->getValType());
      if (at->symbol->hasFlag(FLAG_GENERIC) == false) {
        resolveTypeWithInitializer(at, fn);
      }
    }

    if (fn->hasFlag(FLAG_NO_FN_BODY) || fn->hasFlag(FLAG_EXTERN)) {
      // TODO: Should be empty, can we just remove this?
      resolveBlockStmt(fn->body);


      if (fn->retExprType) {
        resolveSpecifiedReturnType(fn);

      // If there is no body and no return type, the function returns void.
      } else {
        INT_ASSERT(fn->retType == dtVoid || fn->retType == dtUnknown);
        fn->retType = dtVoid;
      }

    } else {
      if (fn->isIterator() == true) {
        markIteratorAndLoops(fn);
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
        resolveAlsoConversions(fn, forCall);
      }

      if (fn->hasFlag(FLAG_RUNTIME_TYPE_INIT_FN))
        adjustRuntimeTypeInitFn(fn);

      markTypesWithDefaultInitEqOrAssign(fn);
    }
    popInstantiationLimit(fn);
    clearCacheInfoIfEmpty(fn);
  }
}

static void markTypesWithDefaultInitEqOrAssign(FnSymbol* fn) {

  if (fn->name == astrSassign &&
      fn->numFormals() >= 1) {
    int lhsNum = 1;
    if (fn->getFormal(1)->typeInfo() == dtMethodToken) {
      // Operator methods have a "_this" and method token argument, but
      // standalone operators do not.  Need to consider both here.
      lhsNum += 2;
    }
    ArgSymbol* lhs = fn->getFormal(lhsNum);
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

static void resolveAlsoConversions(FnSymbol* fn, CallExpr* forCall) {

  Type* toType = NULL;
  Type* fromType = NULL;

  if (fn->name == astrSassign) {
    int i = 1;
    if (fn->getFormal(i)->typeInfo() == dtMethodToken) i++;
    if (fn->getFormal(i)->hasFlag(FLAG_ARG_THIS)) i++;
    toType = fn->getFormal(i)->getValType(); i++;
    fromType = fn->getFormal(i)->getValType();
  } else if (fn->name == astrInitEquals) {
    // arg 1 is the method token
    toType = fn->getFormal(2)->getValType();
    fromType = fn->getFormal(3)->getValType();
  } else {
    // Nothing to do if it's not one of the above cases.
    return;
  }

  // Don't worry about generic types at this point.
  if (fromType->symbol->hasFlag(FLAG_GENERIC) ||
      toType->symbol->hasFlag(FLAG_GENERIC))
    return;

  ConversionsTableValue have;

  // Add whatever we have now to the table
  {
    ConversionsTableKey key(fromType, toType);
    ConversionsTableValue& val = conversionsTable[key]; // adds entry if needed

    if (fn->name == astrSassign)
      val.assign = fn;
    else if (fn->name == astrInitEquals)
      val.initEq = fn;
    else
      INT_FATAL("unexpected case");

    // save a copy of the entry
    have = val;
  }

  // resolve also the implied operations if they are not already present.

  // these are the relationships among implicit convert, assign, init=, cast:
  //
  // implicit conversion implies assign, initialize, cast
  // assign implies initialize, cast
  // initialize implies cast
  // cast doesn't imply any of the others

  // decide which to check for
  bool implicitConverts = false;
  bool checkAssign = false;
  bool checkInitEq = false;
  bool checkCast = false;

  if (canCoerce(fromType, NULL, toType, NULL, NULL)) {
    implicitConverts = true;
    checkAssign = true;
    checkInitEq = true;
    checkCast = true;
  }

  if (fn->name == astrSassign || have.assign != NULL) {
    checkInitEq = true;
    checkCast = true;
  }

  if (fn->name == astrInitEquals || have.initEq != NULL) {
    checkCast = true;
  }

  // Don't worry about checking for casts between the same type
  // since such casts are moot and would be removed anyway.
  if (toType == fromType)
    checkCast = false;

  // Don't worry about checking for casts among class types
  // (these should be handled by the internal code)
  if (isClassLikeOrPtr(toType) ||
      isClassLikeOrManaged(toType) ||
      toType == dtNil) {
    checkAssign = false;
    checkInitEq = false;
    checkCast = false;

    // However, don't allow '=' on these types to be defined
    // outside of the standard/internal modules
    if (have.assign != NULL &&
        have.assign->defPoint->getModule()->modTag == MOD_USER) {
      USR_FATAL_CONT(have.assign->defPoint,
                     "Can't overload assignments for class types");
    }
  }

  // Don't worry about checking for 'init=' for tuples or types
  // with runtime type since these do not use 'init=' in the usual way.
  if (toType->symbol->hasFlag(FLAG_TUPLE) ||
      toType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
    checkInitEq = false;
    // However, don't allow '=' on these types to be defined
    // outside of the standard/internal modules
    if (have.assign != NULL &&
        have.assign->defPoint->getModule()->modTag == MOD_USER) {
      USR_FATAL_CONT(have.assign->defPoint,
                     "unsupported = overload to '%s' from '%s'",
                     toString(toType), toString(fromType));
      USR_PRINT("assignment overloads setting array or tuples "
                "are not currently supported");
      checkAssign = false;
      checkCast = false;
    }
  }

  if (fWarnUnstable &&
      toType->symbol->hasFlag(FLAG_EXTERN) &&
      have.assign != NULL &&
      !have.assign->hasFlag(FLAG_COMPILER_GENERATED) &&
      have.assign->defPoint->getModule()->modTag == MOD_USER) {
    USR_WARN(have.assign->defPoint,
             "unstable assignment with extern LHS");
    USR_PRINT("an init= may also be required in the future");
  }



  // Don't look for casts to array/domain types
  // (these should probably be added, right?)
  if (toType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
    checkCast = false;
  }

  // Don't look for casts or init= to iterators. These don't make sense.
  if (toType->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
    checkCast = false;
    checkInitEq = false;
  }

  // Since we don't yet have init= on non-records or tertiary records,
  // don't worry about init= yet for non-records or extern types.
  if (!(isRecord(toType) || isUnion(toType)) ||
      toType->symbol->hasFlag(FLAG_EXTERN))
    checkInitEq = false;

  // add calls to resolve these if not already known
  if (checkAssign && have.assign == NULL) {
    // also resolve =
    BlockStmt* block = new BlockStmt(BLOCK_SCOPELESS);
    forCall->insertBefore(block);

    VarSymbol* toTmp = newTemp("_check_to", toType);
    VarSymbol* fromTmp = newTemp("_check_from", fromType);
    CallExpr* c = new CallExpr(astrSassign, toTmp, fromTmp);
    block->insertAtTail(new DefExpr(toTmp));
    block->insertAtTail(new DefExpr(fromTmp));
    block->insertAtTail(c);
    tryResolveCall(c);
    block->remove();
    // update our local variable
    // (above resolve call should have updated the map)
    have.assign = c->resolvedFunction();
  }

  if (checkInitEq && have.initEq == NULL) {
    // also resolve init=
    BlockStmt* block = new BlockStmt(BLOCK_SCOPELESS);
    forCall->insertBefore(block);

    VarSymbol* toTmp = newTemp("_check_to", toType);
    VarSymbol* fromTmp = newTemp("_check_from", fromType);
    CallExpr* c = new CallExpr(astrInitEquals, gMethodToken, toTmp, fromTmp);
    block->insertAtTail(new DefExpr(toTmp));
    block->insertAtTail(new DefExpr(fromTmp));
    block->insertAtTail(c);
    tryResolveCall(c);
    block->remove();
    // update our local variable
    // (above resolve call should have updated the map)
    have.initEq = c->resolvedFunction();
  }

  if (checkCast && have.cast == NULL) {
    // also resolve cast
    BlockStmt* block = new BlockStmt(BLOCK_SCOPELESS);
    forCall->insertBefore(block);

    VarSymbol* fromTmp = newTemp("_check_from", fromType);
    CallExpr* c = new CallExpr(astrScolon, fromTmp, toType->symbol);
    block->insertAtTail(new DefExpr(fromTmp));
    block->insertAtTail(c);
    tryResolveCall(c);
    block->remove();
    // update our local variable
    // (above resolve call should have updated the map)
    have.cast = c->resolvedFunction();
  }

  // Now, error if an implied function was not found.
  bool error = false;
  if (checkAssign && have.assign == NULL) {
    USR_FATAL_CONT(forCall, "an = overload setting '%s' from '%s' is missing",
                            toString(toType), toString(fromType));
    error = true;
  }
  if (checkInitEq && have.initEq == NULL) {
    USR_FATAL_CONT(forCall, "an init= initializing '%s' from '%s' is missing",
                            toString(toType), toString(fromType));
    error = true;
  }
  if (checkCast && have.cast == NULL) {
    USR_FATAL_CONT(forCall, "a cast creating '%s' from '%s' is missing",
                            toString(toType), toString(fromType));
    error = true;
  }

  if (error) {
    if (implicitConverts) {
      USR_PRINT("expected because implicit conversion is available between these two types");
    } else if (have.assign != NULL) {
      USR_PRINT(have.assign->defPoint,
                "expected because assignment is defined here between these two types");
    } else if (have.initEq != NULL) {
      USR_PRINT(have.initEq->defPoint,
                "expected because init= is defined here between these two types");
    }
  }
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool isIteratorOfType(FnSymbol* fn, Symbol* iterTag);
static bool isLoopBodyJustYield(LoopStmt* loop);

static void markIteratorAndLoops(FnSymbol* fn) {
  /* Marks loops in iterators as order-independent:
       * if a pragma says to do so
       * or, if the body of the loop is just a yield
   */
  bool markOrderIndep = fn->hasFlag(FLAG_ORDER_INDEPENDENT_YIELDING_LOOPS);
  bool markAllYieldingLoops = fn->hasFlag(FLAG_PROMOTION_WRAPPER) ||
                              fn->hasFlag(FLAG_VECTORIZE_YIELDING_LOOPS) ||
                              markOrderIndep;

  std::vector<CallExpr*> callExprs;

  collectCallExprs(fn->body, callExprs);

  for_vector(CallExpr, call, callExprs) {
    if (call->isPrimitive(PRIM_YIELD)) {
      if (LoopStmt* loop = LoopStmt::findEnclosingLoop(call)) {
        if (!loop->isCoforallLoop() && !loop->isOrderIndependent()) {
          bool justYield = isLoopBodyJustYield(loop);
          if (justYield || markAllYieldingLoops) {
            loop->orderIndependentSet(true);
          } else {
            if (fReportVectorizedLoops && fExplainVerbose) {
              if (!isLeaderIterator(fn)) {
                ModuleSymbol *mod = toModuleSymbol(fn->getModule());
                if (developer || mod->modTag == MOD_USER) {
                  USR_WARN(loop, "not a foreach loop, will not be vectorized");
                }
              }
            }
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

bool isFollowerIterator(FnSymbol* fn) {
  return isIteratorOfType(fn, gFollowerTag);
}

bool isStandaloneIterator(FnSymbol* fn) {
  return isIteratorOfType(fn, gStandaloneTag);
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

static bool isLoopBodyJustYield(AList body, int& numYields) {
  for_alist(expr, body) {
    if (DefExpr* def = toDefExpr(expr))
      if (def->sym->hasFlag(FLAG_TEMP) ||
          def->sym->hasFlag(FLAG_INDEX_VAR) ||
          isLabelSymbol(def->sym))
        continue;

    if (CallExpr* call = toCallExpr(expr)) {
      // PRIM_MOVE / PRIM_ASSIGN with primitive/symexpr RHS is OK
      // (but not if RHS is a user call)
      if (call->isPrimitive(PRIM_MOVE) ||
          call->isPrimitive(PRIM_ASSIGN)) {
        if (CallExpr* rhsCall = toCallExpr(call->get(2))) {
          if (rhsCall->isPrimitive())
            continue;
        } else {
          continue;
        }
      }
      if (call->isPrimitive(PRIM_YIELD)) {
        numYields++;

        if (CallExpr* yCall = toCallExpr(call->get(1))) {
          if (yCall->isPrimitive())
            continue;
        } else {
          continue;
        }
      }
      if (call->isPrimitive(PRIM_END_OF_STATEMENT))
        continue;
    }

    if (BlockStmt* block = toBlockStmt(expr)) {
      if (block->isRealBlockStmt())
        if (isLoopBodyJustYield(block->body, numYields))
          continue;
    }

    // if we haven't continued above, not a simple just-yield loop.
    return false;
  }

  // If we get here, everything is OK
  return true;
}

static bool isLoopBodyJustYield(LoopStmt* loop) {
  int numYields = 0;
  bool ok = isLoopBodyJustYield(loop->body, numYields);
  return ok && numYields == 1;
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
static void insertUnrefForTupleYields(FnSymbol* fn);

static void insertUnrefForArrayOrTupleReturn(FnSymbol* fn) {
  if (fn->isIterator()) {
    insertUnrefForTupleYields(fn);
    return;
  }

  bool skipArray = doNotUnaliasArray(fn);
  bool skipTuple = doNotChangeTupleTypeRefLevel(fn, true);

  // neither tuple nor array unref is necessary, so return
  if (skipArray && skipTuple) return;
  if (fn->hasFlag(FLAG_NO_FN_BODY)) return;

  Symbol* ret = fn->getReturnSymbol();

  // surely returning neither tuple nor array, so return
  // TODO: the addition of this early return resulted in a substantial reduction
  // of compilation time in some cases. We should investigate how to speed up
  // the remainder of this function.
  if (ret->type != dtUnknown                   &&
      ! ret->type->symbol->hasFlag(FLAG_TUPLE) &&
      ! ret->type->symbol->hasFlag(FLAG_ARRAY) &&
      ! ret->type->symbol->hasFlag(FLAG_DOMAIN) ) return;

  for_SymbolSymExprs(se, ret) {
    if (CallExpr* call = toCallExpr(se->parentExpr)) {
      if (call->isPrimitive(PRIM_MOVE) == true &&
          call->get(1)                 == se) {
        Expr* fromExpr = call->get(2);
        Type* rhsType = fromExpr->typeInfo();

        SymExpr* fromSe = toSymExpr(fromExpr);
        bool domain = rhsType->symbol->hasFlag(FLAG_DOMAIN) &&
                      fromSe != NULL &&
                      isCallExprTemporary(fromSe->symbol()) &&
                      isTemporaryFromNoCopyReturn(fromSe->symbol());
        bool array = rhsType->symbol->hasFlag(FLAG_ARRAY);
        bool arrayIsh = (array ||
                         rhsType->symbol->hasFlag(FLAG_ITERATOR_RECORD));

        bool handleDomain = skipArray == false && domain;
        bool handleArray = skipArray == false && arrayIsh;
        bool handleTuple = skipTuple == false &&
                           isTupleContainingAnyReferences(rhsType);

        if ((handleArray || handleDomain || handleTuple) &&
            !isTypeExpr(fromExpr)) {

          FnSymbol* initCopyFn = getInitCopyDuringResolution(rhsType);
          INT_ASSERT(initCopyFn);

          SET_LINENO(call);
          Expr*      rhs       = fromExpr->remove();
          VarSymbol* tmp       = newTemp("copy_ret_tmp", rhsType);
          CallExpr*  initTmp   = new CallExpr(PRIM_MOVE,     tmp, rhs);
          Symbol* definedConst = gFalse;
          CallExpr*  unrefCall = new CallExpr(initCopyFn, tmp, definedConst);
          CallExpr*  shapeSet  = findSetShape(call, ret);

          // Used by callDestructors to catch assignment from
          // a ref to 'tmp' when we know we don't want to copy.
          tmp->addFlag(FLAG_NO_COPY);

          call->insertBefore(new DefExpr(tmp));
          call->insertBefore(initTmp);

          call->insertAtTail(unrefCall);

          if (array && isAliasingArrayType(rhs->getValType()) == false) {
            // If the function does not have this flag, this must
            // be a non-view array. Remove the unref call.
            unrefCall->replace(rhs->copy());

            tmp->defPoint->remove();

            initTmp->remove();

            INT_ASSERT(unrefCall->inTree() == false);

            if (shapeSet) setIteratorRecordShape(shapeSet);

          } else {
            if (shapeSet) {
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
}

static bool doNotUnaliasArray(FnSymbol* fn) {
  return (fn->hasFlag(FLAG_NO_COPY_RETURN) ||
          fn->hasFlag(FLAG_RUNTIME_TYPE_INIT_FN) ||
          fn->hasFlag(FLAG_NO_COPY_RETURNS_OWNED) ||
          fn->hasFlag(FLAG_INIT_COPY_FN) ||
          fn->hasFlag(FLAG_AUTO_COPY_FN) ||
          fn->hasFlag(FLAG_COERCE_FN) ||
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
      fn->hasFlag(FLAG_COERCE_FN)                || // chpl__coerceCopy/Move
      fn->hasFlag(FLAG_AUTO_DESTROY_FN)          || // tuple chpl__autoDestroy
      fn->hasFlag(FLAG_ALLOW_REF)                || // iteratorIndex
      // don't change ref level for tuple assignment
      (fn->name == astrSassign && fn->hasFlag(FLAG_COMPILER_GENERATED)) ||
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

static void insertUnrefForTupleYields(FnSymbol* fn) {
  // do not unref:
  if (fn->retTag == RET_TYPE       ||  // could iterators yield types?
      fn->retTag == RET_REF        ||  // if yielding refs
      fn->retTag == RET_CONST_REF  ||
      fn->hasFlag(FLAG_DONT_UNREF_FOR_YIELDS) ) // if opted out
    return;

  // todo: return also if fn->retType or fn->iteratorInfo->yieldedType
  // is meaningful, i.e., not dtUnknown and not an iterator record,
  // and is not a tuple: not yielding tuples ==> no need to convert

  std::vector<CallExpr*> callExprs;
  collectCallExprs(fn->body, callExprs);
  for_vector(CallExpr, call, callExprs) {
    if (call->isPrimitive(PRIM_YIELD)) {
      Symbol* yieldSym  = toSymExpr(call->get(1))->symbol();
      Type*   yieldType = yieldSym->type;
      if (! yieldType->symbol->hasFlag(FLAG_TUPLE)   ||
          yieldSym->hasFlag(FLAG_NO_COPY)            ||
          ! isTupleContainingAnyReferences(yieldType) )
        continue;

      // follow insertUnrefForArrayOrTupleReturn()
      // yield rhs; --> move(tmp, initCopyFn(rhs)); yield tmp;
      SET_LINENO(call);
      Expr*     rhs        = call->get(1)->remove();
      FnSymbol* initCopyFn = getInitCopyDuringResolution(yieldType);
      VarSymbol* tmp       = newTemp("copy_yield_tmp", initCopyFn->retType);
      Symbol* definedConst = gFalse;
      CallExpr*  unrefCall = new CallExpr(initCopyFn, rhs, definedConst);
      yieldSym->removeFlag(FLAG_YVV);
      tmp->addFlag(FLAG_YVV);

      call->insertBefore(new DefExpr(tmp));
      call->insertBefore(new CallExpr(PRIM_MOVE, tmp, unrefCall));
      call->insertAtHead(tmp); // yield this temp

    } else if (FnSymbol* callee = call->resolvedFunction()) {
      if (isTaskFun(callee))
        insertUnrefForTupleYields(callee);
    }
  }
}

class SplitInitVisitor final : public AstVisitorTraverse {
 public:
  bool inFunction;
  bool changed;
  std::map<Symbol*, Expr*>& preventMap;
  SplitInitVisitor(std::map<Symbol*, Expr*>& preventMap)
    : inFunction(false), changed(false), preventMap(preventMap)
  { }

  bool enterFnSym(FnSymbol* node) override;
  bool enterDefExpr(DefExpr* def) override;
  bool enterCallExpr(CallExpr* call) override;
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

    SymExpr* typeSe = toSymExpr(call->get(2));
    bool requireSplitInit = (typeSe->symbol() == dtSplitInitType->symbol);

    // Don't allow an out-formal to be split-init after a return because
    // that would leave the out-formal uninitialized.
    bool allowReturns = !isOutFormal;
    foundSplitInit = findInitPoints(call, initAssigns, prevent, allowReturns);

    // If the assignment has a different type RHS we should
    // still apply split-init because the language rules
    // here are type-independent. In that case, the type should
    // also have an init= function accepting the different RHS type.

    if (requireSplitInit && !foundSplitInit) {
      splitInitMissingTypeError(sym, prevent, /*unresolved*/false);
    }

    if (foundSplitInit) {
      // Change the PRIM_DEFAULT_INIT_VAR to PRIM_INIT_VAR_SPLIT_DECL
      call->primitive = primitives[PRIM_INIT_VAR_SPLIT_DECL];

      Symbol* type = NULL;

      // remove dummy dtSplitInitType argument if present
      if (typeSe->symbol() == dtSplitInitType->symbol)
        typeSe->remove();
      else
        type = typeSe->symbol();

      // Change the '=' calls found into PRIM_INIT_VAR_SPLIT_INIT
      for_vector(CallExpr, assign, initAssigns) {
        SET_LINENO(assign);
        int rhsNum = 2;
        if (assign->get(1)->typeInfo() == dtMethodToken) {
          rhsNum += 2;
        }
        Expr* rhs = assign->get(rhsNum)->remove();
        CallExpr* init = NULL;
        if (type)
          init = new CallExpr(PRIM_INIT_VAR_SPLIT_INIT, sym, rhs, type);
        else
          init = new CallExpr(PRIM_INIT_VAR_SPLIT_INIT, sym, rhs);
        assign->replace(init);
        resolveInitVar(init);
      }
    } else if (prevent != NULL) {
      preventMap[sym] = prevent;
    }
  }

  return false;
}

class FixPrimInitsVisitor final : public AstVisitorTraverse {
 public:
  bool inFunction;
  bool changed;
  std::map<Symbol*, Expr*>& preventMap;
  FixPrimInitsVisitor(std::map<Symbol*, Expr*>& preventMap)
    : inFunction(false), changed(false), preventMap(preventMap)
  { }

  bool enterFnSym(FnSymbol* node) override;
  bool enterCallExpr(CallExpr* call) override;
};

bool FixPrimInitsVisitor::enterFnSym(FnSymbol* node) {
  // Only visit the top level function requested
  if (inFunction) return false;
  inFunction = true;
  return true;
}

bool FixPrimInitsVisitor::enterCallExpr(CallExpr* call) {
  if (call->isPrimitive(PRIM_DEFAULT_INIT_VAR) ||
      call->isPrimitive(PRIM_NOINIT_INIT_VAR) ||
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

class MarkTempsVisitor final : public AstVisitorTraverse {
 public:
  bool inFunction;
  MarkTempsVisitor() : inFunction(false) { }

  bool enterFnSym(FnSymbol* node) override;
  bool enterDefExpr(DefExpr* node) override;
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

            VarSymbol* tmpVar = toVarSymbol(se->symbol());
            if (tmpVar != NULL && tmpVar != v && tmpVar->hasFlag(FLAG_TEMP)) {
              if (outIntent) {
                // initializing a temp with out intent
                gatherTempsDeadLastMention(tmpVar, temps);
              }
            }
          }
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
          if (lhs != NULL && lhs != v) {

            // Used in initializing a user var, so mark end of block
            if (!lhs->hasFlag(FLAG_TEMP)) {
              makeThemEndOfBlock = true;
              break;

            // For e.g. 'manage new foo() do ...;'
            // The manager handle is a temp that refers to 'new foo()', so
            // what it refers to should live until the end of the block.
            } else if (lhs->hasFlag(FLAG_MANAGER_HANDLE)) {
              makeThemEndOfBlock = true;
              break;
            }
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

  if (fn->hasFlag(FLAG_PROMOTION_WRAPPER)) {
    sym->addFlag(FLAG_PROMOTION_ITERATOR_RECORD);
    for_formals (formal, fn) {
      if (formal->type != gMethodToken->type &&
          formal->type != gFollowerTag->type) {
        if (isAggregateType(formal->type)) {
          VarSymbol* protoField = new VarSymbol(formal->name, formal->type);
          protoField->addFlag(FLAG_PROMOTION_PROTO_FIELD);

          retval->fields.insertAtTail(new DefExpr(protoField));
        }
      }
    }
  }


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
        Symbol *definedConst = ret->hasFlag(FLAG_CONST) ? gTrue : gFalse;
        CallExpr* copy = new CallExpr(astr_autoCopy, rhs->remove(),
                                      definedConst);
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

// Issue an error if this is an interface function whose return type
// is inferred and is not 'void'. Require explicit return type if so.
// This does not cater to iterators as currently those are not allowed
// in an interface.
static void checkInterfaceFunctionRetType(FnSymbol* fn, Type* retType,
                                          bool isIterator) {
  if (! isInterfaceSymbol(fn->defPoint->parentSymbol))
    return; // not an interface function

  if (retType == dtVoid)
    return; // nothing will be returned

  if (fn->retExprType != nullptr)
    return; // the return type is declared explicitly - good

  USR_FATAL_CONT(fn, "interface function %s with inferred return type",
                 fn->name);
  USR_PRINT(fn, "a non-void return type must be declared explicitly");
}

// Resolves an inferred return type.
// resolveSpecifiedReturnType handles the case that the type is
// specified explicitly.
void resolveReturnTypeAndYieldedType(FnSymbol* fn, Type** yieldedType) {
  if (fn->hasFlag(FLAG_NO_FN_BODY)) return;

  // TODO - do we need || fn->iteratorInfo != NULL;
  bool isIterator = fn->isIterator();
  Symbol* ret     = fn->getReturnSymbol();
  Type*   retType = ret->type;

  if (isIterator) {
    // For iterators, the return symbol / return type is void
    // or the iterator record. Here we want to compute the yielded
    // type.
    // We have three cases of the iterator's declared return (yielded) type:
    // * not declared ==> retType==dtUnknown
    // * declared concrete ==> retType is an IR; we have fn->iteratorInfo
    // * declared generic ==> retType has it; no fn->iteratorInfo
    ret = NULL;
    if (retType == dtUnknown)
      ; // keep dtUnknown
    else if (retType->symbol->hasFlag(FLAG_ITERATOR_RECORD))
      retType = fn->iteratorInfo->yieldedType;
    else
      INT_ASSERT(retType->symbol->hasFlag(FLAG_GENERIC)); // fyi
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

  if (fn->retTag != RET_TYPE && retType->symbol->hasFlag(FLAG_GENERIC))
    // The compiler should have reported an error earlier. If it has not,
    // give user-friendly error here - avoid potential confusion at callsite.
    USR_FATAL(fn, "could not determine the concrete type"
              " for the generic %s type '%s'",
              isIterator ? "yielded" : "return", toString(retType));

  if (isIterator == false) {
    ret->type = retType;

    if (retType == dtUnknown) {
      if (fn->hasFlag(FLAG_COMPUTE_UNIFIED_TYPE_HELP)) {
        USR_FATAL(callStack.v[callStack.n-2],
                  "can't compute a unified element type for this array");
      } else {
        USR_FATAL(fn, "unable to resolve return type");
      }
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

  checkInterfaceFunctionRetType(fn, retType, isIterator);
}

void resolveReturnType(FnSymbol* fn) {
  resolveReturnTypeAndYieldedType(fn, NULL);
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

      // Don't destroy formal temps until the end of the function
      tmp->addFlag(FLAG_DEAD_END_OF_BLOCK);

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
    // 'out' requires a temp currently
    //
    (formal->intent == INTENT_OUT ||
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
  if (isRecord(formal->getValType()) || isUnion(formal->getValType()) ||
      isConstrainedType(formal->getValType())) {
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
  return argMustUseCPtr(t) || isConstrainedType(t);
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
  SET_LINENO(fn);

  Expr* start = new CallExpr(PRIM_NOOP);
  fn->insertAtHead(start);

  // Enumerate the formals that have local temps.
  // Process them in formal order so that the order of copy/writeback
  // is consistent.
  for_formals(formal, fn) {
    Symbol* tmp = formals2vars.get(formal);
    if (tmp == NULL)
      continue;

    SET_LINENO(formal);

    // TODO: Move this closer to the location (in code) where we determine
    // whether tmp owns its value or not.  That is, push setting these flags
    // (or not) into the cases below, as appropriate.
    Type* formalType = formal->type->getValType();
    DefExpr* def = NULL;

    // mark CONST as needed
    if (concreteIntent(formal->intent, formalType) & INTENT_FLAG_CONST) {
      tmp->addFlag(FLAG_CONST);

      if (isRefCountedType(formalType) == false) {
        tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
      }
    }

    def = new DefExpr(tmp);
    start->insertBefore(def);

    // Don't do anything complicated for formals of type 'nothing'
    if (formal->getValType() == dtNothing) {
      tmp->type = dtNothing;
      continue;
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
      // Set the formal type to dtUnknown to avoid spurious casts.
      // The formal type should be inferred from the formal-temp we create
      // below when the formal is initialized from the temp in the epilogue.
      formal->type = dtUnknown;

      if (formalType == dtAny || formalType == dtUnknown)
        formalType = dtSplitInitType;

      BlockStmt* defaultExpr = NULL;

      if (formal->defaultExpr &&
          formal->defaultExpr->body.tail->typeInfo() != dtTypeDefaultToken) {
        defaultExpr = formal->defaultExpr->copy();
      }

      if (formalType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
        VarSymbol* typeTmp = NULL;

        if (formal->typeExpr != NULL) {
          typeTmp = newTemp("_formal_type_tmp_");
          typeTmp->addFlag(FLAG_MAYBE_TYPE);
          BlockStmt* typeExpr = formal->typeExpr->copy();
          start->insertBefore(new DefExpr(typeTmp));
          CallExpr* setType = new CallExpr(PRIM_MOVE,
                                           typeTmp,
                                           typeExpr->body.tail->remove());
          start->insertBefore(typeExpr);
          start->insertBefore(setType);
          typeExpr->flattenAndRemove();
        }
        if (defaultExpr != NULL) {
          CallExpr* init = new CallExpr(PRIM_INIT_VAR, tmp,
                                        defaultExpr->body.tail->remove());
          if (typeTmp != NULL)
            init->insertAtTail(new SymExpr(typeTmp));
          start->insertBefore(defaultExpr);
          start->insertBefore(init);
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
          start->insertBefore(init);
          start->insertBefore(new CallExpr(PRIM_END_OF_STATEMENT));
        }
      } else {
        if (defaultExpr != NULL) {
          CallExpr* init = new CallExpr(PRIM_INIT_VAR, tmp,
                                        defaultExpr->body.tail->remove(),
                                        formalType->symbol);
          start->insertBefore(defaultExpr);
          start->insertBefore(init);
        } else {
          CallExpr * init = new CallExpr(PRIM_DEFAULT_INIT_VAR, tmp,
                                         formalType->symbol);
          start->insertBefore(init);
          start->insertBefore(new CallExpr(PRIM_END_OF_STATEMENT));
        }
      }

      tmp->addFlag(FLAG_FORMAL_TEMP);
      tmp->addFlag(FLAG_FORMAL_TEMP_OUT);
      tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
      break;
     }
     case INTENT_INOUT:
      INT_FATAL("Unexpected INTENT case.");
      break;

     case INTENT_IN:
     case INTENT_CONST_IN:
      if (!shouldAddInFormalTempAtCallSite(formal, fn)) {
        Symbol* definedConst = formal->intent == INTENT_CONST_IN ? gTrue : gFalse;
        start->insertBefore(new CallExpr(PRIM_MOVE,
                                         tmp,
                                         new CallExpr(astr_initCopy, formal,
                                                      definedConst)));

        tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
      } else {
        // move from in intent argument to local variable to be destroyed
        // (The local variable is not strictly necessary but is a more
        //  typical pattern for follow-on passes)
        tmp->addFlag(FLAG_NO_COPY);
        start->insertBefore(new CallExpr(PRIM_MOVE, tmp, formal));

        // Default-initializers and '_new' wrappers take ownership
        // Note: FLAG_INSERT_AUTO_DESTROY is blindly applied to any formal
        // with const-in intent at the start of this function, so we need
        // to apply FLAG_NO_AUTO_DESTROY to avoid double-frees.
        if (fn->hasFlag(FLAG_NEW_WRAPPER) || fn->isDefaultInit()) {
          tmp->addFlag(FLAG_NO_AUTO_DESTROY);
        } else {
          if (!tmp->hasFlag(FLAG_NO_AUTO_DESTROY)) {
            tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
          }
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
           start->insertBefore(new CallExpr(PRIM_MOVE, tmp, formal));
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
           start->insertBefore(new CallExpr(PRIM_MOVE,
                                            tmp,
                                            new CallExpr(astr_autoCopy, formal,
                                                         gFalse)));

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
         start->insertBefore(new CallExpr(PRIM_MOVE, tmp, formal));
         // If this is a simple move, then we did not call chpl__autoCopy to
         // create tmp, so then it is a bad idea to insert a call to
         // chpl__autodestroy later.
         tmp->removeFlag(FLAG_INSERT_AUTO_DESTROY);
       }

       break;
     }
    }

    // For out intent, this assigns the modified value back to the
    // formal at the end of the function body.
    if (formal->intent == INTENT_OUT) {
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

static void insertCasts(BaseAST* ast, FnSymbol* fn,
                        std::vector<CallExpr*>& newCalls);

static bool insertAndResolveCasts(FnSymbol* fn) {
  bool changed = false;

  if (fn->retTag != RET_PARAM) {
    std::vector<CallExpr*> newCalls;

    insertCasts(fn->body, fn, newCalls);

    for_vector(CallExpr, call, newCalls) {
      if (call->inTree()) {
        // Resolve PRIM_MOVE etc
        Expr* e = resolveExpr(call);
        // and additionally calls to functions
        if (CallExpr* eCall = toCallExpr(e))
          resolveCallAndCallee(eCall, true);
      }
    }
  }

  return changed;
}

Type* arrayElementType(Type* arrayType) {
  AggregateType* at = toAggregateType(arrayType);
  if (at == NULL) return NULL;

  Symbol* instField = at->getField("_instance", false);
  if (instField == NULL) return NULL;

  Type* instType = instField->type;
  AggregateType* instClass = toAggregateType(canonicalClassType(instType));
  if (instClass == NULL) return NULL;

  Symbol* eltTypeField = instClass->getField("eltType", false);
  if (eltTypeField == NULL) return NULL;

  Type* eltType = eltTypeField->getValType();
  return eltType;
}

static void issueInitConversionError(Symbol* to, Symbol* toType, Symbol* from,
                                     Expr* where) {

  Type* fromValType = from->getValType();

  const char* toName = NULL;
  const char* toTypeStr = NULL;
  const char* fromStr = NULL;
  const char* sep = "";

  if (to->hasFlag(FLAG_RVV)) {
    toName = "return value";
  } else if (to->hasFlag(FLAG_YVV)) {
    toName = "yield value";
  } else if (VarSymbol* var = toVarSymbol(to)) {
    toName = toString(var, false);
    if (toName == astr("<temporary>") ||
        to->defPoint->getModule()->modTag != MOD_USER)
      toName = "a value";
    else if (startsWith(toName, "field "))
      toName = astr("field '", &toName[6], "'");
    else
      toName = astr("'", toName, "'");
  } else {
    toName = astr("'", to->name, "'");
  }

  toTypeStr = toString(toType->type);

  VarSymbol* var = toVarSymbol(from);
  if (var && getSymbolImmediate(var)) {
    sep = "";
    fromStr = toString(var, true);
  } else if (fromValType == dtNil) {
    sep = "";
    fromStr = "nil";
  } else {
    sep = "a ";
    fromStr = toString(fromValType);
  }

  USR_FATAL_CONT(where,
                 "cannot initialize %s of type '%s' from %s'%s'",
                 toName, toTypeStr, sep, fromStr);
}

// Emit an init= or similar pattern to create 'to' of type 'toType' from 'from'
// (the Symbol toType conveys any runtime type info beyond what to->type is.)
//
// No matter what, adds the initialization pattern after 'insertAfter'.
// Adds all calls added to the newCalls vector to be resolved later.
static void insertInitConversion(Symbol* to, Symbol* toType, Symbol* from,
                                 bool fromPrimCoerce,
                                 CallExpr* insertBefore,
                                 std::vector<CallExpr*>& newCalls) {

  Type* fromValType = from->getValType();

  // Adjust and check toType
  {
    Type* toValType = to->getValType();
    // Use the inferred type of 'to' if the 'toType' is generic.
    if (toType->type->symbol->hasFlag(FLAG_GENERIC)) {
      INT_ASSERT(!toValType->symbol->hasFlag(FLAG_GENERIC));
      toType = toValType->symbol;
    }
    // Remainder of this code assumes that to and toType match.
    INT_ASSERT(toValType == toType->type);

    // generate a warning in some cases for int->uint implicit conversion
    warnForIntUintConversion(insertBefore, toValType, fromValType, from);
  }

  // seemingly redundant toType->type->symbol is for lowered runtime type vars
  bool runtimeType = toType->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE);
  // Don't consider runtime types unless from PRIM_COERCE,
  // because if not in a PRIM_COERCE, we have no way of accessing it.
  bool useRttCopy = runtimeType && fromPrimCoerce;
  bool typesDiffer = (toType->type != fromValType);

  // If the types are the same but runtime types are involved, we don't know
  // that the runtime types are the same until runtime (at least without some
  // better smarts in the compiler). So we have to add a special call
  // to handle that case.

  if (!typesDiffer && !useRttCopy) {
    // types are the same and no runtime types.
    // handle reference level adjustments. No cast necessary.

    Expr* newRhs = NULL;

    if (from->type == to->type) {
      newRhs = new SymExpr(from);

    } else if (from->type == to->type->refType) {
      // to a value
      // from a reference
      //  i.e.   myValT = myRefT

      if (typeNeedsCopyInitDeinit(fromValType)) {
        Symbol *definedConst = to->hasFlag(FLAG_CONST) ?
                               gTrue : gFalse;
        // TODO: this could call init= directly in most cases
        CallExpr* autoCopy = new CallExpr(astr_autoCopy,
                                          new SymExpr(from), definedConst);
        newCalls.push_back(autoCopy);
        newRhs = autoCopy;
        // TODO: does this always add a memory leak?
      } else {
        CallExpr* deref = new CallExpr(PRIM_DEREF, from);
        newCalls.push_back(deref);
        newRhs = deref;
        // TODO: is this ever run?
      }

    } else if (from->type->refType == toType->type) {
      INT_FATAL("not possible"); // should be ruled out above
      CallExpr* addrOf = new CallExpr(PRIM_ADDR_OF, new SymExpr(from));
      newCalls.push_back(addrOf);
      newRhs = addrOf;

    } else {
      INT_FATAL("case not handled");
    }

    CallExpr* assign = new CallExpr(PRIM_ASSIGN, to, newRhs);
    newCalls.push_back(assign);
    insertBefore->insertBefore(assign);

    return;
  }

  bool stealRHS = false;

  AggregateType* ir = toAggregateType(from->getValType());
  if (useRttCopy) {
    if (ir && ir->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
      // For iterators, set stealRHS based upon whether
      // the iterator returns by value.
      IteratorInfo* ii = ir->iteratorInfo;
      INT_ASSERT(ii);
      bool yieldsRefs = ii->getValue->getReturnQualType().isRef();
      if (yieldsRefs)
        stealRHS = false;
      else
        stealRHS = true;
    } else {
      // not an iterator, steal if it's a temp
      // but don't steal if the compile-time type differs
      // (e.g. arrays with different element types; see nbody_orig_1.chpl)
      stealRHS = from->hasFlag(FLAG_TEMP) &&
                 !from->isRef() &&
                 !from->hasFlag(FLAG_INSERT_AUTO_DESTROY) &&
                 !from->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW) &&
                 !typesDiffer;
    }
  }

  Symbol *definedConst = to->hasFlag(FLAG_CONST) ?  gTrue : gFalse;

  if (stealRHS == false) {
    // Since the initialization pattern normally does not
    // require adding an auto-destroy for a call-expr-temp,
    // add FLAG_INSERT_AUTO_DESTROY since we're
    // copy-initializing from it.
    from->addFlag(FLAG_INSERT_AUTO_DESTROY);
  }

  if (useRttCopy) {
    CallExpr* newCall = NULL;
    if (stealRHS) {
      newCall = new CallExpr(astr_coerceMove, toType, from, definedConst);
    } else {
      newCall = new CallExpr(astr_coerceCopy, toType, from, definedConst);
    }
    newCalls.push_back(newCall);
    CallExpr* move = new CallExpr(PRIM_MOVE, to, newCall);
    newCalls.push_back(move);
    insertBefore->insertBefore(move);

  } else {
    INT_ASSERT(stealRHS == false); // other case not handled here

    if (toType->type->symbol->hasFlag(FLAG_TUPLE)) {

      if (canCoerce(fromValType, from, toType->type, NULL, NULL)) {
        // use tuple cast (at least to handle ref vs value tuples)
        // TODO: adjust tuples to use init=
        CallExpr* cast = createCast(from, toType->type->symbol);
        newCalls.push_back(cast);
        CallExpr* assign = new CallExpr(PRIM_ASSIGN, to, cast);
        newCalls.push_back(assign);
        insertBefore->insertBefore(assign);
      } else {
        issueInitConversionError(to, toType, from, insertBefore);
      }

    } else if (toType->type == getCopyTypeDuringResolution(fromValType)) {
      // today, this code should only apply to sync/single
      // (since arrays are handled above with useRttCopy)
      // for sync/single, getCopyTypeDuringResolution returns the valType.
      Type* valType = getCopyTypeDuringResolution(fromValType);

      VarSymbol* tmp = newTemp("_cast_tmp_", valType);
      insertBefore->insertBefore(new DefExpr(tmp));

      CallExpr* readCall = NULL;
      if (isSyncType(fromValType)) {
        readCall = new CallExpr("readFE", gMethodToken, from);
        USR_WARN(to, "implicitly reading from a sync is deprecated; "
                     "apply a 'read\?\?()' method to the actual");
      } else if (isSingleType(fromValType)) {
        readCall = new CallExpr("readFF", gMethodToken, from);
        USR_WARN(to, "implicitly reading from a single is deprecated; "
                     "apply a 'read\?\?()' method to the actual");
      } else {
        INT_FATAL("not handled");
      }

      newCalls.push_back(readCall);
      CallExpr* setTmp = new CallExpr(PRIM_ASSIGN, tmp, readCall);
      newCalls.push_back(setTmp);
      insertBefore->insertBefore(setTmp);
      // add a conversion / plain assignment setting to from tmp
      insertInitConversion(to, toType, tmp,
                           fromPrimCoerce, insertBefore, newCalls);

    } else if (isRecord(toType->type) || isUnion(toType->type)) {
      // insert an init= call
      CallExpr* initEq = NULL;
      initEq = new CallExpr(astrInitEquals, gMethodToken, to, from);
      newCalls.push_back(initEq);
      insertBefore->insertBefore(initEq);

    } else {
      // for types where it's not yet possible to write init=,
      // use PRIM_ASSIGN and PRIM_CAST.
      // This should only be occurring for types that are
      // either extern or for non-record types when the coercion is legal.

      // (TODO: use tertiary initializers to remove the exception for
      //  extern types)

      if (toType->type->symbol->hasFlag(FLAG_EXTERN) ||
          fromValType->symbol->hasFlag(FLAG_EXTERN) ||
          canCoerce(fromValType, from, toType->type, NULL, NULL) ||
          (toType->type == dtStringC && fromValType == dtNil)) {
        // Cast and assign
        INT_ASSERT(!typeNeedsCopyInitDeinit(toType->type));

        CallExpr* cast = createCast(from, toType->type->symbol);
        newCalls.push_back(cast);
        CallExpr* assign = new CallExpr(PRIM_ASSIGN, to, cast);
        newCalls.push_back(assign);
        insertBefore->insertBefore(assign);

      } else {
        issueInitConversionError(to, toType, from, insertBefore);
      }
    }
  }
}

// Adds conversions for cases where the lhs and rhs types of
// PRIM_MOVE/PRIM_ASSIGN do not match. The conversions might be implemented
// with a cast or init=.
//
// All new calls added by this function are added to newCalls so that
// they can be resolved appropriately.
static void insertCasts(BaseAST* ast, FnSymbol* fn,
                        std::vector<CallExpr*>& newCalls) {

  if (isSymbol(ast) && ! isShadowVarSymbol(ast))
    return; // do not descend into nested symbols

  if (CallExpr* call = toCallExpr(ast)) {
    if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
      if (SymExpr* lhs = toSymExpr(call->get(1))) {
        Type* lhsType = lhs->symbol()->type;

        if (lhsType != dtUnknown) {
          Expr*     rhs     = call->get(2);
          CallExpr* rhsCall = toCallExpr(rhs);

          // PRIM_ASSIGN will set a value in the LHS type,
          // never set what a reference points to.
          // Same for a PRIM_MOVE with a LHS ref and RHS value.
          if (call->isPrimitive(PRIM_ASSIGN) || rhs->isRef() == false)
            lhsType = lhsType->getValType();

          if (call->id == breakOnResolveID)
            gdbShouldBreakHere();

          Symbol* to = lhs->symbol();
          Symbol* toType = NULL;
          Symbol* from = NULL;

          bool isTypeOf = (rhsCall && rhsCall->isPrimitive(PRIM_TYPEOF));
          bool fromPrimCoerce = false;

          if (isTypeOf == false) {
            if (rhsCall && rhsCall->isPrimitive(PRIM_COERCE)) {
              SymExpr* fromExpr     = toSymExpr(rhsCall->get(1));
              SymExpr* fromTypeExpr = toSymExpr(rhsCall->get(2));
              INT_ASSERT(fromExpr && fromTypeExpr);
              // use the type requested in the PRIM_COERCE
              toType = fromTypeExpr->symbol();
              from = fromExpr->symbol();
              fromPrimCoerce = true;
              // Check that lhsType == the result of coercion
              INT_ASSERT(lhsType == rhsCall->typeInfo());
            } else if (lhsType == rhs->typeInfo()) {
              // the types match exactly and it's not a PRIM_COERCE.
              // There is nothing to do; the PRIM_MOVE/PRIM_ASSIGN is OK.
              from = NULL;
            } else if (lhsType->getValType() == rhs->getValType() &&
                       call->isPrimitive(PRIM_ASSIGN)) {
              // The types match except for reference level. In PRIM_ASSIGN.
              // Nothing to do since the PRIM_ASSIGN is already valid.
              // Note, this case shouldn't add an autoCopy call if the RHS
              // is a ref (that would present problems for use of PRIM_ASSIGN
              // in module code).
              from = NULL;
            } else if (SymExpr* rhsSe = toSymExpr(rhs)) {
              // use the type of the LHS
              toType = lhsType->symbol;
              from = rhsSe->symbol();
            } else {
              // Store the RHS into a temporary
              Symbol* tmp = NULL;
              tmp = newTemp("_cast_tmp_", rhs->typeInfo());
              call->insertBefore(new DefExpr(tmp));
              CallExpr* newMove = new CallExpr(PRIM_MOVE, tmp, rhs->copy());
              call->insertBefore(newMove);
              newCalls.push_back(newMove);
              from = tmp;
              toType = lhsType->symbol;
            }

            if (from != NULL && from->hasFlag(FLAG_TYPE_VARIABLE)) {
              INT_ASSERT(lhs->symbol()->hasFlag(FLAG_TYPE_VARIABLE));
              // The lhs and rhs types differ when the return type is inferred
              // from return statements with different types, ex. see #20481.
              if (lhsType == rhs->typeInfo()) {
                // move from a type to the same type -- nothing else to do
                from = NULL;
              }
            }
            // If rewriting this operation is required, do it
            if (from != NULL) {
              SET_LINENO(rhs);
              insertInitConversion(to, toType, from, fromPrimCoerce,
                                   call, newCalls);
              call->remove();
            }
          }
        }
      }
    }
  }

  AST_CHILDREN_CALL(ast, insertCasts, fn, newCalls);
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
