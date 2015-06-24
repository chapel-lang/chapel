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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include "resolution.h"
#include "astutil.h"
#include "stlUtil.h"
#include "build.h"
#include "caches.h"
#include "callInfo.h"
#include "CForLoop.h"
#include "expr.h"
#include "ForLoop.h"
#include "iterator.h"
#include "ParamForLoop.h"
#include "passes.h"
#include "resolveIntents.h"
#include "scopeResolve.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "WhileStmt.h"
#include "../ifa/prim_data.h"
#include "view.h"

#include <inttypes.h>
#include <map>
#include <sstream>
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

/** Contextual info used by the disambiguation process.
 *
 * This class wraps information that is used by multiple functions during the
 * function disambiguation process.
 */
class DisambiguationContext {
public:
  /// The actual arguments from the call site.
  Vec<Symbol*>* actuals;
  /// The scope in which the call is made.
  Expr* scope;
  /// Whether or not to print out tracing information.
  bool explain;
  /// Indexes used when printing out tracing information.
  int i, j;

  /** A simple constructor that initializes all of the values except i and j.
   *
   * \param actuals The actual arguments from the call site.
   * \param scope   A block representing the scope the call was made in.
   * \param explain Whether or not a trace of this disambiguation process should
   *                be printed for the developer.
   */
  DisambiguationContext(Vec<Symbol*>* actuals, Expr* scope, bool explain) :
    actuals(actuals), scope(scope), explain(explain), i(-1), j(-1) {}

  /** A helper function used to set the i and j members.
   *
   * \param i The index of the left-hand side of the comparison.
   * \param j The index of the right-hand side of the comparison.
   *
   * \return A constant reference to this disambiguation context.
   */
  const DisambiguationContext& forPair(int newI, int newJ) {
    this->i = newI;
    this->j = newJ;

    return *this;
  }
};

/** A wrapper for candidates for function call resolution.
 *
 * If a best candidate was found than the function member will point to it.
 */
class ResolutionCandidate {
public:
  /// A pointer to the best candidate function.
  FnSymbol* fn;

  /** The actual arguments for the candidate, aligned so that they have the same
   *  index as their corresponding formal argument in alignedFormals.
   */
  Vec<Symbol*> alignedActuals;

  /** The formal arguments for the candidate, aligned so that they have the same
   *  index as their corresponding actual argument in alignedActuals.
   */
  Vec<ArgSymbol*> alignedFormals;

  /// A symbol map for substitutions that were made during the copying process.
  SymbolMap substitutions;

  /** The main constructor.
   *
   * \param fn A function that is a candidate for the resolution process.
   */
  ResolutionCandidate(FnSymbol* function) : fn(function) {}

  /** Compute the alignment of actual and formal arguments for the wrapped
   *  function and the current call site.
   *
   * \param info The CallInfo object corresponding to the call site.
   *
   * \return If a valid alignment was found.
   */
  bool computeAlignment(CallInfo& info);

  /// Compute substitutions for wrapped function that is generic.
  void computeSubstitutions();
};

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

//#
//# Static Variables
//#
static int explainCallLine;
static ModuleSymbol* explainCallModule;

static Vec<CallExpr*> inits;
static Vec<FnSymbol*> resolvedFormals;
Vec<CallExpr*> callStack;

static Vec<CondStmt*> tryStack;
static bool tryFailure = false;

static Map<Type*,Type*> runtimeTypeMap; // map static types to runtime types
                                        // e.g. array and domain runtime types
static Map<Type*,FnSymbol*> valueToRuntimeTypeMap; // convertValueToRuntimeType
static Map<Type*,FnSymbol*> runtimeTypeToValueMap; // convertRuntimeTypeToValue

static std::map<std::string, std::pair<AggregateType*, FnSymbol*> > functionTypeMap; // lookup table/cache for function types and their representative parents
static std::map<FnSymbol*, FnSymbol*> functionCaptureMap; //loopup table/cache for function captures

// map of compiler warnings that may need to be reissued for repeated
// calls; the inner compiler warning map is from the compilerWarning
// function; the outer compiler warning map is from the function
// containing the compilerWarning function
// to do: this needs to be a map from functions to multiple strings in
//        order to support multiple compiler warnings are allowed to
//        be in a single function
static Map<FnSymbol*,const char*> innerCompilerWarningMap;
static Map<FnSymbol*,const char*> outerCompilerWarningMap;

Map<Type*,FnSymbol*> autoCopyMap; // type to chpl__autoCopy function
Map<Type*,FnSymbol*> autoDestroyMap; // type to chpl__autoDestroy function

Map<FnSymbol*,FnSymbol*> iteratorLeaderMap; // iterator->leader map for promotion
Map<FnSymbol*,FnSymbol*> iteratorFollowerMap; // iterator->leader map for promotion

//#
//# Static Function Declarations
//#
static bool hasRefField(Type *type);
static bool typeHasRefField(Type *type);
static FnSymbol* resolveUninsertedCall(Type* type, CallExpr* call);
static void makeRefType(Type* type);
static void resolveAutoCopy(Type* type);
static void resolveAutoDestroy(Type* type);
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
                             Vec<Symbol*>& alignedActuals,
                             Vec<ArgSymbol*>& alignedFormals,
                             CallInfo& info);
static Type*
getInstantiationType(Type* actualType, Type* formalType);
static void
computeGenericSubs(SymbolMap &subs,
                   FnSymbol* fn,
                   Vec<Symbol*>& alignedActuals);
static FnSymbol*
expandVarArgs(FnSymbol* origFn, int numActuals);

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
static bool explainCallMatch(CallExpr* call);
static CallExpr* userCall(CallExpr* call);
static void
printResolutionErrorAmbiguous(
                     Vec<FnSymbol*>& candidateFns,
                     CallInfo* info);
static void
printResolutionErrorUnresolved(
                     Vec<FnSymbol*>& visibleFns,
                     CallInfo* info);
static void issueCompilerError(CallExpr* call);
static void reissueCompilerWarning(const char* str, int offset);
BlockStmt* getVisibilityBlock(Expr* expr);
static void buildVisibleFunctionMap();
static BlockStmt*
getVisibleFunctions(BlockStmt* block,
                    const char* name,
                    Vec<FnSymbol*>& visibleFns,
                    Vec<BlockStmt*>& visited);
static Expr* resolve_type_expr(Expr* expr);
static void makeNoop(CallExpr* call);
static void resolveDefaultGenericType(CallExpr* call);

static void
gatherCandidates(Vec<ResolutionCandidate*>& candidates,
                 Vec<FnSymbol*>& visibleFns,
                 CallInfo& info);

static void resolveNormalCall(CallExpr* call);
static void lvalueCheck(CallExpr* call);
static void resolveTupleAndExpand(CallExpr* call);
static void resolveTupleExpand(CallExpr* call);
static void resolveSetMember(CallExpr* call);
static void resolveMove(CallExpr* call);
static void resolveNew(CallExpr* call);
static bool formalRequiresTemp(ArgSymbol* formal);
static void insertFormalTemps(FnSymbol* fn);
static void addLocalCopiesAndWritebacks(FnSymbol* fn, SymbolMap& formals2vars);

static Expr* dropUnnecessaryCast(CallExpr* call);
static AggregateType* createAndInsertFunParentClass(CallExpr *call, const char *name);
static FnSymbol* createAndInsertFunParentMethod(CallExpr *call, AggregateType *parent, AList &arg_list, bool isFormal, Type *retType);
static std::string buildParentName(AList &arg_list, bool isFormal, Type *retType);
static AggregateType* createOrFindFunTypeFromAnnotation(AList &arg_list, CallExpr *call);
static Expr* createFunctionAsValue(CallExpr *call);
static bool
isOuterVar(Symbol* sym, FnSymbol* fn, Symbol* parent = NULL);
static bool
usesOuterVars(FnSymbol* fn, Vec<FnSymbol*> &seen);
static Expr* preFold(Expr* expr);
static void foldEnumOp(int op, EnumSymbol *e1, EnumSymbol *e2, Immediate *imm);
static bool isSubType(Type* sub, Type* super);
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
replaceSetterArgWithTrue(BaseAST* ast, FnSymbol* fn);
static void
replaceSetterArgWithFalse(BaseAST* ast, FnSymbol* fn, Symbol* ret);
static void
insertCasts(BaseAST* ast, FnSymbol* fn, Vec<CallExpr*>& casts);
static void buildValueFunction(FnSymbol* fn);
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
static void resolveTypedefedArgTypes(FnSymbol* fn);
static void computeStandardModuleSet();
static void unmarkDefaultedGenerics();
static void resolveUses(ModuleSymbol* mod);
static void resolveExports();
static void resolveEnumTypes();
static void resolveDynamicDispatches();
static void insertRuntimeTypeTemps();
static void resolveAutoCopies();
static void resolveRecordInitializers();
static void insertDynamicDispatchCalls();
static Type* buildRuntimeTypeInfo(FnSymbol* fn);
static void insertReturnTemps();
static void initializeClass(Expr* stmt, Symbol* sym);
static void handleRuntimeTypes();
static void pruneResolvedTree();
static void removeCompilerWarnings();
static void removeUnusedFunctions();
static void removeUnusedTypes();
static void buildRuntimeTypeInitFns();
static void buildRuntimeTypeInitFn(FnSymbol* fn, Type* runtimeType);
static void removeUnusedGlobals();
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

bool ResolutionCandidate::computeAlignment(CallInfo& info) {
  if (alignedActuals.n != 0) alignedActuals.clear();
  if (alignedFormals.n != 0) alignedFormals.clear();

  return computeActualFormalAlignment(fn, alignedActuals, alignedFormals, info);
}

