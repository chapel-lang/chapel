#include "runtime.h"

ModuleSymbol* prelude = NULL;
ModuleSymbol* baseModule = NULL;
ModuleSymbol* closureModule = NULL;
ModuleSymbol* fileModule = NULL;
ModuleSymbol* tupleModule = NULL;
ModuleSymbol* seqModule = NULL;
ModuleSymbol* standardModule = NULL;
ModuleSymbol* compilerModule = NULL;

TypeSymbol* chpl_seq;
TypeSymbol* chpl_htuple = NULL;

VarSymbol* chpl_stdin = NULL;
VarSymbol* chpl_stdout = NULL;
VarSymbol* chpl_stderr = NULL;

VarSymbol* chpl_input_filename;
VarSymbol* chpl_input_lineno;

VarSymbol* setterToken;
VarSymbol* methodToken;
