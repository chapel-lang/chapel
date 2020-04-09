// write out all the variables

#define lvar(name, type, init) writeln(#name, " ", name);
#include "var-types.h"
#undef lvar

