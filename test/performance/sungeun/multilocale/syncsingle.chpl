use CommDiagnostics;
use Time;
config param doVerboseComm = false;

var sy: sync bool;
resetCommDiagnostics();
startCommDiagnostics();
if doVerboseComm then startVerboseComm();
sync {
  coforall l in Locales do on l {
    if l.id == numLocales-1 {
      begin {
        sy.writeEF(true);
      }
    }
    sy.readFF();
  }
}
if doVerboseComm then stopVerboseComm();
stopCommDiagnostics();
writeln(getCommDiagnostics());

