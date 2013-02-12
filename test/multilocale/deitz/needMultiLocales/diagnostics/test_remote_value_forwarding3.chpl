use CommDiagnostics;

proc main() {
  var x: int = 3;
  var r, s, t: int;

  on Locales(1) {
    foo(2);
    startCommDiagnostics();
    r = x;
    stopCommDiagnostics();
  }

  proc foo(n: int) {
    s = s + x;
    if n > 0 then bar(n-1);
  }

  proc bar(n : int) {
    t = t + x;
    if n > 0 then foo(n-1);
  }

  writeln("results = ", (r, s, t, x));
  writeln("diagnostics = ", getCommDiagnostics());
}
