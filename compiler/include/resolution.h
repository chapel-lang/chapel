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


extern int                              explainCallLine;

extern SymbolMap                        paramMap;

extern Vec<CallExpr*>                   callStack;

extern Map<Type*,     FnSymbol*>        autoDestroyMap;

extern Map<Type*,     FnSymbol*>        valueToRuntimeTypeMap;

extern std::map<Type*,     Serializers> serializeMap;



bool       propagateNotPOD(Type* t);

void       lowerPrimInit(CallExpr* call, Expr* preventingSplitInit);
void       resolveInitVar(CallExpr* call); // lowers PRIM_INIT_VAR_SPLIT_INIT
void       fixPrimInitsAndAddCasts(FnSymbol* fn);

bool       isTupleContainingOnlyReferences(Type* t);

bool       isTupleContainingAnyReferences(Type* t);

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
bool       shouldAddInFormalTempAtCallSite(ArgSymbol* formal, FnSymbol* fn);

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

bool allowImplicitNilabilityRemoval(Type* actualType,
                                    Symbol* actualSym,
                                    Type* formalType,
                                    Symbol* formalSym);

bool canCoerceDecorators(ClassTypeDecorator actual,
                         ClassTypeDecorator formal,
                         bool allowNonSubtypes,
                         bool implicitBang);
bool canInstantiateDecorators(ClassTypeDecorator actual,
                              ClassTypeDecorator formal);
bool canInstantiateOrCoerceDecorators(ClassTypeDecorator actual,
                                      ClassTypeDecorator formal,
                                      bool allowNonSubtypes,
                                      bool implicitBang);

bool canCoerceAsSubtype(Type*     actualType,
                        Symbol*   actualSym,
                        Type*     formalType,
                        ArgSymbol* formalSym,
                        FnSymbol* fn,
                        bool*     promotes = NULL,
                        bool*     paramNarrows = NULL);

bool canCoerce(Type*     actualType,
               Symbol*   actualSym,
               Type*     formalType,
               ArgSymbol* formalSym,
               FnSymbol* fn,
               bool*     promotes = NULL,
               bool*     paramNarrows = NULL);

bool canDispatch(Type*     actualType,
                 Symbol*   actualSym,
                 Type*     formalType,
                 ArgSymbol* formalSym = NULL,
                 FnSymbol* fn          = NULL,
                 bool*     promotes    = NULL,
                 bool*     paramNarrows= NULL,
                 bool      paramCoerce = false);


void parseExplainFlag(char* flag, int* line, ModuleSymbol** module);

FnSymbol* findCopyInitFn(AggregateType* ct, const char*& err);
FnSymbol* findAssignFn(AggregateType* at);
FnSymbol* findZeroArgInitFn(AggregateType* at);

FnSymbol* getTheIteratorFn(Symbol* ic);
FnSymbol* getTheIteratorFn(Type* icType);

// task intents
extern Symbol* markPruned;
bool isReduceOp(Type* type);
void convertFieldsOfRecordThis(FnSymbol* fn);

// forall intents
CallExpr* resolveForallHeader(ForallStmt* pfs, SymExpr* origSE);
void  resolveForallStmts2();
Expr* replaceForWithForallIfNeeded(ForLoop* forLoop);
void  setReduceSVars(ShadowVarSymbol*& PRP, ShadowVarSymbol*& PAS,
                     ShadowVarSymbol*& RP, ShadowVarSymbol* AS);
void setupAndResolveShadowVars(ForallStmt* fs);
bool preserveShadowVar(Symbol* var);
void adjustVoidShadowVariables();
Expr* lowerPrimReduce(CallExpr* call);

void buildFastFollowerChecksIfNeeded(CallExpr* checkCall);

FnSymbol* instantiate(FnSymbol* fn, SymbolMap& subs);
FnSymbol* instantiateSignature(FnSymbol* fn, SymbolMap& subs, CallExpr* call);
void      instantiateBody(FnSymbol* fn);

// generics support
TypeSymbol* getNewSubType(FnSymbol* fn, Symbol* key, TypeSymbol* actualTS);
void checkInfiniteWhereInstantiation(FnSymbol* fn);
void popInstantiationLimit(FnSymbol* fn);
void renameInstantiatedTypeString(TypeSymbol* sym, VarSymbol* var);

FnSymbol* determineRootFunc(FnSymbol* fn);

void determineAllSubs(FnSymbol*  fn,
                      FnSymbol*  root,
                      SymbolMap& subs,
                      SymbolMap& allSubs);

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
void      resolveCall(CallExpr* call);
void      resolveCallAndCallee(CallExpr* call, bool allowUnresolved = false);

