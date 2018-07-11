use BlockDist, CommDiagnostics;

config const n = 100,
             printArray = true;

class C {
  const D = {1..n} dmapped Block({1..n});
  var A: [D] real;
}

var myC = new owned C();

for loc in Locales do
  on loc {
    resetCommDiagnosticsHere();
    startCommDiagnosticsHere();
    for i in myC.D.localSubdomain() do
      myC.A.localAccess(i) = i;
    stopCommDiagnosticsHere();
    writeln(here.id, ": ", getCommDiagnosticsHere());
  }

if printArray then
  writeln("myC.A is: ", myC.A);
