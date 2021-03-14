proc testSafeAdd(a, b, expected) {
  const safe = safeAdd(a,b);
  const res = if safe==true then "safe" else "not-safe";
  write(a, " + ", b, ": ", res);
  if safe!=expected then
    writeln(" *** INCORRECT ***");
  else
    writeln("");
}

testSafeAdd(max(int), min(int), true);
testSafeAdd(min(int), max(int), true);

testSafeAdd(max(int), 0, true);
testSafeAdd(0, max(int), true);

testSafeAdd(max(int), 1, false);
testSafeAdd(1, max(int), false);

testSafeAdd(max(int), -1, true);
testSafeAdd(-1, max(int), true);

testSafeAdd(max(int)-1, 1, true);
testSafeAdd(1, max(int)-1, true);

testSafeAdd(min(int), 0, true);
testSafeAdd(0, min(int), true);

testSafeAdd(min(int), 1, true);
testSafeAdd(1, min(int), true);

testSafeAdd(min(int), -1, false);
testSafeAdd(-1, min(int), false);

testSafeAdd(min(int)+1, -1, true);
testSafeAdd(-1, min(int)+1, true);

testSafeAdd(min(uint), max(uint), true);
testSafeAdd(max(uint), min(uint), true);

testSafeAdd(min(uint)+1, max(uint), false);
testSafeAdd(max(uint), min(uint)+1, false);

