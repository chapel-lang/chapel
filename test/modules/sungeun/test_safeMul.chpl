private use ChapelUtil;
proc testSafeMul(a, b, expected) {
  const safe = safeMul(a,b);
  const res = if safe==true then "safe" else "not-safe";
  write(a, " * ", b, ": ", res);
  if safe!=expected then
    writeln(" *** INCORRECT ***");
  else
    writeln("");
}

testSafeMul(max(int), min(int), false);
testSafeMul(min(int), max(int), false);

testSafeMul(max(int), 1, true);
testSafeMul(1, max(int), true);

testSafeMul(max(int), -1, true);
testSafeMul(-1, max(int), true);

testSafeMul(min(int), 0, true);
testSafeMul(0, min(int), true);

testSafeMul(min(int), 1, true);
testSafeMul(1, min(int), true);

testSafeMul(min(int), -1, false);
testSafeMul(-1, min(int), false);


testSafeMul(min(uint), max(uint), true);
testSafeMul(max(uint), min(uint), true);



