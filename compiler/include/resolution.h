/*
 * Copyright 2004-2016 Cray Inc.
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

extern SymbolMap paramMap;
extern Vec<CallExpr*> callStack;
bool hasAutoCopyForType(Type* type);
FnSymbol* getAutoCopyForType(Type* type);
void getAutoCopyTypeKeys(Vec<Type*> &keys); // type to chpl__autoCopy function
extern Map<Type*,FnSymbol*> autoDestroyMap; // type to chpl__autoDestroy function
extern Map<FnSymbol*,FnSymbol*> iteratorLeaderMap;
extern Map<FnSymbol*,FnSymbol*> iteratorFollowerMap;
extern std::map<CallExpr*,CallExpr*> eflopiMap;

FnSymbol* requiresImplicitDestroy(CallExpr* call);
bool isLeaderIterator(FnSymbol* fn);
bool isStandaloneIterator(FnSymbol* fn);

bool isDispatchParent(Type* t, Type* pt);

bool canCoerce(Type* actualType, Symbol* actualSym, Type* formalType, FnSymbol* fn, bool* promotes = NULL);
bool canDispatch(Type* actualType, Symbol* actualSym, Type* formalType, FnSymbol* fn = NULL, bool* promotes = NULL, bool paramCoerce = false);

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

FnSymbol* instantiate(FnSymbol* fn, SymbolMap& subs, CallExpr* call);
FnSymbol* instantiateSignature(FnSymbol* fn, SymbolMap& subs, CallExpr* call);
void      instantiateBody(FnSymbol* fn);

void resolveFormals(FnSymbol* fn);
void resolveBlockStmt(BlockStmt* blockStmt);
void resolveCall(CallExpr* call);
void resolveCallAndCallee(CallExpr* call, bool allowUnresolved = false);
void makeRefType(Type* type);
FnSymbol* tryResolveCall(CallExpr* call);
void resolveFns(FnSymbol* fn);

FnSymbol* defaultWrap(FnSymbol* fn, Vec<ArgSymbol*>* actualFormals,  CallInfo* info);
void reorderActuals(FnSymbol* fn, Vec<ArgSymbol*>* actualFormals,  CallInfo* info);
void coerceActuals(FnSymbol* fn, CallInfo* info);
FnSymbol* promotionWrap(FnSymbol* fn, CallInfo* info, bool buildFastFollowerChecks);

FnSymbol* getAutoCopy(Type* t);
FnSymbol* getAutoDestroy(Type* t);
FnSymbol* getUnalias(Type* t);


bool isPOD(Type* t);

// tuples
FnSymbol* createTupleSignature(FnSymbol* fn, SymbolMap& subs, CallExpr* call);
void fixupTupleFunctions(FnSymbol* fn, FnSymbol* newFn, CallExpr* call);
AggregateType* computeNonRefTuple(Type* t);
AggregateType* computeTupleWithIntent(IntentTag intent, Type* t);

#endif
