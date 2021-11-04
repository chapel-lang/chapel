use OrderedSet;
use OsetTest;



config const testIters = 8;

proc doTest(type eltType) {
  var s1 = new orderedSet(eltType, false, defaultComparator);
  var s2 = new orderedSet(eltType, false, defaultComparator);
  var s3 = new orderedSet(eltType, false, defaultComparator);

  for i in 1..testIters {
    var x = i:eltType;
    s1.add(x);
  }

  for i in 1..(testIters * 2) {
    var x = i:eltType;
    s2.add(x);
  }

  // Clearing a list that was on the LHS should not clear the RHS list.
  s3 = s1;
  s1.clear();

  assert(s1.size == 0);
  assert(s2.size == (testIters * 2));
  assert(s3.size == testIters);

  // Likewise for adding elements to a cleared list.
  for i in 1..testIters {
    var x = i:eltType;
    s1.add(x);
  }

  assert(s1.size == testIters);
  assert(s2.size == (testIters * 2));
  assert(s3.size == testIters);

  s2 = s3;

  for x in s2 do
    assert(s1.contains(x) && s3.contains(x));

  s1.clear();

  for i in 1..testIters {
    var x = i:eltType;
    assert(!s1.contains(x) && s2.contains(x) && s3.contains(x));
  }

  s1.clear();
  s2.clear();
  s3.clear();

  assert(s1.size == s2.size && s2.size == s3.size && s3.size == 0);
}

doTest(int);
doTest(testRecord);

