use IO, Set;

config const markerLen = 4;

var buffer: bytes;
while readLine(buffer) {  // use a while loop to try multiple tests in one run
  const inds = 0..<buffer.size-markerLen;
  var (_, loc) = maxloc reduce zip([i in inds] isMarker(buffer[i..#markerLen]),
                                   inds+markerLen);
  writeln(loc);
}

proc isMarker(window) {
  return uniqueChars(window) == markerLen;
}

proc uniqueChars(window) {
  var s: set(uint(8));

  for ch in window do
    s.add(ch);

  return s.size;
}
