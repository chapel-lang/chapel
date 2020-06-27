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
  var s4: set(eltType);

  assert(s1.size == s2.size == s3.size == 0);

  for i in 1..testIters {
    var x = i:eltType;
    s1.add(x);
  }

  for i in 1..(testIters * 2) {
    var x = i:eltType;
    s2.add(x);
  }

  s3 = s2;

  for x in s1 do
    assert(s3.remove(x));

  for x in s3 do
    assert(!s1.contains(x) && s2.contains(x));

  s4 = s1 | s3;
  var s5 = s3 | s1;

  assert(s4.size == s2.size);
  assert(s5 == s4);

  s1 |= s3;
  assert(s1 == s4);

  for x in s4 do
    assert(s2.contains(x));
}

doTest(int);
doTest(testRecord);
