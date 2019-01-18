use driver_domains;

var A: [Dom4D] 4*int = {(...Dom4D.dims())};
var B: [Dom4D] 4*int;

resetCommDiagnostics();
startCommDiagnostics();
ref AA = A;
stopCommDiagnostics();
writeln(getCommDiagnostics());
resetCommDiagnostics();
startCommDiagnostics();
B = AA;
stopCommDiagnostics();
writeln(getCommDiagnostics());
for i in Dom4D do if B[i]!=i then writeln("ERROR: B[", i, "]==", B[i]);
