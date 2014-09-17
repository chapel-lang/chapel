// Verify that values are not captured upon a 'coforall' with a ref clause.
// This needs #include "support-decls.cpp".
// Can be placed in any scope.

#if DBG
#include "support-decls.cpp"
#endif

#include "var-decls.cpp"
var kkkkkk: int;

coforall jjjjj in 1..2 with (
#define lvar(name, type, init) ref name,
#include "var-types.h"
#undef lvar
          ref kkkkkk)
{
  if jjjjj == 1 {
    writeln("in coforall");
#include "var-writes.cpp"
#include "var-assigns.cpp"
//writeln("after assigns in coforall");
//#include "var-writes.cpp"
  }
}

writeln("after coforall");
#include "var-writes.cpp"
