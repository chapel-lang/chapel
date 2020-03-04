use ReplicatedDist, CommDiagnostics;

config const n = 11;


// test a standard block distribution
const D = {1..n, 1..n} dmapped Replicated();
var A: [D] real;

testit(D, A);

proc postfix!(A:[]) { var B = for a in A do a!; return B; } //#15080

// test one with a shuffled locale map
if (numLocales == 4) {
  var targetLocs: [1..2, 1..2] locale?;
  targetLocs[1,1] = Locales[1];
  targetLocs[1,2] = Locales[3];
  targetLocs[2,1] = Locales[0];
  targetLocs[2,2] = Locales[2];

  const D = {1..n, 1..n} dmapped Replicated(targetLocales = targetLocs!);
  var A: [D] real;

  testit(D, A);

  // test a slice of the above (in which not all target locales own something)
  const D2 = D[2..3, 1..n];
  var A2: [D2] real;
  testit(D2, A2);
}

// test one that targets a subset of locales
if (numLocales == 4) {
  var targetLocs: [1..1, 1..2] locale?;
  targetLocs[1,1] = Locales[3];
  targetLocs[1,2] = Locales[1];

  const D = {1..n, 1..n} dmapped Replicated(targetLocales = targetLocs!);
  var A: [D] real;

  testit(D, A);
}

proc testit(D, A) {
  writeln("Testing domain D: ", D);
  testit(D);
  writeln();

  writeln("Testing array A (declared over D):");
  testit(A);
}

proc testit(X) {
  for loc in Locales do
    on loc {
      resetCommDiagnosticsHere();
      writeln("From locale ", loc.id);
      for loc in Locales {
        write("  locale ", loc.id, " owns: ");

        /* test localSubdomain() interface if it is expected to work */
        if (X.hasSingleLocalSubdomain()) {
          startCommDiagnosticsHere();
          const subD = X.localSubdomain(loc);
          stopCommDiagnosticsHere();
          writeln(subD);
        }

        write("  locale ", loc.id, " owns: ");

        /* test localSubdomains() iterator in any case */
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

      writeln(getCommDiagnosticsHere());
      writeln();
    }
}

