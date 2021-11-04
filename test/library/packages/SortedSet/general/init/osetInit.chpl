use SortedSet;
use OsetTest;



var s1 = new sortedSet(int, false, defaultComparator);
var s2 = new sortedSet(testRecord, false, defaultComparator);
var s3 = new sortedSet(borrowed testClass, false, defaultComparator);
var s4 = new sortedSet(int, 1..10, false, defaultComparator);

assert(s1.size == 0);
assert(s2.size == 0);
assert(s3.size == 0);
assert(s4.size == 10);
