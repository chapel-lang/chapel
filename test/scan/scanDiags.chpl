use CommDiagnostics, BlockDist;

config const n = 10;
var A: [{1..n} dmapped new blockDist({1..n})] int = [i in 1..n] i;

startCommDiagnostics();
var B = + scan A;
stopCommDiagnostics();
printCommDiagnosticsTable();
