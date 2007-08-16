#include "runtime.h"

ModuleSymbol* baseModule = NULL;
ModuleSymbol* fileModule = NULL;
ModuleSymbol* tupleModule = NULL;
ModuleSymbol* theProgram = NULL;
ModuleSymbol* domainModule = NULL;

TypeSymbol* chpl_array;
Type* dtWriter;

VarSymbol* chpl_stdin = NULL;
VarSymbol* chpl_stdout = NULL;
VarSymbol* chpl_stderr = NULL;
