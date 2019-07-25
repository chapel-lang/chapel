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

var set4 = set1;
var set5 = set2;
var set6 = set3;

writeln(set1);
writeln(set2);
writeln(set3);
writeln(set4);
writeln(set5);
writeln(set6);

// Attempting to discard an element not in the list should be a NOP.
for 1..4 do
  for i in 1..testIters {
    set1.discard(i);
    var tr = new testRecord();
    tr.dummy = i;
    set2.discard(tr);
    var tc = new testClass();
    tc.dummy = i;
    set3.discard(tc);
  }

// Verify that the copied lists still have their elements.
writeln(set1);
writeln(set2);
writeln(set3);
writeln(set4);
writeln(set5);
writeln(set6);


