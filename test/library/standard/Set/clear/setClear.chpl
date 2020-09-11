use Set;

config const testIters = 8;

record testRecord {
  var dummy: int = 0;
  proc init(dummy: int=0) { this.dummy = dummy; }
}

proc _cast(type t: testRecord, x: int) {
  return new testRecord(x);
}

proc doTest(type eltType) {
  var s1: set(eltType);
  var s2: set(eltType);
  var s3: set(eltType);

  for i in 1..testIters {
    var x = i:eltType;
    s1.add(x);
  }

  for i in 1..(testIters * 2) {
    var x = i:eltType;
    s2.add(x);
  }

  // Clearing a list that was on the LHS should not clear the RHS list.
  s3 = s1;
  s1.clear();

  assert(s1.size == 0);
  assert(s2.size == (testIters * 2));
  assert(s3.size == testIters);

  // Likewise for adding elements to a cleared list.
  for i in 1..testIters {
    var x = i:eltType;
    s1.add(x);
  }

  assert(s1.size == testIters);
  assert(s2.size == (testIters * 2));
  assert(s3.size == testIters);

  s2 = s3;

  for x in s2 do
    assert(s1.contains(x) && s3.contains(x));

  s1.clear();

  for i in 1..testIters {
    var x = i:eltType;
    assert(!s1.contains(x) && s2.contains(x) && s3.contains(x));
  }

  s1.clear();
  s2.clear();
  s3.clear();

  assert(s1.size == 0);
  assert(s2.size == 0);
  assert(s3.size == 0);
}

doTest(int);
doTest(testRecord);