Type*     resolveDefaultGenericTypeSymExpr(SymExpr* se);
Type*     resolveTypeAlias(SymExpr* se);

FnSymbol* tryResolveCall(CallExpr* call, bool checkWithin=false);
void      makeRefType(Type* type);

// FnSymbol changes
void      insertFormalTemps(FnSymbol* fn);
void      ensureInMethodList(FnSymbol* fn);


bool      hasAutoCopyForType(Type* type);
FnSymbol* getAutoCopyForType(Type* type);   // requires hasAutoCopyForType()==true
void      getAutoCopyTypeKeys(Vec<Type*>& keys);
FnSymbol* getAutoCopy(Type* t);             // returns NULL if there are none
FnSymbol* getAutoDestroy(Type* t);          //  "
FnSymbol* getUnalias(Type* t);
const char* getErroneousCopyError(FnSymbol* fn);
void markCopyErroneous(FnSymbol* fn, const char* err);


bool isPOD(Type* t);
bool recordContainingCopyMutatesField(Type* at);

// resolution errors and warnings

// This one does not call USR_STOP
void printResolutionErrorUnresolved(CallInfo&                  info,
                                    Vec<FnSymbol*>&            visibleFns);

void printResolutionErrorAmbiguous (CallInfo&                  info,
                                    Vec<ResolutionCandidate*>& candidates);
void printUndecoratedClassTypeNote(Expr* ctx, Type* type);

FnSymbol* resolveNormalCall(CallExpr* call);

void resolveNormalCallCompilerWarningStuff(CallExpr* call, FnSymbol* resolvedFn);

void checkMoveIntoClass(CallExpr* call, Type* lhs, Type* rhs);

void lvalueCheck(CallExpr* call);

void checkForStoringIntoTuple(CallExpr* call, FnSymbol* resolvedFn);

bool signatureMatch(FnSymbol* fn, FnSymbol* gn);

bool isSubtypeOrInstantiation(Type* sub, Type* super, Expr* ctx);
bool isCoercibleOrInstantiation(Type* sub, Type* super, Expr* ctx);

void printTaskOrForallConstErrorNote(Symbol* aVar);

// tuples
FnSymbol* createTupleSignature(FnSymbol* fn, SymbolMap& subs, CallExpr* call);

// returns true if the function was handled
bool fixupTupleFunctions(FnSymbol* fn, FnSymbol* newFn, CallExpr* call);

AggregateType* computeNonRefTuple(AggregateType* t);

AggregateType* computeTupleWithIntent(IntentTag intent, AggregateType* t);
AggregateType* computeTupleWithIntentForArg(IntentTag intent, AggregateType* t, ArgSymbol* arg);

void addTupleCoercion(AggregateType* fromT, AggregateType* toT, Symbol* fromSym, Symbol* toSym, Expr* insertBefore);

// other resolution functions
bool evaluateWhereClause(FnSymbol* fn);

bool isAutoDestroyedVariable(Symbol* sym);

static inline bool isUnresolvedOrGenericReturnType(Type* retType) {
  return retType == dtUnknown || retType->symbol->hasFlag(FLAG_GENERIC);
}

SymExpr* findSourceOfYield(CallExpr* yield);

void expandInitFieldPrims();

void removeCopyFns(Type* t);

bool isUnusedClass(Type* t);

void pruneResolvedTree();

void resolveTypeWithInitializer(AggregateType* at, FnSymbol* fn);

void resolvePromotionType(AggregateType* at);

void resolveDestructor(AggregateType* at);

void fixTypeNames(AggregateType* at);

Type* getInstantiationType(Type* actualType, Symbol* actualSym,
                           Type* formalType, Symbol* formalSym,
                           Expr* ctx,
                           bool allowCoercion=true,
                           bool implicitBang=false);

void resolveIfExprType(CondStmt* stmt);

void trimVisibleCandidates(CallInfo& call,
                           Vec<FnSymbol*>& mostApplicable,
                           Vec<FnSymbol*>& visibleFns);

void resolveGenericActuals(CallExpr* call);

Type* computeDecoratedManagedType(AggregateType* canonicalClassType,
                                  ClassTypeDecorator useDec,
                                  AggregateType* manager,
                                  Expr* ctx);

void checkDuplicateDecorators(Type* decorator, Type* decorated, Expr* ctx);

// These enable resolution for functions that don't really match
// according to the language definition in order to get more errors
// reported at once. E.g. C? can pass to C.
void startGenerousResolutionForErrors();
bool inGenerousResolutionForErrors();
void stopGenerousResolutionForErrors();

#endif
