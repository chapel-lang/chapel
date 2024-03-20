use BlockDist;
use CommDiagnostics;

config const size = 5;

var D = blockDist.createDomain(0..#size);

var a = 0;

startCommDiagnostics();
forall i in D with (ref a) {
  a += 1; // beware! race condition!
}
stopCommDiagnostics();
printCommDiagnosticsTable();

writeln(a);
