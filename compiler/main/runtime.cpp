#include "runtime.h"

ModuleSymbol* prelude = NULL;
ModuleSymbol* fileModule = NULL;
ModuleSymbol* tupleModule = NULL;

TypeSymbol* htuple = NULL;

VarSymbol* chpl_true = NULL;
VarSymbol* chpl_false = NULL;
