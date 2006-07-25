#ifndef _RUNTIME_H_
#define _RUNTIME_H_

#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

extern ModuleSymbol* baseModule;
extern ModuleSymbol* closureModule;
extern ModuleSymbol* fileModule;
extern ModuleSymbol* tupleModule;
extern ModuleSymbol* seqModule;
extern ModuleSymbol* standardModule;
extern ModuleSymbol* compilerModule;

extern TypeSymbol* chpl_seq;

extern VarSymbol* chpl_stdin;
extern VarSymbol* chpl_stdout;
extern VarSymbol* chpl_stderr;

#endif
