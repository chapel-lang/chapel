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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include "resolution.h"

#include "AstCount.h"
#include "astutil.h"
#include "build.h"
#include "caches.h"
#include "callInfo.h"
#include "CatchStmt.h"
#include "CForLoop.h"
#include "DeferStmt.h"
#include "driver.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "initializerResolution.h"
#include "iterator.h"
#include "ModuleSymbol.h"
#include "ParamForLoop.h"
#include "PartialCopyData.h"
#include "passes.h"
#include "postFold.h"
#include "preFold.h"
#include "ResolutionCandidate.h"
#include "resolveIntents.h"
#include "scopeResolve.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "TryStmt.h"
#include "typeSpecifier.h"
#include "view.h"
#include "virtualDispatch.h"
#include "visibleFunctions.h"
#include "wellknown.h"
#include "WhileStmt.h"
#include "wrappers.h"

#include "../ifa/prim_data.h"

#include <inttypes.h>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class DisambiguationState {
public:
        DisambiguationState();

  void  updateParamPrefers(int                          preference,
                           const char*                  argStr,
                           const DisambiguationContext& DC);

  bool  fn1MoreSpecific;
  bool  fn2MoreSpecific;

  bool  fn1Promotes;
  bool  fn2Promotes;

  // 1 == fn1, 2 == fn2, -1 == conflicting signals
  int   paramPrefers;
};

// map: (block id) -> (map: sym -> sym)
typedef std::map<int, SymbolMap*> CapturedValueMap;

//#
//# Global Variables
//#
char                               arrayUnrefName[] = "array_unref_ret_tmp";

bool                               resolved                  = false;
bool                               tryFailure                = false;
bool                               beforeLoweringForallStmts = true;

int                                explainCallLine           = 0;

SymbolMap                          paramMap;

Vec<CallExpr*>                     callStack;
Vec<CallExpr*>                     inits;

Vec<BlockStmt*>                    standardModuleSet;

std::map<CallExpr*, CallExpr*>     eflopiMap;

std::map<Type*,     FnSymbol*>     autoCopyMap;
std::map<Type*,     Serializers>   serializeMap;

Map<Type*,          FnSymbol*>     autoDestroyMap;
Map<Type*,          FnSymbol*>     unaliasMap;
Map<Type*,          FnSymbol*>     valueToRuntimeTypeMap;
Map<FnSymbol*,      FnSymbol*>     iteratorLeaderMap;
Map<FnSymbol*,      FnSymbol*>     iteratorFollowerMap;

//#
//# Static Variables
//#
static ModuleSymbol*               explainCallModule;

static Vec<FnSymbol*>              resolvedFormals;

static Vec<CondStmt*>              tryStack;

static Map<Type*,     Type*>       runtimeTypeMap;

static Map<Type*,     FnSymbol*>   runtimeTypeToValueMap;

static Map<FnSymbol*, const char*> innerCompilerWarningMap;

static Map<FnSymbol*, const char*> outerCompilerWarningMap;

static CapturedValueMap            capturedValues;


//#
//# Static Function Declarations
//#
static bool hasRefField(Type *type);
static bool typeHasRefField(Type *type);
static FnSymbol* resolveUninsertedCall(Type* type, CallExpr* call);
static bool hasUserAssign(Type* type);
static void resolveOther();
static FnSymbol*
protoIteratorMethod(IteratorInfo* ii, const char* name, Type* retType);
static void protoIteratorClass(FnSymbol* fn);
static void resolveSpecifiedReturnType(FnSymbol* fn);
static bool fits_in_int(int width, Immediate* imm);
static bool fits_in_uint(int width, Immediate* imm);
static bool canParamCoerce(Type* actualType, Symbol* actualSym, Type* formalType);
static bool
moreSpecific(FnSymbol* fn, Type* actualType, Type* formalType);
static BlockStmt* getParentBlock(Expr* expr);
static bool
isMoreVisibleInternal(BlockStmt* block, FnSymbol* fn1, FnSymbol* fn2,
                      Vec<BlockStmt*>& visited);
static bool
isMoreVisible(Expr* expr, FnSymbol* fn1, FnSymbol* fn2);
static CallExpr* userCall(CallExpr* call);
static void reissueCompilerWarning(const char* str, int offset);
static Type* resolveDefaultGenericTypeSymExpr(SymExpr* se);

static FnSymbol* resolveNormalCall(CallExpr* call, bool checkonly=false);
static void resolveTupleAndExpand(CallExpr* call);
static void resolveTupleExpand(CallExpr* call);
static void resolveSetMember(CallExpr* call);
static void resolveInitField(CallExpr* call);
static void resolveInitVar(CallExpr* call);
static void resolveMove(CallExpr* call);
static void resolveNew(CallExpr* call);
static void temporaryInitializerFixup(CallExpr* call);
static void resolveCoerce(CallExpr* call);
static bool formalRequiresTemp(ArgSymbol* formal);
static void addLocalCopiesAndWritebacks(FnSymbol* fn, SymbolMap& formals2vars);

static Expr* resolveExpr(Expr* expr);

static Expr* foldTryCond(Expr* expr);

static void  computeReturnTypeParamVectors(BaseAST*      ast,
                                           Symbol*       retSymbol,
                                           Vec<Type*>&   retTypes,
                                           Vec<Symbol*>& retParams);

static void  insertCasts(BaseAST* ast, FnSymbol* fn, Vec<CallExpr*>& casts);
static void computeStandardModuleSet();
static void unmarkDefaultedGenerics();
static void resolveUses(ModuleSymbol* mod);
static void resolveSupportForModuleDeinits();
static void resolveExports();
static void resolveEnumTypes();
static void insertRuntimeTypeTemps();
static void resolveAutoCopies();
static void resolveSerializers();
static void resolveRecordInitializers();
static Type* buildRuntimeTypeInfo(FnSymbol* fn);
static void insertReturnTemps();
static void initializeClass(Expr* stmt, Symbol* sym);
static void ensureAndResolveInitStringLiterals();
static void handleRuntimeTypes();
static void pruneResolvedTree();
static void removeUnusedFunctions();
static void removeUnusedTypes();
static void buildRuntimeTypeInitFns();
static void buildRuntimeTypeInitFn(FnSymbol* fn, Type* runtimeType);
static void removeUnusedModuleVariables();
static void removeParamArgs();
static void removeRandomPrimitives();
static void removeActualNames();
static void removeTypeBlocks();
static void removeFormalTypeAndInitBlocks();
static void replaceTypeArgsWithFormalTypeTemps();
static void replaceValuesWithRuntimeTypes();
static void removeWhereClauses();
static void replaceReturnedValuesWithRuntimeTypes();
static void insertRuntimeInitTemps();
static void removeInitFields();
static void removeMootFields();
static void expandInitFieldPrims();
static void fixTypeNames(AggregateType* ct);
static void setScalarPromotionType(AggregateType* ct);
static FnSymbol* findGenMainFn();
static void printCallGraph(FnSymbol* startPoint = NULL,
                           int indent = 0,
                           std::set<FnSymbol*>* alreadyCalled = NULL);

static void handleTaskIntentArgs(CallInfo& info, FnSymbol* taskFn);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool hasRefField(Type *type) {
  if (isPrimitiveType(type)) return false;
  if (type->symbol->hasFlag(FLAG_OBJECT_CLASS)) return false;

  if (!isClass(type)) { // record or union
    if (AggregateType *ct = toAggregateType(type)) {
      for_fields(field, ct) {
        if (hasRefField(field->type)) return true;
      }
    }
    return false;
  }

  return true;
}

static bool typeHasRefField(Type *type) {
  if (AggregateType *ct = toAggregateType(type)) {
    for_fields(field, ct) {
      if (hasRefField(field->typeInfo())) return true;
    }
  }
  return false;
}

//
// Invoke resolveFns(fn), while having 'call' be on the top of 'callStack'.
//
void resolveFnForCall(FnSymbol* fn, CallExpr* call)
{
  // If 'call' is already on the call stack, do not add it.
  // If this assertion fails, change it to 'if'.
  INT_ASSERT(callStack.n == 0 || call != callStack.v[callStack.n-1]);

  // When 'call' is on 'callStack', its parentSymbol etc. may be queried
  // in printCallStack(), which resolveFns() may invoke.
  INT_ASSERT(call->inTree());

  // Push 'call' onto the stack. In case of an error or warning,
  // this allows the user to see how they got there.
  callStack.add(call);

  // do real work
  resolveFns(fn);

  callStack.pop();
}

//
// Resolve 'call', then resolve its target function if applicable.
// 'call' must be resolved successfully, except when allowUnresolved==true.
//
void resolveCallAndCallee(CallExpr* call, bool allowUnresolved) {
  resolveCall(call);

  if (FnSymbol* callee = call->resolvedFunction()) {
    resolveFnForCall(callee, call);
  } else if (!allowUnresolved) {
    INT_ASSERT(false);
  }
}


// Fills in the refType field of a type
// with the type's corresponding reference type.
void makeRefType(Type* type) {
  if (!type)
    // Should this be an assert?
    return;

  if (type->refType) {
    // Already done.
    return;
  }

  if (type == dtMethodToken ||
      type == dtUnknown ||
      type->symbol->hasFlag(FLAG_REF) ||
      type->symbol->hasFlag(FLAG_GENERIC)) {

    return;
  }

  CallExpr* call = new CallExpr("_type_construct__ref", type->symbol);
  FnSymbol* fn   = resolveUninsertedCall(type, call);
  type->refType  = toAggregateType(fn->retType);

  type->refType->getField(1)->type = type;

  if (type->symbol->hasFlag(FLAG_ATOMIC_TYPE))
    type->refType->symbol->addFlag(FLAG_ATOMIC_TYPE);
}

static bool
hasUserAssign(Type* type) {

  // Workaround for problems with resolution finding =
  // for tuples types causing compile failures.
  // See
  //  modules/sungeun/no-use-enum

  // In the future, hasUserAssign should just return
  // false if the = call does not resolve
  // (instead of causing a compile error).
  if( type->symbol->hasFlag(FLAG_TUPLE) ) return false;

  SET_LINENO(type->symbol);
  Symbol* tmp = newTemp(type);
  chpl_gen_main->insertAtHead(new DefExpr(tmp));
  CallExpr* call = new CallExpr("=", tmp, tmp);
  FnSymbol* fn = resolveUninsertedCall(type, call);
  // Don't think we need to resolve the whole function
  // since we're just looking for a flag.
  //resolveFns(fn);
  tmp->defPoint->remove();
  bool compilerAssign = fn->hasFlag(FLAG_COMPILER_GENERATED);
  return !compilerAssign;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool hasAutoCopyForType(Type* type) {
  std::map<Type*, FnSymbol*>::iterator it = autoCopyMap.find(type);

  return autoCopyMap.find(type) != autoCopyMap.end() && it->second != NULL;
}

// This function is intended to protect gets from the autoCopyMap so that
// we can insert NULL values for a type and avoid segfaults
FnSymbol* getAutoCopyForType(Type* type) {
  std::map<Type*, FnSymbol*>::iterator it = autoCopyMap.find(type);

  if (it == autoCopyMap.end() || it->second == NULL) {
    INT_FATAL(type,
              "Trying to obtain autoCopy for type '%s', which defines none",
              type->symbol->name);
  }

  return it->second;
}

void getAutoCopyTypeKeys(Vec<Type*>& keys) {
  std::map<Type*, FnSymbol*>::iterator it;

  for (it = autoCopyMap.begin(); it != autoCopyMap.end(); ++it) {
    keys.add(it->first);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// This function is called by generic instantiation
// for the default initCopy function in ChapelBase.chpl.
bool fixupDefaultInitCopy(FnSymbol* fn, FnSymbol* newFn, CallExpr* call) {
  ArgSymbol* arg = newFn->getFormal(1);

  if (AggregateType* ct = toAggregateType(arg->type)) {
    if (isUserDefinedRecord(ct) &&
        ct->initializerStyle == DEFINES_INITIALIZER) {
      // If the user has defined any initializer,
      // initCopy function should call the copy-initializer.
      //
      // If no copy-initializer exists, we should make initCopy
      // be a dummy function that generates an error
      // if it remains in the AST after callDestructors. We do
      // that since callDestructors can remove some initCopy calls
      // and we'd like types that cannot be copied to survive
      // compilation until callDestructors has a chance to
      // remove those calls.

      // Go ahead and instantiate the body now so we can fix
      // it up completely...
      instantiateBody(newFn);

      Symbol* thisTmp = newTemp(ct);
      DefExpr* def = new DefExpr(thisTmp);
      newFn->insertBeforeEpilogue(def);
      CallExpr* initCall = new CallExpr("init", gMethodToken, thisTmp, arg);
      def->insertAfter(initCall);

      FnSymbol* initFn = tryResolveCall(initCall);

      if (initFn == NULL) {
        // No copy-initializer could be found
        def->remove();
        initCall->remove();
        newFn->addFlag(FLAG_ERRONEOUS_INITCOPY);
      } else {
        // Replace the other setting of the return-value-variable
        // with what we have now...

        // find the RVV
        Symbol* retSym = newFn->getReturnSymbol();

        // Remove other PRIM_MOVEs to the RVV
        for_alist(stmt, newFn->body->body) {
          if (CallExpr* callStmt = toCallExpr(stmt))
            if (callStmt->isPrimitive(PRIM_MOVE)) {
              SymExpr* se = toSymExpr(callStmt->get(1));
              INT_ASSERT(se);
              if (se->symbol() == retSym)
                stmt->remove();
            }
        }

        // Set the RVV to the copy
        newFn->insertBeforeEpilogue(new CallExpr(PRIM_MOVE, retSym, thisTmp));
      }
      return true;
    }
  }
  return false;
}


FnSymbol* getAutoCopy(Type *t) {
  return getAutoCopyForType(t);
}


FnSymbol* getAutoDestroy(Type* t) {
  return autoDestroyMap.get(t);
}
FnSymbol* getUnalias(Type* t) {
  return unaliasMap.get(t);
}

static FnSymbol*
protoIteratorMethod(IteratorInfo* ii, const char* name, Type* retType) {
  FnSymbol* fn = new FnSymbol(name);
  fn->addFlag(FLAG_AUTO_II);
  if (strcmp(name, "advance"))
    fn->addFlag(FLAG_INLINE);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->addFlag(FLAG_METHOD);
  fn->_this = new ArgSymbol(INTENT_BLANK, "this", ii->iclass);
  fn->_this->addFlag(FLAG_ARG_THIS);
  fn->retType = retType;
  fn->insertFormalAtTail(fn->_this);
  ii->iterator->defPoint->insertBefore(new DefExpr(fn));
  normalize(fn);

  // Pretend that this function is already resolved.
  // Its body will be filled in during the lowerIterators pass.
  fn->addFlag(FLAG_RESOLVED);
  return fn;
}


static void
protoIteratorClass(FnSymbol* fn) {
  INT_ASSERT(!fn->iteratorInfo);

  SET_LINENO(fn);

  IteratorInfo* ii = new IteratorInfo();
  fn->iteratorInfo = ii;
  fn->iteratorInfo->iterator = fn;

  const char* className = astr(fn->name);
  if (fn->_this)
    className = astr(className, "_", fn->_this->type->symbol->cname);

  ii->iclass = new AggregateType(AGGREGATE_CLASS);

  TypeSymbol* cts = new TypeSymbol(astr("_ic_", className), ii->iclass);

  cts->addFlag(FLAG_ITERATOR_CLASS);
  cts->addFlag(FLAG_POD);

  ii->iclass->addRootType();

  fn->defPoint->insertBefore(new DefExpr(cts));

  ii->irecord = new AggregateType(AGGREGATE_RECORD);

  TypeSymbol* rts = new TypeSymbol(astr("_ir_", className), ii->irecord);

  rts->addFlag(FLAG_ITERATOR_RECORD);

  // TODO -- do a better job of deciding if an iterator record is
  // POD or not POD.
  rts->addFlag(FLAG_NOT_POD);

  if (fn->retTag == RET_REF) { // TODO -- handle RET_CONST_REF
    rts->addFlag(FLAG_REF_ITERATOR_CLASS);
  }

  fn->defPoint->insertBefore(new DefExpr(rts));

  ii->tag      = it_iterator;
  ii->advance  = protoIteratorMethod(ii, "advance",  dtVoid);
  ii->zip1     = protoIteratorMethod(ii, "zip1",     dtVoid);
  ii->zip2     = protoIteratorMethod(ii, "zip2",     dtVoid);
  ii->zip3     = protoIteratorMethod(ii, "zip3",     dtVoid);
  ii->zip4     = protoIteratorMethod(ii, "zip4",     dtVoid);
  ii->hasMore  = protoIteratorMethod(ii, "hasMore",  dtInt[INT_SIZE_DEFAULT]);
  ii->getValue = protoIteratorMethod(ii, "getValue", fn->retType);
  ii->init     = protoIteratorMethod(ii, "init",     dtVoid);
  ii->incr     = protoIteratorMethod(ii, "incr",     dtVoid);

  // Save the iterator info in the iterator record.
  // The iterator info is still owned by the iterator function.
  ii->irecord->iteratorInfo        = ii;
  ii->irecord->scalarPromotionType = fn->retType;

  fn->retType = ii->irecord;
  fn->retTag  = RET_VALUE;

  makeRefType(fn->retType);

  ii->getIterator = new FnSymbol("_getIterator");
  ii->getIterator->addFlag(FLAG_AUTO_II);
  ii->getIterator->addFlag(FLAG_INLINE);
  ii->getIterator->retType = ii->iclass;
  ii->getIterator->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "ir", ii->irecord));
  VarSymbol* ret = newTemp("_ic_", ii->iclass);
  ii->getIterator->insertAtTail(new DefExpr(ret));
  CallExpr* icAllocCall = callChplHereAlloc(ret->typeInfo());
  ii->getIterator->insertAtTail(new CallExpr(PRIM_MOVE, ret, icAllocCall));
  ii->getIterator->insertAtTail(new CallExpr(PRIM_SETCID, ret));
  ii->getIterator->insertAtTail(new CallExpr(PRIM_RETURN, ret));
  fn->defPoint->insertBefore(new DefExpr(ii->getIterator));
  // Save the iterator info in the iterator class also.
  // This makes it easy to obtain the iterator given
  // just a symbol of the iterator class type.  This may include _getIterator
  // and _getIteratorZip functions in the module code.
  ii->iclass->iteratorInfo = ii;
  normalize(ii->getIterator);
  resolveFns(ii->getIterator);  // No shortcuts.
}


// Generally speaking, tuples containing refs should be converted
// to tuples without refs before returning.
// This function returns true for exceptional FnSymbols
// where tuples containing refs can be returned.
static bool
doNotChangeTupleTypeRefLevel(FnSymbol* fn, bool forRet)
{

  if( fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)         || // _type_construct__tuple
      fn->hasFlag(FLAG_CONSTRUCTOR)              || // _construct__tuple
      fn->hasFlag(FLAG_BUILD_TUPLE)              || // _build_tuple(_allow_ref)
      fn->hasFlag(FLAG_BUILD_TUPLE_TYPE)         || // _build_tuple_type
      fn->hasFlag(FLAG_TUPLE_CAST_FN)            || // _cast for tuples
      fn->hasFlag(FLAG_EXPAND_TUPLES_WITH_VALUES)|| // iteratorIndex
      (!forRet && fn->hasFlag(FLAG_INIT_COPY_FN))|| // tuple chpl__initCopy
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

bool isTupleContainingOnlyReferences(Type* t)
{
  if(t->symbol->hasFlag(FLAG_TUPLE)) {
    bool allRef = true;
    AggregateType* at = toAggregateType(t);
    int i = 0;
    for_fields(field, at) {
      if (i != 0) { // skip size field
        if (!isReferenceType(field->type))
          allRef = false;
      }
      i++;
    }
    return allRef;
  }

  return false;
}

static bool
isTupleContainingAnyReferences(Type* t)
{
  if(t->symbol->hasFlag(FLAG_TUPLE)) {
    bool anyRef = false;
    AggregateType* at = toAggregateType(t);
    int i = 0;
    for_fields(field, at) {
      if (i != 0) { // skip size field
        if (isReferenceType(field->type))
          anyRef = true;
        if (field->type->symbol->hasFlag(FLAG_TUPLE) &&
            isTupleContainingAnyReferences(field->type))
          anyRef = true;

      }
      i++;
    }
    return anyRef;
  }

  return false;
}


static Type*
getReturnedTupleType(FnSymbol* fn, AggregateType* retType)
{
  INT_ASSERT(retType->symbol->hasFlag(FLAG_TUPLE));

  if (fn->returnsRefOrConstRef()) {
    return computeTupleWithIntent(INTENT_REF, retType);
  } else {
    // Compute the tuple type without any refs
    return computeNonRefTuple(retType);
  }
}

static void
resolveSpecifiedReturnType(FnSymbol* fn) {
  Type* retType;

  resolveBlockStmt(fn->retExprType);
  retType = fn->retExprType->body.tail->typeInfo();

  if (SymExpr* se = toSymExpr(fn->retExprType->body.tail)) {
    // Try resolving global type aliases
    if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE))
      retType = resolveTypeAlias(se);

    if (retType->symbol->hasFlag(FLAG_GENERIC)) {
      SET_LINENO(fn->retExprType->body.tail);
      // Try resolving records/classes with default types
      // e.g. range
      retType = resolveDefaultGenericTypeSymExpr(se);
    }
  }
  fn->retType = retType;

  if (retType != dtUnknown) {

    // Difficult to call e.g. _unref_type in build/normalize
    // b/c of bugs in pulling out function symbols.
    // So we make sure returned tuple types capture values here.
    if (retType->symbol->hasFlag(FLAG_TUPLE) &&
        !doNotChangeTupleTypeRefLevel(fn, true)) {
      AggregateType* tupleType = toAggregateType(retType);
      INT_ASSERT(tupleType);

      retType = getReturnedTupleType(fn, tupleType);
      fn->retType = retType;
    } else if (fn->returnsRefOrConstRef()) {
      makeRefType(retType);
      retType = fn->retType->refType;
      fn->retType = retType;
    }

    fn->retExprType->remove();

    if (fn->isIterator() && !fn->iteratorInfo) {
      // Note: protoIteratorClass changes fn->retType
      // to the iterator record. The original return type
      // is stored here in retType.
      protoIteratorClass(fn);
    }
  }

   // Also update the return symbol type
   Symbol* ret = fn->getReturnSymbol();
   if (ret->type == dtUnknown) {
     // uses the local variable saving the resolved declared return type
     // since for iterators, fn->retType is the iterator record.
     ret->type = retType;
   }
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
//   test/studies/shootout/spectral-norm/spectralnorm.chpl
//   test/release/examples/benchmarks/ssca2/SSCA2_main.chpl
//   test/parallel/taskPar/sungeun/barrier/*.chpl
//
static bool convertAtomicFormalTypeToRef(ArgSymbol* formal, FnSymbol* fn) {
  return (formal->intent == INTENT_BLANK &&
          !formal->hasFlag(FLAG_TYPE_VARIABLE) &&
          isAtomicType(formal->type))
    && fn->name != astrSequals
    && !fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR)
    && !fn->hasFlag(FLAG_CONSTRUCTOR)
    && !fn->hasFlag(FLAG_BUILD_TUPLE);
}


void
resolveFormals(FnSymbol* fn) {
  static Vec<FnSymbol*> done;

  if (!fn->hasFlag(FLAG_GENERIC)) {
    if (done.set_in(fn))
      return;
    done.set_add(fn);

    for_formals(formal, fn) {
      if (formal->type == dtUnknown) {
        if (!formal->typeExpr) {
          formal->type = dtObject;
        } else {
          resolveBlockStmt(formal->typeExpr);
          formal->type = formal->typeExpr->body.tail->getValType();
        }
      }

      //
      // Fix up value types that need to be ref types.
      //
      if (formal->type->symbol->hasFlag(FLAG_REF))
        // Already a ref type, so done.
        continue;

      // Don't pass dtString params in by reference
      if(formal->type == dtString && formal->hasFlag(FLAG_INSTANTIATED_PARAM))
        continue;

      if (formal->type->symbol->hasFlag(FLAG_RANGE) &&
          (formal->intent == INTENT_BLANK || formal->intent == INTENT_IN) &&
          fn->hasFlag(FLAG_BEGIN)) {
        // For begin functions, copy ranges in if passed by blank intent.
        // This is a temporary workaround.
        // * arguably blank intent for ranges should be 'const ref',
        //   but the current compiler uses a mix of 'const in' and 'const ref'.
        // * resolveIntents is changing INTENT_IN to INTENT_REF
        //   which interferes with the logic in parallel.cpp
        //   (another approach to that problem might be to separately
        //    store the 'requested intent' and the 'concrete intent').
        formal->intent = INTENT_CONST_IN;
      }

      if (formal->intent == INTENT_INOUT ||
          formal->intent == INTENT_OUT ||
          formal->intent == INTENT_REF ||
          formal->intent == INTENT_CONST_REF ||
          convertAtomicFormalTypeToRef(formal, fn) ||
          formal->hasFlag(FLAG_WRAP_WRITTEN_FORMAL) ||
          (formal == fn->_this &&
           !formal->hasFlag(FLAG_TYPE_VARIABLE) &&
           (isUnion(formal->type) ||
            isRecord(formal->type)))) {
        makeRefType(formal->type);
        if (formal->type->refType) {
          formal->type = formal->type->refType;
          // The type of the formal is its own ref type!
        } else {
          formal->qual = QUAL_REF;
        }
      }

      if (isRecordWrappedType(formal->type) &&
          fn->hasFlag(FLAG_ITERATOR_FN)) {
        // Pass domains, arrays into iterators by ref.
        // This is a temporary workaround for issues with
        // iterator lowering.
        // It is temporary because we expect more of the
        // compiler to handle 'refness' of an ArgSymbol
        // in the future.
        makeRefType(formal->type);
        formal->type = formal->type->refType;
      }

      // Adjust tuples for intent.
      // This should not apply to 'ref' , 'out', or 'inout' formals,
      // but these are currently turned into reference types above.
      // It probably should not apply to 'const ref' either but
      // that is more debatable.
      if (formal->type->symbol->hasFlag(FLAG_TUPLE) &&
          !formal->hasFlag(FLAG_TYPE_VARIABLE) &&
          !(formal == fn->_this) &&
          !doNotChangeTupleTypeRefLevel(fn, false)) {

        // Let 'in' intent work similarly to the blank intent.
        IntentTag intent = formal->intent;
        if (intent == INTENT_IN) intent = INTENT_BLANK;
        AggregateType* tupleType = toAggregateType(formal->type);
        INT_ASSERT(tupleType);
        Type* newType = computeTupleWithIntent(intent, tupleType);
        formal->type = newType;
      }

    }
    if (fn->retExprType)
      resolveSpecifiedReturnType(fn);

    resolvedFormals.set_add(fn);
  }
}

static bool fits_in_int_helper(int width, int64_t val) {
  switch (width) {
    default: INT_FATAL("bad width in fits_in_int_helper");
    case 1:
      return (val == 0 || val == 1);
    case 8:
      return (val >= INT8_MIN && val <= INT8_MAX);
    case 16:
      return (val >= INT16_MIN && val <= INT16_MAX);
    case 32:
      return (val >= INT32_MIN && val <= INT32_MAX);
    case 64:
      // As an int64_t will always fit within a 64 bit int.
      return true;
  }
}

static bool fits_in_int(int width, Immediate* imm) {
  if (imm->const_kind == NUM_KIND_INT && imm->num_index == INT_SIZE_DEFAULT) {
    int64_t i = imm->int_value();
    return fits_in_int_helper(width, i);
  }


  /* BLC: There is some question in my mind about whether this
     function should include the following code as well -- that is,
     whether default-sized uint params should get the same special
     treatment in cases like this.  I didn't enable it for now because
     nothing seemed to rely on it and I didn't come up with a case
     that would.  But it's worth keeping around for future
     consideration.

     Similarly, we may want to consider enabling such param casts for
     int sizes other then default-width.

  else if (imm->const_kind == NUM_KIND_UINT &&
           imm->num_index == INT_SIZE_DEFAULT) {
    uint64_t u = imm->uint_value();
    int64_t i = (int64_t)u;
    if (i < 0)
      return false;
    return fits_in_int_helper(width, i);
  }*/

  return false;
}

static bool fits_in_uint_helper(int width, uint64_t val) {
  switch (width) {
  default: INT_FATAL("bad width in fits_in_uint_helper");
  case 8:
    return (val <= UINT8_MAX);
  case 16:
    return (val <= UINT16_MAX);
  case 32:
    return (val <= UINT32_MAX);
  case 64:
    // As a uint64_t will always fit inside a 64 bit uint.
    return true;
  }
}

static bool fits_in_uint(int width, Immediate* imm) {
  if (imm->const_kind == NUM_KIND_INT && imm->num_index == INT_SIZE_DEFAULT) {
    int64_t i = imm->int_value();
    if (i < 0)
      return false;
    return fits_in_uint_helper(width, (uint64_t)i);
  }

  /* BLC: See comment just above in fits_in_int()...

  else if (imm->const_kind == NUM_KIND_UINT && imm->num_index == INT_SIZE_64) {
    uint64_t u = imm->uint_value();
    return fits_in_uint_helper(width, u);
  }*/

  return false;
}


// Is this a legal actual argument where an l-value is required?
// I.e. for an out/inout/ref formal.
static bool
isLegalLvalueActualArg(ArgSymbol* formal, Expr* actual) {
  Symbol* calledFn = formal->defPoint->parentSymbol;
  if (SymExpr* se = toSymExpr(actual))
    if (se->symbol()->hasFlag(FLAG_EXPR_TEMP) ||
        ((se->symbol()->hasFlag(FLAG_REF_TO_CONST) ||
          se->symbol()->isConstant()) && !formal->hasFlag(FLAG_ARG_THIS)) ||
        se->symbol()->isParameter())
      // But ignore for now errors with this argument
      // to functions marked with FLAG_REF_TO_CONST_WHEN_CONST_THIS.
      // These will be checked for later, along with ref-pairs.
      if (! (formal->hasFlag(FLAG_ARG_THIS) &&
             calledFn->hasFlag(FLAG_REF_TO_CONST_WHEN_CONST_THIS)))
      return false;
  // Perhaps more checks are needed.
  return true;
}


// Is this a legal actual argument for a 'const ref' formal?
// At present, params cannot be passed to 'const ref'.
static bool
isLegalConstRefActualArg(ArgSymbol* formal, Expr* actual) {
  bool retval = true;

  if (SymExpr* se = toSymExpr(actual))
    if (se->symbol()->isParameter()                   ==  true &&
        isString(se->symbol())                        == false)
      retval = false;

  return retval;
}

/* If we have a generic parent, e.g.:
   class Parent { type t; proc foo(){} }
   class Child : Parent { }

   when we have e.g. Child(int) and we call foo,
   we need we to instantiate Parent.foo() to Parent(int).foo().
   In that case, the actual is Child(int) and the formal is Parent.
   We need to return Parent(int), which should be the dispatch
   parent for Child(int).

   This table helps one to understand the situation

                                              (formal)
     Parent(int) ------ instantiatedFrom --->  Parent
         |                                      |
      dispatch child                         dispatch child
         |                                      |
         v                                      v
     Child(int)  ------ instantiatedFrom ---> Child
      (actual)

   This function detects that situation and returns the type that
   should be instantiated.
*/
Type* getConcreteParentForGenericFormal(Type* actualType, Type* formalType) {
  Type* retval = NULL;

  forv_Vec(Type, parent, actualType->dispatchParents) {
    if (isInstantiation(parent, formalType) == true) {
      retval = parent;
      break;

    } else if (Type* t = getConcreteParentForGenericFormal(parent,
                                                           formalType)) {
      retval = t;
      break;
    }
  }

  return retval;
}

// Returns true iff dispatching the actualType to the formalType
// results in an instantiation.
bool canInstantiate(Type* actualType, Type* formalType) {
  if (actualType == dtMethodToken) {
    return false;
  }

  if (formalType == dtAny) {
    return true;
  }

  if (formalType == dtIntegral &&
      (is_int_type(actualType) || is_uint_type(actualType))) {
    return true;
  }

  if (formalType == dtAnyEnumerated && is_enum_type(actualType)) {
    return true;
  }

  if (formalType == dtNumeric &&
      (is_int_type(actualType)  ||
       is_uint_type(actualType) ||
       is_imag_type(actualType) ||
       is_real_type(actualType) ||
       is_complex_type(actualType))) {
    return true;
  }

  if (formalType == dtAnyComplex && is_complex_type(actualType)) {
    return true;
  }

  if (formalType == dtString && actualType == dtStringC) {
    return true;
  }

  if (formalType == dtStringC && actualType == dtStringCopy) {
    return true;
  }

  if (formalType                                        == dtIteratorRecord &&
      actualType->symbol->hasFlag(FLAG_ITERATOR_RECORD) == true) {
    return true;
  }

  if (formalType                                        == dtIteratorClass &&
      actualType->symbol->hasFlag(FLAG_ITERATOR_CLASS)  == true) {
    return true;
  }

  if (actualType == formalType) {
    return true;
  }

  if (AggregateType* atActual = toAggregateType(actualType)) {
    if (AggregateType* atFrom = atActual->instantiatedFrom) {
      if (canInstantiate(atFrom, formalType) == true) {
        return true;
      }

      if (formalType->symbol->hasFlag(FLAG_GENERIC)                 == true &&
          getConcreteParentForGenericFormal(actualType, formalType) != NULL) {
        return true;
      }
    }
  }

  return false;
}


//
// returns true if dispatching from actualType to formalType results
// in a compile-time coercion; this is a subset of canCoerce below as,
// for example, real(32) cannot be coerced to real(64) at compile-time
//
static bool canParamCoerce(Type*   actualType,
                           Symbol* actualSym,
                           Type*   formalType) {
  if (is_bool_type(formalType) && is_bool_type(actualType)) {
    return true;
  }

  if (is_int_type(formalType)) {
    if (is_bool_type(actualType)) {
      return true;
    }

    if (is_int_type(actualType) &&
        get_width(actualType) < get_width(formalType)) {
      return true;
    }

    if (is_uint_type(actualType) &&
        get_width(actualType) < get_width(formalType)) {
      return true;
    }

    //
    // If the actual is an enum, check to see if *all* its values
    // are small enough that they fit into this integer width
    //
    if (EnumType* etype = toEnumType(actualType)) {
      ensureEnumTypeResolved(etype);

      if (get_width(etype->getIntegerType()) <= get_width(formalType)) {
        return true;
      }
    }

    //
    // For smaller integer types, if the argument is a param, does it
    // store a value that's small enough that it could dispatch to
    // this argument?
    //
    if (get_width(formalType) < 64) {
      if (VarSymbol* var = toVarSymbol(actualSym)) {
        if (var->immediate) {
          if (fits_in_int(get_width(formalType), var->immediate)) {
            return true;
          }
        }
      }

      if (EnumType* etype = toEnumType(actualType)) {
        ensureEnumTypeResolved(etype);

        if (EnumSymbol* enumsym = toEnumSymbol(actualSym)) {
          if (Immediate* enumval = enumsym->getImmediate()) {
            if (fits_in_int(get_width(formalType), enumval)) {
              return true;
            }
          }
        }
      }
    }
  }

  if (is_uint_type(formalType)) {
    if (is_bool_type(actualType)) {
      return true;
    }

    if (is_uint_type(actualType) &&
        get_width(actualType) < get_width(formalType)) {
      return true;
    }

    if (VarSymbol* var = toVarSymbol(actualSym)) {
      if (var->immediate) {
        if (fits_in_uint(get_width(formalType), var->immediate)) {
          return true;
        }
      }
    }
  }

  // param strings can coerce between string and c_string
  if ((formalType == dtString || formalType == dtStringC) &&
      (actualType == dtString || actualType == dtStringC)) {
    if (actualSym && actualSym->isImmediate()) {
      return true;
    }
  }

  return false;
}

