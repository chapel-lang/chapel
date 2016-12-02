use CommDiagnostics;

proc main() {
  var x = 5;
  var a : atomic int;
  var z = 0;

  if numLocales <= 1 then halt("This test requires more than one locale.");

  startCommDiagnostics();
  on Locales[numLocales-1] {
    a.add(1);
    z = x;
  }
  stopCommDiagnostics();
  assert(z == x);

  // The atomic operation in the on-statement should disable RVF. We would
  // therefore expect there to be a GET for 'x'.
  var data = getCommDiagnostics();
  writeln("GETs from last locale: ", data[numLocales-1].get);
}
