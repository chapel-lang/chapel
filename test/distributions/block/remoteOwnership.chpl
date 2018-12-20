use BlockDist;

config const n = 11;

const D = {1..n, 1..n} dmapped Block({1..n, 1..n});
var A: [D] real;

for loc in Locales do
  on loc {
    writeln("From locale ", loc.id);
    for loc in Locales {
      write("  locale ", loc.id, " owns:");

      if (D.hasSingleLocalSubdomain(loc)) {
        writeln(D.localSubdomain(loc));
      } else {
        var first = true;
        for sd in D.localSubdomains(loc) {
          if first {
            first = false;
          } else {
            writeln(", ");
          }
        }
      }
    }
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