static
bool canCoerceTuples(Type*     actualType,
                     Symbol*   actualSym,
                     Type*     formalType,
                     FnSymbol* fn) {

  if (actualType->symbol->hasFlag(FLAG_TUPLE) &&
      formalType->symbol->hasFlag(FLAG_TUPLE)) {
    // Both are tuple types, but the types do not match.
    // Could we coerce each individual tuple element?
    // If so, we can coerce the tuples.
    AggregateType *at = toAggregateType(actualType);
    AggregateType *ft = toAggregateType(formalType);

    Type* atFieldType = NULL;
    Type* ftFieldType = NULL;

    bool starTuple = (actualType->symbol->hasFlag(FLAG_STAR_TUPLE) &&
                      formalType->symbol->hasFlag(FLAG_STAR_TUPLE));

    int i = 1;

    if (at->numFields() != ft->numFields()) {
      // Number of fields differs, so not coercible.
      return false;
    }

    for_fields(atField, at) {
      Symbol* ftField = ft->getField(i);

      bool prom = false;
      bool ok;

      atFieldType = atField->type;
      ftFieldType = ftField->type;

      // Can we coerce without promotion?
      // If the types are the same, yes
      if (atFieldType != ftFieldType) {
        ok = canDispatch(atFieldType, actualSym, ftFieldType, fn, &prom, false);

        // If we couldn't coerce or the coercion would promote, no
        if (!ok || prom)
          return false;
      }

      // For star tuples, we only needed to consider first 2 fields
      // (size and 1st element)
      if (starTuple && i == 2)
        return true;

      i++;
    }

    return true;

  }

  return false;
}



//
// returns true iff dispatching the actualType to the formalType
// results in a coercion.
//
// fn is the function being called usually but in resolveReturnType it
// is the function we're finding return types for.
bool canCoerce(Type*     actualType,
               Symbol*   actualSym,
               Type*     formalType,
               FnSymbol* fn,
               bool*     promotes) {
  if (canParamCoerce(actualType, actualSym, formalType))
    return true;

  if (is_real_type(formalType)) {
    if ((is_int_type(actualType) || is_uint_type(actualType))
        && get_width(formalType) >= 64)
      return true;
    if (is_real_type(actualType) &&
        get_width(actualType) < get_width(formalType))
      return true;
  }

  if (is_complex_type(formalType)) {
    if ((is_int_type(actualType) || is_uint_type(actualType))
        && get_width(formalType) >= 128)
      return true;

    if (is_real_type(actualType) &&
        (get_width(actualType) <= get_width(formalType)/2))
      return true;

    if (is_imag_type(actualType) &&
        (get_width(actualType) <= get_width(formalType)/2))
      return true;

    if (is_complex_type(actualType) &&
        (get_width(actualType) < get_width(formalType)))
      return true;
  }

  if (isSyncType(actualType) || isSingleType(actualType)) {
    Type* baseType = actualType->getField("valType")->type;

    return canDispatch(baseType, NULL, formalType, fn, promotes);
  }

  if (canCoerceTuples(actualType, actualSym, formalType, fn)) {
    return true;
  }

  if (actualType->symbol->hasFlag(FLAG_REF))
    return canDispatch(actualType->getValType(),
                       NULL,
                       // MPF: Should this be formalType->getValType() ?
                       formalType,
                       fn,
                       promotes);

  if (formalType == dtString && actualType == dtStringCopy)
    return true;

  if (formalType == dtStringC && actualType == dtStringCopy)
    return true;

  if (actualType->symbol->hasFlag(FLAG_C_PTR_CLASS) &&
      (formalType == dtCVoidPtr))
    return true;

  return false;
}

/************************************* | **************************************
*                                                                             *
* Returns true iff the actualType can dispatch to the formalType.             *
*                                                                             *
* The function symbol is used to avoid scalar promotion on =.                 *
* param is set if the actual is a parameter (compile-time constant).          *
* fn is the function being called                                             *
*                                                                             *
************************************** | *************************************/

static bool isGenericInstantiation(Type*     actualType,
                                   Type*     formalType,
                                   FnSymbol* fn);

bool canDispatch(Type*     actualType,
                 Symbol*   actualSym,
                 Type*     formalType,
                 FnSymbol* fn,
                 bool*     promotes,
                 bool      paramCoerce) {
  if (promotes) {
    *promotes = false;
  }

  if (actualType == formalType) {
    return true;
  }

  if (isGenericInstantiation(actualType, formalType, fn) == true) {
    return true;
  }

  // The following check against FLAG_REF ensures that 'nil' can't be
  // passed to a by-ref argument (for example, an atomic type).  I
  // found that without this, calls like autocopy(nil) became
  // ambiguous when given the choice between the completely generic
  // autocopy(x) and the autocopy(x: atomic int) (represented as
  // autocopy(x: ref(atomic int)) internally).
  //
  if (actualType                            == dtNil  &&
      isClass(formalType)                   == true   &&
      formalType->symbol->hasFlag(FLAG_REF) == false) {
    return true;
  }

  if (actualType->refType == formalType &&
      // This is a workaround for type problems with tuples
      // in implement forall intents...
      !(fn && fn->hasFlag(FLAG_BUILD_TUPLE) && fn->hasFlag(FLAG_ALLOW_REF))) {
    return true;
  }

  if (paramCoerce == false &&
      canCoerce(actualType, actualSym, formalType, fn, promotes) == true) {
    return true;
  }

  if (paramCoerce == true  &&
      canParamCoerce(actualType, actualSym, formalType) == true) {
    return true;
  }

  forv_Vec(Type, parent, actualType->dispatchParents) {
    if (parent                                              == formalType ||
        canDispatch(parent, NULL, formalType, fn, promotes) == true) {
      return true;
    }
  }

  if (fn                              != NULL        &&
      fn->name                        != astrSequals &&
      actualType->scalarPromotionType != NULL        &&
      canDispatch(actualType->scalarPromotionType, NULL, formalType, fn)) {

    if (promotes) {
      *promotes = true;
    }

    return true;
  }

  return false;
}

static bool isGenericInstantiation(Type*     actualType,
                                   Type*     formalType,
                                   FnSymbol* fn) {
  AggregateType* atActual = toAggregateType(actualType);
  AggregateType* atFormal = toAggregateType(formalType);
  bool           retval   = false;

  if (atActual                                != NULL &&
      atActual->symbol->hasFlag(FLAG_GENERIC) == true &&

      atFormal                                != NULL &&
      atFormal->isInstantiatedFrom(atActual)  == true) {

    retval = true;
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool
moreSpecific(FnSymbol* fn, Type* actualType, Type* formalType) {
  if (canDispatch(actualType, NULL, formalType, fn))
    return true;
  if (canInstantiate(actualType, formalType)) {
    return true;
  }
  return false;
}

static BlockStmt*
getParentBlock(Expr* expr) {
  for (Expr* tmp = expr->parentExpr; tmp; tmp = tmp->parentExpr) {
    if (BlockStmt* block = toBlockStmt(tmp))
      return block;
  }
  if (expr->parentSymbol) {
    FnSymbol* parentFn = toFnSymbol(expr->parentSymbol);
    if (parentFn && parentFn->instantiationPoint)
      return parentFn->instantiationPoint;
    else if (expr->parentSymbol->defPoint)
      return getParentBlock(expr->parentSymbol->defPoint);
  }
  return NULL;
}


//
// helper routine for isMoreVisible (below);
// returns true if fn1 is more visible than fn2
//
static bool
isMoreVisibleInternal(BlockStmt* block, FnSymbol* fn1, FnSymbol* fn2,
                      Vec<BlockStmt*>& visited) {
  //
  // fn1 is more visible
  //
  if (fn1->defPoint->parentExpr == block)
    return true;

  //
  // fn2 is more visible
  //
  if (fn2->defPoint->parentExpr == block)
    return false;

  visited.set_add(block);

  //
  // default to true if neither are visible
  //
  bool moreVisible = true;

  //
  // ensure f2 is not more visible via parent block, and recurse
  //
  if (BlockStmt* parentBlock = getParentBlock(block))
    if (!visited.set_in(parentBlock))
      moreVisible &= isMoreVisibleInternal(parentBlock, fn1, fn2, visited);

  //
  // ensure f2 is not more visible via module uses, and recurse
  //
  if (block && block->useList) {
    for_actuals(expr, block->useList) {
      UseStmt* use = toUseStmt(expr);
      INT_ASSERT(use);
      SymExpr* se = toSymExpr(use->src);
      INT_ASSERT(se);
      // We only care about uses of modules during function resolution, not
      // uses of enums.
      if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
        if (!visited.set_in(mod->block))
          moreVisible &= isMoreVisibleInternal(mod->block, fn1, fn2, visited);
      }
    }
  }

  return moreVisible;
}


//
// return true if fn1 is more visible than fn2 from expr
//
// assumption: fn1 and fn2 are visible from expr; if this assumption
//             is violated, this function will return true
//
static bool
isMoreVisible(Expr* expr, FnSymbol* fn1, FnSymbol* fn2) {
  //
  // common-case check to see if functions have equal visibility
  //
  if (fn1->defPoint->parentExpr == fn2->defPoint->parentExpr) {
    // Special check which makes cg-initializers inferior to user-defined constructors
    // with the same args.
    if (fn2->hasFlag(FLAG_DEFAULT_CONSTRUCTOR))
      return true;
    return false;
  }

  //
  // call helper function with visited set to avoid infinite recursion
  //
  Vec<BlockStmt*> visited;
  BlockStmt* block = toBlockStmt(expr);
  if (!block)
    block = getParentBlock(expr);
  return isMoreVisibleInternal(block, fn1, fn2, visited);
}


static bool paramWorks(Symbol* actual, Type* formalType) {
  Immediate* imm = NULL;

  if (VarSymbol* var = toVarSymbol(actual)) {
    imm = var->immediate;
  }
  if (EnumSymbol* enumsym = toEnumSymbol(actual)) {
    ensureEnumTypeResolved(toEnumType(enumsym->type));
    imm = enumsym->getImmediate();
  }
  if (imm) {
    if (is_int_type(formalType)) {
      return fits_in_int(get_width(formalType), imm);
    }
    if (is_uint_type(formalType)) {
      return fits_in_uint(get_width(formalType), imm);
    }
    if (imm->const_kind == CONST_KIND_STRING) {
      if (formalType == dtStringC && actual->type == dtString) {
        return true;
      }
    }
  }

  return false;
}


static bool considerParamMatches(Type* actualType,
                                 Type* arg1Type,
                                 Type* arg2Type) {
  /* BLC: Seems weird to have to add this; could just add it in the enum
     case if enums have to be special-cased here.  Otherwise, how are the
     int cases handled later...? */
  if (actualType->symbol->hasFlag(FLAG_REF)) {
    actualType = actualType->getValType();
  }

  if (actualType == arg1Type && actualType != arg2Type) {
    return true;
  }

  // If we don't have an exact match in the previous line, let's see if
  // we have a bool(w1) passed to bool(w2) or non-bool case;  This is
  // based on the enum case developed in r20208
  if (is_bool_type(actualType) &&
      is_bool_type(arg1Type)   &&
      !is_bool_type(arg2Type)) {
    return true;
  }

  if (actualType != arg1Type && actualType != arg2Type) {
    // Otherwise, have bool cast to default-sized integer over a smaller size
    if (is_bool_type(actualType)) {
      return considerParamMatches(dtInt[INT_SIZE_DEFAULT],
                                  arg1Type,
                                  arg2Type);
    }

    if (is_enum_type(actualType)) {
      return considerParamMatches(dtInt[INT_SIZE_DEFAULT],
                                  arg1Type,
                                  arg2Type);
    }

    if (isSyncType(actualType)) {
      return considerParamMatches(actualType->getField("valType")->type,
                                  arg1Type,
                                  arg2Type);
    }

    if (isSingleType(actualType)) {
      return considerParamMatches(actualType->getField("valType")->type,
                                  arg1Type,
                                  arg2Type);
    }
  }

  return false;
}




bool
explainCallMatch(CallExpr* call) {
  if (!call->isNamed(fExplainCall))
    return false;
  if (explainCallModule && explainCallModule != call->getModule())
    return false;
  if (explainCallLine != -1 && explainCallLine != call->linenum())
    return false;
  return true;
}


static CallExpr*
userCall(CallExpr* call) {
  if (developer)
    return call;
  // If the called function is compiler-generated or is in one of the internal
  // modules, back up the stack until a call is encountered whose target
  // function is neither.
  // TODO: This function should be rewritten so each test appears only once.
  if (call->getFunction()->hasFlag(FLAG_COMPILER_GENERATED) ||
      call->getModule()->modTag == MOD_INTERNAL) {
    for (int i = callStack.n-1; i >= 0; i--) {
      if (!callStack.v[i]->getFunction()->hasFlag(FLAG_COMPILER_GENERATED) &&
          callStack.v[i]->getModule()->modTag != MOD_INTERNAL)
        return callStack.v[i];
    }
  }
  return call;
}

static void reissueCompilerWarning(const char* str, int offset) {
  //
  // Disable compiler warnings in internal modules that are triggered
  // within a dynamic dispatch context because of potential user
  // confusion.  See note in 'issueCompileError' above.
  //
  if (inDynamicDispatchResolution)
    if (callStack.tail()->getModule()->modTag == MOD_INTERNAL &&
        callStack.head()->getModule()->modTag == MOD_INTERNAL)
      return;

  CallExpr* from = NULL;
  for (int i = callStack.n-offset; i >= 0; i--) {
    from = callStack.v[i];
    // We report calls whose target function is not compiler-generated and is
    // not defined in one of the internal modules.
    if (from->linenum() > 0 &&
        from->getModule()->modTag != MOD_INTERNAL &&
        !from->getFunction()->hasFlag(FLAG_COMPILER_GENERATED))
      break;
  }
  gdbShouldBreakHere();
  USR_WARN(from, "%s", str);
}

//
// The following several functions support const-ness checking.
// Which is tailored to how our existing Chapel code is written
// and to the current constructor story. In particular:
//
// * Const-ness of fields is not honored within constructors
// and initialize() functions
//
// * A function invoked directly from a constructor or initialize()
// are treated as if were a constructor.
//
// The implementation also tends to the case where such an invocation
// occurs inside a task function within the constructor or initialize().
//
// THESE RULES ARE INTERIM.
// They will change - and the Chapel code will need to be updated -
// for the upcoming new constructor story.
//
// Implementation note: we need to propagate the constness property
// through temp assignments, dereferences, and calls to methods with
// FLAG_REF_TO_CONST_WHEN_CONST_THIS.
//

static bool leftCallIsOrContainsRightCall(CallExpr* callLeft,
                                          CallExpr* callRight) {
  if (callLeft == callRight)
    return true;

  if (callLeft == callRight->parentExpr)
    return true;

  if (ContextCallExpr* cc = toContextCallExpr(callRight->parentExpr))
    if (callLeft == cc->parentExpr)
      return true;

  return false;
}

static void findNonTaskFnParent(CallExpr* call,
                                FnSymbol*& parent, int& stackIdx) {
  // We assume that 'call' is at the top of the call stack.
  INT_ASSERT(callStack.n >= 1);
  INT_ASSERT(leftCallIsOrContainsRightCall(callStack.v[callStack.n-1], call));

  int ix;
  for (ix = callStack.n-1; ix >= 0; ix--) {
    CallExpr* curr = callStack.v[ix];
    Symbol* parentSym = curr->parentSymbol;
    FnSymbol* parentFn = toFnSymbol(parentSym);
    if (!parentFn)
      break;
    if (!isTaskFun(parentFn)) {
      stackIdx = ix;
      parent = parentFn;
      return;
    }
  }
  // backup plan
  parent = toFnSymbol(call->parentSymbol);
  stackIdx = -1;
}

static bool isConstructorLikeFunction(FnSymbol* fn) {
  return fn->hasFlag(FLAG_CONSTRUCTOR)  == true  ||
         strcmp(fn->name, "init")       ==    0  ||
         strcmp(fn->name, "initialize") ==    0;
}

// Is 'call' in a constructor or in initialize()?
// This includes being in a task function invoked from the above.
static bool isInConstructorLikeFunction(CallExpr* call) {
  FnSymbol* parent;
  int stackIdx;
  findNonTaskFnParent(call, parent, stackIdx); // sets the args
  return parent && isConstructorLikeFunction(parent);
}

// Is the function of interest invoked from a constructor
// or initialize(), with the constructor's or initialize's 'this'
// as the receiver actual.
static bool isInvokedFromConstructorLikeFunction(int stackIdx) {
  if (stackIdx > 0) {
    CallExpr* call2 = callStack.v[stackIdx - 1];
    if (FnSymbol* parent2 = toFnSymbol(call2->parentSymbol))
     if (isConstructorLikeFunction(parent2))
      if (call2->numActuals() >= 2)
        if (SymExpr* thisArg2 = toSymExpr(call2->get(2)))
          if (thisArg2->symbol()->hasFlag(FLAG_ARG_THIS))
            return true;
  }
  return false;
}

// Check whether the actual comes from accessing a const field of 'this'
// and the call is in a function invoked directly from this's constructor.
// In such case, fields of 'this' are not considered 'const',
// so we remove the const-ness flag.
static bool checkAndUpdateIfLegalFieldOfThis(CallExpr* call, Expr* actual,
                                             FnSymbol*& nonTaskFnParent) {
  int stackIdx;
  findNonTaskFnParent(call, nonTaskFnParent, stackIdx); // sets the args

  if (SymExpr* se = toSymExpr(actual))
    if (se->symbol()->hasFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS))
      if (isInvokedFromConstructorLikeFunction(stackIdx)) {
          // Yes, this is the case we are looking for.
          se->symbol()->removeFlag(FLAG_REF_TO_CONST);
          return true;
      }

  return false;
}


// little helper
static Symbol* getBaseSymForConstCheck(CallExpr* call) {
  // ensure this is a method call
  INT_ASSERT(call->get(1)->typeInfo() == dtMethodToken);
  SymExpr* baseExpr = toSymExpr(call->get(2));
  INT_ASSERT(baseExpr); // otherwise, cannot do the checking
  return baseExpr->symbol();
}


// Report an error when storing a sync or single variable into a tuple.
// This is because currently we deallocate memory excessively in this case.
void checkForStoringIntoTuple(CallExpr* call, FnSymbol* resolvedFn)
{
  // Do not perform the checks if:
      // not building a tuple
  if (!resolvedFn->hasFlag(FLAG_BUILD_TUPLE) ||
      // sync/single tuples are used in chpl__autoCopy(x: _tuple), allow them
      resolvedFn->hasFlag(FLAG_ALLOW_REF)    ||
      // sync/single tuple *types* and params seem OK
      resolvedFn->retTag != RET_VALUE)
    return;

  for_formals_actuals(formal, actual, call)
    if (isSyncType(formal->type) || isSingleType(formal->type)) {
      const char* name = "";

      if (SymExpr* aSE = toSymExpr(actual))
        if (!aSE->symbol()->hasFlag(FLAG_TEMP))
          name = aSE->symbol()->name;

      USR_FATAL_CONT(actual,
                     "storing a sync or single variable %s in a tuple "
                     "is not currently implemented - "
                     "apply readFE() or readFF()",
                     name);
    }
}


// If 'fn' is the default assignment for a record type, return
// the name of that record type; otherwise return NULL.
static const char* defaultRecordAssignmentTo(FnSymbol* fn) {
  if (fn->name == astrSequals) {
    if (fn->hasFlag(FLAG_COMPILER_GENERATED)) {
      Type* desttype = fn->getFormal(1)->type->getValType();
      INT_ASSERT(desttype != dtUnknown); // otherwise this test is unreliable
      if (isRecord(desttype) || isUnion(desttype))
        return desttype->symbol->name;
    }
  }
  return NULL;
}


//
// finds the concrete type of a SymExpr when it refers
// to a generic type. This can happen when the generic type
// has a default concrete instantiation, e.g.
//
//   record R { type t = int; }
//
// Assumes that SET_LINENO has been called in the enclosing scope.
//
// Returns a concrete type. Replaces a SymExpr referring to
// a generic type (which could be referring to a TypeSymbol
// directly or to a VarSymbol with the flag FLAG_TYPE_VARIABLE).
//
// also handles some complicated extern vars like
//   extern var x: c_ptr(c_int)
// which do not work in the usual order of resolution.
static Type*
resolveDefaultGenericTypeSymExpr(SymExpr* te) {

  // te could refer to a type in two ways:
  //  1. it could refer to a TypeSymbol directly
  //  2. it could refer to a VarSymbol with FLAG_TYPE_VARIABLE
  Type* teRefersToType = NULL;

  if (TypeSymbol* ts = toTypeSymbol(te->symbol()))
    teRefersToType = ts->type;

  if (VarSymbol* vs = toVarSymbol(te->symbol())) {
    if (vs->hasFlag(FLAG_TYPE_VARIABLE)) {
      teRefersToType = vs->typeInfo();

      // Fix for complicated extern vars like
      // extern var x: c_ptr(c_int);
      // This really just amounts to an adjustment to the order of resolution.
      if( vs->hasFlag(FLAG_EXTERN) &&
          vs->defPoint && vs->defPoint->init &&
          vs->getValType() == dtUnknown ) {
        vs->type = resolveTypeAlias(te);
      }
    }
  }

  // Now, if te refers to a generic type, replace te with
  // a concrete type.
  if (AggregateType* ct = toAggregateType(teRefersToType)) {
    if (ct->symbol->hasFlag(FLAG_GENERIC)) {
      CallExpr* cc = new CallExpr(ct->defaultTypeConstructor->name);
      te->replace(cc);
      resolveCall(cc);
      TypeSymbol* retTS = cc->typeInfo()->symbol;
      cc->replace(new SymExpr(retTS));
      return retTS->type;
    }
  }

  return te->typeInfo();
}

void
resolveDefaultGenericType(CallExpr* call) {
  SET_LINENO(call);
  for_actuals(actual, call) {
    if (NamedExpr* ne = toNamedExpr(actual))
      actual = ne->actual;
    if (SymExpr* te = toSymExpr(actual)) {
      resolveDefaultGenericTypeSymExpr(te);
    }
  }
}

