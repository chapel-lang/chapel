use OrderedSet;
use OsetTest;


config const testIters = 8;

proc doTest(type eltType) {
  var s1 = new orderedSet(eltType, false, defaultComparator);
  var s2 = new orderedSet(eltType, false, defaultComparator);
  var s3 = new orderedSet(eltType, false, defaultComparator);
  var s4 = new orderedSet(eltType, false, defaultComparator);

  assert(s1.size == s2.size && s2.size == s3.size && s3.size == 0);

  for i in 1..testIters {
    var x = i:eltType;
    s1.add(x);
  }

  for i in 1..(testIters * 2) {
    var x = i:eltType;
    s2.add(x);
  }

  assert(s1.size == testIters);
  assert(s2.size == (testIters * 2));

  s3 = s1 & s2;
  s4 = s2 & s1;

  var s2copy = s2;
  
  assert(s3 == s1);
  assert(s4 == s3);

  for x in s3 do
    assert(s1.contains(x) && s2.contains(x));

  for x in s1 do
    s2.remove(x);

  s4 = s2 & s3;

  assert(s4.size == 0 && s4.isEmpty());

  var s5 = s3 & s2;
  assert(s5 == s4);

  s1 &= s2copy; // replicate s3 = s1 & s2 above
  assert(s1 == s3);
}

doTest(int);
doTest(testRecord);
