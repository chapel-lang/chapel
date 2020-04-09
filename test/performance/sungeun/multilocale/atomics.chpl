use CommDiagnostics;

var au: atomic uint;
resetCommDiagnostics();
startCommDiagnostics();
coforall l in Locales do on l {
  var x = au.fetchAdd(l.id:uint);
}
stopCommDiagnostics();
writeln(getCommDiagnostics());

var ai: atomic int;
ai.write(-1);
resetCommDiagnostics();
startCommDiagnostics();
coforall l in Locales do on l {
  var x = ai.compareAndSwap(l.id-1, l.id);
}
stopCommDiagnostics();
writeln(getCommDiagnostics());

var ab: atomic bool;
resetCommDiagnostics();
startCommDiagnostics();
coforall l in Locales do on l {
  var f = ab.testAndSet();
}
stopCommDiagnostics();
writeln(getCommDiagnostics());

