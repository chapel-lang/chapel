#include "builtin.h"

#define S(_n) Sym* sym_##_n = 0;
#include "builtin_symbols.h"
#undef S

