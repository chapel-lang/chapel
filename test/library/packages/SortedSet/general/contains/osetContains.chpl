use OrderedSet;
use OsetTest;



config const testIters = 8;

proc doTest(type eltType) {
  var s1 = new orderedSet(eltType, false, defaultComparator);
  var s2 = new orderedSet(eltType, false, defaultComparator);
  var s3 = new orderedSet(eltType, false, defaultComparator);

  assert(s1.size == s2.size && s2.size == s3.size && s3.size == 0);

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

