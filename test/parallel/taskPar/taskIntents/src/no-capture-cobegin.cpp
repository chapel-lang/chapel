// Verify that values are not captured upon a 'cobegin' with a ref clause.
// This needs #include "support-decls.cpp".
// Can be placed in any scope.

#if DBG
#include "support-decls.cpp"
#endif

#include "var-decls.cpp"
var kkkkkk: int;

cobegin with (
#define lvar(name, type, init) ref name,
#include "var-types.h"
#undef lvar
          ref kkkkkk)
{
  var jjjjjj: int;
  {
    writeln("in cobegin");
#include "var-writes.cpp"
#include "var-assigns.cpp"
//writeln("after assigns in cobegin");
//#include "var-writes.cpp"
  }
}

writeln("after cobegin");
#include "var-writes.cpp"