// Collect methods with a particular name from a type and from
// any type it's instantiated from.
static void collectVisibleMethodsNamed(Type*                   t,
                                       const char*             nameAstr,
                                       std::vector<FnSymbol*>& methods) {

  forv_Vec(FnSymbol, method, t->methods) {
    if (method->name == nameAstr &&
        !method->hasFlag(FLAG_INVISIBLE_FN)) {
      methods.push_back(method);
    }
  }

  if (AggregateType* at = toAggregateType(t)) {
    // Collect also methods from whatever type t is instantiated from
    if (at->instantiatedFrom != NULL) {
      collectVisibleMethodsNamed(at->instantiatedFrom, nameAstr, methods);

      // Currently it's not possible for a specific instantiation
      // to inherit from another type when the generic version
      // does not.
      return;
    }

    size_t maxChildMethods = methods.size();
    // Collect also methods from a parent class type
    forv_Vec(Type, parent, at->dispatchParents) {
      collectVisibleMethodsNamed(parent, nameAstr, methods);
    }
    // Filter out methods any of the parent methods
    // that have a signature match with the child's methods.
    // Such methods represent overrides with inheritance.
    for (size_t i = maxChildMethods; i < methods.size(); i++) {
      bool remove = false;
      for (size_t j = 0; j < maxChildMethods; j++) {
        if (methods[i] != NULL &&
            methods[j] != NULL &&
            signatureMatch(methods[i], methods[j])) {
          remove = true;
          break;
        }
      }
      if (remove)
        methods[i] = NULL;
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static FnSymbol* resolveUninsertedCall(BlockStmt* insert, CallExpr* call);
static FnSymbol* resolveUninsertedCall(Expr*      insert, CallExpr* call);

static Expr*     getInsertPointForTypeFunction(Type* type) {
  AggregateType* at     = toAggregateType(type);
  Expr*          retval = NULL;

  if (at == NULL || at->defaultInitializer == NULL) {
    retval = chpl_gen_main->body;

  } else if (BlockStmt* point = at->defaultInitializer->instantiationPoint) {
    retval = point;

  } else {
    retval = at->symbol->defPoint;
  }

  return retval;
}

static FnSymbol* resolveUninsertedCall(Type* type, CallExpr* call) {
  FnSymbol*      retval = NULL;

  Expr* where = getInsertPointForTypeFunction(type);
  if (BlockStmt* stmt = toBlockStmt(where))
    retval = resolveUninsertedCall(stmt, call);
  else
    retval = resolveUninsertedCall(where, call);

  return retval;
}

static FnSymbol* resolveUninsertedCall(BlockStmt* insert, CallExpr* call) {
  BlockStmt* block = new BlockStmt(call);

  insert->insertAtHead(block);

  resolveCall(call);

  block->remove();

  return call->resolvedFunction();
}

static FnSymbol* resolveUninsertedCall(Expr* insert, CallExpr* call) {
  BlockStmt* block = new BlockStmt(call);

  insert->insertBefore(block);

  resolveCall(call);

  block->remove();

  return call->resolvedFunction();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void resolveCall(CallExpr* call) {
  if (call->primitive) {
    switch (call->primitive->tag) {
    case PRIM_TUPLE_AND_EXPAND:
      resolveTupleAndExpand(call);
      break;

    case PRIM_TUPLE_EXPAND:
      resolveTupleExpand(call);
      break;

    case PRIM_SET_MEMBER:
      resolveSetMember(call);
      break;

    case PRIM_INIT:
    case PRIM_NO_INIT:
    case PRIM_TYPE_INIT:
      resolveDefaultGenericType(call);
      break;

    case PRIM_INIT_FIELD:
      resolveInitField(call);
      break;

    case PRIM_INIT_VAR:
      resolveInitVar(call);
      break;

    case PRIM_MOVE:
      resolveMove(call);
      break;

    case PRIM_COERCE:
      resolveCoerce(call);
      break;

    case PRIM_NEW:
      resolveNew(call);
      break;

    default:
      break;
    }

  } else {
    resolveNormalCall(call);
  }
}


FnSymbol* tryResolveCall(CallExpr* call) {
  return resolveNormalCall(call, true);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool      isGenericRecordInit(CallExpr* call);

static FnSymbol* resolveNormalCall(CallInfo& info, bool checkOnly);

static void      findVisibleFunctionsAndCandidates(
                                     CallInfo&                  info,
                                     Vec<FnSymbol*>&            visibleFns,
                                     Vec<ResolutionCandidate*>& candidates);

static int       disambiguateByMatch(CallInfo&                  info,
                                     Vec<ResolutionCandidate*>& candidates,
                                     ResolutionCandidate*&      bestRef,
                                     ResolutionCandidate*&      bestConstRef,
                                     ResolutionCandidate*&      bestValue);

static FnSymbol* resolveNormalCall(CallInfo&            info,
                                   bool                 checkOnly,
                                   ResolutionCandidate* best);

static FnSymbol* resolveNormalCall(CallInfo&            info,
                                   bool                 checkOnly,
                                   ResolutionCandidate* bestRef,
                                   ResolutionCandidate* bestConstRef,
                                   ResolutionCandidate* bestValue);

static void      resolveNormalCallConstRef(CallExpr* call);

static void      resolveNormalCallFinalChecks(CallExpr* call);

static FnSymbol* wrapAndCleanUpActuals(ResolutionCandidate* best,
                                       CallInfo&            info,
                                       bool                 followerChecks);


FnSymbol* resolveNormalCall(CallExpr* call, bool checkOnly) {
  CallInfo  info;
  FnSymbol* retval = NULL;

  if (call->id == breakOnResolveID) {
    printf("breaking on resolve call:\n");
    print_view(call);
    gdbShouldBreakHere();
  }

  temporaryInitializerFixup(call);

  resolveDefaultGenericType(call);

  if (isGenericRecordInit(call) == true) {
    retval = resolveInitializer(call);

  } else if (info.isNotWellFormed(call) == true) {
    if (checkOnly == false) {
      info.haltNotWellFormed();

    } else {
      return NULL;
    }
  } else {
    retval = resolveNormalCall(info, checkOnly);
  }

  return retval;
}

static bool isGenericRecordInit(CallExpr* call) {
  bool retval = false;

  if (UnresolvedSymExpr* ures = toUnresolvedSymExpr(call->baseExpr)) {
    if (strcmp(ures->unresolved, "init") == 0 &&
        call->numActuals()               >= 2) {
      Type* t1 = call->get(1)->typeInfo();
      Type* t2 = call->get(2)->typeInfo();

      if (t1                                  == dtMethodToken &&
          isGenericRecordWithInitializers(t2) == true) {
        retval = true;
      }
    }
  }

  return retval;
}

static FnSymbol* resolveNormalCall(CallInfo& info, bool checkOnly) {
  Vec<FnSymbol*>            visibleFns;
  Vec<ResolutionCandidate*> candidates;

  ResolutionCandidate*      bestRef    = NULL;
  ResolutionCandidate*      bestCref   = NULL;
  ResolutionCandidate*      bestVal    = NULL;

  int                       numMatches = 0;

  FnSymbol*                 retval     = NULL;

  findVisibleFunctionsAndCandidates(info, visibleFns, candidates);

  numMatches = disambiguateByMatch(info,
                                   candidates,

                                   bestRef,
                                   bestCref,
                                   bestVal);

  if (numMatches == 0) {
    if (info.call->partialTag == false) {
      if (checkOnly == false) {
        if (tryStack.n > 0) {
          tryFailure = true;

        } else if (candidates.n == 0) {
          printResolutionErrorUnresolved(info, visibleFns);

        } else {
          printResolutionErrorAmbiguous (info, candidates);
        }
      }
    }

  } else if (numMatches == 1) {
    ResolutionCandidate* best = NULL;

    if        (bestRef  != NULL) {
      best = bestRef;

    } else if (bestVal  != NULL) {
      best = bestVal;

    } else if (bestCref != NULL) {
      best = bestCref;
    }

    retval = resolveNormalCall(info, checkOnly, best);

  } else {
    retval = resolveNormalCall(info, checkOnly, bestRef, bestCref, bestVal);
  }

  forv_Vec(ResolutionCandidate*, candidate, candidates) {
    delete candidate;
  }

  return retval;
}

static FnSymbol* resolveNormalCall(CallInfo&            info,
                                   bool                 checkOnly,
                                   ResolutionCandidate* best) {
  CallExpr* call   = info.call;
  FnSymbol* retval = NULL;

  instantiateBody(best->fn);

  if (explainCallLine != 0 && explainCallMatch(call) == true) {
    USR_PRINT(best->fn, "best candidate is: %s", toString(best->fn));
  }

  if (call->partialTag                  == false ||
      best->fn->hasFlag(FLAG_NO_PARENS) == true) {
    retval = wrapAndCleanUpActuals(best, info, true);

    if (checkOnly == false &&
        retval->name                         == astrSequals &&
        isRecord(retval->getFormal(1)->type) == true        &&
        retval->getFormal(2)->type           == dtNil) {
      USR_FATAL(userCall(call),
                "type mismatch in assignment from nil to %s",
                toString(retval->getFormal(1)->type));
    } else {
      SET_LINENO(call);

      if (call->partialTag == true) {
        call->partialTag = false;
      }

      call->baseExpr->replace(new SymExpr(retval));

      resolveNormalCallConstRef(call);

      if (checkOnly == false) {
        resolveNormalCallFinalChecks(call);
      }
    }
  }

  return retval;
}

static FnSymbol* resolveNormalCall(CallInfo&            info,
                                   bool                 checkOnly,
                                   ResolutionCandidate* bestRef,
                                   ResolutionCandidate* bestConstRef,
                                   ResolutionCandidate* bestValue) {
  CallExpr*            call         = info.call;
  CallExpr*            refCall      = NULL;
  CallExpr*            valueCall    = NULL;
  CallExpr*            constRefCall = NULL;
  ResolutionCandidate* best         = NULL;
  FnSymbol*            retval       = NULL;

  if (bestRef      != NULL) {
    refCall = call;

    instantiateBody(bestRef->fn);
  }

  if (bestValue    != NULL) {
    if (bestRef == NULL) {
      valueCall = call;

    } else {
      valueCall = call->copy();

      call->insertAfter(valueCall);
    }

    instantiateBody(bestValue->fn);
  }

  if (bestConstRef != NULL) {
    constRefCall = call->copy();

    call->insertAfter(constRefCall);

    instantiateBody(bestConstRef->fn);
  }

  if        (bestRef      != NULL) {
    best = bestRef;

  } else if (bestValue    != NULL) {
    best = bestValue;

  } else if (bestConstRef != NULL) {
    best = bestConstRef;
  }

  if (explainCallLine != 0 && explainCallMatch(call) == true) {
    USR_PRINT(best->fn, "best candidate is: %s", toString(best->fn));
  }

  if (call->partialTag                  == true &&
      best->fn->hasFlag(FLAG_NO_PARENS) == false) {
    if (valueCall    != NULL) valueCall->remove();
    if (constRefCall != NULL) constRefCall->remove();

  } else {
    wrapAndCleanUpActuals(best, info, true);

    if (valueCall != NULL && valueCall != call) {
      CallInfo tmpInfo;

      if (tmpInfo.isNotWellFormed(valueCall) == true) {
        if (checkOnly == false) {
          tmpInfo.haltNotWellFormed();
        }

      } else {
        wrapAndCleanUpActuals(bestValue, tmpInfo, false);
      }
    }

    if (constRefCall != NULL) {
      CallInfo tmpInfo;

      if (tmpInfo.isNotWellFormed(constRefCall) == true) {
        if (checkOnly == false) {
          tmpInfo.haltNotWellFormed();
        }

      } else {
        wrapAndCleanUpActuals(bestConstRef, tmpInfo, false);
      }
    }

    if (call->partialTag == true) {
      call->partialTag = false;
    }

    if (checkOnly == false) {
      if (best->fn->name                         == astrSequals &&
          isRecord(best->fn->getFormal(1)->type) == true        &&
          best->fn->getFormal(2)->type           == dtNil) {
        USR_FATAL(userCall(call),
                  "type mismatch in assignment from nil to %s",
                  toString(best->fn->getFormal(1)->type));
      }
    }

    SET_LINENO(call);

    if (refCall      != NULL) {
      refCall->baseExpr->replace(new SymExpr(bestRef->fn));
    }

    if (valueCall    != NULL) {
      valueCall->baseExpr->replace(new SymExpr(bestValue->fn));
    }

    if (constRefCall != NULL) {
      constRefCall->baseExpr->replace(new SymExpr(bestConstRef->fn));
    }

    // Replace the call with a new ContextCallExpr containing 2 or 3 calls
    ContextCallExpr* contextCall = new ContextCallExpr();

    call->insertAfter(contextCall);

    if (refCall      != NULL) refCall->remove();
    if (valueCall    != NULL) valueCall->remove();
    if (constRefCall != NULL) constRefCall->remove();

    contextCall->setRefValueConstRefOptions(refCall,
                                            valueCall,
                                            constRefCall);

    if (checkOnly == false) {
      resolveNormalCallFinalChecks(call);
    }

    retval = best->fn;
  }

  return retval;
}

static void resolveNormalCallConstRef(CallExpr* call) {
  FnSymbol* fn = call->resolvedFunction();

  if (fn->retTag == RET_CONST_REF) {
    if (CallExpr* parentCall = toCallExpr(call->parentExpr)) {
      if (parentCall->isPrimitive(PRIM_MOVE) == true) {
        if (SymExpr* lhsSe = toSymExpr(parentCall->get(1))) {
          if (VarSymbol* lhs = toVarSymbol(lhsSe->symbol())) {
            if (lhs->hasFlag(FLAG_EXPR_TEMP) == true) {
              Symbol* parentSymbol = parentCall->parentSymbol;

              if (FnSymbol* inFn = toFnSymbol(parentSymbol)) {
                if (isConstructorLikeFunction(inFn) == false) {
                  lhs->addFlag(FLAG_REF_TO_CONST);
                }
              }
            }
          }
        }
      }
    }
  }
}

static void resolveNormalCallFinalChecks(CallExpr* call) {
  FnSymbol* fn = call->resolvedFunction();

  if (fn->hasFlag(FLAG_MODIFIES_CONST_FIELDS) == true) {
    // Not allowed if it is not called directly from a constructor.
    if (isInConstructorLikeFunction(call)                     == false ||
        getBaseSymForConstCheck(call)->hasFlag(FLAG_ARG_THIS) == false) {
      USR_FATAL_CONT(call,
                     "illegal call to %s() - it modifies 'const' fields "
                     "of 'this', therefore it can be invoked only directly "
                     "from a constructor on the object being constructed",
                     fn->name);
    }
  }

  lvalueCheck(call);

  checkForStoringIntoTuple(call, fn);

  resolveNormalCallCompilerWarningStuff(fn);
}

static FnSymbol* wrapAndCleanUpActuals(ResolutionCandidate* best,
                                       CallInfo&            info,
                                       bool                 followerChecks) {
  best->fn = wrapAndCleanUpActuals(best->fn,
                                   info,
                                   &best->actualIdxToFormal,
                                   followerChecks);

  return best->fn;
}

void resolveNormalCallCompilerWarningStuff(FnSymbol* resolvedFn) {
  if (const char* str = innerCompilerWarningMap.get(resolvedFn)) {
    reissueCompilerWarning(str, 2);

    if (callStack.n >= 2) {
      if (FnSymbol* fn = callStack.v[callStack.n - 2]->resolvedFunction()) {
        outerCompilerWarningMap.put(fn, str);
      }
    }
  }

  if (const char* str = outerCompilerWarningMap.get(resolvedFn)) {
    reissueCompilerWarning(str, 1);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void generateMsg(CallInfo& info, Vec<FnSymbol*>& visibleFns);

void printResolutionErrorUnresolved(CallInfo&       info,
                                    Vec<FnSymbol*>& visibleFns) {
  if (info.call == NULL) {
    INT_FATAL("call is NULL");

  } else {
    CallExpr* call = userCall(info.call);

    if (call->isCast() == true) {
      if (info.actuals.head()->hasFlag(FLAG_TYPE_VARIABLE) == false) {
        USR_FATAL_CONT(call, "illegal cast to non-type");
      } else {
        USR_FATAL_CONT(call,
                       "illegal cast from %s to %s",
                       toString(info.actuals.v[1]->type),
                       toString(info.actuals.v[0]->type));
      }

    } else if (strcmp("these", info.name) == 0) {
      if (info.actuals.n          == 2 &&
          info.actuals.v[0]->type == dtMethodToken) {

        if (info.actuals.v[1]->hasFlag(FLAG_TYPE_VARIABLE) == true) {
          USR_FATAL_CONT(call,
                         "unable to iterate over type '%s'",
                         toString(info.actuals.v[1]->type));
        } else {
          USR_FATAL_CONT(call,
                         "cannot iterate over values of type %s",
                         toString(info.actuals.v[1]->type));
        }

      } else {
        generateMsg(info, visibleFns);
      }

    } else if (strcmp("_type_construct__tuple", info.name) == 0) {
      if (info.call->argList.length == 0) {
        USR_FATAL_CONT(call, "tuple size must be specified");

      } else {
        SymExpr* sym = toSymExpr(info.call->get(1));

        if (sym == NULL) {
          USR_FATAL_CONT(call, "tuple size must be static");

        } else if (sym->symbol()->isParameter() == false) {
          USR_FATAL_CONT(call, "tuple size must be static");

        } else {
          USR_FATAL_CONT(call, "invalid tuple");
        }
      }

    } else if (info.name == astrSequals) {
      if        (info.actuals.v[0]                              !=  NULL  &&
                 info.actuals.v[1]                              !=  NULL  &&
                 info.actuals.v[0]->hasFlag(FLAG_TYPE_VARIABLE) == false  &&
                 info.actuals.v[1]->hasFlag(FLAG_TYPE_VARIABLE) ==  true) {
        USR_FATAL_CONT(call,
                       "illegal assignment of type to value");

      } else if (info.actuals.v[0]                              != NULL   &&
                 info.actuals.v[1]                              != NULL   &&
                 info.actuals.v[0]->hasFlag(FLAG_TYPE_VARIABLE) == true   &&
                 info.actuals.v[1]->hasFlag(FLAG_TYPE_VARIABLE) == false) {
        USR_FATAL_CONT(call,
                       "illegal assignment of value to type");

      } else if (info.actuals.v[1]->type == dtNil) {
        USR_FATAL_CONT(call,
                       "type mismatch in assignment from nil to %s",
                       toString(info.actuals.v[0]->type));

      } else {
        USR_FATAL_CONT(call,
                       "type mismatch in assignment from %s to %s",
                       toString(info.actuals.v[1]->type),
                       toString(info.actuals.v[0]->type));
      }

    } else if (info.name == astrThis) {
      Type* type = info.actuals.v[1]->getValType();

      if (type->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
        USR_FATAL_CONT(call,
                       "illegal access of iterator or promoted expression");

      } else if (type->symbol->hasFlag(FLAG_FUNCTION_CLASS)) {
        USR_FATAL_CONT(call,
                       "illegal access of first class function");

      } else {
        USR_FATAL_CONT(call,
                       "unresolved access of '%s' by '%s'",
                       toString(info.actuals.v[1]->type),
                       info.toString());
      }

    } else {
      generateMsg(info, visibleFns);
    }

    if (developer == true) {
      USR_PRINT(call, "unresolved call had id %i", call->id);
    }

    USR_STOP();
  }
}

void printResolutionErrorAmbiguous(CallInfo&                  info,
                                   Vec<ResolutionCandidate*>& candidates) {
  CallExpr* call       = userCall(info.call);
  bool      printedOne = false;

  if (info.name == astrThis) {
    USR_FATAL_CONT(call,
                   "ambiguous access of '%s' by '%s'",
                   toString(info.actuals.v[1]->type),
                   info.toString());

  } else {
    const char* entity = "call";
    const char* str    = info.toString();

    if (strncmp("_type_construct_", info.name, 16) == 0) {
      entity = "type specifier";
    }

    if (info.scope) {
      ModuleSymbol* mod = toModuleSymbol(info.scope->parentSymbol);

      INT_ASSERT(mod);

      str = astr(mod->name, ".", str);
    }

    USR_FATAL_CONT(call, "ambiguous %s '%s'", entity, str);
  }

  if (developer == true) {
    for (int i = callStack.n - 1; i >= 0; i--) {
      CallExpr* cs = callStack.v[i];
      FnSymbol* f  = cs->getFunction();

      if (f->instantiatedFrom) {
        USR_PRINT(callStack.v[i], "  instantiated from %s", f->name);
      } else {
        break;
      }
    }
  }

  forv_Vec(ResolutionCandidate, cand, candidates) {
    if (printedOne == false) {
      USR_PRINT(cand->fn, "candidates are: %s", toString(cand->fn));
      printedOne = true;

    } else {
      USR_PRINT(cand->fn, "                %s", toString(cand->fn));
    }
  }

  USR_STOP();
}

static void generateMsg(CallInfo& info, Vec<FnSymbol*>& visibleFns) {
  CallExpr*   call = userCall(info.call);
  const char* str  = NULL;

  if (info.scope != NULL) {
    ModuleSymbol* mod = toModuleSymbol(info.scope->parentSymbol);

    INT_ASSERT(mod);

    str = astr(mod->name, ".", info.toString());

  } else {
    str = info.toString();
  }

  if (strncmp("_type_construct_", info.name, 16) == 0) {
    USR_FATAL_CONT(call, "unresolved type specifier '%s'", str);

  } else if (info.actuals.n                              >  1             &&
             info.actuals.v[0]->getValType()             == dtMethodToken &&
             isEnumType(info.actuals.v[1]->getValType()) == true) {
    USR_FATAL_CONT(call,
                   "unresolved enumerated type symbol or call '%s'",
                   str);

  } else {
    USR_FATAL_CONT(call, "unresolved call '%s'", str);
  }

  if (visibleFns.n > 0) {
    bool printedOne = false;

    if (developer == true) {
      for (int i = callStack.n - 1; i >= 0; i--) {
        CallExpr* cs = callStack.v[i];
        FnSymbol* f  = cs->getFunction();

        if (f->instantiatedFrom != NULL) {
          USR_PRINT(callStack.v[i], "  instantiated from %s", f->name);
        } else {
          break;
        }
      }
    }

    forv_Vec(FnSymbol, fn, visibleFns) {
      if (printedOne == false) {
        USR_PRINT(fn, "candidates are: %s", toString(fn));
        printedOne = true;

      } else {
        USR_PRINT(fn, "                %s", toString(fn));
      }
    }
  }

  if (visibleFns.n                                == 1 &&
      visibleFns.v[0]->numFormals()               == 0 &&
      strncmp("_type_construct_", info.name, 16) == 0) {
    USR_PRINT(call, "did you forget the 'new' keyword?");
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void findVisibleCandidates(CallInfo&                  info,
                                  Vec<FnSymbol*>&            visibleFns,
                                  Vec<ResolutionCandidate*>& candidates);

static void gatherCandidates(CallInfo&                  info,
                             Vec<FnSymbol*>&            visibleFns,
                             bool                       lastResort,
                             Vec<ResolutionCandidate*>& candidates);

static void filterCandidate (CallInfo&                  info,
                             FnSymbol*                  fn,
                             Vec<ResolutionCandidate*>& candidates);

static bool typeUsesForwarding(Type* t);

static bool populateForwardingMethods(CallInfo& info);

static void findVisibleFunctionsAndCandidates(
                                CallInfo&                  info,
                                Vec<FnSymbol*>&            visibleFns,
                                Vec<ResolutionCandidate*>& candidates) {
  CallExpr* call = info.call;
  FnSymbol* fn   = call->resolvedFunction();

  // First, try finding candidates without forwarding
  if (fn != NULL) {
    visibleFns.add(fn);

    handleTaskIntentArgs(info, fn);

  } else {
    findVisibleFunctions(info, visibleFns);
  }

  findVisibleCandidates(info, visibleFns, candidates);

  // If no candidates were found and it's a method, try forwarding
  if (candidates.n             == 0 &&
      call->numActuals()       >= 1 &&
      call->get(1)->typeInfo() == dtMethodToken) {
    Type* receiverType = call->get(2)->typeInfo()->getValType();

    if (typeUsesForwarding(receiverType) == true &&
        populateForwardingMethods(info)  == true) {
      visibleFns.clear();

      forv_Vec(ResolutionCandidate*, candidate, candidates) {
        delete candidate;
      }

      candidates.clear();

      if (fn != NULL) {
        visibleFns.add(fn);

        handleTaskIntentArgs(info, fn);

      } else {
        findVisibleFunctions(info, visibleFns);
      }

      findVisibleCandidates(info, visibleFns, candidates);
    }
  }

  explainGatherCandidate(info, candidates);
}

static void findVisibleCandidates(CallInfo&                  info,
                                  Vec<FnSymbol*>&            visibleFns,
                                  Vec<ResolutionCandidate*>& candidates) {
  // Search user-defined (i.e. non-compiler-generated) functions first.
  gatherCandidates(info, visibleFns, false, candidates);

  // If no results, try again with any compiler-generated candidates.
  if (candidates.n == 0) {
    gatherCandidates(info, visibleFns, true, candidates);
  }
}

static void gatherCandidates(CallInfo&                  info,
                             Vec<FnSymbol*>&            visibleFns,
                             bool                       lastResort,
                             Vec<ResolutionCandidate*>& candidates) {
  forv_Vec(FnSymbol, fn, visibleFns) {
    // Only consider functions marked with/without FLAG_LAST_RESORT
    // (where existence of the flag matches the lastResort argument)
    if (fn->hasFlag(FLAG_LAST_RESORT) == lastResort) {

      // Consider
      //
      //   c1.foo(10, 20);
      //
      // where foo() is a simple method on some class/record
      //
      // Normalize currently converts this to
      //
      //   #<Call     #<Call "foo" _mt c1>    10    20>
      //
      // Resolution performs a post-order traversal of this expression
      // and so the inner call is visited before the outer call.
      //
      // In this context, the inner "call" is effectively a field access
      // rather than a true function call.  Normalize sets the methodTag
      // property to true to indicate this, and this form of call can only
      // be matched to parentheses-less methods and type constructors.
      //
      // Later steps will convert the outer call to become
      //
      //   #<Call "foo" _mt c1  10    20>
      //
      // This outer call has methodTag set to false and this call
      // should be filtered against the available visibleFunctions.
      //

      if (info.call->methodTag == false) {
        filterCandidate(info, fn, candidates);

      } else {
        if (fn->hasFlag(FLAG_NO_PARENS)        == true ||
            fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) == true) {
          filterCandidate(info, fn, candidates);
        }
      }
    }
  }
}

static void filterCandidate(CallInfo&                  info,
                            FnSymbol*                  fn,
                            Vec<ResolutionCandidate*>& candidates) {
  ResolutionCandidate* candidate = new ResolutionCandidate(fn);

  if (fExplainVerbose &&
      ((explainCallLine && explainCallMatch(info.call)) ||
       info.call->id == explainCallID)) {
    USR_PRINT(fn, "Considering function: %s", toString(fn));

    if (info.call->id == breakOnResolveID) {
      gdbShouldBreakHere();
    }
  }

  if (candidate->isApplicable(info) == true) {
    candidates.add(candidate);
  } else {
    delete candidate;
  }
}

static bool typeUsesForwarding(Type* t) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(t)) {
    retval = (toForwardingStmt(at->forwardingTo.head) != NULL) ? true : false;
  }

  return retval;
}

static bool populateForwardingMethods(CallInfo& info) {
  CallExpr*      forCall    = info.call;
  const char*    calledName = info.name;
  Type*          t          = forCall->get(2)->typeInfo()->getValType();

  AggregateType* at         = toAggregateType(t);
  bool           addedAny   = false;

  // Currently, only AggregateTypes can forward
  if (at == NULL) {
    return false;
  }

  // If the type has not yet been resolved, stop,
  // since otherwise computing the forwarding fn won't go well.
  for_fields(field, at) {
    if (field->type == dtUnknown) {
      return false;
    }
  }

  // try resolving the call on the forwarding expressions
  for_alist(expr, at->forwardingTo) {
    ForwardingStmt* delegate = toForwardingStmt(expr);

    INT_ASSERT(delegate);

    // Forwarding method should use line number of forwarding stmt
    SET_LINENO(delegate);

    const char* fnGetTgt   = delegate->fnReturningForwarding;
    const char* methodName = calledName;

    if (delegate->type == NULL) {
      SET_LINENO(at->symbol);

      delegate->type = dtUnknown; // avoiding loop from recursion

      Symbol*   tmp        = newTemp(at);

      at->symbol->defPoint->insertBefore(new DefExpr(tmp));

      CallExpr* getTgtCall = new CallExpr(fnGetTgt, gMethodToken, tmp);
      FnSymbol* fn         = resolveUninsertedCall(at, getTgtCall);

      resolveFns(fn);

      Type* delegateType = fn->retType->getValType();

      tmp->defPoint->remove();

      INT_ASSERT(delegateType != dtUnknown);

      delegate->type = delegateType;
    }

    // Adjust methodName for rename processing.
    if (delegate->renamed.count(calledName) > 0) {
      methodName = delegate->renamed[calledName];

    } else if (delegate->named.count(calledName)) {
      if (delegate->except) {
        // don't handle this symbol
        methodName = NULL;

      } else {
        // OK, calledName is in the only list.
      }

    } else {
      // It's not a specifically mentioned symbol.
      // It's OK if:
      //  - there was no list at all, or
      //  - the list was an 'except' list
      if ((delegate->renamed.size() == 0 && delegate->named.size() == 0) ||
          delegate->except) {
        // OK
      } else {
        methodName = NULL;
      }
    }

    // Stop processing this delegate if we've ruled out this name.
    if (methodName == NULL) {
      continue;
    }

    // Make sure methodName is a blessed string
    methodName = astr(methodName);

    // Populate delegate->scratchFn now
    if (delegate->scratchFn == NULL) {
      FnSymbol* scratch = new FnSymbol("delegate_scratch_fn");
      scratch->addFlag(FLAG_COMPILER_GENERATED);

      Expr* where = getInsertPointForTypeFunction(at);
      if (BlockStmt* stmt = toBlockStmt(where))
        stmt->insertAtHead(new DefExpr(scratch));
      else
        stmt->insertBefore(new DefExpr(scratch));

      normalize(scratch);

      delegate->scratchFn = scratch;
    }

    // There are 2 ways that more methods can be added to
    // delegate->type during resolution:
    //   1) delegate->type itself use a 'delegate'
    //   2) delegate->type is a generic instantiation
    //      and the method in question hasn't been instantiated yet
    //
    // We handle 1 by resolving a call here to the method in question.
    // We handle 2 below by creating a generic wrapper for a generic function.
    {
      BlockStmt* block    = new BlockStmt();
      Type*      testType = delegate->type;
      Symbol*    tmp      = newTemp(testType);

      CallExpr*  test     = new CallExpr(new UnresolvedSymExpr(methodName),
                                         gMethodToken,
                                         tmp);


      int        i        = 0;

      // The test call should have the same parentheses-less/partial
      // properties as the call we are working with.
      test->methodTag = forCall->methodTag;
      test->partialTag = forCall->partialTag;

      for_actuals(actual, forCall) {
        if (i > 1) { // skip method token, object
          test->insertAtTail(actual->copy());
        }

        i++;
      }

      block->insertAtTail(new DefExpr(tmp));
      block->insertAtTail(test);

      delegate->scratchFn->insertAtHead(block);

      tryResolveCall(test);
    }

    // Now, forward all methods named 'methodName' as 'calledName'.
    // Forward generic functions as generic functions.

    std::vector<FnSymbol*> methods;

    collectVisibleMethodsNamed(delegate->type, methodName, methods);

    // Compute the type of `this` for use in the forwarding function.
    AggregateType* thisType = at;

    while (thisType->instantiatedFrom != NULL) {
      thisType = thisType->instantiatedFrom;
    }

    for_vector(FnSymbol, method, methods) {
      // ignore holes
      if (method == NULL)
        continue;

      // Name should already be filtered out
      INT_ASSERT(method->name == methodName);

      // We shouldn't have collected any invisible fns /
      // instantiations of generics
      INT_ASSERT(!method->hasFlag(FLAG_INVISIBLE_FN));
      INT_ASSERT(method->instantiatedFrom == NULL);

      // Skip any methods that don't match parentheses-less
      // vs parentheses-ful vs the call.
      if (method->hasFlag(FLAG_NO_PARENS) != forCall->methodTag) {
        continue;
      }

      // Skip any methods that are init/ctor/dtor
      // These cannot yet be forwarded.
      if (method->hasFlag(FLAG_DESTRUCTOR)  == true ||
          method->hasFlag(FLAG_CONSTRUCTOR) == true ||
          strcmp(methodName, "init")        ==    0) {
        continue;
      }

      // This wrapper method will be added to the type at and so will be found
      // through normal resolution processes if this comes up agin.
      addedAny = true;

      // Create a "wrapper" method that forwards to the delegate
      FnSymbol* fn = new FnSymbol(calledName);

      fn->copyFlags(method);

      // but we need to resolve the wrapper method again
      fn->removeFlag(FLAG_RESOLVED);

      // Never give an error when returning 'void' from a forwarding fn
      fn->removeFlag(FLAG_VOID_NO_RETURN_VALUE);

      // Also, don't consider it an iterator, since instead it is a
      // function returning an iterator.
      //  (e.g. proc these() return _value.these(); )
      fn->removeFlag(FLAG_ITERATOR_FN);

      fn->addFlag(FLAG_METHOD);
      fn->addFlag(FLAG_INLINE);
      fn->addFlag(FLAG_FORWARDING_FN);
      fn->addFlag(FLAG_COMPILER_GENERATED);

      // Mark it as generic if `this` argument is generic
      if (thisType->symbol->hasFlag(FLAG_GENERIC))
        fn->addFlag(FLAG_GENERIC);

      fn->addFlag(FLAG_LAST_RESORT);

      fn->retTag = method->retTag;

      ArgSymbol* mt    = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
      ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", thisType);

      _this->addFlag(FLAG_ARG_THIS);

      fn->insertFormalAtTail(mt);
      fn->insertFormalAtTail(_this);

      fn->_this = _this;

      // Add a call to the original function
      VarSymbol* tgt = newTemp("tgt");
      tgt->addFlag(FLAG_MAYBE_REF);

      CallExpr* getTgt   = new CallExpr(fnGetTgt, gMethodToken, _this);
      CallExpr* setTgt   = new CallExpr(PRIM_MOVE, tgt, getTgt);
      CallExpr* wrapCall = new CallExpr(new UnresolvedSymExpr(method->name),
                                        gMethodToken,
                                        tgt);

      // Create an argument symbol that we're about to replace
      // with a call to fnGetTgt.
      // This enables forwarding a function that used 'this' in
      // its argument type or default value declarations.
      ArgSymbol* dummyThis = new ArgSymbol(INTENT_BLANK,
                                           "dummyThis",
                                           dtUnknown);
      fn->insertFormalAtHead(dummyThis);

      SymbolMap map;

      // Add mt, dummyThis to SymbolMap since these were computed above
      map.put(method->getFormal(1), mt);
      map.put(method->getFormal(2), dummyThis);

      // Add the arguments to the wrapper function
      // Add the arguments to the call
      int i = 0;

      for_formals(formal, method) {
        if (i > 1) { // skip method token, target - added above
          // Pass map so that later arguments will use it
          // to replace uses of old formals with the new formals
          DefExpr* def = formal->defPoint->copy(&map);

          fn->insertFormalAtTail(def);

          wrapCall->insertAtTail(new SymExpr(def->sym));
        }

        i++;
      }

      // copy the where clause
      if (method->where != NULL) {
        fn->where = method->where->copy(&map);
      }

      // Add forwarding method to the tree so that
      // for_SymbolSymExprs below will work.
      at->symbol->defPoint->insertBefore(new DefExpr(fn));

      std::map<BlockStmt*, VarSymbol*> addedTgts;

      // Fix up any uses of dummyThis to call fnGetTgt.
      for_SymbolSymExprs(se, dummyThis) {
        // Find the parent BlockStmt to insert fixTgt in to.
        Expr* insert = se;

        while ( insert->parentExpr != NULL ) {
          insert = insert->parentExpr;
        }

        INT_ASSERT(insert && isBlockStmt(insert));

        BlockStmt* block  = toBlockStmt(insert);
        VarSymbol* fixTgt = NULL;

        // does this BlockStmt already have a call?
        if (addedTgts.count(block)) {
          fixTgt = addedTgts[block];
        } else {
          // add the call to fnGetTgt and DefExpr for fixTgt.
          fixTgt = newTemp("tgt");

          fixTgt->addFlag(FLAG_MAYBE_REF);

          CallExpr* fixGetTgt = new CallExpr(fnGetTgt, gMethodToken, _this);
          CallExpr* fixSetTgt = new CallExpr(PRIM_MOVE, fixTgt, fixGetTgt);

          block->insertAtHead(fixSetTgt);
          block->insertAtHead(new DefExpr(fixTgt));

          addedTgts[block] = fixTgt;
        }
        se->replace(new SymExpr(fixTgt));
      }

      // Remove dummyThis from the function
      dummyThis->defPoint->remove();

      // at this point, we don't know the return type for
      // wrapCall, and we don't want to resolve it yet.
      // to work with other code in resolution that tolerates
      // "returning" a dtVoid, we take some steps to "normalize"
      // here. Calling normalize directly would result in a PRIM_DEREF
      // that interferes with accepting void returns.
      VarSymbol* retval = newTemp("ret", dtUnknown);
      retval->addFlag(FLAG_RVV);

      fn->body->insertAtTail(new DefExpr(retval));
      fn->body->insertAtTail(new DefExpr(tgt));
      fn->body->insertAtTail(setTgt);
      fn->body->insertAtTail(new CallExpr(PRIM_MOVE, retval, wrapCall));
      fn->body->insertAtTail(new CallExpr(PRIM_RETURN, retval));

      // Add the new function as a method.
      at->methods.add(fn);
    }
  }

  return addedAny;
}

/************************************* | **************************************
*                                                                             *
* Find the best return-intent overloads from a list of candidates.            *
*                                                                             *
* If there was ambiguity, bestRef, bestConstRef, and bestValue will be NULL,  *
* and the vector ambiguous will store any functions that participated in the  *
* ambiguity (i.e. the multiple best matches).                                 *
*                                                                             *
************************************** | *************************************/

// Allow disambiguation tracing to be controlled by the command-line option
// --explain-verbose.
#define ENABLE_TRACING_OF_DISAMBIGUATION 1

#ifdef ENABLE_TRACING_OF_DISAMBIGUATION

#define EXPLAIN(...)                                  \
        if (developer && DC.explain) fprintf(stderr, __VA_ARGS__)

#else

#define EXPLAIN(...)

#endif


static ResolutionCandidate*
disambiguateByMatch(Vec<ResolutionCandidate*>&   candidates,
                    const DisambiguationContext& DC,
                    bool                         ignoreWhere,
                    Vec<ResolutionCandidate*>&   ambiguous);


static ResolutionCandidate*
disambiguateByMatch(Vec<ResolutionCandidate*>&   candidates,
                    const DisambiguationContext& DC,
                    bool                         ignoreWhere,
                    bool                         forGenericInit,
                    Vec<ResolutionCandidate*>&   ambiguous);

static int  compareSpecificity(ResolutionCandidate*         candidate1,
                               ResolutionCandidate*         candidate2,
                               const DisambiguationContext& DC,
                               int                          i,
                               int                          j,
                               bool                         ignoreWhere,
                               bool                         forGenericInit);

static void testArgMapping(FnSymbol*                    fn1,
                           ArgSymbol*                   formal1,
                           FnSymbol*                    fn2,
                           ArgSymbol*                   formal2,
                           Symbol*                      actual,
                           const DisambiguationContext& DC,
                           int                          i,
                           int                          j,
                           DisambiguationState&         DS);

ResolutionCandidate*
disambiguateForInit(CallInfo& info, Vec<ResolutionCandidate*>& candidates) {
  DisambiguationContext     DC(info);
  Vec<ResolutionCandidate*> ambiguous;

  return disambiguateByMatch(candidates, DC, false, true, ambiguous);
}

static int disambiguateByMatch(CallInfo&                  info,
                               Vec<ResolutionCandidate*>& candidates,

                               ResolutionCandidate*&      bestRef,
                               ResolutionCandidate*&      bestConstRef,
                               ResolutionCandidate*&      bestValue) {
  DisambiguationContext     DC(info);

  Vec<ResolutionCandidate*> ambiguous;

  ResolutionCandidate*      best   = disambiguateByMatch(candidates,
                                                         DC,
                                                         true,
                                                         ambiguous);

  int                       retval = 0;

  // The common case is that there is no ambiguity because the
  // return intent overload feature is not used.
  if (best != NULL) {
    if (best->fn->retTag == RET_REF) {
      bestRef = best;

    } else if(best->fn->retTag == RET_CONST_REF) {
      bestConstRef = best;

    } else {
      bestValue = best;
    }

    retval = 1;

  } else {
    // Now, if there was ambiguity, find candidates with different
    // return intent in ambiguousCandidates.
    // If there is only one of each, we are good to go.
    int                  nRef              = 0;
    int                  nConstRef         = 0;
    int                  nValue            = 0;
    int                  nOther            = 0;
    int                  total             = 0;

    ResolutionCandidate* refCandidate      = NULL;
    ResolutionCandidate* constRefCandidate = NULL;
    ResolutionCandidate* valueCandidate    = NULL;

    // Count number of candidates in each category.
    forv_Vec(ResolutionCandidate*, candidate, ambiguous) {
      RetTag retTag = candidate->fn->retTag;

      if (retTag == RET_REF) {
        refCandidate = candidate;
        nRef++;

      } else if(retTag == RET_CONST_REF) {
        constRefCandidate = candidate;
        nConstRef++;

      } else if(retTag == RET_VALUE) {
        valueCandidate = candidate;
        nValue++;

      } else {
        nOther++;
      }
    }

    total = nRef + nConstRef + nValue + nOther;

    // 0 matches -> return now, not a ref pair.
    if (total == 0) {
      retval = 0;

    // 1 match   -> It should not be possible to get here
    } else if (total == 1) {
      INT_ASSERT(false);

    } else if (nOther > 0) {
      ambiguous.clear();

      // If there are *any* type/param candidates, we need to cause ambiguity
      // if they are not selected... including consideration of where clauses.
      bestValue  = disambiguateByMatch(candidates, DC, false, ambiguous);
      if (bestValue)
        retval = 1;
      else
        retval = 0;

    } else {
      if (nRef > 1 || nConstRef > 1 || nValue > 1) {
        // Split candidates into ref, const ref, and value candidates
        Vec<ResolutionCandidate*> refCandidates;
        Vec<ResolutionCandidate*> constRefCandidates;
        Vec<ResolutionCandidate*> valueCandidates;
        Vec<ResolutionCandidate*> tmpAmbiguous;

        // Move candidates to above Vecs according to return intent
        forv_Vec(ResolutionCandidate*, candidate, candidates) {
          RetTag retTag = candidate->fn->retTag;

          if (retTag == RET_REF) {
            refCandidates.push_back(candidate);

          } else if (retTag == RET_CONST_REF) {
            constRefCandidates.push_back(candidate);

          } else if (retTag == RET_VALUE) {
            valueCandidates.push_back(candidate);
          }
        }

        // Disambiguate each group
        refCandidate      = disambiguateByMatch(refCandidates,
                                                DC,
                                                false,
                                                tmpAmbiguous);

        constRefCandidate = disambiguateByMatch(constRefCandidates,
                                                DC,
                                                false,
                                                tmpAmbiguous);

        valueCandidate    = disambiguateByMatch(valueCandidates,
                                                DC,
                                                false,
                                                tmpAmbiguous);
        // update the counts
        if (refCandidate      != NULL) nRef      = 1;
        if (constRefCandidate != NULL) nConstRef = 1;
        if (valueCandidate    != NULL) nValue    = 1;
      }

      // Now we know there are >= 2 matches.
      // If there are more than 2 matches in any category, fail for ambiguity.
      if (nRef > 1 || nConstRef > 1 || nValue > 1) {
        retval = 0;

      } else {
        bestRef      = refCandidate;
        bestConstRef = constRefCandidate;
        bestValue    = valueCandidate;

        int nBestRef      = bestRef      != NULL ? 1 : 0;
        int nBestValue    = bestValue    != NULL ? 1 : 0;
        int nBestConstRef = bestConstRef != NULL ? 1 : 0;

        retval = nBestRef + nBestValue + nBestConstRef;
      }
    }
  }

  return retval;
}

static ResolutionCandidate*
disambiguateByMatch(Vec<ResolutionCandidate*>&   candidates,
                    const DisambiguationContext& DC,
                    bool                         ignoreWhere,
                    Vec<ResolutionCandidate*>&   ambiguous) {
  return disambiguateByMatch(candidates, DC, ignoreWhere, false, ambiguous);
}


static ResolutionCandidate*
disambiguateByMatch(Vec<ResolutionCandidate*>&   candidates,
                    const DisambiguationContext& DC,
                    bool                         ignoreWhere,
                    bool                         forGenericInit,
                    Vec<ResolutionCandidate*>&   ambiguous) {
  // MPF note: A more straightforwardly O(n) version of this
  // function did not appear to be faster. See history of this comment.

  // If index i is set then we can skip testing function F_i because
  // we already know it can not be the best match.
  std::vector<bool> notBest(candidates.n, false);

  for (int i = 0; i < candidates.n; ++i) {
    EXPLAIN("##########################\n");
    EXPLAIN("# Considering function %d #\n", i);
    EXPLAIN("##########################\n\n");

    ResolutionCandidate* candidate1         = candidates.v[i];
    bool                 singleMostSpecific = true;

    EXPLAIN("%s\n\n", toString(candidate1->fn));

    if (notBest[i]) {
      EXPLAIN("Already known to not be best match.  Skipping.\n\n");
      continue;
    }

    for (int j = 0; j < candidates.n; ++j) {
      if (i == j) {
        continue;
      }

      EXPLAIN("Comparing to function %d\n", j);
      EXPLAIN("-----------------------\n");

      ResolutionCandidate* candidate2 = candidates.v[j];

      EXPLAIN("%s\n", toString(candidate2->fn));

      int cmp = compareSpecificity(candidate1,
                                   candidate2,
                                   DC,
                                   i,
                                   j,
                                   ignoreWhere,
                                   forGenericInit);

      if (cmp < 0) {
        EXPLAIN("X: Fn %d is a better match than Fn %d\n\n\n", i, j);
        notBest[j] = true;

      } else if (cmp > 0) {
        EXPLAIN("X: Fn %d is a worse match than Fn %d\n\n\n", i, j);
        notBest[i] = true;
        singleMostSpecific = false;
        break;
      } else {
        EXPLAIN("X: Fn %d is a as good a match as Fn %d\n\n\n", i, j);
        singleMostSpecific = false;
        break;
      }
    }

    if (singleMostSpecific) {
      EXPLAIN("Y: Fn %d is the best match.\n\n\n", i);
      return candidate1;

    } else {
      EXPLAIN("Y: Fn %d is NOT the best match.\n\n\n", i);
    }
  }

  EXPLAIN("Z: No non-ambiguous best match.\n\n");

  for (int i = 0; i < candidates.n; ++i) {
    if (notBest[i] == false) {
      ambiguous.add(candidates.v[i]);
    }
  }

  return NULL;
}

/** Determines if fn1 is a better match than fn2.
 *
 * This function implements the function comparison component of the
 * disambiguation procedure as detailed in section 13.13 of the Chapel
 * language specification.
 *
 * \param candidate1 The function on the left-hand side of the comparison.
 * \param candidate2 The function on the right-hand side of the comparison.
 * \param DC         The disambiguation context.
 * \param ignoreWhere Set to `true` to ignore `where` clauses when
 *                    deciding if one match is better than another.
 *                    This is important for resolving return intent
 *                    overloads.
 *
 * \return -1 if fn1 is a more specific function than f2
 * \return 0 if fn1 and fn2 are equally specific
 * \return 1 if fn2 is a more specific function than f1
 */
static int compareSpecificity(ResolutionCandidate*         candidate1,
                              ResolutionCandidate*         candidate2,
                              const DisambiguationContext& DC,
                              int                          i,
                              int                          j,
                              bool                         ignoreWhere,
                              bool                         forGenericInit) {

  DisambiguationState DS;

  // Initializer work-around: Skip _mt/_this for generic initializers
  int                 start   = (forGenericInit == false) ? 0 : 2;

  bool                prefer1 = false;
  bool                prefer2 = false;

  for (int k = start; k < DC.actuals->n; ++k) {
    Symbol*    actual  = DC.actuals->v[k];
    ArgSymbol* formal1 = candidate1->actualIdxToFormal[k];
    ArgSymbol* formal2 = candidate2->actualIdxToFormal[k];

    EXPLAIN("\nLooking at argument %d\n", k);

    testArgMapping(candidate1->fn,
                   formal1,
                   candidate2->fn,
                   formal2,
                   actual,
                   DC,
                   i,
                   j,
                   DS);
  }

  if (DS.fn1Promotes != DS.fn2Promotes) {
    EXPLAIN("\nP: Fn %d does not require argument promotion; Fn %d does\n",
                                DS.fn1Promotes ? j : i,
                                DS.fn1Promotes ? i : j);

    // Prefer the version that did not promote
    prefer1 = !DS.fn1Promotes;
    prefer2 = !DS.fn2Promotes;

  } else if (DS.fn1MoreSpecific != DS.fn2MoreSpecific) {
    prefer1 = DS.fn1MoreSpecific;
    prefer2 = DS.fn2MoreSpecific;

  } else {
    // If the decision hasn't been made based on the argument mappings...
    if (isMoreVisible(DC.scope, candidate1->fn, candidate2->fn)) {
      EXPLAIN("\nQ: Fn %d is more specific\n", i);
      prefer1 = true;

    } else if (isMoreVisible(DC.scope, candidate2->fn, candidate1->fn)) {
      EXPLAIN("\nR: Fn %d is more specific\n", j);
      prefer2 = true;

    } else if (DS.paramPrefers == 1) {
      EXPLAIN("\nS: Fn %d is more specific\n", i);
      prefer1 = true;

    } else if (DS.paramPrefers == 2) {
      EXPLAIN("\nT: Fn %d is more specific\n", j);
      prefer2 = true;

    } else if (!ignoreWhere) {
      bool fn1where = candidate1->fn->where != NULL &&
                      !candidate1->fn->hasFlag(FLAG_COMPILER_ADDED_WHERE);
      bool fn2where = candidate2->fn->where != NULL &&
                      !candidate2->fn->hasFlag(FLAG_COMPILER_ADDED_WHERE);
      if (fn1where && !fn2where) {
        EXPLAIN("\nU: Fn %d is more specific\n", i);
        prefer1 = true;

      } else if (!fn1where && fn2where) {
        EXPLAIN("\nV: Fn %d is more specific\n", j);
        prefer2 = true;
      }
    }
  }

  INT_ASSERT(!(prefer1 && prefer2));

  if (prefer1) {
    EXPLAIN("\nW: Fn %d is more specific than Fn %d\n",
                                i, j);
    return -1;

  } else if (prefer2) {
    EXPLAIN("\nW: Fn %d is less specific than Fn %d\n",
                                i, j);
    return 1;

  } else {
    // Neither is more specific
    EXPLAIN("\nW: Fn %d and Fn %d are equally specific\n",
                                i, j);
    return 0;
  }
}

/** Compare two argument mappings, given a set of actual arguments, and set the
 *  disambiguation state appropriately.
 *
 * This function implements the argument mapping comparison component of the
 * disambiguation procedure as detailed in section 13.14.3 of the Chapel
 * language specification (page 107).
 *
 * \param fn1     The first function to be compared.
 * \param formal1 The formal argument that correspond to the actual argument
 *                for the first function.
 * \param fn2     The second function to be compared.
 * \param formal2 The formal argument that correspond to the actual argument
 *                for the second function.
 * \param actual  The actual argument from the call site.
 * \param DC      The disambiguation context.
 * \param DS      The disambiguation state.
 */
static void testArgMapping(FnSymbol*                    fn1,
                           ArgSymbol*                   formal1,
                           FnSymbol*                    fn2,
                           ArgSymbol*                   formal2,
                           Symbol*                      actual,
                           const DisambiguationContext& DC,
                           int                          i,
                           int                          j,
                           DisambiguationState&         DS) {
  // We only want to deal with the value types here, avoiding odd overloads
  // working (or not) due to _ref.
  Type* f1Type          = formal1->type->getValType();
  Type* f2Type          = formal2->type->getValType();
  Type* actualType      = actual->type->getValType();

  bool  formal1Promotes = false;
  bool  formal2Promotes = false;

  EXPLAIN("Actual's type: %s\n", toString(actualType));

  canDispatch(actualType, actual, f1Type, fn1, &formal1Promotes);

  DS.fn1Promotes |= formal1Promotes;

  EXPLAIN("Formal 1's type: %s\n", toString(f1Type));

  if (formal1Promotes) {
    EXPLAIN("Actual requires promotion to match formal 1\n");

  } else {
    EXPLAIN("Actual DOES NOT require promotion to match formal 1\n");
  }

  if (formal1->hasFlag(FLAG_INSTANTIATED_PARAM)) {
    EXPLAIN("Formal 1 is an instantiated param.\n");

  } else {
    EXPLAIN("Formal 1 is NOT an instantiated param.\n");
  }

  canDispatch(actualType, actual, f2Type, fn1, &formal2Promotes);

  DS.fn2Promotes |= formal2Promotes;

  EXPLAIN("Formal 2's type: %s\n", toString(f2Type));

  if (formal2Promotes) {
    EXPLAIN("Actual requires promotion to match formal 2\n");
  } else {
    EXPLAIN("Actual DOES NOT require promotion to match formal 2\n");
  }

  if (formal2->hasFlag(FLAG_INSTANTIATED_PARAM)) {
    EXPLAIN("Formal 2 is an instantiated param.\n");
  } else {
    EXPLAIN("Formal 2 is NOT an instantiated param.\n");
  }

  if (f1Type == f2Type &&
      formal1->hasFlag(FLAG_INSTANTIATED_PARAM) &&
      !formal2->hasFlag(FLAG_INSTANTIATED_PARAM)) {
    EXPLAIN("A: Fn %d is more specific\n", i);
    DS.fn1MoreSpecific = true;

  } else if (f1Type == f2Type &&
             !formal1->hasFlag(FLAG_INSTANTIATED_PARAM) &&
             formal2->hasFlag(FLAG_INSTANTIATED_PARAM)) {
    EXPLAIN("B: Fn %d is more specific\n", j);
    DS.fn2MoreSpecific = true;

  } else if (!formal1Promotes && formal2Promotes) {
    EXPLAIN("C: Fn %d is more specific\n", i);
    DS.fn1MoreSpecific = true;

  } else if (formal1Promotes && !formal2Promotes) {
    EXPLAIN("D: Fn %d is more specific\n", j);
    DS.fn2MoreSpecific = true;

  } else if (f1Type == f2Type           &&
             !formal1->instantiatedFrom &&
             formal2->instantiatedFrom) {
    EXPLAIN("E: Fn %d is more specific\n", i);
    DS.fn1MoreSpecific = true;

  } else if (f1Type == f2Type &&
             formal1->instantiatedFrom &&
             !formal2->instantiatedFrom) {
    EXPLAIN("F: Fn %d is more specific\n", j);
    DS.fn2MoreSpecific = true;

  } else if (formal1->instantiatedFrom != dtAny &&
             formal2->instantiatedFrom == dtAny) {
    EXPLAIN("G: Fn %d is more specific\n", i);
    DS.fn1MoreSpecific = true;

  } else if (formal1->instantiatedFrom == dtAny &&
             formal2->instantiatedFrom != dtAny) {
    EXPLAIN("H: Fn %d is more specific\n", j);
    DS.fn2MoreSpecific = true;

  } else if (formal1->instantiatedFrom &&
             formal2->instantiatedFrom &&
             formal1->hasFlag(FLAG_NOT_FULLY_GENERIC) &&
             !formal2->hasFlag(FLAG_NOT_FULLY_GENERIC)) {
    EXPLAIN("G1: Fn %d is more specific\n", i);
    DS.fn1MoreSpecific = true;

  } else if (formal1->instantiatedFrom &&
             formal2->instantiatedFrom &&
             !formal1->hasFlag(FLAG_NOT_FULLY_GENERIC) &&
             formal2->hasFlag(FLAG_NOT_FULLY_GENERIC)) {
    EXPLAIN("G2: Fn %d is more specific\n", i);
    DS.fn2MoreSpecific = true;

  } else if (considerParamMatches(actualType, f1Type, f2Type)) {
    EXPLAIN("In first param case\n");

    // The actual matches formal1's type, but not formal2's
    if (paramWorks(actual, f2Type)) {
      // but the actual is a param and works for formal2
      if (formal1->hasFlag(FLAG_INSTANTIATED_PARAM)) {
        // the param works equally well for both, but matches
        // the first lightly better if we had to decide
        DS.updateParamPrefers(1, "formal1", DC);

      } else if (formal2->hasFlag(FLAG_INSTANTIATED_PARAM)) {
        DS.updateParamPrefers(2, "formal2", DC);

      } else {
        // neither is a param, but formal1 is an exact type
        // match, so prefer that one
        DS.updateParamPrefers(1, "formal1", DC);
      }

    } else {
      EXPLAIN("I: Fn %d is more specific\n", i);
      DS.fn1MoreSpecific = true;
    }

  } else if (considerParamMatches(actualType, f2Type, f1Type)) {
    EXPLAIN("In second param case\n");

    // The actual matches formal2's type, but not formal1's
    if (paramWorks(actual, f1Type)) {
      // but the actual is a param and works for formal1
      if (formal2->hasFlag(FLAG_INSTANTIATED_PARAM)) {
        // the param works equally well for both, but matches
        // the second slightly better if we had to decide
        DS.updateParamPrefers(2, "formal2", DC);

      } else if (formal1->hasFlag(FLAG_INSTANTIATED_PARAM)) {
        DS.updateParamPrefers(1, "formal1", DC);

      } else {
        // neither is a param, but formal1 is an exact type match,
        // so prefer that one
        DS.updateParamPrefers(2, "formal2", DC);
      }

    } else {
      EXPLAIN("J: Fn %d is more specific\n", j);
      DS.fn2MoreSpecific = true;
    }

  } else if (moreSpecific(fn1, f1Type, f2Type) && f2Type != f1Type) {
    EXPLAIN("K: Fn %d is more specific\n", i);
    DS.fn1MoreSpecific = true;

  } else if (moreSpecific(fn1, f2Type, f1Type) && f2Type != f1Type) {
    EXPLAIN("L: Fn %d is more specific\n", j);
    DS.fn2MoreSpecific = true;

  } else if (is_int_type(f1Type) && is_uint_type(f2Type)) {
    EXPLAIN("M: Fn %d is more specific\n", i);
    DS.fn1MoreSpecific = true;

  } else if (is_int_type(f2Type) && is_uint_type(f1Type)) {
    EXPLAIN("N: Fn %d is more specific\n", j);
    DS.fn2MoreSpecific = true;

  } else {
    EXPLAIN("O: no information gained from argument\n");
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void  captureTaskIntentValues(int        argNum,
                                     ArgSymbol* formal,
                                     Expr*      actual,
                                     Symbol*    varActual,
                                     CallInfo&  info,
                                     FnSymbol*  taskFn);

static void  verifyTaskFnCall(BlockStmt* parent, CallExpr* call);

static Expr* parentToMarker(BlockStmt* parent, CallExpr* call);

//
// Copy the type of the actual into the type of the corresponding formal
// of a task function. Also do captureTaskIntentValues() when needed.
//
static void handleTaskIntentArgs(CallInfo& info, FnSymbol* taskFn) {
  CallExpr* call = info.call;

  INT_ASSERT(taskFn);

  if (needsCapture(taskFn) == false) {
    // A task function should have args only if it needsCapture.
    if (taskFn->hasFlag(FLAG_ON) == true) {
      // Documenting the current state: fn_on gets a chpl_localeID_t arg.
      INT_ASSERT(call->numActuals() == 1);

    } else {
      INT_ASSERT(call->numActuals() == 0 || isTaskFun(taskFn) == false);
    }

  } else {
    int argNum = 0;

    for_formals_actuals(formal, actual, call) {
      SymExpr* symexpActual = toSymExpr(actual);

      if (symexpActual == NULL) {
        // We add NamedExpr args in propagateExtraLeaderArgs().
        NamedExpr* namedexpActual = toNamedExpr(actual);

        INT_ASSERT(namedexpActual);

        symexpActual = toSymExpr(namedexpActual->actual);
      }

      INT_ASSERT(symexpActual); // because of how we invoke a task function

      Symbol* varActual = symexpActual->symbol();

      // If 'call' is in a generic function, it will have been instantiated by
      // now. Otherwise our task function has to remain generic.
      INT_ASSERT(varActual->type->symbol->hasFlag(FLAG_GENERIC) == false);

      // Need to copy varActual->type even for type variables.
      // BTW some formals' types may have been set in createTaskFunctions().
      formal->type = varActual->type;

      // If the actual is a ref, still need to capture it => remove ref.
      if (isReferenceType(varActual->type) == true) {
        Type* deref = varActual->type->getValType();

        // todo: replace needsCapture() with always resolveArgIntent(formal)
        // then checking (formal->intent & INTENT_FLAG_IN)
        if (needsCapture(deref) == true) {
          formal->type = deref;

          // If the formal has a ref intent, DO need a ref type => restore it.
          resolveArgIntent(formal);

          if (formal->intent & INTENT_FLAG_REF) {
            formal->type = varActual->type;
          }

          if (varActual->isConstant() == true) {
            int newIntent = formal->intent | INTENT_FLAG_CONST;

            // and clear INTENT_FLAG_MAYBE_CONST flag
            newIntent      &= ~INTENT_FLAG_MAYBE_CONST;
            formal->intent =  (IntentTag) newIntent;
          }
        }
      }

      if (varActual->hasFlag(FLAG_TYPE_VARIABLE) == true) {
        formal->addFlag(FLAG_TYPE_VARIABLE);
      }

      // This does not capture records/strings that are passed
      // by blank or const intent. As of this writing (6'2015)
      // records and strings are (incorrectly) captured at the point
      // when the task function/arg bundle is created.
      if (taskFn->hasFlag(FLAG_COBEGIN_OR_COFORALL) == true &&
          varActual->isConstValWillNotChange()      == false &&
          (concreteIntent(formal->intent, formal->type->getValType())
           & INTENT_FLAG_IN)) {
        // skip dummy_locale_arg: chpl_localeID_t
        if (argNum != 0 || taskFn->hasFlag(FLAG_ON) == false) {
          captureTaskIntentValues(argNum,
                                  formal,
                                  actual,
                                  varActual,
                                  info,
                                  taskFn);
        }
      }

      argNum = argNum + 1;
    }

    // Even if some formals are (now) types, if 'taskFn' remained generic,
    // gatherCandidates() would not instantiate it, for some reason.
    taskFn->removeFlag(FLAG_GENERIC);
  }
}

//
// Generate code to store away the value of 'varActual' before
// the cobegin or the coforall loop starts. Use this value
// instead of 'varActual' as the actual to the task function,
// meaning (later in compilation) in the argument bundle.
//
// This is to ensure that all task functions use the same value
// for their respective formal when that has an 'in'-like intent,
// even if 'varActual' is modified between creations of
// the multiple task functions.
//
static void captureTaskIntentValues(int        argNum,
                                    ArgSymbol* formal,
                                    Expr*      actual,
                                    Symbol*    varActual,
                                    CallInfo&  info,
                                    FnSymbol*  taskFn) {
  CallExpr*  call   = info.call;
  BlockStmt* parent = toBlockStmt(call->parentExpr);

  INT_ASSERT(parent);

  if (taskFn->hasFlag(FLAG_ON) && !parent->isForLoop()) {
    // coforall ... { on ... { .... }} ==> there is an intermediate BlockStmt
    parent = toBlockStmt(parent->parentExpr);

    INT_ASSERT(parent);
  }

  if (fVerify == true) {
    if (argNum == 0 || (argNum == 1 && taskFn->hasFlag(FLAG_ON) == true)) {
      verifyTaskFnCall(parent, call); //assertions only
    }
  }

  Expr* marker = parentToMarker(parent, call);

  if (varActual->hasFlag(FLAG_NO_CAPTURE_FOR_TASKING) == true) {

  } else if (varActual->defPoint->parentExpr == parent) {
    // Index variable of the coforall loop? Do not capture it!
    INT_ASSERT(varActual->hasFlag(FLAG_COFORALL_INDEX_VAR));

  } else {
    SymbolMap*& symap   = capturedValues[parent->id];
    Symbol*     capTemp = NULL;

    if (symap != NULL) {
      capTemp = symap->get(varActual);

    } else {
      symap = new SymbolMap();
    }

    if (capTemp == NULL) {
      capTemp = newTemp(astr(formal->name, "_captemp"), formal->type);

      marker->insertBefore(new DefExpr(capTemp));

      if (hasAutoCopyForType(formal->type) == true) {
        FnSymbol* autoCopy = getAutoCopy(formal->type);

        marker->insertBefore("'move'(%S,%S(%S))",
                             capTemp,
                             autoCopy,
                             varActual);

      } else if (isReferenceType(varActual->type) ==  true &&
                 isReferenceType(capTemp->type)   == false) {
        marker->insertBefore("'move'(%S,'deref'(%S))", capTemp, varActual);

      } else {
        marker->insertBefore("'move'(%S,%S)", capTemp, varActual);
      }

      symap->put(varActual, capTemp);
    }

    actual->replace(new SymExpr(capTemp));

    INT_ASSERT(info.actuals.v[argNum] == varActual);

    info.actuals.v[argNum] = capTemp;
  }
}

// Ensure 'parent' is the block before which we want to do the capturing.
static void verifyTaskFnCall(BlockStmt* parent, CallExpr* call) {
  if (call->isNamed("coforall_fn") == true ||
      call->isNamed("on_fn")       == true) {
    INT_ASSERT(parent->isForLoop());

  } else if (call->isNamed("cobegin_fn") == true) {
    DefExpr* first = toDefExpr(parent->getFirstExpr());

    // just documenting the current state
    INT_ASSERT(first && !strcmp(first->sym->name, "_cobeginCount"));

  } else {
    INT_ASSERT(call->isNamed("begin_fn"));
  }
}

//
// Returns the expression that we want to capture before.
//
// Why not just 'parent'? In users/shetag/fock/fock-dyn-prog-cntr.chpl,
// we cannot do parent->insertBefore() because parent->list is null.
// That's because we have: if ... then cobegin ..., so 'parent' is
// immediately under CondStmt. This motivated me for cobegins to capture
// inside of the 'parent' block, at the beginning of it.
//
static Expr* parentToMarker(BlockStmt* parent, CallExpr* call) {
  Expr*  retval = parent;

  if (call->isNamed("cobegin_fn") == true) {
    DefExpr*  def  = toDefExpr(parent->body.head);
    CallExpr* move = toCallExpr(def->next);
    SymExpr*  arg1 = toSymExpr(move->get(1));

    INT_ASSERT(strcmp(def->sym->name, "_cobeginCount") == 0);
    INT_ASSERT(arg1->symbol()                          == def->sym);

    retval = move->next;
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void lvalueCheck(CallExpr* call) {
  // Check to ensure the actual supplied to an OUT, INOUT or REF argument
  // is an lvalue.
  for_formals_actuals(formal, actual, call) {
    bool errorMsg = false;
    switch (formal->intent) {
     case INTENT_BLANK:
     case INTENT_CONST:
     case INTENT_PARAM:
     case INTENT_TYPE:
     case INTENT_REF_MAYBE_CONST:
      // not checking them here
      break;

     case INTENT_IN:
     case INTENT_CONST_IN:
      // generally, not checking them here
      // but, FLAG_COPY_MUTATES makes INTENT_IN actually modify actual
      if (formal->type->symbol->hasFlag(FLAG_COPY_MUTATES))
        if (!isLegalLvalueActualArg(formal, actual))
          errorMsg = true;
      break;

     case INTENT_INOUT:
     case INTENT_OUT:
     case INTENT_REF:
      if (!isLegalLvalueActualArg(formal, actual))
        errorMsg = true;
      break;

     case INTENT_CONST_REF:
      if (!isLegalConstRefActualArg(formal, actual))
        errorMsg = true;
      break;

     default:
      // all intents should be covered above
      INT_ASSERT(false);
      break;
    }

    FnSymbol* nonTaskFnParent = NULL;

    if (errorMsg &&
        // sets nonTaskFnParent
        checkAndUpdateIfLegalFieldOfThis(call, actual, nonTaskFnParent)) {
      errorMsg = false;

      nonTaskFnParent->addFlag(FLAG_MODIFIES_CONST_FIELDS);
    }

    if (errorMsg == true) {
      if (nonTaskFnParent->hasFlag(FLAG_SUPPRESS_LVALUE_ERRORS)) {
        // we are asked to ignore errors here
        return;
      }

      FnSymbol* calleeFn = call->resolvedFunction();

      INT_ASSERT(calleeFn == formal->defPoint->parentSymbol); // sanity

      if (calleeFn->hasFlag(FLAG_ASSIGNOP)) {
        // This assert is FYI. Perhaps can remove it if it fails.
        INT_ASSERT(callStack.n > 0 && callStack.v[callStack.n - 1] == call);

        FnSymbol*   fnParent   = toFnSymbol(call->parentSymbol);
        const char* recordName = defaultRecordAssignmentTo(fnParent);

        if (recordName && callStack.n >= 2) {
          // blame on the caller of the caller, if available
          USR_FATAL_CONT(callStack.v[callStack.n - 2],
                         "cannot assign to a record of the type %s using "
                         "the default assignment operator because it has "
                         "'const' field(s)",
                         recordName);
        } else {
          USR_FATAL_CONT(actual, "illegal lvalue in assignment");
        }

      } else {
        ModuleSymbol* mod          = calleeFn->getModule();
        char          cn1          = calleeFn->name[0];
        const char*   calleeParens = (isalpha(cn1) || cn1 == '_') ? "()" : "";

        // Should this be the same condition as in insertLineNumber() ?
        if (developer || mod->modTag == MOD_USER) {
          USR_FATAL_CONT(actual,
                         "non-lvalue actual is passed to %s formal '%s' "
                         "of %s%s",
                         formal->intentDescrString(),
                         formal->name,
                         calleeFn->name,
                         calleeParens);

        } else {
          USR_FATAL_CONT(actual,
                         "non-lvalue actual is passed to a %s formal of "
                         "%s%s",
                         formal->intentDescrString(),
                         calleeFn->name,
                         calleeParens);
        }
      }

      if (SymExpr* aSE = toSymExpr(actual)) {
        Symbol* aVar = aSE->symbol();

        if (aVar->hasFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT)) {
          printTaskOrForallConstErrorNote(aVar);
        }
      }
    }
  }
}

void printTaskOrForallConstErrorNote(Symbol* aVar) {
  const char* varname = aVar->name;

  if (strncmp(varname, "_formal_tmp_", 12) == 0) {
    varname += 12;
  }

  if (isArgSymbol(aVar) || aVar->hasFlag(FLAG_TEMP)) {
    Symbol*     enclTaskFn    = aVar->defPoint->parentSymbol;
    BaseAST*    marker        = NULL;
    const char* constructName = NULL;

    if (enclTaskFn->hasFlag(FLAG_BEGIN)) {
      // enclTaskFn points to a good line number
      marker        = enclTaskFn;
      constructName = "begin";

    } else {
      marker        = enclTaskFn->defPoint->parentExpr;
      constructName = "parallel";
    }

    USR_PRINT(marker,
              "The shadow variable '%s' is constant due to task intents "
              "in this %s statement",
              varname,
              constructName);

  } else {
    Expr* enclLoop = aVar->defPoint->parentExpr;

    USR_PRINT(enclLoop,
              "The shadow variable '%s' is constant due to forall intents "
              "in this loop",
              varname);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void resolveTupleAndExpand(CallExpr* call) {
  Expr*      stmt = call->getStmtExpr();
  SymExpr*   se   = toSymExpr(call->get(1));
  int        size = 0;
  CallExpr*  noop = new CallExpr(PRIM_NOOP);
  VarSymbol* tmp  = gTrue;

  for (int i = 0; i < se->symbol()->type->substitutions.n && size == 0; i++) {
    SymbolMapElem& elem = se->symbol()->type->substitutions.v[i];

    if (elem.key != NULL && strcmp("size", elem.key->name) == 0) {
      size = toVarSymbol(elem.value)->immediate->int_value();
    }
  }

  INT_ASSERT(size > 0);

  stmt->insertBefore(noop);

  for (int i = 1; i <= size; i++) {
    VarSymbol* tmp1 = newTemp("_tuple_and_expand_tmp_");
    VarSymbol* tmp2 = newTemp("_tuple_and_expand_tmp_");
    VarSymbol* tmp3 = newTemp("_tuple_and_expand_tmp_");
    VarSymbol* tmp4 = newTemp("_tuple_and_expand_tmp_");

    tmp1->addFlag(FLAG_MAYBE_PARAM);
    tmp1->addFlag(FLAG_MAYBE_TYPE);

    tmp2->addFlag(FLAG_MAYBE_PARAM);
    tmp2->addFlag(FLAG_MAYBE_TYPE);

    tmp3->addFlag(FLAG_MAYBE_PARAM);
    tmp3->addFlag(FLAG_MAYBE_TYPE);

    tmp4->addFlag(FLAG_MAYBE_PARAM);
    tmp4->addFlag(FLAG_MAYBE_TYPE);

    stmt->insertBefore(new DefExpr(tmp1));
    stmt->insertBefore(new DefExpr(tmp2));
    stmt->insertBefore(new DefExpr(tmp3));
    stmt->insertBefore(new DefExpr(tmp4));

    CallExpr* index  = new CallExpr(se->copy(), new_IntSymbol(i));
    CallExpr* query  = new CallExpr(PRIM_QUERY, tmp1);
    CallExpr* test   = new CallExpr("==",       tmp2, call->get(3)->copy());
    CallExpr* bitAnd = new CallExpr("&",        tmp3, tmp);

    for (int j = 2; j < call->numActuals(); j++) {
      query->insertAtTail(call->get(j)->copy());
    }

    stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp1, index));
    stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp2, query));
    stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp3, test));
    stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp4, bitAnd));

    tmp = tmp4;
  }

  call->replace(new SymExpr(tmp));

  noop->replace(call); // put call back in ast for function resolution

  call->convertToNoop();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void resolveTupleExpand(CallExpr* call) {
  SymExpr*  sym    = toSymExpr(call->get(1));
  Type*     type   = sym->symbol()->getValType();

  CallExpr* parent = toCallExpr(call->parentExpr);
  CallExpr* noop   = new CallExpr(PRIM_NOOP);
  Expr*     stmt   = call->getStmtExpr();

  int       size   = 0;

  if (type->symbol->hasFlag(FLAG_TUPLE) == false) {
    USR_FATAL(call, "invalid tuple expand primitive");
  }

  for (int i = 0; i < type->substitutions.n && size == 0; i++) {
    SymbolMapElem& elem = type->substitutions.v[i];

    if (elem.key != NULL && strcmp("size", elem.key->name) == 0) {
      size = toVarSymbol(elem.value)->immediate->int_value();
    }
  }

  if (size == 0) {
    INT_FATAL(call, "Invalid tuple expand primitive");
  }

  stmt->insertBefore(noop);

  for (int i = 1; i <= size; i++) {
    VarSymbol* tmp = newTemp("_tuple_expand_tmp_");
    CallExpr*  e   = NULL;

    tmp->addFlag(FLAG_MAYBE_TYPE);

    if (sym->symbol()->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      tmp->addFlag(FLAG_TYPE_VARIABLE);
    }

    call->insertBefore(new SymExpr(tmp));

    if (call->parentSymbol->hasFlag(FLAG_EXPAND_TUPLES_WITH_VALUES) == false) {
      e = new CallExpr(sym->copy(), new_IntSymbol(i));
    } else {
      e = new CallExpr(PRIM_GET_MEMBER_VALUE,
                       sym->copy(),
                       new_CStringSymbol(astr("x", istr(i))));
    }

    stmt->insertBefore(new DefExpr(tmp));
    stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, e));
  }

  call->remove();

  noop->replace(call); // put call back in ast for function resolution

  call->convertToNoop();

  // increase tuple rank
  if (parent != NULL && parent->isNamed("_type_construct__tuple") == true) {
    Symbol* rank = new_IntSymbol(parent->numActuals() - 1);

    parent->get(1)->replace(new SymExpr(rank));
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void handleSetMemberTypeMismatch(Type*     t,
                                        Symbol*   fs,
                                        CallExpr* call,
                                        SymExpr*  rhs);

static void resolveSetMember(CallExpr* call) {
  // Get the field name.
  SymExpr* sym = toSymExpr(call->get(2));

  if (sym == NULL) {
    INT_FATAL(call, "bad set member primitive");
  }

  VarSymbol* var = toVarSymbol(sym->symbol());

  if (var == NULL || var->immediate == NULL) {
    INT_FATAL(call, "bad set member primitive");
  }

  const char* name = var->immediate->v_string;

  // Special case: An integer field name is actually a tuple member index.
  {
    int64_t i = 0;

    if (get_int(sym, &i) == true) {
      name = astr("x", istr(i));

      call->get(2)->replace(new SymExpr(new_CStringSymbol(name)));
    }
  }

  AggregateType* ct = toAggregateType(call->get(1)->typeInfo()->getValType());

  if (ct == NULL) {
    INT_FATAL(call, "bad set member primitive");
  }

  Symbol* fs = NULL;

  for_fields(field, ct) {
    if (strcmp(field->name, name) == 0) {
      fs = field;
      break;
    }
  }

  if (fs == NULL) {
    INT_FATAL(call, "bad set member primitive");
  }

  Type* t = call->get(3)->typeInfo();

  if (t == dtUnknown) {
    INT_FATAL(call, "Unable to resolve field type");
  }

  INT_ASSERT(isFnSymbol(call->parentSymbol));
  if (isGenericInstantiation(fs->type, t, toFnSymbol(call->parentSymbol))) {
    fs->type = t;
  }

  if (fs->type == dtUnknown) {
    if (t != dtNil) {
      fs->type = t;

    } else {
      USR_FATAL(call->parentSymbol,
                "unable to determine type of field from nil");
    }
  }

  INT_ASSERT(isSymExpr(call->get(3)));

  handleSetMemberTypeMismatch(t, fs, call, toSymExpr(call->get(3)));
}

static void handleSetMemberTypeMismatch(Type*     t,
                                        Symbol*   fs,
                                        CallExpr* call,
                                        SymExpr*  rhs) {
  if (t != fs->type && t != dtNil && t != dtObject) {
    Symbol*   actual = rhs->symbol();
    FnSymbol* fn     = toFnSymbol(call->parentSymbol);

    if (canCoerceTuples(t, actual, fs->type, fn) == true) {
      // Add a PRIM_MOVE so that insertCasts will take care of it later.
      VarSymbol* tmp = newTemp("coerce_elt", fs->type);

      call->insertBefore(new DefExpr(tmp));
      rhs->remove();
      call->insertBefore(new CallExpr(PRIM_MOVE, tmp, rhs));
      call->insertAtTail(tmp);

    } else {
      USR_FATAL(userCall(call),
                "cannot assign expression of type %s to field '%s' of type %s",
                toString(t),
                fs->name,
                toString(fs->type));
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Resolves calls inserted into initializers during Phase 1,                   *
* to fully determine the instantiated type.                                   *
*                                                                             *
* This function is a combination of resolveMove and resolveSetMember          *
*                                                                             *
************************************** | *************************************/

static void resolveInitField(CallExpr* call) {
  if (call->id == breakOnResolveID) {
    gdbShouldBreakHere();
  }

  INT_ASSERT(call->argList.length == 3);
  // PRIM_INIT_FIELD contains three args:
  // fn->_this, the name of the field, and the value/type it is to be given

  SymExpr* rhs = toSymExpr(call->get(3)); // the value/type to give the field

  // Get the field name.
  SymExpr* sym = toSymExpr(call->get(2));
  if (!sym)
    INT_FATAL(call, "bad initializer set field primitive");
  VarSymbol* var = toVarSymbol(sym->symbol());
  if (!var || !var->immediate)
    INT_FATAL(call, "bad initializer set field primitive");
  const char* name = var->immediate->v_string;

  // Get the type
  AggregateType* ct = toAggregateType(call->get(1)->typeInfo()->getValType());
  if (!ct)
    INT_FATAL(call, "bad initializer set field primitive");

  Symbol* fs = NULL;
  int index = 1;
  for_fields(field, ct) {
    if (!strcmp(field->name, name)) {
      fs = field; break;
    }
    index++;
  }

  if (!fs)
    INT_FATAL(call, "bad initializer set field primitive");

  Type* t = rhs->typeInfo();
  // I think this never happens, so can be turned into an assert. <hilde>
  if (t == dtUnknown)
    INT_FATAL(call, "Unable to resolve field type");

  if (t == dtNil && fs->type == dtUnknown)
    USR_FATAL(call->parentSymbol, "unable to determine type of field from nil");
  if (fs->type == dtUnknown) {
    // Update the type of the field.  If necessary, update to a new
    // instantiation of the overarching type (and replaces references to the
    // fields from the old instantiation
    if ((fs->hasFlag(FLAG_TYPE_VARIABLE) && isTypeExpr(rhs)) ||
        fs->hasFlag(FLAG_PARAM) ||
        (fs->defPoint->exprType == NULL && fs->defPoint->init == NULL)) {
      AggregateType* instantiate = ct->getInstantiation(rhs->symbol(), index);
      if (instantiate != ct) {
        // TODO: make this set of operations a helper function I can call
        FnSymbol* parentFn = toFnSymbol(call->parentSymbol);
        INT_ASSERT(parentFn);
        INT_ASSERT(parentFn->_this);
        parentFn->_this->type = instantiate;

        SymbolMap fieldTranslate;
        for (int i = 1; i <= instantiate->fields.length; i++) {
          fieldTranslate.put(ct->getField(i), instantiate->getField(i));
        }
        update_symbols(parentFn, &fieldTranslate);

        ct = instantiate;
        fs = instantiate->getField(index);
      }
    } else {
      // The field is not generic.
      if (fs->defPoint->exprType == NULL) {
        fs->type = t;
      } else if (fs->defPoint->exprType) {
        fs->type = fs->defPoint->exprType->typeInfo();
      }
    }
  }

  if (t != fs->type && t != dtNil && t != dtObject) {
    Symbol*   actual = rhs->symbol();
    FnSymbol* fn     = toFnSymbol(call->parentSymbol);

    if (canCoerceTuples(t, actual, fs->type, fn)) {
      // Add a PRIM_MOVE so that insertCasts will take care of it later.
      VarSymbol* tmp = newTemp("coerce_elt", fs->type);

      call->insertBefore(new DefExpr(tmp));
      call->insertBefore(new CallExpr(PRIM_MOVE, tmp, rhs->remove()));

      call->insertAtTail(tmp);

    } else {
      USR_FATAL(userCall(call),
                "cannot assign expression of type %s to field '%s' of type %s",
                toString(t),
                fs->name,
                toString(fs->type));
    }
  }

  call->primitive = primitives[PRIM_SET_MEMBER];
}

/************************************* | **************************************
*                                                                             *
* This handles expressions of the form                                        *
*      CallExpr(PRIM_INIT_VAR, dst, src)                                      *
*                                                                             *
* 2017/03/06: This initial, trivial, implementation converts this to either   *
*                                                                             *
*      CallExpr(PRIM_MOVE, dst, src)                                          *
*                                                                             *
* or                                                                          *
*                                                                             *
*      CallExpr(PRIM_MOVE, dst, CallExpr("init_copy", src))                   *
*                                                                             *
* and then resolves the PRIM_MOVE.                                            *
*                                                                             *
************************************** | *************************************/

static bool hasCopyConstructor(AggregateType* ct);

static void resolveInitVar(CallExpr* call) {
  SymExpr* dstExpr = toSymExpr(call->get(1));
  Symbol*  dst     = dstExpr->symbol();

  SymExpr* srcExpr = toSymExpr(call->get(2));
  Symbol*  src     = srcExpr->symbol();
  Type*    srcType = src->type;

  if (dst->hasFlag(FLAG_NO_COPY)               == true)  {
    call->primitive = primitives[PRIM_MOVE];
    resolveMove(call);

  } else if (isPrimitiveScalar(srcType)        == true)  {
    call->primitive = primitives[PRIM_MOVE];
    resolveMove(call);

  } else if (isRecordWithInitializers(srcType) == true)  {
    AggregateType* ct  = toAggregateType(srcType);
    SymExpr*       rhs = toSymExpr(call->get(2));

    // The LHS will "own" the record
    if (rhs->symbol()->hasFlag(FLAG_INSERT_AUTO_DESTROY) == false &&
        rhs->symbol()->hasFlag(FLAG_TEMP)                == true) {
      dst->type       = src->type;

      call->primitive = primitives[PRIM_MOVE];

      resolveMove(call);

    } else if (hasCopyConstructor(ct) == true) {
      dst->type = src->type;

      call->setUnresolvedFunction("init");
      call->insertAtHead(gMethodToken);

      resolveCall(call);

    } else {
      USR_FATAL(call, "No copy constructor for initializer");
    }

  } else {
    Expr*     initExpr = srcExpr->remove();
    CallExpr* initCopy = new CallExpr("chpl__initCopy", initExpr);

    call->insertAtTail(initCopy);
    call->primitive = primitives[PRIM_MOVE];

    resolveExpr(initCopy);
    resolveMove(call);
  }
}

// A simplified version of functions_exists().
// It seems unfortunate to export that function in its current state
static bool hasCopyConstructor(AggregateType* ct) {
  bool retval = false;

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->numFormals() == 3 && strcmp(fn->name, "init") == 0) {
      ArgSymbol* _this  = fn->getFormal(2);
      ArgSymbol* _other = fn->getFormal(3);

      if ((_this->type == ct || _this->type == ct->refType) &&
          _other->type == ct) {
        retval = true;
        break;
      }
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool  moveIsAcceptable(CallExpr* call);

static void  moveHaltMoveIsUnacceptable(CallExpr* call);

static bool  moveSupportsUnresolvedFunctionReturn(CallExpr* call);

static Type* moveDetermineRhsType(CallExpr* call);

static Type* moveDetermineLhsType(CallExpr* call);

static bool  moveTypesAreAcceptable(Type* lhsType, Type* rhsType);

static void  moveHaltForUnacceptableTypes(CallExpr* call);

static void  resolveMoveForRhsSymExpr(CallExpr* call);

static void  resolveMoveForRhsCallExpr(CallExpr* call);

static void  moveSetConstFlagsAndCheck(CallExpr* call);

static void  moveSetFlagsAndCheckForConstAccess(Symbol*   lhs,
                                                CallExpr* rhs,
                                                FnSymbol* resolvedFn);

static void  moveSetFlagsForConstAccess(Symbol*   lhs,
                                        CallExpr* rhs,
                                        Symbol*   baseSym,
                                        bool      refConstWCT);

static void  moveFinalize(CallExpr* call);



static void resolveMove(CallExpr* call) {
  if (call->id == breakOnResolveID) {
    gdbShouldBreakHere();
  }


  if (moveIsAcceptable(call) == false) {
    // NB: This call will not return
    moveHaltMoveIsUnacceptable(call);


  // Ignore moves to RVV unless this is a constructor
  } else if (moveSupportsUnresolvedFunctionReturn(call) == true) {


  } else {
    // These calls might modify the fields in call
    Type* rhsType = moveDetermineRhsType(call);
    Type* lhsType = moveDetermineLhsType(call);
    Expr* rhs     = call->get(2);

    if (moveTypesAreAcceptable(lhsType, rhsType) == false) {
      // NB: This call will not return
      moveHaltForUnacceptableTypes(call);

    } else if (isSymExpr(rhs)  == true) {
      resolveMoveForRhsSymExpr(call);

    } else if (isCallExpr(rhs) == true) {
      resolveMoveForRhsCallExpr(call);

    } else {
      INT_ASSERT(false);
    }
  }
}

//
//
//

static bool moveIsAcceptable(CallExpr* call) {
  Symbol* lhsSym = toSymExpr(call->get(1))->symbol();
  Expr*   rhs    = call->get(2);
  bool    retval = true;

  if (isTypeExpr(rhs) == false) {
    if (lhsSym->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      FnSymbol* fn = toFnSymbol(call->parentSymbol);

      if (fn->getReturnSymbol()                  != lhsSym ||
          fn->hasFlag(FLAG_RUNTIME_TYPE_INIT_FN) == false) {
        retval = false;
      }
    }

  } else {
    if (lhsSym->hasFlag(FLAG_TYPE_VARIABLE) == false &&
        lhsSym->hasFlag(FLAG_MAYBE_TYPE)    == false) {
      retval = false;
    }
  }

  return retval;
}

static void moveHaltMoveIsUnacceptable(CallExpr* call) {
  Symbol* lhsSym = toSymExpr(call->get(1))->symbol();
  Expr*   rhs    = call->get(2);

  if (isTypeExpr(rhs) == false) {
    if (lhsSym->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      FnSymbol* fn = toFnSymbol(call->parentSymbol);

      if (fn->getReturnSymbol() != lhsSym) {
        USR_FATAL(call, "illegal assignment of value to type");

      } else if (fn->hasFlag(FLAG_RUNTIME_TYPE_INIT_FN) == false) {
        USR_FATAL(call, "illegal return of value where type is expected");
      }
    }

  } else {
    if (lhsSym->hasFlag(FLAG_TYPE_VARIABLE) == false &&
        lhsSym->hasFlag(FLAG_MAYBE_TYPE)    == false) {
      FnSymbol* fn = toFnSymbol(call->parentSymbol);

      if (fn->getReturnSymbol() == lhsSym) {
        USR_FATAL(call, "illegal return of type where value is expected");

      } else if (lhsSym->hasFlag(FLAG_CHPL__ITER) == true) {
        USR_FATAL(call,
                  "unable to iterate over type '%s'",
                  toString(rhs->getValType()));

      } else {
        USR_FATAL(call, "illegal assignment of type to value");
      }
    }
  }
}

//
// Return true if the move supports a return from a function.
// NB this does not include a constructor
//
static bool moveSupportsUnresolvedFunctionReturn(CallExpr* call) {
  bool retval = false;

  if (FnSymbol* fn = toFnSymbol(call->parentSymbol)) {
    Symbol* lhsSym = toSymExpr(call->get(1))->symbol();

    if (fn->retType           == dtUnknown       && // Return type unresolved
        fn->getReturnSymbol() == lhsSym          && // LHS is the RVV
        fn->_this             != lhsSym          && // Not a constructor
        call->parentExpr      != fn->where       &&
        call->parentExpr      != fn->retExprType) {
      retval = true;
    }
  }

  return retval;
}

//
//
//

// Determine type of RHS.
// NB: This function may update the RHS
static Type* moveDetermineRhsType(CallExpr* call) {
  Symbol* lhsSym = toSymExpr(call->get(1))->symbol();
  Expr*   rhs    = call->get(2);
  Type*   retval = rhs->typeInfo();

  // Workaround for order-of-resolution problems with extern type aliases
  if (retval == dtUnknown) {
    bool rhsIsTypeExpr = isTypeExpr(rhs);

    if (rhsIsTypeExpr == true && isSymExpr(rhs) == true) {
      // Try resolving type aliases now.
      retval = resolveTypeAlias(toSymExpr(rhs));
    }
  }

  if (retval == dtVoid) {
    if (CallExpr* rhsCall = toCallExpr(rhs)) {
      if (FnSymbol* rhsFn = rhsCall->resolvedFunction()) {
        if (rhsFn->hasFlag(FLAG_VOID_NO_RETURN_VALUE) == true) {
          USR_FATAL(userCall(call),
                    "illegal use of function that does not "
                    "return a value: '%s'",
                    rhsFn->name);
        }
      }
    }
  }

  // Workaround for problems where the _iterator in buildForLoopExpr would
  // be an _array instead of a ref(_array) in 4-init-array-forexpr.chpl.
  // This could be improved with QualifiedType.
  if (lhsSym->hasFlag(FLAG_MAYBE_REF) ==  true &&
      isReferenceType(retval)         == false) {
    if (SymExpr* se = toSymExpr(rhs)) {
      if (ArgSymbol* arg = toArgSymbol(se->symbol())) {
        if (concreteIntent(arg->intent, arg->type) & INTENT_FLAG_REF) {
          makeRefType(retval);

          retval = retval->refType;

          // Add PRIM_ADDR_OF
          // (this won't be necessary with QualifiedType/PRIM_SET_REFERENCE)
          VarSymbol* addrOfTmp = newTemp("moveAddr", retval);
          CallExpr*  addrOf    = new CallExpr(PRIM_ADDR_OF, rhs->copy());
          SymExpr*   newRhs    = new SymExpr(addrOfTmp);

          call->insertBefore(new DefExpr(addrOfTmp));
          call->insertBefore(new CallExpr(PRIM_MOVE, addrOfTmp, addrOf));

          rhs->replace(newRhs);
        }
      }
    }
  }

  return retval;
}

static Type* moveDetermineLhsType(CallExpr* call) {
  Symbol* lhsSym = toSymExpr(call->get(1))->symbol();

  if (lhsSym->type == dtUnknown || lhsSym->type == dtNil) {
    if (lhsSym->id == breakOnResolveID) {
      gdbShouldBreakHere();
    }

    lhsSym->type = call->get(2)->typeInfo();
  }

  return lhsSym->type;
}





//
//
//

static bool moveTypesAreAcceptable(Type* lhsType, Type* rhsType) {
  bool retval = true;

  if (rhsType == dtUnknown) {
    retval = false;

  } else if (rhsType == dtNil) {
    if (lhsType != dtNil && isClass(lhsType) == false) {
      retval = false;
    }
  }

  return retval;
}

static void moveHaltForUnacceptableTypes(CallExpr* call) {
  Type* lhsType = call->get(1)->typeInfo();
  Type* rhsType = call->get(2)->typeInfo();

  if (rhsType == dtUnknown) {
    Expr* rhs = call->get(2);

    if (CallExpr* rhsCall = toCallExpr(rhs)) {
      if (FnSymbol* fn = rhsCall->resolvedFunction()) {
        USR_FATAL_CONT(fn,
                       "unable to resolve return type of function '%s'",
                       fn->name);
        USR_FATAL(rhs, "called recursively at this point");
      }
    }

    USR_FATAL(call, "unable to resolve type");

  } else if (rhsType == dtNil) {
    if (lhsType != dtNil && isClass(lhsType) == false) {
      USR_FATAL(userCall(call),
                "type mismatch in assignment from nil to %s",
                toString(lhsType));
    }
  }
}


//
//
//

static void resolveMoveForRhsSymExpr(CallExpr* call) {
  SymExpr* rhs = toSymExpr(call->get(2));

  // If this assigns into a loop index variable from a non-var iterator,
  // mark the variable constant.
  // If RHS is this special variable...
  if (rhs->symbol()->hasFlag(FLAG_INDEX_OF_INTEREST) == true) {
    Symbol* lhsSym  = toSymExpr(call->get(1))->symbol();
    Type*   rhsType = rhs->typeInfo();

    INT_ASSERT(lhsSym->hasFlag(FLAG_INDEX_VAR));

    // ... and not of a reference type
    // ... and not an array (arrays are always yielded by reference)
    // todo: differentiate based on ref-ness, not _ref type
    // todo: not all const if it is zippered and one of iterators is var
    if (isReferenceType(rhsType)                == false &&
        isTupleContainingAnyReferences(rhsType) == false &&
        rhsType->symbol->hasFlag(FLAG_ARRAY)    == false) {
      // ... then mark LHS constant.
      lhsSym->addFlag(FLAG_CONST);
    }
  } else if (rhs->symbol()->hasFlag(FLAG_DELAY_GENERIC_EXPANSION)) {
    Symbol* lhsSym  = toSymExpr(call->get(1))->symbol();
    lhsSym->addFlag(FLAG_DELAY_GENERIC_EXPANSION);
  }

  moveFinalize(call);
}

static void resolveMoveForRhsCallExpr(CallExpr* call) {
  CallExpr* rhs = toCallExpr(call->get(2));

  moveSetConstFlagsAndCheck(call);

  if (rhs->resolvedFunction() == gChplHereAlloc) {
    Symbol*  lhsType = call->get(1)->typeInfo()->symbol;
    Symbol*  tmp     = newTemp("cast_tmp", rhs->typeInfo());

    call->insertBefore(new DefExpr(tmp));
    call->insertBefore(new CallExpr(PRIM_MOVE, tmp,     rhs->remove()));
    call->insertAtTail(new CallExpr(PRIM_CAST, lhsType, tmp));

  } else if (rhs->isPrimitive(PRIM_SIZEOF) == true) {
    // Fix up arg to sizeof(), as we may not have known the type earlier
    SymExpr* sizeSym  = toSymExpr(rhs->get(1));
    Type*    sizeType = sizeSym->symbol()->typeInfo();

    rhs->replace(new CallExpr(PRIM_SIZEOF, sizeType->symbol));

  } else if (rhs->isPrimitive(PRIM_CAST_TO_VOID_STAR) == true) {
    if (isReferenceType(rhs->get(1)->typeInfo())) {
      // Add a dereference as needed, as we did not have complete
      // type information earlier
      SymExpr*   castVar   = toSymExpr(rhs->get(1));
      Type*      castType  = castVar->typeInfo()->getValType();

      VarSymbol* derefTmp  = newTemp("castDeref", castType);
      CallExpr*  derefCall = new CallExpr(PRIM_DEREF, castVar->symbol());

      call->insertBefore(new DefExpr(derefTmp));
      call->insertBefore(new CallExpr(PRIM_MOVE, derefTmp, derefCall));

      rhs->replace(new CallExpr(PRIM_CAST_TO_VOID_STAR, derefTmp));
    }

    moveFinalize(call);

  } else if (rhs->isPrimitive(PRIM_INIT) == true) {
    moveFinalize(call);

    if (SymExpr* se = toSymExpr(rhs->get(1))) {
      Type* seType = se->symbol()->type;

      if (isNonGenericRecordWithInitializers(seType) == true) {
        Expr*     callLhs  = call->get(1)->remove();
        CallExpr* callInit = new CallExpr("init", gMethodToken, callLhs);

        // This juggling is required by use of
        // for_exprs_postorder() in resolveBlockStmt
        call->insertBefore(callInit);
        call->convertToNoop();

        resolveCallAndCallee(callInit);
      }
    }

  // Fix up PRIM_COERCE : remove it if it has a param RHS.
  } else if (rhs->isPrimitive(PRIM_COERCE) == true) {
    moveFinalize(call);

    if (SymExpr* coerceSE = toSymExpr(rhs->get(1))) {
      Symbol* coerceSym = coerceSE->symbol();

      // This transformation is normally handled in insertCasts
      // but we need to do it earlier for parameters. We can't just
      // call insertCasts here since that would dramatically change the
      // resolution order (and would be apparently harder to get working).
      if (coerceSym->isParameter()               == true  ||
          coerceSym->hasFlag(FLAG_TYPE_VARIABLE) == true) {
        // Can we coerce from the argument to the function return type?
        // Note that rhsType here is the function return type
        // (since that is what the primitive returns as its type).
        Type* coerceType = coerceSym->type;
        Type* rhsType    = rhs->typeInfo();

        if (coerceType                                     == rhsType ||
            canParamCoerce(coerceType, coerceSym, rhsType) == true)   {
          SymExpr* lhs = toSymExpr(call->get(1));

          call->get(1)->replace(lhs->copy());
          call->get(2)->replace(new SymExpr(coerceSym));

        } else if (canCoerce(coerceType, coerceSym, rhsType, NULL) == true) {

          // any case that doesn't param coerce but that does coerce
          // will be handled in insertCasts.

        } else {
          USR_FATAL(userCall(call),
                    "type mismatch in return from %s to %s",
                    toString(coerceType),
                    toString(rhsType));
        }
      }
    }

  } else {
    if (rhs->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
      Type* baseType = rhs->get(1)->getValType();
      const char* memberName = get_string(rhs->get(2));
      Symbol* sym = baseType->getField(memberName);
      if (sym->hasFlag(FLAG_DELAY_GENERIC_EXPANSION)) {
        if (SymExpr* se = toSymExpr(call->get(1))) {
          se->symbol()->addFlag(FLAG_DELAY_GENERIC_EXPANSION);
        }
      }
    }

    moveFinalize(call);
  }
}


static void moveSetConstFlagsAndCheck(CallExpr* call) {
  CallExpr* rhs    = toCallExpr(call->get(2));

  if (rhs->isPrimitive(PRIM_GET_MEMBER)) {
    if (SymExpr* rhsBase = toSymExpr(rhs->get(1))) {
      if (rhsBase->symbol()->hasFlag(FLAG_CONST)        == true  ||
          rhsBase->symbol()->hasFlag(FLAG_REF_TO_CONST) == true) {
        toSymExpr(call->get(1))->symbol()->addFlag(FLAG_REF_TO_CONST);
      }

    } else {
      INT_ASSERT(false);
    }

  } else if (FnSymbol* resolvedFn = rhs->resolvedFunction()) {
    Symbol* lhsSym = toSymExpr(call->get(1))->symbol();

    moveSetFlagsAndCheckForConstAccess(lhsSym, rhs, resolvedFn);
  }
}

// If 'call' is an access to a const thing, for example a const field
// or a field of a const record, set const flag(s) on the symbol
// that stores the result of 'call'.
static void moveSetFlagsAndCheckForConstAccess(Symbol*   lhsSym,
                                               CallExpr* rhs,
                                               FnSymbol* resolvedFn) {
  bool refConst    = resolvedFn->hasFlag(FLAG_REF_TO_CONST);
  bool refConstWCT = resolvedFn->hasFlag(FLAG_REF_TO_CONST_WHEN_CONST_THIS);

  INT_ASSERT(refConst == false || refConstWCT == false);

  if (refConst == true) {
    Symbol* baseSym = NULL;

    if (resolvedFn->hasFlag(FLAG_FIELD_ACCESSOR)    == true &&
        resolvedFn->hasFlag(FLAG_PROMOTION_WRAPPER) == false) {
      baseSym = getBaseSymForConstCheck(rhs);
    }

    moveSetFlagsForConstAccess(lhsSym, rhs, baseSym, false);

  } else if (resolvedFn->hasFlag(FLAG_NEW_ALIAS_FN) == true &&
             lhsSym->hasFlag(FLAG_ARRAY_ALIAS)      == true) {
    if (lhsSym->isConstant() == false) {
      // We are creating a var alias - ensure aliasee is not const either.
      SymExpr* aliaseeSE = toSymExpr(rhs->get(2));

      INT_ASSERT(aliaseeSE);

      if (aliaseeSE->symbol()->isConstant() == true) {
        USR_FATAL_CONT(rhs,
                       "creating a non-const alias '%s' of a const array "
                       "or domain",
                       lhsSym->name);
      }
    }

  } else if (refConstWCT == true) {
    Symbol* baseSym = getBaseSymForConstCheck(rhs);

    if (baseSym->isConstant()               == true ||
        baseSym->hasFlag(FLAG_CONST)        == true ||
        baseSym->hasFlag(FLAG_REF_TO_CONST) == true) {
      moveSetFlagsForConstAccess(lhsSym, rhs, baseSym, true);
    }

  } else if (lhsSym->hasFlag(FLAG_ARRAY_ALIAS)      == true &&
             resolvedFn->hasFlag(FLAG_AUTO_COPY_FN) == true) {
    INT_ASSERT(false);
  }
}

static void moveSetFlagsForConstAccess(Symbol*   lhsSym,
                                       CallExpr* rhs,
                                       Symbol*   baseSym,
                                       bool      refConstWCT) {
  bool isArgThis = baseSym != NULL && baseSym->hasFlag(FLAG_ARG_THIS) == true;

  // Do not consider it const if it is an access to 'this' in a constructor.
  if (isArgThis == false || isInConstructorLikeFunction(rhs) == false) {

    if (isReferenceType(lhsSym->type) == true) {
      lhsSym->addFlag(FLAG_REF_TO_CONST);
    } else {
      lhsSym->addFlag(FLAG_CONST);
    }

    if (isArgThis == true ||
        (refConstWCT                                        == true &&
         baseSym->hasFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS) == true)) {
      lhsSym->addFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS);
    }
  }
}

//
//
//

static void moveFinalize(CallExpr* call) {
  Type* lhsType    = call->get(1)->typeInfo();
  Type* rhsType    = call->get(2)->typeInfo();

  Type* lhsValType = lhsType->getValType();
  Type* rhsValType = rhsType->getValType();

  if (isDispatchParent(rhsValType, lhsValType) == true) {
    if (rhsType != lhsType) {
      Expr*   rhs = call->get(2);
      Symbol* tmp = newTemp("cast_tmp", rhsType);

      call->insertBefore(new DefExpr(tmp));
      call->insertBefore(new CallExpr(PRIM_MOVE, tmp, rhs->remove()));

      call->insertAtTail(new CallExpr(PRIM_CAST, lhsValType->symbol, tmp));
    }

  } else {
    if (rhsValType != lhsValType) {
      if (rhsType != dtNil) {
        USR_FATAL(userCall(call),
                  "type mismatch in assignment from %s to %s",
                  toString(rhsType),
                  toString(lhsType));
      }
    }
  }
}

//
//
//

bool isDispatchParent(Type* t, Type* pt) {
  bool retval = false;

  forv_Vec(Type, p, t->dispatchParents) {
    if (p == pt || isDispatchParent(p, pt) == true) {
      retval = true;
      break;
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* Resolve a PRIM_NEW expression.                                              *
* The 1st argument is a type or a partial call to get the type and the        *
* remaining arguments are assumed to be arguments for the constructor or      *
* initializer call                                                            *
*                                                                             *
* Some new expressions are converted in normalize().                          *
*   For example, a call to  type function is resolved at this point.          *
*                                                                             *
* The syntax supports calling the result of a type function as a constructor  *
* but this is not fully implemented.                                          *
*                                                                             *
************************************** | *************************************/

static SymExpr* resolveNewTypeExpr(CallExpr* call);

static bool     resolveNewHasInitializer(AggregateType* at);

static void     resolveNewHandleConstructor(CallExpr*      call,
                                            AggregateType* at,
                                            SymExpr*       typeExpr);

static void     resolveNewHandleGenericInitializer(CallExpr*      call,
                                                   AggregateType* at,
                                                   SymExpr*       typeExpr);

static void     resolveNewHandleNonGenericInitializer(CallExpr*      call,
                                                      AggregateType* at,
                                                      SymExpr*       typeExpr);

static void     resolveNewHalt(CallExpr* call);

static void resolveNew(CallExpr* call) {
  if (SymExpr* typeExpr = resolveNewTypeExpr(call)) {
    if (Type* type = resolveTypeAlias(typeExpr)) {
      if (AggregateType* at = toAggregateType(type)) {
        if (resolveNewHasInitializer(at) == false) {
          resolveNewHandleConstructor(call, at, typeExpr);

        } else if (at->symbol->hasFlag(FLAG_GENERIC) == false) {
          resolveNewHandleNonGenericInitializer(call, at, typeExpr);

        } else {
          resolveNewHandleGenericInitializer(call, at, typeExpr);
        }

      } else if (PrimitiveType* pt = toPrimitiveType(type)) {
        const char* name = pt->symbol->name;

        USR_FATAL(call, "invalid use of 'new' on primitive %s", name);

      } else if (EnumType* et = toEnumType(type)) {
        const char* name = et->symbol->name;

        USR_FATAL(call, "invalid use of 'new' on enum %s", name);

      } else {
        USR_FATAL(call, "new must be applied to a record or class");
      }
    }

  } else {
    resolveNewHalt(call);
  }
}

// Find the SymExpr that captures the type
static SymExpr* resolveNewTypeExpr(CallExpr* call) {
  Expr*    arg1   = call->get(1);
  SymExpr* retval = NULL;

  // The common case e.g new MyClass(1, 2, 3);
  if (SymExpr* se = toSymExpr(arg1)) {
    if (se->symbol() != gModuleToken) {
      retval = se;

    } else {
      retval = toSymExpr(call->get(3));
      INT_ASSERT(retval != NULL);
    }

  // 'new' (call (partial) R2 _mt this), call_tmp0, call_tmp1, ...
  // due to nested classes (i.e. R2 is a nested class type)
  } else if (CallExpr* subCall = toCallExpr(arg1)) {
    if (SymExpr* se = toSymExpr(subCall->baseExpr)) {
      retval = (subCall->partialTag) ? se : NULL;
    }
  }

  return retval;
}

static bool resolveNewHasInitializer(AggregateType* at) {
  FnSymbol* di     = at->defaultInitializer;
  bool      retval = false;

  if (at->initializerStyle == DEFINES_INITIALIZER) {
    retval = true;

  } else if (di != NULL && strcmp(di->name, "init") == 0) {
    retval = true;
  }

  return retval;
}

static void resolveNewHandleConstructor(CallExpr*      call,
                                        AggregateType* at,
                                        SymExpr*       typeExpr) {
  SET_LINENO(call);

  if (FnSymbol* atInit = at->defaultInitializer) {
    Expr* baseExpr = NULL;

    typeExpr->replace(new UnresolvedSymExpr(atInit->name));

    // Convert the PRIM_NEW to a normal call
    if (SymExpr* se = toSymExpr(call->get(1))) {
      baseExpr = (se->symbol() == gModuleToken) ? call->get(3) : call->get(1);

    } else {
      baseExpr = call->get(1);
    }

    call->primitive = NULL;
    call->baseExpr  = baseExpr->remove();

    parent_insert_help(call, call->baseExpr);

    resolveExpr(call);

  } else {
    USR_FATAL(call,
              "could not generate default initializer for type "
              "'%s', please define one",
              at->symbol->name);
  }
}

static void resolveNewHandleNonGenericInitializer(CallExpr*      call,
                                                  AggregateType* at,
                                                  SymExpr*       typeExpr) {
  SET_LINENO(call);

  VarSymbol* newTmp = newTemp("new_temp", at);
  DefExpr*   def    = new DefExpr(newTmp);

  if (isCallExpr(call->get(1))) {
    // Happens when the type on which we are calling new is a nested type.
    // In that case, the second argument to that inner call should be used as
    // the "outer" argument to the _new or init function.
    CallExpr* partial = toCallExpr(call->get(1)->remove());

    call->insertAtHead(partial->get(2)->remove());
    INT_ASSERT(partial->baseExpr == typeExpr);
    call->insertAtHead(typeExpr);
  }


  if (isClass(at) == true) {
    // Convert the PRIM_NEW to a normal call
    call->primitive = NULL;
    call->baseExpr  = new UnresolvedSymExpr("_new");
    parent_insert_help(call, call->baseExpr);

    if (isBlockStmt(call->parentExpr) == true) {
      call->insertBefore(def);

    } else {
      call->parentExpr->insertBefore(def);
    }

    resolveExpr(call);

  } else {
    // Convert the PRIM_NEW to a normal call
    call->primitive = NULL;
    call->baseExpr  = new UnresolvedSymExpr("init");

    parent_insert_help(call, call->baseExpr);

    if (isBlockStmt(call->parentExpr) == true) {
      call->insertBefore(def);

    } else {
      Expr* parent = call->parentExpr;

      // NB: This removes the "init" call from the tree
      call->replace(new SymExpr(newTmp));

      // Insert <def> and then re-insert the "init" call
      parent->insertBefore(def);
      parent->insertBefore(call);
    }

    typeExpr->remove();

    // Invoking an instance method
    call->insertAtHead(new SymExpr(newTmp));
    call->insertAtHead(new SymExpr(gMethodToken));

    resolveExpr(call);
  }
}

static void resolveNewHandleGenericInitializer(CallExpr*      call,
                                               AggregateType* at,
                                               SymExpr*       typeExpr) {
  SET_LINENO(call);

  typeExpr->replace(new UnresolvedSymExpr("init"));

  // Convert the PRIM_NEW to a normal call
  call->primitive = NULL;
  call->baseExpr  = call->get(1)->remove();

  parent_insert_help(call, call->baseExpr);

  VarSymbol* new_temp  = newTemp("new_temp", at);
  DefExpr*   def       = new DefExpr(new_temp);

  new_temp->addFlag(FLAG_DELAY_GENERIC_EXPANSION);

  if (isBlockStmt(call->parentExpr) == true) {
    call->insertBefore(def);

  } else {
    call->parentExpr->insertBefore(def);
  }

  // Invoking an instance method
  call->insertAtHead(new NamedExpr("this", new SymExpr(new_temp)));
  call->insertAtHead(new SymExpr(gMethodToken));

  temporaryInitializerFixup(call);

  resolveDefaultGenericType(call);

  resolveInitializer(call);

  // Because initializers determine the type they utilize based on the
  // execution of Phase 1, if the type is generic we will need to update the
  // type of the actual we are sending in for the this arg
  if (at->symbol->hasFlag(FLAG_GENERIC) == true) {
    new_temp->type = call->resolvedFunction()->_this->type;

    if (isClass(at) == true) {
      // use the allocator instead of directly calling the init method
      // Need to convert the call into the right format
      call->baseExpr->replace(new UnresolvedSymExpr("_new"));
      call->get(1)->replace(new SymExpr(new_temp->type->symbol));
      call->get(2)->remove();
      // Need to resolve the allocator
      resolveCall(call);
      resolveFns(call->resolvedFunction());

      def->remove();
    }
  }
}

static void resolveNewHalt(CallExpr* call) {
  const char* name = NULL;

  if (Expr* arg = call->get(1)) {
    if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(arg)) {
      name = urse->unresolved;

    } else if (CallExpr* subCall = toCallExpr(arg)) {
      if (FnSymbol* fn = subCall->resolvedFunction()) {
        name = fn->name;
      }
    }
  }

  if (name == NULL) {
    USR_FATAL(call, "invalid use of 'new'");
  } else {
    USR_FATAL(call, "invalid use of 'new' on %s", name);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool isRefWrapperForNonGenericRecord(AggregateType* at);

static void temporaryInitializerFixup(CallExpr* call) {
  if (UnresolvedSymExpr* usym = toUnresolvedSymExpr(call->baseExpr)) {
    // Support super.init() calls (for instance) when the super type
    // does not define either an initializer or a constructor.
    // Also ignores errors from improperly inserted .init() calls
    // (so be sure to check here if something is behaving oddly
    // - Lydia, 08/19/16)
    if (strcmp(usym->unresolved, "init") ==     0 &&
        call->numActuals()               >=     2 &&
        isNamedExpr(call->get(2))        == false) {
      // Arg 2 will be a NamedExpr to "this" if we're in an intentionally
      // inserted initializer call
      SymExpr* _mt = toSymExpr(call->get(1));
      SymExpr* sym = toSymExpr(call->get(2));

      INT_ASSERT(sym != NULL);

      if (AggregateType* ct = toAggregateType(sym->symbol()->getValType())) {

        if (isRefWrapperForNonGenericRecord(ct) == false &&
            ct->initializerStyle                == DEFINES_NONE_USE_DEFAULT) {
          // Transitioning to a default initializer world.
          // Lydia note 03/14/17)
          if (strcmp(ct->defaultInitializer->name, "init") != 0) {
            // This code should be removed when the compiler generates
            // initializers as the default method of construction and
            // initialization for a type (Lydia note, 08/19/16)
            usym->unresolved = astr("_construct_", ct->symbol->name);

            _mt->remove();
          }
        }
      }
    }
  }
}


//
// Noakes 2017/03/26
//   The function temporaryInitializerFixup is designed to update
//   certain calls to init() while the initializer update matures.
//
//   Unfortunately this transformation is triggered incorrectly for uses of
//           this.init(...);
//
//   inside initializers for non-generic records.
//
//   For those uses of init() the "this" argument has currently has type
//   _ref(<Record>) rather than <Record>
//
//  This rather unfortunate function catches this case and enables the
//  transformation to be skipped.
//
static bool isRefWrapperForNonGenericRecord(AggregateType* at) {
  bool retval = false;

  if (isClass(at)                           == true &&
      strncmp(at->symbol->name, "_ref(", 5) == 0    &&
      at->fields.length                     == 1) {
    Symbol* sym = toDefExpr(at->fields.head)->sym;

    if (strcmp(sym->name, "_val") == 0) {
      retval = isNonGenericRecordWithInitializers(sym->type);
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void
resolveCoerce(CallExpr* call) {

  resolveDefaultGenericType(call);

  FnSymbol* fn = toFnSymbol(call->parentSymbol);
  Type* toType = call->get(2)->typeInfo();

  INT_ASSERT(fn);

  // Adjust tuple reference-level for return if necessary
  if (toType->symbol->hasFlag(FLAG_TUPLE) &&
      !doNotChangeTupleTypeRefLevel(fn, true)) {
    AggregateType* tupleType = toAggregateType(toType);
    INT_ASSERT(tupleType);

    Type* retType = getReturnedTupleType(fn, tupleType);
    if (retType != toType) {
      // Also adjust any PRIM_COERCE calls
      call->get(2)->replace(new SymExpr(retType->symbol));
    }
  }
}

//
// This tells us whether we can rely on the compiler's back end (e.g.,
// C) to provide the copy for us for 'in' or 'const in' intents when
// passing an argument of type 't'.
//
static bool backendRequiresCopyForIn(Type* t) {
  return (isRecord(t) ||
          isUnion(t) ||
          t->symbol->hasFlag(FLAG_ARRAY) ||
          t->symbol->hasFlag(FLAG_DOMAIN));
}


// Returns true if the formal needs an internal temporary, false otherwise.
static bool
formalRequiresTemp(ArgSymbol* formal) {
  //
  // get the formal's function
  //
  FnSymbol* fn = toFnSymbol(formal->defPoint->parentSymbol);
  INT_ASSERT(fn);

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

void
insertFormalTemps(FnSymbol* fn) {
  SymbolMap formals2vars;

  for_formals(formal, fn) {
    if (formalRequiresTemp(formal)) {
      SET_LINENO(formal);
      VarSymbol* tmp = newTemp(astr("_formal_tmp_", formal->name));
      if (formal->hasFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT))
        tmp->addFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT);
      formals2vars.put(formal, tmp);
    }
  }

  if (formals2vars.n > 0) {
    // The names of formals in the body of this function are replaced with the
    // names of their corresponding local temporaries.
    update_symbols(fn->body, &formals2vars);

    // Add calls to chpl__initCopy to create local copies as necessary.
    // Add writeback code for out and inout intents.
    addLocalCopiesAndWritebacks(fn, formals2vars);
  }
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
static void addLocalCopiesAndWritebacks(FnSymbol* fn, SymbolMap& formals2vars)
{
  // Enumerate the formals that have local temps.
  form_Map(SymbolMapElem, e, formals2vars) {
    ArgSymbol* formal = toArgSymbol(e->key); // Get the formal.
    Symbol*    tmp    = e->value; // Get the temp.

    SET_LINENO(formal);

    // TODO: Move this closer to the location (in code) where we determine
    // whether tmp owns its value or not.  That is, push setting these flags
    // (or not) into the cases below, as appropriate.
    Type* formalType = formal->type->getValType();

    // mark CONST as needed
    if (concreteIntent(formal->intent, formalType) & INTENT_FLAG_CONST) {
      tmp->addFlag(FLAG_CONST);
      if (!isRefCountedType(formalType)) // TODO - remove isRefCountedType?
        tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
    }

    // This switch adds the extra code inside the current function necessary
    // to implement the ref-to-value semantics, where needed.
    switch (formal->intent)
    {
      // Make sure we handle every case.
     default:
      INT_FATAL("Unhandled INTENT case.");
      break;

      // These cases are weeded out by formalRequiresTemp() above.
     case INTENT_PARAM:
     case INTENT_TYPE:
     case INTENT_REF:
     case INTENT_CONST_REF:
      INT_FATAL("Unexpected INTENT case.");
      break;

     case INTENT_OUT:
      if (formal->defaultExpr &&
          formal->defaultExpr->body.tail->typeInfo() != dtTypeDefaultToken) {
        BlockStmt* defaultExpr = formal->defaultExpr->copy();

        fn->insertAtHead(new CallExpr(PRIM_MOVE,
                                      tmp,
                                      defaultExpr->body.tail->remove()));
        fn->insertAtHead(defaultExpr);

      } else {
        AggregateType* formalAt = toAggregateType(formal->getValType());
        if (isNonGenericRecordWithInitializers(formalAt)) {
          fn->insertAtHead(new CallExpr("init",
                                        gMethodToken,
                                        tmp));
          tmp->type = formalAt;
        } else {
          VarSymbol* typeTmp = newTemp("_formal_type_tmp_");

          typeTmp->addFlag(FLAG_MAYBE_TYPE);

          fn->insertAtHead(new CallExpr(PRIM_MOVE,
                                        tmp,
                                        new CallExpr(PRIM_INIT, typeTmp)));

          fn->insertAtHead(new CallExpr(PRIM_MOVE,
                                        typeTmp,
                                        new CallExpr(PRIM_TYPEOF, formal)));

          fn->insertAtHead(new DefExpr(typeTmp));
        }
      }

      tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
      tmp->addFlag(FLAG_FORMAL_TEMP);
      break;

     case INTENT_INOUT:
      fn->insertAtHead(new CallExpr(PRIM_MOVE,
                                    tmp,
                                    new CallExpr("chpl__initCopy", formal)));

      tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
      tmp->addFlag(FLAG_FORMAL_TEMP);

      break;

     case INTENT_IN:
     case INTENT_CONST_IN:
      // TODO: Adding a formal temp for INTENT_CONST_IN is conservative.
      // If the compiler verifies in a separate pass that it is never written,
      // we don't have to copy it.
      fn->insertAtHead(new CallExpr(PRIM_MOVE,
                                    tmp,
                                    new CallExpr("chpl__initCopy", formal)));

      tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
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
           //  and test/users/recordbug3.chpl.
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
       } else
       {
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
    if (formal->intent == INTENT_INOUT || formal->intent == INTENT_OUT) {
      fn->insertIntoEpilogue(new CallExpr("=", formal, tmp));
    }
  }
}

static CallExpr* toPrimToLeaderCall(Expr* expr) {
  if (CallExpr* call = toCallExpr(expr))
    if (call->isPrimitive(PRIM_TO_LEADER) ||
        call->isPrimitive(PRIM_TO_STANDALONE))
      return call;
  return NULL;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool isInstantiation(Type* sub, Type* super) {
  bool retval = false;

  INT_ASSERT(super != NULL);

  if (AggregateType* at = toAggregateType(sub)) {
    AggregateType* cur = at->instantiatedFrom;

    while (cur != NULL && cur != super) {
      cur = cur->instantiatedFrom;
    }

    retval = cur == super;
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static Expr* resolveTypeOrParamExpr(Expr* expr);

void ensureEnumTypeResolved(EnumType* etype) {
  if (etype->integerType == NULL) {
    // Make sure to resolve all enum types.
    for_enums(def, etype) {
      if (def->init != NULL) {
        Expr* enumTypeExpr = resolveTypeOrParamExpr(def->init);

        if (enumTypeExpr->typeInfo() == dtUnknown) {
          INT_FATAL(def->init, "Unable to resolve enumerator type expression");
        }
      }
    }

    // Now try computing the enum size...
    etype->sizeAndNormalize();
  }

  INT_ASSERT(etype->integerType != NULL);
}

// Recursively resolve typedefs
Type* resolveTypeAlias(SymExpr* se) {
  Type* retval = NULL;

  if (se != NULL) {
    Type* valType = se->getValType();

    if (valType != dtUnknown) {
      retval = valType;

    } else if (VarSymbol* var = toVarSymbol(se->symbol())) {
      SET_LINENO(var->defPoint);

      DefExpr* def      = var->defPoint;
      Expr*    typeExpr = resolveTypeOrParamExpr(def->init);
      SymExpr* tse      = toSymExpr(typeExpr);

      retval = resolveTypeAlias(tse);
    }
  }

  return retval;
}

static Expr* resolveTypeOrParamExpr(Expr* expr) {
  Expr* retval = NULL;

  for_exprs_postorder(e, expr) {
    if (CallExpr* call = toCallExpr(e)) {
      Expr* result = preFold(call);

      if (CallExpr* callFolded = toCallExpr(result)) {
        if (callFolded->parentSymbol != NULL) {
          callStack.add(callFolded);

          resolveCall(callFolded);

          if (callFolded->parentSymbol != NULL) {
            if (FnSymbol* fn = callFolded->resolvedFunction()) {
              resolveFormals(fn);

              if (fn->retTag  == RET_PARAM || fn->retTag  == RET_TYPE) {
                resolveFns(fn);

              } else if (fn->retType == dtUnknown) {
                resolveFns(fn);
              }
            }
          }

          callStack.pop();
        }
      }

      retval = foldTryCond(postFold(result));

    } else {
      retval = foldTryCond(postFold(e));
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void resolveBlockStmt(BlockStmt* blockStmt) {
  for_exprs_postorder(expr, blockStmt) {
    expr = resolveExpr(expr);

    if (tryFailure == true) {
      if (expr != NULL) {
        tryFailure = false;
      } else {
        break;
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Resolves an expression and manages the callStack and tryStack.              *
*                                                                             *
* On success, returns the call that was passed in.                            *
*                                                                             *
* On a try failure, returns either the expression preceding the elseStmt,     *
* substituted for the body of the param condition (if that substitution       *
* could be made), or NULL.                                                    *
*                                                                             *
* If null, then resolution of the current block should be aborted.            *
* tryFailure is true in this case, so the search for a matching elseStmt      *
* continue in the surrounding block or call.                                  *
*                                                                             *
************************************** | *************************************/

static bool        isParamResolved(FnSymbol* fn, Expr* expr);

static ForallStmt* toForallForIteratedExpr(SymExpr* expr);

static Expr*       resolveExprPhase2(Expr* origExpr, FnSymbol* fn, Expr* expr);

static Expr*       resolveExprResolveEachCall(ContextCallExpr* cc);

static bool        contextTypesMatch(FnSymbol* valueFn,
                                     FnSymbol* constRefFn,
                                     FnSymbol* refFn);

static void        contextTypeInfo(FnSymbol* fn);

static void        resolveExprExpandGenerics(CallExpr* call);

static void        resolveExprTypeConstructor(SymExpr* symExpr);

static Expr*       resolveExprHandleTryFailure(FnSymbol* fn);

static void        resolveExprMaybeIssueError(CallExpr* call);

static Expr* resolveExpr(Expr* expr) {
  FnSymbol* fn     = toFnSymbol(expr->parentSymbol);
  Expr*     retval = NULL;

  SET_LINENO(expr);

  if (isContextCallExpr(expr) == true) {
    retval = expr;

  } else if (isParamResolved(fn, expr) == true) {
    retval = expr;

  // This must be after isParamResolved
  } else if (BlockStmt* block = toBlockStmt(expr)) {
    // Possibly pop try block and delete else
    if (tryStack.n) {
      if (tryStack.tail()->thenStmt == block) {
        tryStack.tail()->replace(block->remove());
        tryStack.pop();
      }
    }

    retval = expr;

  } else if (DefExpr* def = toDefExpr(expr)) {
    if (def->sym->hasFlag(FLAG_CHPL__ITER) == true) {
      implementForallIntents1(def);
    }

    retval = foldTryCond(postFold(expr));

  } else if (SymExpr* se = toSymExpr(expr)) {
    makeRefType(se->symbol()->type);

    if (ForallStmt* pfs = toForallForIteratedExpr(se)) {
      CallExpr* call = resolveParallelIteratorAndForallIntents(pfs, se);

      if (tryFailure == false) {
        retval = resolveExprPhase2(expr, fn, preFold(call));

      } else {
        retval = resolveExprHandleTryFailure(fn);
      }

    } else {
      retval = resolveExprPhase2(expr, fn, expr);
    }

  } else if (CallExpr* call = toCallExpr(expr)) {
    retval = resolveExprPhase2(expr, fn, preFold(call));

  } else {
    retval = foldTryCond(postFold(expr));
  }

  return retval;
}

static bool isParamResolved(FnSymbol* fn, Expr* expr) {
  bool retval = false;

  if (fn != NULL && fn->retTag == RET_PARAM) {
    if (BlockStmt* block = toBlockStmt(expr)) {
      if (block->isWhileStmt() == true) {
        USR_FATAL(expr,
                  "param function cannot contain a non-param while loop");

      } else if (block->isForLoop() == true) {
        USR_FATAL(expr, "param function cannot contain a non-param for loop");

      } else if (block->isLoopStmt() == true) {
        USR_FATAL(expr, "param function cannot contain a non-param loop");
      }
    }

    if (BlockStmt* block = toBlockStmt(expr->parentExpr)) {
      if (isCondStmt(block->parentExpr)) {
        USR_FATAL(block->parentExpr,
                  "param function cannot contain a non-param conditional");
      }
    }

    if (paramMap.get(fn->getReturnSymbol())) {
      CallExpr* call = toCallExpr(fn->body->body.tail);

      INT_ASSERT(call);
      INT_ASSERT(call->isPrimitive(PRIM_RETURN));

      call->get(1)->replace(new SymExpr(paramMap.get(fn->getReturnSymbol())));

      retval = true;
    }
  }

  return retval;
}

static ForallStmt* toForallForIteratedExpr(SymExpr* expr) {
  ForallStmt* retval = NULL;

  if (ForallStmt* pfs = toForallStmt(expr->parentExpr)) {
    if (pfs->isIteratedExpression(expr) == true) {
      retval = pfs;
    }
  }

  return retval;
}

static Expr* resolveExprPhase2(Expr* origExpr, FnSymbol* fn, Expr* expr) {
  Expr* retval = NULL;

  if (SymExpr* symExpr = toSymExpr(expr)) {
    resolveExprTypeConstructor(symExpr);

    retval = foldTryCond(postFold(expr));

  } else if (CallExpr* call = toCallExpr(expr)) {
    if (call->isPrimitive(PRIM_ERROR)   == true  ||
        call->isPrimitive(PRIM_WARNING) == true) {
      resolveExprMaybeIssueError(call);
    }

    callStack.add(call);

    INT_ASSERT(tryFailure == false);

    resolveCall(call);

    if (tryFailure == false && call->isResolved() == true) {
      if (CallExpr* origToLeaderCall = toPrimToLeaderCall(origExpr)) {
        // ForallLeaderArgs: process the leader that 'call' invokes.
        implementForallIntents2(call, origToLeaderCall);

      } else if (CallExpr* eflopiHelper = eflopiMap[call]) {
        implementForallIntents2wrapper(call, eflopiHelper);
      }

      if (ContextCallExpr* cc = toContextCallExpr(call->parentExpr)) {
        expr = resolveExprResolveEachCall(cc);

      } else {
        resolveFns(call->resolvedFunction());
      }

      resolveExprExpandGenerics(call);
    }

    if (tryFailure == false) {
      callStack.pop();

      retval = foldTryCond(postFold(expr));

    } else {
      retval = resolveExprHandleTryFailure(fn);
    }

  } else {
    retval = foldTryCond(postFold(expr));
  }

  return retval;
}

// A ContextCallExpr wraps 2 or 3 CallExprs.
// Resolve every call and perform semantic checks
static Expr* resolveExprResolveEachCall(ContextCallExpr* cc) {
  FnSymbol* valueFn    = NULL;
  FnSymbol* constRefFn = NULL;
  FnSymbol* refFn      = NULL;
  int       n          =    0;
  int       nIterator  =    0;

  if (CallExpr* tmpCall = cc->getValueCall()) {
    valueFn    = tmpCall->resolvedFunction();

    resolveFns(valueFn);

    n         += 1;
    nIterator += (valueFn->isIterator()    == true) ? 1 : 0;
  }

  if (CallExpr* tmpCall = cc->getConstRefCall()) {
    constRefFn = tmpCall->resolvedFunction();

    resolveFns(constRefFn);

    n         += 1;
    nIterator += (constRefFn->isIterator() == true) ? 1 : 0;
  }

  if (CallExpr* tmpCall = cc->getRefCall()) {
    refFn      = tmpCall->resolvedFunction();

    resolveFns(refFn);

    n         += 1;
    nIterator += (refFn->isIterator()      == true) ? 1 : 0;
  }

  // If there are no iterators then confirm that the return types match.
  //    (The return type of an iterator is not the yielded type)
  if (nIterator == 0) {
    if (contextTypesMatch(valueFn, constRefFn, refFn) == false) {
      USR_FATAL_CONT(cc,
                     "invalid return intent overload: return types differ");

      contextTypeInfo(valueFn);
      contextTypeInfo(constRefFn);
      contextTypeInfo(refFn);

      USR_STOP();
    }

  // If there are any iterators then they must all be iterators
  } else if (nIterator != n) {
    USR_FATAL_CONT(cc, "invalid ref return pair: mixing proc and iter");

    if (valueFn    != NULL) USR_FATAL_CONT(valueFn,    "here");
    if (constRefFn != NULL) USR_FATAL_CONT(constRefFn, "here");
    if (refFn      != NULL) USR_FATAL_CONT(refFn,      "here");
  }

  // Return the "designated call"
  return getDesignatedCall(cc);
}

static bool contextTypesMatch(FnSymbol* valueFn,
                              FnSymbol* constRefFn,
                              FnSymbol* refFn) {
  Type* type   = NULL;
  bool  retval = true;

  if (valueFn != NULL) {
    type   = valueFn->retType->getValType();
  }

  if (constRefFn != NULL) {
    Type* retType = constRefFn->retType->getValType();

    retval = (type == NULL || type == retType) ? true : false;
    type   = retType;
  }

  if (refFn != NULL) {
    retval = (type == refFn->retType->getValType()) ? true : false;
  }

  return retval;
}

static void contextTypeInfo(FnSymbol* fn) {
  if (fn != NULL) {
    USR_FATAL_CONT(fn, "function returns %s", toString(fn->retType));
  }
}

static void resolveExprExpandGenerics(CallExpr* call) {
  for (int i = 1; i <= call->numActuals(); i++) {
    Expr*   actualExpr = call->get(i);
    Symbol* actualSym  = NULL;

    if (SymExpr* actual = toSymExpr(actualExpr)) {
      actualSym = actual->symbol();

    } else if (NamedExpr* named = toNamedExpr(actualExpr)) {
      SymExpr* namedSe = toSymExpr(named->actual);

      INT_ASSERT(namedSe);

      actualSym = namedSe->symbol();

    } else {
      INT_FATAL(actualExpr, "wasn't expecting this type of Expr");
    }

    if (actualSym->hasFlag(FLAG_DELAY_GENERIC_EXPANSION) == true &&
        actualSym->type->symbol->hasFlag(FLAG_GENERIC)   == true) {
      Symbol*        formal     = call->resolvedFunction()->getFormal(i);
      AggregateType* formalType = toAggregateType(formal->type);

      INT_ASSERT(formalType);
      INT_ASSERT(formalType->symbol->hasFlag(FLAG_GENERIC) == false);

      // The type has been determined to no longer be generic.
      // Update the delayed instance to have the right type.
      actualSym->type = formalType;

      actualSym->removeFlag(FLAG_DELAY_GENERIC_EXPANSION);

      formalType->initializerResolved = true;

      if (actualSym->hasFlag(FLAG_SUPER_TEMP)) {
        if (FnSymbol* fn = toFnSymbol(actualExpr->parentSymbol)) {
          if (fn->_this != NULL && isClass(fn->_this->type) == true) {
            AggregateType* ct         = toAggregateType(fn->_this->type);
            Symbol*        superField = ct->getField(1);

            if (superField->hasFlag(FLAG_DELAY_GENERIC_EXPANSION)) {
              ct              = ct->getInstantiationParent(formalType);
              fn->_this->type = ct;

              superField      = ct->getField(1);

              superField->removeFlag(FLAG_DELAY_GENERIC_EXPANSION);
            }
          }
        }
      }
    }
  }
}

static void resolveExprTypeConstructor(SymExpr* symExpr) {
  if (AggregateType* ct = toAggregateType(symExpr->typeInfo())) {
    if (ct->defaultTypeConstructor                != NULL   &&
        ct->symbol->hasFlag(FLAG_GENERIC)         == false  &&
        ct->symbol->hasFlag(FLAG_ITERATOR_CLASS)  == false  &&
        ct->symbol->hasFlag(FLAG_ITERATOR_RECORD) == false) {
      CallExpr* parent = toCallExpr(symExpr->parentExpr);
      Symbol*   sym    = symExpr->symbol();

      if (parent                               == NULL  ||
          parent->isPrimitive(PRIM_IS_SUBTYPE) == false ||
          sym->hasFlag(FLAG_TYPE_VARIABLE)     == false) {

        // Don't try to resolve the defaultTypeConstructor for string
        // literals (resolution ordering issue, string literals are
        // encountered too early and we don't know enough to be able
        // to resolve them at that point)
        if (ct != dtString ||
            (sym->isParameter()                    == false   &&
             sym->hasFlag(FLAG_INSTANTIATED_PARAM) == false))  {
          resolveFormals(ct->defaultTypeConstructor);

          if (resolvedFormals.set_in(ct->defaultTypeConstructor)) {
            if (hasPartialCopyData(ct->defaultTypeConstructor) == true) {
              instantiateBody(ct->defaultTypeConstructor);
            }

            resolveFns(ct->defaultTypeConstructor);
          }
        }
      }
    }
  }
}

static Expr* resolveExprHandleTryFailure(FnSymbol* fn) {
  Expr* retval = NULL;

  if (tryStack.n > 0 && tryStack.tail()->parentSymbol == fn) {
    // The code in the 'true' branch of a tryToken conditional has failed
    // to resolve fully. Roll the callStack back to the function where
    // the nearest tryToken conditional is and replace the entire
    // conditional with the 'false' branch then continue resolution on
    // it.  If the 'true' branch did fully resolve, we would replace the
    // conditional with the 'true' branch instead.
    BlockStmt* elseBlock  = tryStack.tail()->elseStmt;
    Symbol*    elseParent = elseBlock->parentSymbol;

    while (callStack.n                          >  0 &&
           callStack.tail()->resolvedFunction() != elseParent) {
      callStack.pop();
    }

    tryStack.tail()->replace(elseBlock->remove());
    tryStack.pop();

    if (elseBlock->prev == NULL) {
      elseBlock->insertBefore(new CallExpr(PRIM_NOOP));
    }

    tryFailure = false;
    retval     = elseBlock->prev;
  }

  return retval;
}

static void resolveExprMaybeIssueError(CallExpr* call) {
  //
  // Disable compiler warnings in internal modules that are triggered within
  // a dynamic dispatch context to reduce potential user confusion.
  //
  if (call->isPrimitive(PRIM_ERROR)         == true          ||
      call->getModule()->modTag             != MOD_INTERNAL  ||
      inDynamicDispatchResolution           == false         ||
      callStack.head()->getModule()->modTag != MOD_INTERNAL) {

    //
    // If an errorDepth was specified, report a diagnostic about the call
    // that deep into the callStack. The default depth is 1.
    //
    FnSymbol*   fn            = toFnSymbol(call->parentSymbol);
    DefExpr*    lastFormal    = toDefExpr(fn->formals.tail);
    VarSymbol*  depthParam    = toVarSymbol(paramMap.get(lastFormal->sym));
    int         depth         = 0;
    int         head          = 0;
    bool        foundDepthVal = false;
    CallExpr*   from          = NULL;
    const char* str           = "";
    VarSymbol*  var           = NULL;

    if (depthParam                        != NULL &&
        depthParam->immediate             != NULL &&
        depthParam->immediate->const_kind == NUM_KIND_INT) {
      depth         = (int) depthParam->immediate->int_value();
      foundDepthVal = true;

      if (depth > callStack.n - 1) {
        USR_WARN(call,
                 "compiler diagnostic depth value exceeds call stack depth");

        depth = callStack.n - 1;

      } else if (depth < 0) {
        USR_WARN(call, "compiler diagnostic depth value can not be negative");

        depth = 0;
      }

    } else {
      depth         = (callStack.n == 1) ? 0 : 1;
      foundDepthVal = false;
    }

    head = callStack.n - 1 - depth;

    for (int i = head; i >= 0; i--) {
      CallExpr*     frame  = callStack.v[i];
      ModuleSymbol* module = frame->getModule();
      FnSymbol*     fn     = frame->getFunction();

      from = frame;

      if (frame->linenum()                     >  0             &&
          fn->hasFlag(FLAG_COMPILER_GENERATED) == false         &&
          module->modTag                       != MOD_INTERNAL) {
        break;
      }
    }

    for_formals(arg, fn) {
      if (foundDepthVal == false || arg->defPoint != fn->formals.tail) {
        var = toVarSymbol(paramMap.get(arg));

        INT_ASSERT(var                        != NULL &&
                   var->immediate             != NULL &&
                   var->immediate->const_kind == CONST_KIND_STRING);

        str = astr(str, var->immediate->v_string);
      }
    }

    // collapse newlines and other escape sequences before printing
    str = astr(unescapeString(str, var).c_str());

    if (call->isPrimitive(PRIM_ERROR) == true) {
      USR_FATAL(from, "%s", str);
    } else {
      gdbShouldBreakHere();
      USR_WARN (from, "%s", str);
    }

    if (FnSymbol* fn = callStack.tail()->resolvedFunction())  {
      innerCompilerWarningMap.put(fn, str);
    }

    if (FnSymbol* fn = callStack.v[head]->resolvedFunction()) {
      outerCompilerWarningMap.put(fn, str);
    }
  }
}

static Expr* foldTryCond(Expr* expr) {
  Expr* retval = expr;

  if (CondStmt* cond = toCondStmt(expr->parentExpr)) {
    if (cond->condExpr == expr) {
      if (CallExpr* noop = cond->foldConstantCondition()) {
        retval = noop;

      } else {
        // push try block
        if (SymExpr* se = toSymExpr(expr)) {
          if (se->symbol() == gTryToken) {
            tryStack.add(cond);
          }
        }
      }
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void
computeReturnTypeParamVectors(BaseAST* ast,
                              Symbol* retSymbol,
                              Vec<Type*>& retTypes,
                              Vec<Symbol*>& retParams) {
  if (CallExpr* call = toCallExpr(ast)) {
    if (call->isPrimitive(PRIM_MOVE)) {
      if (SymExpr* sym = toSymExpr(call->get(1))) {
        if (sym->symbol() == retSymbol) {
          if (SymExpr* sym = toSymExpr(call->get(2)))
            retParams.add(sym->symbol());
          else
            retParams.add(NULL);
          retTypes.add(call->get(2)->typeInfo());
        }
      }
    }
  }

  AST_CHILDREN_CALL(ast, computeReturnTypeParamVectors, retSymbol, retTypes, retParams);
}

static void
insertCasts(BaseAST* ast, FnSymbol* fn, Vec<CallExpr*>& casts) {
  if (CallExpr* call = toCallExpr(ast)) {
    if (call->parentSymbol == fn) {
      if (call->isPrimitive(PRIM_MOVE)) {
        if (SymExpr* lhs = toSymExpr(call->get(1))) {
          Type* lhsType = lhs->symbol()->type;
          if (lhsType != dtUnknown) {
            Expr* rhs = call->get(2);
            Type* rhsType = rhs->typeInfo();
            CallExpr* rhsCall = toCallExpr(rhs);

            if (call->id == breakOnResolveID)
              gdbShouldBreakHere();

            if (rhsCall && rhsCall->isPrimitive(PRIM_COERCE)) {
              rhsType = rhsCall->get(1)->typeInfo();
            }

            // would this be simpler with getValType?
            bool typesDiffer = (rhsType != lhsType &&
                                rhsType->refType != lhsType &&
                                rhsType != lhsType->refType);

            SET_LINENO(rhs);

            // Generally, we want to add casts for PRIM_MOVE that have two
            // different types. This function also handles PRIM_COERCE on the
            // right-hand side by either removing the PRIM_COERCE entirely if
            // the types are the same, or by using a = call if the types are
            // different. It could use a _cast call if the types are different,
            // but the = call works better in cases where an array is returned.

            if (rhsCall && rhsCall->isPrimitive(PRIM_COERCE)) {
              // handle move lhs, coerce rhs
              SymExpr* fromExpr = toSymExpr(rhsCall->get(1));
              SymExpr* fromTypeExpr = toSymExpr(rhsCall->get(2));
              Symbol* from = fromExpr->symbol();
              Symbol* fromType = fromTypeExpr->symbol();
              Symbol* to = lhs->symbol();

              // Check that lhsType == the result of coercion
              INT_ASSERT(lhsType == rhsCall->typeInfo());

              if (!typesDiffer) {
                // types are the same. remove coerce and
                // handle reference level adjustments. No cast necessary.

                if (rhsType == lhsType)
                  rhs = new SymExpr(from);
                else if (rhsType == lhsType->refType)
                  rhs = new CallExpr(PRIM_DEREF, new SymExpr(from));
                else if (rhsType->refType == lhsType)
                  rhs = new CallExpr(PRIM_ADDR_OF, new SymExpr(from));

                CallExpr* move = new CallExpr(PRIM_MOVE, to, rhs);
                call->replace(move);
                casts.add(move);
              } else if (lhsType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE) ||
                         rhsType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE) ) {

                // Use = if the types differ.  This should cause the 'from'
                // value to be coerced to 'to' if possible or result in an
                // compilation error. We use = here (vs _cast) in order to work
                // better with returning arrays. We could probably use _cast
                // instead of = if fromType does not have a runtime type.

                CallExpr* init = new CallExpr(PRIM_NO_INIT, fromType);
                CallExpr* moveInit = new CallExpr(PRIM_MOVE, to, init);
                call->insertBefore(moveInit);

                // By resolving =, we will generate an error if from cannot be
                // coerced into to.
                CallExpr* assign = new CallExpr("=", to, from);
                call->insertBefore(assign);

                // Resolve each of the new CallExprs They need to be resolved
                // separately since resolveExpr does not recurse.
                resolveExpr(init);
                resolveExpr(moveInit);
                resolveExpr(assign);

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

                CallExpr* cast = createCast(tmp, lhsType->symbol);
                call->insertAtTail(cast);
                casts.add(cast);
              }

            } else {
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
                  call->insertBefore(new CallExpr(PRIM_MOVE, tmp, rhs->copy()));
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
                  CallExpr* move = new CallExpr(PRIM_MOVE, lhs, new CallExpr(PRIM_DEREF, rhs));
                  call->replace(move);
                  casts.add(move);
                }
              }
            }
          }
        }
      }
    }
  }
  AST_CHILDREN_CALL(ast, insertCasts, fn, casts);
}

//
// insert casts as necessary
//
void insertAndResolveCasts(FnSymbol* fn) {
  if (fn->retTag != RET_PARAM) {
    Vec<CallExpr*> casts;

    insertCasts(fn->body, fn, casts);

    forv_Vec(CallExpr, cast, casts) {
      resolveCallAndCallee(cast, true);
    }
  }
}


static void instantiate_default_constructor(FnSymbol* fn) {
  //
  // instantiate initializer
  //
  // but don't bother for tuples since tuple init is created
  // along with tuple type.
  //
  if (fn->instantiatedFrom && !fn->hasFlag(FLAG_PARTIAL_TUPLE)) {
    AggregateType* retAt = toAggregateType(fn->retType);
    INT_ASSERT(retAt);

    INT_ASSERT(!retAt->defaultInitializer);
    FnSymbol* instantiatedFrom = fn->instantiatedFrom;
    while (instantiatedFrom->instantiatedFrom)
      instantiatedFrom = instantiatedFrom->instantiatedFrom;

    AggregateType* instanceRetAt = toAggregateType(instantiatedFrom->retType);
    INT_ASSERT(instanceRetAt);

    CallExpr* call = new CallExpr(instanceRetAt->defaultInitializer);

    // This should not be happening for iterators.
    TypeSymbol* ts = instanceRetAt->symbol;
    INT_ASSERT(!ts->hasEitherFlag(FLAG_ITERATOR_RECORD, FLAG_ITERATOR_CLASS));

    for_formals(formal, fn) {
      if (formal->type == dtMethodToken || formal == fn->_this) {
        call->insertAtTail(formal);
      } else if (paramMap.get(formal)) {
        call->insertAtTail(new NamedExpr(formal->name, new SymExpr(paramMap.get(formal))));
      } else {
        Symbol* field = fn->retType->getField(formal->name);
        if (instantiatedFrom->hasFlag(FLAG_TUPLE)) {
          call->insertAtTail(field);
        } else {
          call->insertAtTail(new NamedExpr(formal->name, new SymExpr(field)));
        }
      }
    }
    fn->insertBeforeEpilogue(call);
    resolveCall(call);
    retAt->defaultInitializer = call->resolvedFunction();
    INT_ASSERT(retAt->defaultInitializer);
    //      resolveFns(retAt->defaultInitializer);
    call->remove();
  }
}

void resolveReturnType(FnSymbol* fn)
{
  // Resolve return type.
  Symbol* ret = fn->getReturnSymbol();
  Type* retType = ret->type;

  if (retType == dtUnknown) {

    Vec<Type*> retTypes;
    Vec<Symbol*> retParams;
    computeReturnTypeParamVectors(fn, ret, retTypes, retParams);

    if (retTypes.n == 1)
      retType = retTypes.head();
    else if (retTypes.n > 1) {
      // adjust the reference level:
      //  if they are all references, leave it that way
      //  otherwise make them all values.
      bool allRef = true;
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
            if (!canDispatch(retTypes.v[j], retParams.v[j], retTypes.v[i], fn, &requireScalarPromotion))
              best = false;
            if (requireScalarPromotion)
              best = false;
          }
        }
        if (best) {
          retType = retTypes.v[i];
          break;
        }
      }
    }
    if (!fn->iteratorInfo) {
      if (retTypes.n == 0)
        retType = dtVoid;
    }
  }

  // For tuples, generally do not allow a tuple to contain a reference
  // when it is returned
  if (retType->symbol->hasFlag(FLAG_TUPLE) &&
      !doNotChangeTupleTypeRefLevel(fn, true)) {
    // Compute the tuple type without any refs
    // Set the function return type to that type.
    AggregateType* tupleType = toAggregateType(retType);
    INT_ASSERT(tupleType);
    retType = getReturnedTupleType(fn, tupleType);
  }

  ret->type = retType;
  if (!fn->iteratorInfo) {
    if (retType == dtUnknown)
      USR_FATAL(fn, "unable to resolve return type");
    fn->retType = retType;
  }

}

void ensureInMethodList(FnSymbol* fn) {
  if (fn->hasFlag(FLAG_METHOD) && fn->_this != NULL) {
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

// Simple wrappers to check if a function is a specific type of iterator
static bool isIteratorOfType(FnSymbol* fn, Symbol* iterTag) {
  if (fn->isIterator()) {
    for_formals(formal, fn) {
      if (formal->type == iterTag->type && paramMap.get(formal) == iterTag) {
        return true;
      }
    }
  }
  return false;
}
bool isLeaderIterator(FnSymbol* fn) {
  return isIteratorOfType(fn, gLeaderTag);
}
static bool isFollowerIterator(FnSymbol* fn) {
  return isIteratorOfType(fn, gFollowerTag);
}
bool isStandaloneIterator(FnSymbol* fn) {
  return isIteratorOfType(fn, gStandaloneTag);
}

// helper to identify explicitly vectorized iterators
static bool isVecIterator(FnSymbol* fn) {
  if (fn->isIterator()) {
    return fn->hasFlag(FLAG_VECTORIZE_YIELDING_LOOPS);
  }
  return false;
}

//
// If the returnSymbol of 'fn' is assigned to from an _array record, insert
// an autoCopy for that _array. If the autoCopy has not yet been resolved, this
// function will call 'resolveAutoCopyEtc'.
//
// This supports the 'copy-out' rule for returning arrays.
//
static void insertUnrefForArrayReturn(FnSymbol* fn) {
  Symbol* ret = fn->getReturnSymbol();

  // BHARSH: Should this also check if fn->retTag != RET_TYPE?
  //
  // Do nothing for these kinds of functions:
  if (fn->hasFlag(FLAG_CONSTRUCTOR) ||
      fn->hasFlag(FLAG_NO_COPY_RETURN) ||
      fn->hasFlag(FLAG_UNALIAS_FN) ||
      fn->hasFlag(FLAG_RUNTIME_TYPE_INIT_FN) ||
      fn->hasFlag(FLAG_INIT_COPY_FN) ||
      fn->hasFlag(FLAG_AUTO_COPY_FN) ||
      fn->hasFlag(FLAG_IF_EXPR_FN) ||
      fn->hasFlag(FLAG_RETURNS_ALIASING_ARRAY)) {
    return;
  }

  for_SymbolSymExprs(se, ret) {
    if (CallExpr* call = toCallExpr(se->parentExpr)) {
      if (call->isPrimitive(PRIM_MOVE) && se == call->get(1)) {
        Type* rhsType = call->get(2)->typeInfo();
        // TODO: Should we check if the RHS is a symbol with 'no auto
        // destroy' on it? If it is, then we'd be copying the RHS and it
        // would never be destroyed...
        if (rhsType->symbol->hasFlag(FLAG_ARRAY) &&
            isTypeExpr(call->get(2)) == false) {
          Expr* origRHS = call->get(2)->remove();
          VarSymbol* tmp = newTemp(arrayUnrefName, rhsType);

          // Used by callDestructors to catch assignment from a ref to
          // 'tmp' when we know we don't want to copy.
          tmp->addFlag(FLAG_NO_COPY);

          call->insertBefore(new DefExpr(tmp));
          CallExpr* init_unref_tmp = new CallExpr(PRIM_MOVE, tmp, origRHS->copy());
          call->insertBefore(init_unref_tmp);

          CallExpr* unrefCall = new CallExpr("chpl__unref", tmp);
          call->insertAtTail(unrefCall);
          FnSymbol* unrefFn = resolveNormalCall(unrefCall);
          resolveFns(unrefFn);
          // Relies on the ArrayView variant having the 'unref fn' flag in
          // ChapelArray.
          if (unrefFn->hasFlag(FLAG_UNREF_FN) == false) {
            // If the function does not have this flag, we must be dealing with
            // a non-view array, so we can remove the useless unref call.
            unrefCall->replace(origRHS->copy());

            // Remove now-useless AST
            tmp->defPoint->remove();
            init_unref_tmp->remove();
            INT_ASSERT(unrefCall->inTree() == false);
          }
        }
      }
    }
  }
}

void
resolveFns(FnSymbol* fn) {
  if (fn->isResolved())
    return;

  if (fn->id == breakOnResolveID) {
    printf("breaking on resolve fn:\n");
    print_view(fn);
    gdbShouldBreakHere();
  }

  fn->addFlag(FLAG_RESOLVED);

  if (fn->hasFlag(FLAG_EXTERN)) {
    resolveBlockStmt(fn->body);
    resolveReturnType(fn);
    return;
  }

  //
  // Mark serial loops that yield inside of follower, standalone, and
  // explicitly vectorized iterators as order independent. By using a forall
  // loop or a loop over a vectorized iterator, a user is asserting that the
  // loop can be executed in any iteration order. Here we just mark the
  // iterator's yielding loops as order independent as they are ones that will
  // actually execute the body of the loop that invoked the iterator. Note that
  // for nested loops with a single yield, only the inner most loop is marked.
  //
  if (isFollowerIterator(fn)   ||
      isStandaloneIterator(fn) ||
      isVecIterator(fn)) {
    std::vector<CallExpr*> callExprs;

    collectCallExprs(fn->body, callExprs);

    for_vector(CallExpr, call, callExprs) {
      if (call->isPrimitive(PRIM_YIELD)) {
        if (LoopStmt* loop = LoopStmt::findEnclosingLoop(call)) {
          if (loop->isCoforallLoop() == false) {
            loop->orderIndependentSet(true);
          }
        }
      }
    }
  }

  //
  // Mark leader and standalone parallel iterators for inlining. Also stash a
  // pristine copy of the iterator (required by forall intents)
  //
  if (isLeaderIterator(fn) || isStandaloneIterator(fn)) {
    fn->addFlag(FLAG_INLINE_ITERATOR);
    stashPristineCopyOfLeaderIter(fn, /*ignore_isResolved:*/ true);
  }

  insertFormalTemps(fn);

  resolveBlockStmt(fn->body);

  if (tryFailure) {
    fn->removeFlag(FLAG_RESOLVED);
    return;
  }

  if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)) {
    AggregateType* ct = toAggregateType(fn->retType);

    if (!ct) {
      INT_FATAL(fn, "Constructor has no class type");
    }

    setScalarPromotionType(ct);

    if (!developer) {
      fixTypeNames(ct);
    }
  }


  insertUnrefForArrayReturn(fn);
  resolveReturnType(fn);

  //
  // insert casts as necessary
  //
  insertAndResolveCasts(fn);

  if (fn->isIterator() && !fn->iteratorInfo) {
    protoIteratorClass(fn);
  }

  // Resolve base class type constructors as well.
  if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)) {
    forv_Vec(Type, parent, fn->retType->dispatchParents) {
      AggregateType* pt = toAggregateType(parent);
      if (pt                         != NULL     &&
          pt                         != dtObject &&
          pt->defaultTypeConstructor != NULL) {
        resolveFormals(pt->defaultTypeConstructor);

        if (resolvedFormals.set_in(pt->defaultTypeConstructor)) {
          resolveFns(pt->defaultTypeConstructor);
        }
      }
    }
    AggregateType* ct = toAggregateType(fn->retType);

    if (ct) {
      for_fields(field, ct) {
        if (AggregateType* fct = toAggregateType(field->type)) {
          if (fct->defaultTypeConstructor) {
            resolveFormals(fct->defaultTypeConstructor);

            if (resolvedFormals.set_in(fct->defaultTypeConstructor)) {
              resolveFns(fct->defaultTypeConstructor);
            }
          }
        }
      }
    }

    if (ct                   != NULL &&
        ct->instantiatedFrom != NULL &&
        (ct->initializerStyle          == DEFINES_INITIALIZER ||
         ct->wantsDefaultInitializer() == true)) {
      // Don't instantiate the default constructor for generic types that
      // define initializers, they don't have one!

    } else {
      // This instantiates the default constructor
      // for  the corresponding type constructor.
      instantiate_default_constructor(fn);
    }

    //
    // resolve destructor
    //
    if (ct                                  != NULL  &&
        ct->hasDestructor()                 == false &&
        ct->symbol->hasFlag(FLAG_REF)       == false &&
        isTupleContainingOnlyReferences(ct) == false) {
      BlockStmt* block = new BlockStmt();
      VarSymbol* tmp   = newTemp(ct);
      CallExpr*  call  = new CallExpr("deinit", gMethodToken, tmp);

      // In case resolveCall drops other stuff into the tree ahead
      // of the call, we wrap everything in a block for safe removal.

      block->insertAtHead(call);

      fn->insertAtHead(block);
      fn->insertAtHead(new DefExpr(tmp));

      resolveCallAndCallee(call);

      ct->setDestructor(call->resolvedFunction());

      block->remove();

      tmp->defPoint->remove();
    }
  }

  //
  // mark privatized classes
  //
  if (fn->hasFlag(FLAG_PRIVATIZED_CLASS)) {
    if (fn->getReturnSymbol() == gTrue) {
      fn->getFormal(1)->type->symbol->addFlag(FLAG_PRIVATIZED_CLASS);
    }
  }

  //
  // make sure methods are in the methods list
  //
  ensureInMethodList(fn);
}

void
parseExplainFlag(char* flag, int* line, ModuleSymbol** module) {
  *line = 0;
  if (strcmp(flag, "")) {
    char *token, *str1 = NULL, *str2 = NULL;
    token = strstr(flag, ":");
    if (token) {
      *token = '\0';
      str1 = token+1;
      token = strstr(str1, ":");
      if (token) {
        *token = '\0';
        str2 = token + 1;
      }
    }
    if (str1) {
      if (str2 || !atoi(str1)) {
        forv_Vec(ModuleSymbol, mod, allModules) {
          if (!strcmp(mod->name, str1)) {
            *module = mod;
            break;
          }
        }
        if (!*module)
          USR_FATAL("invalid module name '%s' passed to --explain-call flag", str1);
      } else
        *line = atoi(str1);
      if (str2)
        *line = atoi(str2);
    }
    if (*line == 0)
      *line = -1;
  }
}


static void resolveExternVarSymbols()
{
  forv_Vec(VarSymbol, vs, gVarSymbols)
  {
    if (! vs->hasFlag(FLAG_EXTERN))
      continue;

    DefExpr* def = vs->defPoint;
    Expr* init = def->next;
    // We expect the expression following the DefExpr for an extern to be a
    // type block that initializes the variable.
    BlockStmt* block = toBlockStmt(init);
    if (block)
      resolveBlockStmt(block);
  }
}


static void
computeStandardModuleSet() {
  // Lydia NOTE: 09/12/16 - this code does not follow the same code path used
  // to add the standard module set to the root module's block, so misses some
  // cases, causing qualified access to functions from certain modules (List,
  // Search, Sort, at the time of this writing) to fail to resolve.  We
  // should take the time to time this optimization to the code path it wishes
  // to follow - however, I am not sure where that is occurring right now.

  // Private uses will also help avoid the bug, but it is sweeping the bug under
  // the rug rather than solving the problem at hand, and it is hard to say
  // when the next time this code will bite us will be.
  standardModuleSet.set_add(rootModule->block);
  standardModuleSet.set_add(theProgram->block);

  Vec<ModuleSymbol*> stack;
  stack.add(standardModule);

  while (ModuleSymbol* mod = stack.pop()) {
    if (mod->block->useList) {
      for_actuals(expr, mod->block->useList) {
        UseStmt* use = toUseStmt(expr);
        INT_ASSERT(use);
        SymExpr* se = toSymExpr(use->src);
        INT_ASSERT(se);
        if (ModuleSymbol* usedMod = toModuleSymbol(se->symbol())) {
          INT_ASSERT(usedMod);
          if (!standardModuleSet.set_in(usedMod->block)) {
            stack.add(usedMod);
            standardModuleSet.set_add(usedMod->block);
          }
        }
      }
    }
  }
}


void resolve() {
  bool changed = true;

  parseExplainFlag(fExplainCall, &explainCallLine, &explainCallModule);

  computeStandardModuleSet(); // Lydia NOTE 09/12/16: is not linked to our
  // treatment on functions included by default, leading to bugs with qualified
  // access to symbols included in this way.

  // call _nilType nil so as to not confuse the user
  dtNil->symbol->name = gNil->name;

  // Iterate until all generic functions have been tagged with FLAG_GENERIC
  while (changed == true) {
    changed = false;

    forv_Vec(FnSymbol, fn, gFnSymbols) {
      // Returns true if status of fn is changed
      if (fn->tagIfGeneric() == true) {
        changed = true;
      }
    }
  }

  unmarkDefaultedGenerics();

  resolveExternVarSymbols();

  resolveUses(ModuleSymbol::mainModule());

  // --ipe does not build printModuleInitModule
  if (printModuleInitModule)
    resolveUses(printModuleInitModule);

  // --ipe does not build chpl_gen_main
  if (chpl_gen_main)
    resolveFns(chpl_gen_main);

  resolveSupportForModuleDeinits();

  USR_STOP();

  resolveExports();

  resolveEnumTypes();

  insertRuntimeTypeTemps();

  resolveAutoCopies();

  resolveRecordInitializers();

  resolveOther();

  resolveDynamicDispatches();

  // MPF - this 2nd resolveAutoCopies call is a workaround
  // for the case when resolving a dynamic dispatch created a
  // new type. Resolution should instead have some sort of work-queue
  // and iterate until everything is resolved. (Or at least
  // resolveDynamicDispatches and resolveAutoCopies should be called
  // in a loop).
  resolveAutoCopies();

  resolveSerializers();

  insertDynamicDispatchCalls();

  beforeLoweringForallStmts = false;
  lowerForallStmts();

  insertReturnTemps();

  // Resolve the string literal constructors after everything else since new
  // ones may be created during postFold
  ensureAndResolveInitStringLiterals();

  handleRuntimeTypes();

  if (fPrintCallGraph) {
    // This needs to go after resolution is complete, but before
    // pruneResolvedTree() removes unused functions (like the uninstantiated
    // versions of generic functions).
    printCallGraph();
  }

  pruneResolvedTree();

  freeCache(ordersCache);
  freeCache(defaultsCache);
  freeCache(genericsCache);
  freeCache(coercionsCache);
  freeCache(promotionsCache);

  visibleFunctionsClear();

  std::map<int, SymbolMap*>::iterator it;

  for (it = capturedValues.begin(); it != capturedValues.end(); ++it) {
    delete it->second;
  }

  clearPartialCopyDataFnMap();

  forv_Vec(BlockStmt, stmt, gBlockStmts) {
    stmt->useListClear();
  }

  resolved = true;
}

static void unmarkDefaultedGenerics() {
  //
  // make it so that arguments with types that have default values for
  // all generic arguments used those defaults
  //
  // FLAG_MARKED_GENERIC is used to identify places where the user inserted
  // '?' (queries) to mark such a type as generic.
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!fn->inTree())
      continue;

    bool unmark = fn->hasFlag(FLAG_GENERIC);
    for_formals(formal, fn) {
      if (formal->type->hasGenericDefaults) {
        if (!formal->hasFlag(FLAG_MARKED_GENERIC) &&
            formal != fn->_this &&
            !formal->hasFlag(FLAG_IS_MEME)) {
          SET_LINENO(formal);
          AggregateType* formalAt = toAggregateType(formal->type);
          INT_ASSERT(formalAt);
          formal->typeExpr = new BlockStmt(new CallExpr(formalAt->defaultTypeConstructor));
          insert_help(formal->typeExpr, NULL, formal);
          formal->type = dtUnknown;
        } else {
          unmark = false;
        }
      } else if (formal->type->symbol->hasFlag(FLAG_GENERIC) || formal->intent == INTENT_PARAM) {
        unmark = false;
      }
    }
    if (unmark) {
      fn->removeFlag(FLAG_GENERIC);
      INT_ASSERT(false);
    }
  }
}

// Resolve uses in postorder (removing back-links).
// We have to resolve modules in dependency order,
// so that the types of globals are ready when we need them.
static void resolveUses(ModuleSymbol* mod)
{
  static Vec<ModuleSymbol*> initMods;
  static int module_resolution_depth = 0;

  // Test and set to break loops and prevent infinite recursion.
  if (initMods.set_in(mod))
    return;
  initMods.set_add(mod);

  ++module_resolution_depth;

  // I use my parent implicitly.
  if (ModuleSymbol* parent = mod->defPoint->getModule())
    if (parent != theProgram && parent != rootModule)
      resolveUses(parent);

  // Now, traverse my use statements, and call the initializer for each
  // module I use.
  forv_Vec(ModuleSymbol, usedMod, mod->modUseList)
    resolveUses(usedMod);

  // Finally, myself.
  if (fPrintModuleResolution) {
    fprintf(stderr, "%2d Resolving module %30s ...",
            module_resolution_depth, mod->name);
  }

  FnSymbol* fn = mod->initFn;
  resolveFormals(fn);
  resolveFns(fn);

  if (FnSymbol* defn = mod->deinitFn) {
    resolveFormals(defn);
    resolveFns(defn);
  }

  if (fPrintModuleResolution) {
    AstCount visitor = AstCount();
    mod->accept(&visitor);
    fprintf(stderr, " %6d asts\n", visitor.total());
  }

  --module_resolution_depth;
}

static void resolveSupportForModuleDeinits() {
  SET_LINENO(chpl_gen_main);

  Expr*      modNameDum = buildCStringLiteral("");
  VarSymbol* fnPtrDum   = newTemp("fnPtr", dtCFnPtr);
  CallExpr*  addModule  = new CallExpr("chpl_addModule", modNameDum, fnPtrDum);

  resolveUninsertedCall(chpl_gen_main->body, addModule);

  gAddModuleFn = addModule->resolvedFunction();

  resolveFns(gAddModuleFn);
}

static void resolveExports() {
  // We need to resolve any additional functions that will be exported.
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn == initStringLiterals) {
      // initStringLiterals is exported but is explicitly resolved last since
      // code may be added to it in postFold calls while resolving other
      // functions
      continue;
    }

    if (fn->hasFlag(FLAG_EXPORT)) {
      SET_LINENO(fn);
      resolveFormals(fn);
      resolveFns(fn);
    }
  }
}

static void resolveEnumTypes() {
  // need to handle enumerated types better
  forv_Vec(TypeSymbol, type, gTypeSymbols) {
    if (EnumType* et = toEnumType(type->type)) {
      SET_LINENO(et);
      ensureEnumTypeResolved(et);
    }
  }
}

static void insertRuntimeTypeTemps() {
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->defPoint &&
        ts->defPoint->parentSymbol &&
        ts->hasFlag(FLAG_HAS_RUNTIME_TYPE) &&
        !ts->hasFlag(FLAG_GENERIC)) {
      SET_LINENO(ts);
      AggregateType* at = toAggregateType(ts->type);
      INT_ASSERT(at);

      VarSymbol* tmp = newTemp("_runtime_type_tmp_", at);
      at->defaultInitializer->insertBeforeEpilogue(new DefExpr(tmp));
      CallExpr* call = new CallExpr("chpl__convertValueToRuntimeType", tmp);
      at->defaultInitializer->insertBeforeEpilogue(call);
      resolveCallAndCallee(call);
      valueToRuntimeTypeMap.put(at, call->resolvedFunction());
      call->remove();
      tmp->defPoint->remove();
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static FnSymbol* resolveNormalSerializer(CallExpr* call) {
  BlockStmt* block = new BlockStmt(call);

  chpl_gen_main->body->insertAtHead(block);

  tryResolveCall(call);

  block->remove();

  return call->resolvedFunction();
}

static bool resolveSerializeDeserialize(AggregateType* at) {
  SET_LINENO(at->symbol);
  VarSymbol* tmp          = newTemp(at);
  FnSymbol* serializeFn   = NULL;
  FnSymbol* deserializeFn = NULL;
  bool retval             = false;

  chpl_gen_main->insertAtHead(new DefExpr(tmp));

  CallExpr* serializeCall = new CallExpr("chpl__serialize", gMethodToken, tmp);
  serializeFn = resolveNormalSerializer(serializeCall);
  if (serializeFn != NULL && serializeFn->hasFlag(FLAG_PROMOTION_WRAPPER)) {
    // Without this check we would resolve a serializer for arrays despite a
    // serializer only being implemented for domains.
    serializeFn = NULL;
  }

  if (serializeFn != NULL) {
    resolveFns(serializeFn);
    Type* retType = serializeFn->retType->getValType();

    if (retType == dtVoid) {
      USR_FATAL(serializeFn, "chpl__serialize cannot return void");
    }

    if (isPrimitiveType(retType) == false && autoDestroyMap.get(retType) == NULL) {
      USR_FATAL_CONT(serializeFn, "chpl__serialize must return a type that can be automatically memory managed (e.g. a record)");
      serializeFn = NULL;
    } else {
      VarSymbol* data = newTemp(serializeFn->retType);
      chpl_gen_main->insertAtHead(new DefExpr(data));

      CallExpr* deserializeCall = new CallExpr("chpl__deserialize", gMethodToken, at->symbol, data);
      deserializeFn = resolveNormalSerializer(deserializeCall);

      if (deserializeFn != NULL) {
        resolveFns(deserializeFn);

        Type* retType = deserializeFn->retType->getValType();
        if (retType == dtVoid) {
          USR_FATAL(deserializeFn, "chpl__deserialize cannot return void");
        } else if (retType != at) {
          USR_FATAL(deserializeFn, "chpl__deserialize returning '%s' when it must return '%s'", retType->symbol->name, at->symbol->name);
        }
      }

      data->defPoint->remove();
    }
  }

  if (serializeFn != NULL && deserializeFn == NULL) {
    USR_WARN("Found chpl__serialize for type '%s', but did not find matching chpl__deserialize", at->symbol->cname);
  } else if (serializeFn != NULL && deserializeFn != NULL) {
    Serializers ser;
    ser.serializer = serializeFn;
    ser.deserializer = deserializeFn;
    serializeMap[at] = ser;

    retval = true;
  }

  tmp->defPoint->remove();

  return retval;
}

static void resolveBroadcasters(AggregateType* at) {
  SET_LINENO(at->symbol);
  Serializers& ser = serializeMap[at];
  INT_ASSERT(ser.serializer != NULL);

  VarSymbol* tmp = newTemp("global_temp", at);
  chpl_gen_main->insertAtHead(new DefExpr(tmp));

  FnSymbol* broadcastFn;
  FnSymbol* destroyFn;
  {
    SET_LINENO(tmp);
    CallExpr* call = new CallExpr("chpl__broadcastGlobal", tmp, new_IntSymbol(0, INT_SIZE_64));
    broadcastFn = resolveNormalSerializer(call);
    broadcastFn->addFlag(FLAG_BROADCAST_FN);
  }
  {
    SET_LINENO(tmp);
    CallExpr* call = new CallExpr("chpl__destroyBroadcastedGlobal", tmp, new_IntSymbol(0, INT_SIZE_64));
    destroyFn = resolveNormalSerializer(call);
  }
  if (broadcastFn == NULL || destroyFn == NULL) {
    INT_FATAL("Unable to resolve serialized broadcasting for type %s", at->symbol->cname);
  }

  resolveFns(broadcastFn);
  resolveFns(destroyFn);

  ser.broadcaster = broadcastFn;
  ser.destroyer   = destroyFn;
}

static void resolveSerializers() {
  if (fNoRemoteSerialization == true) {
    return;
  }

  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->defPoint->parentSymbol               != NULL   &&
        ts->hasFlag(FLAG_GENERIC)                == false  &&
        ts->hasFlag(FLAG_ITERATOR_RECORD)        == false &&
        isSingleType(ts->type)                   == false &&
        isSyncType(ts->type)                     == false &&
        ts->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION) == false) {
      if (AggregateType* at = toAggregateType(ts->type)) {
        if (isRecord(at) == true) {
          bool success = resolveSerializeDeserialize(at);
          if (success) {
            resolveBroadcasters(at);
          }
        }
      }
    }
  }

  resolveAutoCopies();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void        resolveAutoCopyEtc(AggregateType* at);
static const char* autoCopyFnForType(AggregateType* at);
static FnSymbol*   autoMemoryFunction(AggregateType* at, const char* fnName);

static void resolveAutoCopies() {
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->defPoint->parentSymbol               != NULL   &&
        ts->hasFlag(FLAG_GENERIC)                == false  &&
        ts->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION) == false) {
      if (AggregateType* at = toAggregateType(ts->type)) {
        if (isRecord(at) == true) {
          resolveAutoCopyEtc(at);
        }

        propagateNotPOD(at);
      }
    }
  }
}

static void resolveAutoCopyEtc(AggregateType* at) {
  SET_LINENO(at->symbol);

  if (isNonGenericRecordWithInitializers(at) == false) {
    // resolve autoCopy
    if (hasAutoCopyForType(at) == false) {
      FnSymbol* fn = autoMemoryFunction(at, autoCopyFnForType(at));

      autoCopyMap[at] = fn;
    }
  }

  // resolve autoDestroy
  if (autoDestroyMap.get(at) == NULL) {
    FnSymbol* fn = autoMemoryFunction(at, "chpl__autoDestroy");

    fn->addFlag(FLAG_AUTO_DESTROY_FN);

    autoDestroyMap.put(at, fn);
  }

  // resolve unalias
  // We make the 'unalias' hook available to all user records,
  // but for now it only applies to array/domain/distribution
  // in order to minimize the changes.
  if (unaliasMap.get(at) == NULL && isRecordWrappedType(at) == true) {
    FnSymbol* fn = autoMemoryFunction(at, "chpl__unalias");

    unaliasMap.put(at, fn);
  }
}

// Just use 'chpl__initCopy' instead of 'chpl__autoCopy'
// for user-defined records. This way, if the type does not
// support copying, the autoCopyMap will store a function
// marked with FLAG_ERRONEOUS_INITCOPY. Additionally, user-defined
// records shouldn't be defining chpl__initCopy or chpl__autoCopy
// and certainly shouldn't rely on the differences between the two.
static const char* autoCopyFnForType(AggregateType* at) {
  const char* retval = "chpl__autoCopy";

  if (isUserDefinedRecord(at)                == true  &&

      at->symbol->hasFlag(FLAG_TUPLE)        == false &&
      isRecordWrappedType(at)                == false &&
      isSyncType(at)                         == false &&
      isSingleType(at)                       == false &&
      at->symbol->hasFlag(FLAG_COPY_MUTATES) == false) {
    retval = "chpl__initCopy";
  }

  return retval;
}

static FnSymbol* autoMemoryFunction(AggregateType* at, const char* fnName) {
  VarSymbol* tmp    = newTemp(at);
  CallExpr*  call   = new CallExpr(fnName, tmp);
  FnSymbol*  retval = NULL;

  chpl_gen_main->insertAtHead(new DefExpr(tmp));

  retval = resolveUninsertedCall(at, call);

  resolveFns(retval);

  INT_ASSERT(retval->hasFlag(FLAG_PROMOTION_WRAPPER) == false);

  tmp->defPoint->remove();

  return retval;
}

/************************************* | **************************************
*                                                                             *
* In order to correctly initialize records or tuples in which a component     *
* has FLAG_IGNORE_NOINIT, we need to propagate that flag to the parent types  *
* as well.                                                                    *
*                                                                             *
* While doing so, we also propagate whether or not that type is not POD       *
* (Plain-Old-Data which would mean it does not need auto copies or auto       *
* destroys - bit copies will do).                                             *
*                                                                             *
* After this function is called on an aggregate type, that type will be       *
* marked FLAG_POD and FLAG_NOT_POD, and this function will be called          *
* recursively to also mark any aggregate type fields with FLAG_POD or         *
* FLAG_NOT_POD.                                                               *
*                                                                             *
* After setting either FLAG_POD or FLAG_NOT_POD if necessary, returns true    *
* if FLAG_NOT_POD is set, false otherwise.                                    *
*                                                                             *
* This function should only be called during resolution.                      *
* Call isPOD (or check FLAG_POD/FLAG_NOT_POD) after resolution.               *
*                                                                             *
************************************** | *************************************/

static bool isCompilerGenerated(FnSymbol* fn);

bool propagateNotPOD(Type* t) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(t)) {
    if        (at->symbol->hasFlag(FLAG_POD)     == true) {
      retval = false;

    } else if (at->symbol->hasFlag(FLAG_NOT_POD) == true) {
      retval =  true;

    } else {
      // Some special rules for special things.
      if (isSyncType(at)                        == true ||
          isSingleType(at)                      == true ||
          at->symbol->hasFlag(FLAG_ATOMIC_TYPE) == true) {
        retval = true;
      }

      // Most class types are POD (user classes, _ddata, c_ptr)
      // Also, there is no need to check the fields of a class type
      // since a variable of that type is a pointer to the instance.
      if (isClass(at) == true) {
        // don't enumerate sub-fields or check for autoCopy etc

      } else {
        // If any field in a record/tuple is not POD, the aggregate is not POD.
        for_fields(field, at) {
          retval = retval | propagateNotPOD(field->typeInfo());
        }

        // Make sure we have resolved auto copy/auto destroy.
        // Except not for runtime types, because that causes
        // some sort of fatal resolution error. This is a workaround.
        if (at->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE) == false) {
          resolveAutoCopyEtc(at);
        }

        if (at->symbol->hasFlag(FLAG_IGNORE_NOINIT)      == true  ||
            isCompilerGenerated(autoCopyMap[at])         == false ||
            isCompilerGenerated(autoDestroyMap.get(at))  == false ||
            isCompilerGenerated(at->getDestructor())     == false) {
          retval = true;
        }

        // Since hasUserAssign tries to resolve =, we only
        // check it if we think we have a POD type.
        if (retval == false && hasUserAssign(at) == true) {
          retval = true;
        }
      }

      at->symbol->addFlag((retval == true) ? FLAG_NOT_POD : FLAG_POD);
    }
  }

  return retval;
}

static bool isCompilerGenerated(FnSymbol* fn) {
  return fn != NULL && fn->hasFlag(FLAG_COMPILER_GENERATED) == true;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void resolveRecordInitializers() {
  //
  // resolve PRIM_INITs for records
  //
  forv_Vec(CallExpr, init, inits)
  {
    // Ignore if dead.
    if (!init->parentSymbol)
      continue;

    Type* type = init->get(1)->typeInfo();

    // Don't resolve initializers for runtime types.
    // These have to be resolved after runtime types are replaced by values in
    // insertRuntimeInitTemps().
    if (type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE))
      continue;

    // Extract the value type.
    if (type->symbol->hasFlag(FLAG_REF))
      type = type->getValType();

    // Resolve the AggregateType that has noinit used on it.
    if (init->isPrimitive(PRIM_NO_INIT)) {
      // Replaces noinit with a call to the defaultTypeConstructor,
      // providing the param and type arguments necessary to allocate
      // space for the instantiation of this (potentially) generic type.
      // defaultTypeConstructor calls are already cleaned up at the end of
      // function resolution, so the noinit cleanup would be redundant.
      SET_LINENO(init);
      AggregateType* rec = toAggregateType(type);
      INT_ASSERT(rec);

      CallExpr* res = new CallExpr(rec->defaultTypeConstructor);
      for_formals(formal, rec->defaultTypeConstructor) {
        Vec<Symbol *> keys;
        // Finds each named argument in the type constructor and inserts
        // the substitution provided.
        rec->substitutions.get_keys(keys);
        // I don't think we can guarantee that the substitutions will be
        // in the same order as the arguments for the defaultTypeConstructor.
        // That would make this O(n) instead of potentially O(n*n)
        forv_Vec(Symbol, key, keys) {
          if (!strcmp(formal->name, key->name)) {
            Symbol* formalVal = rec->substitutions.get(key);
            res->insertAtTail(new NamedExpr(formal->name,
                                            new SymExpr(formalVal)));
          }
        }
      }

      init->get(1)->replace(res);

      resolveCall(res);

      toCallExpr(init->parentExpr)->convertToNoop();

      // Now that we've resolved the type constructor and thus resolved the
      // generic type of the variable we were assigning to, the outer move
      // is no longer needed, so remove it and continue to the next init.
      continue;
    }

    // This could be an assert...
    if (type->defaultValue)
      INT_FATAL(init, "PRIM_INIT should have been replaced already");

    SET_LINENO(init);
    if (type->symbol->hasFlag(FLAG_DISTRIBUTION)) {
      // This initialization cannot be replaced by a _defaultOf function
      // earlier in the compiler, there is not enough information to build a
      // default function for it.  When we have the ability to call a
      // constructor from a type alias, it can be moved directly into module
      // code
      Symbol* tmp = newTemp("_distribution_tmp_");
      init->getStmtExpr()->insertBefore(new DefExpr(tmp));
      AggregateType* instanceAt = toAggregateType(type->getField("_instance")->type);

      CallExpr* classCall = new CallExpr(instanceAt->defaultInitializer);
      CallExpr* move = new CallExpr(PRIM_MOVE, tmp, classCall);
      init->getStmtExpr()->insertBefore(move);
      resolveCallAndCallee(classCall);
      resolveCall(move);
      CallExpr* distCall = new CallExpr("chpl__buildDistValue", tmp);
      init->replace(distCall);
      resolveCallAndCallee(distCall);
    } else {
      CallExpr* call = new CallExpr("_defaultOf", type->symbol);
      init->replace(call);
      // At this point in the compiler, we can resolve the _defaultOf function
      // for the type, so do so.
      resolveCallAndCallee(call);
    }
  }
}

//
// Resolve other things we might want later
//
static void resolveOther() {
  //
  // When compiling with --minimal-modules, gPrintModuleInitFn is not
  // defined.
  //
  if (gPrintModuleInitFn) {
    // Resolve the function that will print module init order
    resolveFns(gPrintModuleInitFn);
  }

  std::vector<FnSymbol*> fns = getWellKnownFunctions();
  for_vector(FnSymbol, fn, fns) {
    if (!fn->hasFlag(FLAG_GENERIC)) {
      resolveFormals(fn);
      resolveFns(fn);
    }
  }
}


static Type*
buildRuntimeTypeInfo(FnSymbol* fn) {
  SET_LINENO(fn);
  AggregateType* ct = new AggregateType(AGGREGATE_RECORD);
  TypeSymbol* ts = new TypeSymbol(astr("_RuntimeTypeInfo"), ct);
  for_formals(formal, fn) {
    if (formal->hasFlag(FLAG_INSTANTIATED_PARAM))
      continue;

    VarSymbol* field = new VarSymbol(formal->name, formal->type);
    ct->fields.insertAtTail(new DefExpr(field));

    if (formal->hasFlag(FLAG_TYPE_VARIABLE))
      field->addFlag(FLAG_TYPE_VARIABLE);
  }
  theProgram->block->insertAtTail(new DefExpr(ts));
  ct->symbol->addFlag(FLAG_RUNTIME_TYPE_VALUE);
  makeRefType(ts->type); // make sure the new type has a ref type.
  propagateNotPOD(ts->type); // mark the runtime type as POD/NOT_POD
  return ct;
}


static void insertReturnTemps() {
  //
  // Insert return temps for functions that return values if no
  // variable captures the result. If the value is a sync/single var or a
  // reference to a sync/single var, pass it through the _statementLevelSymbol
  // function to get the semantics of reading a sync/single var. If the value
  // is an iterator pass it through another overload of
  // _statementLevelSymbol to iterate through it for side effects.
  // Note that we do not do this for --minimal-modules compilation
  // because we do not support sync/singles for minimal modules.
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol) {
      if (FnSymbol* fn = call->resolvedFunction()) {
        if (fn->retType != dtVoid) {
          ContextCallExpr* contextCall = toContextCallExpr(call->parentExpr);
          Expr*            contextCallOrCall; // insert before, remove it

          if (contextCall) {
            contextCallOrCall = contextCall;
            // Only consider the designated call; the other
            // call need not be considered by this transformation.
            if (call != getDesignatedCall(contextCall))
              continue;
          } else {
            contextCallOrCall = call;
          }

          Expr* parent = contextCallOrCall->parentExpr;

          if (!isCallExpr(parent) && !isDefExpr(parent)) { // no use
            SET_LINENO(call); // TODO: reset_ast_loc() below?
            VarSymbol* tmp = newTemp("_return_tmp_", fn->retType);
            DefExpr*   def = new DefExpr(tmp);

            if (isUserDefinedRecord(fn->retType) == true)
              tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);

            contextCallOrCall->insertBefore(def);

            if (fMinimalModules == false) {
              if ((fn->retType->getValType() &&
                   (isSyncType(fn->retType->getValType()) ||
                    isSingleType(fn->retType->getValType())))         ||

                  isSyncType(fn->retType)                             ||
                  isSingleType(fn->retType)                           ||
                  fn->isIterator()) {
                CallExpr* sls = new CallExpr("_statementLevelSymbol", tmp);

                contextCallOrCall->insertBefore(sls);
                reset_ast_loc(sls, call);
                resolveCallAndCallee(sls);
              }
            }

            if (isTypeExpr(contextCallOrCall)) {
              tmp->addFlag(FLAG_MAYBE_TYPE);
              tmp->addFlag(FLAG_MAYBE_PARAM);
            }
            def->insertAfter(new CallExpr(PRIM_MOVE,
                                          tmp,
                                          contextCallOrCall->remove()));
          }
        }
      }
    }
  }
}


//
// insert code to initialize a class or record
//
static void
initializeClass(Expr* stmt, Symbol* sym) {
  AggregateType* ct = toAggregateType(sym->type);
  INT_ASSERT(ct);
  for_fields(field, ct) {
    if (!field->hasFlag(FLAG_SUPER_CLASS)) {
      SET_LINENO(field);
      if (field->type->defaultValue) {
        stmt->insertBefore(new CallExpr(PRIM_SET_MEMBER, sym, field, field->type->defaultValue));
      } else if (isRecord(field->type)) {
        VarSymbol* tmp = newTemp("_init_class_tmp_", field->type);
        stmt->insertBefore(new DefExpr(tmp));
        initializeClass(stmt, tmp);
        stmt->insertBefore(new CallExpr(PRIM_SET_MEMBER, sym, field, tmp));
      }
    }
  }
}


static void ensureAndResolveInitStringLiterals() {
  if (!initStringLiterals) {
    INT_ASSERT(fMinimalModules);
    createInitStringLiterals();
  }
  resolveFns(initStringLiterals);
}


static void handleRuntimeTypes()
{
  // insertRuntimeTypeTemps is also called earlier in resolve().  That call
  // can insert variables that need autoCopies and inserting autoCopies can
  // insert things that need runtime type temps.  These need to be fixed up
  // by insertRuntimeTypeTemps before buildRuntimeTypeInitFns is called to
  // update the type -> runtimeType mapping.  Without this, there is an
  // actual/formal type mismatch (with --verify) for the code:
  // record R { var A: [1..1][1..1] real; }
  insertRuntimeTypeTemps();
  buildRuntimeTypeInitFns();
  replaceValuesWithRuntimeTypes();
  replaceReturnedValuesWithRuntimeTypes();
  insertRuntimeInitTemps();
}

//
// A few internal pointers may point to nodes not in tree.
// Zero out such pointers whether or not their targets are live,
// to ensure they are not looked at again.
static void cleanupAfterRemoves() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->instantiatedFrom != NULL)
      fn->addFlag(FLAG_INSTANTIATED_GENERIC);
    fn->instantiatedFrom = NULL;
    fn->instantiationPoint = NULL;
    // How about fn->substitutions, basicBlocks, calledBy ?
  }

  forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
    // Zero the initFn pointer if the function is now dead. Ditto deinitFn.
    if (mod->initFn && !isAlive(mod->initFn))
      mod->initFn = NULL;
    if (mod->deinitFn && !isAlive(mod->deinitFn))
      mod->deinitFn = NULL;
  }

  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    if (arg->instantiatedFrom != NULL)
      arg->addFlag(FLAG_INSTANTIATED_GENERIC);
    arg->instantiatedFrom = NULL;
  }
}


