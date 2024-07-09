use CommDiagnostics;

var a = 0;

startCommDiagnostics();
on Locales[1] {
  for i in 0..#5 do
    a += 1;
}
stopCommDiagnostics();
printCommDiagnosticsTable();

writeln(a);
