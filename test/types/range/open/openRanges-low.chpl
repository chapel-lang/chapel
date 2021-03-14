testRanges(int);
testRanges(int(8));
testRanges(uint(8));
testRanges(uint(64));

proc testRanges(type t) {
  const lo = 3: t;
  const hi = 7: t;

  testRange(lo<..hi);
  testRange(lo<..#4);
}

proc testRange(r) {
  writeln(r, " (idxType = ", r.idxType:string, "):");
  for i in r do
    writeln(i);
  writeln();
}
