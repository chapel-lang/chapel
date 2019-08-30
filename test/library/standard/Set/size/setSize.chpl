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

  assert(s1.size == 0);

  for i in 1..testIters {
    var x = i:eltType;
    s1.add(x);
    assert(s1.size == i);
  }

  assert(s1.size == testIters);

  var s2 = s1;

  assert(s1.size == s2.size && s1.size == testIters);

  var oldSize = s2.size;

  for i in 1..(testIters * 2) {
    var x = i:eltType;
    s2.add(x);
    assert(s1.size == oldSize);
  }

  assert(s2.size == (2 * oldSize));

  oldSize = s2.size;

  for x in s1 {
    s2.remove(x);
    assert(s1.size == testIters);
    assert(s2.size < oldSize);
  }


  s1.clear();
  assert(s1.size == 0);
  s2.clear();
  assert(s2.size == 0);
}

doTest(int);
doTest(testRecord);

