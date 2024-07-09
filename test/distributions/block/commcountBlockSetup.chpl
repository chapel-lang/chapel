use BlockDist, CommDiagnostics;

resetCommDiagnostics();
startCommDiagnostics();
var dom = {1..10} dmapped new blockDist({1..10});
stopCommDiagnostics();
printCommDiagnosticsTable();

resetCommDiagnostics();
startCommDiagnostics();
var dom2 = blockDist.createDomain({1..10});
stopCommDiagnostics();
printCommDiagnosticsTable();

resetCommDiagnostics();
startCommDiagnostics();
var dist = new blockDist({1..10});
stopCommDiagnostics();
printCommDiagnosticsTable();

resetCommDiagnostics();
startCommDiagnostics();
var dom3 = {1..10} dmapped dist;
stopCommDiagnostics();
printCommDiagnosticsTable();

writeln(dom);
writeln(dom2);
writeln(dom3);
writeln(dist);
