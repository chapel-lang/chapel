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

#ifndef _RESOLUTION_H_
#define _RESOLUTION_H_

#include "baseAST.h"
#include "symbol.h"

#include <map>
#include <vector>

class CallInfo;
class ResolutionCandidate;

struct Serializers {
  FnSymbol* serializer;
  FnSymbol* deserializer;
  FnSymbol* broadcaster;
  FnSymbol* destroyer;
};


extern bool                             beforeLoweringForallStmts;

extern int                              explainCallLine;

extern SymbolMap                        paramMap;

extern Vec<CallExpr*>                   callStack;

extern bool                             tryFailure;

extern Vec<CallExpr*>                   inits;

extern Vec<BlockStmt*>                  standardModuleSet;

extern char                             arrayUnrefName[];

extern Map<Type*,     FnSymbol*>        autoDestroyMap;

extern Map<FnSymbol*, FnSymbol*>        iteratorLeaderMap;

extern Map<FnSymbol*, FnSymbol*>        iteratorFollowerMap;

extern Map<Type*,     FnSymbol*>        valueToRuntimeTypeMap;

extern std::map<Type*,     Serializers> serializeMap;

extern std::map<CallExpr*, CallExpr*>   eflopiMap;





bool       propagateNotPOD(Type* t);

Expr*      resolvePrimInit(CallExpr* call);

bool       isTupleContainingOnlyReferences(Type* t);

void       ensureEnumTypeResolved(EnumType* etype);

void       resolveFnForCall(FnSymbol* fn, CallExpr* call);

bool       canInstantiate(Type* actualType, Type* formalType);

Type*      getConcreteParentForGenericFormal(Type* actualType,
                                             Type* formalType);

bool       isInstantiation(Type* sub, Type* super);

bool       formalRequiresTemp(ArgSymbol* formal, FnSymbol* fn);

// If formalRequiresTemp(formal,fn), when this function returns true,
// the new strategy of making the temporary at the call site will be used.
// (if it returns false, the temporary will be inside fn)
bool       shouldAddFormalTempAtCallSite(ArgSymbol* formal, FnSymbol* fn);

// This function concerns an initialization expression such as:
//   var x = <expr>;
// It returns `true` if <expr> should be copy-initialized and `false` if
// ownership can be transferred.
//
// In particular:
//  var y = ...;
//  var x = y;   // requires a copy-init
//  var z = functionReturningRecordByValue(); // does not require a copy-init
bool       doesCopyInitializationRequireCopy(Expr* initFrom);

// Similar to the above, but it's OK to return a local value
// variable without a copy.
bool       doesValueReturnRequireCopy(Expr* initFrom);

// explain call stuff
bool explainCallMatch(CallExpr* call);

bool isDispatchParent(Type* t, Type* pt);

bool canCoerce(Type*     actualType,
               Symbol*   actualSym,
               Type*     formalType,
               FnSymbol* fn,
               bool*     promotes = NULL,
               bool*     paramNarrows = NULL);

bool canDispatch(Type*     actualType,
                 Symbol*   actualSym,
                 Type*     formalType,
                 FnSymbol* fn          = NULL,
                 bool*     promotes    = NULL,
                 bool*     paramNarrows= NULL,
                 bool      paramCoerce = false);


void parseExplainFlag(char* flag, int* line, ModuleSymbol** module);

FnSymbol* findCopyInit(AggregateType* ct);

FnSymbol* getTheIteratorFn(Symbol* ic);
FnSymbol* getTheIteratorFn(CallExpr* call);
FnSymbol* getTheIteratorFn(Type* icType);
FnSymbol* getTheIteratorFnFromIteratorRec(Type* irType);

