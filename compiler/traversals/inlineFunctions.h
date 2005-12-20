#ifndef _INLINE_FUNCTIONS_
#define _INLINE_FUNCTIONS_

#include "stmt.h"
#include "expr.h"

void inlineFunctions(void);

Stmt* inline_call(CallExpr* call);

#endif
