// maxloc
// shifting a range
// [x in y] style forall

use IO, Set;

config const matchSize = 4;

var line: bytes;
while readLine(line) {
  const inds = 0..<(line.size-matchSize);
  var (_, loc) = maxloc reduce zip([i in inds] matches(line[i..#matchSize]),
                                   inds+matchSize);
  writeln(loc);
}

proc matches(subline) {
  return numUnique(subline) == matchSize;
}

proc numUnique(subline) {
  var s: set(uint(8));

  for ch in subline do
    s.add(ch);

  return s.size;
}
