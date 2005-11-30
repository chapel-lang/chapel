#include "runtime.h"

ModuleSymbol* prelude = NULL;
ModuleSymbol* fileModule = NULL;
ModuleSymbol* tupleModule = NULL;
ModuleSymbol* seqModule = NULL;

TypeSymbol* chpl_seq;
TypeSymbol* chpl_htuple = NULL;

VarSymbol* setterToken;
VarSymbol* methodToken;

VarSymbol* chpl_true = NULL;
VarSymbol* chpl_false = NULL;
