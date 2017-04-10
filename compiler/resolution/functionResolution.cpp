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

#include "astutil.h"
#include "build.h"
#include "caches.h"
#include "callInfo.h"
#include "CatchStmt.h"
#include "CForLoop.h"
#include "driver.h"
#include "expr.h"
#include "ForLoop.h"
#include "initializerResolution.h"
#include "iterator.h"
#include "ParamForLoop.h"
#include "passes.h"
#include "resolveIntents.h"
#include "scopeResolve.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "TryStmt.h"
#include "typeSpecifier.h"
#include "view.h"
#include "WhileStmt.h"

#include "../ifa/prim_data.h"

#include <inttypes.h>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// Allow disambiguation tracing to be controlled by the command-line option
// --explain-verbose.
#define ENABLE_TRACING_OF_DISAMBIGUATION 1

#ifdef ENABLE_TRACING_OF_DISAMBIGUATION
#define TRACE_DISAMBIGUATE_BY_MATCH(...)                    \
  if (developer && DC.explain) fprintf(stderr, __VA_ARGS__)
#else
#define TRACE_DISAMBIGUATE_BY_MATCH(...)
#endif

/// State information used during the disambiguation process.
class DisambiguationState {
public:
  /// Is fn1 more specific than fn2?
  bool fn1MoreSpecific;
  /// Is fn2 more specific than fn1?
  bool fn2MoreSpecific;

  /// Does fn1 require promotion?
  bool fn1Promotes;
  /// Does fn2 require promotion?
  bool fn2Promotes;

  /// 1 == fn1, 2 == fn2, -1 == conflicting signals
  int paramPrefers;

  /// Initialize the state to the starting values.
  DisambiguationState()
    : fn1MoreSpecific(false), fn2MoreSpecific(false),
      fn1Promotes(false), fn2Promotes(false), paramPrefers(0) {}

  /** Prints out information for tracing of the disambiguation process.
   *
   * \param DBMLoc A string representing the location in the DBM process the
   *               message is coming from.
   * \param DC     The disambiguation context.
   */
  void printSummary(const char* DBMLoc, const DisambiguationContext& DC) {
    if (this->fn1MoreSpecific) {
      TRACE_DISAMBIGUATE_BY_MATCH("\n%s: Fn %d is more specific than Fn %d\n", DBMLoc, DC.i, DC.j);
    } else {
      TRACE_DISAMBIGUATE_BY_MATCH("\n%s: Fn %d is NOT more specific than Fn %d\n", DBMLoc, DC.i, DC.j);
    }

    if (this->fn2MoreSpecific) {
      TRACE_DISAMBIGUATE_BY_MATCH("%s: Fn %d is more specific than Fn %d\n", DBMLoc, DC.j, DC.i);
    } else {
      TRACE_DISAMBIGUATE_BY_MATCH("%s: Fn %d is NOT more specific than Fn %d\n", DBMLoc, DC.j, DC.i);
    }
  }
};

//#
//# Global Variables
//#
bool resolved = false;
bool inDynamicDispatchResolution = false;
SymbolMap paramMap;

int explainCallLine;
bool tryFailure = false;

//#
//# Static Variables
//#
static ModuleSymbol* explainCallModule;

static Vec<CallExpr*> inits;
static Vec<FnSymbol*> resolvedFormals;
Vec<CallExpr*> callStack;

static Vec<CondStmt*> tryStack;

static Map<Type*,Type*> runtimeTypeMap; // map static types to runtime types
                                        // e.g. array and domain runtime types
static Map<Type*,FnSymbol*> valueToRuntimeTypeMap; // convertValueToRuntimeType
static Map<Type*,FnSymbol*> runtimeTypeToValueMap; // convertRuntimeTypeToValue

static std::map<std::string, std::pair<AggregateType*, FnSymbol*> > functionTypeMap; // lookup table/cache for function types and their representative parents
static std::map<FnSymbol*, FnSymbol*> functionCaptureMap; //lookup table/cache for function captures

// map of compiler warnings that may need to be reissued for repeated
// calls; the inner compiler warning map is from the compilerWarning
// function; the outer compiler warning map is from the function
// containing the compilerWarning function
// to do: this needs to be a map from functions to multiple strings in
//        order to support multiple compiler warnings are allowed to
//        be in a single function
static Map<FnSymbol*,const char*> innerCompilerWarningMap;
static Map<FnSymbol*,const char*> outerCompilerWarningMap;

std::map<Type*,FnSymbol*> autoCopyMap; // type to chpl__autoCopy function
Map<Type*,FnSymbol*> autoDestroyMap; // type to chpl__autoDestroy function
Map<Type*,FnSymbol*> unaliasMap; // type to chpl__unalias function


Map<FnSymbol*,FnSymbol*> iteratorLeaderMap; // iterator->leader map for promotion
Map<FnSymbol*,FnSymbol*> iteratorFollowerMap; // iterator->leader map for promotion
std::map<CallExpr*, CallExpr*> eflopiMap; // for-loops over par iterators

static char arrayUnrefName[] = "array_unref_ret_tmp";

//#
//# Static Function Declarations
//#
static bool hasRefField(Type *type);
static bool typeHasRefField(Type *type);
static FnSymbol* resolveUninsertedCall(Type* type, CallExpr* call,
                                       bool checkonly=false);
static bool hasUserAssign(Type* type);
static void resolveAutoCopyEtc(Type* type);
static void resolveOther();
static FnSymbol*
protoIteratorMethod(IteratorInfo* ii, const char* name, Type* retType);
static void protoIteratorClass(FnSymbol* fn);
static bool isInstantiatedField(Symbol* field);
static Symbol* determineQueriedField(CallExpr* call);
static void resolveSpecifiedReturnType(FnSymbol* fn);
static bool fits_in_int(int width, Immediate* imm);
static bool fits_in_uint(int width, Immediate* imm);
static bool canInstantiate(Type* actualType, Type* formalType);
static bool canParamCoerce(Type* actualType, Symbol* actualSym, Type* formalType);
static bool
moreSpecific(FnSymbol* fn, Type* actualType, Type* formalType);
static bool
computeActualFormalAlignment(FnSymbol* fn,
                             Vec<Symbol*>& formalIdxToActual,
                             Vec<ArgSymbol*>& actualIdxToFormal,
                             CallInfo& info);
static Type*
getInstantiationType(Type* actualType, Type* formalType);
static void
computeGenericSubs(SymbolMap &subs,
                   FnSymbol* fn,
                   Vec<Symbol*>& formalIdxToActual,
                   bool inInitRes);

static void
filterCandidate(Vec<ResolutionCandidate*>& candidates,
                ResolutionCandidate* currCandidate,
                CallInfo& info);

static void
filterCandidate(Vec<ResolutionCandidate*>& candidates,
                FnSymbol* fn,
                CallInfo& info);

static BlockStmt* getParentBlock(Expr* expr);
static bool
isMoreVisibleInternal(BlockStmt* block, FnSymbol* fn1, FnSymbol* fn2,
                      Vec<BlockStmt*>& visited);
static bool
isMoreVisible(Expr* expr, FnSymbol* fn1, FnSymbol* fn2);
static CallExpr* userCall(CallExpr* call);
static void issueCompilerError(CallExpr* call);
static void reissueCompilerWarning(const char* str, int offset);
BlockStmt* getVisibilityBlock(Expr* expr);
static void buildVisibleFunctionMap();
static BlockStmt*
getVisibleFunctions(BlockStmt* block,
                    const char* name,
                    Vec<FnSymbol*>& visibleFns,
                    Vec<BlockStmt*>& visited,
                    CallExpr* callOrigin);
static Expr* resolve_type_expr(Expr* expr);
static void makeNoop(CallExpr* call);
void resolveDefaultGenericType(CallExpr* call);
static Type* resolveDefaultGenericTypeSymExpr(SymExpr* se);

static void
gatherCandidates(Vec<ResolutionCandidate*>& candidates,
                 Vec<FnSymbol*>& visibleFns,
                 CallInfo& info);

static FnSymbol* resolveNormalCall(CallExpr* call, bool checkonly=false);
static void resolveTupleAndExpand(CallExpr* call);
static void resolveTupleExpand(CallExpr* call);
static void handleSetMemberTypeMismatch(Type* t, Symbol* fs, CallExpr* call,
                                        SymExpr* rhs);
static void resolveSetMember(CallExpr* call);
static void resolveInitField(CallExpr* call);
static void resolveInitVar(CallExpr* call);
static void resolveMove(CallExpr* call);
static void resolveNew(CallExpr* call);
static void resolveCoerce(CallExpr* call);
static bool formalRequiresTemp(ArgSymbol* formal);
static void addLocalCopiesAndWritebacks(FnSymbol* fn, SymbolMap& formals2vars);

static Expr* dropUnnecessaryCast(CallExpr* call);
static AggregateType* createAndInsertFunParentClass(CallExpr *call, const char *name);
static FnSymbol* createAndInsertFunParentMethod(CallExpr *call, AggregateType *parent, AList &arg_list, bool isFormal, Type *retType);
static std::string buildParentName(AList &arg_list, bool isFormal, Type *retType);
static AggregateType* createOrFindFunTypeFromAnnotation(AList &arg_list, CallExpr *call);
static Expr* createFunctionAsValue(CallExpr *call);
static Type* resolveTypeAlias(SymExpr* se);
static Expr* preFold(Expr* expr);
static void foldEnumOp(int op, EnumSymbol *e1, EnumSymbol *e2, Immediate *imm);
static bool isSubType(Type* sub, Type* super);
static bool isInstantiation(Type* sub, Type* super);
static bool isSubTypeOrInstantiation(Type* sub, Type* super);
static void insertValueTemp(Expr* insertPoint, Expr* actual);
FnSymbol* requiresImplicitDestroy(CallExpr* call);
static Expr* postFold(Expr* expr);
static Expr* resolveExpr(Expr* expr);
static void
computeReturnTypeParamVectors(BaseAST* ast,
                              Symbol* retSymbol,
                              Vec<Type*>& retTypes,
                              Vec<Symbol*>& retParams);
static void
insertCasts(BaseAST* ast, FnSymbol* fn, Vec<CallExpr*>& casts);
static bool
possible_signature_match(FnSymbol* fn, FnSymbol* gn);
static bool signature_match(FnSymbol* fn, FnSymbol* gn);
static void
collectInstantiatedAggregateTypes(Vec<Type*>& icts, Type* ct);
static bool isVirtualChild(FnSymbol* child, FnSymbol* parent);
static void addToVirtualMaps(FnSymbol* pfn, AggregateType* ct);
static void addAllToVirtualMaps(FnSymbol* fn, AggregateType* ct);
static void buildVirtualMaps();
static void
addVirtualMethodTableEntry(Type* type, FnSymbol* fn, bool exclusive = false);
static void computeStandardModuleSet();
static void unmarkDefaultedGenerics();
static void resolveUses(ModuleSymbol* mod);
static void resolveSupportForModuleDeinits();
static void resolveExports();
static void resolveEnumTypes();
static void resolveDynamicDispatches();
static void insertRuntimeTypeTemps();
static void resolveAutoCopies();
static bool propagateNotPOD(Type* t);
static void resolveRecordInitializers();
static void insertDynamicDispatchCalls();
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
static Expr* resolvePrimInit(CallExpr* call);
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


bool ResolutionCandidate::computeAlignment(CallInfo& info) {
  if (formalIdxToActual.n != 0) formalIdxToActual.clear();
  if (actualIdxToFormal.n != 0) actualIdxToFormal.clear();

  return computeActualFormalAlignment(fn, formalIdxToActual, actualIdxToFormal, info);
}

void ResolutionCandidate::computeSubstitutions(bool inInitRes) {
  if (substitutions.n != 0) substitutions.clear();
  computeGenericSubs(substitutions, fn, formalIdxToActual, inInitRes);
}

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

// Temporarily add a call, resolve it, then remove it.
// Return the function that the call resolved to, or NULL if it didn't.
// Either insideBlock or beforeExpr must be != NULL and
// indicate where the call should be added.
static FnSymbol*
resolveUninsertedCall(BlockStmt* insideBlock,
                      Expr* beforeExpr,
                      CallExpr* call,
                      bool checkonly) {

  // In case resolveCall drops other stuff into the tree ahead of the
  // call, we wrap everything in a block for safe removal.
  BlockStmt* block = new BlockStmt();

  if (insideBlock) {
    insideBlock->insertAtHead(block);
  } else if(beforeExpr) {
    beforeExpr->insertBefore(block);
  } else {
    INT_ASSERT(insideBlock != NULL || beforeExpr != NULL);
  }

  INT_ASSERT(block->parentSymbol);

  block->insertAtHead(call);
  if (checkonly && !call->primitive) {
    resolveNormalCall(call, checkonly);
  } else {
    if (checkonly) {
      INT_FATAL(call, "checkonly is being discarded because the call is a "
                "primitive.\nIf that is not intended, please extend "
                "resolveCall");
    }
    resolveCall(call);
  }
  block->remove();

  return call->isResolved();
}

// Resolve a call to do with a particular type.
static FnSymbol*
resolveUninsertedCall(Type* type, CallExpr* call, bool checkonly) {

  BlockStmt* insideBlock = NULL;
  Expr* beforeExpr = NULL;

  if (type->defaultInitializer) {
    if (type->defaultInitializer->instantiationPoint)
      insideBlock = type->defaultInitializer->instantiationPoint;
    else
      beforeExpr = type->symbol->defPoint;
  } else {
    insideBlock = chpl_gen_main->body;
  }

  return resolveUninsertedCall(insideBlock, beforeExpr, call, checkonly);
}

