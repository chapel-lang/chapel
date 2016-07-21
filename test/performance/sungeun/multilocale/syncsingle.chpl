use CommDiagnostics;
use Time;
config param doVerboseComm = false;
config const dosleep = false;

var si: single bool;
resetCommDiagnostics();
startCommDiagnostics();
if doVerboseComm then startVerboseComm();
coforall l in Locales do on l {
  if l.id == numLocales-1 then
    begin {
      si.writeEF(true);
      if dosleep then sleep(3);
    }
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
    begin {
      sy.writeEF(true);
      if dosleep then sleep(3);
    }
  sy.readFF();
}
if doVerboseComm then stopVerboseComm();
stopCommDiagnostics();
writeln(getCommDiagnostics());

