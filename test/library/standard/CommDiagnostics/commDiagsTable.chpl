use CommDiagnostics;


proc runtrial(numtrials) {
  var x: int;
  resetCommDiagnostics();
  startCommDiagnostics();
  for i in 1..numtrials do
    coforall loc in Locales with (ref x) do
      on loc do
        x = 1;
  stopCommDiagnostics();
  printCommDiagnosticsTable();
  writeln();
}

runtrial(1);
runtrial(999);
runtrial(1000);
runtrial(1001);
runtrial(10000);