void ResolutionCandidate::computeSubstitutions() {
  if (substitutions.n != 0) substitutions.clear();
  computeGenericSubs(substitutions, fn, alignedActuals);
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

//
// build reference type
//
static FnSymbol*
resolveUninsertedCall(Type* type, CallExpr* call) {
  if (type->defaultInitializer) {
    if (type->defaultInitializer->instantiationPoint)
      type->defaultInitializer->instantiationPoint->insertAtHead(call);
    else
      type->symbol->defPoint->insertBefore(call);
  } else {
    chpl_gen_main->insertAtHead(call);
  }

  resolveCall(call);
  call->remove();

  return call->isResolved();
}


// Fills in the refType field of a type
// with the type's corresponding reference type.
static void makeRefType(Type* type) {
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


static void
resolveAutoCopy(Type* type) {
  SET_LINENO(type->symbol);
  Symbol* tmp = newTemp(type);
  chpl_gen_main->insertAtHead(new DefExpr(tmp));
  CallExpr* call = new CallExpr("chpl__autoCopy", tmp);
  FnSymbol* fn = resolveUninsertedCall(type, call);
  resolveFns(fn);
  autoCopyMap.put(type, fn);
  tmp->defPoint->remove();
}


static void
resolveAutoDestroy(Type* type) {
  SET_LINENO(type->symbol);
  Symbol* tmp = newTemp(type);
  chpl_gen_main->insertAtHead(new DefExpr(tmp));
  CallExpr* call = new CallExpr("chpl__autoDestroy", tmp);
  FnSymbol* fn = resolveUninsertedCall(type, call);
  resolveFns(fn);
  autoDestroyMap.put(type, fn);
  tmp->defPoint->remove();
}


FnSymbol* getAutoCopy(Type *t) {
  return autoCopyMap.get(t);
}


FnSymbol* getAutoDestroy(Type* t) {
  return autoDestroyMap.get(t);
}


const char* toString(Type* type) {
  return type->getValType()->symbol->name;
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
        info->actuals.v[i]->type->defaultInitializer->hasFlag(FLAG_PROMOTION_WRAPPER))
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
    INT_ASSERT(!strncmp("_construct_", fn->name, 11));
    str = astr(fn->name+11);
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
        str = astr(str, arg->name, ": ", sym->var->name);
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
  add_root_type(ii->iclass);    // Add super : dtObject.
  fn->defPoint->insertBefore(new DefExpr(cts));

  ii->irecord = new AggregateType(AGGREGATE_RECORD);
  TypeSymbol* rts = new TypeSymbol(astr("_ir_", className), ii->irecord);
  rts->addFlag(FLAG_ITERATOR_RECORD);
  if (fn->retTag == RET_REF)
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

  // The original iterator function is stashed in the defaultInitializer field
  // of the iterator record type.  Since we are only creating shell functions
  // here, we still need a way to obtain the original iterator function, so we
  // can fill in the bodies of the above 9 methods in the lowerIterators pass.
  ii->irecord->defaultInitializer = fn;
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
  CallExpr* icAllocCall = callChplHereAlloc(ret->typeInfo()->symbol);
  ii->getIterator->insertAtTail(new CallExpr(PRIM_MOVE, ret, icAllocCall));
  ii->getIterator->insertAtTail(new CallExpr(PRIM_SETCID, ret));
  ii->getIterator->insertAtTail(new CallExpr(PRIM_RETURN, ret));
  fn->defPoint->insertBefore(new DefExpr(ii->getIterator));
  // The _getIterator function is stashed in the defaultInitializer field of
  // the iterator class type.  This makes it easy to obtain the iterator given
  // just a symbol of the iterator class type.  This may include _getIterator
  // and _getIteratorZip functions in the module code.
  ii->iclass->defaultInitializer = ii->getIterator;
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
  VarSymbol* var = toVarSymbol(last->var);
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
      VarSymbol* var = toVarSymbol(actual->var);
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


//
// For some types, e.g. _domain/_array records, implementing
// Chapel's ref/out/... intents can be done simply by passing the
// value itself, rather than address-of. This function flags such cases
// by returning false, meaning "not OK to convert".
//
static bool
okToConvertFormalToRefType(Type* type) {
  if (isRecordWrappedType(type))
    // no, don't
    return false;

  // otherwise, proceed with the original plan
  return true;
}


static void
resolveSpecifiedReturnType(FnSymbol* fn) {
  resolveBlockStmt(fn->retExprType);
  fn->retType = fn->retExprType->body.tail->typeInfo();
  if (fn->retType != dtUnknown) {
    if (fn->retTag == RET_REF) {
      makeRefType(fn->retType);
      fn->retType = fn->retType->refType;
    }
    fn->retExprType->remove();
    if (fn->isIterator() && !fn->iteratorInfo) {
      protoIteratorClass(fn);
    }
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

      if (formal->intent == INTENT_INOUT ||
          formal->intent == INTENT_OUT ||
          formal->intent == INTENT_REF ||
          formal->intent == INTENT_CONST_REF ||
          convertAtomicFormalTypeToRef(formal, fn) ||
          formal->hasFlag(FLAG_WRAP_WRITTEN_FORMAL) ||
          (formal == fn->_this &&
           (isUnion(formal->type) ||
            isRecord(formal->type)))) {
        if (okToConvertFormalToRefType(formal->type)) {
          makeRefType(formal->type);
          formal->type = formal->type->refType;
          // The type of the formal is its own ref type!
        }
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
  if (SymExpr* se = toSymExpr(actual))
    if (se->var->hasFlag(FLAG_EXPR_TEMP) ||
        ((se->var->hasFlag(FLAG_REF_TO_CONST) ||
          se->var->isConstant()) && !formal->hasFlag(FLAG_ARG_THIS)) ||
        se->var->isParameter())
      if (okToConvertFormalToRefType(formal->type) ||
          // If the user says 'const', it means 'const'.
          // Honor FLAG_CONST if it is a coerce temp, too.
          (se->var->hasFlag(FLAG_CONST) &&
           (!se->var->hasFlag(FLAG_TEMP) || se->var->hasFlag(FLAG_COERCE_TEMP))
         ))
        return false;
  // Perhaps more checks are needed.
  return true;
}


// Is this a legal actual argument for a 'const ref' formal?
// At present, params cannot be passed to 'const ref'.
static bool
isLegalConstRefActualArg(ArgSymbol* formal, Expr* actual) {
  if (SymExpr* se = toSymExpr(actual))
    if (se->var->isParameter())
      if (okToConvertFormalToRefType(formal->type))
        return false;
  // Perhaps more checks are needed.
  return true;
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
  return false;
}


//
// returns true iff dispatching the actualType to the formalType
// results in a coercion.
//
bool
canCoerce(Type* actualType, Symbol* actualSym, Type* formalType, FnSymbol* fn, bool* promotes) {
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
  if (isSyncType(actualType)) {
    Type* baseType = actualType->getField("base_type")->type;
    return canDispatch(baseType, NULL, formalType, fn, promotes);
  }
  if (actualType->symbol->hasFlag(FLAG_REF))
    return canDispatch(actualType->getValType(), NULL, formalType, fn, promotes);
  if (// isLcnSymbol(actualSym) && // What does this exclude?
      actualType == dtStringC && formalType == dtString)
    return true;
  if (formalType == dtStringC && actualType == dtStringCopy)
    return true;
  if (formalType == dtString && actualType == dtStringCopy)
    return true;
  return false;
}

// Returns true iff the actualType can dispatch to the formalType.
// The function symbol is used to avoid scalar promotion on =.
// param is set if the actual is a parameter (compile-time constant).
bool
canDispatch(Type* actualType, Symbol* actualSym, Type* formalType, FnSymbol* fn, bool* promotes, bool paramCoerce) {
  if (promotes)
    *promotes = false;
  if (actualType == formalType)
    return true;
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
  if (actualType->refType == formalType)
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
                             Vec<Symbol*>& alignedActuals,
                             Vec<ArgSymbol*>& alignedFormals,
                             CallInfo& info) {
  alignedActuals.fill(fn->numFormals());
  alignedFormals.fill(info.actuals.n);

  // Match named actuals against formal names in the function signature.
  // Record successful matches.
  for (int i = 0; i < alignedFormals.n; i++) {
    if (info.actualNames.v[i]) {
      bool match = false;
      int j = 0;
      for_formals(formal, fn) {
        if (!strcmp(info.actualNames.v[i], formal->name)) {
          match = true;
          alignedFormals.v[i] = formal;
          alignedActuals.v[j] = info.actuals.v[i];
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
  for (int i = 0; i < alignedFormals.n; i++) {
    if (!info.actualNames.v[i]) {
      bool match = false;
      while (formal) {
        if (formal->variableExpr)
          return (fn->hasFlag(FLAG_GENERIC)) ? true : false;
        if (!alignedActuals.v[j]) {
          match = true;
          alignedFormals.v[i] = formal;
          alignedActuals.v[j] = info.actuals.v[i];
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
    if (!alignedActuals.v[j] && !formal->defaultExpr)
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
getInstantiationType(Type* actualType, Type* formalType) {
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


static void
computeGenericSubs(SymbolMap &subs,
                   FnSymbol* fn,
                   Vec<Symbol*>& alignedActuals) {
  int i = 0;
  for_formals(formal, fn) {
    if (formal->intent == INTENT_PARAM) {
      if (alignedActuals.v[i] && alignedActuals.v[i]->isParameter()) {
        if (!formal->type->symbol->hasFlag(FLAG_GENERIC) ||
            canInstantiate(alignedActuals.v[i]->type, formal->type))
          subs.put(formal, alignedActuals.v[i]);
      } else if (!alignedActuals.v[i] && formal->defaultExpr) {

        // break because default expression may reference generic
        // arguments earlier in formal list; make those substitutions
        // first (test/classes/bradc/paramInClass/weirdParamInit4)
        if (subs.n)
          break;

        resolveBlockStmt(formal->defaultExpr);
        SymExpr* se = toSymExpr(formal->defaultExpr->body.tail);
        if (se && se->var->isParameter() &&
            (!formal->type->symbol->hasFlag(FLAG_GENERIC) || canInstantiate(se->var->type, formal->type)))
          subs.put(formal, se->var);
        else
          INT_FATAL(fn, "unable to handle default parameter");
      }
    } else if (formal->type->symbol->hasFlag(FLAG_GENERIC)) {

      //
      // check for field with specified generic type
      //
      if (!formal->hasFlag(FLAG_TYPE_VARIABLE) && formal->type != dtAny &&
          strcmp(formal->name, "outer") && strcmp(formal->name, "meme") &&
          (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) || fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)))
        USR_FATAL(formal, "invalid generic type specification on class field");

      if (alignedActuals.v[i]) {
        if (Type* type = getInstantiationType(alignedActuals.v[i]->type, formal->type)) {
          // String literal actuals aligned with non-param generic
          // formals of type dtAny will result in an instantiation of
          // a dtString formal.  This is in line with variable
          // declarations with non-typed initializing expressions and
          // non-param formals with string literal default expressions
          // (see fix_def_expr() and hack_resolve_types() in
          // normalize.cpp). This conversion is not performed for extern
          // functions.
          if ((!fn->hasFlag(FLAG_EXTERN)) && (formal->type == dtAny) &&
              (!formal->hasFlag(FLAG_PARAM)) && (type == dtStringC) &&
              (alignedActuals.v[i]->type == dtStringC) &&
              (alignedActuals.v[i]->isImmediate()))
            subs.put(formal, dtString->symbol);
          else
            subs.put(formal, type->symbol);
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


/** Common code for multiple paths through expandVarArgs.
 *
 * This code handles the case where the number of varargs are known at compile
 * time.  It inserts the necessary code to copy the values into and out of the
 * varargs tuple.
 */
static void
handleSymExprInExpandVarArgs(FnSymbol* workingFn, ArgSymbol* formal, SymExpr* sym) {
  workingFn->addFlag(FLAG_EXPANDED_VARARGS);

  // Handle specified number of variable arguments.
  if (VarSymbol* n_var = toVarSymbol(sym->var)) {
    if (n_var->type == dtInt[INT_SIZE_DEFAULT] && n_var->immediate) {
      int n = n_var->immediate->int_value();
      CallExpr* tupleCall = new CallExpr((formal->hasFlag(FLAG_TYPE_VARIABLE)) ?
                                         "_type_construct__tuple" : "_construct__tuple");
      for (int i = 0; i < n; i++) {
        DefExpr* new_arg_def = formal->defPoint->copy();
        ArgSymbol* new_formal = toArgSymbol(new_arg_def->sym);
        new_formal->variableExpr = NULL;
        tupleCall->insertAtTail(new SymExpr(new_formal));
        new_formal->name = astr("_e", istr(i), "_", formal->name);
        new_formal->cname = astr("_e", istr(i), "_", formal->cname);
        formal->defPoint->insertBefore(new_arg_def);
      }

      VarSymbol* var = new VarSymbol(formal->name);

      // Replace mappings to the old formal with mappings to the new variable.
      if (workingFn->hasFlag(FLAG_PARTIAL_COPY)) {
        for (int index = workingFn->partialCopyMap.n; --index >= 0;) {
          SymbolMapElem& mapElem = workingFn->partialCopyMap.v[index];

          if (mapElem.value == formal) {
            mapElem.value = var;
            break;
          }
        }
      }

      if (formal->hasFlag(FLAG_TYPE_VARIABLE)) {
        var->addFlag(FLAG_TYPE_VARIABLE);
      }

      if (formal->intent == INTENT_OUT || formal->intent == INTENT_INOUT) {
        int i = 1;
        for_actuals(actual, tupleCall) {
          VarSymbol* tmp = newTemp("_varargs_tmp_");
          workingFn->insertBeforeReturnAfterLabel(new DefExpr(tmp));
          workingFn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_MOVE, tmp, new CallExpr(var, new_IntSymbol(i))));
          workingFn->insertBeforeReturnAfterLabel(new CallExpr("=", actual->copy(), tmp));
          i++;
        }
      }

      tupleCall->insertAtHead(new_IntSymbol(n));
      workingFn->insertAtHead(new CallExpr(PRIM_MOVE, var, tupleCall));
      workingFn->insertAtHead(new DefExpr(var));
      formal->defPoint->remove();

      if (workingFn->hasFlag(FLAG_PARTIAL_COPY)) {
        // If this is a partial copy, store the mapping for substitution later.
        workingFn->partialCopyMap.put(formal, var);
      } else {
        // Otherwise, do the substitution now.
        subSymbol(workingFn->body, formal, var);
      }

      if (workingFn->where) {
        VarSymbol* var = new VarSymbol(formal->name);

        if (formal->hasFlag(FLAG_TYPE_VARIABLE)) {
          var->addFlag(FLAG_TYPE_VARIABLE);
        }

        workingFn->where->insertAtHead(new CallExpr(PRIM_MOVE, var, tupleCall->copy()));
        workingFn->where->insertAtHead(new DefExpr(var));
        subSymbol(workingFn->where, formal, var);
      }
    }
  }
}


static FnSymbol*
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
    // The addition of the arg->type test in the folloiwng conditional is a
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

      Symbol*  newSym     = substitutions.get(def->sym);
      SymExpr* newSymExpr = new SymExpr(new_IntSymbol(numCopies));
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
    CallExpr* typeConstructorCall = new CallExpr(astr("_type", fn->name));
    for_formals(formal, fn) {
      if (strcmp(formal->name, "meme")) {
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

  /*
   * A derived generic type will use the type of its parent, and expects this to
   * be instantiated before it is.
   */
  resolveFormals(currCandidate->fn);

  int coindex = -1;
  for_formals(formal, currCandidate->fn) {
    if (Symbol* actual = currCandidate->alignedActuals.v[++coindex]) {
      if (actual->hasFlag(FLAG_TYPE_VARIABLE) != formal->hasFlag(FLAG_TYPE_VARIABLE)) {
        return;
      }

      if (!canDispatch(actual->type, actual, formal->type, currCandidate->fn, NULL, formal->hasFlag(FLAG_INSTANTIATED_PARAM))) {
        return;
      }
    }
  }

  candidates.add(currCandidate);
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

  /*
   * Early rejection of generic functions.
   */
  int coindex = 0;
  for_formals(formal, currCandidate->fn) {
    if (formal->type != dtUnknown) {
      if (Symbol* actual = currCandidate->alignedActuals.v[coindex]) {
        if (actual->hasFlag(FLAG_TYPE_VARIABLE) != formal->hasFlag(FLAG_TYPE_VARIABLE)) {
          return;
        }

        if (formal->type->symbol->hasFlag(FLAG_GENERIC)) {
          Type* vt = actual->getValType();
          Type* st = actual->type->scalarPromotionType;
          Type* svt = (vt) ? vt->scalarPromotionType : NULL;
          if (!canInstantiate(actual->type, formal->type) &&
              (!vt  || !canInstantiate(vt, formal->type)) &&
              (!st  || !canInstantiate(st, formal->type)) &&
              (!svt || !canInstantiate(svt, formal->type))) {

            return;

          }
        } else {
          if (!canDispatch(actual->type, actual, formal->type, currCandidate->fn, NULL, formal->hasFlag(FLAG_INSTANTIATED_PARAM))) {
            return;
          }
        }
      }
    }
    ++coindex;
  }

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
      SymExpr* se = toSymExpr(expr);
      INT_ASSERT(se);
      ModuleSymbol* mod = toModuleSymbol(se->var);
      INT_ASSERT(mod);
      if (!visited.set_in(mod->block))
        moreVisible &= isMoreVisibleInternal(mod->block, fn1, fn2, visited);
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


static bool considerParamMatches(Type* actualtype,
                                 Type* arg1type, Type* arg2type) {
  /* BLC: Seems weird to have to add this; could just add it in the enum
     case if enums have to be special-cased here.  Otherwise, how are the
     int cases handled later...? */
  if (actualtype->symbol->hasFlag(FLAG_REF)) {
    actualtype = actualtype->getValType();
  }
  if (actualtype == arg1type && actualtype != arg2type) {
    return true;
  }
  // If we don't have an exact match in the previous line, let's see if
  // we have a bool(w1) passed to bool(w2) or non-bool case;  This is
  // based on the enum case developed in r20208
  if (is_bool_type(actualtype) && is_bool_type(arg1type) && !is_bool_type(arg2type)) {
    return true;
  }
  // Otherwise, have bool cast to default-sized integer over a smaller size
  if (is_bool_type(actualtype) && actualtype != arg1type && actualtype != arg2type) {
    return considerParamMatches(dtInt[INT_SIZE_DEFAULT], arg1type, arg2type);
  }
  if (is_enum_type(actualtype) && actualtype != arg1type && actualtype != arg2type) {
    return considerParamMatches(dtInt[INT_SIZE_DEFAULT], arg1type, arg2type);
  }
  if (isSyncType(actualtype) && actualtype != arg1type && actualtype != arg2type) {
    return considerParamMatches(actualtype->getField("base_type")->type,
                                arg1type, arg2type);
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
static bool isBetterMatch(ResolutionCandidate* candidate1,
                          ResolutionCandidate* candidate2,
                          const DisambiguationContext& DC) {

  DisambiguationState DS;

  for (int k = 0; k < candidate1->alignedFormals.n; ++k) {
    Symbol* actual = DC.actuals->v[k];
    ArgSymbol* formal1 = candidate1->alignedFormals.v[k];
    ArgSymbol* formal2 = candidate2->alignedFormals.v[k];

    TRACE_DISAMBIGUATE_BY_MATCH("\nLooking at argument %d\n", k);

    testArgMapping(candidate1->fn, formal1, candidate2->fn, formal2, actual, DC, DS);
  }

  if (!DS.fn1Promotes && DS.fn2Promotes) {
    TRACE_DISAMBIGUATE_BY_MATCH("\nP: Fn %d does not require argument promotion; Fn %d does\n", DC.i, DC.j);
    DS.printSummary("P", DC);
    return true;
  }

  if (!(DS.fn1MoreSpecific || DS.fn2MoreSpecific)) {
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
static ResolutionCandidate*
disambiguateByMatch(Vec<ResolutionCandidate*>& candidates, DisambiguationContext DC) {

  // If index i is set then we can skip testing function F_i because we already
  // know it can not be the best match.
  std::vector<bool> notBest(candidates.n, false);

  for (int i = 0; i < candidates.n; ++i) {

    TRACE_DISAMBIGUATE_BY_MATCH("##########################\n");
    TRACE_DISAMBIGUATE_BY_MATCH("# Considering function %d #\n", i);
    TRACE_DISAMBIGUATE_BY_MATCH("##########################\n\n");

    ResolutionCandidate* candidate1 = candidates.v[i];
    bool best = true; // is fn1 the best candidate?

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


static bool
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


static void
printResolutionErrorAmbiguous(
                     Vec<FnSymbol*>& candidates,
                     CallInfo* info) {
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

static void
printResolutionErrorUnresolved(
                     Vec<FnSymbol*>& visibleFns,
                     CallInfo* info) {
  CallExpr* call = userCall(info->call);
  if (!strcmp("_cast", info->name)) {
    if (!info->actuals.head()->hasFlag(FLAG_TYPE_VARIABLE)) {
      USR_FATAL(call, "illegal cast to non-type",
                toString(info->actuals.v[1]->type),
                toString(info->actuals.v[0]->type));
    } else {
      USR_FATAL(call, "illegal cast from %s to %s",
                toString(info->actuals.v[1]->type),
                toString(info->actuals.v[0]->type));
    }
  } else if (!strcmp("free", info->name)) {
    if (info->actuals.n > 0 &&
        isRecord(info->actuals.v[2]->type))
      USR_FATAL(call, "delete not allowed on records");
  } else if (!strcmp("these", info->name)) {
    if (info->actuals.n == 2 &&
        info->actuals.v[0]->type == dtMethodToken)
    USR_FATAL(call, "cannot iterate over values of type %s",
              toString(info->actuals.v[1]->type));
  } else if (!strcmp("_type_construct__tuple", info->name)) {
    if (info->call->argList.length == 0)
      USR_FATAL(call, "tuple size must be specified");
    SymExpr* sym = toSymExpr(info->call->get(1));
    if (!sym || !sym->var->isParameter()) {
      USR_FATAL(call, "tuple size must be static");
    } else {
      USR_FATAL(call, "invalid tuple");
    }
  } else if (!strcmp("=", info->name)) {
    if (info->actuals.v[0] && !info->actuals.v[0]->hasFlag(FLAG_TYPE_VARIABLE) &&
        info->actuals.v[1] && info->actuals.v[1]->hasFlag(FLAG_TYPE_VARIABLE)) {
      USR_FATAL(call, "illegal assignment of type to value");
    } else if (info->actuals.v[0] && info->actuals.v[0]->hasFlag(FLAG_TYPE_VARIABLE) &&
               info->actuals.v[1] && !info->actuals.v[1]->hasFlag(FLAG_TYPE_VARIABLE)) {
      USR_FATAL(call, "illegal assignment of value to type");
    } else if (info->actuals.v[1]->type == dtNil) {
      USR_FATAL(call, "type mismatch in assignment from nil to %s",
                toString(info->actuals.v[0]->type));
    } else {
      USR_FATAL(call, "type mismatch in assignment from %s to %s",
                toString(info->actuals.v[1]->type),
                toString(info->actuals.v[0]->type));
    }
  } else if (!strcmp("this", info->name)) {
    Type* type = info->actuals.v[1]->getValType();
    if (type->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
      USR_FATAL(call, "illegal access of iterator or promoted expression");
    } else if (type->symbol->hasFlag(FLAG_FUNCTION_CLASS)) {
      USR_FATAL(call, "illegal access of first class function");
    } else {
      USR_FATAL(call, "unresolved access of '%s' by '%s'",
                toString(info->actuals.v[1]->type),
                toString(info));
    }
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
    USR_STOP();
  }
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
  for_formals(arg, fn) {
    if (foundDepthVal && arg->defPoint == fn->formals.tail)
      continue;
    VarSymbol* var = toVarSymbol(paramMap.get(arg));
    INT_ASSERT(var && var->immediate && var->immediate->const_kind == CONST_KIND_STRING);
    str = astr(str, var->immediate->v_string);
  }
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
  return sym->var == gTryToken;
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
    INT_FATAL(expr, "Expresion has no visibility block.");
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

static BlockStmt*
getVisibleFunctions(BlockStmt* block,
                    const char* name,
                    Vec<FnSymbol*>& visibleFns,
                    Vec<BlockStmt*>& visited) {
  //
  // all functions in standard modules are stored in a single block
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
      visibleFns.append(*fns);
    }
  }

  if (block->modUses) {
    for_actuals(expr, block->modUses) {
      SymExpr* se = toSymExpr(expr);
      INT_ASSERT(se);
      ModuleSymbol* mod = toModuleSymbol(se->var);
      INT_ASSERT(mod);
      canSkipThisBlock = false; // cannot skip if this block uses modules
      getVisibleFunctions(mod->block, name, visibleFns, visited);
    }
  }

  //
  // visibilityBlockCache contains blocks that can be skipped
  //
  if (BlockStmt* next = visibilityBlockCache.get(block)) {
    getVisibleFunctions(next, name, visibleFns, visited);
    return (canSkipThisBlock) ? next : block;
  }

  if (block != rootModule->block) {
    BlockStmt* next = getVisibilityBlock(block);
    BlockStmt* cache = getVisibleFunctions(next, name, visibleFns, visited);
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
    INT_ASSERT(arg1->var == def->sym);
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
  if (varActual->defPoint->parentExpr == parent) {
    // Index variable of the coforall loop? Do not capture it!
    if (fVerify) {
      // This is what currently happens.
      CallExpr* move = toCallExpr(varActual->defPoint->next);
      INT_ASSERT(move);
      INT_ASSERT(move->isPrimitive(PRIM_MOVE));
      SymExpr* src = toSymExpr(move->get(2));
      INT_ASSERT(src);
      INT_ASSERT(!strcmp(src->var->name, "_indexOfInterest"));
    }
    // do nothing
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
    FnSymbol* autoCopy = getAutoCopy(formal->type);
    if (autoCopy)
      marker->insertBefore("'move'(%S,%S(%S))", captemp, autoCopy, varActual);
    else if (isReferenceType(varActual->type) &&
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
    Symbol* varActual = symexpActual->var;

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
  return fn->hasFlag(FLAG_CONSTRUCTOR) || !strcmp(fn->name, "initialize");
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
// or initialize(), with the constructor's or intialize's 'this'
// as the receiver actual.
static bool isInvokedFromConstructorLikeFunction(int stackIdx) {
  if (stackIdx > 0) {
    CallExpr* call2 = callStack.v[stackIdx - 1];
    if (FnSymbol* parent2 = toFnSymbol(call2->parentSymbol))
     if (isConstructorLikeFunction(parent2))
      if (call2->numActuals() >= 2)
        if (SymExpr* thisArg2 = toSymExpr(call2->get(2)))
          if (thisArg2->var->hasFlag(FLAG_ARG_THIS))
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
    if (se->var->hasFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS))
      if (isInvokedFromConstructorLikeFunction(stackIdx)) {
          // Yes, this is the case we are looking for.
          se->var->removeFlag(FLAG_REF_TO_CONST);
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
  return baseExpr->var;
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
             resolvedFn->hasFlag(FLAG_AUTO_COPY_FN) &&
             !dest->hasFlag(FLAG_CONST))
  {
    // We are creating a var alias - ensure aliasee is not const either.
    SymExpr* aliaseeSE = toSymExpr(call->get(1));
    INT_ASSERT(aliaseeSE);
    if (aliaseeSE->var->isConstant() ||
        aliaseeSE->var->hasFlag(FLAG_CONST))
      USR_FATAL_CONT(call, "creating a non-const alias '%s' of a const array or domain", dest->name);
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
static void checkForStoringIntoTuple(CallExpr* call, FnSymbol* resolvedFn)
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
    if (isSyncType(formal->type)) {
      const char* name = "";
      if (SymExpr* aSE = toSymExpr(actual))
        if (!aSE->var->hasFlag(FLAG_TEMP))
          name = aSE->var->name;
      USR_FATAL_CONT(actual, "storing a sync or single variable %s in a tuple is not currently implemented - apply readFE() or readFF()", name);
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
// special case cast of class w/ type variables that is not generic
//   i.e. type variables are type definitions (have default types)
//
static void
resolveDefaultGenericType(CallExpr* call) {
  SET_LINENO(call);
  for_actuals(actual, call) {
    if (NamedExpr* ne = toNamedExpr(actual))
      actual = ne->actual;
    if (SymExpr* te = toSymExpr(actual)) {
      if (TypeSymbol* ts = toTypeSymbol(te->var)) {
        if (AggregateType* ct = toAggregateType(ts->type)) {
          if (ct->symbol->hasFlag(FLAG_GENERIC)) {
            CallExpr* cc = new CallExpr(ct->defaultTypeConstructor->name);
            te->replace(cc);
            resolveCall(cc);
            cc->replace(new SymExpr(cc->typeInfo()->symbol));
          }
        }
      }
      if (VarSymbol* vs = toVarSymbol(te->var)) {
        // Fix for complicated extern vars like
        // extern var x: c_ptr(c_int);
        if( vs->hasFlag(FLAG_EXTERN) && vs->hasFlag(FLAG_TYPE_VARIABLE) &&
            vs->defPoint && vs->defPoint->init ) {
          if( CallExpr* def = toCallExpr(vs->defPoint->init) ) {
            vs->defPoint->init = resolveExpr(def);
            te->replace(new SymExpr(vs->defPoint->init->typeInfo()->symbol));
          }
        }
      }
    }
  }
}


static void
gatherCandidates(Vec<ResolutionCandidate*>& candidates,
                 Vec<FnSymbol*>& visibleFns,
                 CallInfo& info) {

  // Search user-defined (i.e. non-compiler-generated) functions first.
  forv_Vec(FnSymbol, visibleFn, visibleFns) {
    if (visibleFn->hasFlag(FLAG_COMPILER_GENERATED)) {
      continue;
    }

    if (info.call->methodTag &&
        ! (visibleFn->hasFlag(FLAG_NO_PARENS) ||
           visibleFn->hasFlag(FLAG_TYPE_CONSTRUCTOR))) {
      continue;
    }

    if (fExplainVerbose &&
        ((explainCallLine && explainCallMatch(info.call)) ||
         info.call->id == explainCallID))
    {
      USR_PRINT(visibleFn, "Considering function: %s", toString(visibleFn));
    }

    filterCandidate(candidates, visibleFn, info);
  }

  // Return if we got a successful match with user-defined functions.
  if (candidates.n) {
    return;
  }

  // No.  So search compiler-defined functions.
  forv_Vec(FnSymbol, visibleFn, visibleFns) {
    if (!visibleFn->hasFlag(FLAG_COMPILER_GENERATED)) {
      continue;
    }

    if (info.call->methodTag &&
        ! (visibleFn->hasFlag(FLAG_NO_PARENS) ||
           visibleFn->hasFlag(FLAG_TYPE_CONSTRUCTOR))) {
      continue;
    }

    if (fExplainVerbose &&
        ((explainCallLine && explainCallMatch(info.call)) ||
         info.call->id == explainCallID))
    {
      USR_PRINT(visibleFn, "Considering function: %s", toString(visibleFn));
    }

    filterCandidate(candidates, visibleFn, info);
  }
}


void
resolveCall(CallExpr* call)
{
  if (call->primitive)
  {
    switch (call->primitive->tag)
    {
     default:                       /* do nothing */                    break;
     case PRIM_TUPLE_AND_EXPAND:    resolveTupleAndExpand(call);        break;
     case PRIM_TUPLE_EXPAND:        resolveTupleExpand(call);           break;
     case PRIM_SET_MEMBER:          resolveSetMember(call);             break;
     case PRIM_MOVE:                resolveMove(call);                  break;
     case PRIM_TYPE_INIT:
     case PRIM_INIT:                resolveDefaultGenericType(call);    break;
     case PRIM_NO_INIT:             resolveDefaultGenericType(call);    break;
     case PRIM_NEW:                 resolveNew(call);                   break;
    }
  }
  else
  {
    resolveNormalCall(call);
  }
}


void resolveNormalCall(CallExpr* call) {

  resolveDefaultGenericType(call);

  CallInfo info(call);

  Vec<FnSymbol*> visibleFns; // visible functions

  //
  // update visible function map as necessary
  //
  if (gFnSymbols.n != nVisibleFunctions) {
    buildVisibleFunctionMap();
  }

  if (!call->isResolved()) {
    if (!info.scope) {
      Vec<BlockStmt*> visited;
      getVisibleFunctions(getVisibilityBlock(call), info.name, visibleFns, visited);
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

  Vec<ResolutionCandidate*> candidates;
  gatherCandidates(candidates, visibleFns, info);

  if ((explainCallLine && explainCallMatch(info.call)) ||
      call->id == explainCallID)
  {
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

  Expr* scope = (info.scope) ? info.scope : getVisibilityBlock(call);
  bool explain = fExplainVerbose &&
    ((explainCallLine && explainCallMatch(call)) ||
     info.call->id == explainCallID);
  DisambiguationContext DC(&info.actuals, scope, explain);

  ResolutionCandidate* best = disambiguateByMatch(candidates, DC);

  if (best && best->fn) {
    /*
     * Finish instantiating the body.  This is a noop if the function wasn't
     * partially instantiated.
     */
    instantiateBody(best->fn);

    if (explainCallLine && explainCallMatch(call)) {
      USR_PRINT(best->fn, "best candidate is: %s", toString(best->fn));
    }
  }

  // Future work note: the repeated check to best and best->fn means that we
  // could probably restructure this function to a better form.
  if (call->partialTag && (!best || !best->fn ||
                           !best->fn->hasFlag(FLAG_NO_PARENS))) {
    if (best != NULL) {
      delete best;
      best = NULL;
    }
  } else if (!best) {
    if (tryStack.n) {
      tryFailure = true;
      return;

    } else {
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
  } else {
    best->fn = defaultWrap(best->fn, &best->alignedFormals, &info);
    reorderActuals(best->fn, &best->alignedFormals, &info);
    coerceActuals(best->fn, &info);
    best->fn = promotionWrap(best->fn, &info);
  }

  FnSymbol* resolvedFn = best != NULL ? best->fn : NULL;

  forv_Vec(ResolutionCandidate*, candidate, candidates) {
    delete candidate;
  }

  if (call->partialTag) {
    if (!resolvedFn) {
      return;
    }
    call->partialTag = false;
  }

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

  if (call->parentSymbol) {
    SET_LINENO(call);
    call->baseExpr->replace(new SymExpr(resolvedFn));
  }

  if (resolvedFn->hasFlag(FLAG_MODIFIES_CONST_FIELDS))
    // Not allowed if it is not called directly from a constructor.
    if (!isInConstructorLikeFunction(call) ||
        !getBaseSymForConstCheck(call)->hasFlag(FLAG_ARG_THIS)
        )
      USR_FATAL_CONT(call, "illegal call to %s() - it modifies 'const' fields of 'this', therefore it can be invoked only directly from a constructor on the object being constructed", resolvedFn->name);

  lvalueCheck(call);
  checkForStoringIntoTuple(call, resolvedFn);

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


static void lvalueCheck(CallExpr* call)
{
  // Check to ensure the actual supplied to an OUT, INOUT or REF argument
  // is an lvalue.
  for_formals_actuals(formal, actual, call) {
    bool errorMsg = false;
    switch (formal->intent) {
     case INTENT_BLANK:
     case INTENT_IN:
     case INTENT_CONST:
     case INTENT_CONST_IN:
     case INTENT_PARAM:
     case INTENT_TYPE:
      // not checking them here
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
    }
  }
}


// We do some const-related work upon PRIM_MOVE
static void setConstFlagsAndCheckUponMove(Symbol* lhs, Expr* rhs) {
  // If this assigns into a loop index variable from a non-var iterator,
  // mark the variable constant.
  if (SymExpr* rhsSE = toSymExpr(rhs)) {
    // If RHS is this special variable...
    if (rhsSE->var->hasFlag(FLAG_INDEX_OF_INTEREST)) {
      INT_ASSERT(lhs->hasFlag(FLAG_INDEX_VAR));
      // ... and not of a reference type
      // todo: differentiate based on ref-ness, not _ref type
      // todo: not all const if it is zippered and one of iterators is var
      if (!isReferenceType(rhsSE->var->type))
       // ... and not an array (arrays are always yielded by reference)
       if (!rhsSE->var->type->symbol->hasFlag(FLAG_ARRAY))
        // ... then mark LHS constant.
        lhs->addFlag(FLAG_CONST);
    }
  } else if (CallExpr* rhsCall = toCallExpr(rhs)) {
    if (rhsCall->isPrimitive(PRIM_GET_MEMBER)) {
      if (SymExpr* rhsBase = toSymExpr(rhsCall->get(1))) {
        if (rhsBase->var->hasFlag(FLAG_CONST) ||
            rhsBase->var->hasFlag(FLAG_REF_TO_CONST)
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
  for (int i = 0; i < se->var->type->substitutions.n; i++) {
    if (se->var->type->substitutions.v[i].key) {
      if (!strcmp("size", se->var->type->substitutions.v[i].key->name)) {
        size = toVarSymbol(se->var->type->substitutions.v[i].value)->immediate->int_value();
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
  Type* type = sym->var->getValType();

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
    DefExpr* def = new DefExpr(tmp);
    call->getStmtExpr()->insertBefore(def);
    CallExpr* e = NULL;
    if (!call->parentSymbol->hasFlag(FLAG_EXPAND_TUPLES_WITH_VALUES)) {
      e = new CallExpr(sym->copy(), new_IntSymbol(i));
    } else {
      e = new CallExpr(PRIM_GET_MEMBER_VALUE, sym->copy(),
                       new_StringSymbol(astr("x", istr(i))));
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
  VarSymbol* var = toVarSymbol(sym->var);
  if (!var || !var->immediate)
    INT_FATAL(call, "bad set member primitive");
  const char* name = var->immediate->v_string;

  // Special case: An integer field name is actually a tuple member index.
  {
    int64_t i;
    if (get_int(sym, &i)) {
      name = astr("x", istr(i));
      call->get(2)->replace(new SymExpr(new_StringSymbol(name)));
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
  if (fs->type == dtUnknown)
    fs->type = t;

  if (t != fs->type && t != dtNil && t != dtObject) {
    USR_FATAL(userCall(call),
              "cannot assign expression of type %s to field of type %s",
              toString(t), toString(fs->type));
  }
}



static void resolveMove(CallExpr* call) {
  Expr* rhs = call->get(2);
  Symbol* lhs = NULL;
  if (SymExpr* se = toSymExpr(call->get(1)))
    lhs = se->var;
  INT_ASSERT(lhs);

  FnSymbol* fn = toFnSymbol(call->parentSymbol);
  bool isReturn = fn ? lhs == fn->getReturnSymbol() : false;

  if (lhs->hasFlag(FLAG_TYPE_VARIABLE) && !isTypeExpr(rhs)) {
    if (isReturn) {
      if (!call->parentSymbol->hasFlag(FLAG_RUNTIME_TYPE_INIT_FN))
        USR_FATAL(call, "illegal return of value where type is expected");
    } else {
      USR_FATAL(call, "illegal assignment of value to type");
    }
  }

  if (!lhs->hasFlag(FLAG_TYPE_VARIABLE) && !lhs->hasFlag(FLAG_MAYBE_TYPE) && isTypeExpr(rhs)) {
    if (isReturn) {
      USR_FATAL(call, "illegal return of type where value is expected");
    } else {
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

  if (rhsType == dtVoid) {
    if (isReturn && (lhs->type == dtVoid || lhs->type == dtUnknown))
    {
      // It is OK to assign void to the return value variable as long as its
      // type is void or is not yet established.
    }
    else
    {
      if (CallExpr* rhsFn = toCallExpr(rhs)) {
        if (FnSymbol* rhsFnSym = rhsFn->isResolved()) {
          USR_FATAL(userCall(call),
                    "illegal use of function that does not return a value: '%s'",
                    rhsFnSym->name);
        }
      }
      USR_FATAL(userCall(call),
                "illegal use of function that does not return a value");
    }
  }

  if (lhs->type == dtUnknown || lhs->type == dtNil)
    lhs->type = rhsType;

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
      rhs->replace(new CallExpr(PRIM_SIZEOF, sizeSym->var->typeInfo()->symbol));
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
                                                     new SymExpr(castVar->var))));
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
}


// Some new expressions are converted in normalize().  For example, a call to a
// type function is resolved at this point.
// The syntax supports calling the result of a type function as a constructor,
// but this is not fully implemented.
static void
resolveNew(CallExpr* call)
{
  // This is a 'new' primitive, so we expect the argument to be a constructor
  // call.
  CallExpr* ctor = toCallExpr(call->get(1));

  // May need to resolve ctor here.
  if (FnSymbol* fn = ctor->isResolved())
  {
    // If the function is a constructor, just bridge out the 'new' primitive
    // and call the constructor.  Done.
    if (fn->hasFlag(FLAG_CONSTRUCTOR))
    {
      call->replace(ctor);
      return;
    }

    // Not a constructor, so issue an error.
    USR_FATAL(call, "invalid use of 'new' on %s", fn->name);
    return;
  }

  if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(ctor->baseExpr))
  {
    USR_FATAL(call, "invalid use of 'new' on %s", urse->unresolved);
    return;
  }

  USR_FATAL(call, "invalid use of 'new'");
}


//
// This tells us whether we can rely on the compiler's back end (e.g.,
// C) to provide the copy for us for 'in' or 'const in' intents when
// passing an argument of type 't'.
//
static bool backendRequiresCopyForIn(Type* t) {
  return (isRecord(t) ||
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
      (backendRequiresCopyForIn(formal->type) || !fn->hasFlag(FLAG_INLINE)))
     //
     // The following case reduces memory leaks for zippered forall
     // leader/follower pairs where the communicated intermediate
     // result is a tuple of ranges, but I can't explain why it'd be
     // needed.  Tom has said that iterators haven't really been
     // bolted down in terms of memory leaks, so future work would be
     // to remove this hack and close the leak properly.
     //
     || strcmp(formal->name, "followThis") == 0
     );
}

static void
insertFormalTemps(FnSymbol* fn) {
  SymbolMap formals2vars;
  for_formals(formal, fn) {
    if (formalRequiresTemp(formal)) {
      SET_LINENO(formal);
      VarSymbol* tmp = newTemp(astr("_formal_tmp_", formal->name));
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
// Note that autoCopy is called in this case, but not for "inout", "in" and "const in".
// Either record-wrapped types are always passed by ref, or some unexpected
// behavior will result by applying "in" intents to them.
static void addLocalCopiesAndWritebacks(FnSymbol* fn, SymbolMap& formals2vars)
{
  // Enumerate the formals that have local temps.
  form_Map(SymbolMapElem, e, formals2vars) {
    ArgSymbol* formal = toArgSymbol(e->key); // Get the formal.
    Symbol* tmp = e->value; // Get the temp.

    SET_LINENO(formal);

    // TODO: Move this closer to the location (in code) where we determine
    // whether tmp owns its value or not.  That is, push setting these flags
    // (or not) into the cases below, as appropriate.
    Type* formalType = formal->type->getValType();
    if ((formal->intent == INTENT_BLANK ||
         formal->intent == INTENT_CONST ||
         formal->intent == INTENT_CONST_IN) &&
        !isSyncType(formalType) &&
        !isRefCountedType(formalType))
    {
      tmp->addFlag(FLAG_CONST);
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
      if (formal->defaultExpr && formal->defaultExpr->body.tail->typeInfo() != dtTypeDefaultToken) {
        BlockStmt* defaultExpr = formal->defaultExpr->copy();
        fn->insertAtHead(new CallExpr(PRIM_MOVE, tmp, defaultExpr->body.tail->remove()));
        fn->insertAtHead(defaultExpr);
      } else {
        VarSymbol* refTmp = newTemp("_formal_ref_tmp_");
        VarSymbol* typeTmp = newTemp("_formal_type_tmp_");
        typeTmp->addFlag(FLAG_MAYBE_TYPE);
        fn->insertAtHead(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_INIT, typeTmp)));
        fn->insertAtHead(new CallExpr(PRIM_MOVE, typeTmp, new CallExpr(PRIM_TYPEOF, refTmp)));
        fn->insertAtHead(new CallExpr(PRIM_MOVE, refTmp, new CallExpr(PRIM_DEREF, formal)));
        fn->insertAtHead(new DefExpr(refTmp));
        fn->insertAtHead(new DefExpr(typeTmp));
      }
      break;

     case INTENT_INOUT:
     case INTENT_IN:
     case INTENT_CONST_IN:
      // TODO: Adding a formal temp for INTENT_CONST_IN is conservative.
      // If the compiler verifies in a separate pass that it is never written,
      // we don't have to copy it.
      fn->insertAtHead(new CallExpr(PRIM_MOVE, tmp, new CallExpr("chpl__initCopy", formal)));
      tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
      break;

     case INTENT_BLANK:
     case INTENT_CONST:
     {
       TypeSymbol* ts = formal->type->symbol;

       if (!getRecordWrappedFlags(ts).any() &&
           !ts->hasFlag(FLAG_ITERATOR_CLASS) &&
           !ts->hasFlag(FLAG_ITERATOR_RECORD) &&
           !getSyncFlags(ts).any()) {
         if (fn->hasFlag(FLAG_BEGIN)) {
           // autoCopy/autoDestroy will be added later, in parallel pass
           // by insertAutoCopyDestroyForTaskArg()
           fn->insertAtHead(new CallExpr(PRIM_MOVE, tmp, formal));
           tmp->removeFlag(FLAG_INSERT_AUTO_DESTROY);
         } else {
           // Note that because we reject the case of record-wrapped types above,
           // the only way we can get a formal whose call to chpl__autoCopy does
           // anything different from calling chpl__initCopy is if the formal is a
           // tuple containing a record-wrapped type.  This is probably not
           // intentional: It gives tuple-wrapped record-wrapped types different
           // behavior from bare record-wrapped types.
           fn->insertAtHead(new CallExpr(PRIM_MOVE, tmp, new CallExpr("chpl__autoCopy", formal)));
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
      fn->insertBeforeReturnAfterLabel(new CallExpr("=", formal, tmp));
    }
  }
}

//
//
//

static Expr* dropUnnecessaryCast(CallExpr* call) {
  // Check for and remove casts to the original type and size
  Expr* result = call;
  if (!call->isNamed("_cast"))
    INT_FATAL("dropUnnecessaryCasts called on non _cast call");

  if (SymExpr* sym = toSymExpr(call->get(2))) {
    if (VarSymbol* var = toVarSymbol(sym->var)) {
      if (SymExpr* sym = toSymExpr(call->get(1))) {
        Type* oldType = var->type;
        Type* newType = sym->var->type;

        if (newType == oldType) {
          result = new SymExpr(var);
          call->replace(result);
        }
      }
    } else if (EnumSymbol* e = toEnumSymbol(sym->var)) {
      if (SymExpr* sym = toSymExpr(call->get(1))) {
        EnumType* oldType = toEnumType(e->type);
        EnumType* newType = toEnumType(sym->var->type);
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
  Creates the parent class which will represent the function's type.  Children of the parent class will capture different functions which
  happen to share the same function type.  By using the parent class we can assign new values onto variable that match the function type
  but may currently be pointing at a different function.
*/
static AggregateType* createAndInsertFunParentClass(CallExpr *call, const char *name) {
  AggregateType *parent = new AggregateType(AGGREGATE_CLASS);
  TypeSymbol *parent_ts = new TypeSymbol(name, parent);

  parent_ts->addFlag(FLAG_FUNCTION_CLASS);

  // Because this function type needs to be globally visible (because we don't know the modules it will be passed to), we put
  // it at the highest scope
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
  To mimic a function call, we create a .this method for the parent class.  This will allow the object to look and feel like a
  first-class function, by both being an object and being invoked using parentheses syntax.  Children of the parent class will
  override this method and wrap the function that is being used as a first-class value.

  To focus on just the types of the arguments and not their names or default values, we use the parent method's names and types
  as the basis for all children which override it.

  The function is put at the highest scope so that all functions of a given type will share the same parent class.
*/
static FnSymbol* createAndInsertFunParentMethod(CallExpr *call, AggregateType *parent, AList &arg_list, bool isFormal, Type *retType) {
  FnSymbol* parent_method = new FnSymbol("this");
  parent_method->addFlag(FLAG_FIRST_CLASS_FUNCTION_INVOCATION);
  parent_method->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  parent_method->addFlag(FLAG_METHOD);
  ArgSymbol* thisParentSymbol = new ArgSymbol(INTENT_BLANK, "this", parent);
  thisParentSymbol->addFlag(FLAG_ARG_THIS);
  parent_method->insertFormalAtTail(thisParentSymbol);
  parent_method->_this = thisParentSymbol;

  int i = 0, alength = arg_list.length;

  //We handle the arg list differently depending on if it's a list of formal args or actual args
  if (isFormal) {

    for_alist(formalExpr, arg_list) {
      DefExpr* dExp = toDefExpr(formalExpr);
      ArgSymbol* fArg = toArgSymbol(dExp->sym);

      if (fArg->type != dtVoid) {
        ArgSymbol* newFormal = new ArgSymbol(INTENT_BLANK, fArg->name, fArg->type);
        if (fArg->typeExpr)
          newFormal->typeExpr = fArg->typeExpr->copy();

        parent_method->insertFormalAtTail(newFormal);
      }
    }
  }
  else {
    char name_buffer[100];
    int name_index = 0;

    for_alist(actualExpr, arg_list) {
      sprintf(name_buffer, "name%i", name_index++);
      if (i != (alength-1)) {
        SymExpr* sExpr = toSymExpr(actualExpr);
        if (sExpr->var->type != dtVoid) {
          ArgSymbol* newFormal = new ArgSymbol(INTENT_BLANK, name_buffer, sExpr->var->type);

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

  // Because this function type needs to be globally visible (because we don't know the modules it will be passed to), we put
  // it at the highest scope
  theProgram->block->body.insertAtTail(new DefExpr(parent_method));

  normalize(parent_method);

  parent->methods.add(parent_method);

  return parent_method;
}

/*
  Builds up the name of the parent for lookup by looking through the types of the arguments, either formal or actual
*/
static std::string buildParentName(AList &arg_list, bool isFormal, Type *retType) {
  std::ostringstream oss;
  oss << "chpl__fcf_type_";

  bool isFirst = true;

  if (isFormal) {
    if (arg_list.length == 0) {
      oss << "void";
    }
    else {
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
  }
  else {
    int i = 0, alength = arg_list.length;

    if (alength == 1) {
      oss << "void_";
    }

    for_alist(actualExpr, arg_list) {
      if (!isFirst)
        oss << "_";

      SymExpr* sExpr = toSymExpr(actualExpr);

      ++i;

      oss << sExpr->var->type->symbol->cname;

      isFirst = false;
    }
  }

  return oss.str();
}

/*
  Helper function for creating or finding the parent class for a given function type specified
  by the type signature.  The last type given in the signature is the return type, the remainder
  represent arguments to the function.
*/
static AggregateType* createOrFindFunTypeFromAnnotation(AList &arg_list, CallExpr *call) {
  AggregateType *parent;

  SymExpr *retTail = toSymExpr(arg_list.tail);
  Type *retType = retTail->var->type;

  std::string parent_name = buildParentName(arg_list, false, retType);

  if (functionTypeMap.find(parent_name) != functionTypeMap.end()) {
    parent = functionTypeMap[parent_name].first;

  } else {
    parent                  = createAndInsertFunParentClass(call, parent_name.c_str());
    FnSymbol* parent_method = createAndInsertFunParentMethod(call, parent, arg_list, false, retType);

    functionTypeMap[parent_name] = std::pair<AggregateType*, FnSymbol*>(parent, parent_method);
  }

  return parent;
}

/*
  Captures a function as a first-class value by creating an object that will represent the function.  The class is
  created at the same scope as the function being referenced.  Each class is unique and shared among all
  uses of that function as a value.  Once built, the class will override the .this method of the parent and wrap
  the call to the function being captured as a value.  Then, an instance of the class is instantiated and returned.
*/
static Expr*
createFunctionAsValue(CallExpr *call) {
  static int unique_fcf_id = 0;
  UnresolvedSymExpr* use = toUnresolvedSymExpr(call->get(1));
  INT_ASSERT(use);
  const char *flname = use->unresolved;

  Vec<FnSymbol*> visibleFns;
  Vec<BlockStmt*> visited;
  getVisibleFunctions(getVisibilityBlock(call), flname, visibleFns, visited);

  if (visibleFns.n > 1) {
    USR_FATAL(call, "%s: can not capture overloaded functions as values",
                    visibleFns.v[0]->name);
  }

  INT_ASSERT(visibleFns.n == 1);

  FnSymbol* captured_fn = visibleFns.head();

  //Check to see if we've already cached the capture somewhere
  if (functionCaptureMap.find(captured_fn) != functionCaptureMap.end()) {
    return new CallExpr(functionCaptureMap[captured_fn]);
  }

  resolveFormals(captured_fn);
  resolveFns(captured_fn);

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

//
// returns true if the symbol is defined in an outer function to fn
// third argument not used at call site
//
static bool
isOuterVar(Symbol* sym, FnSymbol* fn, Symbol* parent /* = NULL*/) {
  if (!parent)
    parent = fn->defPoint->parentSymbol;
  if (!isFnSymbol(parent))
    return false;
  else if (sym->defPoint->parentSymbol == parent)
    return true;
  else
    return isOuterVar(sym, fn, parent->defPoint->parentSymbol);
}


//
// finds outer vars directly used in a function
//
static bool
usesOuterVars(FnSymbol* fn, Vec<FnSymbol*> &seen) {
  std::vector<BaseAST*> asts;
  collect_asts(fn, asts);
  for_vector(BaseAST, ast, asts) {
    if (toCallExpr(ast)) {
      CallExpr *call = toCallExpr(ast);

      //dive into calls
      Vec<FnSymbol*> visibleFns;
      Vec<BlockStmt*> visited;

      getVisibleFunctions(getVisibilityBlock(call), call->parentSymbol->name, visibleFns, visited);

      forv_Vec(FnSymbol, called_fn, visibleFns) {
        bool seen_this_fn = false;
        forv_Vec(FnSymbol, seen_fn, seen) {
          if (called_fn == seen_fn) {
            seen_this_fn = true;
            break;
          }
        }
        if (!seen_this_fn) {
          seen.add(called_fn);
          if (usesOuterVars(called_fn, seen)) {
            return true;
          }
        }
      }
    }
    if (SymExpr* symExpr = toSymExpr(ast)) {
      Symbol* sym = symExpr->var;

      if (isLcnSymbol(sym)) {
        if (isOuterVar(sym, fn))
          return true;
      }
    }
  }
  return false;
}

static bool
isNormalField(Symbol* field)
{
  if( field->hasFlag(FLAG_IMPLICIT_ALIAS_FIELD) ) return false;
  if( field->hasFlag(FLAG_TYPE_VARIABLE) ) return false;
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

// Recursively resolve typedefs
static Type* resolveTypeAlias(SymExpr* se)
{
  if (! se)
    return NULL;

  // Quick exit if the type is already known.
  Type* result = se->getValType();
  if (result != dtUnknown)
    return result;

  VarSymbol* var = toVarSymbol(se->var);
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
      if (destSE->var->hasFlag(FLAG_INDEX_VAR)) {
        // The destination is constant only if both the tuple
        // and the current component are non-references.
        // And it's not an array (arrays are always yielded by reference)
        // - see boundaries() in release/examples/benchmarks/miniMD/miniMD.
        if (!fieldType->symbol->hasFlag(FLAG_ARRAY)) {
          destSE->var->addFlag(FLAG_CONST);
        }
      } else {
        INT_ASSERT(destSE->var->hasFlag(FLAG_TEMP));
        // We are detupling into another tuple,
        // which will be detupled later.
        destSE->var->addFlag(FLAG_INDEX_OF_INTEREST);
      }
    }

    if (!isReferenceType(baseVar->type))
      // If either a non-var iterator or zippered,
      // extract with PRIM_GET_MEMBER_VALUE.
      intoIndexVarByVal = true;
  }

  Expr* result;
  if (isReferenceType(fieldType) || intoIndexVarByVal)
    result = new CallExpr(PRIM_GET_MEMBER_VALUE, baseVar, new_StringSymbol(field));
  else
    result = new CallExpr(PRIM_GET_MEMBER, baseVar, new_StringSymbol(field));

  call->replace(result);
  return result;
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
  if (!se->var->hasFlag(FLAG_TYPE_VARIABLE))
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

  CallExpr* defOfCall = new CallExpr("_defaultOf", type->symbol);
  call->replace(defOfCall);
  resolveCall(defOfCall);
  resolveFns(defOfCall->isResolved());
  result = postFold(defOfCall);
  return result;
}


static Expr*
preFold(Expr* expr) {
  Expr* result = expr;
  if (CallExpr* call = toCallExpr(expr)) {
    // Match calls that look like:  (<type-symbol> <immediate-integer>)
    // and replace them with:       <new-type-symbol>
    // <type-symbol> is in {dtBools, dtInt, dtUint, dtReal, dtImag, dtComplex}.
    // This replaces, e.g. ( dtInt[INT_SIZE_DEFAULT] 32) with dtInt[INT_SIZE_32].
    if (SymExpr* sym = toSymExpr(call->baseExpr)) {
      if (TypeSymbol* type = toTypeSymbol(sym->var)) {
        if (call->numActuals() == 1) {
          if (SymExpr* arg = toSymExpr(call->get(1))) {
            if (VarSymbol* var = toVarSymbol(arg->var)) {
              if (var->immediate) {
                if (NUM_KIND_INT == var->immediate->const_kind ||
                    NUM_KIND_UINT == var->immediate->const_kind) {
                  int size;
                  if (NUM_KIND_INT == var->immediate->const_kind) {
                    size = var->immediate->int_value();
                  } else {
                    size = (int)var->immediate->uint_value();
                  }
                  TypeSymbol* tsize = NULL;
                  if (type == dtBools[BOOL_SIZE_SYS]->symbol) {
                    switch (size) {
                    case 8: tsize = dtBools[BOOL_SIZE_8]->symbol; break;
                    case 16: tsize = dtBools[BOOL_SIZE_16]->symbol; break;
                    case 32: tsize = dtBools[BOOL_SIZE_32]->symbol; break;
                    case 64: tsize = dtBools[BOOL_SIZE_64]->symbol; break;
                    default:
                      USR_FATAL( call, "illegal size %d for bool", size);
                    }
                    result = new SymExpr(tsize);
                    call->replace(result);
                  } else if (type == dtInt[INT_SIZE_DEFAULT]->symbol) {
                    switch (size) {
                    case 8: tsize = dtInt[INT_SIZE_8]->symbol; break;
                    case 16: tsize = dtInt[INT_SIZE_16]->symbol; break;
                    case 32: tsize = dtInt[INT_SIZE_32]->symbol; break;
                    case 64: tsize = dtInt[INT_SIZE_64]->symbol; break;
                    default:
                      USR_FATAL( call, "illegal size %d for int", size);
                    }
                    result = new SymExpr(tsize);
                    call->replace(result);
                  } else if (type == dtUInt[INT_SIZE_DEFAULT]->symbol) {
                    switch (size) {
                    case  8: tsize = dtUInt[INT_SIZE_8]->symbol;  break;
                    case 16: tsize = dtUInt[INT_SIZE_16]->symbol; break;
                    case 32: tsize = dtUInt[INT_SIZE_32]->symbol; break;
                    case 64: tsize = dtUInt[INT_SIZE_64]->symbol; break;
                    default:
                      USR_FATAL( call, "illegal size %d for uint", size);
                    }
                    result = new SymExpr(tsize);
                    call->replace(result);
                  } else if (type == dtReal[FLOAT_SIZE_64]->symbol) {
                    switch (size) {
                    case 32:  tsize = dtReal[FLOAT_SIZE_32]->symbol;  break;
                    case 64:  tsize = dtReal[FLOAT_SIZE_64]->symbol;  break;
                    default:
                      USR_FATAL( call, "illegal size %d for real", size);
                    }
                    result = new SymExpr(tsize);
                    call->replace(result);
                  } else if (type == dtImag[FLOAT_SIZE_64]->symbol) {
                    switch (size) {
                    case 32:  tsize = dtImag[FLOAT_SIZE_32]->symbol;  break;
                    case 64:  tsize = dtImag[FLOAT_SIZE_64]->symbol;  break;
                    default:
                      USR_FATAL( call, "illegal size %d for imag", size);
                    }
                    result = new SymExpr(tsize);
                    call->replace(result);
                  } else if (type == dtComplex[COMPLEX_SIZE_128]->symbol) {
                    switch (size) {
                    case 64:  tsize = dtComplex[COMPLEX_SIZE_64]->symbol;  break;
                    case 128: tsize = dtComplex[COMPLEX_SIZE_128]->symbol; break;
                    default:
                      USR_FATAL( call, "illegal size %d for complex", size);
                    }
                    result = new SymExpr(tsize);
                    call->replace(result);
                  }
                }
              }
            }
          }
        }
      }
    }

    if (SymExpr* sym = toSymExpr(call->baseExpr)) {
      if (isLcnSymbol(sym->var)) {
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
      INT_ASSERT(base);
      if (isVarSymbol(base->var) && base->var->hasFlag(FLAG_TYPE_VARIABLE)) {
        if (call->numActuals() == 2)
          USR_FATAL(call, "illegal call of type");
        int64_t index;
        if (!get_int(call->get(3), &index))
          USR_FATAL(call, "illegal type index expression");
        char field[8];
        sprintf(field, "x%" PRId64, index);
        result = new SymExpr(base->var->type->getField(field)->type->symbol);
        call->replace(result);
      } else if (base && isLcnSymbol(base->var)) {
        //
        // resolve tuple indexing by an integral parameter
        //
        Type* t = base->var->getValType();
        if (t->symbol->hasFlag(FLAG_TUPLE))
          if (Expr* expr = resolveTupleIndexing(call, base->var))
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
      if (!se->var->hasFlag(FLAG_TYPE_VARIABLE))
        USR_FATAL(call, "invalid type specification");
      Type* type = call->get(1)->getValType();
      if (isAggregateType(type)) {
        if (type->symbol->hasFlag(FLAG_IGNORE_NOINIT)) {
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
                Symbol* dest = holdsDest->var;
                if (dest->hasFlag(FLAG_TEMP) && !strcmp(dest->name, "ret")) {
                  nowarn = true;
                }
              }
            }
          }
          if (!nowarn)
            USR_WARN("type %s does not currently support noinit, using default initialization", type->symbol->name);
          result = new CallExpr(PRIM_INIT, call->get(1)->remove());
          call->replace(result);
          inits.add((CallExpr *)result);
        } else {
          result = call;
          inits.add(call);
        }
      }

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
          result = new CallExpr(PRIM_GET_MEMBER_VALUE, call->get(1)->remove(), new_StringSymbol(field->name));
        else
          result = new CallExpr(field->name, gMethodToken, call->get(1)->remove());
        call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, result));
        call->replace(new CallExpr(PRIM_TYPEOF, tmp));
      } else
        USR_FATAL(call, "invalid query -- queried field must be a type or parameter");
    } else if (call->isPrimitive(PRIM_CAPTURE_FN)) {
      result = createFunctionAsValue(call);
      call->replace(result);
    } else if (call->isPrimitive(PRIM_CREATE_FN_TYPE)) {
      AggregateType *parent = createOrFindFunTypeFromAnnotation(call->argList, call);

      result = new SymExpr(parent->symbol);
      call->replace(result);
    } else if (call->isNamed("chpl__initCopy") ||
               call->isNamed("chpl__autoCopy")) {
      if (call->numActuals() == 1) {
        if (SymExpr* symExpr = toSymExpr(call->get(1))) {
          if (VarSymbol* var = toVarSymbol(symExpr->var)) {
            if (var->immediate) {
              result = new SymExpr(var);
              call->replace(result);
            }
          } else {
            if (EnumSymbol* var = toEnumSymbol(symExpr->var)) {
              // Treat enum values as immediates
              result = new SymExpr(var);
              call->replace(result);
            }
          }
        }
      }
    } else if (call->isNamed("_cast")) {
      result = dropUnnecessaryCast(call);
      if (result == call) {
        // The cast was not dropped.  Remove integer casts on immediate values.
        if (SymExpr* sym = toSymExpr(call->get(2))) {
          if (VarSymbol* var = toVarSymbol(sym->var)) {
            if (var->immediate) {
              if (SymExpr* sym = toSymExpr(call->get(1))) {
                Type* oldType = var->type;
                Type* newType = sym->var->type;
                if ((is_int_type(oldType) || is_uint_type(oldType) ||
                     is_bool_type(oldType)) &&
                    (is_int_type(newType) || is_uint_type(newType) ||
                     is_bool_type(newType) || is_enum_type(newType) ||
                     (newType == dtStringC))) {
                  VarSymbol* typevar = toVarSymbol(newType->defaultValue);
                  EnumType* typeenum = toEnumType(newType);
                  if (typevar) {
                    if (!typevar->immediate)
                      INT_FATAL("unexpected case in cast_fold");

                    Immediate coerce = *typevar->immediate;
                    coerce_immediate(var->immediate, &coerce);
                    result = new SymExpr(new_ImmediateSymbol(&coerce));
                    call->replace(result);
                  } else if (typeenum) {
                    int64_t value, count = 0;
                    bool replaced = false;
                    if (!get_int(call->get(2), &value)) {
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
                        break;
                      }
                    }
                    if (!replaced) {
                      USR_FATAL(call->get(2), "enum cast out of bounds");
                    }
                  } else {
                    INT_FATAL("unexpected case in cast_fold");
                  }
                }
              }
            }
          } else if (EnumSymbol* enumSym = toEnumSymbol(sym->var)) {
            if (SymExpr* sym = toSymExpr(call->get(1))) {
              Type* newType = sym->var->type;
              if (newType == dtStringC) {
                result = new SymExpr(new_StringSymbol(enumSym->name));
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
        if (VarSymbol* var = toVarSymbol(sym->var)) {
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
      bool removed = false;
      SymExpr* sym1 = toSymExpr(call->get(1));
      if (VarSymbol* sym = toVarSymbol(sym1->var)) {
        if (sym->immediate || paramMap.get(sym)) {
          CallExpr* mvCall = toCallExpr(call->parentExpr);
          SymExpr* sym = toSymExpr(mvCall->get(1));
          VarSymbol* var = toVarSymbol(sym->var);
          removed = true;
          var->addFlag(FLAG_MAYBE_PARAM);
          result = call->get(2)->remove();
          call->replace(result);
        }
      }
      if (!removed) {
        result = call->get(2)->remove();
        call->replace(result);
      }
    } else if (call->isPrimitive(PRIM_ADDR_OF)) {
      // remove set ref if already a reference
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
        result = call->get(1)->remove();
        call->replace(result);
      } else {
        // This test is turned off if we are in a wrapper function.
        FnSymbol* fn = call->getFunction();
        if (!fn->hasFlag(FLAG_WRAPPER)) {
          SymExpr* lhs = NULL;
          // check legal var function return
          if (CallExpr* move = toCallExpr(call->parentExpr)) {
            if (move->isPrimitive(PRIM_MOVE)) {
              lhs = toSymExpr(move->get(1));
              if (lhs && lhs->var == fn->getReturnSymbol()) {
                SymExpr* ret = toSymExpr(call->get(1));
                INT_ASSERT(ret);
                if (ret->var->defPoint->getFunction() == move->getFunction() &&
                    !ret->var->type->symbol->hasFlag(FLAG_ITERATOR_RECORD) &&
                    !ret->var->type->symbol->hasFlag(FLAG_ARRAY))
                  // Should this conditional include domains, distributions, sync and/or single?
                  USR_FATAL(ret, "illegal expression to return by ref");
                if (ret->var->isConstant() || ret->var->isParameter())
                  USR_FATAL(ret, "function cannot return constant by ref");
              }
            }
          }
          //
          // check that the operand of 'addr of' is a legal lvalue.
          if (SymExpr* rhs = toSymExpr(call->get(1))) {
            if (!(lhs && lhs->var->hasFlag(FLAG_REF_VAR) && lhs->var->hasFlag(FLAG_CONST))) {
              if (rhs->var->hasFlag(FLAG_EXPR_TEMP) || rhs->var->isConstant() || rhs->var->isParameter()) {
                if (lhs && lhs->var->hasFlag(FLAG_REF_VAR)) {
                  if (rhs->var->isImmediate()) {
                    USR_FATAL_CONT(call, "Cannot set a non-const reference to a literal value.");
                  } else {
                    // We should not fall into this case... should be handled in normalize
                    INT_FATAL(call, "Cannot set a non-const reference to a const variable.");
                  }
                } else {
                  // This probably indicates that an invalid 'addr of' primitive
                  // was inserted, which would be the compiler's fault, not the
                  // user's.
                  // At least, we might perform the check at or before the 'addr
                  // of' primitive is inserted.
                  INT_FATAL(call, "A non-lvalue appears where an lvalue is expected.");
                }
              }
            }
          }
        }
      }
    } else if (call->isPrimitive(PRIM_DEREF)) {
      // remove deref if arg is already a value
      if (!call->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
        result = call->get(1)->remove();
        call->replace(result);
      }
    } else if (call->isPrimitive(PRIM_TYPE_TO_STRING)) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se && se->var->hasFlag(FLAG_TYPE_VARIABLE));
      result = new SymExpr(new_StringSymbol(se->var->type->symbol->name));
      call->replace(result);
    } else if (call->isPrimitive(PRIM_WIDE_GET_LOCALE) ||
               call->isPrimitive(PRIM_WIDE_GET_NODE)) {
      Type* type = call->get(1)->getValType();

      //
      // ensure .locale (and on) are applied to lvalues or classes
      // (locale type is a class)
      //
      SymExpr* se = toSymExpr(call->get(1));
      if (se->var->hasFlag(FLAG_EXPR_TEMP) && !isClass(type))
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
      FnSymbol* iterator = call->get(1)->typeInfo()->defaultInitializer->getFormal(1)->type->defaultInitializer;
     CallExpr* standaloneCall = new CallExpr(iterator->name);
      for_formals(formal, iterator) {
        standaloneCall->insertAtTail(new NamedExpr(formal->name, new SymExpr(formal)));
      }
      // "tag" should be placed at the end of the formals in the source code as
      // well, to avoid insertion of an order wrapper.
      standaloneCall->insertAtTail(new NamedExpr("tag", new SymExpr(gStandaloneTag)));
      call->replace(standaloneCall);
      result = standaloneCall;
    } else if (call->isPrimitive(PRIM_TO_LEADER)) {
      FnSymbol* iterator = call->get(1)->typeInfo()->defaultInitializer->getFormal(1)->type->defaultInitializer;
      CallExpr* leaderCall;
      if (FnSymbol* leader = iteratorLeaderMap.get(iterator))
        leaderCall = new CallExpr(leader);
      else
        leaderCall = new CallExpr(iterator->name);
      for_formals(formal, iterator) {
        leaderCall->insertAtTail(new NamedExpr(formal->name, new SymExpr(formal)));
      }
      // "tag" should be placed at the end of the formals in the source code as
      // well, to avoid insertion of an order wrapper.
      leaderCall->insertAtTail(new NamedExpr("tag", new SymExpr(gLeaderTag)));
      call->replace(leaderCall);
      result = leaderCall;
    } else if (call->isPrimitive(PRIM_TO_FOLLOWER)) {
      FnSymbol* iterator = call->get(1)->typeInfo()->defaultInitializer->getFormal(1)->type->defaultInitializer;
      CallExpr* followerCall;
      if (FnSymbol* follower = iteratorFollowerMap.get(iterator))
        followerCall = new CallExpr(follower);
      else
        followerCall = new CallExpr(iterator->name);
      for_formals(formal, iterator) {
        followerCall->insertAtTail(new NamedExpr(formal->name, new SymExpr(formal)));
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
      AggregateType* classtype = toAggregateType(toSymExpr(call->get(1))->var->type);
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
      AggregateType* classtype = toAggregateType(toSymExpr(call->get(1))->var->type);
      INT_ASSERT( classtype != NULL );
      classtype = toAggregateType(classtype->getValType());
      INT_ASSERT( classtype != NULL );

      VarSymbol* var = toVarSymbol(toSymExpr(call->get(2))->var);

      INT_ASSERT( var != NULL );

      int fieldnum = var->immediate->int_value();
      int fieldcount = 0;
      const char* name = NULL;
      for_fields(field, classtype) {
        if( ! isNormalField(field) ) continue;

        fieldcount++;
        if (fieldcount == fieldnum) {
          name = field->name;
        }
      }
      if (!name) {
        // In this case, we ran out of fields without finding the number
        // specified.  This is the user's error.
        USR_FATAL(call, "'%d' is not a valid field number", fieldnum);
      }
      result = new SymExpr(new_StringSymbol(name));
      call->replace(result);
    } else if (call->isPrimitive(PRIM_FIELD_VALUE_BY_NUM)) {
      AggregateType* classtype = toAggregateType(call->get(1)->typeInfo());
      INT_ASSERT( classtype != NULL );
      classtype = toAggregateType(classtype->getValType());
      INT_ASSERT( classtype != NULL );

      VarSymbol* var = toVarSymbol(toSymExpr(call->get(2))->var);

      INT_ASSERT( var != NULL );

      int fieldnum = var->immediate->int_value();
      int fieldcount = 0;

      for_fields(field, classtype) {
        if( ! isNormalField(field) ) continue;

        fieldcount++;
        if (fieldcount == fieldnum) {
          result = new CallExpr(PRIM_GET_MEMBER, call->get(1)->copy(),
                                new_StringSymbol(field->name));
          break;
        }
      }
      call->replace(result);
    } else if (call->isPrimitive(PRIM_FIELD_ID_BY_NUM)) {
      AggregateType* classtype = toAggregateType(call->get(1)->typeInfo());
      INT_ASSERT( classtype != NULL );
      classtype = toAggregateType(classtype->getValType());
      INT_ASSERT( classtype != NULL );

      VarSymbol* var = toVarSymbol(toSymExpr(call->get(2))->var);

      INT_ASSERT( var != NULL );

      int fieldnum = var->immediate->int_value();
      int fieldcount = 0;
      for_fields(field, classtype) {
        if( ! isNormalField(field) ) continue;

        fieldcount++;
        if (fieldcount == fieldnum) {
          result = new SymExpr(new_IntSymbol(field->id));
          break;
        }
      }
      call->replace(result);
    } else if (call->isPrimitive(PRIM_FIELD_VALUE_BY_NAME)) {
      AggregateType* classtype = toAggregateType(call->get(1)->typeInfo());
      INT_ASSERT( classtype != NULL );
      classtype = toAggregateType(classtype->getValType());
      INT_ASSERT( classtype != NULL );

      VarSymbol* var = toVarSymbol(toSymExpr(call->get(2))->var);
      INT_ASSERT( var != NULL );

      Immediate* imm = var->immediate;

      INT_ASSERT( classtype != NULL );
      // fail horribly if immediate is not a string .
      INT_ASSERT(imm->const_kind == CONST_KIND_STRING);

      const char* fieldname = imm->v_string;
      int fieldcount = 0;
      for_fields(field, classtype) {
        if( ! isNormalField(field) ) continue;

        fieldcount++;
        if ( 0 == strcmp(field->name,  fieldname) ) {
          result = new CallExpr(PRIM_GET_MEMBER, call->get(1)->copy(),
                                new_StringSymbol(field->name));
          break;
        }
      }
      call->replace(result);
    } else if (call->isPrimitive(PRIM_ENUM_MIN_BITS) || call->isPrimitive(PRIM_ENUM_IS_SIGNED)) {
      EnumType* et = toEnumType(toSymExpr(call->get(1))->var->type);


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

    } else if (call->isPrimitive(PRIM_IS_SYNC_TYPE)) {
      Type* syncType = call->get(1)->typeInfo();
      if (syncType->symbol->hasFlag(FLAG_SYNC))
        result = new SymExpr(gTrue);
      else
        result = new SymExpr(gFalse);
      call->replace(result);
    } else if (call->isPrimitive(PRIM_IS_SINGLE_TYPE)) {
      Type* singleType = call->get(1)->typeInfo();
      if (singleType->symbol->hasFlag(FLAG_SINGLE))
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
      INT_ASSERT(tupleType->symbol->hasFlag(FLAG_TUPLE));
      if (tupleType->symbol->hasFlag(FLAG_STAR_TUPLE))
        result = new SymExpr(gTrue);
      else
        result = new SymExpr(gFalse);
      call->replace(result);
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
    if (VarSymbol* lhs = toVarSymbol(sym->var)) {          \
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
    if (VarSymbol* lhs = toVarSymbol(sym->var)) {          \
      if (lhs->immediate) {                                             \
        if (SymExpr* sym = toSymExpr(call->get(2))) {      \
          if (VarSymbol* rhs = toVarSymbol(sym->var)) {    \
            if (rhs->immediate) {                                       \
              Immediate i3;                                             \
              fold_constant(prim, lhs->immediate, rhs->immediate, &i3); \
              result = new SymExpr(new_ImmediateSymbol(&i3));           \
              call->replace(result);                                    \
            }                                                           \
          }                                                             \
        }                                                               \
      }                                                                 \
    } else if (EnumSymbol* lhs = toEnumSymbol(sym->var)) { \
      if (SymExpr* sym = toSymExpr(call->get(2))) {        \
        if (EnumSymbol* rhs = toEnumSymbol(sym->var)) {    \
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

static void
insertValueTemp(Expr* insertPoint, Expr* actual) {
  if (SymExpr* se = toSymExpr(actual)) {
    if (!se->var->type->refType) {
      VarSymbol* tmp = newTemp("_value_tmp_", se->var->getValType());
      insertPoint->insertBefore(new DefExpr(tmp));
      insertPoint->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_DEREF, se->var)));
      se->var = tmp;
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

        // Return type is a record (which includes array, record, and
        // dist) or a ref counted type that is passed by reference
        (isRecord(fn->retType) ||
         (fn->retType->symbol->hasFlag(FLAG_REF) &&
          isRefCountedType(fn->retType->getValType()))) &&

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
        } else if (fn->retTag == RET_PARAM) {
          USR_FATAL(call, "param function does not resolve to a param symbol");
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
    } else if (call->isPrimitive(PRIM_QUERY_TYPE_FIELD) ||
               call->isPrimitive(PRIM_QUERY_PARAM_FIELD)) {
      SymExpr* classWrap = toSymExpr(call->get(1));
      // Really should be a symExpr
      INT_ASSERT(classWrap);
      AggregateType* ct = toAggregateType(classWrap->var->type);
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
        if (lhs->var->hasFlag(FLAG_MAYBE_PARAM) || lhs->var->isParameter()) {
          if (paramMap.get(lhs->var))
            INT_FATAL(call, "parameter set multiple times");
          VarSymbol* lhsVar = toVarSymbol(lhs->var);
          // We are expecting the LHS to be a var (what else could it be?
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
              if (VarSymbol* rhsVar = toVarSymbol(rhs->var)) {
                if (rhsVar->immediate) {
                  paramMap.put(lhs->var, rhsVar);
                  lhs->var->defPoint->remove();
                  makeNoop(call);
                  set = true;
                }
              }
              if (EnumSymbol* rhsv = toEnumSymbol(rhs->var)) {
                paramMap.put(lhs->var, rhsv);
                lhs->var->defPoint->remove();
                makeNoop(call);
                set = true;
              }
            }
          }
          if (!set && lhs->var->isParameter())
            USR_FATAL(call, "Initializing parameter '%s' to value not known at compile time", lhs->var->name);
        }
        if (!set) {
          if (lhs->var->hasFlag(FLAG_MAYBE_TYPE)) {
            // Add FLAG_TYPE_VARIABLE when relevant
            if (SymExpr* rhs = toSymExpr(call->get(2))) {
              if (rhs->var->hasFlag(FLAG_TYPE_VARIABLE))
                lhs->var->addFlag(FLAG_TYPE_VARIABLE);
            } else if (CallExpr* rhs = toCallExpr(call->get(2))) {
              if (FnSymbol* fn = rhs->isResolved()) {
                if (fn->retTag == RET_TYPE)
                  lhs->var->addFlag(FLAG_TYPE_VARIABLE);
              } else if (rhs->isPrimitive(PRIM_DEREF)) {
                if (isTypeExpr(rhs->get(1)))
                  lhs->var->addFlag(FLAG_TYPE_VARIABLE);
              }
            }
          }
          if (CallExpr* rhs = toCallExpr(call->get(2))) {
            if (rhs->isPrimitive(PRIM_TYPEOF)) {
              lhs->var->addFlag(FLAG_TYPE_VARIABLE);
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
          if (lhs->var->hasFlag(FLAG_EXPR_TEMP) &&
              !lhs->var->hasFlag(FLAG_TYPE_VARIABLE)) {
            if (CallExpr* rhsCall = toCallExpr(call->get(2))) {
              if (requiresImplicitDestroy(rhsCall)) {
                lhs->var->addFlag(FLAG_INSERT_AUTO_COPY);
                lhs->var->addFlag(FLAG_INSERT_AUTO_DESTROY);
              }
            }
          }

          if (isReferenceType(lhs->var->type) ||
              lhs->var->type->symbol->hasFlag(FLAG_REF_ITERATOR_CLASS) ||
              lhs->var->type->symbol->hasFlag(FLAG_ARRAY))
            // Should this conditional include domains, distributions, sync and/or single?
            lhs->var->removeFlag(FLAG_EXPR_TEMP);
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
      if (left && left->var->hasFlag(FLAG_TYPE_VARIABLE)) {
        result = new SymExpr(sym->type->symbol);
        call->replace(result);
      } else if (sym->isParameter()) {
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
          bool is_true = false;
          if (lt->instantiatedFrom == rt)
            is_true = true;
          if (isSubType(lt, rt))
            is_true = true;
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
      if (lhs->var->isParameter() && rhs->var->isParameter()) {
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
      if (lhs->var->isParameter() && rhs->var->isParameter()) {
        const char* lstr = get_string(lhs);
        const char* rstr = get_string(rhs);
        result = new SymExpr(new_StringSymbol(astr(lstr, rstr)));
        call->replace(result);
      }
    } else if (call->isPrimitive("string_length")) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      if (se->var->isParameter()) {
        const char* str = get_string(se);
        result = new SymExpr(new_IntSymbol(strlen(str), INT_SIZE_DEFAULT));
        call->replace(result);
      }
    } else if (call->isPrimitive("ascii")) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      if (se->var->isParameter()) {
        const char* str = get_string(se);
        result = new SymExpr(new_IntSymbol((int)str[0], INT_SIZE_DEFAULT));
        call->replace(result);
      }
    } else if (call->isPrimitive("string_contains")) {
      SymExpr* lhs = toSymExpr(call->get(1));
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(lhs && rhs);
      if (lhs->var->isParameter() && rhs->var->isParameter()) {
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
    } else if (call->isPrimitive(PRIM_ARRAY_ALLOC) ||
               call->isPrimitive(PRIM_SYNC_INIT) ||
               call->isPrimitive(PRIM_SYNC_LOCK) ||
               call->isPrimitive(PRIM_SYNC_UNLOCK) ||
               call->isPrimitive(PRIM_SYNC_WAIT_FULL) ||
               call->isPrimitive(PRIM_SYNC_WAIT_EMPTY) ||
               call->isPrimitive(PRIM_SYNC_SIGNAL_FULL) ||
               call->isPrimitive(PRIM_SYNC_SIGNAL_EMPTY) ||
               call->isPrimitive(PRIM_SINGLE_INIT) ||
               call->isPrimitive(PRIM_SINGLE_LOCK) ||
               call->isPrimitive(PRIM_SINGLE_UNLOCK) ||
               call->isPrimitive(PRIM_SINGLE_WAIT_FULL) ||
               call->isPrimitive(PRIM_SINGLE_SIGNAL_FULL) ||
               call->isPrimitive(PRIM_WRITEEF) ||
               call->isPrimitive(PRIM_WRITEFF) ||
               call->isPrimitive(PRIM_WRITEXF) ||
               call->isPrimitive(PRIM_READFF) ||
               call->isPrimitive(PRIM_READFE) ||
               call->isPrimitive(PRIM_READXX) ||
               call->isPrimitive(PRIM_SYNC_IS_FULL) ||
               call->isPrimitive(PRIM_SINGLE_WRITEEF) ||
               call->isPrimitive(PRIM_SINGLE_READFF) ||
               call->isPrimitive(PRIM_SINGLE_READXX) ||
               call->isPrimitive(PRIM_SINGLE_IS_FULL) ||
               call->isPrimitive(PRIM_EXECUTE_TASKS_IN_LIST) ||
               call->isPrimitive(PRIM_FREE_TASK_LIST) ||
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
    if (Symbol* val = paramMap.get(sym->var)) {
      CallExpr* call = toCallExpr(sym->parentExpr);
      if (call && call->get(1) == sym) {
        // This is a place where param substitution has already determined the
        // value of a move or assignment, so we can just ignore the update.
        if (call->isPrimitive(PRIM_MOVE)) {
          makeNoop(call);
          return result;
        }

        // The substitution usually happens before resolution, so for
        // assignment, we key off of the name :-(
        if (call->isNamed("="))
        {
          makeNoop(call);
          return result;
        }
      }

      if (sym->var->type != dtUnknown && sym->var->type != val->type) {
        CallExpr* cast = new CallExpr("_cast", sym->var, val);
        sym->replace(cast);
        result = preFold(cast);
      } else {
        sym->var = val;
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
          if (se->var == gTryToken)
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

  if (SymExpr* se = toSymExpr(expr)) {
    if (se->var) {
      makeRefType(se->var->type);
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

    // Resolve expressions of the form:  <type> ( args )
    // These will be constructor calls (or type constructor calls) that slipped
    // past normalization due to the use of typedefs.
    if (SymExpr* se = toSymExpr(call->baseExpr)) {
      if (TypeSymbol* ts = toTypeSymbol(se->var)) {
        if (call->numActuals() == 0 ||
            (call->numActuals() == 2 && isSymExpr(call->get(1)) &&
             toSymExpr(call->get(1))->var == gMethodToken)) {
          // This looks like a typedef, so ignore it.

        } else {
          // More needed here ... .
          INT_FATAL(ts, "not yet implemented.");
        }
      }
    }

    callStack.add(call);

    resolveCall(call);

    if (!tryFailure && call->isResolved()) {
      if (CallExpr* origToLeaderCall = toPrimToLeaderCall(origExpr))
        // ForallLeaderArgs: process the leader that 'call' invokes.
        implementForallIntents2(call, origToLeaderCall);

      resolveFns(call->isResolved());
    }

    if (tryFailure) {
      if (tryStack.tail()->parentSymbol == fn) {
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
        !sym->var->hasFlag(FLAG_TYPE_VARIABLE)) {

      if (AggregateType* ct = toAggregateType(sym->typeInfo())) {
        if (!ct->symbol->hasFlag(FLAG_GENERIC) &&
            !ct->symbol->hasFlag(FLAG_ITERATOR_CLASS) &&
            !ct->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
          resolveFormals(ct->defaultTypeConstructor);
          if (resolvedFormals.set_in(ct->defaultTypeConstructor)) {
            if (ct->defaultTypeConstructor->hasFlag(FLAG_PARTIAL_COPY))
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
        if (sym->var == retSymbol) {
          if (SymExpr* sym = toSymExpr(call->get(2)))
            retParams.add(sym->var);
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
replaceSetterArgWithTrue(BaseAST* ast, FnSymbol* fn) {
  if (SymExpr* se = toSymExpr(ast)) {
    if (se->var == fn->setter->sym) {
      se->var = gTrue;
      if (fn->isIterator())
        USR_WARN(fn, "setter argument is not supported in iterators");
    }
  }
  AST_CHILDREN_CALL(ast, replaceSetterArgWithTrue, fn);
}


static void
replaceSetterArgWithFalse(BaseAST* ast, FnSymbol* fn, Symbol* ret) {
  if (SymExpr* se = toSymExpr(ast)) {
    if (se->var == fn->setter->sym)
      se->var = gFalse;
    else if (se->var == ret) {
      if (CallExpr* move = toCallExpr(se->parentExpr))
        if (move->isPrimitive(PRIM_MOVE))
          if (CallExpr* call = toCallExpr(move->get(2)))
            if (call->isPrimitive(PRIM_ADDR_OF))
              call->primitive = primitives[PRIM_DEREF];
    }
  }
  AST_CHILDREN_CALL(ast, replaceSetterArgWithFalse, fn, ret);
}


static void
insertCasts(BaseAST* ast, FnSymbol* fn, Vec<CallExpr*>& casts) {
  if (CallExpr* call = toCallExpr(ast)) {
    if (call->parentSymbol == fn) {
      if (call->isPrimitive(PRIM_MOVE)) {
        if (SymExpr* lhs = toSymExpr(call->get(1))) {
          Type* lhsType = lhs->var->type;
          if (lhsType != dtUnknown) {
          Expr* rhs = call->get(2);
          Type* rhsType = rhs->typeInfo();
          if (rhsType != lhsType &&
              rhsType->refType != lhsType &&
              rhsType != lhsType->refType) {
            SET_LINENO(rhs);
            rhs->remove();
            Symbol* tmp = NULL;
            if (SymExpr* se = toSymExpr(rhs)) {
              tmp = se->var;
            } else {
              tmp = newTemp("_cast_tmp_", rhs->typeInfo());
              call->insertBefore(new DefExpr(tmp));
              call->insertBefore(new CallExpr(PRIM_MOVE, tmp, rhs));
            }
            CallExpr* cast = new CallExpr("_cast", lhsType->symbol, tmp);
            call->insertAtTail(cast);
            casts.add(cast);
          }
          }
        }
      }
    }
  }
  AST_CHILDREN_CALL(ast, insertCasts, fn, casts);
}


static void instantiate_default_constructor(FnSymbol* fn) {
  //
  // instantiate initializer
  //
  if (fn->instantiatedFrom) {
    INT_ASSERT(!fn->retType->defaultInitializer);
    FnSymbol* instantiatedFrom = fn->instantiatedFrom;
    while (instantiatedFrom->instantiatedFrom)
      instantiatedFrom = instantiatedFrom->instantiatedFrom;
    CallExpr* call = new CallExpr(instantiatedFrom->retType->defaultInitializer);
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
    fn->insertBeforeReturn(call);
    resolveCall(call);
    fn->retType->defaultInitializer = call->isResolved();
    INT_ASSERT(fn->retType->defaultInitializer);
    //      resolveFns(fn->retType->defaultInitializer);
    call->remove();
  }
}


static void buildValueFunction(FnSymbol* fn) {
  if (!fn->isIterator()) {
    FnSymbol* copy;
    bool valueFunctionExists = fn->valueFunction;
    if (!valueFunctionExists) {
      // Build the value function when it does not already exist.
      copy = fn->copy();
      copy->removeFlag(FLAG_RESOLVED);
      copy->addFlag(FLAG_INVISIBLE_FN);
      if (fn->hasFlag(FLAG_NO_IMPLICIT_COPY))
        copy->addFlag(FLAG_NO_IMPLICIT_COPY);
      copy->retTag = RET_VALUE;   // Change ret flag to value (not ref).
      fn->defPoint->insertBefore(new DefExpr(copy));
      fn->valueFunction = copy;
      Symbol* ret = copy->getReturnSymbol();
      replaceSetterArgWithFalse(copy, copy, ret);
      replaceSetterArgWithTrue(fn, fn);
    } else {
      copy = fn->valueFunction;
    }
    resolveFns(copy);
  } else {
    replaceSetterArgWithTrue(fn, fn);
  }
}


static void resolveReturnType(FnSymbol* fn)
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

  ret->type = retType;
  if (!fn->iteratorInfo) {
    if (retType == dtUnknown)
      USR_FATAL(fn, "unable to resolve return type");
    fn->retType = retType;
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
static bool isLeaderIterator(FnSymbol* fn) {
  return isIteratorOfType(fn, gLeaderTag);
}
static bool isFollowerIterator(FnSymbol* fn) {
  return isIteratorOfType(fn, gFollowerTag);
}
static bool isStandaloneIterator(FnSymbol* fn) {
  return isIteratorOfType(fn, gStandaloneTag);
}

void
resolveFns(FnSymbol* fn) {
  if (fn->isResolved())
    return;

  fn->addFlag(FLAG_RESOLVED);

  if (fn->hasFlag(FLAG_EXTERN)) {
    resolveBlockStmt(fn->body);
    resolveReturnType(fn);
    return;
  }

  if (fn->hasFlag(FLAG_FUNCTION_PROTOTYPE))
    return;

  //
  // Mark serial loops that yield inside of follower and standalone iterators
  // as order independent. By using a forall loop, a user is asserting that
  // their loop is order independent. Here we just mark the serial loops inside
  // of the "follower" with this information. Note that only loops that yield
  // are marked since other loops are not necessarily order independent. Only
  // the inner most loop of a loop nest will be marked.
  //
  if (isFollowerIterator(fn) || isStandaloneIterator(fn)) {
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

  if (fn->retTag == RET_REF) {
    buildValueFunction(fn);
  }

  insertFormalTemps(fn);

  resolveBlockStmt(fn->body);

  if (tryFailure) {
    fn->removeFlag(FLAG_RESOLVED);
    return;
  }

  if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)) {
    AggregateType* ct = toAggregateType(fn->retType);
    if (!ct)
      INT_FATAL(fn, "Constructor has no class type");
    setScalarPromotionType(ct);
    fixTypeNames(ct);
  }

  resolveReturnType(fn);

  //
  // insert casts as necessary
  //
  if (fn->retTag != RET_PARAM) {
    Vec<CallExpr*> casts;
    insertCasts(fn->body, fn, casts);
    forv_Vec(CallExpr, cast, casts) {
      resolveCall(cast);
      if (cast->isResolved()) {
        resolveFns(cast->isResolved());
      }
    }
  }

  if (fn->isIterator() && !fn->iteratorInfo) {
    protoIteratorClass(fn);
  }

  // Resolve base class type constructors as well.
  if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)) {
    forv_Vec(Type, parent, fn->retType->dispatchParents) {
      if (toAggregateType(parent) && parent != dtValue && parent != dtObject && parent->defaultTypeConstructor) {
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

    // This instantiates the default constructor for the corresponding type constructor.
    instantiate_default_constructor(fn);

    //
    // resolve destructor
    //
    if (AggregateType* ct = toAggregateType(fn->retType)) {
      if (!ct->destructor &&
          !ct->symbol->hasFlag(FLAG_REF)) {
        VarSymbol* tmp = newTemp(ct);
        CallExpr* call = new CallExpr("~chpl_destroy", gMethodToken, tmp);

        // In case resolveCall drops other stuff into the tree ahead of the
        // call, we wrap everything in a block for safe removal.
        BlockStmt* block = new BlockStmt();
        block->insertAtHead(call);

        fn->insertAtHead(block);
        fn->insertAtHead(new DefExpr(tmp));
        resolveCall(call);

        resolveFns(call->isResolved());
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
          ct->defaultTypeConstructor)
        icts.add(ts->type);
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


static void
addToVirtualMaps(FnSymbol* pfn, AggregateType* ct) {
  forv_Vec(FnSymbol, cfn, ct->methods) {
    if (cfn && !cfn->instantiatedFrom && possible_signature_match(pfn, cfn)) {
      Vec<Type*> types;
      if (ct->symbol->hasFlag(FLAG_GENERIC))
        collectInstantiatedAggregateTypes(types, ct);
      else
        types.add(ct);

      forv_Vec(Type, type, types) {
        SymbolMap subs;
        if (ct->symbol->hasFlag(FLAG_GENERIC))
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
          fn = instantiate(fn, subs, NULL);
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
          if (signature_match(pfn, fn)) {
            resolveFns(fn);
            if (fn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD) &&
                pfn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
              if (!isSubType(fn->retType->defaultInitializer->iteratorInfo->getValue->retType,
                  pfn->retType->defaultInitializer->iteratorInfo->getValue->retType)) {
                USR_FATAL_CONT(pfn, "conflicting return type specified for '%s: %s'", toString(pfn),
                               pfn->retType->defaultInitializer->iteratorInfo->getValue->retType->symbol->name);
                USR_FATAL_CONT(fn, "  overridden by '%s: %s'", toString(fn),
                               fn->retType->defaultInitializer->iteratorInfo->getValue->retType->symbol->name);
                USR_STOP();
              } else {
                pfn->retType->dispatchChildren.add_exclusive(fn->retType);
                fn->retType->dispatchParents.add_exclusive(pfn->retType);
                Type* pic = pfn->retType->defaultInitializer->iteratorInfo->iclass;
                Type* ic = fn->retType->defaultInitializer->iteratorInfo->iclass;
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


static void
addAllToVirtualMaps(FnSymbol* fn, AggregateType* ct) {
  forv_Vec(Type, t, ct->dispatchChildren) {
    AggregateType* ct = toAggregateType(t);
    if (ct->defaultTypeConstructor &&
        (ct->defaultTypeConstructor->hasFlag(FLAG_GENERIC) ||
         ct->defaultTypeConstructor->isResolved()))
      addToVirtualMaps(fn, ct);

    if (!ct->instantiatedFrom)
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
      // Parenthesesless functions are statically bound; that is, they are not
      // dispatched through the virtual table.
      continue;

    if (fn->retTag == RET_PARAM || fn->retTag == RET_TYPE)
      // Only run-time functions populate the virtual map.
      continue;

    if (fn->numFormals() > 1 && fn->getFormal(1)->type == dtMethodToken) {
      // Only methods go in the virtual function table.
      if (AggregateType* pt = toAggregateType(fn->getFormal(2)->type)) {
        if (isClass(pt) && !pt->symbol->hasFlag(FLAG_GENERIC)) {
          addAllToVirtualMaps(fn, pt);
        }
      }
    }
  }
}


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


static void resolveTypedefedArgTypes(FnSymbol* fn)
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
        if (se->var->hasFlag(FLAG_TYPE_VARIABLE))
        {
          Type* type = resolveTypeAlias(toSymExpr(se));
          INT_ASSERT(type);
          formal->type = type;
        }
      }
    }
  }
}


static void
computeStandardModuleSet() {
  standardModuleSet.set_add(rootModule->block);
  standardModuleSet.set_add(theProgram->block);

  Vec<ModuleSymbol*> stack;
  stack.add(standardModule);

  while (ModuleSymbol* mod = stack.pop()) {
    if (mod->block->modUses) {
      for_actuals(expr, mod->block->modUses) {
        SymExpr* se = toSymExpr(expr);
        INT_ASSERT(se);
        ModuleSymbol* use = toModuleSymbol(se->var);
        INT_ASSERT(use);
        if (!standardModuleSet.set_in(use->block)) {
          stack.add(use);
          standardModuleSet.set_add(use->block);
        }
      }
    }
  }
}


void
resolve() {
  parseExplainFlag(fExplainCall, &explainCallLine, &explainCallModule);

  computeStandardModuleSet();

  // call _nilType nil so as to not confuse the user
  dtNil->symbol->name = gNil->name;

  bool changed = true;
  while (changed) {
    changed = false;
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      changed = fn->tag_generic() || changed;
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

  USR_STOP();

  resolveExports();
  resolveEnumTypes();
  resolveDynamicDispatches();

  insertRuntimeTypeTemps();

  resolveAutoCopies();
  resolveRecordInitializers();
  resolveOther();

  insertDynamicDispatchCalls();
  insertReturnTemps();

  handleRuntimeTypes();

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

  if (fPrintModuleResolution)
    putc('\n', stderr);

  --module_resolution_depth;
}

static void resolveExports() {
  // We need to resolve any additional functions that will be exported.
  forv_Vec(FnSymbol, fn, gFnSymbols) {
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
      ensureEnumTypeResolved(et);
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

  for (int i = 0; i < virtualMethodTable.n; i++) {
    if (virtualMethodTable.v[i].key) {
      virtualMethodTable.v[i].value->reverse();
      for (int j = 0; j < virtualMethodTable.v[i].value->n; j++) {
        virtualMethodMap.put(virtualMethodTable.v[i].value->v[j], j);
      }
    }
  }
  inDynamicDispatchResolution = false;

  if (fPrintDispatch) {
    printf("Dynamic dispatch table:\n");
    for (int i = 0; i < virtualMethodTable.n; i++) {
      if (Type* t = virtualMethodTable.v[i].key) {
        printf("  %s\n", toString(t));
        for (int j = 0; j < virtualMethodTable.v[i].value->n; j++) {
          printf("    %s\n", toString(virtualMethodTable.v[i].value->v[j]));
        }
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
      ts->type->defaultInitializer->insertBeforeReturn(new DefExpr(tmp));
      CallExpr* call = new CallExpr("chpl__convertValueToRuntimeType", tmp);
      ts->type->defaultInitializer->insertBeforeReturn(call);
      resolveCall(call);
      resolveFns(call->isResolved());
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

    if (isRecord(ts->type) || getSyncFlags(ts).any())
    {
      resolveAutoCopy(ts->type);
      resolveAutoDestroy(ts->type);
    }
  }
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
      makeNoop((CallExpr *)init->parentExpr);
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
      CallExpr* classCall = new CallExpr(type->getField("_valueType")->type->defaultInitializer);
      CallExpr* move = new CallExpr(PRIM_MOVE, tmp, classCall);
      init->getStmtExpr()->insertBefore(move);
      resolveCall(classCall);
      resolveFns(classCall->isResolved());
      resolveCall(move);
      CallExpr* distCall = new CallExpr("chpl__buildDistValue", tmp);
      init->replace(distCall);
      resolveCall(distCall);
      resolveFns(distCall->isResolved());
    } else {
      CallExpr* call = new CallExpr("_defaultOf", type->symbol);
      init->replace(call);
      resolveNormalCall(call);
      // At this point in the compiler, we can resolve the _defaultOf function
      // for the type, so do so.
      if (call->isResolved())
        resolveFns(call->isResolved());
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

    //Check to see if any of the overridden methods reference outer variables.  If they do, then when we later change the
    //signature in flattenFunctions, the vtable style will break (function signatures will no longer match).  To avoid this
    //we switch to the if-block style in the case where outer variables are discovered.
    //Note: This is conservative, as we haven't finished resolving functions and calls yet, we check all possibilities.

    bool referencesOuterVars = false;

    Vec<FnSymbol*> seen;
    referencesOuterVars = usesOuterVars(key, seen);

    if (!referencesOuterVars) {
      for (int i = 0; i < fns->n; ++i) {
        seen.clear();
        if ( (referencesOuterVars = usesOuterVars(key, seen)) ) {
          break;
        }
      }
    }

    if ((fns->n + 1 > fConditionalDynamicDispatchLimit) && (!referencesOuterVars)) {
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
        ifBlock->insertAtTail(new CallExpr(PRIM_MOVE, cid,
                                new CallExpr(PRIM_TESTCID,
                                             call->get(2)->copy(),
                                             type->symbol)));
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
          se->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_CAST, type->symbol, se->var)));
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
          CallExpr* parent = toCallExpr(call->parentExpr);
          if (!parent && !isDefExpr(call->parentExpr)) { // no use
            SET_LINENO(call); // TODO: reset_ast_loc() below?
            VarSymbol* tmp = newTemp("_return_tmp_", fn->retType);
            DefExpr* def = new DefExpr(tmp);
            call->insertBefore(def);
            if (!fMinimalModules &&
                ((fn->retType->getValType() &&
                  isSyncType(fn->retType->getValType())) ||
                 isSyncType(fn->retType) ||
                 fn->isIterator())) {
              CallExpr* sls = new CallExpr("_statementLevelSymbol", tmp);
              call->insertBefore(sls);
              reset_ast_loc(sls, call);
              resolveCall(sls);
              INT_ASSERT(sls->isResolved());
              resolveFns(sls->isResolved());
            }
            def->insertAfter(new CallExpr(PRIM_MOVE, tmp, call->remove()));
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

  removeUnusedGlobals();
  removeUnusedTypes();
  removeActualNames();
  removeFormalTypeAndInitBlocks();
  removeTypeBlocks();
  removeInitFields();
  removeWhereClauses();
  removeMootFields();
  expandInitFieldPrims();
  removeCompilerWarnings();
}

static void clearDefaultInitFns(FnSymbol* unusedFn) {
  // Before removing an unused function, check if it is a defaultInitializer.
  // If unusedFn is a defaultInitializer, its retType's defaultInitializer
  // field will be unusedFn. Set the defaultInitializer field to NULL so the
  // removed function doesn't leave behind a garbage pointer.
  if (unusedFn->retType->defaultInitializer == unusedFn) {
    unusedFn->retType->defaultInitializer = NULL;
  }
}

static void removeUnusedFunctions() {
  // Remove unused functions
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_PRINT_MODULE_INIT_FN)) continue;
    if (fn->defPoint && fn->defPoint->parentSymbol) {
      if (! fn->isResolved() || fn->retTag == RET_PARAM) {
        clearDefaultInitFns(fn);
        fn->defPoint->remove();
      }
    }
  }
}

static void removeCompilerWarnings() {
  // Warnings have now been issued, no need to keep the function around.
  // Remove calls to compilerWarning and let dead code elimination handle
  // the rest.
  typedef MapElem<FnSymbol*, const char*> FnSymbolElem;
  form_Map(FnSymbolElem, el, innerCompilerWarningMap) {
    forv_Vec(CallExpr, call, *(el->key->calledBy)) {
      call->remove();
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

  // FALSE if initializers are used
  if (ct->defaultInitializer && ct->defaultInitializer->isResolved())
    return false;

  // FALSE if the type constructor is used.
  if (ct->defaultTypeConstructor && ct->defaultTypeConstructor->isResolved())
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

static void removeUnusedGlobals()
{
  forv_Vec(DefExpr, def, gDefExprs)
  {
    // Remove unused global variables
    if (toVarSymbol(def->sym))
      if (toModuleSymbol(def->parentSymbol))
        if (def->sym->type == dtUnknown)
          def->remove();
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
      SymExpr* sym = toSymExpr(call->get(2));
      if (sym && isTypeSymbol(sym->var))
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

  // Replace the body of the orignal chpl__buildRuntime...Type() function.
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

    // We do not remove type args from extern functions
    // TODO: Find out if we really support type args in extern functions.
    if (fn->hasFlag(FLAG_EXTERN))
      continue;

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
        Type* rt = se->var->type;

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
            call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp,
                                                           new CallExpr(PRIM_GET_MEMBER_VALUE, se->var, field)));
            if (formal->hasFlag(FLAG_TYPE_VARIABLE))
              tmp->addFlag(FLAG_TYPE_VARIABLE);
            runtimeTypeToValueCall->insertAtTail(tmp);
          }
          VarSymbol* tmp = newTemp("_runtime_type_tmp_", runtimeTypeToValueFn->retType);
          call->getStmtExpr()->insertBefore(new DefExpr(tmp));
          call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, runtimeTypeToValueCall));
          INT_ASSERT(autoCopyMap.get(tmp->type));
          call->replace(new CallExpr(autoCopyMap.get(tmp->type), tmp));
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
        if (se->var->hasFlag(FLAG_TYPE_VARIABLE))
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
    if (! def->sym->hasFlag(FLAG_TYPE_VARIABLE)) continue;
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
      initializeClass(call, toSymExpr(call->get(1))->var);
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
    ct->symbol->name = ct->getField("_valueType")->type->symbol->name;
  }
}


static void
setScalarPromotionType(AggregateType* ct) {
  for_fields(field, ct) {
    if (!strcmp(field->name, "_promotionType"))
      ct->scalarPromotionType = field->type;
  }
}