//
// Print a representation of the call graph of the program.
// This needs to be done after function resolution so we can follow calls
// into the called function.  However, it needs to be done before
// removeUnusedFunctions so that we can consider multiple instantiations
// of a function to be the same by tracking them using the function they
// were instantiated from before they are removed as unused.
//
static void printCallGraph(FnSymbol* startPoint, int indent, std::set<FnSymbol*>* alreadyCalled) {

  std::vector<BaseAST*> asts;
  std::set<FnSymbol*> alreadySeenLocally;
  bool freeAlreadyCalledSet = false;
  const bool printLocalMultiples = false;

  if (!startPoint) {
    startPoint = findGenMainFn();
  }

  if (alreadyCalled == NULL) {
    alreadyCalled = new std::set<FnSymbol*>();
    freeAlreadyCalledSet = true;
  }

  collect_asts_postorder(startPoint, asts);
  if (startPoint->hasFlag(FLAG_COMPILER_GENERATED) ||
      startPoint->hasFlag(FLAG_MODULE_INIT)) {
    // Don't print chpl_gen_main and chpl__init_moduleName
    // Set the indent to -2 so recursing down to the next level
    // will set it back to 0.
    indent = -2;
  } else {
    fprintf(stdout, "%*s%s\n", indent, "", startPoint->name);
  }

  for_vector(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (FnSymbol* fn = call->resolvedFunction()) {
        if (fn->getModule()->modTag == MOD_USER &&
            !fn->hasFlag(FLAG_COMPILER_GENERATED) &&
            !fn->hasFlag(FLAG_COMPILER_NESTED_FUNCTION)) {
          FnSymbol* instFn = fn;
          if (fn->instantiatedFrom) {
            instFn = fn->instantiatedFrom;
          }
          if (printLocalMultiples || 0 == alreadySeenLocally.count(instFn)) {
            alreadySeenLocally.insert(instFn);
            if (0 == alreadyCalled->count(fn)) {
              alreadyCalled->insert(fn);
              printCallGraph(fn, indent+2, alreadyCalled);
              alreadyCalled->erase(fn);
            } else {
              fprintf(stdout, "%*s%s (Recursive)\n", indent+2, "", fn->name);
            }
          }
        }
      }
    }
  }
  if (freeAlreadyCalledSet) {
    delete alreadyCalled;
  }
}

