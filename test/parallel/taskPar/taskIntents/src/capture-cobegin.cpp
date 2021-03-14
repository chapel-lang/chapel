// Verify that values are captured upon a 'cobegin'.
// This needs #include "support-decls.cpp".
// Can be placed in any scope.

#if DBG
#include "support-decls.cpp"
#endif

#include "var-decls.cpp"
var s$: sync int;

writeln("before cobegin");
#include "var-writes.cpp"

proc updateVars() {
  writeln("updateVars()");
#include "var-assigns.cpp"
#include "var-writes.cpp"
}

cobegin {
  {
    updateVars();
    writeln("cobegin - after updateVars");
#include "var-writes.cpp"
    s$ = 1;
  }
  {
    s$;
    writeln("cobegin - the other stmt");
#include "var-writes.cpp"
  }
}

writeln("after cobegin");
#include "var-writes.cpp"

writeln("done");
