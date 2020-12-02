use driver_domains;
private use CommDiagnostics;

var A: [Dom4D] 4*int = {(...Dom4D.dims())};
var B: [Dom4D] 4*int = (0, 0, 0, 0);

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
