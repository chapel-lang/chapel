use SortedSet;
use OsetTest;


config const testIters = 8;

proc doTest(type eltType) {
  var s1 = new sortedSet(eltType, false, new DefaultComparator());
  var s2 = new sortedSet(eltType, false, new DefaultComparator());
  var s3 = new sortedSet(eltType, false, new DefaultComparator());

  assert(s1 == s2 && s2 == s3);

  for i in 1..testIters {
    var x = i:eltType;
    s1.add(x);
  }

  assert(s1 != s2);
  assert(s2 == s3);

  for i in 1..(testIters * 2) {
    var x = i:eltType;
    s2.add(x);
  }

  assert(s1 != s2 && s2 != s3 && s3.size == 0);

  s3 = s1;

  assert(s3 == s1);
  assert(s1 != s2);

  s1 = s2;

  assert(s3 != s1);
  assert(s1 == s2);

  s2 = s3;

  assert(s1 != s2);
  assert(s2 == s3);

  for i in 1..testIters {
    var x = i:eltType;
    assert(s1.contains(x) && s2.contains(x) && s3.contains(x));
  }
}

doTest(int);
doTest(testRecord);