//
// Invoke resolveFns(fn), while having 'call' be on the top of 'callStack'.
//
static void resolveFnForCall(FnSymbol* fn, CallExpr* call)
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

  if (FnSymbol* callee = call->isResolved()) {
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

bool hasAutoCopyForType(Type* type) {
  return autoCopyMap[type] != NULL;
}

// This function is intended to protect gets from the autoCopyMap so that
// we can insert NULL values for a type and avoid segfaults
FnSymbol* getAutoCopyForType(Type* type) {
  FnSymbol* ret = autoCopyMap[type]; // Do not try this at home
  if (ret == NULL) {
    INT_FATAL(type, "Trying to obtain autoCopy for type '%s',"
                    " which defines none", type->symbol->name);
  }
  return ret;
}

void getAutoCopyTypeKeys(Vec<Type*> &keys) {
  for (std::map<Type*, FnSymbol*>::iterator it = autoCopyMap.begin();
       it != autoCopyMap.end(); ++it) {
    keys.add(it->first);
  }
}

// This function is called by generic instantiation
// for the default initCopy function in ChapelBase.chpl.
bool fixupDefaultInitCopy(FnSymbol* fn, FnSymbol* newFn, CallExpr* call)
{
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


static void
resolveAutoCopyEtc(Type* type) {

  // resolve autoCopy
  if (!hasAutoCopyForType(type)) {

    const char* copyFnToCall = "chpl__autoCopy";

    if (isUserDefinedRecord(type) &&
        !type->symbol->hasFlag(FLAG_TUPLE) &&
        !isRecordWrappedType(type) &&
        !isSyncType(type) &&
        !isSingleType(type) &&
        !type->symbol->hasFlag(FLAG_COPY_MUTATES)) {
      // Just use 'chpl__initCopy' instead of 'chpl__autoCopy'
      // for user-defined records. This way, if the type does not
      // support copying, the autoCopyMap will store a function
      // marked with FLAG_ERRONEOUS_INITCOPY. Additionally, user-defined
      // records shouldn't be defining chpl__initCopy or chpl__autoCopy
      // and certainly shouldn't rely on the differences between the two.

      copyFnToCall = "chpl__initCopy";
    }

    SET_LINENO(type->symbol);
    Symbol* tmp = newTemp(type);
    chpl_gen_main->insertAtHead(new DefExpr(tmp));
    CallExpr* call = new CallExpr(copyFnToCall, tmp);
    FnSymbol* fn = resolveUninsertedCall(type, call);
    resolveFns(fn);
    INT_ASSERT(!fn->hasFlag(FLAG_PROMOTION_WRAPPER));
    autoCopyMap[type] = fn;

    tmp->defPoint->remove();
  }

  // resolve autoDestroy
  if (autoDestroyMap.get(type) == NULL) {
    SET_LINENO(type->symbol);
    Symbol* tmp = newTemp(type);
    chpl_gen_main->insertAtHead(new DefExpr(tmp));
    CallExpr* call = new CallExpr("chpl__autoDestroy", tmp);
    FnSymbol* fn = resolveUninsertedCall(type, call);
    // Adding the flag here isn't sufficient for checks
    // to auto destroy fn during resolution, so
    // I added it with a pragma in the modules.
    fn->addFlag(FLAG_AUTO_DESTROY_FN);
    resolveFns(fn);
    INT_ASSERT(!fn->hasFlag(FLAG_PROMOTION_WRAPPER));
    autoDestroyMap.put(type, fn);
    tmp->defPoint->remove();
  }

  // resolve unalias
  // We make the 'unalias' hook available to all user records,
  // but for now it only applies to array/domain/distribution
  // in order to minimize the changes.
  if (isRecordWrappedType(type) && unaliasMap.get(type) == NULL) {
    SET_LINENO(type->symbol);
    Symbol* tmp = newTemp(type);
    chpl_gen_main->insertAtHead(new DefExpr(tmp));
    CallExpr* call = new CallExpr("chpl__unalias", tmp);
    FnSymbol* fn = resolveUninsertedCall(type, call);
    resolveFns(fn);
    INT_ASSERT(!fn->hasFlag(FLAG_PROMOTION_WRAPPER));
    unaliasMap.put(type, fn);
    tmp->defPoint->remove();
  }
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

const char* toString(Type* type) {
  if( type ) return type->getValType()->symbol->name;
  return "null type";
}


const char* toString(CallInfo* info) {
  bool method = false;
  bool _this = false;
  const char *str = "";
  if (info->actuals.n > 1)
    if (info->actuals.head()->type == dtMethodToken)
      method = true;
  if (!strcmp("this", info->name)) {
    _this = true;
    method = false;
  }
  if (method) {
    if (info->actuals.v[1] && info->actuals.v[1]->hasFlag(FLAG_TYPE_VARIABLE))
      str = astr(str, "type ", toString(info->actuals.v[1]->type), ".");
    else
      str = astr(str, toString(info->actuals.v[1]->type), ".");
  }
  if (!developer && !strncmp("_type_construct_", info->name, 16)) {
    str = astr(str, info->name+16);
  } else if (!developer && !strncmp("_construct_", info->name, 11)) {
    str = astr(str, info->name+11);
    str = astr(str, ".init");
  } else if (!_this) {
    str = astr(str, info->name);
  }
  if (!info->call->methodTag) {
    if (info->call->square)
      str = astr(str, "[");
    else
      str = astr(str, "(");
  }
  bool first = false;
  int start = 0;
  if (method)
    start = 2;
  if (_this)
    start = 2;
  for (int i = start; i < info->actuals.n; i++) {
    if (!first)
      first = true;
    else
      str = astr(str, ", ");
    if (info->actualNames.v[i])
      str = astr(str, info->actualNames.v[i], "=");
    VarSymbol* var = toVarSymbol(info->actuals.v[i]);
    if (info->actuals.v[i]->type->symbol->hasFlag(FLAG_ITERATOR_RECORD) &&
        toAggregateType(info->actuals.v[i]->type)->iteratorInfo->iterator->hasFlag(FLAG_PROMOTION_WRAPPER))
      str = astr(str, "promoted expression");
    else if (info->actuals.v[i] && info->actuals.v[i]->hasFlag(FLAG_TYPE_VARIABLE))
      str = astr(str, "type ", toString(info->actuals.v[i]->type));
    else if (var && var->immediate) {
      if (var->immediate->const_kind == CONST_KIND_STRING) {
        str = astr(str, "\"", var->immediate->v_string, "\"");
      } else {
        const size_t bufSize = 512;
        char buff[bufSize];
        snprint_imm(buff, bufSize, *var->immediate);
        str = astr(str, buff);
      }
    } else
      str = astr(str, toString(info->actuals.v[i]->type));
  }
  if (!info->call->methodTag) {
    if (info->call->square)
      str = astr(str, "]");
    else
      str = astr(str, ")");
  }
  return str;
}


const char* toString(FnSymbol* fn) {
  if (fn->userString) {
    if (developer)
      return astr(fn->userString, " [", istr(fn->id), "]");
    else
      return fn->userString;
  }
  const char* str;
  int start = 0;
 if (developer) {
   // report the name as-is and include all args
   str = fn->name;
 } else {
  if (fn->instantiatedFrom)
    fn = fn->instantiatedFrom;
  if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)) {
    // if not, make sure 'str' is built as desired
    INT_ASSERT(!strncmp("_type_construct_", fn->name, 16));
    str = astr(fn->name+16);
  } else if (fn->hasFlag(FLAG_CONSTRUCTOR)) {
    if (!strncmp("_construct_", fn->name, 11)) {
      str = astr(fn->name+11, ".init");
    } else if (!strcmp("init", fn->name)) {
      str = "init";
    } else {
      str = "";
      INT_FATAL(fn, "flagged as constructor but not named _construct_ or init");
    }
  } else if (fn->isPrimaryMethod()) {
    if (!strcmp(fn->name, "this")) {
      INT_ASSERT(fn->hasFlag(FLAG_FIRST_CLASS_FUNCTION_INVOCATION));
      str = astr(toString(fn->getFormal(2)->type));
      start = 1;
    } else {
      INT_ASSERT(! fn->hasFlag(FLAG_FIRST_CLASS_FUNCTION_INVOCATION));
      str = astr(toString(fn->getFormal(2)->type), ".", fn->name);
      start = 2;
    }
  } else if (fn->hasFlag(FLAG_MODULE_INIT)) {
    INT_ASSERT(!strncmp("chpl__init_", fn->name, 11)); //if not, fix next line
    str = astr("top-level module statements for ", fn->name+11);
  } else
    str = astr(fn->name);
 } // if !developer

  bool skipParens =
    fn->hasFlag(FLAG_NO_PARENS) ||
    (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) && fn->numFormals() == 0) ||
    (fn->hasFlag(FLAG_MODULE_INIT) && !developer);

  if (!skipParens)
    str = astr(str, "(");
  bool first = false;
  for (int i = start; i < fn->numFormals(); i++) {
    ArgSymbol* arg = fn->getFormal(i+1);
    if (arg->hasFlag(FLAG_IS_MEME))
      continue;
    if (!first) {
      first = true;
      if (skipParens)
        str = astr(str, " ");
    } else
      str = astr(str, ", ");
    if (arg->intent == INTENT_PARAM || arg->hasFlag(FLAG_INSTANTIATED_PARAM))
      str = astr(str, "param ");
    if (arg->hasFlag(FLAG_TYPE_VARIABLE))
      str = astr(str, "type ", arg->name);
    else if (arg->type == dtUnknown) {
      SymExpr* sym = NULL;
      if (arg->typeExpr)
        sym = toSymExpr(arg->typeExpr->body.tail);
      if (sym)
        str = astr(str, arg->name, ": ", sym->symbol()->name);
      else
        str = astr(str, arg->name);
    } else if (arg->type == dtAny) {
      str = astr(str, arg->name);
    } else
      str = astr(str, arg->name, ": ", toString(arg->type));
    if (arg->variableExpr)
      str = astr(str, " ...");
  }
  if (!skipParens)
    str = astr(str, ")");
  if (developer)
    str = astr(str, " [", istr(fn->id), "]");
  return str;
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
  add_root_type(ii->iclass);    // Add super : dtObject.
  fn->defPoint->insertBefore(new DefExpr(cts));

  ii->irecord = new AggregateType(AGGREGATE_RECORD);
  TypeSymbol* rts = new TypeSymbol(astr("_ir_", className), ii->irecord);
  rts->addFlag(FLAG_ITERATOR_RECORD);
  // TODO -- do a better job of deciding if an iterator record is
  // POD or not POD.
  rts->addFlag(FLAG_NOT_POD);
  if (fn->retTag == RET_REF) // TODO -- handle RET_CONST_REF
    rts->addFlag(FLAG_REF_ITERATOR_CLASS);
  fn->defPoint->insertBefore(new DefExpr(rts));

  ii->tag = it_iterator;
  ii->advance = protoIteratorMethod(ii, "advance", dtVoid);
  ii->zip1 = protoIteratorMethod(ii, "zip1", dtVoid);
  ii->zip2 = protoIteratorMethod(ii, "zip2", dtVoid);
  ii->zip3 = protoIteratorMethod(ii, "zip3", dtVoid);
  ii->zip4 = protoIteratorMethod(ii, "zip4", dtVoid);
  ii->hasMore = protoIteratorMethod(ii, "hasMore", dtInt[INT_SIZE_DEFAULT]);
  ii->getValue = protoIteratorMethod(ii, "getValue", fn->retType);
  ii->init = protoIteratorMethod(ii, "init", dtVoid);
  ii->incr = protoIteratorMethod(ii, "incr", dtVoid);

  // Save the iterator info in the iterator record.
  // The iterator info is still owned by the iterator function.
  ii->irecord->iteratorInfo = ii;
  ii->irecord->scalarPromotionType = fn->retType;
  fn->retType = ii->irecord;
  fn->retTag = RET_VALUE;

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


//
// returns true if the field was instantiated
//
static bool
isInstantiatedField(Symbol* field) {
  TypeSymbol* ts = toTypeSymbol(field->defPoint->parentSymbol);
  INT_ASSERT(ts);
  AggregateType* ct = toAggregateType(ts->type);
  INT_ASSERT(ct);
  for_formals(formal, ct->defaultTypeConstructor) {
    if (!strcmp(field->name, formal->name))
      if (formal->hasFlag(FLAG_TYPE_VARIABLE))
        return true;
  }
  return false;
}


//
// determine field associated with query expression
//
static Symbol*
determineQueriedField(CallExpr* call) {
  AggregateType* ct = toAggregateType(call->get(1)->getValType());
  INT_ASSERT(ct);
  SymExpr* last = toSymExpr(call->get(call->numActuals()));
  INT_ASSERT(last);
  VarSymbol* var = toVarSymbol(last->symbol());
  INT_ASSERT(var && var->immediate);
  if (var->immediate->const_kind == CONST_KIND_STRING) {
    // field queried by name
    return ct->getField(var->immediate->v_string, false);
  } else {
    // field queried by position
    int position = var->immediate->int_value();
    Vec<ArgSymbol*> args;
    for_formals(arg, ct->defaultTypeConstructor) {
      args.add(arg);
    }
    for (int i = 2; i < call->numActuals(); i++) {
      SymExpr* actual = toSymExpr(call->get(i));
      INT_ASSERT(actual);
      VarSymbol* var = toVarSymbol(actual->symbol());
      INT_ASSERT(var && var->immediate && var->immediate->const_kind == CONST_KIND_STRING);
      for (int j = 0; j < args.n; j++) {
        if (args.v[j] && !strcmp(args.v[j]->name, var->immediate->v_string))
          args.v[j] = NULL;
      }
    }
    forv_Vec(ArgSymbol, arg, args) {
      if (arg) {
        if (position == 1)
          return ct->getField(arg->name, false);
        position--;
      }
    }
  }
  return NULL;
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

static bool
isTupleContainingOnlyReferences(Type* t)
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
getReturnedTupleType(FnSymbol* fn, Type* retType)
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

      retType = getReturnedTupleType(fn, retType);
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
    && !fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR)
    && !fn->hasFlag(FLAG_CONSTRUCTOR)
    && strcmp(fn->name,"=") != 0
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
        formal->type = formal->type->refType;
        // The type of the formal is its own ref type!
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
        Type* newType = computeTupleWithIntent(intent, formal->type);
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


static void ensureEnumTypeResolved(EnumType* etype) {
  INT_ASSERT( etype != NULL );

  if( ! etype->integerType ) {
    // Make sure to resolve all enum types.
    for_enums(def, etype) {
      if (def->init) {
        Expr* enumTypeExpr =
        resolve_type_expr(def->init);

        Type* enumtype = enumTypeExpr->typeInfo();
        if (enumtype == dtUnknown)
          INT_FATAL(def->init, "Unable to resolve enumerator type expression");

        // printf("Type of %s.%s is %s\n", etype->symbol->name, def->sym->name,
        // enumtype->symbol->name);
      }
    }
    // Now try computing the enum size...
    etype->sizeAndNormalize();
  }

  INT_ASSERT(etype->integerType != NULL);
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
static
Type* getConcreteParentForGenericFormal(Type* actualType, Type* formalType)
{
  forv_Vec(Type, parent, actualType->dispatchParents) {
    if (isInstantiation(parent, formalType))
      return parent;
    if (Type* t = getConcreteParentForGenericFormal(parent, formalType))
      return t;
  }
  return NULL;
}

// Returns true iff dispatching the actualType to the formalType
// results in an instantiation.
static bool
canInstantiate(Type* actualType, Type* formalType) {
  if (actualType == dtMethodToken)
    return false;
  if (formalType == dtAny)
    return true;
  if (formalType == dtIntegral && (is_int_type(actualType) || is_uint_type(actualType)))
    return true;
  if (formalType == dtAnyEnumerated && (is_enum_type(actualType)))
    return true;
  if (formalType == dtNumeric &&
      (is_int_type(actualType) || is_uint_type(actualType) || is_imag_type(actualType) ||
       is_real_type(actualType) || is_complex_type(actualType)))
    return true;
  if (formalType == dtAnyComplex && (is_complex_type(actualType)))
    return true;
  if (formalType == dtString && actualType==dtStringC)
    return true;
  if (formalType == dtStringC && actualType==dtStringCopy)
    return true;
  if (formalType == dtIteratorRecord && actualType->symbol->hasFlag(FLAG_ITERATOR_RECORD))
    return true;
  if (formalType == dtIteratorClass && actualType->symbol->hasFlag(FLAG_ITERATOR_CLASS))
    return true;
  if (actualType == formalType)
    return true;
  if (actualType->instantiatedFrom && canInstantiate(actualType->instantiatedFrom, formalType))
    return true;
  if (actualType->instantiatedFrom &&
      formalType->symbol->hasFlag(FLAG_GENERIC) &&
      getConcreteParentForGenericFormal(actualType, formalType))
    return true;

  return false;
}


//
// returns true if dispatching from actualType to formalType results
// in a compile-time coercion; this is a subset of canCoerce below as,
// for example, real(32) cannot be coerced to real(64) at compile-time
//
static bool canParamCoerce(Type* actualType, Symbol* actualSym, Type* formalType) {
  if (is_bool_type(formalType) && is_bool_type(actualType))
    return true;
  if (is_int_type(formalType)) {
    if (is_bool_type(actualType))
      return true;
    if (is_int_type(actualType) &&
        get_width(actualType) < get_width(formalType))
      return true;
    if (is_uint_type(actualType) &&
        get_width(actualType) < get_width(formalType))
      return true;

    //
    // If the actual is an enum, check to see if *all* its values
    // are small enough that they fit into this integer width
    //
    if (EnumType* etype = toEnumType(actualType)) {
      ensureEnumTypeResolved(etype);
      if (get_width(etype->getIntegerType()) <= get_width(formalType))
        return true;
    }

    //
    // For smaller integer types, if the argument is a param, does it
    // store a value that's small enough that it could dispatch to
    // this argument?
    //
    if (get_width(formalType) < 64) {
      if (VarSymbol* var = toVarSymbol(actualSym))
        if (var->immediate)
          if (fits_in_int(get_width(formalType), var->immediate))
            return true;

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
    if (is_bool_type(actualType))
      return true;
    if (is_uint_type(actualType) &&
        get_width(actualType) < get_width(formalType))
      return true;
    if (VarSymbol* var = toVarSymbol(actualSym))
      if (var->immediate)
        if (fits_in_uint(get_width(formalType), var->immediate))
          return true;
  }
  // param strings can coerce between string and c_string
  if ((formalType == dtString || formalType == dtStringC) &&
      (actualType == dtString || actualType == dtStringC))
    if (actualSym && actualSym->isImmediate())
      return true;

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

// Returns true iff the actualType can dispatch to the formalType.
// The function symbol is used to avoid scalar promotion on =.
// param is set if the actual is a parameter (compile-time constant).
// fn is the function being called
bool
canDispatch(Type* actualType, Symbol* actualSym, Type* formalType, FnSymbol* fn, bool* promotes, bool paramCoerce) {
  if (promotes)
    *promotes = false;
  if (actualType == formalType)
    return true;
  if (actualType->symbol->hasFlag(FLAG_GENERIC) &&
      actualType == formalType->instantiatedFrom) {
    // The actual should only be generic when we're resolving an initializer
    // If either of these asserts fail, something is very, very wrong.
    AggregateType* at = toAggregateType(actualType);
    INT_ASSERT(at && at->initializerStyle == DEFINES_INITIALIZER);
    INT_ASSERT(strcmp(fn->name, "init") == 0);

    return true;
  }
  //
  // The following check against FLAG_REF ensures that 'nil' can't be
  // passed to a by-ref argument (for example, an atomic type).  I
  // found that without this, calls like autocopy(nil) became
  // ambiguous when given the choice between the completely generic
  // autocopy(x) and the autocopy(x: atomic int) (represented as
  // autocopy(x: ref(atomic int)) internally).
  //
  if (actualType == dtNil && isClass(formalType) &&
      !formalType->symbol->hasFlag(FLAG_REF))
    return true;
  if (actualType->refType == formalType &&
      // This is a workaround for type problems with tuples
      // in implement forall intents...
      !(fn && fn->hasFlag(FLAG_BUILD_TUPLE) && fn->hasFlag(FLAG_ALLOW_REF)))
    return true;
  if (!paramCoerce && canCoerce(actualType, actualSym, formalType, fn, promotes))
    return true;
  if (paramCoerce && canParamCoerce(actualType, actualSym, formalType))
    return true;

  forv_Vec(Type, parent, actualType->dispatchParents) {
    if (parent == formalType || canDispatch(parent, NULL, formalType, fn, promotes)) {
      return true;
    }
  }

  if (fn &&
      strcmp(fn->name, "=") &&
      actualType->scalarPromotionType &&
      (canDispatch(actualType->scalarPromotionType, NULL, formalType, fn))) {
    if (promotes)
      *promotes = true;
    return true;
  }

  return false;
}

bool
isDispatchParent(Type* t, Type* pt) {
  forv_Vec(Type, p, t->dispatchParents)
    if (p == pt || isDispatchParent(p, pt))
      return true;
  return false;
}

static bool
moreSpecific(FnSymbol* fn, Type* actualType, Type* formalType) {
  if (canDispatch(actualType, NULL, formalType, fn))
    return true;
  if (canInstantiate(actualType, formalType)) {
    return true;
  }
  return false;
}

static bool
computeActualFormalAlignment(FnSymbol* fn,
                             Vec<Symbol*>& formalIdxToActual,
                             Vec<ArgSymbol*>& actualIdxToFormal,
                             CallInfo& info) {
  formalIdxToActual.fill(fn->numFormals());
  actualIdxToFormal.fill(info.actuals.n);

  // Match named actuals against formal names in the function signature.
  // Record successful matches.
  for (int i = 0; i < actualIdxToFormal.n; i++) {
    if (info.actualNames.v[i]) {
      bool match = false;
      int j = 0;
      for_formals(formal, fn) {
        if (!strcmp(info.actualNames.v[i], formal->name)) {
          match = true;
          actualIdxToFormal.v[i] = formal;
          formalIdxToActual.v[j] = info.actuals.v[i];
          break;
        }
        j++;
      }
      // Fail if no matching formal is found.
      if (!match)
        return false;
    }
  }

  // Fill in unmatched formals in sequence with the remaining actuals.
  // Record successful substitutions.
  int j = 0;
  ArgSymbol* formal = (fn->numFormals()) ? fn->getFormal(1) : NULL;
  for (int i = 0; i < actualIdxToFormal.n; i++) {
    if (!info.actualNames.v[i]) {
      bool match = false;
      while (formal) {
        if (formal->variableExpr)
          return (fn->hasFlag(FLAG_GENERIC)) ? true : false;
        if (!formalIdxToActual.v[j]) {
          match = true;
          actualIdxToFormal.v[i] = formal;
          formalIdxToActual.v[j] = info.actuals.v[i];
          formal = next_formal(formal);
          j++;
          break;
        }
        formal = next_formal(formal);
        j++;
      }
      // Fail if there are too many unnamed actuals.
      if (!match && !(fn->hasFlag(FLAG_GENERIC) && fn->hasFlag(FLAG_TUPLE)))
        return false;
    }
  }

  // Make sure that any remaining formals are matched by name
  // or have a default value.
  while (formal) {
    if (!formalIdxToActual.v[j] && !formal->defaultExpr)
      // Fail if not.
      return false;
    formal = next_formal(formal);
    j++;
  }
  return true;
}

//
// returns the type that a formal type should be instantiated to when
// instantiated by a given actual type
//
static Type*
getBasicInstantiationType(Type* actualType, Type* formalType) {
  if (canInstantiate(actualType, formalType)) {
    return actualType;
  }
  if (Type* st = actualType->scalarPromotionType) {
    if (canInstantiate(st, formalType))
      return st;
  }
  if (Type* vt = actualType->getValType()) {
    if (canInstantiate(vt, formalType))
      return vt;
    else if (Type* st = vt->scalarPromotionType)
      if (canInstantiate(st, formalType))
        return st;
  }
  return NULL;
}

static Type*
getInstantiationType(Type* actualType, Type* formalType) {
  Type *ret = getBasicInstantiationType(actualType, formalType);

  // Now, if formalType is a generic parent type to actualType,
  // we should instantiate the parent actual type
  if (ret->instantiatedFrom &&
      formalType->symbol->hasFlag(FLAG_GENERIC)) {
    Type* concrete = getConcreteParentForGenericFormal(ret, formalType);
    if (concrete) {
      ret = concrete;
    }
  }

  return ret;
}


static void
computeGenericSubs(SymbolMap &subs,
                   FnSymbol* fn,
                   Vec<Symbol*>& formalIdxToActual,
                   bool inInitRes) {
  int i = 0;
  for_formals(formal, fn) {
    if (formal->intent == INTENT_PARAM) {
      if (formalIdxToActual.v[i] && formalIdxToActual.v[i]->isParameter()) {
        if (!formal->type->symbol->hasFlag(FLAG_GENERIC) ||
            canInstantiate(formalIdxToActual.v[i]->type, formal->type))
          subs.put(formal, formalIdxToActual.v[i]);
      } else if (!formalIdxToActual.v[i] && formal->defaultExpr) {

        // break because default expression may reference generic
        // arguments earlier in formal list; make those substitutions
        // first (test/classes/bradc/paramInClass/weirdParamInit4)
        if (subs.n)
          break;

        resolveBlockStmt(formal->defaultExpr);
        SymExpr* se = toSymExpr(formal->defaultExpr->body.tail);
        if (se && se->symbol()->isParameter() &&
            (!formal->type->symbol->hasFlag(FLAG_GENERIC) || canInstantiate(se->symbol()->type, formal->type)))
          subs.put(formal, se->symbol());
        else
          INT_FATAL(fn, "unable to handle default parameter");
      }
    } else if (formal->type->symbol->hasFlag(FLAG_GENERIC)) {

      //
      // check for field with specified generic type
      //
      if (!formal->hasFlag(FLAG_TYPE_VARIABLE) && formal->type != dtAny &&
          strcmp(formal->name, "outer") && !formal->hasFlag(FLAG_IS_MEME) &&
          (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) || fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)))
        USR_FATAL(formal, "invalid generic type specification on class field");

      if (formalIdxToActual.v[i]) {
        if (formal->hasFlag(FLAG_ARG_THIS) && inInitRes &&
            formalIdxToActual.v[i]->type->symbol->hasFlag(FLAG_GENERIC)) {
          // If the "this" arg is generic, we're resolving an initializer, and
          // the actual being passed is also still generic, don't count this as
          // a substitution.  Otherwise, we'll end up in an infinite loop if
          // one of the later generic args has a defaultExpr, as we will always
          // count the this arg as a substitution and so always approach the
          // generic arg with a defaultExpr as though a substitution was going
          // to take place.
        } else if (Type* type = getInstantiationType(formalIdxToActual.v[i]->type, formal->type)) {
          // String literal actuals aligned with non-param generic formals of
          // type dtAny will result in an instantiation of dtStringC when the
          // function is extern. In other words, let us write:
          //   extern proc foo(str);
          //   foo("bar");
          // and pass "bar" as a c_string instead of a string
          if (fn->hasFlag(FLAG_EXTERN) && (formal->type == dtAny) &&
              (!formal->hasFlag(FLAG_PARAM)) && (type == dtString) &&
              (formalIdxToActual.v[i]->type == dtString) &&
              (formalIdxToActual.v[i]->isImmediate())) {
            subs.put(formal, dtStringC->symbol);
          } else {
            subs.put(formal, type->symbol);
          }
        }
      } else if (formal->defaultExpr) {

        // break because default expression may reference generic
        // arguments earlier in formal list; make those substitutions
        // first (test/classes/bradc/genericTypes)
        if (subs.n)
          break;

        resolveBlockStmt(formal->defaultExpr);
        Type* defaultType = formal->defaultExpr->body.tail->typeInfo();
        if (defaultType == dtTypeDefaultToken)
          subs.put(formal, dtTypeDefaultToken->symbol);
        else if (Type* type = getInstantiationType(defaultType, formal->type))
          subs.put(formal, type->symbol);
      }
    }
    i++;
  }
}


//
// Assuming 'parent' is an indexing expression into the tuple 'se',
// return the index, between 1 and numArgs. Otherwise return 0.
//
static int varargAccessIndex(SymExpr* se, CallExpr* parent, int numArgs) {
  if (se == parent->baseExpr) {
    if(parent->numActuals() == 1) {
      VarSymbol* idxVar = toVarSymbol(toSymExpr(parent->get(1))->symbol());
      if (idxVar && idxVar->immediate &&
          idxVar->immediate->const_kind == NUM_KIND_INT) {
        int idxNum = idxVar->immediate->int_value();
        if (1 <= idxNum && idxNum <= numArgs)
          return idxNum;
        // report an "index out of bounds" error otherwise?
      }
    }
  }
  return 0;
}

//
// 'parent' corresponds to tuple(i); replace it with 'replFormal'.
//
// If 'parent' is the source of a prim_move into a temp 'dest',
// then eliminate 'dest' altogether, replacing its uses with 'replFormal'.
//
// Q: Why not just keep move(dest,replFormal), which is simpler?
// A: because 'move' does not preserv replFormal's ref-ness and const-ness,
// so we will get errors when using dest on the l.h.s. and miss errors
// when dest or deref(dest) is modified and it shouldn't.
// Note that we are here before the code is resolved, so we do not
// necessarily know whether 'replFormal' is a const and/or a ref.
// To keep 'dest' around, instead of move we probably need to add
// the AST equivalent of Chapel's "ref dest = replFormal;".
//
static void replaceVarargAccess(CallExpr* parent, SymExpr* se,
                                ArgSymbol* replFormal, SymbolMap& tempReps)
{
  if (CallExpr* move = toCallExpr(parent->parentExpr))
    if (move->isPrimitive(PRIM_MOVE)) {
      Symbol* dest = toSymExpr(move->get(1))->symbol();
      if (dest->hasFlag(FLAG_TEMP)) {
        // Remove the def of 'dest'.
        dest->defPoint->remove();
        move->remove();
        // Uses of 'dest' will come later. We replace them with 'replFormal'
        // in substituteVarargTupleRefs() using 'tempReps'.
        tempReps.put(dest, replFormal);
        return;
      }
    }
  // Otherwise, just replace 'parent'.
  parent->replace(new SymExpr(replFormal));
}

//
// Is 'parent' the call 'se'.size?
//
static bool isVarargSizeExpr(SymExpr* se, CallExpr* parent)
{
  if (parent->isNamed("size"))
    // Is 'parent' a method call?
    if (parent->numActuals() == 2)
      if (SymExpr* firstArg = toSymExpr(parent->get(1)))
        if (firstArg->symbol()->type == dtMethodToken) {
          // If this fails, ensure that isVarargSizeExpr still works correctly.
          INT_ASSERT(parent->get(2) == se);
          return true;
        }
  return false;
}

//
// Does 'ast' contain use(s) of the vararg tuple 'formal'
// that require 'formal' as a whole tuple?
//
// needVarArgTupleAsWhole() and substituteVarargTupleRefs() should handle
// the exact same set of SymExprs.
//
static bool needVarArgTupleAsWhole(BlockStmt* ast, int numArgs,
                                   ArgSymbol* formal)
{
  std::vector<SymExpr*> symExprs;
  collectSymExprs(ast, symExprs);

  for_vector(SymExpr, se, symExprs) {
    if (se->symbol() == formal) {
      if (CallExpr* parent = toCallExpr(se->parentExpr)) {
        if (parent->isPrimitive(PRIM_TUPLE_EXPAND)) {
          // (...formal) -- does not require
          continue;
        }
        if (varargAccessIndex(se, parent, numArgs) > 0) {
          // formal(i) -- does not require
          continue;
        }
        if (isVarargSizeExpr(se, parent)) {
          // formal.size -- does not require
          continue;
        }
      }
      // Something else ==> may require.
      return true;
    }
  }

  // The 'formal' is used only in "does not require" cases.
  return false;
}

//
// Replace, in 'ast', uses of the vararg tuple 'formal'
// with references to individual formals.
//
// needVarArgTupleAsWhole() and substituteVarargTupleRefs() should handle
// the exact same set of SymExprs.
//
void substituteVarargTupleRefs(BlockStmt* ast, int numArgs, ArgSymbol* formal,
                               std::vector<ArgSymbol*>& varargFormals)
{
  std::vector<SymExpr*> symExprs;
  collectSymExprs(ast, symExprs);
  SymbolMap tempReps;

  for_vector(SymExpr, se, symExprs) {
    if (se->symbol() == formal) {
      if (CallExpr* parent = toCallExpr(se->parentExpr)) {
        SET_LINENO(parent);

        if (parent->isPrimitive(PRIM_TUPLE_EXPAND)) {
          // Replace 'parent' with a sequence of individual args.
          for_vector(ArgSymbol, arg, varargFormals)
            parent->insertBefore(new SymExpr(arg));
          parent->remove();
          continue;
        }

        if (int idxNum = varargAccessIndex(se, parent, numArgs)) {
          INT_ASSERT(1 <= idxNum && idxNum <= numArgs);
          ArgSymbol* replFormal = varargFormals[idxNum-1];
          replaceVarargAccess(parent, se, replFormal, tempReps);
          continue;
        }

        if (isVarargSizeExpr(se, parent)) {
          parent->replace(new SymExpr(new_IntSymbol(numArgs)));
          continue;
        }

      }
      // Cases not "continue"-ed above should have been ruled out
      // by needVarArgTupleAsWhole().
      INT_ASSERT(false);
    } else if (Symbol* replmt = tempReps.get(se->symbol())) {
      SET_LINENO(se);
      se->replace(new SymExpr(replmt));
    }
  }
}


/** Common code for multiple paths through expandVarArgs.
 *
 * This code handles the case where the number of varargs are known at compile
 * time.  It inserts the necessary code to copy the values into and out of the
 * varargs tuple.
 */
static void
handleSymExprInExpandVarArgs(FnSymbol*  workingFn,
                             ArgSymbol* formal,
                             SymExpr*   sym) {
  // Handle specified number of variable arguments.
  // sym->symbol() is not a VarSymbol e.g. in: proc f(param n, v...n)
  if (VarSymbol* nVar = toVarSymbol(sym->symbol())) {
    if (nVar->type == dtInt[INT_SIZE_DEFAULT] && nVar->immediate) {
      workingFn->addFlag(FLAG_EXPANDED_VARARGS);

      SET_LINENO(formal);
      VarSymbol* var       = new VarSymbol(formal->name);
      int        n         = nVar->immediate->int_value();
      CallExpr*  tupleCall = NULL;

      // MPF 2016-10-02
      // This code used to be run for _build_tuple
      // and _build_tuple_always_allow_ref but now
      // only runs for var-args functions.

      std::vector<ArgSymbol*> varargFormals(n);
      if (formal->hasFlag(FLAG_TYPE_VARIABLE) == true)
        tupleCall = new CallExpr("_type_construct__tuple");
      else {
        tupleCall = new CallExpr("_construct__tuple");
        // _construct__tuple now calls initCopy, so var
        // needs to be auto-destroyed.
        var->addFlag(FLAG_INSERT_AUTO_DESTROY);
      }

      tupleCall->insertAtTail(new_IntSymbol(n));

      for (int i = 0; i < n; i++) {
        DefExpr*   newArgDef = formal->defPoint->copy();
        ArgSymbol* newFormal = toArgSymbol(newArgDef->sym);

        newFormal->variableExpr = NULL;
        newFormal->name         = astr("_e", istr(i), "_", formal->name);
        newFormal->cname        = astr("_e", istr(i), "_", formal->cname);

        tupleCall->insertAtTail(new SymExpr(newFormal));
        formal->defPoint->insertBefore(newArgDef);
        varargFormals[i] = newFormal;
      }

      bool needTupleInBody = true; //avoid "may be used uninitialized" warning

      // Replace mappings to the old formal with mappings to the new variable.
      if (PartialCopyData* pci = getPartialCopyInfo(workingFn)) {
        bool gotFormal = false; // for assertion only
        for (int index = pci->partialCopyMap.n; --index >= 0;) {
          SymbolMapElem& mapElem = pci->partialCopyMap.v[index];

          if (mapElem.value == formal) {
            gotFormal = true;
            needTupleInBody =
              needVarArgTupleAsWhole(pci->partialCopySource->body, n,
                                     toArgSymbol(mapElem.key));

            if (needTupleInBody) {
              mapElem.value = var;
            } else {
              // We will rely on mapElem.value==formal to replace it away
              // in finalizeCopy().  This assumes a single set of varargs.
              pci->varargOldFormal = formal;
              pci->varargNewFormals = varargFormals;
            }

            break;
          }
        }
        // If !gotFormal, still need to compute needTupleInBody.
        // What to pass for 'formal' argument? Or maybe doesn't matter?
        INT_ASSERT(gotFormal);
      } else {
        needTupleInBody = needVarArgTupleAsWhole(workingFn->body, n, formal);
      }

      if (formal->hasFlag(FLAG_TYPE_VARIABLE)) {
        var->addFlag(FLAG_TYPE_VARIABLE);
      }

      // This is needed regardless of needTupleInBody...
      if (formal->intent == INTENT_OUT || formal->intent == INTENT_INOUT) {
        // ... however, we need temporaries even if !needTupleInBody.
        // Creating one temporary per formal is left as future work.
        // A challenge is whether varargFormals should contain these
        // per-formal temporaries instead of the formals, in this case.
        needTupleInBody = true;

        int i = 0;

        for_actuals(actual, tupleCall) {
          // Skip the tuple count
          if (i > 0) {
            VarSymbol* tmp    = newTemp("_varargs_tmp_");

            CallExpr*  elem   = new CallExpr(var, new_IntSymbol(i));
            CallExpr*  move   = new CallExpr(PRIM_MOVE, tmp,            elem);

            CallExpr*  assign = new CallExpr("=",       actual->copy(), tmp);

            workingFn->insertIntoEpilogue(new DefExpr(tmp));
            workingFn->insertIntoEpilogue(move);
            workingFn->insertIntoEpilogue(assign);
          }

          i++;
        }
      }

      if (needTupleInBody) {

        // MDN 2016/02/01. Enable special handling for strings with blank intent

        // If the original formal is a string with blank intent then the
        // new formals appear to be strings with blank intent.  However
        // the resolver is about to update these to be const ref intent.
        //
        // Currently this will cause the tuple to appear to be a tuple
        // ref(String) which is not supported.
        //
        // Insert local copies that can be used for the tuple

        if (isString(formal->type) && formal->intent == INTENT_BLANK) {
          DefExpr* varDefn = new DefExpr(var);
          Expr*    tail    = NULL;

          // Insert the new locals
          for (int i = 0; i < n; i++) {
            const char* localName = astr("_e", istr(i + n), "_", formal->name);
            VarSymbol*  local     = newTemp(localName, formal->type);
            DefExpr*    defn      = new DefExpr(local);

            if (tail == NULL)
              workingFn->insertAtHead(defn);
            else
              tail->insertAfter(defn);

            tail = defn;
          }

          // Insert the definition for the tuple
          tail->insertAfter(varDefn);
          tail = varDefn;

          // Insert the copies from the blank-intent strings to the locals
          // Update the arguments to the tuple call
          for (int i = 1; i <= n; i++) {
            DefExpr*  localDefn = toDefExpr(workingFn->body->body.get(i));
            Symbol*   local     = localDefn->sym;
            SymExpr*  localExpr = new SymExpr(local);

            SymExpr*  tupleArg  = toSymExpr(tupleCall->get(i + 1));
            SymExpr*  copyArg   = tupleArg->copy();

            CallExpr* moveExpr  = new CallExpr(PRIM_MOVE, localExpr, copyArg);

            tupleArg->setSymbol(local);

            tail->insertAfter(moveExpr);
            tail = moveExpr;
          }

          tail->insertAfter(new CallExpr(PRIM_MOVE, var, tupleCall));
        } else {
          workingFn->insertAtHead(new CallExpr(PRIM_MOVE, var, tupleCall));
          workingFn->insertAtHead(new DefExpr(var));
        }

        if (PartialCopyData* pci = getPartialCopyInfo(workingFn)) {
          // If this is a partial copy, store the mapping for substitution later.
          pci->partialCopyMap.put(formal, var);
        } else {
          // Otherwise, do the substitution now.
          subSymbol(workingFn->body, formal, var);
        }
      } else {
        // !needTupleInBody
        substituteVarargTupleRefs(workingFn->body, n, formal, varargFormals);
      }

      if (workingFn->where) {
        if (needVarArgTupleAsWhole(workingFn->where, n, formal)) {
          VarSymbol* var  = new VarSymbol(formal->name);
          CallExpr*  move = new CallExpr(PRIM_MOVE, var, tupleCall->copy());

          if (formal->hasFlag(FLAG_TYPE_VARIABLE)) {
            var->addFlag(FLAG_TYPE_VARIABLE);
          }

          workingFn->where->insertAtHead(move);
          workingFn->where->insertAtHead(new DefExpr(var));

          subSymbol(workingFn->where, formal, var);
        } else {
          // !needVarArgTupleAsWhole()
          substituteVarargTupleRefs(workingFn->where, n, formal, varargFormals);
        }
      }

      formal->defPoint->remove();
    } else {
      // Just documenting the current status.
      INT_FATAL(formal, "unexpected non-VarSymbol");
    }
  }
}


FnSymbol*
expandVarArgs(FnSymbol* origFn, int numActuals) {

  bool      genericArgSeen = false;
  FnSymbol* workingFn      = origFn;

  SymbolMap substitutions;

  static Map<FnSymbol*,Vec<FnSymbol*>*> cache;

  // check for cached stamped out function
  if (Vec<FnSymbol*>* cfns = cache.get(origFn)) {
    forv_Vec(FnSymbol, cfn, *cfns) {
      if (cfn->numFormals() == numActuals) return cfn;
    }
  }

  for_formals(formal, origFn) {

    if (workingFn != origFn) {
      formal = toArgSymbol(substitutions.get(formal));
    }

    if (!genericArgSeen && formal->variableExpr && !isDefExpr(formal->variableExpr->body.tail)) {
      resolveBlockStmt(formal->variableExpr);
    }

    /*
     * Set genericArgSeen to true if a generic argument appears before the
     * argument with the variable expression.
     */

    // INT_ASSERT(arg->type);
    // Adding 'ref' intent to the "ret" arg of
    //  inline proc =(ref ret:syserr, x:syserr) { __primitive("=", ret, x); }
    // in SysBasic.chpl:150 causes a segfault.
    // The addition of the arg->type test in the following conditional is a
    // workaround.
    // A better approach would be to add a check that each formal of a function
    // has a type (if that can be expected) and then fix the fault where it occurs.
    if (formal->type && formal->type->symbol->hasFlag(FLAG_GENERIC)) {
      genericArgSeen = true;
    }

    if (!formal->variableExpr) {
      continue;
    }

    // Handle unspecified variable number of arguments.
    if (DefExpr* def = toDefExpr(formal->variableExpr->body.tail)) {
      // This assumes a single set of varargs.
      int numCopies = numActuals - workingFn->numFormals() + 1;
      if (numCopies <= 0) {
        if (workingFn != origFn) delete workingFn;
        return NULL;
      }

      if (workingFn == origFn) {
        workingFn = origFn->copy(&substitutions);
        INT_ASSERT(! workingFn->hasFlag(FLAG_RESOLVED));
        workingFn->addFlag(FLAG_INVISIBLE_FN);

        origFn->defPoint->insertBefore(new DefExpr(workingFn));

        formal = static_cast<ArgSymbol*>(substitutions.get(formal));
      }

      // newSym queries the number of varargs. Replace it with int literal.
      Symbol*  newSym     = substitutions.get(def->sym);
      SymExpr* newSymExpr = new SymExpr(new_IntSymbol(numCopies));
      newSymExpr->astloc = newSym->astloc;
      newSym->defPoint->replace(newSymExpr);

      subSymbol(workingFn, newSym, new_IntSymbol(numCopies));

      handleSymExprInExpandVarArgs(workingFn, formal, newSymExpr);
      genericArgSeen = false;

    } else if (SymExpr* sym = toSymExpr(formal->variableExpr->body.tail)) {

      handleSymExprInExpandVarArgs(workingFn, formal, sym);

    } else if (!workingFn->hasFlag(FLAG_GENERIC)) {
      INT_FATAL("bad variableExpr");
    }
  }

  Vec<FnSymbol*>* cfns = cache.get(origFn);
  if (cfns == NULL) {
    cfns = new Vec<FnSymbol*>();
  }
  cfns->add(workingFn);
  cache.put(origFn, cfns);

  return workingFn;
}


static void
resolve_type_constructor(FnSymbol* fn, CallInfo& info) {
    SET_LINENO(fn);

    // Don't bother with tuple constructors since we generated
    // them along with their type constructors.
    if( fn->hasFlag(FLAG_PARTIAL_TUPLE) )
      return;

    CallExpr* typeConstructorCall = new CallExpr(astr("_type", fn->name));
    for_formals(formal, fn) {
      if (!formal->hasFlag(FLAG_IS_MEME)) {
        if (fn->_this->type->symbol->hasFlag(FLAG_TUPLE)) {
          if (formal->instantiatedFrom) {
            typeConstructorCall->insertAtTail(formal->type->symbol);
          } else if (formal->hasFlag(FLAG_INSTANTIATED_PARAM)) {
            typeConstructorCall->insertAtTail(paramMap.get(formal));
          }
        } else {
          if (!strcmp(formal->name, "outer") || formal->type == dtMethodToken) {
            typeConstructorCall->insertAtTail(formal);
          } else if (formal->instantiatedFrom) {
            typeConstructorCall->insertAtTail(new NamedExpr(formal->name, new SymExpr(formal->type->symbol)));
          } else if (formal->hasFlag(FLAG_INSTANTIATED_PARAM)) {
            typeConstructorCall->insertAtTail(new NamedExpr(formal->name, new SymExpr(paramMap.get(formal))));
          }
        }
      }
    }
    info.call->insertBefore(typeConstructorCall);
    // If instead we call resolveCallAndCallee(typeConstructorCall)
    // then the line number reported in an error would change
    // e.g.: domains/deitz/test_generic_class_of_sparse_domain
    // or:   classes/diten/multipledestructor
    resolveCall(typeConstructorCall);
    INT_ASSERT(typeConstructorCall->isResolved());
    resolveFns(typeConstructorCall->isResolved());
    fn->_this->type = typeConstructorCall->isResolved()->retType;
    typeConstructorCall->remove();
}


/** Candidate filtering logic specific to concrete functions.
 *
 * \param candidates    The list to add possible candidates to.
 * \param currCandidate The current candidate to consider.
 * \param info          The CallInfo object for the call site.
 */
static void
filterConcreteCandidate(Vec<ResolutionCandidate*>& candidates,
                        ResolutionCandidate* currCandidate,
                        CallInfo& info) {

  currCandidate->fn = expandVarArgs(currCandidate->fn, info.actuals.n);

  if (!currCandidate->fn) return;

  resolveTypedefedArgTypes(currCandidate->fn);

  if (!currCandidate->computeAlignment(info)) {
    return;
  }

  /*
   * Make sure that type constructor is resolved before other constructors.
   */
  if (currCandidate->fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR)) {
    resolve_type_constructor(currCandidate->fn, info);
  }

  // We should reject this candidate if any of the situations handled by this
  // function are violated.
  if (checkResolveFormalsWhereClauses(currCandidate) == false)
    return;

  candidates.add(currCandidate);
}

// Verifies the usage of this candidate function
bool checkResolveFormalsWhereClauses(ResolutionCandidate* currCandidate) {
  /*
   * A derived generic type will use the type of its parent, and expects this to
   * be instantiated before it is.
   */
  resolveFormals(currCandidate->fn);

  int coindex = -1;
  for_formals(formal, currCandidate->fn) {
    if (Symbol* actual = currCandidate->formalIdxToActual.v[++coindex]) {
      if (actual->hasFlag(FLAG_TYPE_VARIABLE) != formal->hasFlag(FLAG_TYPE_VARIABLE)) {
        return false;
      }

      bool formalIsParam = formal->hasFlag(FLAG_INSTANTIATED_PARAM) || formal->intent == INTENT_PARAM;
      if (!canDispatch(actual->type, actual, formal->type, currCandidate->fn, NULL, formalIsParam)) {
        return false;
      }
    }
  }

  //
  // Evaluate where clauses
  //
  if (!evaluateWhereClause(currCandidate->fn)) {
    return false;
  }
  return true;
}


/** Candidate filtering logic specific to generic functions.
 *
 * \param candidates    The list to add possible candidates to.
 * \param currCandidate The current candidate to consider.
 * \param info          The CallInfo object for the call site.
 */
static void
filterGenericCandidate(Vec<ResolutionCandidate*>& candidates,
                       ResolutionCandidate* currCandidate,
                       CallInfo& info) {

  currCandidate->fn = expandVarArgs(currCandidate->fn, info.actuals.n);

  if (!currCandidate->fn) return;

  if (!currCandidate->computeAlignment(info)) {
    return;
  }

  if (checkGenericFormals(currCandidate) == false)
    return;


  // Compute the param/type substitutions for generic arguments.
  currCandidate->computeSubstitutions();

  /*
   * If no substitutions were made we can't instantiate this generic, and must
   * reject it.
   */
  if (currCandidate->substitutions.n > 0) {
    /*
     * Instantiate just enough of the generic to get through the rest of the
     * filtering and disambiguation processes.
     */
    currCandidate->fn = instantiateSignature(currCandidate->fn, currCandidate->substitutions, info.call);

    if (currCandidate->fn != NULL) {
      filterCandidate(candidates, currCandidate, info);
    }
  }
}

// Verify that the generic formals are matched correctly
bool checkGenericFormals(ResolutionCandidate* currCandidate) {
  /*
   * Early rejection of generic functions.
   */
  int coindex = 0;
  for_formals(formal, currCandidate->fn) {
    if (formal->type != dtUnknown) {
      if (Symbol* actual = currCandidate->formalIdxToActual.v[coindex]) {
        if (actual->hasFlag(FLAG_TYPE_VARIABLE) != formal->hasFlag(FLAG_TYPE_VARIABLE)) {
          return false;
        }

        if (formal->type->symbol->hasFlag(FLAG_GENERIC)) {
          Type* vt = actual->getValType();
          Type* st = actual->type->scalarPromotionType;
          Type* svt = (vt) ? vt->scalarPromotionType : NULL;
          if (!canInstantiate(actual->type, formal->type) &&
              (!vt  || !canInstantiate(vt, formal->type)) &&
              (!st  || !canInstantiate(st, formal->type)) &&
              (!svt || !canInstantiate(svt, formal->type))) {

            return false;

          }
        } else {
          bool formalIsParam = formal->hasFlag(FLAG_INSTANTIATED_PARAM) || formal->intent == INTENT_PARAM;
          if (!canDispatch(actual->type, actual, formal->type, currCandidate->fn, NULL, formalIsParam)) {
            return false;
          }
        }
      }
    }
    ++coindex;
  }
  return true;
}


/** Tests to see if a function is a candidate for resolving a specific call.  If
 *  it is a candidate, we add it to the candidate lists.
 *
 * This version of filterCandidate is called by other versions of
 * filterCandidate, and shouldn't be called outside this family of functions.
 *
 * \param candidates    The list to add possible candidates to.
 * \param currCandidate The current candidate to consider.
 * \param info          The CallInfo object for the call site.
 */
static void
filterCandidate(Vec<ResolutionCandidate*>& candidates,
                ResolutionCandidate* currCandidate,
                CallInfo& info) {

  if (currCandidate->fn->hasFlag(FLAG_GENERIC)) {
    filterGenericCandidate(candidates, currCandidate, info);

  } else {
    filterConcreteCandidate(candidates, currCandidate, info);
  }
}


/** Tests to see if a function is a candidate for resolving a specific call.  If
 *  it is a candidate, we add it to the candidate lists.
 *
 * This version of filterCandidate is called by code outside the filterCandidate
 * family of functions.
 *
 * \param candidates    The list to add possible candidates to.
 * \param currCandidate The current candidate to consider.
 * \param info          The CallInfo object for the call site.
 */
static void
filterCandidate(Vec<ResolutionCandidate*>& candidates, FnSymbol* fn, CallInfo& info) {
  ResolutionCandidate* currCandidate = new ResolutionCandidate(fn);
  filterCandidate(candidates, currCandidate, info);

  if (candidates.tail() != currCandidate) {
    // The candidate was not accepted.  Time to clean it up.
    delete currCandidate;
  }
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
  if (block && block->modUses) {
    for_actuals(expr, block->modUses) {
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


//
// This is a utility function that essentially tracks which function,
// if any, the param arguments prefer.
//
static inline void registerParamPreference(int& paramPrefers, int preference,
                                    const char* argstr,
                                    DisambiguationContext DC) {

  if (paramPrefers == 0 || paramPrefers == preference) {
    /* if the param currently has no preference or it matches the new
       preference, preserve the current preference */
    paramPrefers = preference;
    TRACE_DISAMBIGUATE_BY_MATCH("param prefers %s\n", argstr);
  } else {
    /* otherwise its preference contradicts the previous arguments, so
       mark it as not preferring either */
    paramPrefers = -1;
    TRACE_DISAMBIGUATE_BY_MATCH("param prefers differing things\n");
  }
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
static void testArgMapping(FnSymbol* fn1, ArgSymbol* formal1,
                           FnSymbol* fn2, ArgSymbol* formal2,
                           Symbol* actual,
                           const DisambiguationContext& DC,
                           DisambiguationState& DS) {

  // We only want to deal with the value types here, avoiding odd overloads
  // working (or not) due to _ref.
  Type *f1Type = formal1->type->getValType();
  Type *f2Type = formal2->type->getValType();
  Type *actualType = actual->type->getValType();

  TRACE_DISAMBIGUATE_BY_MATCH("Actual's type: %s\n", toString(actualType));

  bool formal1Promotes = false;
  canDispatch(actualType, actual, f1Type, fn1, &formal1Promotes);
  DS.fn1Promotes |= formal1Promotes;

  TRACE_DISAMBIGUATE_BY_MATCH("Formal 1's type: %s\n", toString(f1Type));
  if (formal1Promotes) {
    TRACE_DISAMBIGUATE_BY_MATCH("Actual requires promotion to match formal 1\n");
  } else {
    TRACE_DISAMBIGUATE_BY_MATCH("Actual DOES NOT require promotion to match formal 1\n");
  }

  if (formal1->hasFlag(FLAG_INSTANTIATED_PARAM)) {
    TRACE_DISAMBIGUATE_BY_MATCH("Formal 1 is an instantiated param.\n");
  } else {
    TRACE_DISAMBIGUATE_BY_MATCH("Formal 1 is NOT an instantiated param.\n");
  }

  bool formal2Promotes = false;
  canDispatch(actualType, actual, f2Type, fn1, &formal2Promotes);
  DS.fn2Promotes |= formal2Promotes;

  TRACE_DISAMBIGUATE_BY_MATCH("Formal 2's type: %s\n", toString(f2Type));
  if (formal2Promotes) {
    TRACE_DISAMBIGUATE_BY_MATCH("Actual requires promotion to match formal 2\n");
  } else {
    TRACE_DISAMBIGUATE_BY_MATCH("Actual DOES NOT require promotion to match formal 2\n");
  }

  if (formal2->hasFlag(FLAG_INSTANTIATED_PARAM)) {
    TRACE_DISAMBIGUATE_BY_MATCH("Formal 2 is an instantiated param.\n");
  } else {
    TRACE_DISAMBIGUATE_BY_MATCH("Formal 2 is NOT an instantiated param.\n");
  }

  if (f1Type == f2Type && formal1->hasFlag(FLAG_INSTANTIATED_PARAM) && !formal2->hasFlag(FLAG_INSTANTIATED_PARAM)) {
    TRACE_DISAMBIGUATE_BY_MATCH("A: Fn %d is more specific\n", DC.i);
    DS.fn1MoreSpecific = true;

  } else if (f1Type == f2Type && !formal1->hasFlag(FLAG_INSTANTIATED_PARAM) && formal2->hasFlag(FLAG_INSTANTIATED_PARAM)) {
    TRACE_DISAMBIGUATE_BY_MATCH("B: Fn %d is more specific\n", DC.j);
    DS.fn2MoreSpecific = true;

  } else if (!formal1Promotes && formal2Promotes) {
    TRACE_DISAMBIGUATE_BY_MATCH("C: Fn %d is more specific\n", DC.i);
    DS.fn1MoreSpecific = true;

  } else if (formal1Promotes && !formal2Promotes) {
    TRACE_DISAMBIGUATE_BY_MATCH("D: Fn %d is more specific\n", DC.j);
    DS.fn2MoreSpecific = true;

  } else if (f1Type == f2Type && !formal1->instantiatedFrom && formal2->instantiatedFrom) {
    TRACE_DISAMBIGUATE_BY_MATCH("E: Fn %d is more specific\n", DC.i);
    DS.fn1MoreSpecific = true;

  } else if (f1Type == f2Type && formal1->instantiatedFrom && !formal2->instantiatedFrom) {
    TRACE_DISAMBIGUATE_BY_MATCH("F: Fn %d is more specific\n", DC.j);
    DS.fn2MoreSpecific = true;

  } else if (formal1->instantiatedFrom != dtAny && formal2->instantiatedFrom == dtAny) {
    TRACE_DISAMBIGUATE_BY_MATCH("G: Fn %d is more specific\n", DC.i);
    DS.fn1MoreSpecific = true;

  } else if (formal1->instantiatedFrom == dtAny && formal2->instantiatedFrom != dtAny) {
    TRACE_DISAMBIGUATE_BY_MATCH("H: Fn %d is more specific\n", DC.j);
    DS.fn2MoreSpecific = true;

  } else if (considerParamMatches(actualType, f1Type, f2Type)) {
    TRACE_DISAMBIGUATE_BY_MATCH("In first param case\n");
    // The actual matches formal1's type, but not formal2's
    if (paramWorks(actual, f2Type)) {
      // but the actual is a param and works for formal2
      if (formal1->hasFlag(FLAG_INSTANTIATED_PARAM)) {
        // the param works equally well for both, but
        // matches the first slightly better if we had to
        // decide
        registerParamPreference(DS.paramPrefers, 1, "formal1", DC);
      } else if (formal2->hasFlag(FLAG_INSTANTIATED_PARAM)) {
        registerParamPreference(DS.paramPrefers, 2, "formal2", DC);
      } else {
        // neither is a param, but formal1 is an exact type
        // match, so prefer that one
        registerParamPreference(DS.paramPrefers, 1, "formal1", DC);
      }
    } else {
      TRACE_DISAMBIGUATE_BY_MATCH("I: Fn %d is more specific\n", DC.i);
      DS.fn1MoreSpecific = true;
    }
  } else if (considerParamMatches(actualType, f2Type, f1Type)) {
    TRACE_DISAMBIGUATE_BY_MATCH("In second param case\n");
    // The actual matches formal2's type, but not formal1's
    if (paramWorks(actual, f1Type)) {
      // but the actual is a param and works for formal1
      if (formal2->hasFlag(FLAG_INSTANTIATED_PARAM)) {
        // the param works equally well for both, but
        // matches the second slightly better if we had to
        // decide
        registerParamPreference(DS.paramPrefers, 2, "formal2", DC);
      } else if (formal1->hasFlag(FLAG_INSTANTIATED_PARAM)) {
        registerParamPreference(DS.paramPrefers, 1, "formal1", DC);
      } else {
        // neither is a param, but formal1 is an exact type
        // match, so prefer that one
        registerParamPreference(DS.paramPrefers, 2, "formal2", DC);
      }
    } else {
      TRACE_DISAMBIGUATE_BY_MATCH("J: Fn %d is more specific\n", DC.j);
      DS.fn2MoreSpecific = true;
    }
  } else if (moreSpecific(fn1, f1Type, f2Type) && f2Type != f1Type) {
    TRACE_DISAMBIGUATE_BY_MATCH("K: Fn %d is more specific\n", DC.i);
    DS.fn1MoreSpecific = true;

  } else if (moreSpecific(fn1, f2Type, f1Type) && f2Type != f1Type) {
    TRACE_DISAMBIGUATE_BY_MATCH("L: Fn %d is more specific\n", DC.j);
    DS.fn2MoreSpecific = true;

  } else if (is_int_type(f1Type) && is_uint_type(f2Type)) {
    TRACE_DISAMBIGUATE_BY_MATCH("M: Fn %d is more specific\n", DC.i);
    DS.fn1MoreSpecific = true;

  } else if (is_int_type(f2Type) && is_uint_type(f1Type)) {
    TRACE_DISAMBIGUATE_BY_MATCH("N: Fn %d is more specific\n", DC.j);
    DS.fn2MoreSpecific = true;

  } else {
    TRACE_DISAMBIGUATE_BY_MATCH("O: no information gained from argument\n");
  }
}


/** Determines if fn1 is a better match than fn2.
 *
 * This function implements the function comparison component of the
 * disambiguation procedure as detailed in section 13.14.3 of the Chapel
 * language specification (page 106).
 *
 * \param candidate1 The function on the left-hand side of the comparison.
 * \param candidate2 The function on the right-hand side of the comparison.
 * \param DC         The disambiguation context.
 *
 * \return True if fn1 is a more specific function than f2, false otherwise.
 */
bool isBetterMatch(ResolutionCandidate* candidate1,
                          ResolutionCandidate* candidate2,
                          const DisambiguationContext& DC,
                          bool onlyConsiderPromotion=false) {

  DisambiguationState DS;

  for (int k = 0; k < candidate1->actualIdxToFormal.n; ++k) {
    Symbol* actual = DC.actuals->v[k];
    ArgSymbol* formal1 = candidate1->actualIdxToFormal.v[k];
    ArgSymbol* formal2 = candidate2->actualIdxToFormal.v[k];

    TRACE_DISAMBIGUATE_BY_MATCH("\nLooking at argument %d\n", k);

    testArgMapping(candidate1->fn, formal1, candidate2->fn, formal2, actual, DC, DS);
  }

  if (!DS.fn1Promotes && DS.fn2Promotes) {
    TRACE_DISAMBIGUATE_BY_MATCH("\nP: Fn %d does not require argument promotion; Fn %d does\n", DC.i, DC.j);
    DS.printSummary("P", DC);
    return true;
  }

  if (!onlyConsiderPromotion && !(DS.fn1MoreSpecific || DS.fn2MoreSpecific)) {
    // If the decision hasn't been made based on the argument mappings...

    if (isMoreVisible(DC.scope, candidate1->fn, candidate2->fn)) {
      TRACE_DISAMBIGUATE_BY_MATCH("\nQ: Fn %d is more specific\n", DC.i);
      DS.fn1MoreSpecific = true;

    } else if (isMoreVisible(DC.scope, candidate2->fn, candidate1->fn)) {
      TRACE_DISAMBIGUATE_BY_MATCH("\nR: Fn %d is more specific\n", DC.j);
      DS.fn2MoreSpecific = true;

    } else if (DS.paramPrefers == 1) {
      TRACE_DISAMBIGUATE_BY_MATCH("\nS: Fn %d is more specific\n", DC.i);
      DS.fn1MoreSpecific = true;

    } else if (DS.paramPrefers == 2) {
      TRACE_DISAMBIGUATE_BY_MATCH("\nT: Fn %d is more specific\n", DC.j);
      DS.fn2MoreSpecific = true;

    } else if (candidate1->fn->where && !candidate2->fn->where) {
      TRACE_DISAMBIGUATE_BY_MATCH("\nU: Fn %d is more specific\n", DC.i);
      DS.fn1MoreSpecific = true;

    } else if (!candidate1->fn->where && candidate2->fn->where) {
      TRACE_DISAMBIGUATE_BY_MATCH("\nV: Fn %d is more specific\n", DC.j);
      DS.fn2MoreSpecific = true;
    }
  }

  DS.printSummary("W", DC);
  return DS.fn1MoreSpecific && !DS.fn2MoreSpecific;
}

/** Find the best candidate from a list of candidates.
 *
 * This function finds the best Chapel function from a set of candidates, given
 * a call site.  This is an implementation of 13.14.3 of the Chapel language
 * specification (page 106).
 *
 * \param candidates A list of the candidate functions, from which the best
 *                   match is selected.
 * \param DC         The disambiguation context.
 *
 * \return The result of the disambiguation process.
 */
ResolutionCandidate*
disambiguateByMatch(Vec<ResolutionCandidate*>& candidates,
                    DisambiguationContext DC,
                    disambiguate_kind_t kind) {

  // If index i is set then we can skip testing function F_i because we already
  // know it can not be the best match.
  std::vector<bool> notBest(candidates.n, false);

  for (int i = 0; i < candidates.n; ++i) {

    TRACE_DISAMBIGUATE_BY_MATCH("##########################\n");
    TRACE_DISAMBIGUATE_BY_MATCH("# Considering function %d #\n", i);
    TRACE_DISAMBIGUATE_BY_MATCH("##########################\n\n");

    ResolutionCandidate* candidate1 = candidates.v[i];
    bool best = true; // is fn1 the best candidate?

    // Only consider ref return fns in ref return part
    if (kind == FIND_REF && (candidate1->fn->retTag != RET_REF)) continue;
    if (kind == FIND_CONST_REF && (candidate1->fn->retTag != RET_CONST_REF)) continue;
    if (kind == FIND_NOT_REF_OR_CONST_REF && (candidate1->fn->retTag == RET_REF || candidate1->fn->retTag == RET_CONST_REF)) continue;

    TRACE_DISAMBIGUATE_BY_MATCH("%s\n\n", toString(candidate1->fn));

    if (notBest[i]) {
      TRACE_DISAMBIGUATE_BY_MATCH("Already known to not be best match.  Skipping.\n\n");
      continue;
    }

    for (int j = 0; j < candidates.n; ++j) {
      if (i == j) continue;

      TRACE_DISAMBIGUATE_BY_MATCH("Comparing to function %d\n", j);
      TRACE_DISAMBIGUATE_BY_MATCH("-----------------------\n");

      ResolutionCandidate* candidate2 = candidates.v[j];

      // Only consider ref return fns in ref return part
      if (kind == FIND_REF && (candidate2->fn->retTag != RET_REF)) continue;
      if (kind == FIND_CONST_REF && (candidate2->fn->retTag != RET_CONST_REF)) continue;
      if (kind == FIND_NOT_REF_OR_CONST_REF && (candidate2->fn->retTag == RET_REF || candidate2->fn->retTag == RET_CONST_REF)) continue;

      TRACE_DISAMBIGUATE_BY_MATCH("%s\n", toString(candidate2->fn));

      if (isBetterMatch(candidate1, candidate2, DC.forPair(i, j))) {
        TRACE_DISAMBIGUATE_BY_MATCH("X: Fn %d is a better match than Fn %d\n\n\n", i, j);
        notBest[j] = true;

      } else {
        TRACE_DISAMBIGUATE_BY_MATCH("X: Fn %d is NOT a better match than Fn %d\n\n\n", i, j);
        best = false;
        break;
      }
    }

    if (best) {
      TRACE_DISAMBIGUATE_BY_MATCH("Y: Fn %d is the best match.\n\n\n", i);
      return candidate1;

    } else {
      TRACE_DISAMBIGUATE_BY_MATCH("Y: Fn %d is NOT the best match.\n\n\n", i);
    }
  }

  TRACE_DISAMBIGUATE_BY_MATCH("Z: No non-ambiguous best match.\n\n");

  return NULL;
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


void
printResolutionErrorAmbiguous(Vec<FnSymbol*>& candidates, CallInfo* info) {
  CallExpr* call = userCall(info->call);
  if (!strcmp("this", info->name)) {
    USR_FATAL(call, "ambiguous access of '%s' by '%s'",
              toString(info->actuals.v[1]->type),
              toString(info));
  } else {
    const char* entity = "call";
    if (!strncmp("_type_construct_", info->name, 16))
      entity = "type specifier";
    const char* str = toString(info);
    if (info->scope) {
      ModuleSymbol* mod = toModuleSymbol(info->scope->parentSymbol);
      INT_ASSERT(mod);
      str = astr(mod->name, ".", str);
    }
    USR_FATAL_CONT(call, "ambiguous %s '%s'", entity, str);
    if (developer) {
      for (int i = callStack.n-1; i>=0; i--) {
        CallExpr* cs = callStack.v[i];
        FnSymbol* f = cs->getFunction();
        if (f->instantiatedFrom)
          USR_PRINT(callStack.v[i], "  instantiated from %s", f->name);
        else
          break;
      }
    }
    bool printed_one = false;
    forv_Vec(FnSymbol, fn, candidates) {
      USR_PRINT(fn, "%s %s",
                printed_one ? "               " : "candidates are:",
                toString(fn));
      printed_one = true;
    }
    USR_STOP();
  }
}

void
printResolutionErrorUnresolved(Vec<FnSymbol*>& visibleFns, CallInfo* info) {
  if( ! info ) INT_FATAL("CallInfo is NULL");
  if( ! info->call ) INT_FATAL("call is NULL");
  bool needToReport = false;
  CallExpr* call = userCall(info->call);

  if (call->isCast()) {
    if (!info->actuals.head()->hasFlag(FLAG_TYPE_VARIABLE)) {
      USR_FATAL_CONT(call, "illegal cast to non-type");
    } else {
      USR_FATAL_CONT(call, "illegal cast from %s to %s",
                     toString(info->actuals.v[1]->type),
                     toString(info->actuals.v[0]->type));
    }
  } else if (!strcmp("free", info->name)) {
    if (info->actuals.n > 0 &&
        isRecord(info->actuals.v[2]->type))
      USR_FATAL_CONT(call, "delete not allowed on records");
    else
      needToReport = true;
  } else if (!strcmp("these", info->name)) {
    if (info->actuals.n == 2 &&
        info->actuals.v[0]->type == dtMethodToken) {
      if (info->actuals.v[1]->hasFlag(FLAG_TYPE_VARIABLE)) {
        USR_FATAL_CONT(call, "unable to iterate over type '%s'",
                       toString(info->actuals.v[1]->type));
      } else {
        USR_FATAL_CONT(call, "cannot iterate over values of type %s",
                       toString(info->actuals.v[1]->type));
      }
    } else {
      needToReport = true;
    }
  } else if (!strcmp("_type_construct__tuple", info->name)) {
    if (info->call->argList.length == 0)
      USR_FATAL_CONT(call, "tuple size must be specified");
    SymExpr* sym = toSymExpr(info->call->get(1));
    if (!sym || !sym->symbol()->isParameter()) {
      USR_FATAL_CONT(call, "tuple size must be static");
    } else {
      USR_FATAL_CONT(call, "invalid tuple");
    }
  } else if (!strcmp("=", info->name)) {
    if (info->actuals.v[0] && !info->actuals.v[0]->hasFlag(FLAG_TYPE_VARIABLE) &&
        info->actuals.v[1] && info->actuals.v[1]->hasFlag(FLAG_TYPE_VARIABLE)) {
      USR_FATAL_CONT(call, "illegal assignment of type to value");
    } else if (info->actuals.v[0] && info->actuals.v[0]->hasFlag(FLAG_TYPE_VARIABLE) &&
               info->actuals.v[1] && !info->actuals.v[1]->hasFlag(FLAG_TYPE_VARIABLE)) {
      USR_FATAL_CONT(call, "illegal assignment of value to type");
    } else if (info->actuals.v[1]->type == dtNil) {
      USR_FATAL_CONT(call, "type mismatch in assignment from nil to %s",
                toString(info->actuals.v[0]->type));
    } else {
      USR_FATAL_CONT(call, "type mismatch in assignment from %s to %s",
                     toString(info->actuals.v[1]->type),
                     toString(info->actuals.v[0]->type));
    }
  } else if (!strcmp("this", info->name)) {
    Type* type = info->actuals.v[1]->getValType();
    if (type->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
      USR_FATAL_CONT(call, "illegal access of iterator or promoted expression");
    } else if (type->symbol->hasFlag(FLAG_FUNCTION_CLASS)) {
      USR_FATAL_CONT(call, "illegal access of first class function");
    } else {
      USR_FATAL_CONT(call, "unresolved access of '%s' by '%s'",
                     toString(info->actuals.v[1]->type),
                     toString(info));
    }
  } else {
    needToReport = true;
  }
  // It would be easier to just check exit_eventually to catch all needToReport cases.
  // Alas exit_eventually is static to misc.cpp.
  if (needToReport) {
    const char* entity = "call";
    if (!strncmp("_type_construct_", info->name, 16))
      entity = "type specifier";
    const char* str = toString(info);
    if (info->scope) {
      ModuleSymbol* mod = toModuleSymbol(info->scope->parentSymbol);
      INT_ASSERT(mod);
      str = astr(mod->name, ".", str);
    }
    if(info->actuals.n > 1 && ((info->actuals.v[0]->getValType()) == dtMethodToken)){
      EnumType* typeE = toEnumType(info->actuals.v[1]->getValType());
      if (typeE != NULL) {
        entity = "enumerated type symbol or call";
      }
    }
    USR_FATAL_CONT(call, "unresolved %s '%s'", entity, str);
    if (visibleFns.n > 0) {
      if (developer) {
        for (int i = callStack.n-1; i>=0; i--) {
          CallExpr* cs = callStack.v[i];
          FnSymbol* f = cs->getFunction();
          if (f->instantiatedFrom)
            USR_PRINT(callStack.v[i], "  instantiated from %s", f->name);
          else
            break;
        }
      }
      bool printed_one = false;
      forv_Vec(FnSymbol, fn, visibleFns) {
        // Consider "visible functions are"
        USR_PRINT(fn, "%s %s",
                  printed_one ? "               " : "candidates are:",
                  toString(fn));
        printed_one = true;
      }
    }
    if (visibleFns.n == 1 &&
        visibleFns.v[0]->numFormals() == 0
        && !strncmp("_type_construct_", info->name, 16))
      USR_PRINT(call, "did you forget the 'new' keyword?");
  }
  if( developer ) {
    // Should this be controlled another way?
    USR_PRINT(call, "unresolved call had id %i", call->id);
  }
  USR_STOP();
}

static void issueCompilerError(CallExpr* call) {
  //
  // Disable compiler warnings in internal modules that are triggered
  // within a dynamic dispatch context because of potential user
  // confusion.  Removed the following code and See the following
  // tests:
  //
  //   test/arrays/bradc/workarounds/arrayOfSpsArray.chpl
  //   test/arrays/deitz/part4/test_array_of_associative_arrays.chpl
  //   test/classes/bradc/arrayInClass/genericArrayInClass-otharrs.chpl
  //
  if (call->isPrimitive(PRIM_WARNING))
    if (inDynamicDispatchResolution)
      if (call->getModule()->modTag == MOD_INTERNAL &&
          callStack.head()->getModule()->modTag == MOD_INTERNAL)
        return;
  //
  // If an errorDepth was specified, report a diagnostic about the call
  // that deep into the callStack. The default depth is 1.
  //
  FnSymbol* fn = toFnSymbol(call->parentSymbol);
  VarSymbol* depthParam = toVarSymbol(paramMap.get(toDefExpr(fn->formals.tail)->sym));
  int64_t depth;
  bool foundDepthVal;
  if (depthParam && depthParam->immediate &&
      depthParam->immediate->const_kind == NUM_KIND_INT) {
    depth = depthParam->immediate->int_value();
    foundDepthVal = true;
  } else {
    depth = 1;
    foundDepthVal = false;
  }
  if (depth > callStack.n - 1) {
    if (foundDepthVal)
      USR_WARN(call, "compiler diagnostic depth value exceeds call stack depth");
    depth = callStack.n - 1;
  }
  if (depth < 0) {
    USR_WARN(call, "compiler diagnostic depth value can not be negative");
    depth = 0;
  }
  CallExpr* from = NULL;
  for (int i = callStack.n-1 - depth; i >= 0; i--) {
    from = callStack.v[i];
    // We report calls whose target function is not compiler-generated and is
    // not defined in one of the internal modules.
    if (from->linenum() > 0 &&
        from->getModule()->modTag != MOD_INTERNAL &&
        !from->getFunction()->hasFlag(FLAG_COMPILER_GENERATED))
      break;
  }

  const char* str = "";
  VarSymbol* var = NULL;
  for_formals(arg, fn) {
    if (foundDepthVal && arg->defPoint == fn->formals.tail)
      continue;
    var = toVarSymbol(paramMap.get(arg));
    INT_ASSERT(var && var->immediate && var->immediate->const_kind == CONST_KIND_STRING);
    str = astr(str, var->immediate->v_string);
  }
  // collapse newlines and other escape sequences before printing
  str = astr(unescapeString(str, var).c_str());
  if (call->isPrimitive(PRIM_ERROR)) {
    USR_FATAL(from, "%s", str);
  } else {
    USR_WARN(from, "%s", str);
  }
  if (FnSymbol* fn = toFnSymbol(callStack.tail()->isResolved()))
    innerCompilerWarningMap.put(fn, str);
  if (FnSymbol* fn = toFnSymbol(callStack.v[callStack.n-1 - depth]->isResolved()))
    outerCompilerWarningMap.put(fn, str);
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
  USR_WARN(from, "%s", str);
}

class VisibleFunctionBlock {
 public:
  Map<const char*,Vec<FnSymbol*>*> visibleFunctions;
  VisibleFunctionBlock() { }
};

static Map<BlockStmt*,VisibleFunctionBlock*> visibleFunctionMap;
static int nVisibleFunctions = 0; // for incremental build
static Map<BlockStmt*,BlockStmt*> visibilityBlockCache;
static Vec<BlockStmt*> standardModuleSet;

//
// return true if expr is a CondStmt with chpl__tryToken as its condition
//
static bool isTryTokenCond(Expr* expr) {
  CondStmt* cond = toCondStmt(expr);
  if (!cond) return false;
  SymExpr* sym = toSymExpr(cond->condExpr);
  if (!sym) return false;
  return sym->symbol() == gTryToken;
}


//
// return the innermost block for searching for visible functions
//
BlockStmt*
getVisibilityBlock(Expr* expr) {
  if (BlockStmt* block = toBlockStmt(expr->parentExpr)) {
    if (block->blockTag == BLOCK_SCOPELESS)
      return getVisibilityBlock(block);
    else if (block->parentExpr && isTryTokenCond(block->parentExpr)) {
      // Make the visibility block of the then and else blocks of a
      // conditional using chpl__tryToken be the block containing the
      // conditional statement.  Without this, there were some cases where
      // a function gets instantiated into one side of the conditional but
      // used in both sides, then the side with the instantiation gets
      // folded out leaving expressions with no visibility block.
      // test/functions/iterators/angeles/dynamic.chpl is an example that
      // currently fails without this.
      return getVisibilityBlock(block->parentExpr);
    } else
      return block;
  } else if (expr->parentExpr) {
    return getVisibilityBlock(expr->parentExpr);
  } else if (Symbol* s = expr->parentSymbol) {
      FnSymbol* fn = toFnSymbol(s);
      if (fn && fn->instantiationPoint)
        return fn->instantiationPoint;
      else
        return getVisibilityBlock(s->defPoint);
  } else {
    INT_FATAL(expr, "Expression has no visibility block.");
    return NULL;
  }
}

static void buildVisibleFunctionMap() {
  for (int i = nVisibleFunctions; i < gFnSymbols.n; i++) {
    FnSymbol* fn = gFnSymbols.v[i];
    if (!fn->hasFlag(FLAG_INVISIBLE_FN) && fn->defPoint->parentSymbol && !isArgSymbol(fn->defPoint->parentSymbol)) {
      BlockStmt* block = NULL;
      if (fn->hasFlag(FLAG_AUTO_II)) {
        block = theProgram->block;
      } else {
        block = getVisibilityBlock(fn->defPoint);
        //
        // add all functions in standard modules to theProgram
        //
        // Lydia NOTE 09/12/16: The computation of the standardModuleSet is not
        // tied to what is actually placed within theProgram->block.  As such
        // there could be bugs where that implementation differs.  We have
        // already encountered some with qualified access to default-included
        // modules like List and Sort.  This implementation needs to be linked
        // to the computation of the standardModuleSet.
        //
        if (standardModuleSet.set_in(block))
          block = theProgram->block;
      }
      VisibleFunctionBlock* vfb = visibleFunctionMap.get(block);
      if (!vfb) {
        vfb = new VisibleFunctionBlock();
        visibleFunctionMap.put(block, vfb);
      }
      Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(fn->name);
      if (!fns) {
        fns = new Vec<FnSymbol*>();
        vfb->visibleFunctions.put(fn->name, fns);
      }
      fns->add(fn);
    }
  }
  nVisibleFunctions = gFnSymbols.n;
}

//
// Collects functions called 'name' visible in 'block' and up the visibility
// chain, which is defined by getVisibilityBlock().
// The functions defined/visible in a block are given by 'visibleFunctionMap'.
//
// 'visibilityBlockCache' maps a block to the nearest block up the visibility
// chain that contains any functions, according to visibleFunctionMap.
// In other words, it skips those blocks that do not define any visible
// functions (of any name).
// Todo: some blocks only define if-functions (i.e. _if_fnNNN); such blocks
// probably should not be present in visibleFunctionMap.
//
// getVisibleFunctions returns the block appropriate for visibilityBlockCache
// or NULL if there is none, e.g. when the next block up is the rootModule.
//
static BlockStmt*
getVisibleFunctions(BlockStmt* block,
                    const char* name,
                    Vec<FnSymbol*>& visibleFns,
                    Vec<BlockStmt*>& visited,
                    CallExpr* callOrigin) {
  //
  // all functions in standard modules are stored in a single block
  //
  // Lydia NOTE 09/12/16: The computation of the standardModuleSet is not
  // tied to what is actually placed within theProgram->block.  As such
  // there could be bugs where that implementation differs.  We have
  // already encountered some with qualified access to default-included
  // modules like List and Sort.  This implementation needs to be linked
  // to the computation of the standardModuleSet.
  //
  if (standardModuleSet.set_in(block))
    block = theProgram->block;

  //
  // avoid infinite recursion due to modules with mutual uses
  //
  if (visited.set_in(block))
    return NULL;

  if (isModuleSymbol(block->parentSymbol))
    visited.set_add(block);

  bool canSkipThisBlock = true;

  VisibleFunctionBlock* vfb = visibleFunctionMap.get(block);
  if (vfb) {
    canSkipThisBlock = false; // cannot skip if this block defines functions
    Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(name);
    if (fns) {
      forv_Vec(FnSymbol, fn, *fns) {
        if (fn->isVisible(callOrigin)) {
          // isVisible checks if the function is private to its defining
          // module (and in that case, if we are under its defining module)
          // This ensures that private functions will not be used outside
          // of their proper scope.
          visibleFns.add(fn);
        }
      }
    }
  }

  if (block->modUses) {
    for_actuals(expr, block->modUses) {
      UseStmt* use = toUseStmt(expr);
      INT_ASSERT(use);
      if (use->skipSymbolSearch(name)) {
        continue;
      }
      SymExpr* se = toSymExpr(use->src);
      INT_ASSERT(se);
      if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
        // The use statement could be of an enum instead of a module, but only
        // modules can define functions.
        canSkipThisBlock = false; // cannot skip if this block uses modules
        if (mod->isVisible(callOrigin)) {
          if (use->isARename(name)) {
            getVisibleFunctions(mod->block, use->getRename(name), visibleFns, visited, callOrigin);
          } else {
            getVisibleFunctions(mod->block, name, visibleFns, visited, callOrigin);
          }
        }
      }
    }
  }

  //
  // visibilityBlockCache contains blocks that can be skipped
  //
  if (BlockStmt* next = visibilityBlockCache.get(block)) {
    getVisibleFunctions(next, name, visibleFns, visited, callOrigin);
    return (canSkipThisBlock) ? next : block;
  }

  if (block != rootModule->block) {
    BlockStmt* next = getVisibilityBlock(block);
    BlockStmt* cache = getVisibleFunctions(next, name, visibleFns, visited, callOrigin);
    if (cache)
      visibilityBlockCache.put(block, cache);
    return (canSkipThisBlock) ? cache : block;
  }

  return NULL;
}

// Ensure 'parent' is the block before which we want to do the capturing.
static void verifyTaskFnCall(BlockStmt* parent, CallExpr* call) {
  if (call->isNamed("coforall_fn") || call->isNamed("on_fn")) {
    INT_ASSERT(parent->isForLoop());
  } else if (call->isNamed("cobegin_fn")) {
    DefExpr* first = toDefExpr(parent->getFirstExpr());
    // just documenting the current state
    INT_ASSERT(first && !strcmp(first->sym->name, "_cobeginCount"));
  } else {
    INT_ASSERT(call->isNamed("begin_fn"));
  }
}

//
// Allow invoking isConstValWillNotChange() even on formals
// with blank and 'const' intents.
//
static bool isConstValWillNotChange(Symbol* sym) {
  if (ArgSymbol* arg = toArgSymbol(sym)) {
    IntentTag cInt = concreteIntent(arg->intent, arg->type->getValType());
    return cInt == INTENT_CONST_IN;
  }
  return sym->isConstValWillNotChange();
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
  if (call->isNamed("cobegin_fn")) {
    // I want to be cute and keep _cobeginCount def and move
    // as the first two statements in the block.
    DefExpr* def = toDefExpr(parent->body.head);
    INT_ASSERT(def);
    // Just so we know what we are doing.
    INT_ASSERT(!strcmp((def->sym->name), "_cobeginCount"));
    CallExpr* move = toCallExpr(def->next);
    INT_ASSERT(move);
    SymExpr* arg1 = toSymExpr(move->get(1));
    INT_ASSERT(arg1->symbol() == def->sym);
    // And this is where we want to insert:
    return move->next;
  }
  // Otherwise insert before 'parent'
  return parent;
}

// map: (block id) -> (map: sym -> sym)
typedef std::map<int, SymbolMap*> CapturedValueMap;
static CapturedValueMap capturedValues;
static void freeCache(CapturedValueMap& c) {
  for (std::map<int, SymbolMap*>::iterator it = c.begin(); it != c.end(); ++it)
    delete it->second;
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
static void captureTaskIntentValues(int argNum, ArgSymbol* formal,
                                    Expr* actual, Symbol* varActual,
                                    CallInfo& info, CallExpr* call,
                                    FnSymbol* taskFn)
{
  BlockStmt* parent = toBlockStmt(call->parentExpr);
  INT_ASSERT(parent);
  if (taskFn->hasFlag(FLAG_ON) && !parent->isForLoop()) {
    // coforall ... { on ... { .... }} ==> there is an intermediate BlockStmt
    parent = toBlockStmt(parent->parentExpr);
    INT_ASSERT(parent);
  }
  if (fVerify && (argNum == 0 || (argNum == 1 && taskFn->hasFlag(FLAG_ON))))
    verifyTaskFnCall(parent, call); //assertions only
  Expr* marker = parentToMarker(parent, call);
  if (varActual->hasFlag(FLAG_NO_CAPTURE_FOR_TASKING)) {
      // No need to worry about this.
      return;
  }
  if (varActual->defPoint->parentExpr == parent) {
    // Index variable of the coforall loop? Do not capture it!
    INT_ASSERT(varActual->hasFlag(FLAG_COFORALL_INDEX_VAR));
    return;
  }
  SymbolMap*& symap = capturedValues[parent->id];
  Symbol* captemp = NULL;
  if (symap)
    captemp = symap->get(varActual);
  else
    symap = new SymbolMap();
  if (!captemp) {
    captemp = newTemp(astr(formal->name, "_captemp"), formal->type);
    marker->insertBefore(new DefExpr(captemp));
    // todo: once AMM is in effect, drop chpl__autoCopy - do straight move
    if (hasAutoCopyForType(formal->type)) {
      FnSymbol* autoCopy = getAutoCopy(formal->type);
      marker->insertBefore("'move'(%S,%S(%S))", captemp, autoCopy, varActual);
    } else if (isReferenceType(varActual->type) &&
             !isReferenceType(captemp->type))
      marker->insertBefore("'move'(%S,'deref'(%S))", captemp, varActual);
    else
      marker->insertBefore("'move'(%S,%S)", captemp, varActual);
    symap->put(varActual, captemp);
  }
  actual->replace(new SymExpr(captemp));
  Symbol*& iact = info.actuals.v[argNum];
  INT_ASSERT(iact == varActual);
  iact = captemp;
}

//
// Copy the type of the actual into the type of the corresponding formal
// of a task function. (I think resolution wouldn't make this happen
// automatically and correctly in all cases.)
// Also do captureTaskIntentValues() when needed.
//
static void handleTaskIntentArgs(CallExpr* call, FnSymbol* taskFn,
                                 CallInfo& info)
{
  INT_ASSERT(taskFn);
  if (!needsCapture(taskFn)) {
    // A task function should have args only if it needsCapture.
    if (taskFn->hasFlag(FLAG_ON)) {
      // Documenting the current state: fn_on gets a chpl_localeID_t arg.
      INT_ASSERT(call->numActuals() == 1);
    } else {
      INT_ASSERT(!isTaskFun(taskFn) || call->numActuals() == 0);
    }
    return;
  }

  int argNum = -1;
  for_formals_actuals(formal, actual, call) {
    argNum++;
    SymExpr* symexpActual = toSymExpr(actual);
    if (!symexpActual) {
      // We add NamedExpr args in propagateExtraLeaderArgs().
      NamedExpr* namedexpActual = toNamedExpr(actual);
      INT_ASSERT(namedexpActual);
      symexpActual = toSymExpr(namedexpActual->actual);
    }
    INT_ASSERT(symexpActual); // because of how we invoke a task function
    Symbol* varActual = symexpActual->symbol();

    // If 'call' is in a generic function, it is supposed to have been
    // instantiated by now. Otherwise our task function has to remain generic.
    INT_ASSERT(!varActual->type->symbol->hasFlag(FLAG_GENERIC));

    // Need to copy varActual->type even for type variables.
    // BTW some formals' types may have been set in createTaskFunctions().
    formal->type = varActual->type;

    // If the actual is a ref, still need to capture it => remove ref.
    if (isReferenceType(varActual->type)) {
      Type* deref = varActual->type->getValType();
      // todo: replace needsCapture() with always resolveArgIntent(formal)
      // then checking (formal->intent & INTENT_FLAG_IN)
      if (needsCapture(deref)) {
        formal->type = deref;
        // If the formal has a ref intent, DO need a ref type => restore it.
        resolveArgIntent(formal);
        if (formal->intent & INTENT_FLAG_REF) {
          formal->type = varActual->type;
        }
        if (varActual->isConstant()) {
          int newIntent = formal->intent | INTENT_FLAG_CONST;
          // and clear INTENT_FLAG_MAYBE_CONST flag
          newIntent &= ~ INTENT_FLAG_MAYBE_CONST;
          formal->intent = (IntentTag)(newIntent);
        }
      }
    }

    if (varActual->hasFlag(FLAG_TYPE_VARIABLE))
      formal->addFlag(FLAG_TYPE_VARIABLE);

    // This does not capture records/strings that are passed
    // by blank or const intent. As of this writing (6'2015)
    // records and strings are (incorrectly) captured at the point
    // when the task function/arg bundle is created.
    if (taskFn->hasFlag(FLAG_COBEGIN_OR_COFORALL) &&
        !isConstValWillNotChange(varActual) &&
        (concreteIntent(formal->intent, formal->type->getValType())
         & INTENT_FLAG_IN))
      // skip dummy_locale_arg: chpl_localeID_t
      if (argNum != 0 || !taskFn->hasFlag(FLAG_ON))
        captureTaskIntentValues(argNum, formal, actual, varActual, info, call,
                                taskFn);
  }

  // Even if some formals are (now) types, if 'taskFn' remained generic,
  // gatherCandidates() would not instantiate it, for some reason.
  taskFn->removeFlag(FLAG_GENERIC);
}

void fillVisibleFuncVec(CallExpr* call, CallInfo &info,
                        Vec<FnSymbol*> &visibleFns) {
  //
  // update visible function map as necessary
  //
  if (gFnSymbols.n != nVisibleFunctions) {
    buildVisibleFunctionMap();
  }

  if (!call->isResolved()) {
    if (!info.scope) {
      Vec<BlockStmt*> visited;
      getVisibleFunctions(getVisibilityBlock(call), info.name, visibleFns, visited, call);
    } else {
      if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(info.scope)) {
        if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(info.name)) {
          visibleFns.append(*fns);
        }
      }
    }
  } else {
    visibleFns.add(call->isResolved());
    handleTaskIntentArgs(call, call->isResolved(), info);
  }

  if ((explainCallLine && explainCallMatch(call)) ||
      call->id == explainCallID)
  {
    USR_PRINT(call, "call: %s", toString(&info));
    if (visibleFns.n == 0)
      USR_PRINT(call, "no visible functions found");
    bool first = true;
    forv_Vec(FnSymbol, visibleFn, visibleFns) {
      USR_PRINT(visibleFn, "%s %s",
                first ? "visible functions are:" : "                      ",
                toString(visibleFn));
      first = false;
    }
  }
}


// Resolves a param or type expression
static Expr*
resolve_type_expr(Expr* expr) {
  Expr* result = NULL;
  for_exprs_postorder(e, expr) {
    result = preFold(e);
    if (CallExpr* call = toCallExpr(result)) {
      if (call->parentSymbol) {
        callStack.add(call);
        resolveCall(call);
        FnSymbol* fn = call->isResolved();
        if (fn && call->parentSymbol) {
          resolveFormals(fn);
          if (fn->retTag == RET_PARAM || fn->retTag == RET_TYPE ||
              fn->retType == dtUnknown)
            resolveFns(fn);
        }
        callStack.pop();
      }
    }
    result = postFold(result);
  }
  return result;
}


static void
makeNoop(CallExpr* call) {
  if (call->baseExpr)
    call->baseExpr->remove();
  while (call->numActuals())
    call->get(1)->remove();
  call->primitive = primitives[PRIM_NOOP];
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

static void findNonTaskFnParent(CallExpr* call,
                                FnSymbol*& parent, int& stackIdx) {
  // We assume that 'call' is at the top of the call stack.
  INT_ASSERT(callStack.n >= 1);
  INT_ASSERT(callStack.v[callStack.n-1] == call ||
             callStack.v[callStack.n-1] == call->parentExpr);

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


// If 'call' is an access to a const thing, for example a const field
// or a field of a const record, set const flag(s) on the symbol
// that stores the result of 'call'.
static void setFlagsAndCheckForConstAccess(Symbol* dest,
                                         CallExpr* call, FnSymbol* resolvedFn)
{
  // Is the outcome of 'call' a reference to a const?
  bool refConst = resolvedFn->hasFlag(FLAG_REF_TO_CONST);
  // Another flag that's relevant.
  const bool constWCT = resolvedFn->hasFlag(FLAG_REF_TO_CONST_WHEN_CONST_THIS);
  // The second flag does not make sense when the first flag is set.
  INT_ASSERT(!(refConst && constWCT));

  // The symbol whose field is accessed, if applicable:
  Symbol* baseSym = NULL;

  if (refConst) {
    if (resolvedFn->hasFlag(FLAG_FIELD_ACCESSOR) &&
        // promotion wrappers are not handled currently
        !resolvedFn->hasFlag(FLAG_PROMOTION_WRAPPER)
        )
      baseSym = getBaseSymForConstCheck(call);

  } else if (resolvedFn->hasFlag(FLAG_NEW_ALIAS_FN) &&
             dest->hasFlag(FLAG_ARRAY_ALIAS)) {
    if (!dest->isConstant()) {
      // We are creating a var alias - ensure aliasee is not const either.
      SymExpr* aliaseeSE = toSymExpr(call->get(2));
      INT_ASSERT(aliaseeSE);
      if (aliaseeSE->symbol()->isConstant())
        USR_FATAL_CONT(call, "creating a non-const alias '%s' of a const array or domain", dest->name);
    }
  } else if (constWCT) {
    baseSym = getBaseSymForConstCheck(call);
    if (baseSym->isConstant()               ||
        baseSym->hasFlag(FLAG_REF_TO_CONST) ||
        baseSym->hasFlag(FLAG_CONST)
       )
      refConst = true;
    else
      // The result is not constant.
      baseSym = NULL;

  } else if (dest->hasFlag(FLAG_ARRAY_ALIAS)        &&
             resolvedFn->hasFlag(FLAG_AUTO_COPY_FN))
  {
    INT_ASSERT(false); // should not happen
  }

  // Do not consider it const if it is an access to 'this'
  // in a constructor. Todo: will need to reconcile with UMM.
  // btw (baseSym != NULL) ==> (refConst == true).
  if (baseSym) {
    // Aside: at this point 'baseSym' can have reference or value type,
    // seemingly without a particular rule.
    if (baseSym->hasFlag(FLAG_ARG_THIS)   &&
        isInConstructorLikeFunction(call)
        )
      refConst = false;
  }

  if (refConst) {
    if (isReferenceType(dest->type))
      dest->addFlag(FLAG_REF_TO_CONST);
    else
      dest->addFlag(FLAG_CONST);

    if (baseSym && baseSym->hasFlag(FLAG_ARG_THIS))
      // 'call' can be a field accessor or an array element accessor or ?
      dest->addFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS);

    // Propagate this flag.  btw (recConst && constWCT) ==> (baseSym != NULL)
    if (constWCT && baseSym->hasFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS))
      dest->addFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS);
  }
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
  if (!strcmp("=", fn->name)) {
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

static bool
typeUsesForwarding(Type* t) {
  if (AggregateType* at = toAggregateType(t)) {
    if (toForwardingStmt(at->forwardingTo.head)) {
      return true;
    }
  }
  return false;
}

static bool
populateForwardingMethods(Type* t,
                          const char* calledName,
                          CallExpr* forCall)
{
  AggregateType* at = toAggregateType(t);
  bool addedAny = false;

  // Currently, only AggregateTypes can forward
  if (!at) return false;

  // If the type has not yet been resolved, stop,
  // since otherwise computing the forwarding fn won't go well.
  for_fields(field, at) {
    if (field->type == dtUnknown)
      return false;
  }

  // try resolving the call on the forwarding expressions
  for_alist(expr, at->forwardingTo) {
    ForwardingStmt* delegate = toForwardingStmt(expr);
    INT_ASSERT(delegate);

    const char* fnGetTgt = delegate->fnReturningForwarding;
    const char* methodName = calledName;

    if (!delegate->type) {
      delegate->type = dtUnknown; // avoiding loop from recursion

      // First, figure out the type of the delegate
      SET_LINENO(at->symbol);
      Symbol* tmp = newTemp(at);
      at->symbol->defPoint->insertBefore(new DefExpr(tmp));
      CallExpr* getTgtCall = new CallExpr(fnGetTgt, gMethodToken, tmp);
      FnSymbol* fn = resolveUninsertedCall(at, getTgtCall);
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
    if (methodName == NULL)
      continue;

    // Make sure methodName is a blessed string
    methodName = astr(methodName);

    // There are 2 ways that more methods can be added to
    // delegate->type during resolution:
    //   1) delegate->type itself use a 'delegate'
    //   2) delegate->type is a generic instantiation
    //      and the method in question hasn't been instantiated yet
    //
    // We handle 1 by resolving a call here to the method in question.
    // We handle 2 below by creating a generic wrapper for a generic function.
    {
      BlockStmt* block = new BlockStmt();
      Type* testType = delegate->type;
      Symbol* tmp = newTemp(testType);

      CallExpr* test = new CallExpr(new UnresolvedSymExpr(methodName),
                                    gMethodToken,
                                    tmp);


      int i = 0;
      for_actuals(actual, forCall) {
        if (i > 1) { // skip method token, object
          test->insertAtTail(actual->copy());
        }
        i++;
      }

      block->insertAtTail(new DefExpr(tmp));
      block->insertAtTail(test);

      forCall->getStmtExpr()->insertAfter(block);
      tryResolveCall(test);
      block->remove();
    }

    // Now, forward all methods named 'methodName' as 'calledName'.
    // Forward generic functions as generic functions.
    forv_Vec(FnSymbol, method, delegate->type->methods) {
      // Skip any methods with a different name
      // TODO: this could be more efficient if methods were a map
      if (method->name != methodName)
        continue;

      // Skip any methods that don't match parentheses-less
      // vs parentheses-ful vs the call.
      if (method->hasFlag(FLAG_NO_PARENS) != forCall->methodTag)
        continue;

      // Skip any methods that are init/ctor/dtor
      // These cannot yet be forwarded.
      if (method->hasFlag(FLAG_DESTRUCTOR) ||
          method->hasFlag(FLAG_CONSTRUCTOR) ||
          0 == strcmp(methodName, "init"))
        continue;

      // Skip any instantiations of functions with
      // with generic arguments (but not counting the this argument,
      // since a method can be instantiated just for a generic this,
      // but that should count as concrete for us here.
      if (method->instantiatedFrom != NULL) {
        bool skip = false;

        int i = 0;
        for_formals(formal, method->instantiatedFrom) {
          // skip method token
          // skip `this` argument
          if (i >= 2) {
            if (formal->type->symbol->hasFlag(FLAG_GENERIC))
              skip = true;
          }
          i++;
        }

        if (skip)
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
      fn->removeFlag(FLAG_INVISIBLE_FN);

      fn->addFlag(FLAG_METHOD);
      fn->addFlag(FLAG_INLINE);
      fn->addFlag(FLAG_FORWARDING_FN);
      fn->addFlag(FLAG_COMPILER_GENERATED);
      fn->retTag = method->retTag;

      ArgSymbol* mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
      ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", at);
      _this->addFlag(FLAG_ARG_THIS);

      fn->insertFormalAtTail(mt);
      fn->insertFormalAtTail(_this);
      fn->_this = _this;

      // Add a call to the original function
      VarSymbol* tgt = newTemp("tgt");
      tgt->addFlag(FLAG_MAYBE_REF);
      CallExpr* getTgt = new CallExpr(fnGetTgt, gMethodToken, _this);
      CallExpr* setTgt = new CallExpr(PRIM_MOVE, tgt, getTgt);
      CallExpr* wrapCall = new CallExpr(method, gMethodToken, tgt);

      // Add the arguments to the wrapper function
      // Add the arguments to the call
      int i = 0;
      for_formals(formal, method) {
        if (i > 1) { // skip method token, target - added above
          ArgSymbol* arg = formal->copy();
          fn->insertFormalAtTail(arg);
          wrapCall->insertAtTail(new SymExpr(arg));
        }
        i++;
      }

      // Copy the where clause, if any
      // TODO: replace this.type with the delegate expression.type
      if (method->where != NULL) {
        SymbolMap map;

        int nFormals = method->numFormals();
        for (int i = 1; i <= nFormals; i++) {
          Symbol* from = method->getFormal(i);
          Symbol* to = fn->getFormal(i);
          map.put(from, to);
        }
        fn->where = method->where->copy(&map);
      }

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
      at->symbol->defPoint->insertBefore(new DefExpr(fn));

      // Add the new function as a method.
      at->methods.add(fn);
    }
  }

  return addedAny;
}



static void
doGatherCandidates(Vec<ResolutionCandidate*>& candidates,
                 Vec<FnSymbol*>& visibleFns,
                 CallInfo& info,
                 bool compilerGenerated) {

  forv_Vec(FnSymbol, visibleFn, visibleFns) {
    // Only consider user functions or compiler-generated functions
    if (visibleFn->hasFlag(FLAG_COMPILER_GENERATED) == compilerGenerated) {

      // Some expressions might resolve to methods without parenthesis.
      // If the call is marked with methodTag, it indicates the called
      // function should be a no-parens function or a type constructor.
      // (a type constructor call without parens uses default arguments)
      if (info.call->methodTag) {
        if (visibleFn->hasEitherFlag(FLAG_NO_PARENS, FLAG_TYPE_CONSTRUCTOR)) {
          // OK
        } else {
          // Skip this candidate
          continue;
        }
      }

      if (fExplainVerbose &&
          ((explainCallLine && explainCallMatch(info.call)) ||
           info.call->id == explainCallID))
      {
        USR_PRINT(visibleFn, "Considering function: %s", toString(visibleFn));
        if( info.call->id == breakOnResolveID ) {
          gdbShouldBreakHere();
        }
      }

      filterCandidate(candidates, visibleFn, info);
    }
  }
}


static void
gatherCandidates(Vec<ResolutionCandidate*>& candidates,
                 Vec<FnSymbol*>& visibleFns,
                 CallInfo& info) {

  // Search user-defined (i.e. non-compiler-generated) functions first.
  doGatherCandidates(candidates, visibleFns, info, false);

  // If no results, try again with any compiler-generated candidates.
  if (candidates.n == 0) {
    doGatherCandidates(candidates, visibleFns, info, true);
  }
}


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


static void findVisibleCandidates(CallInfo& info,
                                  Vec<FnSymbol*>& visibleFns,
                                  Vec<ResolutionCandidate*>& candidates) {


  CallExpr* call = info.call;

  fillVisibleFuncVec(call, info, visibleFns);

  gatherCandidates(candidates, visibleFns, info);
}

// if checkonly is provided, don't print any errors; just check
// to see if the particular function could be resolved.
// returns the result of resolving - or NULL if we couldn't do it.
// If checkonly is set, NULL can be returned - otherwise that would
// be a fatal error.
FnSymbol* resolveNormalCall(CallExpr* call, bool checkonly) {

  if( call->id == breakOnResolveID ) {
    printf("breaking on resolve call:\n");
    print_view(call);
    gdbShouldBreakHere();
  }

  temporaryInitializerFixup(call);

  resolveDefaultGenericType(call);

  CallInfo info(call, checkonly);

  // Return early if creating the call info would have been an error.
  if( checkonly && info.badcall ) return NULL;

  Vec<FnSymbol*> visibleFns; // visible functions
  Vec<ResolutionCandidate*> candidates;

  // First, try finding candidates without delegation
  findVisibleCandidates(info, visibleFns, candidates);

  bool retry_find = false;

  // if no candidate was found, try it with delegation
  if (candidates.n == 0) {
    // if it's a method, try delegating
    if (call->numActuals() >= 1 &&
        call->get(1)->typeInfo() == dtMethodToken) {
      Type* receiverType = call->get(2)->typeInfo()->getValType();
      if (typeUsesForwarding(receiverType)) {
        retry_find = populateForwardingMethods(receiverType, info.name, info.call);
      }
    }
  }

  if (retry_find) {
    // clear out visibleFns, candidates
    visibleFns.clear();
    forv_Vec(ResolutionCandidate*, candidate, candidates) {
      delete candidate;
    }
    candidates.clear();

    // try again to include forwarded functions
    findVisibleCandidates(info, visibleFns, candidates);
  }

  explainGatherCandidate(candidates, info, call);


  Expr* scope = (info.scope) ? info.scope : getVisibilityBlock(call);
  bool explain = fExplainVerbose &&
    ((explainCallLine && explainCallMatch(call)) ||
     info.call->id == explainCallID);
  DisambiguationContext DC(&info.actuals, scope, explain);

  ResolutionCandidate* bestRef = disambiguateByMatch(candidates, DC, FIND_REF);
  ResolutionCandidate* bestConstRef = disambiguateByMatch(candidates, DC, FIND_CONST_REF);
  ResolutionCandidate* bestValue = disambiguateByMatch(candidates, DC, FIND_NOT_REF_OR_CONST_REF);

  // If one requires more promotion than the other, this is not a ref-pair.
  if (bestRef && bestValue && isBetterMatch(bestRef, bestValue, DC, true)) {
    bestValue = NULL; // Don't consider the value function.
  }
  if (bestRef && bestValue && isBetterMatch(bestValue, bestRef, DC, true)) {
    bestRef = NULL; // Don't consider the ref function.
  }
  if (bestRef && bestConstRef && isBetterMatch(bestRef, bestConstRef, DC, true)) {
    bestConstRef = NULL; // Don't consider the const ref function.
  }
  if (bestRef && bestConstRef && isBetterMatch(bestConstRef, bestRef, DC, true)) {
    bestRef = NULL; // Don't consider the ref function.
  }
  if (bestConstRef && bestValue && isBetterMatch(bestConstRef, bestValue, DC, true)) {
    bestValue = NULL; // Don't consider the value function.
  }
  if (bestConstRef && bestValue && isBetterMatch(bestValue, bestConstRef, DC, true)) {
    bestConstRef = NULL; // Don't consider the const ref function.
  }

  ResolutionCandidate* best = bestRef;
  if (!best && bestValue) best = bestValue;
  if (!best && bestConstRef) best = bestConstRef;

  bool refPair = (bestRef && (bestValue || bestConstRef));

  // If we have both ref and value matches:
  //  'call' will invoke the ref function best->fn
  //  'valueCall' will invoke the value function bestValue->fn
  //  'constRefCall' will invoke the value function bestConstRef->fn
  //  we will manipulate these three side by side.
  //  valueCall is always NULL if there aren't ref and value matches.
  CallExpr* valueCall = NULL;
  CallExpr* constRefCall = NULL;

  if (refPair && bestValue) {
    valueCall = call->copy();
    call->insertAfter(valueCall);
  }
  if (refPair && bestConstRef) {
    constRefCall = call->copy();
    call->insertAfter(constRefCall);
  }


  if (best && best->fn) {
    /*
     * Finish instantiating the body.  This is a noop if the function wasn't
     * partially instantiated.
     */

    instantiateBody(best->fn);
    if (valueCall) {
      // If we're resolving a ref and non-ref pair,
      // also instantiate the value version. best is the ref version.
      INT_ASSERT(bestValue->fn);
      instantiateBody(bestValue->fn);
    }
    if (constRefCall) {
      // If we're resolving a ref and non-ref pair,
      // also instantiate the const-ref version. best is the ref version.
      INT_ASSERT(bestConstRef->fn);
      instantiateBody(bestConstRef->fn);
    }

    if (explainCallLine && explainCallMatch(call)) {
      USR_PRINT(best->fn, "best candidate is: %s", toString(best->fn));
    }
  }

  // Future work note: the repeated check to best and best->fn means that we
  // could probably restructure this function to a better form.
  if (call->partialTag && (!best || !best->fn ||
                           !best->fn->hasFlag(FLAG_NO_PARENS))) {
    if (best != NULL) {
      // MPF 2016-0106 - this appears to be dead code
      // at least in a full single-locale test run.

      // best is deleted below with the other candidates
      best = NULL;
    }
  } else if (!best) {
    if (tryStack.n) {
      // MPF -- doesn't this leak memory for the ResolutionCandidates?
      if( ! checkonly ) tryFailure = true;
      return NULL;

    } else {
      // if we're just checking, don't print errors
      if( ! checkonly ) {

        if (candidates.n > 0) {
          Vec<FnSymbol*> candidateFns;
          forv_Vec(ResolutionCandidate*, candidate, candidates) {
            candidateFns.add(candidate->fn);
          }

          printResolutionErrorAmbiguous(candidateFns, &info);
        } else {
          printResolutionErrorUnresolved(visibleFns, &info);
        }
      }
    }
  } else {
    wrapAndCleanUpActuals(best, info, true);
    if (valueCall) {
      // If we're resolving a ref and non-ref pair,
      // also handle the value version. best is the ref version.
      CallInfo valueInfo(valueCall, checkonly);

      wrapAndCleanUpActuals(bestValue, valueInfo, false);
    }
    if (constRefCall) {
      // If we're resolving a ref and non-ref pair,
      // also handle the const ref version. best is the ref version.
      CallInfo constRefInfo(constRefCall, checkonly);

      wrapAndCleanUpActuals(bestConstRef, constRefInfo, false);
    }
  }

  FnSymbol* resolvedFn = best != NULL ? best->fn : NULL;
  FnSymbol* resolvedRefFn = bestRef != NULL ? bestRef->fn : NULL;
  FnSymbol* resolvedValueFn = bestValue != NULL ? bestValue->fn : NULL;
  FnSymbol* resolvedConstRefFn = bestConstRef != NULL ? bestConstRef->fn : NULL;

  // Only keep it a ref-pair if a ref version is present and
  // we have options that resolved.
  if (refPair)
    refPair = resolvedRefFn && (resolvedValueFn || resolvedConstRefFn);

  forv_Vec(ResolutionCandidate*, candidate, candidates) {
    delete candidate;
  }

  if (call->partialTag) {
    if (!resolvedFn) {
      if (valueCall) valueCall->remove();
      if (constRefCall) constRefCall->remove();
      return NULL;
    }
    call->partialTag = false;
  }


  if( ! checkonly ) {
    if (resolvedFn &&
        !strcmp("=", resolvedFn->name) &&
        isRecord(resolvedFn->getFormal(1)->type) &&
        resolvedFn->getFormal(2)->type == dtNil) {
      USR_FATAL(userCall(call), "type mismatch in assignment from nil to %s",
                toString(resolvedFn->getFormal(1)->type));
    }

    if (!resolvedFn) {
      INT_FATAL(call, "unable to resolve call");
    }
  }

  if (resolvedFn && call->parentSymbol) {
    SET_LINENO(call);
    call->baseExpr->replace(new SymExpr(resolvedFn));
    if (refPair) {
      if (valueCall && resolvedValueFn)
        valueCall->baseExpr->replace(new SymExpr(resolvedValueFn));
      if (constRefCall && resolvedConstRefFn)
        constRefCall->baseExpr->replace(new SymExpr(resolvedConstRefFn));

      // Replace the call with a new ContextCallExpr containing 2 or 3
      // calls, where the first returns ref and the 2nd does not.
      // The 3 calls might be:
      //  ref version
      //  value version
      //  const ref version
      ContextCallExpr* contextCall = new ContextCallExpr();
      call->insertAfter(contextCall);

      call->remove();
      if (valueCall) valueCall->remove();
      if (constRefCall) constRefCall->remove();

      if (valueCall && constRefCall)
        contextCall->setRefValueConstRefOptions(call, valueCall, constRefCall);
      else
        contextCall->setRefRValueOptions(call, valueCall?valueCall:constRefCall);

    } else if (valueCall) {
      // value call was added but didn't resolve right. Remove it.
      valueCall->remove();
    } else if (constRefCall) {
      // const ref call was added but didn't resolve right. Remove it.
      constRefCall->remove();
    } else {
      // If we aren't working with a ref not-ref return intent pair,
      // adjust the returned value to have flag FLAG_REF_TO_CONST,
      // but disable this behavior for constructors, so that they
      // can set 'const' fields.
      if (resolvedFn->retTag == RET_CONST_REF)
        if (CallExpr* parentCall = toCallExpr(call->parentExpr))
          if (parentCall->isPrimitive(PRIM_MOVE))
            if (SymExpr* lhsSe = toSymExpr(parentCall->get(1)))
              if (VarSymbol* lhs = toVarSymbol(lhsSe->symbol()))
                if (lhs->hasFlag(FLAG_EXPR_TEMP))
                  if (FnSymbol* inFn = toFnSymbol(parentCall->parentSymbol))
                    if (!isConstructorLikeFunction(inFn))
                      lhs->addFlag(FLAG_REF_TO_CONST);
    }
  }

  if( ! checkonly ) {
    if (resolvedFn->hasFlag(FLAG_MODIFIES_CONST_FIELDS))
      // Not allowed if it is not called directly from a constructor.
      if (!isInConstructorLikeFunction(call) ||
          !getBaseSymForConstCheck(call)->hasFlag(FLAG_ARG_THIS)
          )
        USR_FATAL_CONT(call, "illegal call to %s() - it modifies 'const' fields of 'this', therefore it can be invoked only directly from a constructor on the object being constructed", resolvedFn->name);

    lvalueCheck(call);
    checkForStoringIntoTuple(call, resolvedFn);

    resolveNormalCallCompilerWarningStuff(resolvedFn);
  }

  return resolvedFn;
}

void explainGatherCandidate(Vec<ResolutionCandidate*>& candidates,
                            CallInfo& info, CallExpr* call) {
  if ((explainCallLine && explainCallMatch(info.call)) ||
      call->id == explainCallID) {
    if (candidates.n == 0) {
      USR_PRINT(info.call, "no candidates found");

    } else {
      bool first = true;
      forv_Vec(ResolutionCandidate*, candidate, candidates) {
        USR_PRINT(candidate->fn, "%s %s",
                  first ? "candidates are:" : "               ",
                  toString(candidate->fn));
        first = false;
      }
    }
  }
}

void wrapAndCleanUpActuals(ResolutionCandidate* best, CallInfo& info,
                           bool buildFastFollowerChecks) {
  INT_ASSERT(best->fn);
  best->fn = defaultWrap(best->fn, &best->actualIdxToFormal, &info);
  reorderActuals(best->fn, &best->actualIdxToFormal, &info);
  coerceActuals(best->fn, &info);
  best->fn = promotionWrap(best->fn, &info, buildFastFollowerChecks);
}

void resolveNormalCallCompilerWarningStuff(FnSymbol* resolvedFn) {
  if (const char* str = innerCompilerWarningMap.get(resolvedFn)) {
    reissueCompilerWarning(str, 2);
    if (callStack.n >= 2)
      if (FnSymbol* fn = toFnSymbol(callStack.v[callStack.n-2]->isResolved()))
        outerCompilerWarningMap.put(fn, str);
  }

  if (const char* str = outerCompilerWarningMap.get(resolvedFn)) {
    reissueCompilerWarning(str, 1);
  }
}

void lvalueCheck(CallExpr* call)
{
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
        checkAndUpdateIfLegalFieldOfThis(call, actual, nonTaskFnParent)
    ) {
      errorMsg = false;
      nonTaskFnParent->addFlag(FLAG_MODIFIES_CONST_FIELDS);
    }
    if (errorMsg) {
      if (nonTaskFnParent->hasFlag(FLAG_SUPPRESS_LVALUE_ERRORS))
        // we are asked to ignore errors here
        return;
      FnSymbol* calleeFn = call->isResolved();
      INT_ASSERT(calleeFn == formal->defPoint->parentSymbol); // sanity
      if (calleeFn->hasFlag(FLAG_ASSIGNOP)) {
        // This assert is FYI. Perhaps can remove it if it fails.
        INT_ASSERT(callStack.n > 0 && callStack.v[callStack.n-1] == call);
        const char* recordName =
          defaultRecordAssignmentTo(toFnSymbol(call->parentSymbol));
        if (recordName && callStack.n >= 2)
          // blame on the caller of the caller, if available
          USR_FATAL_CONT(callStack.v[callStack.n-2],
                         "cannot assign to a record of the type %s"
                         " using the default assignment operator"
                         " because it has 'const' field(s)", recordName);
        else
          USR_FATAL_CONT(actual, "illegal lvalue in assignment");
      }
      else
      {
        ModuleSymbol* mod = calleeFn->getModule();
        char cn1 = calleeFn->name[0];
        const char* calleeParens = (isalpha(cn1) || cn1 == '_') ? "()" : "";
        // Should this be the same condition as in insertLineNumber() ?
        if (developer || mod->modTag == MOD_USER) {
          USR_FATAL_CONT(actual, "non-lvalue actual is passed to %s formal '%s'"
                         " of %s%s", formal->intentDescrString(), formal->name,
                         calleeFn->name, calleeParens);
        } else {
          USR_FATAL_CONT(actual, "non-lvalue actual is passed to a %s formal of"
                         " %s%s", formal->intentDescrString(),
                         calleeFn->name, calleeParens);
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
          if (!strncmp(varname, "_formal_tmp_", 12))
            varname += 12;
          if (isArgSymbol(aVar) || aVar->hasFlag(FLAG_TEMP)) {
            Symbol* enclTaskFn = aVar->defPoint->parentSymbol;
            BaseAST* marker;
            const char* constructName;
            if (enclTaskFn->hasFlag(FLAG_BEGIN)) {
              // enclTaskFn points to a good line number
              marker = enclTaskFn;
              constructName = "begin";
            } else {
              marker = enclTaskFn->defPoint->parentExpr;
              constructName = "parallel";
            }
            USR_PRINT(marker, "The shadow variable '%s' is constant due to task intents in this %s statement", varname, constructName);
          } else {
            Expr* enclLoop = aVar->defPoint->parentExpr;
            USR_PRINT(enclLoop, "The shadow variable '%s' is constant due to forall intents in this loop", varname);
          }
}

// We do some const-related work upon PRIM_MOVE
static void setConstFlagsAndCheckUponMove(Symbol* lhs, Expr* rhs) {
  // If this assigns into a loop index variable from a non-var iterator,
  // mark the variable constant.
  if (SymExpr* rhsSE = toSymExpr(rhs)) {
    // If RHS is this special variable...
    if (rhsSE->symbol()->hasFlag(FLAG_INDEX_OF_INTEREST)) {
      INT_ASSERT(lhs->hasFlag(FLAG_INDEX_VAR));
      Type* rhsType = rhsSE->symbol()->type;
      // ... and not of a reference type
      // todo: differentiate based on ref-ness, not _ref type
      // todo: not all const if it is zippered and one of iterators is var
      if (!isReferenceType(rhsType) && !isTupleContainingAnyReferences(rhsType))
       // ... and not an array (arrays are always yielded by reference)
       if (!rhsSE->symbol()->type->symbol->hasFlag(FLAG_ARRAY))
        // ... then mark LHS constant.
        lhs->addFlag(FLAG_CONST);
    }
  } else if (CallExpr* rhsCall = toCallExpr(rhs)) {
    if (rhsCall->isPrimitive(PRIM_GET_MEMBER)) {
      if (SymExpr* rhsBase = toSymExpr(rhsCall->get(1))) {
        if (rhsBase->symbol()->hasFlag(FLAG_CONST) ||
            rhsBase->symbol()->hasFlag(FLAG_REF_TO_CONST)
            )
          lhs->addFlag(FLAG_REF_TO_CONST);
      } else {
        INT_ASSERT(false); // PRIM_GET_MEMBER of a non-SymExpr??
      }
    } else if (FnSymbol* resolvedFn = rhsCall->isResolved()) {
        setFlagsAndCheckForConstAccess(lhs, rhsCall, resolvedFn);
    }
  }
}


static void resolveTupleAndExpand(CallExpr* call) {
  SymExpr* se = toSymExpr(call->get(1));
  int size = 0;
  for (int i = 0; i < se->symbol()->type->substitutions.n; i++) {
    if (se->symbol()->type->substitutions.v[i].key) {
      if (!strcmp("size", se->symbol()->type->substitutions.v[i].key->name)) {
        size = toVarSymbol(se->symbol()->type->substitutions.v[i].value)->immediate->int_value();
        break;
      }
    }
  }
  INT_ASSERT(size);
  CallExpr* noop = new CallExpr(PRIM_NOOP);
  call->getStmtExpr()->insertBefore(noop);
  VarSymbol* tmp = gTrue;
  for (int i = 1; i <= size; i++) {
    VarSymbol* tmp1 = newTemp("_tuple_and_expand_tmp_");
    tmp1->addFlag(FLAG_MAYBE_PARAM);
    tmp1->addFlag(FLAG_MAYBE_TYPE);
    VarSymbol* tmp2 = newTemp("_tuple_and_expand_tmp_");
    tmp2->addFlag(FLAG_MAYBE_PARAM);
    tmp2->addFlag(FLAG_MAYBE_TYPE);
    VarSymbol* tmp3 = newTemp("_tuple_and_expand_tmp_");
    tmp3->addFlag(FLAG_MAYBE_PARAM);
    tmp3->addFlag(FLAG_MAYBE_TYPE);
    VarSymbol* tmp4 = newTemp("_tuple_and_expand_tmp_");
    tmp4->addFlag(FLAG_MAYBE_PARAM);
    tmp4->addFlag(FLAG_MAYBE_TYPE);
    call->getStmtExpr()->insertBefore(new DefExpr(tmp1));
    call->getStmtExpr()->insertBefore(new DefExpr(tmp2));
    call->getStmtExpr()->insertBefore(new DefExpr(tmp3));
    call->getStmtExpr()->insertBefore(new DefExpr(tmp4));
    call->getStmtExpr()->insertBefore(
      new CallExpr(PRIM_MOVE, tmp1,
                   new CallExpr(se->copy(), new_IntSymbol(i))));
    CallExpr* query = new CallExpr(PRIM_QUERY, tmp1);
    for (int i = 2; i < call->numActuals(); i++)
      query->insertAtTail(call->get(i)->copy());
    call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp2, query));
    call->getStmtExpr()->insertBefore(
      new CallExpr(PRIM_MOVE, tmp3,
                   new CallExpr("==", tmp2, call->get(3)->copy())));
    call->getStmtExpr()->insertBefore(
      new CallExpr(PRIM_MOVE, tmp4,
                   new CallExpr("&", tmp3, tmp)));
    tmp = tmp4;
  }
  call->replace(new SymExpr(tmp));
  noop->replace(call); // put call back in ast for function resolution
  makeNoop(call);
}

static void resolveTupleExpand(CallExpr* call) {
  SymExpr* sym = toSymExpr(call->get(1));
  Type* type = sym->symbol()->getValType();

  if (!type->symbol->hasFlag(FLAG_TUPLE))
    USR_FATAL(call, "invalid tuple expand primitive");

  int size = 0;
  for (int i = 0; i < type->substitutions.n; i++) {
    if (type->substitutions.v[i].key) {
      if (!strcmp("size", type->substitutions.v[i].key->name)) {
        size = toVarSymbol(type->substitutions.v[i].value)->immediate->int_value();
        break;
      }
    }
  }
  if (size == 0)
    INT_FATAL(call, "Invalid tuple expand primitive");
  CallExpr* parent = toCallExpr(call->parentExpr);
  CallExpr* noop = new CallExpr(PRIM_NOOP);
  call->getStmtExpr()->insertBefore(noop);
  for (int i = 1; i <= size; i++) {
    VarSymbol* tmp = newTemp("_tuple_expand_tmp_");
    tmp->addFlag(FLAG_MAYBE_TYPE);
    if (sym->symbol()->hasFlag(FLAG_TYPE_VARIABLE))
      tmp->addFlag(FLAG_TYPE_VARIABLE);
    DefExpr* def = new DefExpr(tmp);
    call->getStmtExpr()->insertBefore(def);
    CallExpr* e = NULL;
    if (!call->parentSymbol->hasFlag(FLAG_EXPAND_TUPLES_WITH_VALUES)) {
      e = new CallExpr(sym->copy(), new_IntSymbol(i));
    } else {
      e = new CallExpr(PRIM_GET_MEMBER_VALUE, sym->copy(),
                       new_CStringSymbol(astr("x", istr(i))));
    }
    CallExpr* move = new CallExpr(PRIM_MOVE, tmp, e);
    call->getStmtExpr()->insertBefore(move);
    call->insertBefore(new SymExpr(tmp));
  }
  call->remove();
  noop->replace(call); // put call back in ast for function resolution
  makeNoop(call);
  // increase tuple rank
  if (parent && parent->isNamed("_type_construct__tuple")) {
    parent->get(1)->replace(new SymExpr(new_IntSymbol(parent->numActuals()-1)));
  }
}

static void resolveSetMember(CallExpr* call) {
  // Get the field name.
  SymExpr* sym = toSymExpr(call->get(2));
  if (!sym)
    INT_FATAL(call, "bad set member primitive");
  VarSymbol* var = toVarSymbol(sym->symbol());
  if (!var || !var->immediate)
    INT_FATAL(call, "bad set member primitive");
  const char* name = var->immediate->v_string;

  // Special case: An integer field name is actually a tuple member index.
  {
    int64_t i;
    if (get_int(sym, &i)) {
      name = astr("x", istr(i));
      call->get(2)->replace(new SymExpr(new_CStringSymbol(name)));
    }
  }

  AggregateType* ct = toAggregateType(call->get(1)->typeInfo());
  if (!ct)
    INT_FATAL(call, "bad set member primitive");

  Symbol* fs = NULL;
  for_fields(field, ct) {
    if (!strcmp(field->name, name)) {
      fs = field; break;
    }
  }

  if (!fs)
    INT_FATAL(call, "bad set member primitive");

  Type* t = call->get(3)->typeInfo();
  // I think this never happens, so can be turned into an assert. <hilde>
  if (t == dtUnknown)
    INT_FATAL(call, "Unable to resolve field type");

  if (t == dtNil && fs->type == dtUnknown)
    USR_FATAL(call->parentSymbol, "unable to determine type of field from nil");
  if (fs->type == dtUnknown) {
    // Set the field type.
    fs->type = t;
  }

  INT_ASSERT(isSymExpr(call->get(3)));
  handleSetMemberTypeMismatch(t, fs, call, toSymExpr(call->get(3)));
}


static void handleSetMemberTypeMismatch(Type* t, Symbol* fs, CallExpr* call,
                                        SymExpr* rhs) {
  if (t != fs->type && t != dtNil && t != dtObject) {
    Symbol* actual = rhs->symbol();
    FnSymbol* fn = toFnSymbol(call->parentSymbol);
    if (canCoerceTuples(t, actual, fs->type, fn)) {
      // Add a PRIM_MOVE so that insertCasts will take care of it later.
      VarSymbol* tmp = newTemp("coerce_elt", fs->type);
      call->insertBefore(new DefExpr(tmp));
      rhs->remove();
      call->insertBefore(new CallExpr(PRIM_MOVE, tmp, rhs));
      call->insertAtTail(tmp);
    } else {
      USR_FATAL(userCall(call),
                "cannot assign expression of type %s to field of type %s",
                toString(t), toString(fs->type));
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
      AggregateType* instantiate = ct->getInstantiation(rhs, index);
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
                "cannot assign expression of type %s to field of type %s",
                toString(t),
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

  if (dst->hasFlag(FLAG_NO_COPY)                         == true)  {
    call->primitive = primitives[PRIM_MOVE];
    resolveMove(call);

  } else if (isPrimitiveScalar(srcType)                  == true)  {
    call->primitive = primitives[PRIM_MOVE];
    resolveMove(call);

  } else if (isNonGenericRecordWithInitializers(srcType) == true)  {
    AggregateType* ct  = toAggregateType(srcType);
    SymExpr*       rhs = toSymExpr(call->get(2));

    // The LHS will "own" the record
    if (rhs->symbol()->hasFlag(FLAG_INSERT_AUTO_DESTROY) == false) {
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

static void resolveMove(CallExpr* call) {
  if (call->id == breakOnResolveID )
    gdbShouldBreakHere();

  Expr* rhs = call->get(2);
  Symbol* lhs = NULL;
  if (SymExpr* se = toSymExpr(call->get(1)))
    lhs = se->symbol();
  INT_ASSERT(lhs);

  FnSymbol* fn = toFnSymbol(call->parentSymbol);
  bool isReturn = fn ? lhs == fn->getReturnSymbol() : false;
  bool rhsIsTypeExpr = isTypeExpr(rhs);

  if (lhs->hasFlag(FLAG_TYPE_VARIABLE) && !rhsIsTypeExpr) {
    if (isReturn) {
      if (!call->parentSymbol->hasFlag(FLAG_RUNTIME_TYPE_INIT_FN))
        USR_FATAL(call, "illegal return of value where type is expected");
    } else {
      USR_FATAL(call, "illegal assignment of value to type");
    }
  }

  if (!lhs->hasFlag(FLAG_TYPE_VARIABLE) && !lhs->hasFlag(FLAG_MAYBE_TYPE) && rhsIsTypeExpr) {
    if (isReturn) {
      USR_FATAL(call, "illegal return of type where value is expected");
    } else {
      if (lhs->hasFlag(FLAG_CHPL__ITER))
        USR_FATAL(call, "unable to iterate over type '%s'",
                  toString(rhs->getValType()));
      else
        USR_FATAL(call, "illegal assignment of type to value");
    }
  }

  // do not resolve function return type yet
  // except for constructors
  if (fn && call->parentExpr != fn->where && call->parentExpr != fn->retExprType &&
      isReturn && fn->_this != lhs) {

    if (fn->retType == dtUnknown) {
      return;
    }
  }

  Type* rhsType = rhs->typeInfo();

  // This is a workaround for order-of-resolution problems with
  // extern type aliases
  if (rhsIsTypeExpr && rhs->typeInfo() == dtUnknown && isSymExpr(rhs)) {
    // Try resolving type aliases now.
    rhsType = resolveTypeAlias(toSymExpr(rhs));
  }

  // This is a workaround for problems where the _iterator
  // in buildForLoopExpr would be an _array instead of a ref(_array)
  // in 4-init-array-forexpr.chpl. This could be improved with
  // QualifiedType.
  if (lhs->hasFlag(FLAG_MAYBE_REF) && !isReferenceType(rhsType)) {
    if (SymExpr* se = toSymExpr(rhs)) {
      if (ArgSymbol* arg = toArgSymbol(se->symbol())) {
        if (concreteIntent(arg->intent, arg->type) & INTENT_FLAG_REF) {
          makeRefType(rhsType);
          rhsType = rhsType->refType;

          // Add PRIM_ADDR_OF
          //  (this won't be necessary with QualifiedType/PRIM_SET_REFERENCE)
          VarSymbol* addrOfTmp = newTemp("moveAddr", rhsType);
          call->insertBefore(new DefExpr(addrOfTmp));
          call->insertBefore(new CallExpr(PRIM_MOVE, addrOfTmp,
                                        new CallExpr(PRIM_ADDR_OF,
                                                     rhs->copy())));
          SymExpr* newRhs = new SymExpr(addrOfTmp);
          rhs->replace(newRhs);
          rhs = newRhs;
        }
      }
    }
  }

  if (lhs->type == dtUnknown || lhs->type == dtNil) {
    if (lhs->id == breakOnResolveID )
      gdbShouldBreakHere();

    INT_ASSERT(rhsType);
    lhs->type = rhsType;
  }

  Type* lhsType = lhs->type;

  setConstFlagsAndCheckUponMove(lhs, rhs);

  if (CallExpr* call = toCallExpr(rhs)) {
    if (FnSymbol* fn = call->isResolved()) {
      if (rhsType == dtUnknown) {
        USR_FATAL_CONT(fn, "unable to resolve return type of function '%s'", fn->name);
        USR_FATAL(rhs, "called recursively at this point");
      }
    }
  }
  if (rhsType == dtUnknown)
    USR_FATAL(call, "unable to resolve type");

  if (rhsType == dtNil && lhsType != dtNil && !isClass(lhsType))
    USR_FATAL(userCall(call), "type mismatch in assignment from nil to %s",
              toString(lhsType));
  Type* lhsBaseType = lhsType->getValType();
  Type* rhsBaseType = rhsType->getValType();
  bool isChplHereAlloc = false;
  // Fix up calls inserted by callChplHereAlloc()
  if (CallExpr* rhsCall = toCallExpr(rhs)) {
    // Here we are going to fix up calls inserted by
    // callChplHereAlloc() and callChplHereFree()
    //
    // Currently this code assumes that such primitives are only
    // inserted by the compiler.  TODO: Add a check to make sure
    // these are the ones added by the above functions.
    //
    if (rhsCall->isPrimitive(PRIM_SIZEOF)) {
      // Fix up arg to sizeof(), as we may not have known the
      // type earlier
      SymExpr* sizeSym = toSymExpr(rhsCall->get(1));
      INT_ASSERT(sizeSym);
      rhs->replace(new CallExpr(PRIM_SIZEOF, sizeSym->symbol()->typeInfo()->symbol));
      return;
    } else if (rhsCall->isPrimitive(PRIM_CAST_TO_VOID_STAR)) {
      if (isReferenceType(rhsCall->get(1)->typeInfo())) {
        // Add a dereference as needed, as we did not have complete
        // type information earlier
        SymExpr* castVar = toSymExpr(rhsCall->get(1));
        INT_ASSERT(castVar);
        VarSymbol* derefTmp = newTemp("castDeref", castVar->typeInfo()->getValType());
        call->insertBefore(new DefExpr(derefTmp));
        call->insertBefore(new CallExpr(PRIM_MOVE, derefTmp,
                                        new CallExpr(PRIM_DEREF,
                                                     new SymExpr(castVar->symbol()))));
        rhsCall->replace(new CallExpr(PRIM_CAST_TO_VOID_STAR,
                                      new SymExpr(derefTmp)));
      }
    } else if (rhsCall->isResolved() == gChplHereAlloc) {
      // Insert cast below for calls to chpl_here_*alloc()
      isChplHereAlloc = true;
    }
  }
  if (!isChplHereAlloc && rhsType != dtNil &&
      rhsBaseType != lhsBaseType &&
      !isDispatchParent(rhsBaseType, lhsBaseType))
    USR_FATAL(userCall(call), "type mismatch in assignment from %s to %s",
              toString(rhsType), toString(lhsType));
  if (isChplHereAlloc ||
      (rhsType != lhsType && isDispatchParent(rhsBaseType, lhsBaseType))) {
    Symbol* tmp = newTemp("cast_tmp", rhsType);
    call->insertBefore(new DefExpr(tmp));
    call->insertBefore(new CallExpr(PRIM_MOVE, tmp, rhs->remove()));
    call->insertAtTail(new CallExpr(PRIM_CAST,
                                    isChplHereAlloc ? lhs->type->symbol :
                                    lhsBaseType->symbol, tmp));
  }

  // Fix up PRIM_COERCE : remove it if it has a param RHS.
  CallExpr* rhsCall = toCallExpr(rhs);

  if (rhsCall && rhsCall->isPrimitive(PRIM_COERCE)) {
    SymExpr* toCoerceSE = toSymExpr(rhsCall->get(1));
    if (toCoerceSE) {
      Symbol* toCoerceSym = toCoerceSE->symbol();
      bool promotes = false;
      // This transformation is normally handled in insertCasts
      // but we need to do it earlier for parameters. We can't just
      // call insertCasts here since that would dramatically change the
      // resolution order (and would be apparently harder to get working).
      if (toCoerceSym->isParameter() ||
          toCoerceSym->hasFlag(FLAG_TYPE_VARIABLE) ) {
        // Can we coerce from the argument to the function return type?
        // Note that rhsType here is the function return type (since
        // that is what the primitive returns as its type).
        if (toCoerceSym->type == rhsType ||
            canParamCoerce(toCoerceSym->type, toCoerceSym, rhsType)) {
          // Replacing the arguments to the move works, but for
          // some reason replacing the whole move doesn't.
          call->get(1)->replace(new SymExpr(lhs));
          call->get(2)->replace(new SymExpr(toCoerceSym));
        } else if (canCoerce(toCoerceSym->type, toCoerceSym, rhsType,
                             NULL, &promotes)) {
          // any case that doesn't param coerce but does coerce will
          // be handled in insertCasts later.
        } else {
          USR_FATAL(userCall(call), "type mismatch in return from %s to %s",
                    toString(toCoerceSym->type), toString(rhsType));
        }
      }
    }
  }
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

static SymExpr* primNewTypeExpr(CallExpr* call);

static void resolveNew(CallExpr* call) {
  if (SymExpr* typeExpr = primNewTypeExpr(call)) {
    if (Type* type = resolveTypeAlias(typeExpr)) {
      if (AggregateType* at = toAggregateType(type)) {
        SET_LINENO(call);

        // Begin to support new-style initializers
        if (at->initializerStyle == DEFINES_INITIALIZER) {
          if (at->symbol->hasFlag(FLAG_GENERIC) == false) {
            VarSymbol* newTmp = newTemp("new_temp", at);
            DefExpr*   def    = new DefExpr(newTmp);

            if (isClass(at) == true) {
              typeExpr->replace(new UnresolvedSymExpr("_new"));
            } else {
              typeExpr->replace(new UnresolvedSymExpr("init"));
            }

            // Convert the PRIM_NEW to a normal call
            call->primitive = NULL;
            call->baseExpr  = call->get(1)->remove();

            parent_insert_help(call, call->baseExpr);

            if (isBlockStmt(call->parentExpr) == true) {
              call->insertBefore(def);
            } else {
              call->parentExpr->insertBefore(def);
            }

            if (isClass(at) == true) {
              // Invoking a type  method
              call->insertAtHead(new SymExpr(at->symbol));

            } else {
              // Invoking an instance method
              call->insertAtHead(new SymExpr(newTmp));
              call->insertAtHead(new SymExpr(gMethodToken));
            }

            resolveExpr(call);

          } else {
            typeExpr->replace(new UnresolvedSymExpr("init"));
            // call special case function for generic initializers
            modAndResolveInitCall(call, at);
          }

        // Continue to support old-style constructors
        } else {
          FnSymbol* ctInit = at->defaultInitializer;

          typeExpr->replace(new UnresolvedSymExpr(ctInit->name));

          // Convert the PRIM_NEW to a normal call
          call->primitive = NULL;
          call->baseExpr  = call->get(1)->remove();

          parent_insert_help(call, call->baseExpr);

          resolveExpr(call);
        }
      }
    }

  } else {
    if (Expr* arg = call->get(1)) {
      if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(arg)) {
        USR_FATAL(call, "invalid use of 'new' on %s", urse->unresolved);
        return;

      } else if (CallExpr* subCall = toCallExpr(arg)) {
        if (FnSymbol* fn = subCall->isResolved()) {
          USR_FATAL(call, "invalid use of 'new' on %s", fn->name);
          return;
        }
      }
    }

    USR_FATAL(call, "invalid use of 'new'");
  }
}

// Find the SymExpr that captures the type
static SymExpr* primNewTypeExpr(CallExpr* call) {
  Expr*    arg1   = call->get(1);
  SymExpr* retval = NULL;

  // The common case e.g new MyClass(1, 2, 3);
  if (SymExpr* se = toSymExpr(arg1)) {
    retval = se;

  // 'new' (call (partial) R2 _mt this), call_tmp0, call_tmp1, ...
  // due to nested classes (i.e. R2 is a nested class type)
  } else if (CallExpr* subCall = toCallExpr(arg1)) {
    if (SymExpr* se = toSymExpr(subCall->baseExpr)) {
      retval = (subCall->partialTag) ? se : NULL;
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

    Type* retType = getReturnedTupleType(fn, toType);
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
     // inlined.
     //
     ((formal->intent == INTENT_IN || formal->intent == INTENT_CONST_IN) &&
      (backendRequiresCopyForIn(formal->type)))
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
        VarSymbol* refTmp  = newTemp("_formal_ref_tmp_");
        VarSymbol* typeTmp = newTemp("_formal_type_tmp_");

        typeTmp->addFlag(FLAG_MAYBE_TYPE);

        fn->insertAtHead(new CallExpr(PRIM_MOVE,
                                      tmp,
                                      new CallExpr(PRIM_INIT, typeTmp)));

        fn->insertAtHead(new CallExpr(PRIM_MOVE,
                                      typeTmp,
                                      new CallExpr(PRIM_TYPEOF, refTmp)));

        fn->insertAtHead(new CallExpr(PRIM_MOVE,
                                      refTmp,
                                      new CallExpr(PRIM_DEREF, formal)));

        fn->insertAtHead(new DefExpr(refTmp));

        fn->insertAtHead(new DefExpr(typeTmp));
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

//
//
//

static Expr* dropUnnecessaryCast(CallExpr* call) {
  // Check for and remove casts to the original type and size
  Expr* result = call;
  if (!call->isCast())
    INT_FATAL("dropUnnecessaryCasts called on non cast call");

  if (SymExpr* sym = toSymExpr(call->castFrom())) {
    if (LcnSymbol* var = toLcnSymbol(sym->symbol())) {
      // Casts of type variables are always required
      // eg. foo.type:string
      if (!var->hasFlag(FLAG_TYPE_VARIABLE)) {
        if (SymExpr* sym = toSymExpr(call->castTo())) {
          Type* oldType = var->type->getValType();
          Type* newType = sym->symbol()->type->getValType();

          if (newType == oldType) {
            result = new SymExpr(var);
            call->replace(result);
          }
        }
      }
    } else if (EnumSymbol* e = toEnumSymbol(sym->symbol())) {
      if (SymExpr* sym = toSymExpr(call->castTo())) {
        EnumType* oldType = toEnumType(e->type);
        EnumType* newType = toEnumType(sym->symbol()->type);
        if (newType && oldType == newType) {
          result = new SymExpr(e);
          call->replace(result);
        }
      }
    }
  }
  return result;
}

/*
  Creates the parent class which will represent the function's type.
  Children of the parent class will capture different functions which
  happen to share the same function type.  By using the parent class
  we can assign new values onto variable that match the function type
  but may currently be pointing at a different function.
*/
static AggregateType* createAndInsertFunParentClass(CallExpr*   call,
                                                    const char* name) {
  AggregateType* parent    = new AggregateType(AGGREGATE_CLASS);
  TypeSymbol*    parent_ts = new TypeSymbol(name, parent);

  parent_ts->addFlag(FLAG_FUNCTION_CLASS);

  // Because this function type needs to be globally visible (because
  // we don't know the modules it will be passed to), we put it at the
  // highest scope
  theProgram->block->body.insertAtTail(new DefExpr(parent_ts));

  parent->dispatchParents.add(dtObject);
  dtObject->dispatchChildren.add(parent);

  VarSymbol* parent_super = new VarSymbol("super", dtObject);
  parent_super->addFlag(FLAG_SUPER_CLASS);
  parent->fields.insertAtHead(new DefExpr(parent_super));

  build_constructors(parent);
  buildDefaultDestructor(parent);

  return parent;
}

/*
  To mimic a function call, we create a .this method for the parent class.
  This will allow the object to look and feel like a first-class function,
  by both being an object and being invoked using parentheses syntax.

  Children of the parent class will override this method and wrap the
  function that is being used as a first-class value.

  To focus on just the types of the arguments and not their names or
  default values, we use the parent method's names and types as the basis
  for all children which override it.

  The function is put at the highest scope so that all functions of a given
  type will share the same parent class.
*/
static FnSymbol* createAndInsertFunParentMethod(CallExpr*      call,
                                                AggregateType* parent,
                                                AList&         arg_list,
                                                bool           isFormal,
                                                Type*          retType) {
  // Add a "getter" method that returns the return type of the function
  //
  //   * The return type itself is not actually stored as a member variable
  //
  //   * A function that returns void will similarly cause a call to retType
  //     to return void, which is to say it doesn't return anything.  This is
  //     maybe not terribly intuitive to the user, but should be resolved
  //     when "void" becomes a fully-featured type.
  {
    FnSymbol* rtGetter = new FnSymbol("retType");

    rtGetter->addFlag(FLAG_NO_IMPLICIT_COPY);
    rtGetter->addFlag(FLAG_INLINE);
    rtGetter->retTag = RET_TYPE;
    rtGetter->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                               "_mt",
                                               dtMethodToken));

    ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", parent);

    _this->addFlag(FLAG_ARG_THIS);
    rtGetter->insertFormalAtTail(_this);
    rtGetter->insertAtTail(new CallExpr(PRIM_RETURN, retType->symbol));

    DefExpr* def = new DefExpr(rtGetter);

    parent->symbol->defPoint->insertBefore(def);
    normalize(rtGetter);
    parent->methods.add(rtGetter);

    rtGetter->addFlag(FLAG_METHOD);
    rtGetter->addFlag(FLAG_METHOD_PRIMARY);
    rtGetter->cname = astr("chpl_get_",
                           parent->symbol->cname,
                           "_",
                           rtGetter->cname);
    rtGetter->addFlag(FLAG_NO_PARENS);
    rtGetter->_this = _this;
  }

  // Add a "getter" method that returns the tuple of argument types
  // for the function
  {
    FnSymbol* atGetter = new FnSymbol("argTypes");

    atGetter->addFlag(FLAG_NO_IMPLICIT_COPY);
    atGetter->addFlag(FLAG_INLINE);
    atGetter->retTag = RET_TYPE;
    atGetter->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                               "_mt",
                                               dtMethodToken));

    CallExpr* expr = new CallExpr(PRIM_ACTUALS_LIST);

    if (isFormal) {
      for_alist(formalExpr, arg_list) {
        DefExpr*   dExp = toDefExpr(formalExpr);
        ArgSymbol* fArg = toArgSymbol(dExp->sym);

        expr->insertAtTail(fArg->type->symbol);
      }
    }
    else {
      for_alist(actualExpr, arg_list) {
        if (actualExpr != arg_list.tail) {
          SymExpr* sExpr = toSymExpr(actualExpr);

          expr->insertAtTail(sExpr->symbol()->type->symbol);
        }
      }
    }

    ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", parent);

    _this->addFlag(FLAG_ARG_THIS);

    atGetter->insertFormalAtTail(_this);
    atGetter->insertAtTail(new CallExpr(PRIM_RETURN,
                                        new CallExpr("_build_tuple", expr)));

    DefExpr* def = new DefExpr(atGetter);

    parent->symbol->defPoint->insertBefore(def);
    normalize(atGetter);
    parent->methods.add(atGetter);

    atGetter->addFlag(FLAG_METHOD);
    atGetter->addFlag(FLAG_METHOD_PRIMARY);
    atGetter->cname = astr("chpl_get_",
                           parent->symbol->cname, "_",
                           atGetter->cname);
    atGetter->addFlag(FLAG_NO_PARENS);
    atGetter->_this = _this;
  }

  FnSymbol* parent_method = new FnSymbol("this");

  parent_method->addFlag(FLAG_FIRST_CLASS_FUNCTION_INVOCATION);
  parent_method->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                                  "_mt",
                                                  dtMethodToken));
  parent_method->addFlag(FLAG_METHOD);

  ArgSymbol* thisParentSymbol = new ArgSymbol(INTENT_BLANK, "this", parent);

  thisParentSymbol->addFlag(FLAG_ARG_THIS);

  parent_method->insertFormalAtTail(thisParentSymbol);
  parent_method->_this = thisParentSymbol;

  int i = 0, alength = arg_list.length;

  // We handle the arg list differently depending on if it's a list of
  // formal args or actual args
  if (isFormal) {

    for_alist(formalExpr, arg_list) {
      DefExpr* dExp = toDefExpr(formalExpr);
      ArgSymbol* fArg = toArgSymbol(dExp->sym);

      if (fArg->type != dtVoid) {
        ArgSymbol* newFormal = new ArgSymbol(INTENT_BLANK,
                                             fArg->name,
                                             fArg->type);

        if (fArg->typeExpr)
          newFormal->typeExpr = fArg->typeExpr->copy();

        parent_method->insertFormalAtTail(newFormal);
      }
    }

  } else {
    char name_buffer[100];
    int  name_index = 0;

    for_alist(actualExpr, arg_list) {
      sprintf(name_buffer, "name%i", name_index++);

      if (i != (alength-1)) {
        SymExpr* sExpr = toSymExpr(actualExpr);

        if (sExpr->symbol()->type != dtVoid) {
          ArgSymbol* newFormal = new ArgSymbol(INTENT_BLANK,
                                               name_buffer,
                                               sExpr->symbol()->type);

          parent_method->insertFormalAtTail(newFormal);
        }
      }

      ++i;
    }
  }

  if (retType != dtVoid) {
    VarSymbol *tmp = newTemp("_return_tmp_", retType);

    parent_method->insertAtTail(new DefExpr(tmp));
    parent_method->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
  }

  // Because this function type needs to be globally visible
  // (because we don't know the modules it will be passed to), we put
  // it at the highest scope
  theProgram->block->body.insertAtTail(new DefExpr(parent_method));

  normalize(parent_method);

  parent->methods.add(parent_method);

  return parent_method;
}

/*
  Builds up the name of the parent for lookup by looking through the types
  of the arguments, either formal or actual
*/
static std::string buildParentName(AList& arg_list,
                                   bool   isFormal,
                                   Type*  retType) {
  std::ostringstream oss;
  bool               isFirst = true;

  oss << "chpl__fcf_type_";

  if (isFormal) {
    if (arg_list.length == 0) {
      oss << "void";

    } else {
      for_alist(formalExpr, arg_list) {
        DefExpr* dExp = toDefExpr(formalExpr);
        ArgSymbol* fArg = toArgSymbol(dExp->sym);

        if (!isFirst)
          oss << "_";

        oss << fArg->type->symbol->cname;

        isFirst = false;
      }
    }
    oss << "_";
    oss << retType->symbol->cname;

  } else {
    int i = 0, alength = arg_list.length;

    if (alength == 1) {
      oss << "void_";
    }

    for_alist(actualExpr, arg_list) {
      if (!isFirst)
        oss << "_";

      SymExpr* sExpr = toSymExpr(actualExpr);

      ++i;

      oss << sExpr->symbol()->type->symbol->cname;

      isFirst = false;
    }
  }

  return oss.str();
}

/*
  Helper function for creating or finding the parent class for a given
  function type specified by the type signature.  The last type given
  in the signature is the return type, the remainder represent arguments
  to the function.
*/
static AggregateType* createOrFindFunTypeFromAnnotation(AList&     argList,
                                                        CallExpr*  call) {
  AggregateType* parent      = NULL;
  SymExpr*       retTail     = toSymExpr(argList.tail);
  Type*          retType     = retTail->symbol()->type;
  std::string    parent_name = buildParentName(argList, false, retType);

  if (functionTypeMap.find(parent_name) != functionTypeMap.end()) {
    parent = functionTypeMap[parent_name].first;

  } else {
    FnSymbol* parentMethod = NULL;

    parent       = createAndInsertFunParentClass(call,
                                                 parent_name.c_str());
    parentMethod = createAndInsertFunParentMethod(call,
                                                  parent,
                                                  argList,
                                                  false,
                                                  retType);

    functionTypeMap[parent_name] = std::pair<AggregateType*,
                                             FnSymbol*>(parent, parentMethod);
  }

  return parent;
}

/*
  Captures a function as a first-class value by creating an object that will
  represent the function.  The class is created at the same scope as the
  function being referenced.  Each class is unique and shared among all
  uses of that function as a value.  Once built, the class will override
  the .this method of the parent and wrap the call to the function being
  captured as a value.  Then, an instance of the class is instantiated and
  returned.
*/
static Expr* createFunctionAsValue(CallExpr *call) {
  static int unique_fcf_id = 0;

  UnresolvedSymExpr* use    = toUnresolvedSymExpr(call->get(1));
  const char*        flname = use->unresolved;

  Vec<FnSymbol*>     visibleFns;
  Vec<BlockStmt*>    visited;

  getVisibleFunctions(getVisibilityBlock(call),
                      flname,
                      visibleFns,
                      visited,
                      call);

  if (visibleFns.n > 1) {
    USR_FATAL(call, "%s: can not capture overloaded functions as values",
                    visibleFns.v[0]->name);
  }

  INT_ASSERT(visibleFns.n == 1);

  FnSymbol* captured_fn = visibleFns.head();

  if (call->isPrimitive(PRIM_CAPTURE_FN_FOR_CHPL)) {
    //
    // If we're doing a Chapel first-class function, we can re-use the
    // cached first-class function information from an earlier call if
    // there was one, so check to see if we've already cached the
    // capture somewhere.
    //
    if (functionCaptureMap.find(captured_fn) != functionCaptureMap.end()) {
      return new CallExpr(functionCaptureMap[captured_fn]);
    }
  }

  resolveFormals(captured_fn);
  resolveFnForCall(captured_fn, call);

  //
  // When all we need is a C pointer, we can cut out here, returning
  // a reference to the function symbol.
  //
  if (call->isPrimitive(PRIM_CAPTURE_FN_FOR_C)) {
    return new SymExpr(captured_fn);
  }
  //
  // Otherwise, we need to create a Chapel first-class function (fcf)...
  //

  AggregateType *parent;
  FnSymbol *thisParentMethod;

  std::string parent_name = buildParentName(captured_fn->formals, true, captured_fn->retType);

  if (functionTypeMap.find(parent_name) != functionTypeMap.end()) {
    std::pair<AggregateType*, FnSymbol*> ctfs = functionTypeMap[parent_name];
    parent = ctfs.first;
    thisParentMethod = ctfs.second;
  }
  else {
    parent = createAndInsertFunParentClass(call, parent_name.c_str());
    thisParentMethod = createAndInsertFunParentMethod(call, parent, captured_fn->formals, true, captured_fn->retType);
    functionTypeMap[parent_name] = std::pair<AggregateType*, FnSymbol*>(parent, thisParentMethod);
  }

  AggregateType *ct = new AggregateType(AGGREGATE_CLASS);
  std::ostringstream fcf_name;
  fcf_name << "_chpl_fcf_" << unique_fcf_id++ << "_" << flname;

  TypeSymbol *ts = new TypeSymbol(astr(fcf_name.str().c_str()), ct);

  call->parentExpr->insertBefore(new DefExpr(ts));

  ct->dispatchParents.add(parent);
  bool inserted = parent->dispatchChildren.add_exclusive(ct);
  INT_ASSERT(inserted);
  VarSymbol* super = new VarSymbol("super", parent);
  super->addFlag(FLAG_SUPER_CLASS);
  ct->fields.insertAtHead(new DefExpr(super));

  build_constructors(ct);
  buildDefaultDestructor(ct);

  FnSymbol *thisMethod = new FnSymbol("this");
  thisMethod->addFlag(FLAG_FIRST_CLASS_FUNCTION_INVOCATION);
  thisMethod->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  thisMethod->addFlag(FLAG_METHOD);
  ArgSymbol *thisSymbol = new ArgSymbol(INTENT_BLANK, "this", ct);
  thisSymbol->addFlag(FLAG_ARG_THIS);
  thisMethod->insertFormalAtTail(thisSymbol);
  thisMethod->_this = thisSymbol;

  CallExpr *innerCall = new CallExpr(captured_fn);

  int skip = 2;
  for_alist(formalExpr, thisParentMethod->formals) {
    //Skip the first two arguments from the parent, which are _mt and this
    if (skip) {
      --skip;
      continue;
    }

    DefExpr* dExp = toDefExpr(formalExpr);
    ArgSymbol* fArg = toArgSymbol(dExp->sym);

    if (fArg->type->symbol->hasFlag(FLAG_GENERIC)) {
      USR_FATAL_CONT("Generic functions can't be captured as values");
    }

    ArgSymbol* newFormal = new ArgSymbol(INTENT_BLANK, fArg->name, fArg->type);
    if (fArg->typeExpr)
      newFormal->typeExpr = fArg->typeExpr->copy();
    SymExpr* argSym = new SymExpr(newFormal);
    innerCall->insertAtTail(argSym);

    thisMethod->insertFormalAtTail(newFormal);
  }

  std::vector<CallExpr*> calls;
  collectCallExprs(captured_fn, calls);

  for_vector(CallExpr, cl, calls) {
    if (cl->isPrimitive(PRIM_YIELD)) {
      USR_FATAL_CONT(cl, "Iterators not allowed in first class functions");
    }
  }

  if (captured_fn->retType == dtVoid) {
    thisMethod->insertAtTail(innerCall);
  }
  else {
    VarSymbol *tmp = newTemp("_return_tmp_");
    thisMethod->insertAtTail(new DefExpr(tmp));
    thisMethod->insertAtTail(new CallExpr(PRIM_MOVE, tmp, innerCall));

    thisMethod->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
  }

  call->parentExpr->insertBefore(new DefExpr(thisMethod));
  normalize(thisMethod);

  ct->methods.add(thisMethod);

  FnSymbol *wrapper = new FnSymbol("wrapper");
  wrapper->addFlag(FLAG_INLINE);

  wrapper->insertAtTail(new CallExpr(PRIM_RETURN, new CallExpr(PRIM_CAST, parent->symbol, new CallExpr(ct->defaultInitializer))));

  call->getStmtExpr()->insertBefore(new DefExpr(wrapper));

  normalize(wrapper);

  CallExpr *call_wrapper = new CallExpr(wrapper);
  functionCaptureMap[captured_fn] = wrapper;

  return call_wrapper;
}

// This function returns true for user fields, including
// const, param, and type fields.
// It returns false for compiler-introduced fields like
// super and outer.
static bool
isNormalField(Symbol* field)
{
  if( field->hasFlag(FLAG_IMPLICIT_ALIAS_FIELD) ) return false;
  if( field->hasFlag(FLAG_SUPER_CLASS) ) return false;
  // TODO -- this will break user fields named outer!
  if( 0 == strcmp("outer", field->name)) return false;

  return true;
}

static CallExpr* toPrimToLeaderCall(Expr* expr) {
  if (CallExpr* call = toCallExpr(expr))
    if (call->isPrimitive(PRIM_TO_LEADER) ||
        call->isPrimitive(PRIM_TO_STANDALONE))
      return call;
  return NULL;
}

static SymExpr* symOrParamExpr(Symbol* arg) {
  Symbol* result = arg;
  if (Symbol* paramVal = paramMap.get(arg))
    result = paramVal;
  return new SymExpr(result);
}

// Recursively resolve typedefs
static Type* resolveTypeAlias(SymExpr* se)
{
  if (! se)
    return NULL;

  // Quick exit if the type is already known.
  Type* result = se->getValType();
  if (result != dtUnknown)
    return result;

  VarSymbol* var = toVarSymbol(se->symbol());
  if (! var)
    return NULL;

  DefExpr* def = var->defPoint;
  SET_LINENO(def);
  Expr* typeExpr = resolve_type_expr(def->init);
  SymExpr* tse = toSymExpr(typeExpr);

  return resolveTypeAlias(tse);
}


// Returns NULL if no substitution was made.  Otherwise, returns the expression
// that replaced 'call'.
static Expr* resolveTupleIndexing(CallExpr* call, Symbol* baseVar)
{
  if (call->numActuals() != 3)
    USR_FATAL(call, "illegal tuple indexing expression");
  Type* indexType = call->get(3)->getValType();
  if (!is_int_type(indexType) && !is_uint_type(indexType))
    USR_FATAL(call, "tuple indexing expression is not of integral type");

  AggregateType* baseType = toAggregateType(baseVar->getValType());
  int64_t index;
  uint64_t uindex;
  char field[8];

  if (get_int(call->get(3), &index)) {
    sprintf(field, "x%" PRId64, index);
    if (index <= 0 || index >= baseType->fields.length)
      USR_FATAL(call, "tuple index out-of-bounds error (%ld)", index);
  } else if (get_uint(call->get(3), &uindex)) {
    sprintf(field, "x%" PRIu64, uindex);
    if (uindex <= 0 || uindex >= (unsigned long)baseType->fields.length)
      USR_FATAL(call, "tuple index out-of-bounds error (%lu)", uindex);
  } else {
    return NULL; // not a tuple indexing expression
  }

  Type* fieldType = baseType->getField(field)->type;

  // Decomposing into a loop index variable from a non-var iterator?
  // In some cases, extract the value and mark constant.
  // See e.g. test/statements/vass/index-variable-const-errors.chpl
  bool intoIndexVarByVal = false;

  // If decomposing this special variable
  // or another tuple that we just decomposed.
  if (baseVar->hasFlag(FLAG_INDEX_OF_INTEREST)) {
    // Find the destination.
    CallExpr* move = toCallExpr(call->parentExpr);
    INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
    SymExpr* destSE = toSymExpr(move->get(1));
    INT_ASSERT(destSE);

    if (!isReferenceType(baseVar->type) &&
        !isReferenceType(fieldType)) {
      if (destSE->symbol()->hasFlag(FLAG_INDEX_VAR)) {
        // The destination is constant only if both the tuple
        // and the current component are non-references.
        // And it's not an array (arrays are always yielded by reference)
        // - see boundaries() in release/examples/benchmarks/miniMD/miniMD.
        if (!fieldType->symbol->hasFlag(FLAG_ARRAY)) {
          destSE->symbol()->addFlag(FLAG_CONST);
        }
      } else {
        INT_ASSERT(destSE->symbol()->hasFlag(FLAG_TEMP));
        // We are detupling into another tuple,
        // which will be detupled later.
        destSE->symbol()->addFlag(FLAG_INDEX_OF_INTEREST);
      }
    }

    if (!isReferenceType(baseVar->type))
      // If either a non-var iterator or zippered,
      // extract with PRIM_GET_MEMBER_VALUE.
      intoIndexVarByVal = true;
  }

  Expr* result;
  if (isReferenceType(fieldType) || intoIndexVarByVal)
    result = new CallExpr(PRIM_GET_MEMBER_VALUE, baseVar, new_CStringSymbol(field));
  else
    result = new CallExpr(PRIM_GET_MEMBER, baseVar, new_CStringSymbol(field));

  call->replace(result);
  return result;
}


//
// Make sure that 'type' is sufficiently non-generic to declare a
// variable over it in the context of 'call'.
//
static void ensureGenericSafeForDeclarations(CallExpr* call, Type* type) {
  TypeSymbol* typeSym = type->symbol;

  //
  // This function only cares about generic types
  //
  if (typeSym->hasFlag(FLAG_GENERIC)) {
    bool unsafeGeneric = true;  // Assume the worst until proven otherwise

    //
    // Grab the generic type's constructor if it has one.  Things like
    // classes and records should.  Things like 'integral' will not.
    //
    FnSymbol* typeCons = type->defaultTypeConstructor;

    if (typeCons) {
      //
      // If it had one, create a zero-argument call to the type
      // constructor, insert it into the code point in question,
      // and try to resolve it (saving the answer).
      //
      CallExpr* typeConsCall = new CallExpr(typeCons->name);
      call->replace(typeConsCall);
      unsafeGeneric = (tryResolveCall(typeConsCall) == NULL);

      //
      // Put things back the way they were.
      //
      typeConsCall->replace(call);
    }

    //
    // If the generic was unresolved (either because its type
    // constructor couldn't be called with zero arguments or because
    // it didn't have a type constructor), print a message.
    // Specialize it based on whether or not it had a type
    // constructor.
    //
    if (unsafeGeneric) {
      USR_FATAL(call,
                "Variables can't be declared using %s generic types like '%s'",
                (typeCons ? "not-fully-instantiated" : "abstract"),
                typeSym->name);
    }
  }
}


// Returns NULL if no substitution was made.  Otherwise, returns the expression
// that replaced the PRIM_INIT (or PRIM_NO_INIT) expression.
// Here, "replaced" means that the PRIM_INIT (or PRIM_NO_INIT) primitive is no
// longer in the tree.
static Expr* resolvePrimInit(CallExpr* call)
{
  Expr* result = NULL;

  // ('init' foo) --> A default value or the result of an initializer call.
  // ('no_init' foo) --> Ditto, only in some cases a simpler default value.

  // The argument is expected to be a type variable.
  SymExpr* se = toSymExpr(call->get(1));
  INT_ASSERT(se);
  if (!se->symbol()->hasFlag(FLAG_TYPE_VARIABLE))
    USR_FATAL(call, "invalid type specification");

  Type* type = resolveTypeAlias(se);

  // Do not resolve PRIM_INIT on extern types.
  // These are removed later.
  // It is useful to leave them in the tree, because PRIM_INIT behaves like an
  // expression and has a type.
  if (type->symbol->hasFlag(FLAG_EXTERN))
  {
    CallExpr* stmt = toCallExpr(call->parentExpr);
    INT_ASSERT(stmt->isPrimitive(PRIM_MOVE));
//    makeNoop(stmt);
//    result = stmt;
    return result;
  }

  // Do not resolve runtime type values yet.
  // Let these flow through to replaceInitPrims().
  if (type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE))
    return result;

  SET_LINENO(call);

  if (type->defaultValue ||
      type->symbol->hasFlag(FLAG_TUPLE)) {
    // Very special case for the method token.
    // Unfortunately, dtAny cannot (currently) bind to dtMethodToken, so
    // inline proc _defaultOf(type t) where t==_MT cannot be written in module
    // code.
    // Maybe it would be better to indicate which calls are method calls
    // through the use of a flag.  Until then, we just fake in the needed
    // result and short-circuit the resolution of _defaultOf(type _MT).
    if (type == dtMethodToken)
    {
      result = new SymExpr(gMethodToken);
      call->replace(result);
      return result;
    }
  }

  if (type->defaultInitializer)
  {
    if (type->symbol->hasFlag(FLAG_ITERATOR_RECORD))
      // defaultInitializers for iterator record types cannot be called as
      // default constructors.  So give up now!
      return result;
  }

  //
  // Catch the case of declaring a variable over an uninstantiated
  // generic type in order to print out a useful error message before
  // we get too deep into resolution.
  //
  ensureGenericSafeForDeclarations(call, type);

  CallExpr* defOfCall = new CallExpr("_defaultOf", type->symbol);
  call->replace(defOfCall);
  resolveCallAndCallee(defOfCall);
  result = postFold(defOfCall);
  return result;
}

static Expr* preFold(Expr* expr) {
  Expr* result = expr;

  if (CallExpr* call = toCallExpr(expr)) {
    // Match calls that look like:  (<type-symbol> <immediate-integer>)
    // and replace them with:       <new-type-symbol>
    // <type-symbol> is in {dtBools, dtInt, dtUint, dtReal, dtImag, dtComplex}.
    // This replaces, e.g.
    //   dtInt[INT_SIZE_DEFAULT] 32) with dtInt[INT_SIZE_32].
    if (Type* type = typeForTypeSpecifier(call)) {
      result = new SymExpr(type->symbol);

      call->replace(result);
    }

    if (SymExpr* sym = toSymExpr(call->baseExpr)) {
      if (isLcnSymbol(sym->symbol())) {
        Expr* base = call->baseExpr;
        base->replace(new UnresolvedSymExpr("this"));
        call->insertAtHead(base);
        call->insertAtHead(gMethodToken);
      }
    }

    if (CallExpr* base = toCallExpr(call->baseExpr)) {
      if (base->partialTag) {
        for_actuals_backward(actual, base) {
          actual->remove();
          call->insertAtHead(actual);
        }
        base->replace(base->baseExpr->remove());
      } else {
        VarSymbol* this_temp = newTemp("_this_tmp_");
        this_temp->addFlag(FLAG_EXPR_TEMP);
        base->replace(new UnresolvedSymExpr("this"));
        CallExpr* move = new CallExpr(PRIM_MOVE, this_temp, base);
        call->insertAtHead(new SymExpr(this_temp));
        call->insertAtHead(gMethodToken);
        call->getStmtExpr()->insertBefore(new DefExpr(this_temp));
        call->getStmtExpr()->insertBefore(move);
        result = move;
        return result;
      }
    }

    if (call->isNamed("this")) {
      SymExpr* base = toSymExpr(call->get(2));
      if (!base) {
        if (NamedExpr* nb = toNamedExpr(call->get(2)))
          base = toSymExpr(nb->actual);
      }
      INT_ASSERT(base);
      if (isVarSymbol(base->symbol()) && base->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
        if (call->numActuals() == 2)
          USR_FATAL(call, "illegal call of type");
        int64_t index;
        if (!get_int(call->get(3), &index))
          USR_FATAL(call, "illegal type index expression");
        char field[8];
        sprintf(field, "x%" PRId64, index);
        result = new SymExpr(base->symbol()->type->getField(field)->type->symbol);
        call->replace(result);
      } else if (base && isLcnSymbol(base->symbol())) {
        //
        // resolve tuple indexing by an integral parameter
        //
        Type* t = base->symbol()->getValType();
        if (t->symbol->hasFlag(FLAG_TUPLE))
          if (Expr* expr = resolveTupleIndexing(call, base->symbol()))
            result = expr;  // call was replaced by expr
      }
    }
    else if (call->isPrimitive(PRIM_INIT))
    {
      if (Expr* expr = resolvePrimInit(call))
      {
        // call was replaced by expr.
        result = expr;
      }
      // No default value yet, so defer resolution of this init
      // primitive until record initializer resolution.
    } else if (call->isPrimitive(PRIM_NO_INIT)) {
      // Lydia note: fUseNoinit does not control this section.  This was
      // necessary because with the definition of type defaults in the module
      // code, return temporary variables would cause an infinite loop by
      // trying to default initialize within the default initialization
      // definition.  (It is safe for these temporaries to skip default
      // initialization, as they will always be assigned a value before they
      // are returned.)  Thus noinit must remain attached to these temporaries,
      // even if --no-use-noinit is thrown.  This is an implementation detail
      // that the user does not need to care about.

      // fUseNoinit controls the insertion of PRIM_NO_INIT statements in the
      // normalize pass.
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      if (!se->symbol()->hasFlag(FLAG_TYPE_VARIABLE))
        USR_FATAL(call, "invalid type specification");
      Type* type = call->get(1)->getValType();
      if (isAggregateType(type) || type == dtStringC || type == dtStringCopy) {

        bool containsNotPOD = propagateNotPOD(type);

        if (containsNotPOD) {
          // These types deal with their uninitialized fields differently than
          // normal records/classes.  They may require special case
          // implementations, but were capable of being isolated from the new
          // cases that do work.

          bool nowarn = false;
          // In the case of temporary variables that use noinit (at this point
          // only return variables), it is not useful to warn the user we are
          // still default initializing the values as they weren't the ones to
          // tell us to use noinit in the first place.  So squash the warning
          // in this case.
          if (call->parentExpr) {
            CallExpr* parent = toCallExpr(call->parentExpr);
            if (parent && parent->isPrimitive(PRIM_MOVE)) {
              // Should always be true, but just in case...
              if (SymExpr* holdsDest = toSymExpr(parent->get(1))) {
                Symbol* dest = holdsDest->symbol();
                // TODO: Surely there's a better way to test for this pattern
                bool okName = strcmp(dest->name, "ret") == 0 ||
                              strcmp(dest->name, arrayUnrefName) == 0;
                if (dest->hasFlag(FLAG_TEMP) && okName) {
                  nowarn = true;
                }
              }
            }
          }
          if (!nowarn)
            USR_WARN(call, "type %s does not currently support noinit, using default initialization", type->symbol->name);
          result = new CallExpr(PRIM_INIT, call->get(1)->remove());
          call->replace(result);
          inits.add(toCallExpr(result));
        } else {
          result = call;
          inits.add(call);
        }
      }

    } else if (call->isPrimitive(PRIM_DELETE)) {
      result = new CallExpr("chpl__delete", call->get(1)->remove());
      call->replace(result);
    } else if (call->isPrimitive(PRIM_TYPEOF)) {
      Type* type = call->get(1)->getValType();
      if (type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
        result = new CallExpr("chpl__convertValueToRuntimeType", call->get(1)->remove());
        call->replace(result);

        // If this call is inside a BLOCK_TYPE_ONLY, it will be removed and the
        // runtime type will not be initialized. Unset this bit to fix.
        //
        // Assumption: The block we need to modify is either the parent or
        // grandparent expression of the call.
        BlockStmt* blk = NULL;
        if ((blk = toBlockStmt(result->parentExpr))) {
          // If the call's parent expression is a block, we assume it to
          // be a scopeless type_only block.
          INT_ASSERT(blk->blockTag & BLOCK_TYPE);
        } else {
          // The grandparent block doesn't necessarily have the BLOCK_TYPE_ONLY
          // flag.
          blk = toBlockStmt(result->parentExpr->parentExpr);
        }
        if (blk) {
          (unsigned&)(blk->blockTag) &= ~(unsigned)BLOCK_TYPE_ONLY;
        }
      }
    } else if (call->isPrimitive(PRIM_QUERY)) {
      Symbol* field = determineQueriedField(call);
      if (field && (field->hasFlag(FLAG_PARAM) || field->hasFlag(FLAG_TYPE_VARIABLE))) {
        result = new CallExpr(field->name, gMethodToken, call->get(1)->remove());
        call->replace(result);
      } else if (isInstantiatedField(field)) {
        VarSymbol* tmp = newTemp("_instantiated_field_tmp_");
        call->getStmtExpr()->insertBefore(new DefExpr(tmp));
        if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_TUPLE) && field->name[0] == 'x')
          result = new CallExpr(PRIM_GET_MEMBER_VALUE, call->get(1)->remove(), new_CStringSymbol(field->name));
        else
          result = new CallExpr(field->name, gMethodToken, call->get(1)->remove());
        call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, result));
        call->replace(new CallExpr(PRIM_TYPEOF, tmp));
      } else
        USR_FATAL(call, "invalid query -- queried field must be a type or parameter");
    } else if (call->isPrimitive(PRIM_CAPTURE_FN_FOR_CHPL) ||
               call->isPrimitive(PRIM_CAPTURE_FN_FOR_C)) {
      result = createFunctionAsValue(call);
      call->replace(result);

    } else if (call->isPrimitive(PRIM_GET_COMPILER_VAR)) {

      // Resolving Primitive PRIM_GET_COMPILER_VAR
      Immediate* chplEnv = toVarSymbol(toSymExpr(call->get(1))->symbol())->immediate;
      std::string envKey;

      // Check if this immediate is a string
      if (chplEnv->const_kind == CONST_KIND_STRING) {
        envKey = chplEnv->v_string;
      } else {
        USR_FATAL(call, "expected immediate of type string");
      }

      // Check if string is in envMap, and replace result with mapped value
      if (envMap.find(envKey) != envMap.end()) {
        result = new SymExpr(new_StringSymbol(envMap[envKey]));
        call->replace(result);
      } else {
        USR_FATAL(call, "primitive string does not match any environment variable");
      }

    } else if (call->isPrimitive(PRIM_CREATE_FN_TYPE)) {
      AggregateType *parent = createOrFindFunTypeFromAnnotation(call->argList, call);

      result = new SymExpr(parent->symbol);
      call->replace(result);
    } else if (call->isNamed("chpl__initCopy") ||
               call->isNamed("chpl__autoCopy")) {
      if (call->numActuals() == 1) {
        if (SymExpr* symExpr = toSymExpr(call->get(1))) {
          if (VarSymbol* var = toVarSymbol(symExpr->symbol())) {
            if (var->immediate) {
              result = new SymExpr(var);
              call->replace(result);
            }
          } else {
            if (EnumSymbol* var = toEnumSymbol(symExpr->symbol())) {
              // Treat enum values as immediates
              result = new SymExpr(var);
              call->replace(result);
            }
          }
        }
      }
    } else if (call->isCast()) {
      SymExpr* toSE = toSymExpr(call->castTo());
      // TODO: we could just bail out aka 'return (result=call)' if !toSE.
      if (toSE && !toSE->symbol()->hasFlag(FLAG_TYPE_VARIABLE))
        // TODO: here, also replace 'call' with toSE so it does not trigger
        // the same USR_FATAL_CONT in printResolutionErrorUnresolved().
        USR_FATAL_CONT(call, "illegal cast to non-type");
      result = dropUnnecessaryCast(call);
      if (result == call) {
        // The cast was not dropped.  Remove integer casts on immediate values.
        if (SymExpr* sym = toSymExpr(call->castFrom())) {
          if (VarSymbol* var = toVarSymbol(sym->symbol())) {
            if (var->immediate) {
              if (toSE) {
                Type* oldType = var->type;
                Type* newType = toSE->symbol()->type;
                if ((is_int_type(oldType) || is_uint_type(oldType) ||
                     is_bool_type(oldType)) &&
                    (is_int_type(newType) || is_uint_type(newType) ||
                     is_bool_type(newType) || is_enum_type(newType) ||
                     newType == dtString || newType == dtStringC)) {
                  VarSymbol* typevar = toVarSymbol(newType->defaultValue);
                  EnumType* typeenum = toEnumType(newType);
                  if (typevar) {
                    if (!typevar->immediate)
                      INT_FATAL("unexpected case in cast_fold");

                    Immediate coerce = *typevar->immediate;
                    coerce_immediate(var->immediate, &coerce);
                    result = new SymExpr(new_ImmediateSymbol(&coerce));
                    call->replace(result);
                  } else if (newType == dtString) {
                    // typevar will be null for dtString so we need a special
                    // case.
                    Immediate coerce = Immediate("", STRING_KIND_STRING);
                    coerce_immediate(var->immediate, &coerce);
                    result = new SymExpr(new_StringSymbol(coerce.v_string));
                    call->replace(result);
                  } else if (typeenum) {
                    int64_t value, count = 0;
                    bool replaced = false;
                    if (!get_int(call->castFrom(), &value)) {
                      INT_FATAL("unexpected case in cast_fold");
                    }
                    for_enums(constant, typeenum) {
                      if (!get_int(constant->init, &count)) {
                        count++;
                      }
                      if (count == value) {
                        result = new SymExpr(constant->sym);
                        call->replace(result);
                        replaced = true;
                        // could break here but might have issues with gcc 5.1
                      }
                    }
                    if (!replaced) {
                      USR_FATAL(call->castFrom(), "enum cast out of bounds");
                    }
                  } else {
                    INT_FATAL("unexpected case in cast_fold");
                  }
                } else if (oldType == dtString && newType == dtStringC) {
                  result = new SymExpr(new_CStringSymbol(var->immediate->v_string));
                  call->replace(result);
                } else if (oldType == dtStringC && newType == dtString) {
                  result = new SymExpr(new_StringSymbol(var->immediate->v_string));
                  call->replace(result);
                }

              }
            }
          } else if (EnumSymbol* enumSym = toEnumSymbol(sym->symbol())) {
            if (toSE) {
              Type* newType = toSE->symbol()->type;
              if (newType == dtString) {
                result = new SymExpr(new_StringSymbol(enumSym->name));
                call->replace(result);
              } else if (newType == dtStringC) {
                result = new SymExpr(new_CStringSymbol(enumSym->name));
                call->replace(result);
              }
            }
          }
        }
      }
    } else if (call->isNamed("==")) {
      if (isTypeExpr(call->get(1)) && isTypeExpr(call->get(2))) {
        Type* lt = call->get(1)->getValType();
        Type* rt = call->get(2)->getValType();
        if (lt != dtUnknown && rt != dtUnknown &&
            !lt->symbol->hasFlag(FLAG_GENERIC) &&
            !rt->symbol->hasFlag(FLAG_GENERIC)) {
          result = (lt == rt) ? new SymExpr(gTrue) : new SymExpr(gFalse);
          call->replace(result);
        }
      }
    } else if (call->isNamed("!=")) {
      if (isTypeExpr(call->get(1)) && isTypeExpr(call->get(2))) {
        Type* lt = call->get(1)->getValType();
        Type* rt = call->get(2)->getValType();
        if (lt != dtUnknown && rt != dtUnknown &&
            !lt->symbol->hasFlag(FLAG_GENERIC) &&
            !rt->symbol->hasFlag(FLAG_GENERIC)) {
          result = (lt != rt) ? new SymExpr(gTrue) : new SymExpr(gFalse);
          call->replace(result);
        }
      }
    } else if (call->isNamed("_type_construct__tuple") && !call->isResolved()) {
      if (SymExpr* sym = toSymExpr(call->get(1))) {
        if (VarSymbol* var = toVarSymbol(sym->symbol())) {
          if (var->immediate) {
            int rank = var->immediate->int_value();
            if (rank != call->numActuals() - 1) {
              if (call->numActuals() != 2)
                INT_FATAL(call, "bad homogeneous tuple");
              Expr* actual = call->get(2);
              for (int i = 1; i < rank; i++) {
                call->insertAtTail(actual->copy());
              }
            }
          }
        }
      }
    } else if (call->isPrimitive(PRIM_BLOCK_PARAM_LOOP)) {
      ParamForLoop* paramLoop = toParamForLoop(call->parentExpr);

      result = paramLoop->foldForResolve();
    } else if (call->isPrimitive(PRIM_LOGICAL_FOLDER)) {
      SymExpr* sym1 = toSymExpr(call->get(1));
      VarSymbol* lhs = NULL;
      if (VarSymbol* sym = toVarSymbol(sym1->symbol())) {
        if (sym->immediate || paramMap.get(sym)) {
          CallExpr* mvCall = toCallExpr(call->parentExpr);
          SymExpr* sym = toSymExpr(mvCall->get(1));
          lhs = toVarSymbol(sym->symbol());
          lhs->addFlag(FLAG_MAYBE_PARAM);
        }
      }

      result = call->get(2)->remove();
      if (SymExpr* se = toSymExpr(result)) {
        Symbol *sym2 = se->symbol();
        VarSymbol *v = toVarSymbol(se->symbol());
        ArgSymbol *a = toArgSymbol(se->symbol());
        bool isRef = isReferenceType(sym2->type);
        bool isImmediate = false;
        IntentTag intent = INTENT_BLANK;

        if (v) {
          if (v->immediate)
            isImmediate = true;
          intent = concreteIntent(INTENT_BLANK, v->type);
        }

        if (a) {
          intent = concreteIntent(a->intent, a->type);
        }

        if (v || a) {
          if (isRef) {
            // can't take address of something already a ref
          } else if (sym2->type == dtNil) {
            // can't take address of nil
          } else if (sym2->hasFlag(FLAG_EXPR_TEMP) ||
                     sym2->hasFlag(FLAG_TEMP) ||
                     isImmediate ||
                     paramMap.get(sym2)) {
            // can't take address of call temps, param values
            if (lhs)
              lhs->removeFlag(FLAG_MAYBE_REF);
          } else if (a && (intent & INTENT_FLAG_IN)) {
            // don't take the address of arguments passed with in intent
            // (it doesn't help and causes problems with inlining)
          } else if (v &&
                     (intent & INTENT_FLAG_IN) &&
                     v->isConstValWillNotChange()) {
            // don't take address of outer variables declared to be const
            // (otherwise, after flattenFunctions, we will take the
            //  address of a by-value argument).
          } else {
            Expr* stmt = call->getStmtExpr();
            Type* t = sym2->type;
            makeRefType(t);
            VarSymbol* tmp = newTemp("_fold_tmp", t->refType);
            stmt->insertBefore(new DefExpr(tmp));
            stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_ADDR_OF, result)));
            result = new SymExpr(tmp);
          }
        }
      }
      call->replace(result);
    } else if (call->isPrimitive(PRIM_ADDR_OF)) {
      // remove set ref if already a reference
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF) ||
          isTupleContainingOnlyReferences(call->get(1)->typeInfo())) {
        result = call->get(1)->remove();
        call->replace(result);
      } else {
        // Otherwise, we put off further checking until
        // checkResolved().
      }
    } else if (call->isPrimitive(PRIM_DEREF)) {
      // remove deref if arg is already a value
      if (!call->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
        result = call->get(1)->remove();
        call->replace(result);
      }
    } else if (call->isPrimitive(PRIM_TYPE_TO_STRING)) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se && se->symbol()->hasFlag(FLAG_TYPE_VARIABLE));
      result = new SymExpr(new_StringSymbol(se->symbol()->type->symbol->name));
      call->replace(result);
    } else if (call->isPrimitive(PRIM_WIDE_GET_LOCALE) ||
               call->isPrimitive(PRIM_WIDE_GET_NODE)) {
      Type* type = call->get(1)->getValType();

      //
      // ensure .locale (and on) are applied to lvalues or classes
      // (locale type is a class)
      //
      SymExpr* se = toSymExpr(call->get(1));
      if (se->symbol()->hasFlag(FLAG_EXPR_TEMP) && !isClass(type))
        USR_WARN(se, "accessing the locale of a local expression");

      //
      // if .locale is applied to an expression of array, domain, or distribution
      // wrapper type, apply .locale to the _value field of the
      // wrapper
      //
      if (isRecordWrappedType(type)) {
        VarSymbol* tmp = newTemp("_locale_tmp_");
        call->getStmtExpr()->insertBefore(new DefExpr(tmp));
        result = new CallExpr("_value", gMethodToken, call->get(1)->remove());
        call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, result));
        call->insertAtTail(tmp);
      }
    } else if (call->isPrimitive(PRIM_TO_STANDALONE)) {
      FnSymbol* iterator = getTheIteratorFn(call);
      CallExpr* standaloneCall = new CallExpr(iterator->name);
      for_formals(formal, iterator) {
        // Note: this can add a use formal outside of its function
        // This is cleaned up in cleanupLeaderFollowerIteratorCalls
        standaloneCall->insertAtTail(new NamedExpr(formal->name,
                                                   symOrParamExpr(formal)));
      }
      // "tag" should be placed at the end of the formals in the source code as
      // well, to avoid insertion of an order wrapper.
      standaloneCall->insertAtTail(new NamedExpr("tag", new SymExpr(gStandaloneTag)));
      call->replace(standaloneCall);
      result = standaloneCall;
    } else if (call->isPrimitive(PRIM_TO_LEADER)) {
      FnSymbol* iterator = getTheIteratorFn(call);
      CallExpr* leaderCall;
      if (FnSymbol* leader = iteratorLeaderMap.get(iterator))
        leaderCall = new CallExpr(leader);
      else
        leaderCall = new CallExpr(iterator->name);
      for_formals(formal, iterator) {
        // Note: this can add a use formal outside of its function
        // This is cleaned up in cleanupLeaderFollowerIteratorCalls
        leaderCall->insertAtTail(new NamedExpr(formal->name,
                                               symOrParamExpr(formal)));
      }
      // "tag" should be placed at the end of the formals in the source code as
      // well, to avoid insertion of an order wrapper.
      leaderCall->insertAtTail(new NamedExpr("tag", new SymExpr(gLeaderTag)));
      call->replace(leaderCall);
      result = leaderCall;
    } else if (call->isPrimitive(PRIM_TO_FOLLOWER)) {
      FnSymbol* iterator = getTheIteratorFn(call);
      CallExpr* followerCall;
      if (FnSymbol* follower = iteratorFollowerMap.get(iterator))
        followerCall = new CallExpr(follower);
      else
        followerCall = new CallExpr(iterator->name);
      for_formals(formal, iterator) {
        // Note: this can add a use formal outside of its function
        // This is cleaned up in cleanupLeaderFollowerIteratorCalls
        followerCall->insertAtTail(new NamedExpr(formal->name,
                                                 symOrParamExpr(formal)));
      }
      // "tag", "followThis" and optionally "fast" should be placed at the end
      // of the formals in the source code as well, to avoid insertion of an
      // order wrapper.
      followerCall->insertAtTail(new NamedExpr("tag", new SymExpr(gFollowerTag)));
      followerCall->insertAtTail(new NamedExpr(iterFollowthisArgname, call->get(2)->remove()));
      if (call->numActuals() > 1) {
        followerCall->insertAtTail(new NamedExpr("fast", call->get(2)->remove()));
      }
      call->replace(followerCall);
      result = followerCall;
    } else if (call->isPrimitive(PRIM_NUM_FIELDS)) {
      AggregateType* classtype =
        toAggregateType(toSymExpr(call->get(1))->symbol()->type);
      INT_ASSERT( classtype != NULL );
      classtype = toAggregateType(classtype->getValType());
      INT_ASSERT( classtype != NULL );

      int fieldcount = 0;

      for_fields(field, classtype) {
        if( ! isNormalField(field) ) continue;

        fieldcount++;
      }

      result = new SymExpr(new_IntSymbol(fieldcount));

      call->replace(result);
    } else if (call->isPrimitive(PRIM_FIELD_NUM_TO_NAME)) {
      AggregateType* classtype =
        toAggregateType(toSymExpr(call->get(1))->symbol()->type);
      INT_ASSERT( classtype != NULL );
      classtype = toAggregateType(classtype->getValType());
      INT_ASSERT( classtype != NULL );

      VarSymbol* var = toVarSymbol(toSymExpr(call->get(2))->symbol());

      INT_ASSERT( var != NULL );

      int fieldnum = var->immediate->int_value();
      int fieldcount = 0;
      const char* name = NULL;
      for_fields(field, classtype) {
        if( ! isNormalField(field) ) continue;

        fieldcount++;
        if (fieldcount == fieldnum) {
          name = field->name;
          // break could be here, but might have issues with GCC 5.10
        }
      }
      if (!name) {
        // In this case, we ran out of fields without finding the number
        // specified.  This is the user's error.
        USR_FATAL(call, "'%d' is not a valid field number for %s", fieldnum,
            toString(classtype));
      }
      result = new SymExpr(new_StringSymbol(name));
      call->replace(result);
    } else if (call->isPrimitive(PRIM_FIELD_BY_NUM)) {
      // if call->get(1) is a reference type, dereference it
      AggregateType* classtype = toAggregateType(call->get(1)->typeInfo());
      INT_ASSERT( classtype != NULL );
      classtype = toAggregateType(classtype->getValType());
      INT_ASSERT( classtype != NULL );

      VarSymbol* var = toVarSymbol(toSymExpr(call->get(2))->symbol());

      INT_ASSERT( var != NULL );

      int fieldnum = var->immediate->int_value();
      int fieldcount = 0;
      const char* name = NULL;

      for_fields(field, classtype) {
        if( ! isNormalField(field) ) continue;

        fieldcount++;
        if (fieldcount == fieldnum) {
          name = field->name;
          // break could be here, but seems to cause issues with GCC 5.10
        }
      }
      if (!name) {
        USR_FATAL(call, "'%d' is not a valid field number for %s", fieldnum,
                  toString(classtype));
      }
      result = new CallExpr(PRIM_GET_MEMBER, call->get(1)->copy(),
                            new_CStringSymbol(name));
      call->replace(result);
    } else if (call->isPrimitive(PRIM_FIELD_NAME_TO_NUM)) {
      AggregateType* classtype =
        toAggregateType(toSymExpr(call->get(1))->symbol()->type);
      INT_ASSERT( classtype != NULL );
      classtype = toAggregateType(classtype->getValType());
      INT_ASSERT( classtype != NULL );

      VarSymbol* var = toVarSymbol(toSymExpr(call->get(2))->symbol());
      INT_ASSERT( var != NULL );

      Immediate* imm = var->immediate;

      INT_ASSERT( classtype != NULL );
      // fail horribly if immediate is not a string .
      INT_ASSERT(imm->const_kind == CONST_KIND_STRING);

      const char* fieldname = imm->v_string;
      int fieldcount = 0;
      int num = 0;  // return 0 if the field is not found.
      for_fields(field, classtype) {
        if( ! isNormalField(field) ) continue;

        fieldcount++;
        if ( 0 == strcmp(field->name,  fieldname) ) {
          num = fieldcount;
          // break could be here, but might have issues with GCC 5.10
        }
      }
      result = new SymExpr(new_IntSymbol(num));

      call->replace(result);
    } else if (call->isPrimitive(PRIM_CALL_RESOLVES) ||
               call->isPrimitive(PRIM_METHOD_CALL_RESOLVES)) {
      Expr* fnName = NULL;
      Expr* callThis = NULL;

      // this would be easier if we had a non-normalized AST!
      // That is, if this call could contain a whole expression subtree.
      int first_arg;
      if( call->isPrimitive(PRIM_METHOD_CALL_RESOLVES) ) {
        // get(1) should be a receiver
        // get(2) should be a string function name.
        callThis = call->get(1);
        fnName = call->get(2);
        first_arg = 3;
      } else {
        // get(1) should be a string function name.
        fnName = call->get(1);
        first_arg = 2;
      }
      VarSymbol* var = toVarSymbol(toSymExpr(fnName)->symbol());
      INT_ASSERT( var != NULL );
      // the rest are arguments.
      Immediate* imm = var->immediate;
      // fail horribly if immediate is not a string .
      if (!imm)
        INT_FATAL(call, "proc name required");
      if (imm->const_kind != CONST_KIND_STRING)
        INT_FATAL(call, "proc name must be a string");

      const char* name = imm->v_string;

      // temporarily add a call to try resolving.
      CallExpr* tryCall = NULL;
      if( call->isPrimitive(PRIM_METHOD_CALL_RESOLVES) ) {
        tryCall = new CallExpr(new UnresolvedSymExpr(name),
                               gMethodToken,
                               callThis->copy());
      } else {
        tryCall = new CallExpr(name);
      }

      // Add our new call to the AST temporarily.
      call->getStmtExpr()->insertAfter(tryCall);

      // copy actual args into tryCall.
      int i = 1;
      for_actuals(actual, call) {
        if( i >= first_arg ) { // skip fn name, maybe method receiver
          tryCall->insertAtTail(actual->copy());
        }
        i++;
      }

      // Try to resolve it.
      if( tryResolveCall(tryCall) ) {
        result = new SymExpr(gTrue);
      } else {
        result = new SymExpr(gFalse);
      }

      // remove the call from the AST
      tryCall->remove();

      call->replace(result);
    } else if (call->isPrimitive(PRIM_ENUM_MIN_BITS) || call->isPrimitive(PRIM_ENUM_IS_SIGNED)) {
      EnumType* et = toEnumType(toSymExpr(call->get(1))->symbol()->type);


      ensureEnumTypeResolved(et);

      result = NULL;
      if( call->isPrimitive(PRIM_ENUM_MIN_BITS) ) {
        result = new SymExpr(new_IntSymbol(get_width(et->integerType)));
      } else if( call->isPrimitive(PRIM_ENUM_IS_SIGNED) ) {
        if( is_int_type(et->integerType) )
          result = new SymExpr(gTrue);
        else
          result = new SymExpr(gFalse);
      }
      call->replace(result);
    } else if (call->isPrimitive(PRIM_IS_UNION_TYPE)) {
      AggregateType* classtype = toAggregateType(call->get(1)->typeInfo());

      if( isUnion(classtype) )
        result = new SymExpr(gTrue);
      else
        result = new SymExpr(gFalse);
      call->replace(result);

    } else if (call->isPrimitive(PRIM_IS_ATOMIC_TYPE)) {
      if (isAtomicType(call->get(1)->typeInfo()))
        result = new SymExpr(gTrue);
      else
        result = new SymExpr(gFalse);
      call->replace(result);

    } else if (call->isPrimitive(PRIM_IS_REF_ITER_TYPE)) {
      if (isRefIterType(call->get(1)->typeInfo()))
        result = new SymExpr(gTrue);
      else
        result = new SymExpr(gFalse);
      call->replace(result);
    } else if (call->isPrimitive(PRIM_IS_POD)) {
      Type* t = call->get(1)->typeInfo();
      // call propagateNotPOD to set FLAG_POD/FLAG_NOT_POD
      propagateNotPOD(t);
      if (isPOD(t))
        result = new SymExpr(gTrue);
      else
        result = new SymExpr(gFalse);
      call->replace(result);
    } else if (call->isPrimitive(PRIM_IS_TUPLE_TYPE)) {
      Type* tupleType = call->get(1)->typeInfo();
      if (tupleType->symbol->hasFlag(FLAG_TUPLE))
        result = new SymExpr(gTrue);
      else
        result = new SymExpr(gFalse);
      call->replace(result);
    } else if (call->isPrimitive(PRIM_IS_STAR_TUPLE_TYPE)) {
      Type* tupleType = call->get(1)->typeInfo();
      // If the type isn't a tuple, it definitely isn't a homogeneous tuple!
      if (tupleType->symbol->hasFlag(FLAG_TUPLE) &&
          tupleType->symbol->hasFlag(FLAG_STAR_TUPLE))
        result = new SymExpr(gTrue);
      else
        result = new SymExpr(gFalse);
      call->replace(result);
    } else if (call->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
      // Convert these to PRIM_GET_SVEC_MEMBER_VALUE if the
      // field in question is a reference.
      // An alternative to this transformation here would be
      // to build homog. tuple accessor entirely in tuples.cpp.
      AggregateType *ct = toAggregateType(call->get(1)->getValType());
      if (ct && ct->symbol->hasFlag(FLAG_STAR_TUPLE)) {
        FnSymbol* inFn = toFnSymbol(call->parentSymbol);
        if (inFn && inFn->hasFlag(FLAG_STAR_TUPLE_ACCESSOR)) {
          QualifiedType fieldType = ct->getFieldType(call->get(2));
          if (fieldType.type() && fieldType.isRef()) {
            if (call->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
              Expr* base = call->get(1);
              Expr* field = call->get(2);
              result = new CallExpr(PRIM_GET_SVEC_MEMBER_VALUE,
                                   base->remove(), field->remove());
              call->replace(result);
            }
          }
        }
      }
    }
  }
  //
  // ensure result of pre-folding is in the AST
  //
  INT_ASSERT(result->parentSymbol);
  return result;
}

