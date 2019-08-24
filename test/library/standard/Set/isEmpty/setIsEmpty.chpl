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

  assert(s1.isEmpty());
  assert(s2.isEmpty());
  assert(s3.isEmpty());

  for i in 1..testIters {
    var x = i:eltType;
    s1.add(x);
    assert(!s1.isEmpty());
  }

  assert(!s1.isEmpty());
  assert(s2.isEmpty());
  assert(s3.isEmpty());

  for i in 1..(testIters * 2) {
    var x = i:eltType;
    s2.add(x);
    assert(!s2.isEmpty());
  }

  assert(!s1.isEmpty());
  assert(!s2.isEmpty());
  assert(s3.isEmpty());
}

doTest(int);
doTest(testRecord);

