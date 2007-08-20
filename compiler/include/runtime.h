#ifndef _RUNTIME_H_
#define _RUNTIME_H_

#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

extern ModuleSymbol* baseModule;
extern ModuleSymbol* fileModule;
extern ModuleSymbol* standardModule;
extern ModuleSymbol* tupleModule;
extern ModuleSymbol* theProgram;
extern ModuleSymbol* domainModule;

extern TypeSymbol* chpl_array;
extern Type* dtWriter;

extern VarSymbol* chpl_stdin;
extern VarSymbol* chpl_stdout;
extern VarSymbol* chpl_stderr;

#endif
