use CommDiagnostics;
config const n = 10;

const D = {1..n};
resetCommDiagnostics();
startCommDiagnostics();
on Locales[numLocales-1] {
  chpl__autoCopy(D);
}
stopCommDiagnostics();
writeln(getCommDiagnostics());

resetCommDiagnostics();
startCommDiagnostics();
on Locales[numLocales-1] {
  chpl__autoDestroy(D);
}
stopCommDiagnostics();
writeln(getCommDiagnostics());

var A: [D] int;

resetCommDiagnostics();
startCommDiagnostics();
on Locales[numLocales-1] {
  chpl__autoCopy(A);
}
stopCommDiagnostics();
writeln(getCommDiagnostics());

resetCommDiagnostics();
startCommDiagnostics();
on Locales[numLocales-1] {
  chpl__autoDestroy(A);
}
stopCommDiagnostics();
writeln(getCommDiagnostics());
