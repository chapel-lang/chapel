use OrderedSet;
use OsetTest;


config const testIters = 8;

proc doTest(type eltType) {
  var s1 = new orderedSet(eltType, false, defaultComparator);

  for i in 1..testIters {
    var x = i:eltType;
    s1.add(x);
  }

  var s2 = s1;

  assert(s1.size == s2.size);

  for i in 1..testIters {
    var x = i:eltType;
    assert(s2.contains(x));
  }

  s1.clear();

  for i in 1..testIters {
    var x = i:eltType;
    assert(!s1.contains(x) && s2.contains(x));
  }
}

doTest(int);
doTest(testRecord);
