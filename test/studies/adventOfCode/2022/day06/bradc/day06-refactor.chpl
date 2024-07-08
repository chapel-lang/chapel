// Concepts:
// configs (!!!)
// named ranges
// # on ranges
// string slicing (take two?)

// TODO: maybe remove while loop for blog

use IO, Set;

config const matchSize = 4;

var line: bytes;
while readLine(line) {
  const inds = 0..<(line.size-matchSize);

  for i in inds {
    if numUnique(line[i..#matchSize]) == matchSize {
      writeln(i+matchSize);
      break;
    }
  }
}

proc numUnique(subline) {
  var s: set(uint(8));

  for ch in subline do
    s.add(ch);

  return s.size;
}
