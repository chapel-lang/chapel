#ifndef _RUNTIME_H_
#define _RUNTIME_H_

#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

extern ModuleSymbol* prelude;
extern ModuleSymbol* fileModule;
extern ModuleSymbol* tupleModule;

extern TypeSymbol* htuple;

#endif