// forall intents
CallExpr* resolveForallHeader(ForallStmt* pfs, SymExpr* origSE);
void implementForallIntents1(DefExpr* defChplIter);
void implementForallIntents2(CallExpr* call, CallExpr* origToLeaderCall);
void implementForallIntents2wrapper(CallExpr* call, CallExpr* origToLeaderCall);
void implementForallIntentsNew(ForallStmt* fs, CallExpr* parCall);
void stashPristineCopyOfLeaderIter(FnSymbol* origLeader, bool ignoreIsResolved);

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

void determineAllSubs(FnSymbol*  fn,
                      FnSymbol*  root,
                      SymbolMap& subs,
                      SymbolMap& allSubs);

FnSymbol* instantiateFunction(FnSymbol*  fn,
                              FnSymbol*  root,
                              SymbolMap& allSubs,
                              CallExpr*  call,
                              SymbolMap& subs,
                              SymbolMap& map);

void explainAndCheckInstantiation(FnSymbol* newFn, FnSymbol* fn);

class DisambiguationContext {
public:
                 DisambiguationContext(CallInfo& info);

  Vec<Symbol*>*  actuals;
  Expr*          scope;
  bool           explain;

private:
                 DisambiguationContext();
};


ResolutionCandidate*
disambiguateForInit(CallInfo&                    info,
                    Vec<ResolutionCandidate*>&   candidates);

// Regular resolve functions
void      resolveBlockStmt(BlockStmt* blockStmt);
void      resolveCall(CallExpr* call);
void      resolveCallAndCallee(CallExpr* call, bool allowUnresolved = false);

Type*     resolveDefaultGenericTypeSymExpr(SymExpr* se);
Type*     resolveTypeAlias(SymExpr* se);

FnSymbol* tryResolveCall(CallExpr* call);
void      makeRefType(Type* type);

// FnSymbol changes
void      insertFormalTemps(FnSymbol* fn);
void      insertAndResolveCasts(FnSymbol* fn);
void      ensureInMethodList(FnSymbol* fn);


bool      doNotChangeTupleTypeRefLevel(FnSymbol* fn, bool forRet);

bool      hasAutoCopyForType(Type* type);
FnSymbol* getAutoCopyForType(Type* type);   // requires hasAutoCopyForType()==true
void      getAutoCopyTypeKeys(Vec<Type*>& keys);
FnSymbol* getAutoCopy(Type* t);             // returns NULL if there are none
FnSymbol* getAutoDestroy(Type* t);          //  "
FnSymbol* getUnalias(Type* t);

Expr*     resolveExpr(Expr* expr);



bool isPOD(Type* t);

// resolution errors and warnings
void printResolutionErrorUnresolved(CallInfo&                  info,
                                    Vec<FnSymbol*>&            visibleFns);

void printResolutionErrorAmbiguous (CallInfo&                  info,
                                    Vec<ResolutionCandidate*>& candidates);

FnSymbol* resolveNormalCall(CallExpr* call, bool checkonly=false);

void      resolveNormalCallCompilerWarningStuff(FnSymbol* resolvedFn);

void lvalueCheck(CallExpr* call);

void checkForStoringIntoTuple(CallExpr* call, FnSymbol* resolvedFn);

bool signatureMatch(FnSymbol* fn, FnSymbol* gn);

void printTaskOrForallConstErrorNote(Symbol* aVar);

// tuples
FnSymbol* createTupleSignature(FnSymbol* fn, SymbolMap& subs, CallExpr* call);

// returns true if the function was handled
bool fixupTupleFunctions(FnSymbol* fn, FnSymbol* newFn, CallExpr* call);

AggregateType* computeNonRefTuple(AggregateType* t);

AggregateType* computeTupleWithIntent(IntentTag intent, AggregateType* t);

bool evaluateWhereClause(FnSymbol* fn);

bool isAutoDestroyedVariable(Symbol* sym);

SymExpr* findSourceOfYield(CallExpr* yield);

void resolveTypeWithInitializer(AggregateType* at, FnSymbol* fn);

void resolvePromotionType(AggregateType* at);

void resolveDestructor(AggregateType* at);

void fixTypeNames(AggregateType* at);

#endif
