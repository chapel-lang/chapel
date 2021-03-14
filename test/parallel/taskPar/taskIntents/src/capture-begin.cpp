// Verify that values are captured upon a 'begin'.
// This needs #include "support-decls.cpp".
// Can be placed in any scope.

#if DBG
#include "support-decls.cpp"
#endif

#include "var-decls.cpp"
var s1$, s2$: sync int;

begin {
  s1$;
  writeln("starting in begin");
#include "var-writes.cpp"
  s2$ = 1;
}

#include "var-assigns.cpp"
#include "var-writes.cpp"
writeln("finished outside begin");
s1$ = 1;
s2$;
