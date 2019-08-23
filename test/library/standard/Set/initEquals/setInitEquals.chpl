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

  for i in 1..testIters {
    var x = i:eltType;
    s1.add(x);
  }

  var s2 = s1;

  assert(s1.size == s2.size);

  for i in 1..testIters {
    var x = i:eltType;
    assert(s2.contains(x));
  }

  s1.clear();

  for i in 1..testIters {
    var x = i:eltType;
    assert(!s1.contains(x) && s2.contains(x));
  }
}

doTest(int);
doTest(testRecord);

