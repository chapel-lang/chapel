type locale: integer;
type indexType: integer;

parameter rank: integer = 1;

config const numLocales: integer = 4;
config const n: integer = 10;

var rankD: domain(1) = 1..rank;
var localeD: domain(1) = 1..numLocales;

var locales: [i in localeD] locale = i;

var globLo: [rankD] indexType = 1;
var globHi: [rankD] indexType = n;

var mylo: [rankD] indexType;
var myhi: [rankD] indexType;

for loc in localeD {
  var pos: [rankD] integer = loc;  -- only works for 1D
  for d in 1..rank {
    var firstLoc = 1;            -- localeD.lo(d)
    var numLocs = numLocales;       -- localeD.extent(d)
    var myPos = pos(d) - firstLoc;
    var numElems = globHi(d) - globLo(d) + 1;

    if (numElems < numLocs) {
--    halt("ERROR: fewer elements than locales");
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


function min(type t): integer {
  return -999;
}


function max(type t): integer {
  return 999;
}
