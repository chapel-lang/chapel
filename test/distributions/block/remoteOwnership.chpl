use BlockDist, CommDiagnostics;

config const n = 11;

const D = {1..n, 1..n} dmapped Block({1..n, 1..n});
var A: [D] real;

writeln("Testing domain D: ");
testit(D);
writeln();

/*
writeln("Testing array A: ");
testit(A);
*/

if (numLocales == 4) {
  var targetLocs: [1..2, 1..2] locale;
  targetLocs[1,1] = Locales[1];
  targetLocs[1,2] = Locales[3];
  targetLocs[2,1] = Locales[0];
  targetLocs[2,2] = Locales[2];

  const D = {1..n, 1..n} dmapped Block({1..n, 1..n}, targetLocales = targetLocs);
  var A: [D] real;

  writeln("Testing domain D: ");
  testit(D);
  writeln();

  /*
  writeln("Testing array A: ");
  testit(A);
  */
}

proc testit(X) {
  for loc in Locales do
    on loc {
      resetCommDiagnosticsHere();
      writeln("From locale ", loc.id);
      for loc in Locales {
        write("  locale ", loc.id, " owns:");
        
        
        if (X.hasSingleLocalSubdomain(loc)) {
          startCommDiagnosticsHere();
          const subD = X.localSubdomain(loc);
          stopCommDiagnosticsHere();
          writeln(subD);
        } else {
          var first = true;
          startCommDiagnosticsHere();
          for sd in X.localSubdomains(loc) {
            stopCommDiagnosticsHere();
            if first {
              first = false;
            } else {
              writeln(", ");
            }
            writeln(sd);
            startCommDiagnosticsHere();
          }
          stopCommDiagnosticsHere();
        }
      }
      writeln(getCommDiagnosticsHere());
    }
}

