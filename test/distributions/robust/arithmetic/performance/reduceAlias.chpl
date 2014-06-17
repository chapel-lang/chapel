use driver_domains;

var A: [Dom4D] int;
for e in A do e = next();
const n = next()-1;

resetCommDiagnostics();
startCommDiagnostics();
var AA => A;
stopCommDiagnostics();
writeln(getCommDiagnostics());
resetCommDiagnostics();
startCommDiagnostics();
var c = + reduce AA;
stopCommDiagnostics();
writeln(getCommDiagnostics());
if c != n*(n+1)/2 then writeln("ERROR: ", c);
