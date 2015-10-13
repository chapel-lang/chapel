use CommDiagnostics;
config const n = 10;

const D = {1..n};

// Persuade AMM to not track ownership of the unnamed temporary returned by
// chpl__autoCopy(). 
pragma "return value is not owned"
proc autoCopy(x) { return chpl__autoCopy(x); }

resetCommDiagnostics();
startCommDiagnostics();
on Locales[numLocales-1] {
  autoCopy(D);
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
  autoCopy(A);
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
