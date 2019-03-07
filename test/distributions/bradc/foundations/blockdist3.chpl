type mylocale = int;
type indexType = int;

param rank: int = 1;

config const myNumLocales: int = 4;
config const n: int = 10;

var rankD: domain(1) = {1..rank};
var localeD: domain(1) = {1..myNumLocales};

//var locales: [localeD] locale = [i in localeD] i;
var locales: [localeD] mylocale;
for i in localeD {
  locales(i) = i;
}

var globLo: [rankD] indexType = 1;
var globHi: [rankD] indexType = n;

var mylo: [rankD] indexType;
var myhi: [rankD] indexType;

for loc in localeD {
  var pos: [rankD] int = loc;  // only works for 1D
  for d in 1..rank {
    var firstLoc = 1;            // localeD.lo(d)
    var numLocs = myNumLocales;       // localeD.extent(d)
    var myPos = pos(d) - firstLoc;
    var numElems = globHi(d) - globLo(d) + 1;

    if (numElems < numLocs) {
//    halt("ERROR: fewer elements than locales");
      writeln("ERROR: fewer elements than locales");
    }

    if (myPos == 0) {
      mylo(d) = mymin(indexType);
    } else {
      mylo(d) = (numElems * myPos) / numLocs;
      mylo(d) += globLo(d);
    }
    if (myPos == numLocs - 1) {
      myhi(d) = mymax(indexType);
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


proc mymin(type t): int {
  return -999;
}


proc mymax(type t): int {
  return 999;
}
