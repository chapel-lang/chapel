use CommDiagnostics;
config param doVerboseComm = false;
config const noisy = false;
config const target = numLocales-1;
proc foo() {
  if noisy then writeln("hi");
}

resetCommDiagnostics();
startCommDiagnostics();
if doVerboseComm then startVerboseComm();
on Locales[numLocales-1] do if here.id==target then begin foo();
if doVerboseComm then stopVerboseComm();
stopCommDiagnostics();
writeln(getCommDiagnostics());

resetCommDiagnostics();
startCommDiagnostics();
if doVerboseComm then startVerboseComm();
coforall l in Locales[1..numLocales-1] do on l {
  if l.id==target then begin foo();
}
if doVerboseComm then stopVerboseComm();
stopCommDiagnostics();
writeln(getCommDiagnostics());

resetCommDiagnostics();
startCommDiagnostics();
if doVerboseComm then startVerboseComm();
cobegin {
  on Locales[0] do if here.id==target then begin foo();
  on Locales[numLocales-1] do if here.id==target then begin foo();
}
if doVerboseComm then stopVerboseComm();
stopCommDiagnostics();
writeln(getCommDiagnostics());
