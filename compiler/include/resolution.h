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

#ifndef _RESOLUTION_H_
#define _RESOLUTION_H_

#include "baseAST.h"
#include "symbol.h"
#include <vector>
#include <map>

class CallInfo;

extern SymbolMap       paramMap;

extern Vec<CallExpr*>  callStack;
extern Vec<CondStmt*>  tryStack;

extern Vec<CallExpr*>  inits;

extern Vec<BlockStmt*> standardModuleSet;

extern char            arrayUnrefName[];

bool hasAutoCopyForType(Type* type);
FnSymbol* getAutoCopyForType(Type* type);
void getAutoCopyTypeKeys(Vec<Type*> &keys); // type to chpl__autoCopy function
extern Map<Type*,FnSymbol*> autoDestroyMap; // type to chpl__autoDestroy function
extern Map<FnSymbol*,FnSymbol*> iteratorLeaderMap;
extern Map<FnSymbol*,FnSymbol*> iteratorFollowerMap;
extern std::map<CallExpr*,CallExpr*> eflopiMap;

bool       propagateNotPOD(Type* t);

Expr*      resolvePrimInit(CallExpr* call);

bool       isTupleContainingOnlyReferences(Type* t);

void       ensureEnumTypeResolved(EnumType* etype);

void       resolveFnForCall(FnSymbol* fn, CallExpr* call);

bool       canInstantiate(Type* actualType, Type* formalType);

bool       isInstantiation(Type* sub, Type* super);

// explain call stuff
extern int explainCallLine;
bool explainCallMatch(CallExpr* call);

FnSymbol* requiresImplicitDestroy(CallExpr* call);
bool isLeaderIterator(FnSymbol* fn);
bool isStandaloneIterator(FnSymbol* fn);

bool isDispatchParent(Type* t, Type* pt);

bool canCoerce(Type* actualType, Symbol* actualSym, Type* formalType, FnSymbol* fn, bool* promotes = NULL);
bool canDispatch(Type* actualType, Symbol* actualSym, Type* formalType, FnSymbol* fn = NULL, bool* promotes = NULL, bool paramCoerce = false);

bool fixupDefaultInitCopy(FnSymbol* fn, FnSymbol* newFn, CallExpr* call);

const char* toString(Type* type);
const char* toString(CallInfo* info);
const char* toString(FnSymbol* fn);

void parseExplainFlag(char* flag, int* line, ModuleSymbol** module);

FnSymbol* getTheIteratorFn(Symbol* ic);
FnSymbol* getTheIteratorFn(CallExpr* call);
FnSymbol* getTheIteratorFn(Type* icType);

// forall intents
void implementForallIntents1(DefExpr* defChplIter);
void implementForallIntents2(CallExpr* call, CallExpr* origToLeaderCall);
void implementForallIntents2wrapper(CallExpr* call, CallExpr* origToLeaderCall);
void stashPristineCopyOfLeaderIter(FnSymbol* origLeader, bool ignore_isResolved);

// reduce intents
void cleanupRedRefs(Expr*& redRef1, Expr*& redRef2);
void setupRedRefs(FnSymbol* fn, bool nested, Expr*& redRef1, Expr*& redRef2);
bool isReduceOp(Type* type);

FnSymbol* instantiate(FnSymbol* fn, SymbolMap& subs);
FnSymbol* instantiateSignature(FnSymbol* fn, SymbolMap& subs, CallExpr* call);
void      instantiateBody(FnSymbol* fn);

// generics support
TypeSymbol* getNewSubType(FnSymbol* fn, Symbol* key, TypeSymbol* actualTS);
void checkInfiniteWhereInstantiation(FnSymbol* fn);
void renameInstantiatedTypeString(TypeSymbol* sym, VarSymbol* var);
FnSymbol* determineRootFunc(FnSymbol* fn);
void determineAllSubs(FnSymbol* fn, FnSymbol* root, SymbolMap& subs,
                      SymbolMap& all_subs);
FnSymbol* instantiateFunction(FnSymbol* fn, FnSymbol* root, SymbolMap& all_subs,
                              CallExpr* call, SymbolMap& subs, SymbolMap& map);
