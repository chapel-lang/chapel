use CommDiagnostics;

proc foo(i: int): int
  return if i > 1 then 1 + foo(i-1) else 1;

proc main() {
  var r = foo(3), s = 0;
  on Locales(1) {
    var x, y: int;
    cobegin with (ref x, ref y) {
      x = 1;
      y = 2;
    }
    startCommDiagnostics();
    s = r;
    stopCommDiagnostics();
    s += x;
    s += y;
  }
  writeln(s);
  writeln(getCommDiagnostics());
}
