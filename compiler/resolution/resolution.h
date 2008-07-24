#ifndef _RESOLUTION_H_
#define _RESOLUTION_H_

class CallInfo;

extern SymbolMap paramMap;
extern Vec<CallExpr*> callStack;

bool isDispatchParent(Type* t, Type* pt);

bool canCoerce(Type* actualType, Symbol* actualSym, Type* formalType, FnSymbol* fn, bool* require_scalar_promotion = NULL);
bool canDispatch(Type* actualType,
                        Symbol* actualSym,
                        Type* formalType,
                        FnSymbol* fn = NULL,
                        bool* require_scalar_promotion = NULL,
                        bool paramCoerce = false);

const char* toString(Type* type);
const char* toString(CallInfo* info);
const char* toString(FnSymbol* fn);

void parseExplainFlag(char* flag, int* line, ModuleSymbol** module);

FnSymbol* instantiate(FnSymbol* fn, SymbolMap* subs, CallExpr* call);
void resolveFormals(FnSymbol* fn);
void resolveCall(CallExpr* call, bool errorCheck = true);
void resolveBlock(Expr* body);

FnSymbol* build_default_wrapper(FnSymbol* fn, Vec<ArgSymbol*>* actualFormals, bool isSquare);
FnSymbol* build_order_wrapper(FnSymbol* fn, Vec<ArgSymbol*>* actualFormals, bool isSquare);
FnSymbol* build_coercion_wrapper(FnSymbol* fn, CallInfo* info);
FnSymbol* build_promotion_wrapper(FnSymbol* fn, CallInfo* info);

#endif