void explainAndCheckInstantiation(FnSymbol* newFn, FnSymbol* fn);

// disambiguation
/** A wrapper for candidates for function call resolution.
 *
 * If a best candidate was found than the function member will point to it.
 */
class ResolutionCandidate {
public:
  /// A pointer to the best candidate function.
  FnSymbol* fn;

  /** The actual arguments for the candidate, aligned so that they have the same
   *  index as their corresponding formal argument in the called function.
   */
  Vec<Symbol*> formalIdxToActual; // note: was alignedActuals

  /** The formal arguments for the candidate, aligned so that they have the same
   *  index as their corresponding actual argument in the call.
   */
  Vec<ArgSymbol*> actualIdxToFormal; // note: was alignedFormals

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
  void computeSubstitutions(bool inInitRes = false);
};

void explainGatherCandidate(Vec<ResolutionCandidate*>& candidates,
                            CallInfo& info, CallExpr* call);
void wrapAndCleanUpActuals(ResolutionCandidate* best, CallInfo& info,
                           bool buildFastFollowerChecks);

typedef enum {
  FIND_EITHER = 0,
  FIND_REF,
  FIND_CONST_REF,
  FIND_NOT_REF_OR_CONST_REF, // !(ref || const_ref)
} disambiguate_kind_t;


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


ResolutionCandidate* disambiguateByMatch(Vec<ResolutionCandidate*>& candidates,
                                         DisambiguationContext DC,
                                         disambiguate_kind_t kind);
bool isBetterMatch(ResolutionCandidate* candidate1,
                   ResolutionCandidate* candidate2,
                   const DisambiguationContext& DC,
                   bool onlyConsiderPromotion);

// Regular resolve functions
void      resolveFormals(FnSymbol* fn);
void      resolveBlockStmt(BlockStmt* blockStmt);
void      resolveCall(CallExpr* call);
void      resolveCallAndCallee(CallExpr* call, bool allowUnresolved = false);
void      resolveFns(FnSymbol* fn);
void      resolveDefaultGenericType(CallExpr* call);
void      resolveReturnType(FnSymbol* fn);
Type*     resolveTypeAlias(SymExpr* se);

FnSymbol* tryResolveCall(CallExpr* call);
void      makeRefType(Type* type);

// FnSymbol changes
extern bool tryFailure;
void insertFormalTemps(FnSymbol* fn);
void insertAndResolveCasts(FnSymbol* fn);
void ensureInMethodList(FnSymbol* fn);

FnSymbol* defaultWrap(FnSymbol* fn, Vec<ArgSymbol*>* actualFormals,  CallInfo* info);
void reorderActuals(FnSymbol* fn, Vec<ArgSymbol*>* actualFormals,  CallInfo* info);
void coerceActuals(FnSymbol* fn, CallInfo* info);
FnSymbol* promotionWrap(FnSymbol* fn, CallInfo* info, bool buildFastFollowerChecks);

FnSymbol* getAutoCopy(Type* t);
FnSymbol* getAutoDestroy(Type* t);
FnSymbol* getUnalias(Type* t);


bool isPOD(Type* t);

// resolution errors and warnings
void printResolutionErrorAmbiguous(Vec<FnSymbol*>& candidates, CallInfo* info);
void printResolutionErrorUnresolved(Vec<FnSymbol*>& visibleFns, CallInfo* info);
void resolveNormalCallCompilerWarningStuff(FnSymbol* resolvedFn);
void lvalueCheck(CallExpr* call);
void checkForStoringIntoTuple(CallExpr* call, FnSymbol* resolvedFn);
void printTaskOrForallConstErrorNote(Symbol* aVar);

// tuples
FnSymbol* createTupleSignature(FnSymbol* fn, SymbolMap& subs, CallExpr* call);
// returns true if the function was handled
bool fixupTupleFunctions(FnSymbol* fn, FnSymbol* newFn, CallExpr* call);
AggregateType* computeNonRefTuple(Type* t);
AggregateType* computeTupleWithIntent(IntentTag intent, Type* t);

bool evaluateWhereClause(FnSymbol* fn);


bool isAutoDestroyedVariable(Symbol* sym);

#endif
