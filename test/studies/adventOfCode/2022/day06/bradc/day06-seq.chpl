// Concepts:
// configs (!!!)
// named ranges
// # on ranges
// string slicing (take two?)

use IO, Set;

config const markerLen = 4;

var buffer: bytes;
while readLine(buffer) {  // use a while loop to try multiple tests in one run
  const inds = 0..<(buffer.size-markerLen);
  for i in inds {
    var s: set(uint(8));

    for ch in buffer[i..#markerLen] do
      s.add(ch);

    if s.size == markerLen {
      writeln(i+markerLen);
      break;
    }
  }
}
