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
  }

  assert(s1.size == testIters);
  assert(s2.size == 0);

  for i in 1..(testIters * 2) {
    var x = i:eltType;
    s2.add(x);
  }

  assert(s1.size == testIters);
  assert(s2.size == testIters * 2);

  s3 = s1;
  s1 = s2;

  assert(s3.size == testIters);
  assert(s1.size == (testIters * 2));
  assert(s2.size == (testIters * 2));

  s2 = s3;

  for i in 1..testIters {
    var x = i:eltType;
    assert(s1.contains(x) && s2.contains(x) && s3.contains(x));
  }
}

doTest(int);
doTest(testRecord);

