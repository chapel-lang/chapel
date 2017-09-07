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
class ResolutionCandidate;

extern bool                           beforeLoweringForallStmts;
extern int                            explainCallLine;

extern SymbolMap                      paramMap;

extern Vec<CallExpr*>                 callStack;

extern bool                           tryFailure;

extern Vec<CallExpr*>                 inits;

extern Vec<BlockStmt*>                standardModuleSet;

extern char                           arrayUnrefName[];

extern Map<Type*,     FnSymbol*>      autoDestroyMap;
extern Map<FnSymbol*, FnSymbol*>      iteratorLeaderMap;
extern Map<FnSymbol*, FnSymbol*>      iteratorFollowerMap;

extern std::map<CallExpr*, CallExpr*> eflopiMap;





bool       hasAutoCopyForType(Type* type);

FnSymbol*  getAutoCopyForType(Type* type);

void       getAutoCopyTypeKeys(Vec<Type*>& keys);

bool       propagateNotPOD(Type* t);

Expr*      resolvePrimInit(CallExpr* call);

bool       isTupleContainingOnlyReferences(Type* t);

void       ensureEnumTypeResolved(EnumType* etype);

void       resolveFnForCall(FnSymbol* fn, CallExpr* call);

bool       canInstantiate(Type* actualType, Type* formalType);

Type*      getConcreteParentForGenericFormal(Type* actualType,
                                             Type* formalType);

bool       isInstantiation(Type* sub, Type* super);

// explain call stuff
bool explainCallMatch(CallExpr* call);

bool isLeaderIterator(FnSymbol* fn);

bool isStandaloneIterator(FnSymbol* fn);

bool isDispatchParent(Type* t, Type* pt);

bool canCoerce(Type*     actualType,
               Symbol*   actualSym,
               Type*     formalType,
               FnSymbol* fn,
               bool*     promotes = NULL);

bool canDispatch(Type*     actualType,
                 Symbol*   actualSym,
                 Type*     formalType,
                 FnSymbol* fn          = NULL,
                 bool*     promotes    = NULL,
                 bool      paramCoerce = false);

bool fixupDefaultInitCopy(FnSymbol* fn, FnSymbol* newFn, CallExpr* call);

void parseExplainFlag(char* flag, int* line, ModuleSymbol** module);

FnSymbol* getTheIteratorFn(Symbol* ic);
FnSymbol* getTheIteratorFn(CallExpr* call);
FnSymbol* getTheIteratorFn(Type* icType);

// forall intents
CallExpr* resolveParallelIteratorAndForallIntents(ForallStmt* pfs,
                                                  SymExpr*    origSE);

void implementForallIntents1(DefExpr* defChplIter);

void implementForallIntents2(CallExpr* call, CallExpr* origToLeaderCall);

void implementForallIntents2wrapper(CallExpr* call,
                                    CallExpr* origToLeaderCall);

void implementForallIntentsNew(ForallStmt* fs, CallExpr* parCall);

void stashPristineCopyOfLeaderIter(FnSymbol* origLeader,
                                   bool      ignoreIsResolved);

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
void insertFormalTemps(FnSymbol* fn);
void insertAndResolveCasts(FnSymbol* fn);
void ensureInMethodList(FnSymbol* fn);



FnSymbol* getAutoCopy(Type* t);
FnSymbol* getAutoDestroy(Type* t);
FnSymbol* getUnalias(Type* t);


bool isPOD(Type* t);

// resolution errors and warnings
void printResolutionErrorAmbiguous(CallInfo&                  info,
                                   Vec<ResolutionCandidate*>& candidates);

void printResolutionErrorUnresolved(Vec<FnSymbol*>& visibleFns,
                                    CallInfo*       info);

void resolveNormalCallCompilerWarningStuff(FnSymbol* resolvedFn);

void lvalueCheck(CallExpr* call);

void checkForStoringIntoTuple(CallExpr* call, FnSymbol* resolvedFn);

void printTaskOrForallConstErrorNote(Symbol* aVar);

// tuples
FnSymbol* createTupleSignature(FnSymbol* fn, SymbolMap& subs, CallExpr* call);

// returns true if the function was handled
bool fixupTupleFunctions(FnSymbol* fn, FnSymbol* newFn, CallExpr* call);

AggregateType* computeNonRefTuple(AggregateType* t);

AggregateType* computeTupleWithIntent(IntentTag intent, AggregateType* t);

bool evaluateWhereClause(FnSymbol* fn);

bool isAutoDestroyedVariable(Symbol* sym);


extern Map<Type*,FnSymbol*> valueToRuntimeTypeMap; // convertValueToRuntimeType

struct Serializers {
  FnSymbol* serializer;
  FnSymbol* deserializer;
  FnSymbol* broadcaster;
  FnSymbol* destroyer;
};

extern std::map<Type*, Serializers> serializeMap;

#endif
