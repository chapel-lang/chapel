#ifndef _RESOLUTION_H_
#define _RESOLUTION_H_

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

FnSymbol* instantiate(FnSymbol* fn, SymbolMap* subs, CallExpr* call);
void resolveFormals(FnSymbol* fn);
void resolveCall(CallExpr* call, bool errorCheck = true);
void resolveBlock(Expr* body);

FnSymbol* defaultWrap(FnSymbol* fn, Vec<ArgSymbol*>* actualFormals,  CallInfo* info);
FnSymbol* orderWrap(FnSymbol* fn, Vec<ArgSymbol*>* actualFormals,  CallInfo* info);
FnSymbol* coercionWrap(FnSymbol* fn, CallInfo* info);
FnSymbol* promotionWrap(FnSymbol* fn, CallInfo* info);

FnSymbol* getAutoCopy(Type* t);
FnSymbol* getAutoDestroy(Type* t);

#endif
