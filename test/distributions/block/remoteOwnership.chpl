use BlockDist, CommDiagnostics;

config const n = 11;

const D = {1..n, 1..n} dmapped Block({1..n, 1..n});
var A: [D] real;

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

/*
for loc in Locales do
  on loc {
    writeln("From locale ", loc.id);
    for loc in Locales {
      writeln("locale ", loc.id, " owns:");

      if (A.hasSingleLocalSubdomain(loc)) {
        writeln(A.localSubdomain(loc));
      } else {
        var first = true;
        for sd in A.localSubdomains(loc) {
          if first {
            first = false;
          } else {
            writeln(", ");
          }
        }
      }
    }
  }
*/