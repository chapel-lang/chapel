// Should the line number associated with the error message be the one
// containing the call to compilerError, or the one containing the call to foo?
// If n=0, the line number reported is the latter one.

proc foo (x=1) {
  compilerError("What line number is reported for this message?");
}

config param n = 1;

if n > 0 then
  foo();

proc bar () {
  foo(13);
}

bar();
