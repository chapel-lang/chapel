use BlockDist, CommDiagnostics;

config const n = 11;

const D = {1..n, 1..n} dmapped Block({1..n, 1..n});
var A: [D] real;

proc testit(X) {
  for loc in Locales do
    on loc {
      resetCommDiagnosticsHere();
      writeln("From locale ", loc.id);
      for loc in Locales {
        write("  locale ", loc.id, " owns:");
        
        
        if (D.hasSingleLocalSubdomain(loc)) {
          startCommDiagnosticsHere();
          const subD = D.localSubdomain(loc);
          stopCommDiagnosticsHere();
          writeln(subD);
        } else {
          var first = true;
          startCommDiagnosticsHere();
          for sd in D.localSubdomains(loc) {
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

writeln("Testing domain D: ");
testit(D);
writeln();

writeln("Testing array A: ");
testit(A);
