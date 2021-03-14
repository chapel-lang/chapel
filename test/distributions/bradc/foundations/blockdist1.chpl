type mylocale = int;
type indexType = int;

param rank: int = 1;

config const numLocales: int = 4;
config const n: int = 10;

var rankD: domain(1) = {1..rank};
var localeD: domain(1) = {1..numLocales};

var locales: [localeD] mylocale = [i in localeD] i;

var globLo: [rankD] indexType = 1;
var globHi: [rankD] indexType = n;

var mylo: [rankD] indexType;
var myhi: [rankD] indexType;

proc main() {

for loc in localeD {
  var pos: [rankD] int = loc;  // only works for 1D
  for d in 1..rank {
    var firstLoc = 1;            // localeD.lo(d)
    var numLocs = numLocales;       // localeD.extent(d)
    var myPos = pos(d) - firstLoc;
    var numElems = globHi(d) - globLo(d) + 1;

    if (numElems < numLocs) {
      writeln("ERROR: fewer elements than locales");
    }

    if (myPos == 0) {
      mylo(d) = min(indexType);
    } else {
      mylo(d) = (numElems * myPos) / numLocs;
      mylo(d) += globLo(d);
    }
    if (myPos == numLocs - 1) {
      myhi(d) = max(indexType);
    } else {
      myhi(d) = (numElems * (myPos + 1)) / numLocs;
      myhi(d) -= 1;
      myhi(d) += globLo(d);
    }
  }
  write("locale ", loc, " owns: ");
  for d in 1..rank {
    write(mylo(d), "..", myhi(d));
    if (d != rank) {
      write(", ");
    }
  }
  writeln("]");
}


proc min(type t): t {
  return -999;
}


proc max(type t): t {
  return 999;
}

}
