use BlockDist, CommDiagnostics;

config const n = 11;

const D = {1..n, 1..n} dmapped Block({1..n, 1..n});
var A: [D] real;

startCommDiagnostics();
var SLD = D.localSubdomain(Locales);
var SLA = A.localSubdomain(Locales);
stopCommDiagnostics();

writeln(SLD);
writeln(SLA);
writeln(getCommDiagnostics());
