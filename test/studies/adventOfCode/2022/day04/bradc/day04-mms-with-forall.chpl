// topics:
// * IO: more readf()
// * ranges: contains() and intersection via slicing

use IO;

var Sections = readSections();

var subsumes, overlaps=0;

forall (r1, r2) in Sections with (+ reduce subsumes, + reduce overlaps) {
  subsumes += r1.contains(r2) || r2.contains(r1);
  overlaps += (r1[r2]).size > 0;
}
writeln("#subsumes = ", subsumes);
writeln("#overlaps = ", overlaps);


iter readSections() {
  var lo1, lo2, hi1, hi2:int;
  while readf("%i-%i,%i-%i", lo1, hi1, lo2, hi2) {
    yield (lo1..hi1, lo2..hi2);
  }
}