static void foldEnumOp(int op, EnumSymbol *e1, EnumSymbol *e2, Immediate *imm) {
  int64_t val1 = -1, val2 = -1, count = 0;
  // ^^^ This is an assumption that "long" on the compiler host is at
  // least as big as "int" on the target.  This is not guaranteed to be true.
  EnumType *type1, *type2;

  type1 = toEnumType(e1->type);
  type2 = toEnumType(e2->type);
  INT_ASSERT(type1 && type2);

  // Loop over the enum values to find the int value of e1
  for_enums(constant, type1) {
    if (!get_int(constant->init, &count)) {
      count++;
    }
    if (constant->sym == e1) {
      val1 = count;
      break;
    }
  }
  // Loop over the enum values to find the int value of e2
  count = 0;
  for_enums(constant, type2) {
    if (!get_int(constant->init, &count)) {
      count++;
    }
    if (constant->sym == e2) {
      val2 = count;
      break;
    }
  }

  // All operators on enum types result in a bool
  imm->const_kind = NUM_KIND_BOOL;
  imm->num_index = BOOL_SIZE_SYS;
  switch (op) {
    default: INT_FATAL("fold constant op not supported"); break;
    case P_prim_equal:
      imm->v_bool = val1 == val2;
      break;
    case P_prim_notequal:
      imm->v_bool = val1 != val2;
      break;
    case P_prim_less:
      imm->v_bool = val1 < val2;
      break;
    case P_prim_lessorequal:
      imm->v_bool = val1 <= val2;
      break;
    case P_prim_greater:
      imm->v_bool = val1 > val2;
      break;
    case P_prim_greaterorequal:
      imm->v_bool = val1 >= val2;
      break;
  }
}

