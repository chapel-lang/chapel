#ifndef _INLINE_FUNCTIONS_
#define _INLINE_FUNCTIONS_

#include "stmt.h"
#include "expr.h"

void inlineFunctions(void);

void inline_call(CallExpr* call, Vec<Stmt*>* stmts);

#endif
