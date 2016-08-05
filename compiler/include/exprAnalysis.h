#ifndef _EXPR_ANALYSIS_H_
#define _EXPR_ANALYSIS_H_

#include "expr.h"
#include "symbol.h"

//std::map<Expr*,bool> safeExprCache;
//std::map<FnSymbol*,bool> globalManipFuncCache;

bool isNonEssentialPrimitive(CallExpr* ce);

/*
 * Returns true if `e` has no side effects. Checked side effects are:
 *  - Read/write to a global
 *  - Is/containsessential primitive
 *  - If it's a function call has ref arguments
 */
bool exprHasNoSideEffects(Expr* e);

bool isSafePrimitive(CallExpr* ce);

bool isRegisteredGlobalManip(FnSymbol* fn);
bool getGlobalManip(FnSymbol* fn);
void registerGlobalManip(FnSymbol* fn, bool manip);

#endif