#define FOLD_CALL1(prim)                                                \
  if (SymExpr* sym = toSymExpr(call->get(1))) {            \
    if (VarSymbol* lhs = toVarSymbol(sym->symbol())) {          \
      if (lhs->immediate) {                                             \
        Immediate i3;                                                   \
        fold_constant(prim, lhs->immediate, NULL, &i3);                 \
        result = new SymExpr(new_ImmediateSymbol(&i3));                 \
        call->replace(result);                                          \
      }                                                                 \
    }                                                                   \
  }

#define FOLD_CALL2(prim)                                                \
  if (SymExpr* sym = toSymExpr(call->get(1))) {            \
    if (VarSymbol* lhs = toVarSymbol(sym->symbol())) {          \
      if (lhs->immediate) {                                             \
        if (SymExpr* sym = toSymExpr(call->get(2))) {      \
          if (VarSymbol* rhs = toVarSymbol(sym->symbol())) {    \
            if (rhs->immediate) {                                       \
              Immediate i3;                                             \
              fold_constant(prim, lhs->immediate, rhs->immediate, &i3); \
              result = new SymExpr(new_ImmediateSymbol(&i3));           \
              call->replace(result);                                    \
            }                                                           \
          }                                                             \
        }                                                               \
      }                                                                 \
    } else if (EnumSymbol* lhs = toEnumSymbol(sym->symbol())) { \
      if (SymExpr* sym = toSymExpr(call->get(2))) {        \
        if (EnumSymbol* rhs = toEnumSymbol(sym->symbol())) {    \
          Immediate imm;                                                \
          foldEnumOp(prim, lhs, rhs, &imm);                             \
          result = new SymExpr(new_ImmediateSymbol(&imm));              \
          call->replace(result);                                        \
        }                                                               \
      }                                                                 \
    }                                                                   \
  }

