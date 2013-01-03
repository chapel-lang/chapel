use CommDiagnostics;

var si: single bool;
resetCommDiagnostics();
startCommDiagnostics();
coforall l in Locales do on l {
  if l.id == numLocales-1 then
    begin si.writeEF(true);
  si.readFF();
}
stopCommDiagnostics();
writeln(getCommDiagnostics());

var sy: sync bool;
resetCommDiagnostics();
startCommDiagnostics();
coforall l in Locales do on l {
  if l.id == numLocales-1 then
    begin sy.writeEF(true);
  sy.readFF();
}
stopCommDiagnostics();
writeln(getCommDiagnostics());

