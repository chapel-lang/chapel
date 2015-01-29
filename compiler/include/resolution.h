#ifndef _RESOLUTION_H_
#define _RESOLUTION_H_

#include "baseAST.h"

class CallInfo;

extern SymbolMap paramMap;
extern Vec<CallExpr*> callStack;
extern Map<Type*,FnSymbol*> autoCopyMap; // type to chpl__autoCopy function
extern Map<Type*,FnSymbol*> autoDestroyMap; // type to chpl__autoDestroy function
extern Map<FnSymbol*,FnSymbol*> iteratorLeaderMap;
extern Map<FnSymbol*,FnSymbol*> iteratorFollowerMap;

FnSymbol* requiresImplicitDestroy(CallExpr* call);

bool isDispatchParent(Type* t, Type* pt);

bool canCoerce(Type* actualType, Symbol* actualSym, Type* formalType, FnSymbol* fn, bool* promotes = NULL);
bool canDispatch(Type* actualType, Symbol* actualSym, Type* formalType, FnSymbol* fn = NULL, bool* promotes = NULL, bool paramCoerce = false);

const char* toString(Type* type);
const char* toString(CallInfo* info);
const char* toString(FnSymbol* fn);

void parseExplainFlag(char* flag, int* line, ModuleSymbol** module);

// forall intents
void implementForallIntents1(DefExpr* defChplIter);
void implementForallIntents2(CallExpr* call, CallExpr* origToLeaderCall);
void stashPristineCopyOfLeaderIter(FnSymbol* origLeader, bool ignore_isResolved);

FnSymbol* instantiate(FnSymbol* fn, SymbolMap& subs, CallExpr* call);
FnSymbol* instantiateSignature(FnSymbol* fn, SymbolMap& subs, CallExpr* call);
void      instantiateBody(FnSymbol* fn);

void resolveFormals(FnSymbol* fn);
void resolveBlockStmt(BlockStmt* blockStmt);
void resolveCall(CallExpr* call);
void resolveFns(FnSymbol* fn);

FnSymbol* defaultWrap(FnSymbol* fn, Vec<ArgSymbol*>* actualFormals,  CallInfo* info);
void reorderActuals(FnSymbol* fn, Vec<ArgSymbol*>* actualFormals,  CallInfo* info);
void coerceActuals(FnSymbol* fn, CallInfo* info);
FnSymbol* promotionWrap(FnSymbol* fn, CallInfo* info);

FnSymbol* getAutoCopy(Type* t);
FnSymbol* getAutoDestroy(Type* t);

#endif