static bool
isSubType(Type* sub, Type* super) {
  if (sub == super)
    return true;
  forv_Vec(Type, parent, sub->dispatchParents) {
    if (isSubType(parent, super))
      return true;
  }
  return false;
}

static bool
isInstantiation(Type* sub, Type* super) {
  Type * cur;
  cur = sub->instantiatedFrom;
  while (cur && cur != super) {
    cur = cur->instantiatedFrom;
  }

  return cur == super;
}


static bool
isSubTypeOrInstantiation(Type* sub, Type* super) {
  if (sub == super)
    return true;
  forv_Vec(Type, parent, sub->dispatchParents) {
    if (isSubTypeOrInstantiation(parent, super))
      return true;
  }
  if (sub->instantiatedFrom &&
      isSubTypeOrInstantiation(sub->instantiatedFrom, super))
    return true;
  return false;
}


static void
insertValueTemp(Expr* insertPoint, Expr* actual) {
  if (SymExpr* se = toSymExpr(actual)) {
    if (!se->symbol()->type->refType) {
      VarSymbol* tmp = newTemp("_value_tmp_", se->symbol()->getValType());
      insertPoint->insertBefore(new DefExpr(tmp));
      insertPoint->insertBefore(new CallExpr(PRIM_MOVE, tmp, new
            CallExpr(PRIM_DEREF, se->symbol())));
      se->setSymbol(tmp);
    }
  }
}


