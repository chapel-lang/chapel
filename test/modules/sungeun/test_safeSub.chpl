proc testSafeSub(a, b, expected) {
  const safe = safeSub(a,b);
  const res = if safe==true then "safe" else "not-safe";
  write(a, " - ", b, ": ", res);
  if safe!=expected then
    writeln(" *** INCORRECT ***");
  else
    writeln("");
}

testSafeSub(max(int), min(int), false);
testSafeSub(min(int), max(int), false);

testSafeSub(max(int), 0, true);
testSafeSub(0, max(int), true);

testSafeSub(max(int), 1, true);
testSafeSub(1, max(int), true);

testSafeSub(max(int), -1, false);
testSafeSub(-1, max(int), true);

testSafeSub(max(int)-1, -1, true);
testSafeSub(-1, max(int)-1, true);

testSafeSub(min(int), 0, true);
testSafeSub(0, min(int), false);

testSafeSub(min(int), 1, false);
testSafeSub(1, min(int), false);

testSafeSub(min(int), -1, true);
testSafeSub(-1, min(int), true);

testSafeSub(min(int)+1, 1, true);
testSafeSub(1, min(int)+1, false);

testSafeSub(max(uint), min(uint), true);
testSafeSub(min(uint), max(uint), false);

testSafeSub(min(uint), 0:uint, true);
testSafeSub(0:uint, min(uint), true);

testSafeSub(max(uint), 1:uint, true);
testSafeSub(1:uint, max(uint), false);

testSafeSub(min(uint), 1:uint, false);
testSafeSub(1:uint, min(uint), true);

