use CommDiagnostics;


proc runtrial(numtrials, printEmpty) {
  var x: int;
  resetCommDiagnostics();
  startCommDiagnostics();
  for i in 1..numtrials do
    coforall loc in Locales with (ref x) do
      on loc do
        x = 1;
  stopCommDiagnostics();
  printCommDiagnosticsTable(printEmpty);
  writeln();
}

for printEmpty in false..true {
  runtrial(1, printEmpty);
  runtrial(999, printEmpty);
  runtrial(1000, printEmpty);
  runtrial(1001, printEmpty);
  runtrial(10000, printEmpty);
}
