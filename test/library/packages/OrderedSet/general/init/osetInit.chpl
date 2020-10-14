use OrderedSet;
use OsetTest;



var s1 = new orderedSet(int, false, defaultComparator);
var s2 = new orderedSet(testRecord, false, defaultComparator);
var s3 = new orderedSet(borrowed testClass, false, defaultComparator);
var s4 = new orderedSet(int, 1..10, false, defaultComparator);

assert(s1.size == 0);
assert(s2.size == 0);
assert(s3.size == 0);
assert(s4.size == 10);
