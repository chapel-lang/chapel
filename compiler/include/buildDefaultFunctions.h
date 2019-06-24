#ifndef _BUILD_DEFAULT_FUNCTIONS_H_
#define _BUILD_DEFAULT_FUNCTIONS_H_

#include "symbol.h"
#include "type.h"

// build a hollow `writeThis` FunctionSymbol* for the given class type
// 'ct', returning its file (channel) argument in 'filearg'.  The
// callee is expected to fill in the body of the function and to
// normalize the returned function before continuing.

FnSymbol* buildWriteThisFnSymbol(AggregateType* ct, ArgSymbol** filearg);

#endif

