// Create a function named FUNNAME with formals of INTENT
// that assigns to each formal, then call it.
// Requires #include "support-decls.cpp" and #include "var-decls.cpp"
// to set up the actuals.

// declare the function
proc FUNNAME(
#define lvar(name, type, init) INTENT name: type,
#include "var-types.h"
#undef lvar
             lastarg: int)
{
#include "error-base.cpp"
}


// call the function
FUNNAME(
#define lvar(name, type, init) name,
#include "var-types.h"
#undef lvar
        5);

// done
#undef FUNNAME
#undef INTENT
