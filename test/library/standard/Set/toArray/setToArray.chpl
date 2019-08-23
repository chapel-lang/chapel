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

  assert(s1.size == 0 && s1.isEmpty());

  for i in 1..testIters {
    var x = i:eltType;
    s1.add(x);
  }

  assert(s1.size == testIters);

  var arr = s1.toArray();

  assert(s1.size == arr.size);

  for x in arr do
    assert(s1.contains(x));
}

doTest(int);
doTest(testRecord);

