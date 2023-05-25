use Set;
use List;

config const testIters = 8;

record testRecord {
  var dummy: int = 0;
};

class testClass {
  var dummy: int = 0;
}

var array1: [1..testIters] int;
var array2: [1..testIters] testRecord;

var list1: list(int);
var list2: list(testRecord);

// Construct our test lists.
for i in 1..testIters {
  array1[i] = i;
  array2[i].dummy = i;
  list1.pushBack(i);
  var tr = new testRecord();
  tr.dummy = i;
  list2.pushBack(tr);
}

var set1 = new set(int, array1);
var set2 = new set(testRecord, array2);
var set3 = new set(int, list1);
var set4 = new set(testRecord, array2);

proc checkContains(ref s, ref it) {
  for x in it do
    assert(s.contains(x));
}

checkContains(set1, array1);
checkContains(set2, array2);
checkContains(set3, list1);
checkContains(set4, list2);
checkContains(set1, set3);
checkContains(set2, set4);
