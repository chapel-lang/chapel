use driver_domains;
private use CommDiagnostics;

config type myType = int;

proc alloc_array(D: domain) {
  var A: [D] myType;
}

config const doVerboseComm = false;

writeln("Dom1D");
resetCommDiagnostics();
startCommDiagnostics();
if doVerboseComm then startVerboseComm();
coforall loc in Locales do on loc do alloc_array(Dom1D);
if doVerboseComm then stopVerboseComm();
stopCommDiagnostics();
writeln(getCommDiagnostics());

writeln("Dom2D");
resetCommDiagnostics();
startCommDiagnostics();
if doVerboseComm then startVerboseComm();
coforall loc in Locales do on loc do alloc_array(Dom2D);
if doVerboseComm then stopVerboseComm();
stopCommDiagnostics();
writeln(getCommDiagnostics());

writeln("Dom3D");
resetCommDiagnostics();
startCommDiagnostics();
if doVerboseComm then startVerboseComm();
coforall loc in Locales do on loc do alloc_array(Dom3D);
if doVerboseComm then stopVerboseComm();
stopCommDiagnostics();
writeln(getCommDiagnostics());

writeln("Dom4D");
resetCommDiagnostics();
startCommDiagnostics();
if doVerboseComm then startVerboseComm();
coforall loc in Locales do on loc do alloc_array(Dom4D);
if doVerboseComm then stopVerboseComm();
stopCommDiagnostics();
writeln(getCommDiagnostics());

writeln("Dom2D32");
resetCommDiagnostics();
startCommDiagnostics();
if doVerboseComm then startVerboseComm();
coforall loc in Locales do on loc do alloc_array(Dom2D32);
if doVerboseComm then stopVerboseComm();
stopCommDiagnostics();
writeln(getCommDiagnostics());
