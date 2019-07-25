use Sets;

record testRecord {
  var dummy: int = 0;
  proc deinit() { writeln("Destroying testRecord!"); }
}

class testClass {
  var dummy: int = 0;
  proc deinit() { writeln("Destroying testClass!"); }
}

config const testIters = 8;

var set1: set(int);
var set2: set(testRecord);
var set3: set(testClass);

writeln(set1);
writeln(set2);
writeln(set3);

proc addItems(ref set1, ref set2, ref set3) {
  for i in 1..testIters {
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

writeln(set1);
writeln(set2);
writeln(set3);

var set4: set(int);
var set5: set(testRecord);
var set6: set(testClass);

writeln(set4);
writeln(set5);
writeln(set6);

set4 = set1;
set5 = set2;
set6 = set3;

writeln(set1);
writeln(set2);
writeln(set3);
writeln(set4);
writeln(set5);
writeln(set6);

addItems(set1, set2, set3);
addItems(set4, set5, set6);

writeln(set1);
writeln(set2);
writeln(set3);
writeln(set4);
writeln(set5);
writeln(set6);