//
// returns resolved function if the function requires an implicit
// destroy of its returned value (i.e. reference count)
//
// Currently, FLAG_DONOR_FN is only relevant when placed on
// chpl__autoCopy().
//
FnSymbol*
requiresImplicitDestroy(CallExpr* call) {
  if (FnSymbol* fn = call->isResolved()) {
    FnSymbol* parent = call->getFunction();
    INT_ASSERT(parent);

    if (!parent->hasFlag(FLAG_DONOR_FN) &&
        // No autocopy/destroy calls in a donor function (this might
        // need to change when this flag is used more generally)).
        // Currently, this assumes we have thoughtfully written
        // chpl__autoCopy functions.

        // Return type is a record (which includes array, domain, dist,
        // user record)
        isRecord(fn->retType) &&

        // These are special functions where we don't want to destroy
        // the result
        !fn->hasFlag(FLAG_NO_IMPLICIT_COPY) &&
        !fn->isIterator() &&
        !fn->retType->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE) &&
        !fn->hasFlag(FLAG_DONOR_FN) &&
        !fn->hasFlag(FLAG_INIT_COPY_FN) &&
        strcmp(fn->name, "=") &&
        strcmp(fn->name, "_defaultOf") &&
        !fn->hasFlag(FLAG_AUTO_II) &&
        !fn->hasFlag(FLAG_CONSTRUCTOR) &&
        !fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)) {
      return fn;
    }
  }
  return NULL;
}


