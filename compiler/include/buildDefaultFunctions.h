#ifndef _BUILD_DEFAULT_FUNCTIONS_H_
#define _BUILD_DEFAULT_FUNCTIONS_H_

#include "symbol.h"
#include "type.h"

FnSymbol* buildWriteThisFnSymbol(AggregateType* ct, ArgSymbol** filearg);

#endif

