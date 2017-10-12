use driver_domains;

const bigDom4D = Dom4D.expand((1,1,1,1));
var A: [bigDom4D] 4*int = {(...bigDom4D.dims())};
var B: [Dom4D] 4*int;

resetCommDiagnostics();
startCommDiagnostics();
ref Areind = A.reindex({0..n4+1,0..n4+1,0..n4+1,0..n4+1});
stopCommDiagnostics();
writeln(getCommDiagnostics());
resetCommDiagnostics();
startCommDiagnostics();
B = Areind;
stopCommDiagnostics();
writeln(getCommDiagnostics());

for i in Dom4D do if B[i]!=(i-(1,1,1,1)) then writeln("ERROR: B[", i, "]==", B[i]);
