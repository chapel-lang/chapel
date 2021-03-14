// Verify that values are not captured upon a 'begin' with a ref clause.
// This needs #include "support-decls.cpp".
// Can be placed in any scope.

#if DBG
#include "support-decls.cpp"
#endif

#include "var-decls.cpp"
var s$: sync int;

begin with (
#define lvar(name, type, init) ref name,
#include "var-types.h"
#undef lvar
          ref s$)
{
  writeln("in begin");
#include "var-writes.cpp"
#include "var-assigns.cpp"
//writeln("after assigns in begin");
//#include "var-writes.cpp"
  s$ = 1;
}

s$;
writeln("after begin");
#include "var-writes.cpp"
