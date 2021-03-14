use Set;

config const testIters = 8;

record testRecord {
  var dummy: int = 0;
  proc init(dummy: int=0) { this.dummy = dummy; }
}

operator :(x: int, type t: testRecord) {
  return new testRecord(x);
}

proc doTest(type eltType) {
  var s1: set(eltType);
  var s2: set(eltType);
  var s3: set(eltType);

  assert(s1.size == 0);
  assert(s2.size == 0);
  assert(s3.size == 0);

  for i in 1..testIters {
    var x = i:eltType;
    s1.add(x);
    assert(s1.contains(x));
  }

  assert(s1.size == testIters);
  assert(s2.size == 0);

  for i in 1..(testIters * 2) {
    var x = i:eltType;
    s2.add(x);
    assert(s2.contains(x));
  }

  for x in s1 do
    assert(s2.contains(x));

  var count = 0;

  for x in s2 do
    if s1.contains(x) then
      count += 1;
  
  assert(count == s1.size);

  for i in -1..-8 by -1 {
    var x = i:eltType;
    assert(!s1.contains(x) && !s2.contains(x));
  }
}

doTest(int);
doTest(testRecord);

