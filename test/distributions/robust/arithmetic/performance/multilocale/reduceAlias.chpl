use driver_domains;
use CommDiagnostics;

var A: [Dom4D] int;
for e in A do e = next();
const n = next()-1;

resetCommDiagnostics();
startCommDiagnostics();
ref AA = A;
stopCommDiagnostics();
writeln(getCommDiagnostics());
resetCommDiagnostics();
startCommDiagnostics();
var c = + reduce AA;
stopCommDiagnostics();
writeln(getCommDiagnostics());
if c != n*(n+1)/2 then writeln("ERROR: ", c);
