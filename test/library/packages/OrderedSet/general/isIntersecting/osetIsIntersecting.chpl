use OrderedSet;
use OsetTest;

config param impl: setImpl;
config const testIters = 8;

proc doTest(type eltType) {
  var s1 = new orderedSet(eltType, false, defaultComparator, impl);
  var s2 = new orderedSet(eltType, false, defaultComparator, impl);

  assert(!s1.isIntersecting(s2));
  assert(!s2.isIntersecting(s1));

  for i in 1..testIters {
    var x = i:eltType;
    s1.add(x);
  }

  assert(!s1.isIntersecting(s2));
  assert(!s2.isIntersecting(s1));

  for i in 1..(testIters * 2) {
    var x = i:eltType;
    s2.add(x);
  }

  assert(s1.isIntersecting(s2));
  assert(s2.isIntersecting(s1));

  for x in s1 do
    if s2.contains(x) then
      s2.remove(x);

  assert(!s1.isIntersecting(s2));
  assert(!s2.isIntersecting(s1));
}

doTest(int);
doTest(testRecord);
