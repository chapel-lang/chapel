use Sets;
use Lists;

config const testIters = 8;

record testRecord {
  var dummy: int = 0;
};

class testClass {
  var dummy: int = 0;
}

var array1: [1..testIters] int;
var array2: [1..testIters] testRecord;
var array3: [1..testIters] testClass;

var list1: list(int);
var list2: list(testRecord);
var list3: list(testClass);

// Construct our test lists.
for i in 1..testIters {
  array1[i] = i;
  array2[i].dummy = i;
  array3[i] = new testClass();
  array3[i].dummy = i;
  list1.append(i);
  var tr = new testRecord();
  tr.dummy = i;
  list2.append(tr);
  var tc = new testClass();
  tc.dummy = i;
  list3.append(tc);
}

var set1 = new set(int, false, array1);
var set2 = new set(testRecord, false, array2);
var set3 = new set(testClass, false, array3);
var set4 = new set(int, false, list1);
var set5 = new set(testRecord, false, array2);
var set6 = new set(testClass, false, array3);

writeln(set1);
writeln(set2);
writeln(set3);
writeln(set4);
writeln(set5);
writeln(set6);