//
// Return the compiler generated main function.
//
static FnSymbol* findGenMainFn() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->isResolved()) {
      if (!strcmp("chpl_gen_main", fn->name)) {
        return fn;
      }
    }
  }
  INT_FATAL("couldn't find compiler generated main function");
  return NULL;
}

static bool isVoidOrVoidTupleType(Type* type) {
  if (type == NULL) {
    return false;
  }
  if (type == dtVoid) {
    return true;
  }
  if (type->symbol->hasFlag(FLAG_REF)) {
    if (type->getField("_val", false)) {
      return isVoidOrVoidTupleType(type->getValType());
    } else {
      // The _val field has already been removed because it is
      // void or tuple of void
      return true;
    }
  }
  if (type->symbol->hasFlag(FLAG_STAR_TUPLE)) {
    Symbol* field = type->getField("x1", false);
    if (field == NULL || field->type == dtVoid) {
      return true;
    }
  }
  return false;
}

static void cleanupVoidVarsAndFields() {
  // Remove most uses of void variables and fields
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->inTree()) {
      if (call->isPrimitive(PRIM_MOVE)) {
        if (isVoidOrVoidTupleType(call->get(2)->typeInfo()) ||
            call->get(2)->typeInfo() == dtVoid->refType) {
          INT_ASSERT(call->get(1)->typeInfo() == call->get(2)->typeInfo());
          // Remove moves where the rhs has type void. If the rhs is a
          // call to something other than a few primitives, still make
          // that call, just don't move the result into anything.
          if (CallExpr* rhs = toCallExpr(call->get(2))) {
            if (rhs->isPrimitive(PRIM_DEREF) ||
                rhs->isPrimitive(PRIM_GET_MEMBER) ||
                rhs->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
              call->remove();
            } else {
              Expr* rmRhs = rhs->remove();
              call->insertBefore(rmRhs);
              call->remove();
            }
          } else if (isSymExpr(call->get(2))) {
            call->remove();
          }
        }
      } else if (call->isPrimitive(PRIM_SET_MEMBER)) {
        if (isVoidOrVoidTupleType(call->get(3)->typeInfo())) {
          INT_ASSERT(call->get(2)->typeInfo() == call->get(3)->typeInfo());
          // Remove set_member(a, void, void) calls
          if (CallExpr* rhs = toCallExpr(call->get(2))) {
            Expr* rmRhs = rhs->remove();
            call->insertBefore(rmRhs);
            call->remove();
          } else if (isSymExpr(call->get(2))) {
            call->remove();
          }
        }
      } else if (call->isPrimitive(PRIM_RETURN)) {
        if (isVoidOrVoidTupleType(call->get(1)->typeInfo()) ||
            call->get(1)->typeInfo() == dtVoid->refType) {
          // Change functions that return void to use the global
          // void value instead of a local void.
          if (SymExpr* ret = toSymExpr(call->get(1))) {
            if (ret->symbol() != gVoid) {
              SET_LINENO(call);
              call->replace(new CallExpr(PRIM_RETURN, gVoid));
            }
          }
        }
      } else if (call->isPrimitive(PRIM_YIELD)) {
        if (isVoidOrVoidTupleType(call->get(1)->typeInfo()) ||
            call->get(1)->typeInfo() == dtVoid->refType) {
          // Change iterators that yield void to use the global
          // void value instead of a local void.
          if (SymExpr* ret = toSymExpr(call->get(1))) {
            if (ret->symbol() != gVoid) {
              SET_LINENO(call);
              call->replace(new CallExpr(PRIM_YIELD, gVoid));
            }
          }
        }
      } else if (call->isPrimitive(PRIM_CALL_DESTRUCTOR)) {
        // Remove calls to destructors for homogeneous tuples of void
        if (isVoidOrVoidTupleType(call->get(1)->typeInfo())) {
          call->remove();
        }
      }
      if (call->isResolved()) {
        // Remove actual arguments that are void from function calls
        for_actuals(actual, call) {
          if (isVoidOrVoidTupleType(actual->typeInfo())) {
            actual->remove();
          }
        }
      }
    }
  }

  // Remove void formal arguments from functions.
  // Change functions that return ref(void) to just return void.
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->defPoint->inTree()) {
      for_formals(formal, fn) {
        if (isVoidOrVoidTupleType(formal->type)) {
          if (formal == fn->_this) {
            fn->_this = NULL;
          }
          formal->defPoint->remove();
        }
      }
      if (fn->retType == dtVoid->refType ||
          isVoidOrVoidTupleType(fn->retType)) {
        fn->retType = dtVoid;
      }
    }
    if (fn->_this) {
      if (isVoidOrVoidTupleType(fn->_this->type)) {
        fn->_this = NULL;
      }
    }
  }

  // Set for loop index variables that are void to the global void value
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (ForLoop* loop = toForLoop(block)) {
      if (loop->indexGet() && loop->indexGet()->typeInfo() == dtVoid) {
        loop->indexGet()->setSymbol(gVoid);
      }
    }
  }

  // Now that uses of void have been cleaned up, remove the
  // DefExprs for void variables.
  forv_Vec(DefExpr, def, gDefExprs) {
    if (def->inTree()) {
      if (isVoidOrVoidTupleType(def->sym->type) ||
          def->sym->type == dtVoid->refType) {
        if (VarSymbol* var = toVarSymbol(def->sym)) {
          // Avoid removing the "_val" field from refs
          if (!def->parentSymbol->hasFlag(FLAG_REF)) {
            if (var != gVoid) {
              def->remove();
            }
          }
        }
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* pruneResolvedTree -- prunes and cleans the AST after                        *
* of the function calls and types have been resolved                          *
*                                                                             *
************************************** | *************************************/

static void removeAggTypeFieldInfo();

static void pruneResolvedTree() {
  removeUnusedFunctions();

  if (fRemoveUnreachableBlocks) {
    deadBlockElimination();
  }

  removeRandomPrimitives();

  replaceTypeArgsWithFormalTypeTemps();

  removeParamArgs();

  removeAggTypeFieldInfo();

  removeUnusedModuleVariables();

  removeUnusedTypes();

  removeActualNames();

  removeFormalTypeAndInitBlocks();

  removeTypeBlocks();

  removeInitFields();

  removeWhereClauses();

  removeMootFields();

  expandInitFieldPrims();

  cleanupAfterRemoves();

  cleanupVoidVarsAndFields();
}

static void clearDefaultInitFns(FnSymbol* unusedFn) {
  AggregateType* at = toAggregateType(unusedFn->retType);
  // Before removing an unused function, check if it is a defaultInitializer.
  // If unusedFn is a defaultInitializer, its retType's defaultInitializer
  // field will be unusedFn. Set the defaultInitializer field to NULL so the
  // removed function doesn't leave behind a garbage pointer.
  if (at) {
    if (at->defaultInitializer == unusedFn) {
      at->defaultInitializer = NULL;
    }
    // Also remove unused fns from iterator infos.
    // Ditto for iterator fn in iterator info.
    if (at->iteratorInfo) {
      IteratorInfo* ii = at->iteratorInfo;
      INT_ASSERT(at->symbol->hasEitherFlag(FLAG_ITERATOR_RECORD,
                                           FLAG_ITERATOR_CLASS));
      if (ii) {
        if (ii->iterator == unusedFn)
          ii->iterator = NULL;
        if (ii->getIterator == unusedFn)
          ii->getIterator = NULL;
      }
    }
  }
}

static void removeCopyFns(Type* t) {
  // If they exist, remove the autoCopy and autoDestroy functions
  // for type t, which is about to be removed itself.
  if (FnSymbol* autoDestroy = autoDestroyMap.get(t)) {
    autoDestroyMap.put(t, NULL);
    autoDestroy->defPoint->remove();
  }

  std::map<Type*,FnSymbol*>::iterator it = autoCopyMap.find(t);
  if (it != autoCopyMap.end()) {
    FnSymbol* autoCopy = it->second;
    autoCopyMap.erase(it);
    autoCopy->defPoint->remove();
  }
}

static void removeUnusedFunctions() {
  std::set<FnSymbol*> concreteWellKnownFunctionsSet;

  // Generic well-known functions will no longer be
  // well-known (since after resolution they can't be
  // instantiated, and the generic fn is removed).
  // So remove generic well-known functions from the list.
  clearGenericWellKnownFunctions();

  // Concrete well-known functions need to be preserved,
  // so track them in a set.
  std::vector<FnSymbol*> fns = getWellKnownFunctions();
  for_vector(FnSymbol, fn, fns) {
    // These should have just been removed
    INT_ASSERT(!fn->hasFlag(FLAG_GENERIC));
    concreteWellKnownFunctionsSet.insert(fn);
  }

  // Remove unused functions
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    // Do not remove concrete well-known functions
    if (concreteWellKnownFunctionsSet.count(fn) > 0) continue;

    if (fn->defPoint && fn->defPoint->parentSymbol) {
      if (fn->defPoint->parentSymbol == stringLiteralModule) continue;

      if (! fn->isResolved() || fn->retTag == RET_PARAM) {
        // Look for types defined within this unused function. If any
        // types are defined, remove their autoCopy and autoDestroy
        // functions. Also remove their reference types, and the reference
        // type's defaultTypeConstructor, autoCopy and autoDestroy functions.
        std::vector<DefExpr*> defExprs;
        collectDefExprs(fn, defExprs);
        forv_Vec(DefExpr, def, defExprs) {
          if (TypeSymbol* typeSym = toTypeSymbol(def->sym)) {
            // Remove the autoCopy and autoDestroy functions for this type
            removeCopyFns(typeSym->type);

            AggregateType* refType = toAggregateType(typeSym->type->refType);
            if (refType) {
              // If the default type constructor for this ref type is in
              // the tree, it should be removed.
              if (refType->defaultTypeConstructor->defPoint->parentSymbol) {
                refType->defaultTypeConstructor->defPoint->remove();
              }

              // Remove autoCopy and autoDestroy functions for this refType
              removeCopyFns(refType);

              // Now remove the refType
              refType->symbol->defPoint->remove();
            }
          }
        }
        clearDefaultInitFns(fn);
        fn->defPoint->remove();
      }
    }
  }
}

static bool
isUnusedClass(AggregateType *ct) {
  // Special case for global types.
  if (ct->symbol->hasFlag(FLAG_GLOBAL_TYPE_SYMBOL))
    return false;

  // Runtime types are assumed to be always used.
  if (ct->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE))
    return false;

  // Uses of iterator records get inserted in lowerIterators
  if (ct->symbol->hasFlag(FLAG_ITERATOR_RECORD))
    return false;

  // FALSE if iterator class's getIterator is used
  // (this case may not be necessary)
  if (ct->symbol->hasFlag(FLAG_ITERATOR_CLASS) &&
      ct->iteratorInfo->getIterator->isResolved())
    return false;

  // FALSE if initializers are used
  if (ct->defaultInitializer && ct->defaultInitializer->isResolved())
    return false;

  // FALSE if the type constructor is used.
  if (ct->defaultTypeConstructor && ct->defaultTypeConstructor->isResolved())
    return false;

  // FALSE if the type uses an initializer and that initializer was
  // resolved
  if (ct->initializerStyle != DEFINES_CONSTRUCTOR &&
      ct->initializerResolved)
    return false;

  bool allChildrenUnused = true;
  forv_Vec(Type, child, ct->dispatchChildren) {
    AggregateType* childClass = toAggregateType(child);
    INT_ASSERT(childClass);
    if (!isUnusedClass(childClass)) {
      allChildrenUnused = false;
      break;
    }
  }
  return allChildrenUnused;
}

// Remove unused types
static void removeUnusedTypes() {

  // Remove unused aggregate types.
  forv_Vec(TypeSymbol, type, gTypeSymbols) {
    if (type->defPoint && type->defPoint->parentSymbol)
    {
      // Skip ref and runtime value types:
      //  ref types are handled below;
      //  runtime value types are assumed to be always used.
      if (type->hasFlag(FLAG_REF))
        continue;
      if (type->hasFlag(FLAG_RUNTIME_TYPE_VALUE))
        continue;

      if (AggregateType* ct = toAggregateType(type->type))
        if (isUnusedClass(ct))
          ct->symbol->defPoint->remove();
    }
  }

  // Remove unused ref types.
  forv_Vec(TypeSymbol, type, gTypeSymbols) {
    if (type->defPoint && type->defPoint->parentSymbol) {
      if (type->hasFlag(FLAG_REF)) {
        // Get the value type of the ref type.
        if (AggregateType* ct = toAggregateType(type->getValType())) {
          if (isUnusedClass(ct)) {
            // If the value type is unused, its ref type can also be removed.
            type->defPoint->remove();
          }
        }
        // If the default type constructor for this ref type is in the tree, it
        // can be removed.
        AggregateType* at = toAggregateType(type->type);
        INT_ASSERT(at);
        if (at->defaultTypeConstructor->defPoint->parentSymbol)
          at->defaultTypeConstructor->defPoint->remove();
      }
    }
  }
}

// Remove module level variables if they are not defined or used
// With the exception of variables that are defined in the rootModule
static void removeUnusedModuleVariables() {
  forv_Vec(DefExpr, def, gDefExprs) {
    if (VarSymbol* var = toVarSymbol(def->sym)) {
      if (ModuleSymbol* module = toModuleSymbol(def->parentSymbol)) {
        if (var->isDefined() == false && var->isUsed() == false) {
          if (module != rootModule) {
            def->remove();
          }
        }
      }
    }
  }
}


static void removeRandomPrimitive(CallExpr* call)
{
  if (! call->primitive)
    // TODO: This is weird.
    // Calls which trigger this case appear as the init clause of a type
    // variable.
    // The parent module or function may be resolved, but apparently the type
    // variable is resolved only if it is used.
    // Generally speaking, we resolve a declaration only if it is used.
    // But right now, we only apply this test to functions.
    // The test should be extended to variable declarations as well.  That is,
    // variables need only be resolved if they are actually used.
    return;

  // A primitive.
  switch (call->primitive->tag)
  {
    default: /* do nothing */ break;

    case PRIM_NOOP:
      call->remove();
      break;

    case PRIM_TYPEOF:
    {
      // Remove move(x, PRIM_TYPEOF(y)) calls -- useless after this
      CallExpr* parentCall = toCallExpr(call->parentExpr);
      if (parentCall && parentCall->isPrimitive(PRIM_MOVE) &&
          parentCall->get(2) == call) {
        parentCall->remove();
      } else {
        // Replace PRIM_TYPEOF with argument
        call->replace(call->get(1)->remove());
      }
    }
    break;

    case PRIM_CAST:
      // Remove trivial casts.
      if (call->get(1)->typeInfo() == call->get(2)->typeInfo())
        call->replace(call->get(2)->remove());
       break;

    case PRIM_SET_MEMBER:
    case PRIM_GET_MEMBER:
    case PRIM_GET_MEMBER_VALUE:
    {
      // Remove member accesses of types
      // Replace string literals with field symbols in member primitives
      Type* baseType = call->get(1)->typeInfo();
      if (baseType->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE))
        break;

      if (!call->parentSymbol->hasFlag(FLAG_REF) &&
          baseType->symbol->hasFlag(FLAG_REF))
        baseType = baseType->getValType();

      const char* memberName = get_string(call->get(2));

      Symbol* sym = baseType->getField(memberName);
      if (sym->hasFlag(FLAG_TYPE_VARIABLE) ||
          !strcmp(sym->name, "_promotionType") ||
          sym->isParameter())
        call->getStmtExpr()->remove();
      else {
        SET_LINENO(call->get(2));
        call->get(2)->replace(new SymExpr(sym));
      }
    }
    break;

    // Maybe this can be pushed into the following case, where a PRIM_MOVE gets
    // removed if its rhs is a type symbol.  That is, resolution of a
    // PRIM_TYPE_INIT replaces the primitive with symexpr that contains a type symbol.
    case PRIM_TYPE_INIT:
    {
      // A "type init" call that is in the tree should always have a callExpr
      // parent, as guaranteed by CallExpr::verify().
      CallExpr* parent = toCallExpr(call->parentExpr);
      // We expect all PRIM_TYPE_INIT primitives to have a PRIM_MOVE
      // parent, following the insertion of call temps.
      if (parent->isPrimitive(PRIM_MOVE))
        parent->remove();
      else
        INT_FATAL(parent, "expected parent of PRIM_TYPE_EXPR to be a PRIM_MOVE");
    }
    break;

    case PRIM_MOVE:
    {
      // Remove types to enable --baseline
      SymExpr* se = toSymExpr(call->get(2));
      if (se && se->symbol()) {
        Symbol* sym = se->symbol();
        if (isTypeSymbol(sym) || sym->hasFlag(FLAG_TYPE_VARIABLE))
          call->remove();
      }
    }
    break;

    case PRIM_WARNING:
    case PRIM_ERROR:
    {
      // Warnings have now been issued, no need to keep the function around.
      // Remove calls to compilerWarning and let dead code elimination handle
      // the rest.
      call->remove();
    }
    break;

  }
}


