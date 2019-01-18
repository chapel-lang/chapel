use CommDiagnostics;

config const n = 10;

proc main() {
  var x = 10;
  for i in 1..n do x += 10;

  startCommDiagnostics();
  on Locales[numLocales-1] {
    // We should be allowed to RVF this because the value is only read
    // and is not concurrently modified.
    if x < 0 then writeln("Hello!");
  }
  stopCommDiagnostics();
  
  var stats = getCommDiagnostics();
  writeln("GETs on last locale: ", stats(numLocales-1).get);
}
