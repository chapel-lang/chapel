// overloads
// IO: matchLiteral, read(type t)
// - ??? should we use stdin everywhere for consistency?  use a named channel?
//

use IO;

var Sections = readSections();
writeln(+ reduce fullyContains(Sections));
writeln(+ reduce overlaps(Sections));

proc fullyContains((r1, r2)) {
  return r1.contains(r2) || r2.contains(r1);
}

proc overlaps((r1, r2)) {
  const intersection = r1[r2];
  return intersection.size >= 1;
}

iter readSections() {
  var lo:int;
  while read(lo) {
    const r1 = readSectionPair(lo);
    stdin.matchLiteral(",");
    const r2 = readSectionPair();
//    writeln((r1, r2));
    yield (r1, r2);
  }
}


proc readSectionPair(lo: int) {
  stdin.matchLiteral("-");
  const hi = read(int);
//  writeln("Got ", lo..hi);
  return lo..hi;
}

proc readSectionPair() {
  const lo = read(int);
  return readSectionPair(lo);
}
