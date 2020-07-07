use CommDiagnostics;

var x: int;

resetCommDiagnostics();
startCommDiagnostics();
coforall loc in Locales with (ref x) do
  on loc do
    x = 1;
stopCommDiagnostics();
printCommDiagnosticsTable();


