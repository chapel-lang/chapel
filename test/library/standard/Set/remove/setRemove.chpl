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

  assert(s1.size == s2.size && s1.size == 0);

  for i in 1..testIters {
    var x = i:eltType;
    s1.add(x);
  }

  s2 = s1;

  assert(s1.size == s2.size && s1.size != 0);
  
  for i in 1..(testIters * 2) {
    var x = i:eltType;
    s2.add(x);
  }

  assert(s2.size > s1.size);
  assert(s2.size == (testIters * 2));

  for x in s1 {
    assert(s2.remove(x));
    assert(s1.contains(x));
  }

  assert(s1.size == s2.size);
  assert(s1.size == testIters);

  for x in s2 do
    assert(!s1.remove(x));

  // Conjure up some numbers that shouldn't be in either set.
  for i in -1..-testIters by -1 {
    var x = i:eltType;
    assert(!s1.remove(x) && !s2.remove(x));
  }
}

doTest(int);
doTest(testRecord);

