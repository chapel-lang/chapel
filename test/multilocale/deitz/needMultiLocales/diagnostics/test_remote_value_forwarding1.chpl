use CommDiagnostics;

proc foo(i: int): int
  return if i > 1 then 1 + foo(i-1) else 1;

proc main() {
  var r = foo(3), s = 0;
  on Locales(1) {
    startCommDiagnostics();
    s = r;
    stopCommDiagnostics();
  }
  writeln(s);
  writeln(getCommDiagnostics());
}
