use driver_domains;
private use CommDiagnostics;

config const doVerboseComm = false;

const bigDom4D = Dom4D.expand((1,1,1,1));
const locBigDom = {(...bigDom4D.dims())};
var A: [bigDom4D] 4*int = locBigDom;
var B: [Dom4D] 4*int = (0, 0, 0, 0);

const offset = locBigDom.translate(1);
const slice = offset.expand(-1);

resetCommDiagnostics();
startCommDiagnostics();
if doVerboseComm then startVerboseComm();
ref Areind = A.reindex(offset)[slice];
if doVerboseComm then stopVerboseComm();
stopCommDiagnostics();
writeln(getCommDiagnostics());
resetCommDiagnostics();
startCommDiagnostics();
if doVerboseComm then startVerboseComm();
B = Areind;
if doVerboseComm then stopVerboseComm();
stopCommDiagnostics();
writeln(getCommDiagnostics());

for i in Dom4D do if B[i]!=i then writeln("ERROR: B[", i, "]==", B[i]);