// Remove the method token, parameter and type arguments from
// function signatures and corresponding calls.
static void removeParamArgs()
{
  compute_call_sites();

  forv_Vec(FnSymbol, fn, gFnSymbols)
  {
    if (! fn->isResolved())
      // Don't bother with unresolved functions.
      // They will be removed from the tree.
      continue;

    for_formals(formal, fn)
    {
      if (formal->hasFlag(FLAG_INSTANTIATED_PARAM) ||
          formal->type == dtMethodToken)
      {
        // Remove the argument from the call site.
        forv_Vec(CallExpr, call, *fn->calledBy)
        {
          // Don't bother with calls that are not in the tree.
          if (! call->parentSymbol)
            continue;

          // Performance note: AList::get(int) also performs a linear search.
          for_formals_actuals(cf, ca, call)
          {
            if (cf == formal)
            {
              ca->remove();
              break;
            }
          }
        }
        formal->defPoint->remove();
      }
    }
  }
}

static void removeAggTypeFieldInfo() {
  forv_Vec(AggregateType, at, gAggregateTypes) {
    if (at->symbol->defPoint && at->symbol->defPoint->parentSymbol) {
      // Still in the tree
      if (at->initializerStyle != DEFINES_CONSTRUCTOR) {
        // Defined an initializer (so we left its init
        // and exprType information in the tree)
        for_fields(field, at) {
          if (field->defPoint->exprType) {
            field->defPoint->exprType->remove();
          }

          if (field->defPoint->init) {
            field->defPoint->init->remove();
          }
        }
      }
    }
  }
}

