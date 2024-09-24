use SortedSet;
use OsetTest;


config const testIters = 8;

proc doTest(type eltType) {
  var s1 = new sortedSet(eltType, false, new DefaultComparator());

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
