use OrderedSet;
use OsetTest;


config const testIters = 8;

proc doTest(type eltType) {
  var s1 = new orderedSet(eltType, false, defaultComparator);

  var count = 0;

  for x in s1 do
    count += 1;

  assert(count == 0);

  var expected = 0;

  for i in 1..testIters {
    expected += i;
    var x = i:eltType;
    s1.add(x);
  }

  var total = 0;

  for x in s1 do
    if x.type == testRecord then
      total += x.dummy;
    else
      total += x;

  assert(total == expected);

}

doTest(int);
doTest(testRecord);