static void removeRandomPrimitives()
{
  forv_Vec(CallExpr, call, gCallExprs)
  {
    // Don't bother with calls that are not in the tree.
    if (! call->parentSymbol)
      continue;

    // Ignore calls to actual functions.
    if (call->isResolved())
      continue;

    // Only primitives remain.
    removeRandomPrimitive(call);
  }
}

static void removeActualNames()
{
  forv_Vec(NamedExpr, named, gNamedExprs)
  {
    if (! named->parentSymbol)
      continue;
    // Remove names of named actuals
    Expr* actual = named->actual;
    actual->remove();
    named->replace(actual);
  }
}

static void removeTypeBlocks()
{
  forv_Vec(BlockStmt, block, gBlockStmts)
  {
    if (! block->parentSymbol)
      continue;

    // Remove type blocks--code that exists only to determine types
    if (block->blockTag & BLOCK_TYPE_ONLY)
    {
      block->remove();
    }
  }
}

//
// buildRuntimeTypeInitFns: Build a 'chpl__convertRuntimeTypeToValue'
// (value) function for all functions tagged as runtime type
// initialization functions.  Also, build a function to return the
// runtime type for all runtime type initialization functions.
//
// Functions flagged with the "runtime type init fn" pragma
// (FLAG_RUNTIME_TYPE_INIT_FN during compilation) are designed to
// specify to the compiler how to create a new value of a given type
// from the arguments to the function.  These arguments effectively
// supply whatever static and/or runtime information is required to
// build such a value (and therefore effectively represent the
// "type").  Any non-static arguments are bundled into a runtime type
// (record) by the compiler and passed around to represent the type at
// execution time.
//
// The actual type specified is fully-resolved during function resolution.  So
// the "runtime type" mechanism is a way to create a parameterized type, but up
// to a point handle it uniformly in the compiler.
// Perhaps a more complete implementation of generic types with inheritance
// would get rid of the need for this specialized machinery.
//
// In practice, we currently use these to create
// runtime types for domains and arrays (via procedures named
// 'chpl__buildDomainRuntimeType' and 'chpl__buildArrayRuntimeType',
// respectively).
//
// For each such flagged function:
//
//   - Clone the function, naming it 'chpl__convertRuntimeTypeToValue'
//     and change it to a value function
//
//   - Replace the body of the original function with a new function
//     that returns the dynamic runtime type info
//
// Subsequently, the functions as written in the modules are now
// called 'chpl__convertRuntimeTypeToValue' and used to initialize
// variables with runtime types later in insertRuntimeInitTemps().
//
// Notice also that the original functions had been marked as type
// functions during parsing even though they were not written as such
// (see addPragmaFlags() in build.cpp for more info).  Now they are
// actually type functions.
//
static void buildRuntimeTypeInitFns() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->defPoint && fn->defPoint->parentSymbol) {
      // Look only at functions flagged as "runtime type init fn".
      if (fn->hasFlag(FLAG_RUNTIME_TYPE_INIT_FN)) {

        // Look only at resolved instances.
        if (! fn->isResolved())
          continue;

        INT_ASSERT(fn->retType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE));
        SET_LINENO(fn);

        // Build a new runtime type for this function
        Type* runtimeType = buildRuntimeTypeInfo(fn);
        runtimeTypeMap.put(fn->retType, runtimeType);

        // Build chpl__convertRuntimeTypeToValue() instance.
        buildRuntimeTypeInitFn(fn, runtimeType);
      }
    }
  }
}

