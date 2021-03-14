public use driver_domains;
private use CommDiagnostics;

const bigDom4D = Dom4D.expand((1,1,1,1));
var A: [bigDom4D] int;
for e in A[Dom4D] do e = next();
const n = next()-1;

resetCommDiagnostics();
startCommDiagnostics();
var c = + reduce A[1..n4,1..n4,1..n4,1..n4];
stopCommDiagnostics();
writeln(getCommDiagnostics());
if c != n*(n+1)/2 then writeln("ERROR: ", c);
