// Verify that values are captured upon a 'coforall'.
// This needs #include "support-decls.cpp".
// Can be placed in any scope.

#if DBG
#include "support-decls.cpp"
#endif

#include "var-decls.cpp"
var s$: sync int;

writeln("before coforall");
#include "var-writes.cpp"

proc updateVars() {
  writeln("updateVars()");
#include "var-assigns.cpp"
#include "var-writes.cpp"
}

coforall jjjjj in 1..2 {
  if jjjjj == 1
  {
    updateVars();
    writeln("coforall - after updateVars");
#include "var-writes.cpp"
    s$ = 1;
  }
  else
  {
    s$;
    writeln("coforall - jjjjj=2");
#include "var-writes.cpp"
  }
}

writeln("after coforall");
#include "var-writes.cpp"

writeln("done");