// Build a function to return the runtime type by modifying
// the original function.
static void buildRuntimeTypeInitFn(FnSymbol* fn, Type* runtimeType)
{
  // Clone the original function and call the clone chpl__convertRuntimeTypeToValue.
  FnSymbol* runtimeTypeToValueFn = fn->copy();
  INT_ASSERT(runtimeTypeToValueFn->hasFlag(FLAG_RESOLVED));
  runtimeTypeToValueFn->name = astr("chpl__convertRuntimeTypeToValue");
  runtimeTypeToValueFn->cname = runtimeTypeToValueFn->name;

  // Remove this flag from the clone.
  runtimeTypeToValueFn->removeFlag(FLAG_RUNTIME_TYPE_INIT_FN);

  // Make the clone a value function.
  runtimeTypeToValueFn->getReturnSymbol()->removeFlag(FLAG_TYPE_VARIABLE);
  runtimeTypeToValueFn->retTag = RET_VALUE;
  fn->defPoint->insertBefore(new DefExpr(runtimeTypeToValueFn));

  // Remove static arguments from the RTTV function.
  for_formals(formal, runtimeTypeToValueFn)
  {
    if (formal->hasFlag(FLAG_INSTANTIATED_PARAM))
      formal->defPoint->remove();

    if (formal->hasFlag(FLAG_TYPE_VARIABLE))
    {
      Symbol* field = runtimeType->getField(formal->name);
      if (! field->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE))
        formal->defPoint->remove();
    }
  }

  // Insert the clone (convertRuntimeTypeToValue) into the runtimeTypeToValueMap.
  runtimeTypeToValueMap.put(runtimeType, runtimeTypeToValueFn);

  // Change the return type of the original function.
  fn->retType = runtimeType;
  fn->getReturnSymbol()->type = runtimeType;

  // Build a new body for the original function.
  BlockStmt* block = new BlockStmt();
  VarSymbol* var = newTemp("_return_tmp_", fn->retType);
  block->insertAtTail(new DefExpr(var));

  // Bundle all non-static arguments into the runtime type record.
  // Remove static arguments from this specialized buildRuntimeType function.
  for_formals(formal, fn)
  {
    if (formal->hasFlag(FLAG_INSTANTIATED_PARAM))
      continue;

    Symbol* field = runtimeType->getField(formal->name);

    if (formal->hasFlag(FLAG_TYPE_VARIABLE) &&
        ! field->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE))
      continue;

    block->insertAtTail(new CallExpr(PRIM_SET_MEMBER, var, field, formal));
  }

  block->insertAtTail(new CallExpr(PRIM_RETURN, var));

  // Replace the body of the original chpl__buildRuntime...Type() function.
  fn->body->replace(block);
}

static void removeFormalTypeAndInitBlocks()
{
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->defPoint && fn->defPoint->parentSymbol) {
      for_formals(formal, fn) {
        // Remove formal default values
        if (formal->defaultExpr)
          formal->defaultExpr->remove();
        // Remove formal type expressions
        if (formal->typeExpr)
          formal->typeExpr->remove();
      }
    }
  }
}

static void replaceTypeArgsWithFormalTypeTemps()
{
  compute_call_sites();

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (! fn->isResolved())
      // Don't bother with unresolved functions.
      // They will be removed from the tree.
      continue;

    // Skip this function if it is not in the tree.
    if (! fn->defPoint)
      continue;
    if (! fn->defPoint->parentSymbol)
      continue;

    // We do not remove type args from extern functions so that e.g.:
    //  extern proc sizeof(type t);
    //  sizeof(int)
    // will function correctly.
    // However, in such a case, we'd like to pass the type symbol
    // to the call site rather than a type variable call_tmp
    if (fn->hasFlag(FLAG_EXTERN)) {
      // Replace the corresponding actual with a SymExpr TypeSymbol
      // for all the call sites.
      forv_Vec(CallExpr, call, *fn->calledBy)
      {
        for_formals_actuals(formal, actual, call)
        {
          if (! formal->hasFlag(FLAG_TYPE_VARIABLE))
            continue;

          if (SymExpr* se = toSymExpr(actual)) {
            if (isTypeSymbol(se->symbol()))
              continue;
            if (se->symbol()->hasFlag(FLAG_EXTERN) &&
                se->symbol()->hasFlag(FLAG_TYPE_VARIABLE))
              continue;
          }

          SET_LINENO(actual);
          TypeSymbol* ts = formal->type->symbol;
          actual->replace(new SymExpr(ts));
        }
      }
      continue;
    }

    for_formals(formal, fn)
    {
      // We are only interested in type formals
      if (! formal->hasFlag(FLAG_TYPE_VARIABLE))
        continue;

      // Replace the formal with a _formal_type_tmp_.
      SET_LINENO(formal);
      VarSymbol* tmp = newTemp("_formal_type_tmp_", formal->type);
      fn->insertAtHead(new DefExpr(tmp));
      subSymbol(fn, formal, tmp);

      // Remove the corresponding actual from all call sites.
      forv_Vec(CallExpr, call, *fn->calledBy)
      {
        for_formals_actuals(cf, ca, call)
        {
          if (cf == formal)
          {
            ca->remove();
            break;
          }
        }
      }
      formal->defPoint->remove();
      //
      // If we're removing the formal representing 'this' (if it's a
      // type, say), we need to nullify the 'this' pointer in the
      // function as well to avoid assumptions that it's legal later.
      //
      if (formal == fn->_this) {
        fn->_this = NULL;
      }
    }
  }
}

static void replaceValuesWithRuntimeTypes()
{
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->defPoint && fn->defPoint->parentSymbol) {
      for_formals(formal, fn) {
        if (formal->hasFlag(FLAG_TYPE_VARIABLE) &&
            formal->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
          if (FnSymbol* fn = valueToRuntimeTypeMap.get(formal->type)) {
            Type* rt = (fn->retType->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) ?
                        fn->retType : runtimeTypeMap.get(fn->retType);
            INT_ASSERT(rt);
            formal->type =  rt;
            formal->removeFlag(FLAG_TYPE_VARIABLE);
          }
        }
      }
    }
  }
}

static void removeWhereClauses()
{
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->defPoint && fn->defPoint->parentSymbol) {
      if (fn->where)
        fn->where->remove();
    }
  }
}

static void replaceReturnedValuesWithRuntimeTypes()
{
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->defPoint && fn->defPoint->parentSymbol) {
      if (fn->retTag == RET_TYPE) {
        VarSymbol* ret = toVarSymbol(fn->getReturnSymbol());
        if (ret && ret->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
          if (FnSymbol* rtfn = valueToRuntimeTypeMap.get(ret->type)) {
            Type* rt = (rtfn->retType->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) ?
                        rtfn->retType : runtimeTypeMap.get(rtfn->retType);
            INT_ASSERT(rt);
            ret->type = rt;
            fn->retType = ret->type;
            fn->retTag = RET_VALUE;
          }
        }
      }
    }
  }
}


static void replaceInitPrims(std::vector<BaseAST*>& asts) {
  for_vector(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      // We are only interested in INIT primitives.
      if (call->isPrimitive(PRIM_INIT)) {
        FnSymbol* parent = toFnSymbol(call->parentSymbol);

        // Call must be in the tree and lie in a resolved function.
        if (! parent || ! parent->isResolved()) {
          continue;
        }

        SymExpr* se = toSymExpr(call->get(1));
        Type*    rt = se->symbol()->type;

        if (rt->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) {
          // ('init' foo), where typeof(foo) has flag "runtime type value"
          //
          // ==>
          //
          // (var _runtime_type_tmp_1)
          // ('move' _runtime_type_tmp_1 ('.v' foo "field1"))
          // (var _runtime_type_tmp_2)
          // ('move' _runtime_type_tmp_2 ('.v' foo "field2"))
          // (chpl__convertRuntimeTypeToValue _runtime_type_tmp_1 _rtt_2 ... )
          SET_LINENO(call);
          FnSymbol* runtimeTypeToValueFn = runtimeTypeToValueMap.get(rt);
          INT_ASSERT(runtimeTypeToValueFn);
          CallExpr* runtimeTypeToValueCall = new CallExpr(runtimeTypeToValueFn);
          for_formals(formal, runtimeTypeToValueFn) {
            Symbol* field = rt->getField(formal->name);
            INT_ASSERT(field);
            VarSymbol* tmp = newTemp("_runtime_type_tmp_", field->type);
            call->getStmtExpr()->insertBefore(new DefExpr(tmp));
            call->getStmtExpr()->insertBefore(
                new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE,
                                                          se->symbol(), field)));
            if (formal->hasFlag(FLAG_TYPE_VARIABLE))
              tmp->addFlag(FLAG_TYPE_VARIABLE);
            runtimeTypeToValueCall->insertAtTail(tmp);
          }

          VarSymbol* tmp = newTemp("_runtime_type_tmp_", runtimeTypeToValueFn->retType);
          call->getStmtExpr()->insertBefore(new DefExpr(tmp));
          call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, runtimeTypeToValueCall));
          call->replace(new SymExpr(tmp));

        } else if (rt->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
          //
          // This is probably related to a comment that used to handle
          // this case elsewhere:
          //
          // special handling of tuple constructor to avoid
          // initialization of array based on an array type symbol
          // rather than a runtime array type
          //
          // this code added during the introduction of the new
          // keyword; it should be removed when possible
          //
          call->getStmtExpr()->remove();

        } else {
          Expr* expr = resolvePrimInit(call);

          if (! expr) {
            // This PRIM_INIT could not be resolved.

            // But that's OK if it's an extern type.
            // (We don't expect extern types to have initializers.)
            // Also, we don't generate initializers for iterator records.
            // Maybe we can avoid adding PRIM_INIT for these cases in the first
            // place....
            if (rt->symbol->hasFlag(FLAG_EXTERN) ||
                rt->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
              INT_ASSERT(toCallExpr(call->parentExpr)->isPrimitive(PRIM_MOVE));

              toCallExpr(call->parentExpr)->convertToNoop();

              continue;
            }

            INT_FATAL(call, "PRIM_INIT should have already been handled");
          }
        }
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Handles PRIM_INIT or PRIM_NO_INIT                                           *
*                                                                             *
* Returns NULL if no substitution was made.                                   *
* Otherwise remove the primitive and return the new expression.               *
*                                                                             *
* ('init'    foo) --> A default value or the result of an initializer call.   *
* ('no_init' foo) --> Ditto, only in some cases a simpler default value.      *
*                                                                             *
************************************** | *************************************/

static bool primInitIsIteratorRecord(Type* type);

static bool primInitIsUnacceptableGeneric(CallExpr* call, Type* type);
static void primInitHaltForUnacceptableGeneric(CallExpr* call, Type* type);

Expr* resolvePrimInit(CallExpr* call) {
  SymExpr* se     = toSymExpr(call->get(1));
  Expr*    retval = NULL;

  if (se == NULL) {
    INT_FATAL(call, "Actual 1 is not a sym expr");

  } else if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE) == false) {
    USR_FATAL(call, "invalid type specification");

  } else {
    Type*          type = resolveTypeAlias(se);
    AggregateType* at   = toAggregateType(type);

    // These are handled later
    if (type->symbol->hasFlag(FLAG_EXTERN) == true) {
      INT_ASSERT(toCallExpr(call->parentExpr)->isPrimitive(PRIM_MOVE));

    // These are handled in replaceInitPrims().
    } else if (type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE) == true) {

    // Initializers for IteratorRecords cannot be used as constructors
    } else if (primInitIsIteratorRecord(type)               == true) {

    // Generate a more specific USR_FATAL if resolution would fail
    } else if (primInitIsUnacceptableGeneric(call, type)    == true) {
      primInitHaltForUnacceptableGeneric(call, type);

    // NonGeneric records with initializers do not support _defaultOf
    } else if (at                                           != NULL &&
               at->instantiatedFrom                         == NULL &&
               isNonGenericRecordWithInitializers(at)       == true) {
      // Parent PRIM_MOVE will be updated to init() later in resolution

    } else {
      SET_LINENO(call);

      CallExpr* defOfCall = new CallExpr("_defaultOf", type->symbol);

      call->replace(defOfCall);

      resolveCallAndCallee(defOfCall);

      retval = foldTryCond(postFold(defOfCall));
    }
  }

  return retval;
}


static bool primInitIsIteratorRecord(Type* type) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(type)) {
    if (at->defaultInitializer                      != NULL &&
        type->symbol->hasFlag(FLAG_ITERATOR_RECORD) == true) {
      retval = true;
    }
  }

  return retval;
}

// Return true if this type is generic *and* resolution will fail
static bool primInitIsUnacceptableGeneric(CallExpr* call, Type* type) {
  bool retval = type->symbol->hasFlag(FLAG_GENERIC);

  // If it is generic then try to resolve the default type constructor
  if (retval == true) {
    if (AggregateType* at = toAggregateType(type)) {
      if (FnSymbol* typeCons = at->defaultTypeConstructor) {
        SET_LINENO(call);

        // Swap in a call to the default type constructor and try to resolve it
        CallExpr* typeConsCall = new CallExpr(typeCons->name);

        call->replace(typeConsCall);

        retval = (tryResolveCall(typeConsCall) == NULL) ? true : false;

        // Put things back the way they were.
        typeConsCall->replace(call);
      }
    }
  }

  return retval;
}

// Generate a useful USR_FATAL for an unacceptable Generic
static void primInitHaltForUnacceptableGeneric(CallExpr* call, Type* type) {
  const char* label = "abstract";

  if (AggregateType* at = toAggregateType(type)) {
    if (at->defaultTypeConstructor != NULL) {
      label = "not-fully-instantiated";
    }
  }

  USR_FATAL(call,
            "Variables can't be declared using %s generic types like '%s'",
            label,
            type->symbol->name);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void insertRuntimeInitTemps() {
  std::vector<BaseAST*> asts;
  collect_asts_postorder(rootModule, asts);

  // Collect asts which are definitions of VarSymbols that are type variables
  // and are flagged as runtime types.
  for_vector(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (isVarSymbol(def->sym) &&
          def->sym->hasFlag(FLAG_TYPE_VARIABLE) &&
          def->sym->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {

        // Collapse these through the runtimeTypeMap ...
        Type* rt = runtimeTypeMap.get(def->sym->type);
        INT_ASSERT(rt);
        def->sym->type = rt;

        // ... and remove the type variable flag
        // (Make these declarations look like normal vars.)
        def->sym->removeFlag(FLAG_TYPE_VARIABLE);
      }
    }
  }

  replaceInitPrims(asts);

  for_vector(BaseAST, ast1, asts) {
    if (SymExpr* se = toSymExpr(ast1)) {

      // remove dead type expressions
      if (se->getStmtExpr() == se)
        if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE))
          se->remove();

    }
  }
}

// Remove typedef definitions
static void removeInitFields()
{
  forv_Vec(DefExpr, def, gDefExprs)
  {
    if (! def->inTree()) continue;
    if (! def->init) continue;
    if (! (def->sym->hasFlag(FLAG_TYPE_VARIABLE) || def->sym->type->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE))) continue;
    def->init->remove();
    def->init = NULL;
  }
}

static void removeMootFields() {
  // Remove type fields, parameter fields, and _promotionType field
  forv_Vec(TypeSymbol, type, gTypeSymbols) {
    if (type->defPoint && type->defPoint->parentSymbol) {
      if (AggregateType* ct = toAggregateType(type->type)) {
        for_fields(field, ct) {
          if (field->hasFlag(FLAG_TYPE_VARIABLE) ||
              field->isParameter() ||
              !strcmp(field->name, "_promotionType"))
            field->defPoint->remove();
        }
      }
    }
  }
}

static void expandInitFieldPrims()
{
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_INIT_FIELDS))
    {
      initializeClass(call, toSymExpr(call->get(1))->symbol());
      call->remove();
    }
  }
}


static void fixTypeNames(AggregateType* at) {
  const char defaultDomainName[] = "DefaultRectangularDom";

  if (at->symbol->hasFlag(FLAG_BASE_ARRAY) == false &&
      isArrayClass(at)                     ==  true) {
    const char* domainType = at->getField("dom")->type->symbol->name;
    const char* eltType    = at->getField("eltType")->type->symbol->name;

    at->symbol->name = astr("[", domainType, "] ", eltType);
  }

  if (at->instantiatedFrom                                          != NULL &&
      strcmp(at->instantiatedFrom->symbol->name, defaultDomainName) == 0) {
    at->symbol->name = astr("domain",
                            at->symbol->name + strlen(defaultDomainName));
  }

  if (isRecordWrappedType(at) == true) {
    at->symbol->name = at->getField("_instance")->type->symbol->name;
  }
}


static void setScalarPromotionType(AggregateType* at) {
  for_fields(field, at) {
    if (strcmp(field->name, "_promotionType") == 0) {
      at->scalarPromotionType = field->type;
    }
  }
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

DisambiguationContext::DisambiguationContext(CallInfo& info) {
  actuals = &info.actuals;
  scope   = (info.scope) ? info.scope : getVisibilityBlock(info.call);
  explain = false;

  if (fExplainVerbose == true) {
    if (explainCallLine != 0 && explainCallMatch(info.call) == true) {
      explain = true;
    }

    if (info.call->id == explainCallID) {
      explain = true;
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

DisambiguationState::DisambiguationState() {
  fn1MoreSpecific = false;
  fn2MoreSpecific = false;

  fn1Promotes     = false;
  fn2Promotes     = false;

  paramPrefers    = 0;
}

void DisambiguationState::updateParamPrefers(
                                     int                          preference,
                                     const char*                  argStr,
                                     const DisambiguationContext& DC) {
  if (paramPrefers == 0 || paramPrefers == preference) {
    paramPrefers = preference;
    EXPLAIN("param prefers %s\n", argStr);

  } else {
    paramPrefers = -1;
    EXPLAIN("param prefers differing things\n");
  }
}
