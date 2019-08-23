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
  var s: set(eltType);

  assert(s.size == 0);

  for i in 1..testIters {
    var x = i:eltType;
    s.add(x);
  }

  assert(s.size == testIters);

  for i in 1..testIters {
    var x = i:eltType;
    assert(s.contains(x));
  }

  for i in 1..testIters {
    var x = i:eltType;
    s.add(x);
  }

  assert(s.size == testIters);

  for i in 1..(testIters * 2) {
    var x = i:eltType;
    s.add(x);
  }

  assert(s.size == (testIters * 2));

  for i in 1..(testIters * 2) {
    var x = i:eltType;
    assert(s.contains(x));
  }
}

doTest(int);
doTest(testRecord);