static Expr*
postFold(Expr* expr) {
  Expr* result = expr;
  if (!expr->parentSymbol)
    return result;

  SET_LINENO(expr);
  if (CallExpr* call = toCallExpr(expr)) {
    if (FnSymbol* fn = call->isResolved()) {
      if (fn->retTag == RET_PARAM || fn->hasFlag(FLAG_MAYBE_PARAM)) {
        VarSymbol* ret = toVarSymbol(fn->getReturnSymbol());
        if (ret && ret->immediate) {
          result = new SymExpr(ret);
          expr->replace(result);
        } else if (EnumSymbol* es = toEnumSymbol(fn->getReturnSymbol())) {
          result = new SymExpr(es);
          expr->replace(result);
        } else if (ret == gVoid) {
          result = new SymExpr(gVoid);
          expr->replace(result);
        }
      }
      if (fn->hasFlag(FLAG_MAYBE_TYPE) && fn->getReturnSymbol()->hasFlag(FLAG_TYPE_VARIABLE))
        fn->retTag = RET_TYPE;
      if (fn->retTag == RET_TYPE) {
        Symbol* ret = fn->getReturnSymbol();
        if (!ret->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
          result = new SymExpr(ret->type->symbol);
          expr->replace(result);
        }
      }
      if (call->isNamed("=")) {
        if (SymExpr* lhs = toSymExpr(call->get(1))) {
          if (lhs->symbol()->hasFlag(FLAG_MAYBE_PARAM) || lhs->symbol()->isParameter()) {
            if (paramMap.get(lhs->symbol())) {
              USR_FATAL(call, "parameter set multiple times");
            }
          }
        }
      }
    } else if (call->isPrimitive(PRIM_QUERY_TYPE_FIELD) ||
               call->isPrimitive(PRIM_QUERY_PARAM_FIELD)) {
      SymExpr* classWrap = toSymExpr(call->get(1));
      // Really should be a symExpr
      INT_ASSERT(classWrap);
      AggregateType* ct = toAggregateType(classWrap->symbol()->type);
      if (!ct) {
        USR_FATAL(call, "Attempted to obtain field of a type that was not a record or class");
      }
      const char* memberName = get_string(call->get(2));

      // Finds the field matching the specified name.
      Vec<Symbol *> keys;
      ct->substitutions.get_keys(keys);
      forv_Vec(Symbol, key, keys) {
        if (!strcmp(memberName, key->name)) {
          // If there is a substitution for it, replace this call with that
          // substitution
          if (Symbol* value = ct->substitutions.get(key)) {
              result = new SymExpr(value);
              expr->replace(result);
          }
        }
      }
    }
    // param initialization should not involve PRIM_ASSIGN or "=".
    else if (call->isPrimitive(PRIM_MOVE)) {
      bool set = false;
      if (SymExpr* lhs = toSymExpr(call->get(1))) {
        if (lhs->symbol()->hasFlag(FLAG_MAYBE_PARAM) || lhs->symbol()->isParameter()) {
          if (paramMap.get(lhs->symbol()))
            INT_FATAL(call, "parameter set multiple times");
          VarSymbol* lhsVar = toVarSymbol(lhs->symbol());
          // We are expecting the LHS to be a var (what else could it be? )
          if (lhsVar->immediate) {
            // The value of the LHS of this move has already been
            // established, most likely through a construct like
            // if (cond) return x;
            // return y;
            // In this case, the first 'true' conditional that hits a return
            // can fast-forward to the end of the routine, and some
            // resolution time can be saved.
            // Re-enable the fatal error to catch this case; the correct
            // solution is to ensure that the containing expression is never
            // resolved, using the abbreviated resolution suggested above.
            // INT_ASSERT(!lhsVar->immediate);
            set = true; // That is, set previously.
          } else {
            if (SymExpr* rhs = toSymExpr(call->get(2))) {
              if (VarSymbol* rhsVar = toVarSymbol(rhs->symbol())) {
                if (rhsVar->immediate) {
                  paramMap.put(lhs->symbol(), rhsVar);
                  lhs->symbol()->defPoint->remove();
                  makeNoop(call);
                  set = true;
                }
              }
              if (EnumSymbol* rhsv = toEnumSymbol(rhs->symbol())) {
                paramMap.put(lhs->symbol(), rhsv);
                lhs->symbol()->defPoint->remove();
                makeNoop(call);
                set = true;
              }
            }
          }
          if (Symbol* lhsSym = lhs->symbol()) {
            if (lhsSym->isParameter()) {
              if (!lhsSym->hasFlag(FLAG_TEMP)) {
                if (!isLegalParamType(lhsSym->type)) {
                  USR_FATAL_CONT(call, "'%s' is not of a supported param type", lhsSym->name);
                } else if (!set) {
                  USR_FATAL_CONT(call, "Initializing parameter '%s' to value not known at compile time", lhsSym->name);
                  lhs->symbol()->removeFlag(FLAG_PARAM);
                }
              } else /* this is a compiler temp */ {
                if (lhsSym->hasFlag(FLAG_RVV) && !set) {
                  USR_FATAL_CONT(call, "'param' functions cannot return non-'param' values");
                }
              }
            }
          }
        }
        if (!set) {
          if (lhs->symbol()->hasFlag(FLAG_MAYBE_TYPE)) {
            // Add FLAG_TYPE_VARIABLE when relevant
            if (SymExpr* rhs = toSymExpr(call->get(2))) {
              if (rhs->symbol()->hasFlag(FLAG_TYPE_VARIABLE))
                lhs->symbol()->addFlag(FLAG_TYPE_VARIABLE);
            } else if (CallExpr* rhs = toCallExpr(call->get(2))) {
              if (FnSymbol* fn = rhs->isResolved()) {
                if (fn->retTag == RET_TYPE)
                  lhs->symbol()->addFlag(FLAG_TYPE_VARIABLE);
              } else if (rhs->isPrimitive(PRIM_DEREF)) {
                if (isTypeExpr(rhs->get(1)))
                  lhs->symbol()->addFlag(FLAG_TYPE_VARIABLE);
              }
            }
          }
          if (CallExpr* rhs = toCallExpr(call->get(2))) {
            if (rhs->isPrimitive(PRIM_TYPEOF)) {
              lhs->symbol()->addFlag(FLAG_TYPE_VARIABLE);
            }
            if (FnSymbol* fn = rhs->isResolved()) {
              if (!strcmp(fn->name, "=") && fn->retType == dtVoid) {
                call->replace(rhs->remove());
                result = rhs;
                set = true;
              }
            }
          }
        }

        if (!set) {
          if (lhs->symbol()->hasFlag(FLAG_EXPR_TEMP) &&
              !lhs->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
            if (CallExpr* rhsCall = toCallExpr(call->get(2))) {
              if (requiresImplicitDestroy(rhsCall)) {
                // this still semes to be necessary even if
                // isUserDefinedRecord(lhs->symbol()->type) == true
                // see call-expr-tmp.chpl for example
                lhs->symbol()->addFlag(FLAG_INSERT_AUTO_COPY);
                lhs->symbol()->addFlag(FLAG_INSERT_AUTO_DESTROY);
              }
            }
          }

          if (isReferenceType(lhs->symbol()->type) ||
              lhs->symbol()->type->symbol->hasFlag(FLAG_REF_ITERATOR_CLASS) ||
              lhs->symbol()->type->symbol->hasFlag(FLAG_ARRAY))
            // Should this conditional include domains, distributions, sync and/or single?
            // TODO -- remove this? Or explain its purpose?
            lhs->symbol()->removeFlag(FLAG_EXPR_TEMP);
        }
        if (!set) {
          if (CallExpr* rhs = toCallExpr(call->get(2))) {
            if (rhs->isPrimitive(PRIM_NO_INIT)) {
              // If the lhs is a primitive, then we can safely just remove this
              // value.  Otherwise the type needs to be resolved a little
              // further and so this statement can't be removed until
              // resolveRecordInitializers
              if (!isAggregateType(rhs->get(1)->getValType())) {
                makeNoop(call);
              }
            }
          }
        }
      }
    } else if (call->isPrimitive(PRIM_GET_MEMBER)) {
      Type* baseType = call->get(1)->getValType();
      const char* memberName = get_string(call->get(2));
      Symbol* sym = baseType->getField(memberName);
      SymExpr* left = toSymExpr(call->get(1));
      VarSymbol* varSym = toVarSymbol(sym);
      if (left && left->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
        result = new SymExpr(sym->type->symbol);
        call->replace(result);
      } else if (sym->isParameter() || (varSym && varSym->isType())) {
        Vec<Symbol*> keys;
        baseType->substitutions.get_keys(keys);
        forv_Vec(Symbol, key, keys) {
          if (!strcmp(sym->name, key->name)) {
            if (Symbol* value = baseType->substitutions.get(key)) {
              result = new SymExpr(value);
              call->replace(result);
            }
          }
        }
      }
    } else if (call->isPrimitive(PRIM_IS_SUBTYPE)) {
      if (isTypeExpr(call->get(1)) || isTypeExpr(call->get(2))) {
        Type* lt = call->get(2)->getValType(); // a:t cast is cast(t,a)
        Type* rt = call->get(1)->getValType();
        if (lt != dtUnknown && rt != dtUnknown && lt != dtAny &&
            rt != dtAny && !lt->symbol->hasFlag(FLAG_GENERIC)) {
          bool is_true = isSubTypeOrInstantiation(lt, rt);
          result = (is_true) ? new SymExpr(gTrue) : new SymExpr(gFalse);
          call->replace(result);
        }
      }
    } else if (call->isPrimitive(PRIM_CAST)) {
      Type* t= call->get(1)->typeInfo();
      if (t == dtUnknown)
        INT_FATAL(call, "Unable to resolve type");
      call->get(1)->replace(new SymExpr(t->symbol));
    } else if (call->isPrimitive("string_compare")) {
      SymExpr* lhs = toSymExpr(call->get(1));
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(lhs && rhs);
      if (lhs->symbol()->isParameter() && rhs->symbol()->isParameter()) {
        const char* lstr = get_string(lhs);
        const char* rstr = get_string(rhs);
        int comparison = strcmp(lstr, rstr);
        result = new SymExpr(new_IntSymbol(comparison));
        call->replace(result);
      }
    } else if (call->isPrimitive("string_concat")) {
      SymExpr* lhs = toSymExpr(call->get(1));
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(lhs && rhs);
      if (lhs->symbol()->isParameter() && rhs->symbol()->isParameter()) {
        const char* lstr = get_string(lhs);
        const char* rstr = get_string(rhs);
        if (lhs->symbol()->type == dtString)
          result = new SymExpr(new_StringSymbol(astr(lstr, rstr)));
        else
          result = new SymExpr(new_CStringSymbol(astr(lstr, rstr)));
        call->replace(result);
      }
    } else if (call->isPrimitive("string_length")) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      if (se->symbol()->isParameter()) {
        const char* str = get_string(se);
        int length = unescapeString(str, se).length();
        result = new SymExpr(new_IntSymbol(length, INT_SIZE_DEFAULT));
        call->replace(result);
      }
    } else if (call->isPrimitive("ascii")) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      if (se->symbol()->isParameter()) {
        const char* str = get_string(se);
        const std::string unescaped = unescapeString(str, se);
        result = new SymExpr(new_UIntSymbol((int)unescaped[0], INT_SIZE_8));
        call->replace(result);
      }
    } else if (call->isPrimitive("string_contains")) {
      SymExpr* lhs = toSymExpr(call->get(1));
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(lhs && rhs);
      if (lhs->symbol()->isParameter() && rhs->symbol()->isParameter()) {
        const char* lstr = get_string(lhs);
        const char* rstr = get_string(rhs);
        result = new SymExpr(strstr(lstr, rstr) ? gTrue : gFalse);
        call->replace(result);
      }
    } else if (call->isPrimitive(PRIM_UNARY_MINUS)) {
      FOLD_CALL1(P_prim_minus);
    } else if (call->isPrimitive(PRIM_UNARY_PLUS)) {
      FOLD_CALL1(P_prim_plus);
    } else if (call->isPrimitive(PRIM_UNARY_NOT)) {
      FOLD_CALL1(P_prim_not);
    } else if (call->isPrimitive(PRIM_UNARY_LNOT)) {
      FOLD_CALL1(P_prim_lnot);
    } else if (call->isPrimitive(PRIM_ADD)) {
      FOLD_CALL2(P_prim_add);
    } else if (call->isPrimitive(PRIM_SUBTRACT)) {
      FOLD_CALL2(P_prim_subtract);
    } else if (call->isPrimitive(PRIM_MULT)) {
      FOLD_CALL2(P_prim_mult);
    } else if (call->isPrimitive(PRIM_DIV)) {
      FOLD_CALL2(P_prim_div);
    } else if (call->isPrimitive(PRIM_MOD)) {
      FOLD_CALL2(P_prim_mod);
    } else if (call->isPrimitive(PRIM_EQUAL)) {
      FOLD_CALL2(P_prim_equal);
    } else if (call->isPrimitive(PRIM_NOTEQUAL)) {
      FOLD_CALL2(P_prim_notequal);
    } else if (call->isPrimitive(PRIM_LESSOREQUAL)) {
      FOLD_CALL2(P_prim_lessorequal);
    } else if (call->isPrimitive(PRIM_GREATEROREQUAL)) {
      FOLD_CALL2(P_prim_greaterorequal);
    } else if (call->isPrimitive(PRIM_LESS)) {
      FOLD_CALL2(P_prim_less);
    } else if (call->isPrimitive(PRIM_GREATER)) {
      FOLD_CALL2(P_prim_greater);
    } else if (call->isPrimitive(PRIM_AND)) {
      FOLD_CALL2(P_prim_and);
    } else if (call->isPrimitive(PRIM_OR)) {
      FOLD_CALL2(P_prim_or);
    } else if (call->isPrimitive(PRIM_XOR)) {
      FOLD_CALL2(P_prim_xor);
    } else if (call->isPrimitive(PRIM_POW)) {
      FOLD_CALL2(P_prim_pow);
    } else if (call->isPrimitive(PRIM_LSH)) {
      FOLD_CALL2(P_prim_lsh);
    } else if (call->isPrimitive(PRIM_RSH)) {
      FOLD_CALL2(P_prim_rsh);
    } else if (call->isPrimitive(PRIM_REQUIRE)) {
      Expr* arg = call->argList.only();
      const char* str;
      if (get_string(arg, &str)) {
        processStringInRequireStmt(str, false);
      } else {
        USR_FATAL(call, "'require' statements require string arguments");
      }
      result = new CallExpr(PRIM_NOOP);
      call->replace(result);
    } else if (call->isPrimitive(PRIM_ARRAY_ALLOC) ||
               (call->primitive &&
                (!strncmp("_fscan", call->primitive->name, 6) ||
                 !strcmp("_readToEndOfLine", call->primitive->name) ||
                 !strcmp("_now_timer", call->primitive->name)))) {
      //
      // these primitives require temps to dereference actuals
      //   why not do this to all primitives?
      //
      for_actuals(actual, call) {
        insertValueTemp(call->getStmtExpr(), actual);
      }
    }
  } else if (SymExpr* sym = toSymExpr(expr)) {
    if (Symbol* val = paramMap.get(sym->symbol())) {
      CallExpr* call = toCallExpr(sym->parentExpr);
      if (call && call->get(1) == sym) {
        // This is a place where param substitution has already determined the
        // value of a move or assignment. If it's a RVV, then we should ignore
        // the update because the RVV may have multiple valid defs in the AST
        // that we currently cannot squash if there are multiple return
        // statements. For example, consider the following param function:
        //
        // proc foo(type t) param {
        //   if firstCheck(t) then return true;
        //   if otherCheck(t) then return false;
        //   return true;
        // }
        //
        // The final "return true" can manifest as a PRIM_MOVE into the RVV
        // variable. I think we're currently unable to move the def because
        // of GOTOs.
        //
        // If it's not a RVV, then we might be assigning to a user-defined
        // param multiple times. In that case, we'll just return the result
        // and let resolution catch the problem later.
        //
        // The substitution usually happens before resolution, so for
        // assignment, we key off of the name :-(
        if (call->isPrimitive(PRIM_MOVE) || call->isNamed("=")) {
          if (sym->symbol()->hasFlag(FLAG_RVV)) {
            makeNoop(call);
          }
          return result;
        }
      }

      if (sym->symbol()->type != dtUnknown && sym->symbol()->type != val->type) {
        Symbol* toSym = sym->symbol();
        if (!toSym->hasFlag(FLAG_TYPE_VARIABLE)) {
          // This assertion is here to ensure compatibility with old code.
          // Todo: remove after 1.15 release.
          Symbol* newToSym = toSym->type->symbol;
          INT_ASSERT(toSym->type       // cast machinery used this previously
                  == newToSym->type);  // now it will use this
          toSym = newToSym;
        }
        INT_ASSERT(toSym->hasFlag(FLAG_TYPE_VARIABLE));
        CallExpr* cast = createCast(val, toSym);
        sym->replace(cast);

        // see whether preFold will fold this _cast call
        Expr* prevResult = result;
        result = preFold(cast);
        if (result == cast) {
          // if it doesn't, put things back as they were:
          cast->replace(sym);
          result = prevResult;
          // and then do what we would've done if we hadn't used the cast
          sym->setSymbol(val);
        }
      } else {
        sym->setSymbol(val);
      }
    }
  }

  if (CondStmt* cond = toCondStmt(result->parentExpr)) {
    if (cond->condExpr == result) {
      if (Expr* expr = cond->foldConstantCondition()) {
        result = expr;
      } else {
        //
        // push try block
        //
        if (SymExpr* se = toSymExpr(result))
          if (se->symbol() == gTryToken)
            tryStack.add(cond);
      }
    }
  }

  //
  // pop try block and delete else
  //
  if (tryStack.n) {
    if (BlockStmt* block = toBlockStmt(result)) {
      if (tryStack.tail()->thenStmt == block) {
        tryStack.tail()->replace(block->remove());
        tryStack.pop();
      }
    }
  }

  return result;
}


static bool is_param_resolved(FnSymbol* fn, Expr* expr) {
  if (BlockStmt* block = toBlockStmt(expr)) {
    if (block->isWhileStmt() == true) {
      USR_FATAL(expr, "param function cannot contain a non-param while loop");

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
    return true; // param function is resolved
  }

  return false;
}


// Resolves an expression and manages the callStack and tryStack.
// On success, returns the call that was passed in.
// On a try failure, returns either the expression preceding the elseStmt,
// substituted for the body of the param condition (if that substitution could
// be made), or NULL.
// If null, then resolution of the current block should be aborted.  tryFailure
// is true in this case, so the search for a matching elseStmt continue in the
// surrounding block or call.
static Expr*
resolveExpr(Expr* expr) {
  Expr* const origExpr = expr;
  FnSymbol*   fn       = toFnSymbol(expr->parentSymbol);

  SET_LINENO(expr);

  if (isContextCallExpr(expr)) {
    // context call expressions are always already resolved
    // since they are created in resolveNormalFunction to represent
    // alternative resolutions.
    return expr;
  }

  if (SymExpr* se = toSymExpr(expr)) {
    if (se->symbol()) {
      makeRefType(se->symbol()->type);
    }
  }

  expr = preFold(expr);

  if (fn && fn->retTag == RET_PARAM) {
    if (is_param_resolved(fn, expr)) {
      return expr;
    }
  }

  if (DefExpr* def = toDefExpr(expr)) {
    if (def->init) {
      // This section was added to handle type a = b
      Expr* init = preFold(def->init);
      init = resolveExpr(init);
      // expr is unchanged, so is treated as "resolved".
    }

    if (def->sym->hasFlag(FLAG_CHPL__ITER)) {
      implementForallIntents1(def);
    }
  }

  if (CallExpr* call = toCallExpr(expr)) {
    if (call->isPrimitive(PRIM_ERROR) ||
        call->isPrimitive(PRIM_WARNING)) {
      issueCompilerError(call);
    }

    callStack.add(call);

    resolveCall(call);

    if (!tryFailure && call->isResolved()) {
      if (CallExpr* origToLeaderCall = toPrimToLeaderCall(origExpr))
        // ForallLeaderArgs: process the leader that 'call' invokes.
        implementForallIntents2(call, origToLeaderCall);

      else if (CallExpr* eflopiHelper = eflopiMap[call]) {
        implementForallIntents2wrapper(call, eflopiHelper);
      }

      // For ContextCallExprs, be sure to resolve all of the
      // functions that could be called.
      if (ContextCallExpr* cc = toContextCallExpr(call->parentExpr)) {
        CallExpr* refCall = NULL;
        CallExpr* valueCall = NULL;
        CallExpr* constRefCall = NULL;

        cc->getCalls(refCall, valueCall, constRefCall);

        FnSymbol* refFn = refCall->isResolved();
        FnSymbol* valueFn = valueCall?valueCall->isResolved():NULL;
        FnSymbol* constRefFn = constRefCall?constRefCall->isResolved():NULL;

        INT_ASSERT(refFn && (valueFn || constRefFn));
        resolveFns(refFn);

        if (valueFn)
          resolveFns(valueFn);
        if (constRefFn)
          resolveFns(constRefFn);

        // Produce an error if the return types do not match.
        // This error is skipped for iterators because
        // the return type of an iterator is e.g. an iterator record
        // which is not the same as the yielded type.
        if (!refFn->isIterator()) {
          if (valueFn &&
              refFn->retType->getValType() != valueFn->retType->getValType()) {
            USR_FATAL_CONT(cc, "invalid ref return pair: return types differ");
            USR_FATAL_CONT(valueFn, "function returns %s",
                           toString(valueFn->retType));
            USR_FATAL_CONT(refFn, "function returns %s",
                           toString(refFn->retType));
            USR_STOP();
          }
          if (constRefFn &&
              refFn->retType->getValType() != constRefFn->retType->getValType()) {
            USR_FATAL_CONT(cc, "invalid ref return pair: return types differ");
            USR_FATAL_CONT(constRefFn, "function returns %s",
                           toString(constRefFn->retType));
            USR_FATAL_CONT(refFn, "function returns %s",
                           toString(refFn->retType));
            USR_STOP();
          }
        }

        // Proceed using the designated call option
        expr = getDesignatedCall(cc);
      } else {
        INT_ASSERT(call->isResolved());
        resolveFns(call->isResolved());
      }
    }

    if (tryFailure) {
      if (tryStack.n > 0 && tryStack.tail()->parentSymbol == fn) {
        // The code in the 'true' branch of a tryToken conditional has failed
        // to resolve fully. Roll the callStack back to the function where
        // the nearest tryToken conditional is and replace the entire
        // conditional with the 'false' branch then continue resolution on
        // it.  If the 'true' branch did fully resolve, we would replace the
        // conditional with the 'true' branch instead.
        while (callStack.n > 0 &&
               callStack.tail()->isResolved() !=
               tryStack.tail()->elseStmt->parentSymbol) {
          callStack.pop();
        }

        BlockStmt* block = tryStack.tail()->elseStmt;

        tryStack.tail()->replace(block->remove());
        tryStack.pop();

        if (!block->prev)
          block->insertBefore(new CallExpr(PRIM_NOOP));

        tryFailure = false;

        return block->prev;
      } else {
        return NULL;
      }

    }

    callStack.pop();
  }

  if (SymExpr* sym = toSymExpr(expr)) {
    // Avoid record constructors via cast
    // should be fixed by out-of-order resolution
    CallExpr* parent = toCallExpr(sym->parentExpr);

    if (!parent ||
        !parent->isPrimitive(PRIM_IS_SUBTYPE) ||
        !sym->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {

      if (AggregateType* ct = toAggregateType(sym->typeInfo())) {
        // Don't try to resolve the defaultTypeConstructor for string literals
        // (resolution ordering issue, string literals are encountered too early
        // on and we don't know enough to be able to resolve them at that point)
        if (!(ct == dtString && (sym->symbol()->isParameter() ||
                                 sym->symbol()->hasFlag(FLAG_INSTANTIATED_PARAM))) &&
            !ct->symbol->hasFlag(FLAG_GENERIC) &&
            !ct->symbol->hasFlag(FLAG_ITERATOR_CLASS) &&
            !ct->symbol->hasFlag(FLAG_ITERATOR_RECORD) &&
            ct->defaultTypeConstructor) {
          if (ct->initializerStyle == DEFINES_INITIALIZER &&
              (ct->isGeneric() ||
               (isAggregateType(ct->instantiatedFrom) &&
                toAggregateType(ct->instantiatedFrom)->isGeneric()))) {
            USR_FATAL(ct, "Type constructors are not yet supported for generic types that define initializers.  As a workaround, try relying on type inference");
          }

          resolveFormals(ct->defaultTypeConstructor);
          if (resolvedFormals.set_in(ct->defaultTypeConstructor)) {
            if (getPartialCopyInfo(ct->defaultTypeConstructor))
              instantiateBody(ct->defaultTypeConstructor);
            resolveFns(ct->defaultTypeConstructor);
          }
        }
      }
    }
  }

  return postFold(expr);
}


void
resolveBlockStmt(BlockStmt* blockStmt) {
  for_exprs_postorder(expr, blockStmt) {
    expr = resolveExpr(expr);

    if (tryFailure) {
      if (expr == NULL)
        return;

      tryFailure = false;
    }
  }
}


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
    INT_ASSERT(!fn->retType->defaultInitializer);
    FnSymbol* instantiatedFrom = fn->instantiatedFrom;
    while (instantiatedFrom->instantiatedFrom)
      instantiatedFrom = instantiatedFrom->instantiatedFrom;

    CallExpr* call = new CallExpr(instantiatedFrom->retType->defaultInitializer);

    // This should not be happening for iterators.
    TypeSymbol* ts = instantiatedFrom->retType->symbol;
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
    fn->retType->defaultInitializer = call->isResolved();
    INT_ASSERT(fn->retType->defaultInitializer);
    //      resolveFns(fn->retType->defaultInitializer);
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
    retType = getReturnedTupleType(fn, retType);
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
      if (isAggregateType(parent)        == true     &&
          parent                         != dtValue  &&
          parent                         != dtObject &&
          parent->defaultTypeConstructor != NULL) {
        resolveFormals(parent->defaultTypeConstructor);

        if (resolvedFormals.set_in(parent->defaultTypeConstructor)) {
          resolveFns(parent->defaultTypeConstructor);
        }
      }
    }

    if (AggregateType* ct = toAggregateType(fn->retType)) {
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

    // This instantiates the default constructor
    // for  the corresponding type constructor.
    instantiate_default_constructor(fn);

    //
    // resolve destructor
    //
    if (AggregateType* ct = toAggregateType(fn->retType)) {
      if (!ct->destructor &&
          !ct->symbol->hasFlag(FLAG_REF) &&
          !isTupleContainingOnlyReferences(ct)) {
        BlockStmt* block = new BlockStmt();
        VarSymbol* tmp   = newTemp(ct);
        CallExpr*  call  = new CallExpr("deinit", gMethodToken, tmp);

        // In case resolveCall drops other stuff into the tree ahead of the
        // call, we wrap everything in a block for safe removal.

        block->insertAtHead(call);

        fn->insertAtHead(block);
        fn->insertAtHead(new DefExpr(tmp));

        resolveCallAndCallee(call);

        ct->destructor = call->isResolved();

        block->remove();

        tmp->defPoint->remove();
      }
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


static bool
possible_signature_match(FnSymbol* fn, FnSymbol* gn) {
  if (fn->name != gn->name)
    return false;
  if (fn->numFormals() != gn->numFormals())
    return false;
  for (int i = 3; i <= fn->numFormals(); i++) {
    ArgSymbol* fa = fn->getFormal(i);
    ArgSymbol* ga = gn->getFormal(i);
    if (strcmp(fa->name, ga->name))
      return false;
  }
  return true;
}


// Checks that types match.
// Note - does not currently check that instantiated params match.
static bool
signature_match(FnSymbol* fn, FnSymbol* gn) {
  if (fn->name != gn->name)
    return false;
  if (fn->numFormals() != gn->numFormals())
    return false;
  for (int i = 3; i <= fn->numFormals(); i++) {
    ArgSymbol* fa = fn->getFormal(i);
    ArgSymbol* ga = gn->getFormal(i);
    if (strcmp(fa->name, ga->name))
      return false;
    if (fa->type != ga->type)
      return false;
  }
  return true;
}


//
// add to vector icts all types instantiated from ct
//
static void
collectInstantiatedAggregateTypes(Vec<Type*>& icts, Type* ct) {
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->type->defaultTypeConstructor)
      if (!ts->hasFlag(FLAG_GENERIC) &&
          ts->type->defaultTypeConstructor->instantiatedFrom ==
          ct->defaultTypeConstructor) {
        icts.add(ts->type);
        INT_ASSERT(isInstantiation(ts->type, ct));
        INT_ASSERT(ts->type->instantiatedFrom == ct);
      }
  }
}


//
// return true if child overrides parent in dispatch table
//
static bool
isVirtualChild(FnSymbol* child, FnSymbol* parent) {
  if (Vec<FnSymbol*>* children = virtualChildrenMap.get(parent)) {
    forv_Vec(FnSymbol*, candidateChild, *children) {
      if (candidateChild == child)
        return true;
    }
  }
  return false;
}


/*

-----------
addToVirtualMaps, addAllToVirtualMaps, buildVirtualMaps

buildVirtualMaps invokes addAllToVirtualMaps on each method 'fn' declared in
each class.  Only non-generic classes and instantiations of generic classes are
considered.

addAllToVirtualMaps invokes addToVirtualMaps on each (transitive) dispatch
child for that class, passing that same method 'fn'.

addToVirtualMaps adds to the virtual maps all methods
in that dispatch child that could override the above 'fn'.

These functions go down the inheritance hierarchy because in the current
language, a call to a class with static type could dispatch to any child call.
If calling an arbitrary super method is supported, a call to an object of static
child type could end up calling something in the parent.

-----------
*/

//
// add methods that possibly match pfn to vector,
// but does not add instantiated generics, since addToVirtualMaps
// will instantiate again.
static void
collectMethodsForVirtualMaps(Vec<FnSymbol*>& methods, Type* ct, FnSymbol* pfn) {
  Vec<FnSymbol*> tmp;
  std::set<FnSymbol*> generics;

  // Gather the generic, concrete, instantiated methods
  for (Type* fromType = ct;
       fromType != NULL;
       fromType = fromType->instantiatedFrom) {

    forv_Vec(FnSymbol, cfn, fromType->methods) {
      if (cfn && possible_signature_match(pfn, cfn))
        tmp.add(cfn);
    }
  }

  // Don't add instantiations of generics if we were already
  // going to add the generic version. addToVirtualMaps will
  // re-instantiate.

  // So, gather a set of generic versions.
  forv_Vec(FnSymbol, cfn, tmp)
    if (cfn->hasFlag(FLAG_GENERIC))
      generics.insert(cfn);

  // Then, add anything not instantiated from something in
  // the set.
  forv_Vec(FnSymbol, cfn, tmp) {
    if (cfn->instantiatedFrom && generics.count(cfn->instantiatedFrom)) {
      // skip it
    } else {
      // add it
      methods.add(cfn);
    }
  }
}


// addToVirtualMaps itself goes through each method in ct and if
// that method could override pfn, adds it to the virtual maps
static void
addToVirtualMaps(FnSymbol* pfn, AggregateType* ct) {
  // Collect methods from ct and ct->instantiatedFrom.
  // Does not include generic instantiations - sometimes we
  // have to make the instantiation here, so we always run
  // through a code path that instantiates.
  Vec<FnSymbol*> methods;
  collectMethodsForVirtualMaps(methods, ct, pfn);

  forv_Vec(FnSymbol, cfn, methods) {
    if (cfn && !cfn->instantiatedFrom) {
      Vec<Type*> types;
      if (ct->symbol->hasFlag(FLAG_GENERIC))
        collectInstantiatedAggregateTypes(types, ct);
      else
        types.add(ct);

      forv_Vec(Type, type, types) {
        SymbolMap subs;
        if (ct->symbol->hasFlag(FLAG_GENERIC) ||
            cfn->getFormal(2)->type->symbol->hasFlag(FLAG_GENERIC))
          // instantiateSignature handles subs from formal to a type
          subs.put(cfn->getFormal(2), type->symbol);
        for (int i = 3; i <= cfn->numFormals(); i++) {
          ArgSymbol* arg = cfn->getFormal(i);
          if (arg->intent == INTENT_PARAM) {
            subs.put(arg, paramMap.get(pfn->getFormal(i)));
          } else if (arg->type->symbol->hasFlag(FLAG_GENERIC)) {
            subs.put(arg, pfn->getFormal(i)->type->symbol);
          }
        }

        FnSymbol* fn = cfn;

        if (subs.n) {
          fn = instantiate(fn, subs);

          if (fn) {
            if (type->defaultTypeConstructor->instantiationPoint)
              fn->instantiationPoint = type->defaultTypeConstructor->instantiationPoint;
            else
              fn->instantiationPoint = toBlockStmt(type->defaultTypeConstructor->defPoint->parentExpr);
            INT_ASSERT(fn->instantiationPoint);
          }
        }

        if (fn) {
          resolveFormals(fn);

          if (signature_match(pfn, fn) && evaluateWhereClause(fn)) {
            resolveFns(fn);

            if (fn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD) &&
                pfn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
              AggregateType* fnRetType = toAggregateType(fn->retType);
              IteratorInfo* fnInfo = fnRetType->iteratorInfo;
              AggregateType* pfnRetType = toAggregateType(pfn->retType);
              IteratorInfo* pfnInfo = pfnRetType->iteratorInfo;
              if (!isSubType(fnInfo->getValue->retType,
                             pfnInfo->getValue->retType)) {
                USR_FATAL_CONT(pfn, "conflicting return type specified for '%s: %s'", toString(pfn),
                               pfnInfo->getValue->retType->symbol->name);
                USR_FATAL_CONT(fn, "  overridden by '%s: %s'", toString(fn),
                               fnInfo->getValue->retType->symbol->name);
                USR_STOP();
              } else {
                pfn->retType->dispatchChildren.add_exclusive(fn->retType);
                fn->retType->dispatchParents.add_exclusive(pfn->retType);
                Type* pic = pfnInfo->iclass;
                Type* ic = fnInfo->iclass;
                INT_ASSERT(ic->symbol->hasFlag(FLAG_ITERATOR_CLASS));

                // Iterator classes are created as normal top-level classes (inheriting
                // from dtObject).  Here, we want to re-parent ic with pic, so
                // we need to remove and replace the object base class.
                INT_ASSERT(ic->dispatchParents.n == 1);
                Type* parent = ic->dispatchParents.only();
                if (parent == dtObject)
                {
                  int item = parent->dispatchChildren.index(ic);
                  parent->dispatchChildren.remove(item);
                  ic->dispatchParents.remove(0);
                }
                pic->dispatchChildren.add_exclusive(ic);
                ic->dispatchParents.add_exclusive(pic);
                continue; // do not add to virtualChildrenMap; handle in _getIterator
              }
            } else if (!isSubType(fn->retType, pfn->retType)) {
              USR_FATAL_CONT(pfn, "conflicting return type specified for '%s: %s'", toString(pfn), pfn->retType->symbol->name);
              USR_FATAL_CONT(fn, "  overridden by '%s: %s'", toString(fn), fn->retType->symbol->name);
              USR_STOP();
            }
            {
              Vec<FnSymbol*>* fns = virtualChildrenMap.get(pfn);
              if (!fns) fns = new Vec<FnSymbol*>();
              fns->add(fn);
              virtualChildrenMap.put(pfn, fns);
              fn->addFlag(FLAG_VIRTUAL);
              pfn->addFlag(FLAG_VIRTUAL);
            }
            {
              Vec<FnSymbol*>* fns = virtualRootsMap.get(fn);
              if (!fns) fns = new Vec<FnSymbol*>();
              bool added = false;

              //
              // check if parent or child already exists in vector
              //
              for (int i = 0; i < fns->n; i++) {
                //
                // if parent already exists, do not add child to vector
                //
                if (isVirtualChild(pfn, fns->v[i])) {
                  added = true;
                  break;
                }

                //
                // if child already exists, replace with parent
                //
                if (isVirtualChild(fns->v[i], pfn)) {
                    fns->v[i] = pfn;
                    added = true;
                    break;
                }
              }

              if (!added)
                fns->add(pfn);

              virtualRootsMap.put(fn, fns);
            }
          }
        }
      }
    }
  }
}


