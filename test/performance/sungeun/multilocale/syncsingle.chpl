use CommDiagnostics;
config param doVerboseComm = false;

var si: single bool;
resetCommDiagnostics();
startCommDiagnostics();
if doVerboseComm then startVerboseComm();
coforall l in Locales do on l {
  if l.id == numLocales-1 then
    begin si.writeEF(true);
  si.readFF();
}
if doVerboseComm then stopVerboseComm();
stopCommDiagnostics();
writeln(getCommDiagnostics());

var sy: sync bool;
resetCommDiagnostics();
startCommDiagnostics();
if doVerboseComm then startVerboseComm();
coforall l in Locales do on l {
  if l.id == numLocales-1 then
    begin sy.writeEF(true);
  sy.readFF();
}
if doVerboseComm then stopVerboseComm();
stopCommDiagnostics();
writeln(getCommDiagnostics());

