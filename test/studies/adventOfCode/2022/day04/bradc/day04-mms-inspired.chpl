// topics:
// * IO: more readf()
// * ranges: contains() and intersection via slicing

use IO;

var Sections = readSections();
writeln(+ reduce fullyContains(Sections));
writeln(+ reduce overlaps(Sections));

iter readSections() {
  var lo1, lo2, hi1, hi2:int;
  while readf("%i-%i,%i-%i", lo1, hi1, lo2, hi2) {
    yield (lo1..hi1, lo2..hi2);
  }
}

proc fullyContains((r1, r2)) {
  return r1.contains(r2) || r2.contains(r1);
}

proc overlaps((r1, r2)) {
  const intersection = r1[r2];
  return intersection.size > 0;
}