// Add overrides of fn to virtual maps down the inheritance hierarchy
static void
addAllToVirtualMaps(FnSymbol* fn, AggregateType* pct) {
  forv_Vec(Type, t, pct->dispatchChildren) {
    AggregateType* ct = toAggregateType(t);
    if (ct->defaultTypeConstructor &&
        (ct->defaultTypeConstructor->hasFlag(FLAG_GENERIC) ||
         ct->defaultTypeConstructor->isResolved()))
      addToVirtualMaps(fn, ct);

    // add overrides of method fn by children of ct to virtual maps
    addAllToVirtualMaps(fn, ct);
  }
}


static void
buildVirtualMaps() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_WRAPPER))
      // Only "true" functions are used to populate virtual maps.
      continue;

    if (! fn->isResolved())
      // Only functions that are actually used go into the virtual map.
      continue;

    if (fn->hasFlag(FLAG_NO_PARENS))
      // Parentheses-less functions are statically bound; that is, they are not
      // dispatched through the virtual table.
      continue;

    if (fn->retTag == RET_PARAM || fn->retTag == RET_TYPE)
      // Only run-time functions populate the virtual map.
      continue;

    if (fn->numFormals() > 1 && fn->getFormal(1)->type == dtMethodToken) {
      // Only methods go in the virtual function table.
      if (AggregateType* pt = toAggregateType(fn->getFormal(2)->type)) {

        if (isClass(pt) && !pt->symbol->hasFlag(FLAG_GENERIC)) {
          // MPF - note the check for generic seemed to originate
          // in SVN revision 7103
          addAllToVirtualMaps(fn, pt);
        }
      }
    }
  }
}


// if exclusive=true, check for fn already existing in the virtual method
// table and do not add it a second time if it is already present.
static void
addVirtualMethodTableEntry(Type* type, FnSymbol* fn, bool exclusive /*= false*/) {
  Vec<FnSymbol*>* fns = virtualMethodTable.get(type);
  if (!fns) fns = new Vec<FnSymbol*>();
  if (exclusive) {
    forv_Vec(FnSymbol, f, *fns) {
      if (f == fn)
        return;
    }
  }
  fns->add(fn);
  virtualMethodTable.put(type, fns);
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


void resolveTypedefedArgTypes(FnSymbol* fn)
{
  for_formals(formal, fn)
  {
    INT_ASSERT(formal->type); // Should be *something*.
    if (formal->type != dtUnknown)
      continue;

    if (BlockStmt* block = formal->typeExpr)
    {
      if (SymExpr* se = toSymExpr(block->body.first()))
      {
        if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE))
        {
          Type* type = resolveTypeAlias(se);
          INT_ASSERT(type);
          formal->type = type;
        }
      }
    }
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
    if (mod->block->modUses) {
      for_actuals(expr, mod->block->modUses) {
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


  // --ipe does not build a mainModule
  if (mainModule)
    resolveUses(mainModule);

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

  insertDynamicDispatchCalls();
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
  freeCache(capturedValues);

  Vec<VisibleFunctionBlock*> vfbs;
  visibleFunctionMap.get_values(vfbs);
  forv_Vec(VisibleFunctionBlock, vfb, vfbs) {
    Vec<Vec<FnSymbol*>*> vfns;
    vfb->visibleFunctions.get_values(vfns);
    forv_Vec(Vec<FnSymbol*>, vfn, vfns) {
      delete vfn;
    }
    delete vfb;
  }
  visibleFunctionMap.clear();
  visibilityBlockCache.clear();
  clearPartialCopyFnMap();

  forv_Vec(BlockStmt, stmt, gBlockStmts) {
    stmt->moduleUseClear();
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
          formal->typeExpr = new BlockStmt(new CallExpr(formal->type->defaultTypeConstructor));
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
  if (fPrintModuleResolution)
    fprintf(stderr, "%2d Resolving module %s ...", module_resolution_depth, mod->name);

  FnSymbol* fn = mod->initFn;
  resolveFormals(fn);
  resolveFns(fn);

  if (FnSymbol* defn = mod->deinitFn) {
    resolveFormals(defn);
    resolveFns(defn);
  }

  if (fPrintModuleResolution)
    putc('\n', stderr);

  --module_resolution_depth;
}

static void resolveSupportForModuleDeinits() {
  // We will need these in addInitGuards.cpp
  SET_LINENO(chpl_gen_main);
  Expr*    modNameDum = buildCStringLiteral("");
  VarSymbol* fnPtrDum = newTemp("fnPtr", dtCFnPtr);
  CallExpr* addModule = new CallExpr("chpl_addModule", modNameDum, fnPtrDum);
  resolveUninsertedCall(chpl_gen_main->body, NULL, addModule, false);
  gAddModuleFn = addModule->isResolved();
  resolveFns(gAddModuleFn);
  // Also in buildDefaultFunctions.cpp: new CallExpr("chpl_deinitModules")
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

// removes entries in virtualChildrenMap that are not in virtualMethodTable.
// such entries could not be called and should be dead-code eliminated.
static void filterVirtualChildren()
{
  std::set<FnSymbol*> fns_in_vmt;
  typedef MapElem<Type*,Vec<FnSymbol*>*> VmtMapElem;
  typedef MapElem<FnSymbol*,Vec<FnSymbol*>*> ChildMapElem;
  form_Map(VmtMapElem, el,  virtualMethodTable) {
    if (el->value) {
      forv_Vec(FnSymbol, fn, *el->value) {
        fns_in_vmt.insert(fn);
      }
    }
  }
  form_Map(ChildMapElem, el, virtualChildrenMap) {
    if (el->value) {
      Vec<FnSymbol*>* oldV = el->value;
      Vec<FnSymbol*>* newV = new Vec<FnSymbol*>();
      forv_Vec(FnSymbol, fn, *oldV) {
        if (fns_in_vmt.count(fn)) {
          newV->add(fn);
        }
      }
      el->value = newV;
      delete oldV;
    }
  }
}

static void resolveDynamicDispatches() {
  inDynamicDispatchResolution = true;
  int num_types;
  do {
    num_types = gTypeSymbols.n;
    {
      Vec<Vec<FnSymbol*>*> values;
      virtualChildrenMap.get_values(values);
      forv_Vec(Vec<FnSymbol*>, value, values) {
        delete value;
      }
    }
    virtualChildrenMap.clear();
    {
      Vec<Vec<FnSymbol*>*> values;
      virtualRootsMap.get_values(values);
      forv_Vec(Vec<FnSymbol*>, value, values) {
        delete value;
      }
    }
    virtualRootsMap.clear();
    buildVirtualMaps();
  } while (num_types != gTypeSymbols.n);

  for (int i = 0; i < virtualRootsMap.n; i++) {
    if (virtualRootsMap.v[i].key) {
      for (int j = 0; j < virtualRootsMap.v[i].value->n; j++) {
        FnSymbol* root = virtualRootsMap.v[i].value->v[j];
        addVirtualMethodTableEntry(root->_this->type, root, true);
      }
    }
  }

  Vec<Type*> ctq;
  ctq.add(dtObject);
  forv_Vec(Type, ct, ctq) {
    if (Vec<FnSymbol*>* parentFns = virtualMethodTable.get(ct)) {
      forv_Vec(FnSymbol, pfn, *parentFns) {
        // Each subtype can contribute only one function to the
        // virtual method table.
        Vec<Type*> childSet;
        if (Vec<FnSymbol*>* childFns = virtualChildrenMap.get(pfn)) {
          forv_Vec(FnSymbol, cfn, *childFns) {
            forv_Vec(Type, pt, cfn->_this->type->dispatchParents) {
              if (pt == ct) {
                if (!childSet.set_in(cfn->_this->type)) {
                  addVirtualMethodTableEntry(cfn->_this->type, cfn);
                  childSet.set_add(cfn->_this->type);
                }
                break;
              }
            }
          }
        }
        forv_Vec(Type, childType, ct->dispatchChildren) {
          if (!childSet.set_in(childType)) {
            addVirtualMethodTableEntry(childType, pfn);
          }
        }
      }
    }
    forv_Vec(Type, child, ct->dispatchChildren) {
      ctq.add(child);
    }
  }

  // reverse the entries in the virtual method table
  // populate the virtualMethodMap
  for (int i = 0; i < virtualMethodTable.n; i++) {
    if (virtualMethodTable.v[i].key) {
      virtualMethodTable.v[i].value->reverse();
      for (int j = 0; j < virtualMethodTable.v[i].value->n; j++) {
        virtualMethodMap.put(virtualMethodTable.v[i].value->v[j], j);
      }
    }
  }

  // remove entries in virtualChildrenMap that are not in
  // virtualMethodTable. When a parent has a generic method and
  // a subclass has a specific one, the virtualChildrenMap might
  // get multiple entries while the logic above with childSet
  // ensures that the virtualMethodTable only has one entry.
  filterVirtualChildren();

  inDynamicDispatchResolution = false;

  if (fPrintDispatch) {
    printf("Dynamic dispatch table:\n");
    for (int i = 0; i < virtualMethodTable.n; i++) {
      if (Type* t = virtualMethodTable.v[i].key) {
        printf("  %s\n", toString(t));
        for (int j = 0; j < virtualMethodTable.v[i].value->n; j++) {
          FnSymbol* fn = virtualMethodTable.v[i].value->v[j];
          printf("    %s", toString(fn));
          if (developer) {
            int index = virtualMethodMap.get(fn);
            printf(" index %i", index);
            if ( fn->getFormal(2)->typeInfo() == t ) {
              // print dispatch children if the function is
              // the method in type t (and not, say, the version in
              // some parent class e.g. object).
              Vec<FnSymbol*>* childFns = virtualChildrenMap.get(fn);
              if (childFns) {
                printf(" %i children:\n", childFns->n);
                for (int k = 0; k < childFns->n; k++) {
                  FnSymbol* childFn = childFns->v[k];
                  printf("      %s\n", toString(childFn));
                }
              }
              if( childFns == NULL || childFns->n == 0 ) printf("\n");
            } else {
              printf(" inherited\n");
            }
          }
          printf("\n");
        }
        if (developer)
          printf("\n");
      }
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
      VarSymbol* tmp = newTemp("_runtime_type_tmp_", ts->type);
      ts->type->defaultInitializer->insertBeforeEpilogue(new DefExpr(tmp));
      CallExpr* call = new CallExpr("chpl__convertValueToRuntimeType", tmp);
      ts->type->defaultInitializer->insertBeforeEpilogue(call);
      resolveCallAndCallee(call);
      valueToRuntimeTypeMap.put(ts->type, call->isResolved());
      call->remove();
      tmp->defPoint->remove();
    }
  }
}

static void resolveAutoCopies() {
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (!ts->defPoint->parentSymbol)
      continue; // Type is not in tree

    if (ts->hasFlag(FLAG_GENERIC))
      continue; // Consider only concrete types.

    if (ts->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION))
      continue; // Skip the "dmapped" pseudo-type.

    if (isRecord(ts->type)) {
      resolveAutoCopyEtc(ts->type);
    }

    if (isAggregateType(ts->type) ) {
      // Mark record/tuple/class types as POD or NOT_POD
      propagateNotPOD(ts->type);
    }
  }
}

/* In order to correctly initialize records or tuples in which
   a component has FLAG_IGNORE_NOINIT, we need to propagate that
   flag to the parent types as well. While doing so, we also
   propagate whether or not that type is not Plain-old-Data
   (which would mean it does not need auto copies or auto
    destroys - bit copies will do).

   After this function is called on an aggregate type, that type
   will be marked FLAG_POD and FLAG_NOT_POD, and this function
   will be called recursively to also mark any aggregate type
   fields with FLAG_POD or FLAG_NOT_POD.

   After setting either FLAG_POD or FLAG_NOT_POD if necessary,
   returns true if FLAG_NOT_POD is set, false otherwise.

   This function should only be called during resolution.
   Call isPOD (or check FLAG_POD/FLAG_NOT_POD) after resolution.
 */
static bool propagateNotPOD(Type* t) {

  // non-aggregate types (e.g. int, bool) are POD
  // but we don't run this function on all of them,
  // so we don't mark them with FLAG_POD.
  if (!isAggregateType(t))
    return false;

  // Move past any types that we've already handled
  if (t->symbol->hasFlag(FLAG_POD))
    return false;

  if (t->symbol->hasFlag(FLAG_NOT_POD))
    return true;

  AggregateType* at     = (AggregateType*) t;
  bool           notPOD = false;

  // Some special rules for special things.
  if (isSyncType(t)   ||
      isSingleType(t) ||
      t->symbol->hasFlag(FLAG_ATOMIC_TYPE)) {
    notPOD = true;
  }

  // Most class types are POD (user classes, _ddata, c_ptr)
  // Also, there is no need to check the fields of a class type
  // since a variable of that type is a pointer to the instance.
  if ( isClass(t) ) {
    // don't enumerate sub-fields or check for autoCopy etc

  } else {
    // If any field in a record/tuple is not POD, the
    // aggregate is not POD.
    for_fields(field, at) {
      Type* ft = field->typeInfo();

      notPOD |= propagateNotPOD(ft);
    }

    // Make sure we have resolved auto copy/auto destroy.
    // Except not for runtime types, because that causes
    // some sort of fatal resolution error. This is a workaround.
    if (! t->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) {
      resolveAutoCopyEtc(t);
    }

    // Also check for a non-compiler generated autocopy/autodestroy.
    FnSymbol* autoCopyFn    = autoCopyMap[t];
    FnSymbol* autoDestroyFn = autoDestroyMap.get(t);
    FnSymbol* destructor    = t->destructor;

    // Ignore invisible (compiler-defined) functions.
    if (autoCopyFn && autoCopyFn->hasFlag(FLAG_COMPILER_GENERATED)) {
      autoCopyFn = NULL;
    }

    if (autoDestroyFn && autoDestroyFn->hasFlag(FLAG_COMPILER_GENERATED)) {
      autoDestroyFn = NULL;
    }

    if (destructor && destructor->hasFlag(FLAG_COMPILER_GENERATED)) {
      destructor = NULL;
    }

    // if it has flag ignore no-init, it's not pod
    // if it has a user-specified auto copy / auto destroy, it's not pod
    // if it has a user-specified destructor, it's not pod
    if (t->symbol->hasFlag(FLAG_IGNORE_NOINIT) ||
        autoCopyFn                             ||
        autoDestroyFn                          ||
        destructor) {
      notPOD = true;
    }

    // Since hasUserAssign tries to resolve =, we only
    // check it if we think we have a POD type.
    if (!notPOD && hasUserAssign(t)) {
      notPOD = true;
    }
  }

  if (notPOD) {
    t->symbol->addFlag(FLAG_NOT_POD);
  } else {
    t->symbol->addFlag(FLAG_POD);
  }

  return notPOD;
}

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
      CallExpr* res = new CallExpr(type->defaultTypeConstructor);
      for_formals(formal, type->defaultTypeConstructor) {
        Vec<Symbol *> keys;
        // Finds each named argument in the type constructor and inserts
        // the substitution provided.
        type->substitutions.get_keys(keys);
        // I don't think we can guarantee that the substitutions will be
        // in the same order as the arguments for the defaultTypeConstructor.
        // That would make this O(n) instead of potentially O(n*n)
        forv_Vec(Symbol, key, keys) {
          if (!strcmp(formal->name, key->name)) {
            Symbol* formalVal = type->substitutions.get(key);
            res->insertAtTail(new NamedExpr(formal->name,
                                            new SymExpr(formalVal)));
          }
        }
      }
      init->get(1)->replace(res);
      resolveCall(res);
      makeNoop(toCallExpr(init->parentExpr));
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
      CallExpr* classCall = new CallExpr(type->getField("_instance")->type->defaultInitializer);
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
}


static void insertDynamicDispatchCalls() {
  // Select resolved calls whose function appears in the virtualChildrenMap.
  // These are the dynamically-dispatched calls.
  forv_Vec(CallExpr, call, gCallExprs) {
    if (!call->parentSymbol) continue;
    if (!call->getStmtExpr()) continue;

    FnSymbol* key = call->isResolved();
    if (!key) continue;

    Vec<FnSymbol*>* fns = virtualChildrenMap.get(key);
    if (!fns) continue;

    SET_LINENO(call);

    bool isSuperAccess = false;
    if (SymExpr* base = toSymExpr(call->get(2))) {
      isSuperAccess = base->symbol()->hasFlag(FLAG_SUPER_TEMP);
    }

    if ((fns->n + 1 > fConditionalDynamicDispatchLimit) && !isSuperAccess ) {
      //
      // change call of root method into virtual method call;
      // Insert function SymExpr and virtual method temp at head of argument
      // list.
      //
      // N.B.: The following variable must have the same size as the type of
      // chpl__class_id / chpl_cid_* -- otherwise communication will cause
      // problems when it tries to read the cid of a remote class.  See
      // test/classes/sungeun/remoteDynamicDispatch.chpl (on certain
      // machines and configurations).
      VarSymbol* cid = newTemp("_virtual_method_tmp_", dtInt[INT_SIZE_32]);
      call->getStmtExpr()->insertBefore(new DefExpr(cid));
      call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, cid, new CallExpr(PRIM_GETCID, call->get(2)->copy())));
      call->get(1)->insertBefore(new SymExpr(cid));
      // "remove" here means VMT calls are not really "resolved".
      // That is, calls to isResolved() return NULL.
      call->get(1)->insertBefore(call->baseExpr->remove());
      call->primitive = primitives[PRIM_VIRTUAL_METHOD_CALL];
      // This clause leads to necessary reference temporaries not being inserted,
      // while the clause below works correctly. <hilde>
      // Increase --conditional-dynamic-dispatch-limit to see this.
    } else {
      forv_Vec(FnSymbol, fn, *fns) {
        Type* type = fn->getFormal(2)->type;
        CallExpr* subcall = call->copy();
        SymExpr* tmp = new SymExpr(gNil);

      // Build the IF block.
        BlockStmt* ifBlock = new BlockStmt();
        VarSymbol* cid = newTemp("_dynamic_dispatch_tmp_", dtBool);
        ifBlock->insertAtTail(new DefExpr(cid));

        Expr* getCid = NULL;
        if (isSuperAccess) {
          // We're in a call on the super type.  We already know the answer to
          // this if conditional
          getCid = new SymExpr(gFalse);
        } else {
          getCid = new CallExpr(PRIM_TESTCID, call->get(2)->copy(),
                                type->symbol);
        }

        ifBlock->insertAtTail(new CallExpr(PRIM_MOVE, cid, getCid));
        VarSymbol* _ret = NULL;
        if (key->retType != dtVoid) {
          _ret = newTemp("_return_tmp_", key->retType);
          ifBlock->insertAtTail(new DefExpr(_ret));
        }
      // Build the TRUE block.
        BlockStmt* trueBlock = new BlockStmt();
        if (fn->retType == key->retType) {
          if (_ret)
            trueBlock->insertAtTail(new CallExpr(PRIM_MOVE, _ret, subcall));
          else
            trueBlock->insertAtTail(subcall);
        } else if (isSubType(fn->retType, key->retType)) {
          // Insert a cast to the overridden method's return type
          VarSymbol* castTemp = newTemp("_cast_tmp_", fn->retType);
          trueBlock->insertAtTail(new DefExpr(castTemp));
          trueBlock->insertAtTail(new CallExpr(PRIM_MOVE, castTemp,
                                               subcall));
          INT_ASSERT(_ret);
          trueBlock->insertAtTail(new CallExpr(PRIM_MOVE, _ret,
                                    new CallExpr(PRIM_CAST,
                                                 key->retType->symbol,
                                                 castTemp)));
        } else
          INT_FATAL(key, "unexpected case");

      // Build the FALSE block.
        BlockStmt* falseBlock = NULL;
        if (_ret)
          falseBlock = new BlockStmt(new CallExpr(PRIM_MOVE, _ret, tmp));
        else
          falseBlock = new BlockStmt(tmp);

        ifBlock->insertAtTail(new CondStmt(
                                new SymExpr(cid),
                                trueBlock,
                                falseBlock));
        if (key->retType == dtUnknown)
          INT_FATAL(call, "bad parent virtual function return type");
        call->getStmtExpr()->insertBefore(ifBlock);
        if (_ret)
          call->replace(new SymExpr(_ret));
        else
          call->remove();
        tmp->replace(call);
        subcall->baseExpr->replace(new SymExpr(fn));
        if (SymExpr* se = toSymExpr(subcall->get(2))) {
          VarSymbol* tmp = newTemp("_cast_tmp_", type);
          se->getStmtExpr()->insertBefore(new DefExpr(tmp));
          se->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new
                CallExpr(PRIM_CAST, type->symbol, se->symbol())));
          se->replace(new SymExpr(tmp));
        } else if (CallExpr* ce = toCallExpr(subcall->get(2)))
          if (ce->isPrimitive(PRIM_CAST))
            ce->get(1)->replace(new SymExpr(type->symbol));
          else
            INT_FATAL(subcall, "unexpected");
        else
          INT_FATAL(subcall, "unexpected");
      }
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
      if (FnSymbol* fn = call->isResolved()) {
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
      if (FnSymbol* fn = call->isResolved()) {
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

static void cleanupVoidVarsAndFields() {
  // remove most uses of void variables and fields
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->inTree()) {
      if (call->isPrimitive(PRIM_MOVE)) {
        if (call->get(2)->typeInfo() == dtVoid ||
            call->get(2)->typeInfo() == dtVoid->refType) {
          INT_ASSERT(call->get(1)->typeInfo() == call->get(2)->typeInfo());
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
        if (call->get(3)->typeInfo() == dtVoid) {
          INT_ASSERT(call->get(2)->typeInfo() == dtVoid);
          if (CallExpr* rhs = toCallExpr(call->get(2))) {
            Expr* rmRhs = rhs->remove();
            call->insertBefore(rmRhs);
            call->remove();
          } else if (isSymExpr(call->get(2))) {
            call->remove();
          }
        }
      } else if (call->isPrimitive(PRIM_RETURN)) {
        if (call->get(1)->typeInfo() == dtVoid ||
            call->get(1)->typeInfo() == dtVoid->refType) {
          if (SymExpr* ret = toSymExpr(call->get(1))) {
            if (ret->symbol() != gVoid) {
              SET_LINENO(call);
              call->replace(new CallExpr(PRIM_RETURN, gVoid));
            }
          }
        }
      }
      if (call->isResolved()) {
        for_actuals(actual, call) {
          if (actual->typeInfo() == dtVoid) {
            actual->remove();
          }
        }
      }
    }
  }
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->defPoint->inTree()) {
      for_formals(formal, fn) {
        if (formal->type == dtVoid) {
          if (formal == fn->_this) {
            fn->_this = NULL;
          }
          formal->defPoint->remove();
        }
      }
      if (fn->retType == dtVoid->refType) {
        fn->retType = dtVoid;
      }
    }
  }
  forv_Vec(DefExpr, def, gDefExprs) {
    if (def->inTree()) {
      if (def->sym->type == dtVoid || def->sym->type == dtVoid->refType) {
        if (VarSymbol* var = toVarSymbol(def->sym)) {
          if (def->parentSymbol != dtVoid->refType->symbol) {
            if (var != gVoid) {
              def->remove();
            }
          }
        }
      }
    }
  }
}


//
// pruneResolvedTree -- prunes and cleans the AST after all of the
// function calls and types have been resolved
//
static void
pruneResolvedTree() {
  removeUnusedFunctions();
  if (fRemoveUnreachableBlocks)
    deadBlockElimination();
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
  // Before removing an unused function, check if it is a defaultInitializer.
  // If unusedFn is a defaultInitializer, its retType's defaultInitializer
  // field will be unusedFn. Set the defaultInitializer field to NULL so the
  // removed function doesn't leave behind a garbage pointer.
  if (unusedFn->retType->defaultInitializer == unusedFn) {
    unusedFn->retType->defaultInitializer = NULL;
  }
  // Also remove unused fns from iterator infos.
  // Ditto for iterator fn in iterator info.
  AggregateType* at = toAggregateType(unusedFn->retType);
  if (at && at->iteratorInfo) {
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

static void removeCopyFns(Type* t) {
  // If they exist, remove the autoCopy and autoDestroy functions
  // for type t, which is about to be removed itself.
  if (FnSymbol* autoDestroy = autoDestroyMap.get(t)) {
    autoDestroyMap.put(t, NULL);
    autoDestroy->defPoint->remove();
  }

  if (FnSymbol* autoCopy = autoCopyMap[t]) {
    autoCopyMap.erase(t);
    autoCopy->defPoint->remove();
  }
}

static void removeUnusedFunctions() {
  // Remove unused functions
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_PRINT_MODULE_INIT_FN)) continue;
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

  // FALSE if the type defines an initializer and that initializer was
  // resolved
  if (ct->initializerStyle == DEFINES_INITIALIZER &&
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
        if (type->type->defaultTypeConstructor->defPoint->parentSymbol)
          type->type->defaultTypeConstructor->defPoint->remove();
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


static void replaceInitPrims(std::vector<BaseAST*>& asts)
{
  for_vector(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      // We are only interested in INIT primitives.
      if (call->isPrimitive(PRIM_INIT)) {
        FnSymbol* parent = toFnSymbol(call->parentSymbol);

        // Call must be in the tree and lie in a resolved function.
        if (! parent || ! parent->isResolved())
          continue;

        SymExpr* se = toSymExpr(call->get(1));
        Type* rt = se->symbol()->type;

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
        }
        else
        {
          Expr* expr = resolvePrimInit(call);

          if (! expr)
          {
            // This PRIM_INIT could not be resolved.

            // But that's OK if it's an extern type.
            // (We don't expect extern types to have initializers.)
            // Also, we don't generate initializers for iterator records.
            // Maybe we can avoid adding PRIM_INIT for these cases in the first
            // place....
            if (rt->symbol->hasFlag(FLAG_EXTERN) ||
                rt->symbol->hasFlag(FLAG_ITERATOR_RECORD))
            {
              INT_ASSERT(toCallExpr(call->parentExpr)->isPrimitive(PRIM_MOVE));
              makeNoop(toCallExpr(call->parentExpr));
              continue;
            }

            INT_FATAL(call, "PRIM_INIT should have already been handled");
          }
        }
      }
    }
  }
}


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


static void
fixTypeNames(AggregateType* ct)
{
  const char default_domain_name[] = "DefaultRectangularDom";

  if (!ct->symbol->hasFlag(FLAG_BASE_ARRAY) && isArrayClass(ct))
  {
    const char* domain_type = ct->getField("dom")->type->symbol->name;
    const char* elt_type = ct->getField("eltType")->type->symbol->name;
    ct->symbol->name = astr("[", domain_type, "] ", elt_type);
  }
  if (ct->instantiatedFrom &&
      !strcmp(ct->instantiatedFrom->symbol->name, default_domain_name)) {
    ct->symbol->name = astr("domain", ct->symbol->name+strlen(default_domain_name));
  }
  if (isRecordWrappedType(ct)) {
    ct->symbol->name = ct->getField("_instance")->type->symbol->name;
  }
}


static void
setScalarPromotionType(AggregateType* ct) {
  for_fields(field, ct) {
    if (!strcmp(field->name, "_promotionType"))
      ct->scalarPromotionType = field->type;
  }
}


