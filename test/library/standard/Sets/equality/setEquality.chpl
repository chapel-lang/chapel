use Sets;

record testRecord {
  var dummy: int = 0;
  proc deinit() { writeln("Destroying testRecord!"); }
}

class testClass {
  var dummy: int = 0;
  proc deinit() { writeln("Destroying testClass!"); }
}

proc ==(const ref a: testRecord, const ref b: testRecord) {
  return a.dummy == b.dummy;
}

proc ==(const ref a: testClass, const ref b: testClass) {
  return a.dummy == b.dummy;
}

config const testIters = 8;

var set1: set(int);
var set2: set(testRecord);
var set3: set(testClass);

writeln(set1);
writeln(set2);
writeln(set3);

proc addItems(ref set1, ref set2, ref set3, bound=testIters) {
  for i in 1..bound {
    set1.add(i);
    var tr = new testRecord();
    tr.dummy = i;
    set2.add(tr);
    var tc = new testClass();
    tc.dummy = i;
    set3.add(tc);
  }
}

addItems(set1, set2, set3);

writeln(set1 == set1);
writeln(set2 == set2);
writeln(set3 == set3);

var set4 = set1;
var set5 = set2;
var set6 = set3;

writeln(set1 == set4);
writeln(set2 == set5);
writeln(set3 == set6);

addItems(set4, set5, set6, 32);

// Should all be true...
writeln(set1 != set4);
writeln(set2 != set5);
writeln(set3 != set6);

set1.clear();
set2.clear();
set3.clear();

// Should all be false...
writeln(set1 == set4);
writeln(set2 == set5);
writeln(set3 == set6);

set4.clear();
set5.clear();
set6.clear();

// Should all be true...
writeln(set1 == set4);
writeln(set2 == set5);
writeln(set3 == set6);

